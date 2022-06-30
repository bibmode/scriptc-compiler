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

	struct idVals{
		char *varName;
		int numVal;
		char *strVal;
		int type;
	};
}

%union {
	int i; float f; char* s; char* c;
	struct nodeVals p;
	struct idVals id;
}     

    /* Yacc definitions */

%token <s> display IDENTIFIER STRING NEWLINE INT CHAR FLOAT <i> INTEGERS <f> DECIMALS <c> CHARACTER 
%type <f> expr term factor values
%type <s> type str print 
%type <p> printVals 
%type <id> id 
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
numVar_statements	:	id ':' type								{checkVarDup($1.varName,$3);}
					|	id '=' expr												{checkNumVarExist($1.varName,$3);}
					|	id ':' type '=' expr							{checkVarDup($1.varName,$3); saveThisNumVal($1.varName,$5); updateNumVal($1.varName,$5);}
					;

letVar_statements	:	id ':' CHAR								{checkVarDup($1.varName,$3);}
					|	id '=' str												{checkCharVarExist($1.varName,$3);}
					|	id ':' CHAR '=' str								{
																												checkVarDup($1.varName,$3); 
																												saveThisCharVal($1.varName,$5); 
																												updateCharVal($1.varName,$5);
																											}
					|	id ':' CHAR '=' id				{
																												checkVarDup($1.varName,$3); 
																												char *strVal = getStringFromId($5.varName);
																												saveThisCharVal($1.varName,strVal); 
																												updateCharVal($1.varName,strVal);
																											}
					;

/* type can be either INT or FLOAT */
type		:	INT																{$$ = $1;}
			|	FLOAT															{$$ = $1;}
			;

/* EDITED BY ME */
/* expected inputs for the print statement */
print :	display ':' STRING         						{printValues($3);}
			| display ':' STRING printVals 					{printStruct($3, $4.numbers, $4.numbersLen, $4.stringsLen);}
			;

/* ADDED BY ME */
printVals : ',' expr							{
																		$$.numbers[$$.numbersLen] = $2;
																		$$.numbersLen++;
																	}
				  | ',' str   						{
																		addStr($2, $$.stringsLen);
																		$$.stringsLen++;
																	}
				  | ',' id   							{ 
																		checkIfVarExist($2.varName);
																		if($2.type == 0){
																			addStr($2.strVal, $$.stringsLen);
																			$$.stringsLen++;
																		} else if($2.type == 1){
																			$$.numbers[$$.numbersLen] = $2.numVal;
																			$$.numbersLen++;
																		}
					 												}
				  | printVals ',' expr    {
																		$$.numbers[$$.numbersLen] = $3;
																		$$.numbersLen++;
																	}
					| printVals ',' str			{
																		addStr($3, $$.stringsLen);
																		$$.stringsLen++;
																	}
					| printVals ',' id   		{ 
																		checkIfVarExist($3.varName);
																		if($3.type == 0){
																			addStr($3.strVal, $$.stringsLen);
																			$$.stringsLen++;
																		} else if($3.type == 1){
																			$$.numbers[$$.numbersLen] = $3.numVal;
																			$$.numbersLen++;
																		}
					 												}
					;

/* expected inputs for the arithmetic statement */
expr    	:	term															{$$ = $1;}
       	    |	expr '+' term										{$$ = $1 + $3;}
						|	id '+' factor										{$$ = checkThisNumVar($1.varName) + $3;}
        		|	factor '+' id										{$$ = $1 + checkThisNumVar($3.varName);}
       	    |	expr '-' term										{$$ = $1 - $3;}
						|	id '-' factor										{$$ = checkThisNumVar($1.varName) - $3;}
        		|	factor '-' id										{$$ = $1 - checkThisNumVar($3.varName);}
       	    ;

term		:	factor															{$$ = $1;}
        	|	term '*' factor										{$$ = $1 * $3;}		
        	|	term '/' factor										{$$ = $1 / $3;}
					|	id '*' factor											{$$ = checkThisNumVar($1.varName) * $3;}
        	|	factor '*' id											{$$ = $1 * checkThisNumVar($3.varName);}
        	|	id '/' factor											{$$ = checkThisNumVar($1.varName) / $3;}
        	|	factor '/' id											{$$ = $1 / checkThisNumVar($3.varName);}
        	;

factor		:	values														{$$ = $1;}
			|	'(' expr ')'													{$$ = $2;}	
			;

/* term can be either int or float or variable holding the value */
values	:	INTEGERS													{$$ = $1;}
		  	|	DECIMALS													{$$ = $1;}
		    ;

str			:	CHARACTER													{$$ = $1;}
				| STRING														{$$ = $1;}
				;

id			:	IDENTIFIER												{
																								int typeId = checkReturnType($1);
																								if(typeId == 1){
																									$$.strVal = checkThisCharVar($1);
																									$$.type = 0;
																								} else if(typeId == 2){
																									$$.numVal = checkThisNumVar($1);
																									$$.type = 1;
																								}
																							$$.varName = $1;
																						} 
				;


%%                    

int main (void) {
	return yyparse();
}

void yyerror (const char *errorSTR) {
	fflush(stdout);
	fprintf(stderr, "\nLINE %d Error: %s\n", yylineno, errorSTR);
	exit(0);
}