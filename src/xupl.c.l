/* XUPL */

%option noyywrap reentrant

%{

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

typedef struct xupl_ {
	xmlDocPtr doc;
	xmlNodePtr root;
	xmlNodePtr c;
	xmlAttrPtr prop;
	unsigned char* acc;
} xupl_;

#define YY_EXTRA_TYPE xupl_*

%}

%s DOCUMENT MULTI_COMMENT ELEMENT_STRING ELEMENT ATTRIBUTE BODY

%%
<INITIAL>"?xupl" {
	BEGIN(DOCUMENT); }

<MULTI_COMMENT>{
"*/" {
	if (yyextra->doc) {
		xmlNewDocComment(yyextra->doc, yyextra->acc);
	} else {
		xmlAddChild(yyextra->c, xmlNewComment(yyextra->acc));
	}
	free(yyextra->acc);
	yyextra->acc = NULL;
	BEGIN(DOCUMENT); }

"*"|[^*\n]+ {
	strcat((char*)yyextra->acc,yytext); }

\n {
	strcat((char*)yyextra->acc,yytext);
	yylineno++; }

}

<ELEMENT_STRING>{
"\""|"'" {
	xmlAddChild(yyextra->c, xmlNewText(yyextra->acc));
	free(yyextra->acc);
	yyextra->acc = NULL;
	BEGIN(ELEMENT); }

[^""\n]+ {
	strcat((char*)yyextra->acc,yytext); }

\n {
	strcat((char*)yyextra->acc,yytext);
	yylineno++; }

}

<*>[#][!][^\n]+ {
	if (yyextra->doc) {
		xmlNewDocComment(yyextra->doc, (unsigned char*)(yytext+2));
	} else {
		xmlAddChild(yyextra->c, xmlNewComment((unsigned char*)(yytext+2)));
	}
}

<*>[/][*] {
	yyextra->acc = realloc(yyextra->acc, 200);
	yyextra->acc[0] = 0;
	BEGIN(MULTI_COMMENT); }

<ELEMENT>"\""|"'" {
	yyextra->acc = realloc(yyextra->acc, 200);
	yyextra->acc[0] = 0;
	BEGIN(ELEMENT_STRING); }

<ELEMENT,ATTRIBUTE>[{] {
	BEGIN(ELEMENT); }

<DOCUMENT,ELEMENT,BODY>[}] {
		yyextra->c = yyextra->c->parent;
	}

<DOCUMENT>[ ]+ {
	}

<DOCUMENT>[\t]+ {
	}

<DOCUMENT>[\n\v\f\r]+ {
	yylineno++; }

<INITIAL,ELEMENT>[:_a-zA-Z][:_a-zA-Z0-9.-]+ {
	if (!(yyextra->c)) {
		yyextra->root = yyextra->c = xmlNewNode(NULL, (unsigned char*) yytext);
		xmlDocSetRootElement(yyextra->doc, yyextra->root);		
	}
	yyextra->c = xmlNewChild(yyextra->c, NULL, (unsigned char*) yytext, NULL);
	BEGIN(ATTRIBUTE);
}

<ATTRIBUTE>[:_a-zA-Z][:_a-zA-Z0-9.-]+ {
	if (yyextra->c) {
		xmlNewProp(yyextra->c, (unsigned char*)yytext, (unsigned char*) "True");
	}
}

<DOCUMENT>[^\n\f\v\r\t ,:.\{\}\[\]\(\)\#\\/]+ {
	}

%%

int main () {
	yyscan_t scanner;
	int tok;

	xupl_ X = {0};
	X.doc = xmlNewDoc((const unsigned char*) "1.1");

	yylex_init_extra(&X, &scanner);

	while ((tok=yylex(scanner)) > 0)
	   printf("tok=%d  yytext=%s\n", tok, yyget_text(scanner));

	yylex_destroy(scanner);
	xmlSaveFormatFileEnc("-", X.doc, (const char*) "UTF-8", 1);
	return 0;
}
