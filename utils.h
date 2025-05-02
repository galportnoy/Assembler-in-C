#ifndef utils_h
#define utils_h

/*
*	checks whether the line / file ended or it a comment line , or an guidnce sentence which relevant to second pass
*/
int shouldSkipLine(char *word);

/*
*	checks whether the word is EOF
*/
int isEOF(char *word);

/*
*	checks whether the word is EOF
*/
int isEndOfLine(char *word);

/*
*	checks whether the line is comment line
*/
int isCommentSentence(char *word);

/*
*	checks whether the line is entry guidnce line
*/
int isEntryGuidance(char *word);

/* 
 *	check if word is saved of the assembly syntax
 */
int wordIsSave(char *label);

/**
 * check if label is legal
 * return 0 if illegal
 * return 1 if legal
 * return 2 if legal and end with char ':' (start of sentence)
 */
int isLegalLabel(char *label);

/**
 * check if the char is digit
 * return 1 if yes or 0 if not
 */
int charIsDigit(char n);

/**
 * check if the char is alphabet
 * return 1 if yes or 0 if not
 */
int charIsAlpha(char c);

/**
 * Check if the symbol already in the symbol table.
 * Return 1 if it exists, 0 otherwise.
 */
int symbolExist(char *symbolName);

/**
 * promote the pointer to the next word (deny whitespace)
 * return pointer to the next word or NULL if not exist
 */
char *getNextWord(char *word);

/* check if word is guidance sentence 
 * return 1 if .data
 * return 2 if .string
 * return 3 if .extern
 * return 4 if .entry
 * return -1 if not guidance sentence  */
int getGuidanceCode(char *word);

/**
 * check the operation and return the number of the operation
 * return 0 if mov, 1 if cmp, ..., 15 if stop , -1 if not operation
 */
int getOperationCode(char *word);

/**
 * promote the pointer to the start of the parameters
 */
char *getGuidanceLine(char *word);

/**
 * add a node to the end of the symbol list and update the tail to the end
 */
void addToSymbolList(char *name, int address, int isGuidance, int isExtern, int isEntry);

/**
 * this function read operation sentence
 */
void readOperation(char *word, int operationCode, char *label);

/**
 * read operation sentence with 2 argument operation and check if the operand are match to the opertion
 * stores all the relevant info of the operations and operands in the code
 * finds errors and put them as well
 */
void TwoArgsOperation(char *word, int operationCode, char *label);

/**
 * read operation sentence with 1 argument operation and check if the operand are match to the opertion
 * stores all the relevant info of the operations and operands in the code
 * finds errors and put them as well
 */
void OneArgOperation(char *word, int operationCode, char *label);

/**
 * this function read operation sentence without argument operation
 */
void NoneArgsOperation(int operationCode, char *label);

/**
 * this function check if word is register
 * return 1 if word is register 0 if not
 */
int isRegister(char *word);

/**
 * this function check if word is address of register
 * return 1 if word is address of register 0 if not
 */
int isRegisterAddress(char *word);

/**
 * this function read extern sentence
 */
void readExtern(char *word);

/**
 * this function read .string  guidnce sentence
 */
void readString(char *word);

/**
 * this read the numbers and insert them to the array;
 */
void readData(char *str);

/**
 * read the next number in the string
 * return the number or ERROR (-9999)
 */
int readNextNum(char **str);

/**
 * this function reset the string to zero
 */
void resetLine(char *line);

/**
 * this function add a node to the end of the command list and update the tail to the end
 */
void addToCommandList(int address, int code, int required_args, char *srcArg, char *destArg, int srcMethod, int destMethod, char *label, int funct);

/**
 * this function add a node to the end of the data list and update the tail to the end
 */
void addToDataList(int address, int code, int isString);

/**
 * this function return the next arg
 */
int isLastWord(char *word);

/**
 * this function return the next arg
 */
char *getNextArg(char *word);

/**
 * this function return addressing method
 */
int getAddressReallocationMethod(char *word);

/**
 this function Check for Immediate addressing method
 */
int isDirectNumber(char *word);

/**
 * this function check if the word is number
 * return 1 if yes ERROR otherwise
 */
int isNumber(char *word);


void initLists();

/**
 * this function initilize the global counters
 */
void initCounters();


void readGuidance(char *word, int guidanceCode);

/* 
 *this function delete the garbage chars 
 */
char* file_name(char* path);

/**
 *  this function after first pass its update the adress of stored data acording to instructions sentences founded in program
 */
void updateSymbolList();

/**
 *  this functionedit the lable name as needed
 */
void fixLabelName();

/**
 * this function edit the operation name as needed
 */
void fixOperationLabel();

/**
 * this function update after first pass the adress of stored data acording to instructions sentences founded in program
 */
void updateDataAddress();

/**
 * this func add a given symbole into the symbol table
 */
void addEntryToSymbols(char *symbolName, int lineNum);

/**
 * this func checks if its not guidnce line its skip this line
 * if its a guidnce line as entry guidnce it stores the info in entry guidnce table in order to check the lables 
 */
int shouldSkipLineSecondPass(char *word);

/* return base Address in %16 */
int getOffSet(int address);

/* return offset */
int getBaseAddress(int address , int offset);

char* getFirstWord(char *line);


#endif