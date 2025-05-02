#include "assembler.h"


/*
*	this function works line after line after the firstpass stored the relvant info
*	it checks if the file end and if its not guidnce line its skip this line
*	if its a guidnce line as entry guidnce it stores the info in entry guidnce table in order to check the lables 
*	inside the symbol table. after it check if errors occoured
*/
void secondPass(char *line){
	char *word;
	char delim[3] = " \t,";
	char *symbol;
	int type;
	word = strtok(line, delim);
	type = getGuidanceCode(word);
	if (isEOF(word) || shouldSkipLineSecondPass(word) == TRUE){
		return;
	}
	else if (type != FALSE){
		if (type == ENTRY){
			symbol = getNextWord(word);
			if (symbol[strlen(symbol) - 2] == '\r'){
				symbol[strlen(symbol) - 2] = '\0';
			}
			else if (symbol[strlen(symbol) - 1] == '\n'){
				symbol[strlen(symbol) - 1] = '\0';
			}
			addEntryToSymbols(symbol, lineNumber);
			return;
		}
	}
}