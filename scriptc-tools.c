#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

int line = 1;		// for getting incremented line number indexes
int indexVar = 0;		// for incrementing variable indexes

/*struct for storing ID or Variable data*/
typedef struct indentifiers{
	char var[1000];		// var stores variable names
	char typ[1000];		// typ stores variable's type
	int ival;		// ival stores int type values
	float fval;		// fval stores float type values
	char cval[1000];   // cval stores char type values
} identifier;

float symbols[1000];		// symbols store values to the identifier
char* char_symbols[1000];		// symbols store chartype values to the identifier
identifier id[1000];		// id will be the struct variable name and has 1000 indexes to store data

char stringsDisplay[100][100];

void addStr(char *str, int length){
	strcpy(stringsDisplay[length], str);
}

char* checkThisCharVar(char* variable);
int count(const char *str, const char *sub);
void printValues(char* string);
void substringInsert(int position, char* str1, char* str2);
void replaceNumbers(char* string, float value);
void printFinalString(char *strFinal);
void printStruct(char *inputStr, float numbers[], int numbersLen, int stringsLen);

void test(char *str){
	printf("\ntest -> %s\n", str);
}

char *getStringFromId(char *str){
	char *val = checkThisCharVar(str);
}

void checkIfVarExist(char *variable){
	int i;
	int flag = 0;
	
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			flag=1;
			break;
		}
	}

	if(flag==0) {
		printf("\nLINE %d Error: undeclared variable, '%s'",line,variable);
		exit(0);
	}

}



int checkReturnType(char *variable){
	int i;
	int flag = 0;

	
	
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if (strcmp(id[i].typ,"char")==0){
				flag = 1;
				break;
			} else if (strcmp(id[i].typ,"int")==0 || strcmp(id[i].typ,"float")==0){
				flag = 2;
				break;
			} 
		}
	}

	return flag;

	// char *val = checkThisVar(str);

	// printf("%s", val);
}

///////////////////////

/* compIdxVar will compute the given variable index and return it*/    
int compIdxVar(char* variable){
    int idx = *variable;
    return idx;
} 


/* getNumValue gets the given variable's int or float value and return it to the IDENTIFIER token */
float getNumValue(char* variable){	
	int i;
	
	int bucket = compIdxVar(variable);		// recognized variable index will be initialized to the bucket variable
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){		// <-- this means if the struct id.var is equal to the current variable name
			if(strcmp(id[i].typ,"int")==0){
				symbols[bucket] = id[i].ival;	
				return symbols[bucket];		// returns the given variable's recognized int value according to its index
			}
			else if(strcmp(id[i].typ,"float")==0){
				symbols[bucket] = id[i].fval;
				return symbols[bucket];		// returns the current variable's recognized float value according to its index
			}	
		}
	}
}

int checkForValue(char *str){
	int val = 0;
	char *temp;

	int lenOfStr = 0;
	lenOfStr = strlen(str);

	// printf("line 123 -> %s",temp);
	if(lenOfStr != 0){
		val = 1;
	}

	return val;
}


/* getCharValue gets the given variable's char type value and return it to the IDENTIFIER token */
char* getCharValue(char* variable){	
	int i, valCheck;
	
	int bucket = compIdxVar(variable);		// recognized variable index will be initialized to the bucket variable
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){	
			if(strcmp(id[i].typ,"char")==0){
				valCheck = checkForValue(id[i].cval);
				if(valCheck){
					strcpy(char_symbols[bucket],id[i].cval);
					return char_symbols[bucket]; // returns the current variable's recognized char type value according to its index
				} else {
					return NULL;
				}
						
			}
		}
	}
}


/* updateNumVal updates the given variable's int or float type value when given another new values */
void updateNumVal(char* variable, float value){
	int i;
	int toIntValue = (int)value;		// typecasting to int or to convert float value datatype to int
	int bucket = compIdxVar(variable);

	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"int")==0){			
				symbols[bucket] = toIntValue;
				id[i].ival = symbols[bucket];		// new int values will be saved to the struct identifiers (id.ival)		
				break;
			}
			else if(strcmp(id[i].typ,"float")==0){
				symbols[bucket] = value;
				id[i].fval = symbols[bucket];		// new float values will be saved to the struct identifiers (id.fval)
				break;
			}
		}
	}
}


/* updateCharVal updates the given variable's char type value when given another new values */
void updateCharVal(char* variable, char* value){
	int i;
	
	int bucket = compIdxVar(variable);

	// printf("\nupdatethischarval is working\n");
	
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"char")==0){
				char_symbols[bucket] = value;
				strcpy(id[i].cval,char_symbols[bucket]); // new float values will be saved to the struct identifiers (id.cval)
				break;
			}
		}
	}
}


/* saveThisVar saves the verified given variable and its given type to the struct identifiers */
void saveThisVar(char* variable, char* type){
	strcpy(id[indexVar].var,variable);
	strcpy(id[indexVar].typ,type);
	indexVar++;		// Increments to the next ID index after saving the variable and type
}


/* saveThisNumVal saves the value (int or float type) to the struct identifiers */
void saveThisNumVal(char* variable, float value){
	int i;
	int toIntValue = (int)value;

	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"int")==0){
				id[indexVar].ival = toIntValue;		
				break;		
			}
			else if(strcmp(id[i].typ,"float")==0){
				id[indexVar].fval = value;	
				break;
			}
		}
	}
}


/* saveThisCharVal saves the value (char type) to the struct identifiers */
void saveThisCharVal(char* variable, char* value){
	int i;

	
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"char")==0){
			
				strcpy(id[indexVar].cval,value);
				break;		
			}
		}
	}
}


/* checkVarDup checks if the given variable has duplicates or has been redeclared  */
void checkVarDup(char* variable, char* type){
	int i;
	int flag = 0;
	
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){	
			flag = 1;
			break;
		}
	}
	if(flag==1){
		printf("\n>>>> ERROR LINE %d: '%s' already declared! <<<<",line,variable);	
	}
	else{
		saveThisVar(variable,type);		// otherwise, it will invoke the saveThisVar function to save the variable and its type
		// printf("\nLINE %d: Correct Variable '%s' Declaration!",line,variable);	
	}
}


/* checkNumVarExist checks if the given variable (int or float type) exists during initialization */
void checkNumVarExist(char* variable, float value){
	int i;
	int flag = 0;
	
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"int")==0 || strcmp(id[i].typ,"float")==0){
				flag = 1;
				break;			
			} 
		} 
	}
	if(flag==1){
		saveThisNumVal(variable,value);		// if exists, it will invoke the saveThisVar function to save the variable's value
		updateNumVal(variable,value);		// then, it will invoke the updateNumVal function to update the variable's value
		// printf("\nLINE %d: Correct Variable '%s' Initialization!",line,variable);
	} else {
		printf("\nLINE %d Error: undeclared variable, '%s'",line,variable);
		exit(0);
	}
}


/* checkCharVarExist checks if the given variable (char type) exists during initialization */
void checkCharVarExist(char* variable, char* value){
	int i;
	int flag = 0;


	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"char")==0){

				flag = 1;
				break;			
			}
		}
	}
	if(flag==1){
		saveThisCharVal(variable,value);		// if exists, it will invoke the saveThisVar function to save the variable's value
		updateCharVal(variable,value);		// then, it will invoke the updateCharVal function to update the variable's value
		// printf("\nLINE %d: Correct Variable '%s' Initialization!",line,variable);
		
	} else {
		printf("\nLINE %d Error: undeclared variable, '%s'",line,variable);
		
		exit(0);
	}
}


/* checkThisNumVar checks if the given variable (int or float type) initialized to another variable exists*/
/* checkThisNumVar also checks if the given variable (int or float type) exists during printing*/
float checkThisNumVar(char* variable){
	int i;
	int flag = 0;

	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if(strcmp(id[i].typ,"int")==0){
				flag = 1;
				break;			
			}
			else if (strcmp(id[i].typ,"float")==0){
				flag = 1;
				break;
			}
			else if (strcmp(id[i].typ,"char")==0) {
				flag = 2;
				break;
			}
		}
	}
	if(flag==1){
		return getNumValue(variable); // if exists, then it will invoke the getNumValue function
	} else if(flag==2) {
		checkThisCharVar(variable);
	} else {
		printf("\nLINE %d Error: undeclared variable, '%s'",line,variable);
		exit(0);
	}
}


/* checkThisCharVar checks if the given variable (char type) initialized to another variable exists*/
/* checkThisCharVar also checks if the given variable (char type) exists during printing*/
char* checkThisCharVar(char* variable){
	int i;
	int flag = 0;
	
	for(i=0;i<indexVar;i++){
		if(strcmp(id[i].var,variable)==0){
			if (strcmp(id[i].typ,"char")==0){
				flag = 1;
				break;
			}
		}
	}
	if(flag==1){
		return getCharValue(variable); // if exists, then it will invoke the getCharValue function
	} else {
		printf("\nLINE %d Error: undeclared variable, '%s'",line,variable);
		exit(0);
	}
}


// START OF MY CODES
// ALL OF THE FUNCTIONS HERE ARE INITIALIZED ABOVE AT LINE 27 - 31

// DISPLAYS NORMAL STRING FOR YACC LINE 68
void printValues(char* string){
	printf("%s", string);
}

// USED FOR COUNTING THE SPECIFIERS FOUND IN THE DISPLAY STRING
int count(const char *str, const char *sub) {
    int sublen = strlen(sub);
    if (sublen == 0) return 0;
    int res = 0;
    for (str = strstr(str, sub); str; str = strstr(str + sublen, sub))
        ++res;
    return res;
}

// REPLACES THE IDENTIFIER WITH THEIR VALUES
void substringInsert(int pos, char* str1, char* str2){
	int counter;
	int position = pos-1;
	int strLength = strlen(str1);
	char *mainStr, finalStr[200];
	int leftChars = position;
	int rightChars = strLength - position - 2;

	char left[100] = "";

	for (counter = 0; counter < leftChars; counter++){
		char currentChar = str1[counter];
		strncat(left, &currentChar, 1);
	}

	char right[100] = "";

	for (counter = 0; counter < rightChars; counter++){
		char currentChar = str1[position + 2 + counter];
		strncat(right, &currentChar, 1);
	}

	strcpy(str1, left);
	strcat(str1, str2);
	strcat(str1, right);
}

// GETS THE POSITION OF IDENTIFIER ON STRING
int getPosition(char *str, char *subStr){
	char *dest = strstr(str, subStr);
	int pos;
	pos = dest - str + 1;
	return pos;
}

// PRINTS THE FINAL STRING OUTPUT WITH NEW LINES
void printFinalString(char *strFinal){
	int counter2=0;

	while(strFinal[counter2] != '\0')
		{
		 if(strFinal[counter2] == '\\' && strFinal[counter2+1] == 'n'){	
			printf("\n");
			counter2++;
		} else printf("%c", strFinal[counter2]);
			counter2++;
	}
}


// PRINTS THE STRING FOR THE FINAL PRODUCT IN YACC LINE 69
void printStruct(char* inputStr, float numbers[], int numbersLen, int stringsLen) {
	int numSpecifiers=0, strSpecifiers=0, floatSpecifiers=0, integerSpecifiers=0, charSpecifiers=0, stringSpecifiers=0;
	
	int posfloat=0, posint=0, poschar=0, posstr=0, counter;
	char strFinal[200], strInitial[200], strValue[100];

	floatSpecifiers = count(inputStr, "%f");
	integerSpecifiers = count(inputStr, "%d");
	charSpecifiers = count(inputStr, "%c");
	stringSpecifiers = count(inputStr, "%s");

	// final string
	strcpy(strFinal, inputStr);
	strcpy(strInitial, inputStr);
	
	numSpecifiers = floatSpecifiers + integerSpecifiers;
	strSpecifiers = charSpecifiers + stringSpecifiers;

	int position;

	// prints the number specifiers
	if(numSpecifiers > 0){
		for (counter = 0; counter <= numSpecifiers && counter <= numbersLen; counter++)
		{
			floatSpecifiers = count(strFinal, "%f");
			integerSpecifiers = count(strFinal, "%d");
	
			if(floatSpecifiers) posfloat = getPosition(strFinal, "%f"); 
			if(integerSpecifiers) posint = getPosition(strFinal, "%d"); 

			if(posfloat && posint && (posfloat < posint)){
				sprintf(strValue, "%f", numbers[counter]);
				substringInsert(posfloat, strFinal, strValue);
			} else if (posfloat && posint && (posfloat > posint)) {
				sprintf(strValue, "%d", (int)numbers[counter]);
				substringInsert(posint, strFinal, strValue);
			} else if (!posint && posfloat){
				sprintf(strValue, "%f", numbers[counter]);
				substringInsert(posfloat, strFinal, strValue);
			} else if(!posfloat && posint){
				sprintf(strValue, "%d", (int)numbers[counter]);
				substringInsert(posint, strFinal, strValue);
			}

			posfloat = posint = 0;
		}
	}

	// prints the string specifiers
	if(strSpecifiers > 0){

		for (counter = 0; counter <= strSpecifiers && counter <= stringsLen; counter++)
		{
			charSpecifiers = count(strFinal, "%c");
			stringSpecifiers = count(strFinal, "%s");
	
			if(charSpecifiers) poschar = getPosition(strFinal, "%c"); 
			if(stringSpecifiers) posstr = getPosition(strFinal, "%s"); 

			if(posstr && poschar && (posstr < poschar)){
				sprintf(strValue, "%s", stringsDisplay[counter]);
				substringInsert(posstr, strFinal, strValue);

			} else if (poschar && posstr && (posstr > poschar)) {
				sprintf(strValue, "%c", stringsDisplay[counter][0]);
				substringInsert(poschar, strFinal, strValue);

			} else if (!posstr && poschar){
				sprintf(strValue, "%c", stringsDisplay[counter][0]);
				substringInsert(poschar, strFinal, strValue);

			} else if(!poschar && posstr){
				sprintf(strValue, "%s", stringsDisplay[counter]);
				substringInsert(posstr, strFinal, strValue);
			}

			poschar = posstr = 0;
		} 
	}


	printFinalString(strFinal);
}



