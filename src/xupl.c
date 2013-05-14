#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include <limits.h>
#include <regex.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

static regex_t re_word;

#define REQUIRE(x) _state = (x)
#define ALLOW(x) _state |= (x)
#define DISABLE(x) (_state &= ~(x))
#define STR(x) (x=='"'?ST1:ST2)
#define IS(x) ((x) & _state)
#define NOT(x) (!((x) & _state))
#define IF(x) if IS(x)
#define ELIF(x) else IF(x)

int main (int argc, char **argv) {

	int atty = isatty(0);
	if (argc <= 1 && atty) return 1;

	const char* FILENAME = argv[1];

	if (regcomp(&re_word, "^[:_A-Za-z][:_A-Za-z0-9.-]*", REG_EXTENDED)) {
		err(1, "Could not compile regex\n");
		return 1;
	}

	FILE* in = NULL;
	int fd = -1;
	off_t filesize = 0;
	off_t buffsize = 32 * 1024;

	if (!atty) {
		in = stdin;
	} else {
		fd = open(FILENAME, O_RDONLY);
		if (fd == -1) {
			err(2, "open: %s", FILENAME);
			return 2;
		}
		struct stat fs;
		if (fstat(fd, &fs) == -1) {
			err(3, "stat: %s", FILENAME);
			return 3;
		}
		in = fdopen(fd, "r");
		buffsize = filesize = fs.st_size;
		//printf("filesize=%lld\n", filesize);
	}

	unsigned short bit = 0x0001;
	unsigned short INI = bit;
	unsigned short PRE = (bit <<= 1);
	unsigned short DOC = (bit <<= 1);
	unsigned short ELE = (bit <<= 1);
	unsigned short ATT = (bit <<= 1);
	unsigned short ST1 = (bit <<= 1);
	unsigned short ST2 = (bit <<= 1);
	unsigned short ST_ = ST1 | ST2;
	unsigned short ATN = (bit <<= 1);
	unsigned short ATV = (bit <<= 1);
	unsigned short WS_ = (bit <<= 1);

	unsigned short _state = INI;

	const int default_tksize = 12;
	int tksize = default_tksize + 1;

	unsigned char *tk = NULL;
	int tkndx = 0;

	int chars_read;
	char* buf = malloc(buffsize + 1);

	xmlDocPtr xdoc = xmlNewDoc((const unsigned char*) "1.1");
	xmlNodePtr xroot = NULL;
	xmlNodePtr xc = NULL;

	xmlChar* att = NULL;
	unsigned int att_is_string = 0;

	while ((chars_read = fread(buf, 1, buffsize, in)) > 0) {

		for (int i = 0; i < chars_read; i++) {
			const char c = buf[i];

			switch (c) {

				// These characters define tokens boundaries.
				case ' ':
				case '\n':
				case '\t':
				case '\f':
				case '\v':
				case '\r':
					IF(ST_) break;
					IF(INI|WS_) continue;
					ALLOW(WS_);

				case '{':
				case '}':
				case ',':
					IF(ST_) break;

					if (tk) {
						tk[tkndx] = 0;

						unsigned int tklen = tkndx + 1;
						unsigned char* t = tk;
						unsigned int st = 0;
						if ((st = ('"' == t[0] || t[0] == '\''))) {
							t += 1;
							tklen -= 1;
						}

						unsigned int process_element = 0;

						if (att) {
							if (IS(ATT) || ',' == c) { // Force the attribute
								if (xc) {
									if (att_is_string && st) {
										xmlNewProp(xc, (xmlChar*)"id", att);
										xmlNewProp(xc, (xmlChar*)"id", t);
									} else if (att_is_string) {
										xmlNewProp(xc, t, att);
									} else {
										xmlNewProp(xc, att, t);
									}
								}
							}
							free(att);
							att = NULL;
							att_is_string = 0;
						} ELIF(ATT) {
							if (!att) {
								att = malloc(tklen);
								att_is_string = st;
								memcpy(att, t, tklen);
							}
						} ELIF(DOC) {
							regex_t re_doc;
							if (regcomp(&re_doc, "^[?](xml|xupl)", REG_EXTENDED)) {
								err(4, "Could not compile regex\n");
								return 4;
							}
							regmatch_t pmatch[1];
							process_element = regnexec(&re_doc, (char*) tk, tkndx, 1, pmatch, 0);
							if (!process_element) {
								/*
								printf("matched %.*s from %lld to %lld\n",
								    (int) (pmatch[0].rm_eo - pmatch[0].rm_so),
								    &tk[pmatch[0].rm_so], pmatch[0].rm_so, pmatch[0].rm_eo);
								*/
							}
							REQUIRE(ELE);
						} ELIF(ELE) {
							REQUIRE(ATT);
							process_element = 1;
						} else {
							err(5, "UNK %s\n", tk);
						}

						if (process_element) {
							if (!xc) {
								xc = xroot = xmlNewNode(NULL, tk);
								xmlDocSetRootElement(xdoc, xroot);
							} else {
								if ('"' == tk[0] || tk[0] == '\'') {
									xmlAddChild(xc, xmlNewText(t));
								} else {
									xc = xmlNewChild(xc, NULL, tk, NULL );
								}
							}
						}

						free(tk);
						tk = NULL;
						if (tksize > default_tksize && tkndx < default_tksize) {
							tksize /= 2;
						}
						tkndx = 0;
					}

					// These tokens require a specific state for the token.
					switch (c) {
						case '}':
							if (xc) xc = xc->parent;
						case '{':
							if NOT(ATT|ELE|ST_) {
								printf("STATE ERROR: Expected ELE [%04X], is [%04X]\n",ELE,_state);
								continue;
							}
							REQUIRE(ELE);
							break;
							case ',':
							if NOT(ATT|ST_) {
								printf("STATE ERROR: Expected ATT [%04X], is [%04X]\n",ATT,_state);
								continue;
							}
							break;
							default: break;
					}

					continue;

				case '\'':
				case '"':
					IF(ST_) {
						DISABLE(STR(c));
						continue;
					} else {
						ALLOW(STR(c));
					}
					break;

				default:
					DISABLE(WS_);
					IF(INI) REQUIRE(PRE|DOC|ELE);
					break;
			}
			// Accumulate the tk.
			if (!tk || tkndx >= tksize) {
				// If the tk buffer is too small, double it.
				tk = realloc(tk, tksize *= 2);
			}
			tk[tkndx++] = c;
		}
	}

	if (att) free(att);
	if (tk) free(tk);
	free(buf);

	printf("\n");

	if (filesize > 0) {
		fclose(in);
	}

	xmlSaveFormatFileEnc("-", xdoc, (const char*) "UTF-8", 1);
	xmlCleanupParser();

	return 0;
}

/**
 *
 */
int xmlize (int argc, char **argv) {

	xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;
	unsigned char buff[256];
	int i, j;

	/*
	 * Creates a new document, a node and set it as a root node
	 */
	xmlDocPtr doc = xmlNewDoc((const unsigned char*) "1.1");
	xmlNodePtr root = xmlNewNode(NULL, (const unsigned char*) "root");
	xmlDocSetRootElement(doc, root);

	/*
	 * xmlNewChild() creates a new node, which is "attached" as child node
	 * of root node.
	 */
	xmlNewChild(root, NULL, (const unsigned char*) "node1",
	    (const unsigned char*) "content of node 1");
	/*
	 * The same as above, but the new child node doesn't have a content
	 */
	xmlNewChild(root, NULL, (const unsigned char*) "node2", NULL );

	/*
	 * xmlNewProp() creates attributes, which is "attached" to an node.
	 * It returns xmlAttrPtr, which isn't used here.
	 */
	node = xmlNewChild(root, NULL, (const unsigned char*) "node3",
	    (const unsigned char*) "this node has attributes");
	xmlNewProp(node, (const unsigned char*) "attribute",
	    (const unsigned char*) "yes");
	xmlNewProp(node, (const unsigned char*) "foo", (const unsigned char*) "bar");

	/*
	 * Here goes another way to create nodes. xmlNewNode() and xmlNewText
	 * creates a node and a text node separately. They are "attached"
	 * by xmlAddChild()
	 */
	node = xmlNewNode(NULL, (const unsigned char*) "node4");
	node1 =
	    xmlNewText(
	        (const unsigned char*) "other way to create content (which is also a node)");
	xmlAddChild(node, node1);
	xmlAddChild(root, node);

	/*
	 * A simple loop that "automates" nodes creation
	 */
	for (i = 5; i < 7; i++) {
		sprintf((char* ) buff, (const char* ) "node%d", i);
		node = xmlNewChild(root, NULL, buff, NULL );
		for (j = 1; j < 4; j++) {
			sprintf((char* ) buff, (const char* ) "node%d%d", i, j);
			node1 = xmlNewChild(node, NULL, buff, NULL );
			xmlNewProp(node1, (const unsigned char*) "odd",
			    (const unsigned char*) ((j % 2) ? "no" : "yes"));
		}
	}

	/*
	 * Dumping document to stdio or file
	 */
	xmlSaveFormatFileEnc(argc > 1 ? argv[1] : "-", doc, (const char*) "UTF-8", 1);

	/*
	 *Free the global variables that may
	 *have been allocated by the parser.
	 */
	xmlCleanupParser();

	/*
	 * this is to debug memory for regression tests
	 * /
	 xmlMemoryDump();
	 //*/
	return (0);
}
