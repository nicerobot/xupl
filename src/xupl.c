/*
Copyright 2013 Nice Robot Corporation

This file is part of Xupl.

Xupl is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Xupl is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Xupl.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <libxml2/libxml/tree.h>
#include <limits.h>
#include <regex.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include "xupla.h"

#define REQUIRE(x) _state = (x)
#define ALLOW(x) _state |= (x)
#define DISABLE(x) (_state &= ~(x))
#define STR(x) (x=='"'?DOUBLE_STRING:(x=='\''?SINGLE_STRING:0))
#define CMT(x) (x=='#'?LINE_COMMENT:(x=='*'?MULTI_COMMENT:0))
#define IS(x) ((x) & _state)
#define NOT(x) (!((x) & _state))
#define IF(x) if IS(x)
#define ELIF(x) else IF(x)

typedef struct xupl* (*xupv)(struct xupl*, ...);

typedef struct xupl_ {
	void (*done)(xmlDocPtr doc);
	FILE* in;
	xmlDocPtr doc;
	off_t buffsize;
	int status;
} xupl_;

void xupl_user_done(xmlDocPtr doc);

static regex_t re_word;

xupl *xupl_init_with_file_pointer_and_buffer(FILE* in, off_t buffsize) {
	static unsigned short _init = 0;
	if (!_init) {
		_init = 1;
		if (regcomp(&re_word, "^[:_a-zA-Z][:_a-zA-Z0-9.-]+", REG_EXTENDED | REG_ICASE)) {
			err(4, "Could not compile regex\n");
			return NULL;
		}
	}
	xupl_* xup = (xupl_*) malloc(sizeof (xupl_));
	xup->doc = NULL;
	xup->in = in;
	xup->buffsize = buffsize;
	xup->status = 0;
	xup->done = xupl_user_done;
	return (xupl*) xup;
}

xupl *xupl_init_with_file_descriptor_and_buffer(int fd, off_t buffsize) {
	struct stat fs;
	if (fstat(fd, &fs) == -1) {
		err(3, "stat: %d", fd);
		fs.st_size = 32 * 1024;
	}
	xupl *xup = xupl_init_with_file_pointer_and_buffer(fdopen(fd, "r"), fs.st_size);

	return xup;
}

xupl *xupl_init_with_file_descriptor(int fd) {
	return xupl_init_with_file_descriptor_and_buffer(fd, 32 * 1024);
}

xupl *xupl_init_with_file_name(char* fn) {
	return xupl_init_with_file_descriptor(open(fn, O_RDONLY));
}

xupl *xupl_init_with_file_pointer(FILE* in) {
	return xupl_init_with_file_descriptor(fileno(in));
}

xupl *xupl_init(int argc, char *argv[]) {
	int atty = isatty(0);
	if (argc <= 1 && atty) return NULL;

	xupl* xup = NULL;
	if (!atty) {
		xup = xupl_init_with_file_pointer_and_buffer(stdin, 32 * 1024);
	} else {
		xup = xupl_init_with_file_name(argv[1]);
	}

	return xup;
}

xupl *xupl_print(xupl *xup) {
	xupl_*_ = (xupl_*) xup;
	xmlSaveFormatFileEnc("-", _->doc, (const char*) "UTF-8", 1);
	return xup;
}

xupl *xupl_cleanup(xupl *xup) {
	xmlCleanupParser();
	return xup;
}

xupl *xupl_nestv(xupl *xup, va_list args) {
	xupf f = va_arg(args, xupf);
	if (f) {
		xup = f(xupl_nestv(xup, args));
	}
	return xup;
}

xupl *xupl_nest(xupl *xup, ...) {
	va_list args;
	va_start(args, xup);
	xup = xupl_nestv(xup, args);
	va_end(args);
	return xup;
}

xupl *xupl_chainv(xupl *xup, va_list args) {
	for (xupf f = va_arg(args, xupf); f; f = va_arg(args, xupf)) {
		xup = f(xup);
	}
	return xup;
}

xupl *xupl_chain(xupl *xup, ...) {
	va_list args;
	va_start(args, xup);
	xup = xupl_chainv(xup, args);
	va_end(args);
	return xup;
}

xupl *xupl_done(xupl *xup) {
	int status = 1;
	if (xup) {
		xupl_*_ = (xupl_*) xup;
		xupl_cleanup(xup);
		status = _->status;
		_->doc = NULL;
		_->in = NULL;
		_->buffsize = 0;
		free(xup);
	}
	return NULL;
}

xupl* xupl_parse(xupl *xup) {

	xupl_*_ = (xupl_*) xup;
	FILE* in = _->in;
	off_t buffsize = _->buffsize;

	unsigned short bit = 0x0001;
	unsigned short WHITESPACE = bit;

	unsigned short DOUBLE_STRING = (bit <<= 1);
	unsigned short SINGLE_STRING = (bit <<= 1);
	unsigned short STRING = DOUBLE_STRING | SINGLE_STRING;

	unsigned short LINE_COMMENT = (bit <<= 1);
	unsigned short MULTI_COMMENT = (bit <<= 1);
	unsigned short COMMENT = LINE_COMMENT | MULTI_COMMENT;

	unsigned short _state = 0;

	const int default_tksize = 12;
	int tksize = default_tksize + 1;

	unsigned char *tk = NULL;
	int tkndx = 0;

	int chars_read;
	char* buf = malloc(buffsize + 1);

	xmlNodePtr xroot = NULL;
	xmlNodePtr xc = NULL;
	xmlAttrPtr xprop = NULL;

	const xmlChar* xuplAttr = (const xmlChar*) "data-xupl";
	const xmlChar* xuplClosed = (const xmlChar*) "closed";

	xmlDocPtr xdoc = xmlNewDoc((const unsigned char*) "1.1");
	//xmlNsPtr xuplNs = xmlNewGlobalNs(xdoc,"http://ns.xupl.org/1.1","xupl");

	while ((chars_read = fread(buf, 1, buffsize, in)) > 0) {

		for (int i = 0; i < chars_read; i++) {
			const char c = buf[i];

			switch (c) {

				case '\'':
				case '"':
					IF(COMMENT) break;

					IF(STR(c)) {
						DISABLE(STR(c));
					} else if (NOT(STRING)) {
						ALLOW(STR(c));
						break;
					}

				case '{':
				case '}':
				case ' ':
				case '\n':
				case '\r':
				case '\t':
				case '\f':
				case '\v':
				case ',':
				case '=':
				// Comment characters
				case '*':
				case '/':
				case '#':
				case '!':
					IF(STRING) break;

					switch (c) {
						case ',':
						case '{':
							xprop = NULL;
					}

					if (tk) {
						tk[tkndx] = 0;

						char p = 0;
						if (tkndx >= 1) p = tk[tkndx - 1];
						unsigned char* m = NULL;

						unsigned int tklen = tkndx + 1;
						unsigned char* t = tk;

						IF(COMMENT) {
							if ('\n' == c && IS(LINE_COMMENT)) {
								if (tkndx + 1 < tksize) {
									tk[tkndx++] = ' ';
									tk[tkndx] = 0;
								}
							} else if ('*' == p && '/' == c && IS(MULTI_COMMENT)) {
								tk[tkndx - 1] = 0;
							} else break;
							DISABLE(COMMENT);
							m = tk + 2;
						} else if ('/' == p && '*' == c) {
							ALLOW(MULTI_COMMENT);
							break;
						// Single-line comments can be #! #* #/ ##
						} else if ('#' == p && ('!' == c || '*' == c || '/' == c || '#' == c)) {
							ALLOW(LINE_COMMENT);
							break;
						// If these characters were in the token and not part of a comment,
						// then continue as if they are normal characters of a token.
						} else if ('!' == c || '/' == c || '*' == c || '#' == c) break;

						if (!xc) {
							if (m) {
								xroot = xmlNewDocComment(xdoc, m);
								xmlDocSetRootElement(xdoc, xroot);
							} else {
								xc = xmlNewNode(NULL, tk);
								xmlDocSetRootElement(xdoc, xc);
								if (!xroot) xroot = xc;
							}
						} else if (m) {
							xmlAddChild(xc, xmlNewComment(m));
						} else {
							char *attr = NULL;
							xmlAttrPtr closed = xmlHasProp(xc, xuplAttr);

							switch (tk[0]) {
								case '\'':
								case '"':
									t += 1;
									xmlAddChild(xc, xmlNewText(t));
									break;
									// TODO make this parameterized, characters and names.
								case '.': attr = "class";    break;
								case '#': attr = "id";       break;
								case '@': attr = "project";  break;
								case '/': attr = "href";     break;
								case '[': attr = "data";     break;
								case '~': attr = "duration"; break;
								case '=': attr = "location"; break;
								case '^': attr = "at";       break;
								case ':': attr = "type";     break;
								case '!': attr = "priority"; break;
								default:
								{
									regmatch_t pmatch[1];
									unsigned int isword = 0 == regexec(&re_word, (char*) tk, 1, pmatch, 0);
									if (closed) {
										if (isword) {
											xc = xmlNewChild(xc, NULL, tk, NULL);
										} else {
											xmlAddChild(xc, xmlNewText(tk));
										}
									} else {
										if (xprop) {
											xmlNewProp(xc, xprop->name, tk);
											xmlRemoveProp(xprop);
											xprop = NULL;
										} else if (isword) {
											xprop = xmlNewProp(xc, tk, (unsigned char*) "True");
										} else {
											xprop = xmlNewProp(xc, (unsigned char*) ".fake", tk);
										}
										switch (c) {
											case ',':
											case '{':
												xprop = NULL;
										}
									}
								}
									break;
							}

							if (attr) {
								if (closed) {
									xmlAddChild(xc, xmlNewText(t));
								} else {
									xmlNewProp(xc, (xmlChar*) attr, t);
								}
							}
						}

						free(tk);
						tk = NULL;
						if (tksize > default_tksize && tkndx < default_tksize) {
							tksize /= 2;
						}
						tkndx = 0;

						if (m) continue;
					}

				default:
					break;
			}

			switch (c) {
				case '{':
					xmlNewProp(xc, xuplAttr, xuplClosed);
					continue;
				case '}':
					if (xc) {
						xmlAttrPtr data = xmlHasProp(xc, xuplAttr);
						if (data) xmlRemoveProp(data);
						xc = xc->parent;
					}
					continue;
				default:
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

	// Remove remaining data-xupl.
	while (xc) {
		xmlAttrPtr data = xmlHasProp(xc, xuplAttr);
		if (data) xmlRemoveProp(data);
		xc = xc->parent;
	}

	if (tk) free(tk);
	free(buf);

	_->doc = xdoc;

	return xup;
}

void Xupl(int argc, char *argv[], xupd d, xupf f, ...) {
	xupl* _ = xupl_init(argc, argv);
	xupl_parse(_);
	if (f) _ = f(_);
	if (d) d(((xupl_*) _)->doc);
	xupl_done(_);
}
