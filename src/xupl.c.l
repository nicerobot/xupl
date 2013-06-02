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
	unsigned char quote;
	unsigned short prior;
} xupl_;

#define YY_EXTRA_TYPE xupl_*

%}

%s MULTI_COMMENT SINGLE_STRING DOUBLE_STRING ELEMENT ATTRIBUTE BODY

%%
<INITIAL>"?xupl" {
	BEGIN(ELEMENT); }

<MULTI_COMMENT>{
"*/" {
	if (yyextra->doc) {
		xmlNewDocComment(yyextra->doc, yyextra->acc);
	} else {
		xmlAddChild(yyextra->c, xmlNewComment(yyextra->acc));
	}
	free(yyextra->acc);
	yyextra->acc = NULL;
	BEGIN(yyextra->prior); }

"*"|[^*\n]+ {
	strcat((char*)yyextra->acc,yytext); }

\n {
	strcat((char*)yyextra->acc,yytext);
	yylineno++; }

}

<SINGLE_STRING,DOUBLE_STRING>{
"\""|"'" {
	unsigned short state = (yyg->yy_start - 1)/2;

	if ((state == SINGLE_STRING && 39 != yytext[0])
	    || (state == DOUBLE_STRING && 34 != yytext[0])) {
		strcat((char*)yyextra->acc,yytext);
	} else {
		xmlAddChild(yyextra->c, xmlNewText(yyextra->acc));
		free(yyextra->acc);
		yyextra->acc = NULL;
		BEGIN(yyextra->prior);
	}

}

[^""'\n]+ {
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
	yyextra->prior = (yyg->yy_start - 1)/2;
	BEGIN(MULTI_COMMENT); }

<*>[""'] {
	yyextra->acc = realloc(yyextra->acc, 200);
	yyextra->acc[0] = 0;
	yyextra->prior = (yyg->yy_start - 1)/2;
	if ('\'' == yytext[0]) BEGIN(SINGLE_STRING);
	else BEGIN(DOUBLE_STRING);
}

<ELEMENT,ATTRIBUTE>[{] {
	BEGIN(ELEMENT); }

<ELEMENT,ATTRIBUTE>[}] {
		yyextra->c = yyextra->c->parent;
	}

<ELEMENT>[ ]+ {
	BEGIN(ATTRIBUTE); }

<ELEMENT>[\t]+ {
	BEGIN(ATTRIBUTE); }

<ELEMENT>[\n\v\f\r]+ {
	yylineno++;
	BEGIN(ATTRIBUTE); }

<INITIAL,ELEMENT>[:_a-zA-Z][:_a-zA-Z0-9.-]* {
	if (!(yyextra->c)) {
		yyextra->root = yyextra->c = xmlNewNode(NULL, (unsigned char*) yytext);
		xmlDocSetRootElement(yyextra->doc, yyextra->root);		
	}
	yyextra->c = xmlNewChild(yyextra->c, NULL, (unsigned char*) yytext, NULL);
	BEGIN(ATTRIBUTE);
}

<ATTRIBUTE>[:_a-zA-Z][:_a-zA-Z0-9.-]* {
	if (yyextra->c) {
		xmlNewProp(yyextra->c, (unsigned char*)yytext, (unsigned char*) "True");
	}
}

<ATTRIBUTE>[#][:_a-zA-Z][:_a-zA-Z0-9.-]* {
	if (yyextra->c) {
		xmlNewProp(yyextra->c, (unsigned char*)"id", (unsigned char*)(yytext+1));
	}
}

<ATTRIBUTE>[.][^ \{\t\n]+ {
	if (yyextra->c) {
		xmlNewProp(yyextra->c, (unsigned char*)"class", (unsigned char*)(yytext+1));
	}
}

<ATTRIBUTE>[!][^ \t\n]+ {
	if (yyextra->c) {
		xmlNewProp(yyextra->c, (unsigned char*)"priority", (unsigned char*)(yytext+1));
	}
}

<*>[^\n\f\v\r\t ,""'!:.\{\}\#\\/]+ {
		printf("%d[%s]\n", (yyg->yy_start-1)/2, yytext);
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
