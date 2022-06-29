%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "scriptc-tools.c"

#define YYERROR_VERBOSE 1

extern int yylex();
extern void yyerror (const char *errorSTR);
extern yylineno;

%}

/* THIS IS FOR THE p DATA TYPE */
%code requires {
	struct nodeVals{
			float numbers[100];
			int numbersLen;
			char *strings[100];
			int stringsLen;
	};
}

%union {
	int i; float f; char* s; char* c;
	struct nodeVals p;
}     

    /* Yacc definitions */

%token <s> display IDENTIFIER STRING NEWLINE INT CHAR FLOAT <i> INTEGERS <f> DECIMALS <c> CHARACTER 
%type <f> expr term factor values
%type <s> type str print 
%type <p> printVals 
%%

/* descriptions of expected inputs corresponding actions (in C) */

/* main line */
program		:	commands														
			|	program commands												
			;

commands	:	numVar_statements
			|	letVar_statements
			|	print
			|	NEWLINE															{line++;}
			;

/* expected inputs for the variable declaration & initialization */
numVar_statements	:	IDENTIFIER ':' type										{checkVarDup($1,$3);}
					|	IDENTIFIER '=' expr										{checkNumVarExist($1,$3);}
					|	IDENTIFIER ':' type '=' expr							{checkVarDup($1,$3); saveThisNumVal($1,$5); updateNumVal($1,$5);}
					;
letVar_statements	:	IDENTIFIER ':' CHAR										{checkVarDup($1,$3);}
					|	IDENTIFIER '=' str										{checkCharVarExist($1,$3);}
					|	IDENTIFIER ':' CHAR '=' str								{checkVarDup($1,$3); saveThisCharVal($1,$5); updateCharVal($1,$5);}
					;

/* type can be either INT or FLOAT */
type		:	INT																{$$ = $1;}
			|	FLOAT															{$$ = $1;}
			;

/* EDITED BY ME */
/* expected inputs for the print statement */
print :	display ':' STRING         						{printValues($3);}
			| display ':' STRING printVals 					{printStruct($3, $4.numbers, $4.strings, $4.numbersLen, $4.stringsLen);}
			;

/* ADDED BY ME */
printVals : ',' expr							{
																		$$.numbers[$$.numbersLen] = $2;
																		$$.numbersLen++;
																	}
				  | ',' str   						{
																		$$.strings[$$.stringsLen] = $2;
																		$$.stringsLen++;
																	}
				  | printVals ',' expr    {
																		$$.numbers[$$.numbersLen] = $3;
																		$$.numbersLen++;
																	}
					| printVals ',' str			{
																		$$.strings[$$.stringsLen] = $3;
																		$$.stringsLen++;
																	}
					;

/* expected inputs for the arithmetic statement */
expr    	:	term															{$$ = $1;}
       	    |	expr '+' term													{$$ = $1 + $3;}
       	    |	expr '-' term													{$$ = $1 - $3;}
       	    ;

term		:	factor															{$$ = $1;}
        	|	term '*' factor													{$$ = $1 * $3;}		
        	|	term '/' factor													{$$ = $1 / $3;}
        	;

factor		:	values															{$$ = $1;}
			|	'(' expr ')'													{$$ = $2;}		
			;

/* term can be either int or float or variable holding the value */
values	:	IDENTIFIER												{$$ = checkThisNumVar($1);}
			  |	INTEGERS													{$$ = $1;}
		  	|	DECIMALS													{$$ = $1;}
		    ;

str			:	IDENTIFIER												{$$ = checkThisCharVar($1);}
				|	CHARACTER													{$$ = $1;}
				| STRING														{$$ = $1;}
				; 


%%                    

int main (void) {
	return yyparse();
}

void yyerror (const char *errorSTR) {
	fflush(stdout);
	fprintf(stderr, "\nLINE %d Error: %s\n", yylineno, errorSTR);
}