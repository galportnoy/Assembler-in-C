#include "assembler.h"

/*
*	this function works line after line after the preassembler opened all macros
*	it checks if the line is comment or end of file
*	after it reads the assembly program and dedect if there are syntax errors
*	if all ok its stores and update the operation / symbol / data tables in order to make it binary later
*/
void firstPass(char *line){
	char *word;
	char *labelName;
	char commandName[32];
	int isExtern = 0;
	int isEntry = 0;
	int tempDC;
	int tempIC;
	int guidanceCode;
	int operationCode;
	int firstPassError;
	word = getFirstWord(line);
	if (shouldSkipLine(word)){
		return;
	}
	else if(isLegalLabel(word) == LEGAL_FIRST_WORD){
		labelName = word;
		if(symbolExist(labelName) == 1){
			addError(DuplicateSymbolError, fileName, lineNumber);
			return;
		}
		word = getNextWord(word);
		guidanceCode = getGuidanceCode(word);
		operationCode = getOperationCode(word);
		if(guidanceCode != FALSE){
			tempDC = dc;
			word = getGuidanceLine(word);
			readGuidance(word, guidanceCode);
			isExtern = (guidanceCode == IS_EXTERN);
			addToSymbolList(labelName, tempDC, 1, isExtern , isEntry);
		}
		else if(operationCode != FALSE){
			tempIC = ic;
			strcpy(commandName , word);
			word = getNextWord(word);
			readOperation(word, operationCode, commandName);
			addToSymbolList(labelName, tempIC, 0, 0, isEntry);
		}
		else{
			addError(UnknownCommandInLine, fileName, lineNumber);
		}
	}
	else{
		guidanceCode = getGuidanceCode(word);
		operationCode = getOperationCode(word);
		if (guidanceCode != FALSE){
			word = getGuidanceLine(word);
			readGuidance(word, guidanceCode);
		}
		else if (operationCode != FALSE){
			strcpy(commandName , word);
			word = getNextWord(word);
			readOperation(word, operationCode, commandName);
		}
		else{
			addError(UnknownSymbol, fileName, lineNumber);
		}
	}
	firstPassError = printErrorList();
	if(firstPassError == 0 ){
		exit(0);
	}
}

void print_symbleList(){
	SymbolList *temp;
	temp = symbolHead;
	while(temp != NULL){
		printf("\nfile name: %s\n", temp->filename);
		printf("symbol name: %s\n", temp->name);
		printf("symbol address: %d\n", temp->address);
		printf("symbol base address: %d\n", temp->baseAddress);
		printf("symbol offset: %d\n", temp->offset);
		printf("symbol isEntry: %d\n", temp->isEntry);
		printf("symbol isExtern: %d\n", temp->isExtern);
		printf("symbol isGuidance: %d\n", temp->isGuidance);
		temp = temp->next;
	}
}

void print_commandList(){
	CommandsList *temp;
	temp = commandHead;
	while(temp != NULL){
		printf("\ncommand operationLabel:%s\n", temp->operationLabel);
		printf("command fileName:%s\n", temp->fileName);
		printf("command address:%d\n", temp->address);
		printf("command code:%d\n", temp->code);
		printf("command funct:%d\n", temp->funct);
		printf("command destArg:%s\n", temp->destArg);
		printf("command destMethod:%d\n", temp->destMethod);
		printf("command lineNumber:%d\n", temp->lineNumber);
		printf("command required_args:%d\n", temp->required_args);
		printf("command srcArg:%s\n", temp->srcArg);
		printf("command srcMethod:%d\n", temp->srcMethod);
		temp = temp->next;
	}
}

void print_dataList(){
	DataList *temp;
	temp = dataHead;
	while(temp != NULL){
		printf("\ndata address:%d\n", temp->address);
		printf("data fileName:%s\n", temp->fileName);
		printf("data code:%d\n", temp->code);
		printf("data lineNumber:%d\n", temp->lineNumber);
		temp = temp->next;
	}
}