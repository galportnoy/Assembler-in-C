#include "stdio.h"
#include "string.h"
#include "globals.h"
#include "globalVariables.h"
#include "utils.h"

/*
*	checks whether the line / file ended or it a comment line , or an guidnce sentence which relevant to second pass
*/
int shouldSkipLine(char *word){
	return (isEOF(word) || isEndOfLine(word) || isCommentSentence(word) || isEntryGuidance(word));
}


/*
*	checks whether the word is EOF
*/
int isEOF(char *word){
	return (word == NULL);
}

/*
*	checks whether the word is EOF
*/
int isEndOfLine(char *word){
	return (strcmp(word, "\n") == 0);
}

/*
*	checks whether the line is comment line
*/
int isCommentSentence(char *word){
	return (strcmp(word, ";") == 0);
}

/*
*	checks whether the line is entry guidnce line
*/
int isEntryGuidance(char *word){
	return (strcmp(word, ".entry") == 0);
}

/* 
 *	check if word is saved of the assembly syntax
 */
int wordIsSave(char *label){
	int i=0;
	for(i=0; i < 16; i++){
		if (strcmp(label, REGISTER[i]) == 0){ /* check if register */
			return 1;
		}
		i++;
	}
	for(i=0; i < 4 ; i++){
		if (strcmp(label, GUIDANCE[i]) == 0){ /* check if guidance */
			return 1;
		}
		i++;
	}
	for(i=0; i < 16; i++){
		if (strcmp(label, COMMANDS[i]) == 0){ /* check if register*/
			return 1;
		}
		i++;
	}
	return 0;
}


/**
 * check if label is legal
 * return 0 if illegal
 * return 1 if legal
 * return 2 if legal and end with char ':' (start of sentence)
 */
int isLegalLabel(char *label){
	char *tempLabel;
	int i;
	int length;
	length = (int) strlen(label);
	tempLabel = label;
	if (wordIsSave(label) == 1){
		return ERROR;
	}
	for (i = 0; i < length; i++){
		if (i == 0 && charIsAlpha(*tempLabel) == 0){  /*first char is not alphabet*/
			return 0;
		}
		else if (i == length - 1 && *tempLabel == ':'){  /* if label is at start of sentence */
			return 2; /* Is command line */
		}
		else if ((i == length - 1 && (charIsAlpha(*tempLabel) == 1 || charIsDigit(*tempLabel) == 1))
		         || *tempLabel == '\r' || *tempLabel == '\n'){  /* if last char is number or alphabet */
			return 1;
		}
		else if (charIsAlpha(*tempLabel) == 0 && charIsDigit(*tempLabel) == 0){ /* if char is not a number or letter*/
			return 0;
		}
		else{
			tempLabel = tempLabel + 1;
		}
	}
	return 0;
}

/**
 * check if the char is digit
 * return 1 if yes or 0 if not
 */
bool charIsDigit(char n){
	if (n >= '0' && n <= '9'){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

/**
 * check if the char is alphabet
 * return 1 if yes or 0 if not
 */
bool charIsAlpha(char c){
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

/**
 * Check if the symbol already in the symbol table.
 * Return 1 if it exists, 0 otherwise.
 */
int symbolExist(char *symbolName){
	SymbolList *tempHead;
	tempHead = symbolHead;
	while (tempHead != NULL){
		if (strcmp(tempHead->name, symbolName) == 0){
			return 1;
		}
		else
			tempHead = tempHead->next;
	}
	return -1;
}

/**
 * promote the pointer to the next word (deny whitespace)
 * return pointer to the next word or NULL if not exist
 */
char *getNextWord(char *word){
	char *delim = " \t,";
	word = strtok(NULL, delim);
	return word;
}

/* check if word is guidance sentence 
 * return 1 if .data
 * return 2 if .string
 * return 3 if .extern
 * return 4 if .entry
 * return -1 if not guidance sentence  */
int getGuidanceCode(char *word){
	int i=0;
	for(i=0; i < 4 ; i++){
		if(strcmp(word,GUIDANCE[i]) == 0){
			return i+1;
		}
	}
	return FALSE;
}

/**
 * check the operation and return the number of the operation
 * return 0 if mov, 1 if cmp, ..., 15 if stop , -1 if not operation
 */
int getOperationCode(char *word){
	int i=0 ,length;
	length = strlen(word);

	if (word[length - 2] == '\r')
		word[length - 2] = '\0';
	
	else if (word[length - 1] == '\n')
		word[length - 1] = '\0';
	for(i=0; i < 16 ; i++){
		if (strcmp(word, COMMANDS[i]) == 0){
			break;
		}
	}
		if(i == 3){
			return 2;
		}
		else if(i >= 6 && i <= 8){
			return 5;
		}
		else if(i == 10 || i == 11){
			return 9;
		}
		else if(i == 16){
			return FALSE;
		}
	
	return i;
}

/**
 * promote the pointer to the start of the parameters
 */
char *getGuidanceLine(char *word){
	char *delim = " \t,";
	word = strtok(NULL, delim);
	return word;
}

/**
 * read guidance sentence
 */
void readGuidance(char *word, int guidanceCode){
	if (guidanceCode == DATA){
		readData(word);
	}
	else if (guidanceCode == STRING){
		readString(word);
	}
	else{
		readExtern(word);
	}
}

/**
 * add a node to the end of the symbol list and update the tail to the end
 */
void addToSymbolList(char *name, int address, int isGuidance, int isExtern, int isEntry){
	SymbolList *tempSymbol;
	tempSymbol = (SymbolList *) malloc(sizeof(SymbolList));
	strcpy(tempSymbol->name, name);
	tempSymbol->address = address;
	tempSymbol->offset = 0;
	tempSymbol->baseAddress = 0;
	tempSymbol->isGuidance = isGuidance;
	tempSymbol->isExtern = isExtern;
	tempSymbol->isEntry = isEntry;
	strcpy(tempSymbol->filename, fileName);
	tempSymbol->next = NULL;
	if (symbolHead == NULL){
		symbolHead = tempSymbol;
		symbolTail = symbolHead;
		return;
	}
	symbolTail->next = tempSymbol;
	symbolTail = tempSymbol;
}

/**
 * this function read operation sentence
 */
void readOperation(char *word, int operationCode, char *label){
	if (0 <= operationCode && operationCode <= 4){/*operation with two args*/
		TwoArgsOperation(word, operationCode, label);
	}
	else if (5 <= operationCode && operationCode <= 13){/*operation with one arg*/
		OneArgOperation(word, operationCode, label);
	}
	else{ /*operation without args*/
		NoneArgsOperation(operationCode, label);
	}
}

/**
 * this function get the funct according to defenition of the operation
 */
int getFunct(char *opLabel , int operationCode){
	int i;
	if(operationCode == 2){
		for(i=0; i < 2; i++){
			if(strcmp(opLabel , funct2[i]) == 0){
				if(i == 0){
					return 10; 
				}
				else{
					return 11;
				}
			}
		}
	}
	else if(operationCode == 5){
		for(i=0; i < 4; i++){
			if(strcmp(opLabel , funct5[i]) == 0){
				if(i == 0){
					return 10; 
				}
				else if(i == 1){
					return 11;
				}
				else if(i == 2){
					return 12;
				}
				else{
					return 13;
				}
			}
		}
	}
	else if(operationCode == 9){
		for(i=0; i < 3; i++){
			if(strcmp(opLabel , funct9[i]) == 0){
				if(i == 0){
					return 10; 
				}
				else if(i == 1){
					return 11;
				}
				else{
					return 12;
				}
			}
		}
	}
	return FALSE;
}


/**
 * this function read operation sentence with 2 argument operation and check if the operand are match to the opertion
 * stores all the relevant info of the operations and operands in the code
 * finds errors and put them as well
 */
void TwoArgsOperation(char *word, int operationCode, char *opLabel){
	char *srcArg;
	char *destArg;
	int srcMethod;
	int destMethod;
	int lastWord;
	int funct;
	srcArg = word;
	destArg = getNextArg(word);
	srcMethod = getAddressReallocationMethod(srcArg);
	destMethod = getAddressReallocationMethod(destArg);
	lastWord = isLastWord(word);
	funct = getFunct(opLabel , operationCode);
	if (srcMethod != ERROR && destMethod != ERROR && lastWord == TRUE){
		if (operationCode == 0 || operationCode == 2){
			if (srcMethod == 0){
				if(destMethod != 3){
					addToCommandList(ic, operationCode, 2, srcArg, destArg, srcMethod, destMethod, opLabel, funct);
					ic += 2;
				}
				else if(destMethod == 0){
					addError(IllegalAddressingMethod, fileName, lineNumber);
				}
				addToCommandList(ic, operationCode, 2, srcArg, destArg, srcMethod, destMethod, opLabel, funct);
				ic += 1;
			}
			else if (srcMethod == 1 || srcMethod == 2){
				if(destMethod != 3){
					addToCommandList(ic, operationCode, 2, srcArg, destArg, srcMethod, destMethod, opLabel, funct);
					ic += 2;
				}
				else if(destMethod == 0){
					addError(IllegalAddressingMethod, fileName, lineNumber);
				}
				addToCommandList(ic, operationCode, 2, srcArg, destArg, srcMethod, destMethod, opLabel, funct);
				ic += 2;
			}
			else if (srcMethod == 3){
				if(destMethod != 3){
					addToCommandList(ic, operationCode, 2, srcArg, destArg, srcMethod, destMethod, opLabel, funct);
					ic += 2;
				}
				else if(destMethod == 0){
					addError(IllegalAddressingMethod, fileName, lineNumber);
				}
				else{
					addToCommandList(ic, operationCode, 2, srcArg, destArg, srcMethod, destMethod, opLabel, funct);
				}
			}
		}
		else if (operationCode == 1){
			if (srcMethod == 0){
				if(destMethod == 0){
					addToCommandList(ic, operationCode, 2, srcArg, destArg, srcMethod, destMethod, opLabel, funct);
					ic += 1;
				}
				else if(destMethod == 1 || destMethod == 2){
					addToCommandList(ic, operationCode, 2, srcArg, destArg, srcMethod, destMethod, opLabel, funct);
					ic += 2;
				}
				ic += 1;
			}
			else if (srcMethod == 1 || srcMethod == 2){
				if(destMethod == 0){
					addToCommandList(ic, operationCode, 2, srcArg, destArg, srcMethod, destMethod, opLabel, funct);
					ic += 1;
				}
				else if(destMethod == 1 || destMethod == 2){
					addToCommandList(ic, operationCode, 2, srcArg, destArg, srcMethod, destMethod, opLabel, funct);
					ic += 2;
				}
				ic += 2;
			}
			else if (srcMethod == 3){
				if(destMethod == 0){
					addToCommandList(ic, operationCode, 2, srcArg, destArg, srcMethod, destMethod, opLabel, funct);
					ic += 1;
				}
				else if(destMethod == 1 || destMethod == 2){
					addToCommandList(ic, operationCode, 2, srcArg, destArg, srcMethod, destMethod, opLabel, funct);
					ic += 2;
				}
			}
		}
		else{ /* opcode 4 */
			if (srcMethod == 1 || srcMethod == 2){
				if(destMethod == 1 || destMethod == 2){
					addToCommandList(ic, operationCode, 1, NOT_USED_STRING, word, NOT_USED_INT, destMethod, opLabel, funct);
					ic += 2;
				}
				else if(destMethod == 0){
					addError(IllegalAddressingMethod, fileName, lineNumber);
				}
				addToCommandList(ic, operationCode, 2, srcArg, destArg, srcMethod, destMethod, opLabel, funct);
				ic += 2;
			}
			else{
				addError(IllegalAddressingMethod, fileName, lineNumber);
			}
		}
	}
	else{
		if (lastWord == FALSE){
			addError(IllegalCommandFormat, fileName, lineNumber);
		}
		else{
			addError(IllegalAddressingMethod, fileName, lineNumber);
		}
	}
	ic += 2;
}

/**
 * this function read operation sentence with 1 argument operation and check if the operand are match to the opertion
 * stores all the relevant info of the operations and operands in the code
 * finds errors and put them as well
 */
void OneArgOperation(char *word, int operationCode, char *opLabel){
	int destMethod;
	int lastWord;
	int funct;
	destMethod = getAddressReallocationMethod(word);
	lastWord = isLastWord(word);
	funct = getFunct(opLabel , operationCode);
	if (destMethod == ERROR){
		addError(IllegalAddressingMethod, fileName, lineNumber);
	}
	else if (lastWord == FALSE){
		addError(IllegalCommandFormat, fileName, lineNumber);
	}
	else if (operationCode == 5 || operationCode == 12){
		if (destMethod == 0){
			addError(IllegalAddressingMethod, fileName, lineNumber);
		}
		if(destMethod == 1 || destMethod == 2){
			addToCommandList(ic, operationCode, 1, NOT_USED_STRING, word, NOT_USED_INT, destMethod, opLabel, funct);
			ic += 2;
		}
		if(destMethod == 3){
			addToCommandList(ic, operationCode, 1, NOT_USED_STRING, word, NOT_USED_INT, destMethod, opLabel, funct);
		}
	}
	else if (operationCode == 9){
		if (destMethod == 2 || destMethod == 1){
			addToCommandList(ic, operationCode, 1, NOT_USED_STRING, word, NOT_USED_INT, destMethod, opLabel, funct);
			ic += 2;
		}
		else{
			addError(IllegalAddressingMethod, fileName, lineNumber);
		}
	}
	else if (operationCode == 13){
			if(destMethod == 0){
				addToCommandList(ic, operationCode, 1, NOT_USED_STRING, word, NOT_USED_INT, destMethod, opLabel, funct);
				ic += 1;
			}
			else if(destMethod == 1 || destMethod == 2){
				addToCommandList(ic, operationCode, 1, NOT_USED_STRING, word, NOT_USED_INT, destMethod, opLabel, funct);
				ic += 2;
			}
			else if(destMethod == 3){
			addToCommandList(ic, operationCode, 1, NOT_USED_STRING, word, NOT_USED_INT, destMethod, opLabel, funct);
		}
	}
	ic += 2;
}

/**
 * this function read operation sentence without argument operation
 */
void NoneArgsOperation(int operationCode, char *oplabel){
	int lastWord;
	int funct;
	lastWord = isLastWord(oplabel);
	funct = getFunct(oplabel , operationCode);
	if (lastWord == FALSE){
		addError(IllegalCommandFormat, fileName, lineNumber);
	}
	else{
		addToCommandList(ic, operationCode, 0, NOT_USED_STRING, NOT_USED_STRING, NOT_USED_INT, NOT_USED_INT, oplabel, funct);
		ic++;
	}
}

/**
 * this function check if word is register
 * return 1 if word is register 0 if not
 */
int isRegister(char *word){
	int i;
	for (i = 0; i < 16; i++){
		if (strcmp(word, REGISTER[i]) == 0){
			return 1;
		}
	}
	return 0;
}

/**
 * this function check if word is address of register
 * return 1 if word is address of register 0 if not
 */
int isRegisterAddress(char *word){
	int i, length;
	length = strlen(word);
	for (i = 0; i < 16; i++){
		if (strstr(word , REGISTER_ADDRESS[i])){
			if(word[length - 1] == ']'){
				return 1;
			}
			else{
				return 0;
			}
		}
	}
	return 0;
}

/**
 * this function read extern sentence
 */
void readExtern(char *word){
	char labelName[80];
	int i=0;
	while (*word != ' ' && *word != '\t' && *word != '\r' && *word != '\n' && *word != '\0'){
		labelName[i] = *word;
		word = word + 1;
		i++;
	}
	labelName[i] = '\0';
	if (isLegalLabel(labelName) == LEGAL && symbolExist(labelName) == -1){
		addToSymbolList(labelName, 0, 0, 1, 0);
	}
	else{
		addError(IllegalLabelError, fileName, lineNumber);
	}
}


/**
 * this function read .string  guidnce sentence
 */
void readString(char *word){
	int i=0, length;
	char *delim = "\"";
	char *token;
	token = strtok(word , delim);
	length = strlen(token);
	for (i = 0; i < length + 1; i++){
		addToDataList(dc, (int) token[i], TRUE);
		dc++;
	}
}

/**
 * this read the numbers and insert them to the array
 */
void readData(char *str){
	int number[MAX_LINE_LENGHT];
	int i;
	int k;
	int tempNum;
	i = 0;
	k = 0;
	while (str != NULL){
		while (*str != '\0' && (strcmp(str, "\r\n") != 0) && (*str != '\n')){
			tempNum = readNextNum(&str);
			if (tempNum != ERROR){
				number[i] = tempNum;
				i++;
			}
			else{
				addError(IllegalNumberError, fileName, lineNumber);
			}
		}
		str = getNextWord(str);
	}
	while (k < i){
		addToDataList(dc, number[k], FALSE);
		dc++;
		k++;
	}
}


/**
 * read the next number in the string
 * return the number or ERROR (-9999)
 */
int readNextNum(char **str){
	char number[80];
	int i;
	int num;
	int status; /* 0 mean unknown 1 mean positive number 2 mean negative number*/
	char *currChar;
	int flagWhiteSpace;
	int flagNum;
	int length; /*number of char we already read*/
	length = 0;
	flagNum = 0;
	flagWhiteSpace = 0;
	status = 0;
	i = 0;
	currChar = *str;
	while (*currChar != ',' && *currChar != '\n' && *currChar != '\r' && *currChar != '\0'){
		if (charIsDigit(*currChar)){
			if (flagWhiteSpace == 1 && status != 0){
				addError(MissingCommaError, fileName, lineNumber);
				return ERROR;
			}
			if (status == 0){
				status = 1;
			}
			length++;
			flagNum = 1;
			number[i] = *currChar;
			currChar = currChar + 1;
			i++;
		}
		else if (*currChar == '+' || *currChar == '-'){
			if (status != 0){
				addError(AdditionalSymbolError, fileName, lineNumber);
				return ERROR;
			}
			else if (*currChar == '+'){
				status = 1;
			}
			else{
				status = 2;
			}
			length++;
			currChar++;
		}
		else if (*currChar == ' ' || *currChar == '\t'){
			flagWhiteSpace = 1;
			length++;
			currChar++;
		}
		else{
			addError(IllegalLabelError, fileName, lineNumber);
			return ERROR;
		}
	}
	if (*currChar == ','){
		*str = *str + 1;
		if (flagNum == 1){
			if (**str == '\0'){
				addError(IllegalDataFormat, fileName, lineNumber);
				return ERROR;
			}
		}
		else{
			addError(AdditionalSymbolError, fileName, lineNumber);
			return ERROR;
		}
	}
	number[i] = '\0';
	num = atoi(number);
	*str = *str + length;
	if (status == 1)
		return num;
	else
		return num * (-1);
}

/**
 * this function reset the string to zero
 */
void resetLine(char *line){
	int i;
	for (i = 0; i < MAX_LINE_LENGHT; i++)
		line[i] = 0;
	
}


/**
 * this function add a node to the end of the command list and update the tail to the end
 */
void addToCommandList(int address, int code, int required_args, char *srcArg, char *destArg, int srcMethod, int destMethod, char *label, int funct){
	CommandsList *tempCommand;
	tempCommand = (CommandsList *) malloc(sizeof(CommandsList));
	tempCommand->address = address;
	tempCommand->code = code;
	tempCommand->required_args = required_args;
	tempCommand->next = NULL;
	strcpy(tempCommand->operationLabel, label);
	strcpy(tempCommand->destArg, destArg);
	strcpy(tempCommand->srcArg, srcArg);
	tempCommand->destMethod = destMethod;
	tempCommand->srcMethod = srcMethod;
	strcpy(tempCommand->fileName, fileName);
	tempCommand->lineNumber = lineNumber;
	tempCommand->funct = funct;
	if (commandHead == NULL){
		commandHead = tempCommand;
		commandTail = commandHead;
		return;
	}
	commandTail->next = tempCommand;
	commandTail = tempCommand;
}

/**
 * this function add a node to the end of the data list and update the tail to the end
 */
void addToDataList(int address, int code, int isString){
	DataList *tempData;
	tempData = (DataList *) malloc(sizeof(DataList));
	tempData->address = address;
	tempData->code = code;
	tempData->next = NULL;
	strcpy(tempData->fileName, fileName);
	tempData->lineNumber = lineNumber;
	if (dataHead == NULL){
		dataHead = tempData;
		dataTail = dataHead;
		return;
	}
	dataTail->next = tempData;
	dataTail = tempData;
}


/**
 * this function return 1 if word is last in line -1 otherwise
 */
int isLastWord(char *word){
	char delim[2];
	delim[0] = ' ';
	delim[1] = '\t';
	word = strtok(NULL, delim);
	if (word == NULL || strcmp(word," ")==0 || strcmp(word,"\t")==0 ){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

/**
 * this function return the next arg
 */
char *getNextArg(char *word){
	char delim[3];
	delim[0] = ' ';
	delim[1] = '\t';
	delim[2] = ',';
	word = strtok(NULL, delim);
	return word;
}


/**
 * this function return addressing method
 */
int getAddressReallocationMethod(char *word){
	int length;
	length = strlen(word);
	if (word[length - 2] == '\r'){
		word[length - 2] = '\0';
	}
	else if (word[length - 1] == '\n'){
		word[length - 1] = '\0';
	}
	if (isRegister(word) == TRUE){
		return 3;
	}
	if (isDirectNumber(word) == TRUE){
		return 0;
	}
	if (isLegalLabel(word) == TRUE){
		return 1;
	}
	if (isRegisterAddress(word) == TRUE){
		return 2;
	}
	return ERROR;
}

/**
 this function Check for Immediate addressing method
 */
int isDirectNumber(char *word){
	char *tempWord;
	tempWord = word;
	if (*word == '#'){
		tempWord = tempWord + 1;
		if (isNumber(tempWord)){
			return 1;
		}
	}
	return ERROR;
}

/**
 * this function check if the word is number
 * return 1 if yes ERROR otherwise
 */
int isNumber(char *word){
	while (*word != ' ' && *word != '\t' && *word != '\n'){
		if (!charIsDigit(*word)){
			return ERROR;
		}
		word = word + 1;
	}
	return 1;
}

void initLists(){
	commandHead = NULL;
	symbolHead = NULL;
	dataHead = NULL;
	errorHead = NULL;
}

/**
 * this function initilize the global counters
 */
void initCounters(){
	dc = 0;
	ic = IC_VALUE;
}

/* 
 *this function delete the garbage chars 
 */
char* file_name(char* path){
    int length;
	char *ptr;
	length = strlen(path);
	ptr = malloc(sizeof(char)*length + 3);
	strcpy(ptr, path);
	strcat(ptr, ".am");
	return ptr;
}

/**
 *  this function after first pass its update the adress of stored data acording to instructions sentences founded in program
 */
void updateSymbolList(){
	SymbolList *curr;
	curr = symbolHead;
	while (curr != NULL){
		if (curr->isGuidance == 1){
			curr->address += ic;
			curr->offset = getOffSet(curr->address);
			curr->baseAddress = getBaseAddress(curr->address ,curr->offset);
		}
		else{
			curr->offset = getOffSet(curr->address);
			curr->baseAddress = getBaseAddress(curr->address ,curr->offset);
		}
		curr = curr->next;
	}
}


/**
 *  this functionedit the lable name as needed
 */
void fixLabelName(){
	SymbolList *curr;
	char *currLabel;
	curr = symbolHead;

	while (curr != NULL){
		currLabel = curr->name;
		while (*currLabel != '\0'){
			if (*currLabel == ':' || *currLabel == '\r' || *currLabel == '\n'){
				*currLabel = '\0';
				break;
			}
			currLabel++;
		}
		curr = curr->next;
	}
}

/**
 * this function edit the operation name as needed
 */
void fixOperationLabel(){
	CommandsList *curr;
	char *opLabel;
	curr = commandHead;
	while (curr != NULL){
		opLabel = curr->operationLabel;
		while (*opLabel != '\0'){
			if (*opLabel == ':' || *opLabel == '\r' || *opLabel == '\n'){
				*opLabel = '\0';
				break;
			}
			opLabel++;
		}
		curr = curr->next;
	}
}

/**
 * this function update after first pass the adress of stored data acording to instructions sentences founded in program
 */
void updateDataAddress(){
	DataList *curr;
	curr = dataHead;
	while (curr != NULL){
		curr->address += ic;
		curr = curr->next;
	}
}

/**
 * this func checks if its not guidnce line its skip this line
 * if its a guidnce line as entry guidnce it stores the info in entry guidnce table in order to check the lables 
 */
int shouldSkipLineSecondPass(char *word){
	int type;
	type = getGuidanceCode(word);
	if (type != FALSE){
		if (type == ENTRY){
			return FALSE;
		}
		else{
			return TRUE;
		}
	}
	return FALSE;
}

/**
 * this func add a given symbole into the symbol table
 */
void addEntryToSymbols(char *symbolName, int lineNum){
	SymbolList *symbol;
	symbol = getSymbol(symbolName);
	if (symbol == NULL){
		addError(SymbolNotFoundError, fileName, lineNum);
	}
	else{
		symbol->isEntry = TRUE;
	}
}

/* return base Address in %16 */
int getOffSet(int address){
	return address % 16;
}

/* return offset */
int getBaseAddress(int address , int offset){
	return address - offset;
}


char* getFirstWord(char *line){
	const char delim[3] = ", \t";
	char *word;
	word = strtok(line, delim);
	return word;
}