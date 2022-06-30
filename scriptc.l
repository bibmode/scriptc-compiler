%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scriptc.tab.h"
int iTemp;
char strTemp[200];

%}

%option yylineno

%%
"display"                           {return display;}

"int"                               {yylval.s = strdup(yytext); return INT;}
"float"                             {yylval.s = strdup(yytext); return FLOAT;}
"char"                              {yylval.s = strdup(yytext); return CHAR;}
                             
[-]?[0-9]+                          {sscanf(yytext, "%d", &iTemp); yylval.i = iTemp; return INTEGERS;}
[-]?[0-9]+\.[0-9]+                  {yylval.f = atof(yytext); return DECIMALS;}       
[a-z]+[a-zA-Z_0-9]+                 {yylval.s = strdup(yytext); return IDENTIFIER;}
\"([^"]|\\\")*\"                    {
                                        yylval.s = strdup(yytext+1);
                                        if (yylval.s[yyleng-2] != '"')
                                            yyerror("improperly terminated string");
                                        else
                                            yylval.s[yyleng-2] = 0;
                                        return STRING;
                                    }

\'.*\'                              {
                                          yylval.c = strdup(yytext+1);
                                          if (yylval.c[yyleng-2] != '\'')
                                              yyerror("improperly terminated string");
                                          else
                                              yylval.c[yyleng-2] = 0;
                                          return CHARACTER;
                                    }


\n                                  {return NEWLINE;}


[ \t\n]                             ;
[-+*/=":,()]                        {return yytext[0];}
.                                   {ECHO; yyerror ("unexpected character");}

%%
int yywrap (void) {return 1;}