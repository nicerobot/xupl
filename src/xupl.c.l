
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
%}

%s DOCUMENT MULTI_COMMENT LINE_COMMENT ELEMENT ATTRIBUTE BODY

%%
<INITIAL>"?xupl"                                      { printf("XUPL_DEFINITION\n"); BEGIN(DOCUMENT); }
<MULTI_COMMENT>{
"*/"      { printf("MULTI COMMENT\n"); BEGIN(DOCUMENT); }
[^*\n]+
"*"
\n        yylineno++;
}

<LINE_COMMENT>{
[^\n]+
\n     		{ yylineno++; printf("LINE COMMENT\n"); BEGIN(DOCUMENT); }
}
[#][!][^\n]+     																			{ printf("START COMMENT\n"); BEGIN(LINE_COMMENT); }
[/][*]           																			{ printf("START MULTI COMMENT\n"); BEGIN(MULTI_COMMENT); }
<ELEMENT,ATTRIBUTE>[{]                                { printf("XUPL_OPEN_BLOCK\n"); BEGIN(ELEMENT); }
<DOCUMENT,ELEMENT,BODY>[}]                            { printf("XUPL_CLOSE_BLOCK\n"); }
<DOCUMENT>[ ]+                                        { printf("XUPL_SPACE\n"); }
<DOCUMENT>[\t]+                                       { printf("XUPL_TAB\n"); }
<DOCUMENT>[\n\v\f\r]+                                 { yylineno++; printf("XUPL_LINE\n"); }
<INITIAL,ELEMENT,ATTRIBUTE>[:_a-zA-Z][:_a-zA-Z0-9.-]+ { printf("XUPL_WORD\n"); }
<DOCUMENT>[^\n\f\v\r\t ,:.\{\}\[\]\(\)\#\\/]+         { printf("XUPL_ANY_WORD[%s]\n",yytext); }
%%
