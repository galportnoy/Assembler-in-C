#include "assembler.h"

/*
*	this function create the out put files: entry , extern , hexadecimal coding , errors 
* according to data that was collected in the firs and second passes
*/
void writeOutputFiles()
{
	char baseFileName[FILENAME_MAX];
	char objFileName[FILENAME_MAX];
	char extFileName[FILENAME_MAX];
	char entFileName[FILENAME_MAX];
	CommandsList *currentNode;
	SymbolList *currentSymbol;

	FILE *objFile, *entFile, *extFile;
	
	int binaryString[WORD_LENGTH];
	char *ptrSpecial;
	char *filename;
	int linenumber;
	char *tokken;
	char* registerAfterCut;
	int isExternal;
	
	secondRunErrors = FALSE;

	getBasename(fileName, baseFileName);

	strcpy(objFileName, baseFileName);
	strcat(objFileName, ".ob");

	strcpy(extFileName, baseFileName);
	strcat(extFileName, ".ext");

	strcpy(entFileName, baseFileName);
	strcat(entFileName, ".ent");

	objFile = fopen(objFileName, "w");
	if(objFile == NULL){
		printf("can't open %s\n",objFileName);
	}
	

	entFile = fopen(entFileName, "w");
	if(entFile == NULL){
		printf("can't open %s\n",entFileName);
	}

	extFile = fopen(extFileName, "w");
	if(extFile == NULL){
		printf("can't open %s\n",extFileName);
	}

	fprintf(objFile, "\t%d %d\n", ic - IC_VALUE, dc);

	currentNode = commandHead;
	while (currentNode != NULL)
	{
		/*printf("test\n");
		Check if command is a label 
		if (strcmp(currentNode->operationLabel, NO_LABEL) != 0){
			char *ptrLabel , labelName[20];
			if(currentNode->required_args == 1){
				printf("test1\n");
				if(currentNode->destMethod == 2){
					printf("test2\n");
					printf("label name before cut is:%s\n", currentNode->destArg);
					ptrLabel = cutToLabel(currentNode->destArg);
					printf("label name after cut is:%s\n", ptrLabel);
					currentSymbol = getSymbol(ptrLabel);
					if (currentSymbol == NULL){
						addError(SymbolNotFoundError, currentNode->fileName, currentNode->lineNumber);
					}
				}
				else if(currentNode->destMethod == 1){
					strcpy(labelName , currentNode->destArg);
					ptrLabel = labelName;
					currentSymbol = getSymbol(currentNode->destArg);
					if (currentSymbol == NULL){
						addError(SymbolNotFoundError, currentNode->fileName, currentNode->lineNumber);
					}
				}
			}
			else if(currentNode->required_args == 2){
				printf("test3\n");
				if(currentNode->srcMethod == 2){
					printf("test4\n");
					ptrLabel = cutToLabel(currentNode->srcArg);
					currentSymbol = getSymbol(ptrLabel);
					if (currentSymbol == NULL){
						addError(SymbolNotFoundError, currentNode->fileName, currentNode->lineNumber);
					}
				}
				else if(currentNode->srcMethod == 1){
					currentSymbol = getSymbol(currentNode->srcArg);
					if (currentSymbol == NULL){
						addError(SymbolNotFoundError, currentNode->fileName, currentNode->lineNumber);
					}
				}
				else if(currentNode->destMethod == 2){
					printf("test5\n");
					printf("label name before cut is:%s\n", labelName);
					ptrLabel = cutToLabel(currentNode->destArg);
					printf("label name after cut is:%s\n", labelName);
					currentSymbol = getSymbol(ptrLabel);
					if (currentSymbol == NULL){
						addError(SymbolNotFoundError, currentNode->fileName, currentNode->lineNumber);
					}
				}
				else if(currentNode->destMethod == 1){
					strcpy(labelName , currentNode->destArg);
					printf("label name before cut is:%s\n", labelName);
					ptrLabel = labelName;
					printf("label name after cut is:%s\n", labelName);
					currentSymbol = getSymbol(ptrLabel);
					if (currentSymbol == NULL){
						addError(SymbolNotFoundError, currentNode->fileName, currentNode->lineNumber);
					}
				}
			}
		}*/

		filename = currentNode->fileName;
		linenumber = currentNode->address;
		binaryReset(binaryString);
		currentSymbol = symbolHead;

		/* write the instruction line to the ob.as file */
		translateOpCode(currentNode->code, binaryString);
		getARE(IMMEDIATE_ADDRESSING, FALSE, currentNode->fileName, currentNode->lineNumber, binaryString);
		ptrSpecial = convertToHexadecimal(binaryString);
		writeObjLine(objFile, ptrSpecial, linenumber);
		linenumber++;

		if(currentNode->code != 14 && currentNode->code != 15){
			binaryReset(binaryString);
			if(currentNode->funct != -1){
				putFunct(currentNode->funct , binaryString);
			}
			getARE(IMMEDIATE_ADDRESSING, FALSE, currentNode->fileName, currentNode->lineNumber, binaryString);
			getAddressingMethodInBinary(currentNode->srcMethod, filename, linenumber, binaryString, TRUE);
			if(currentNode->srcMethod == 3 ){
				putRegister(binaryString , currentNode->srcArg, TRUE);
			}
			if(currentNode->srcMethod == 2){
				registerAfterCut = cutToRgister(currentNode->srcArg);
				putRegister(binaryString , registerAfterCut, TRUE);
			}
			getAddressingMethodInBinary(currentNode->destMethod, filename, linenumber, binaryString, FALSE);
			
			if(currentNode->destMethod == 3){
				putRegister(binaryString , currentNode->destArg, FALSE);
			}
			if(currentNode->destMethod == 2){
				registerAfterCut = cutToRgister(currentNode->destArg);
				putRegister(binaryString , registerAfterCut, FALSE);
			}
			ptrSpecial = convertToHexadecimal(binaryString);
			writeObjLine(objFile, ptrSpecial, linenumber);
			binaryReset(binaryString);
			linenumber++;


/**************************************** one argument handle ***********************************************************/
			if(currentNode->required_args == 1){
				if(currentNode->destMethod == IMMEDIATE_ADDRESSING){
					convertStrToBinary(currentNode->destArg , binaryString);
					getARE(IMMEDIATE_ADDRESSING, FALSE, currentNode->fileName, currentNode->lineNumber, binaryString);
					ptrSpecial = convertToHexadecimal(binaryString);
					writeObjLine(objFile, ptrSpecial, linenumber);
					linenumber++;
				}
				else if(currentNode->destMethod == DIRECT_ADDRESSING){
					while(currentSymbol != NULL){
						if(strcmp(currentSymbol->name, currentNode->destArg) == 0){
							isExternal = currentSymbol->isExtern;
							if(isExternal == 1){
								getARE(DIRECT_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								updateBase(currentSymbol , linenumber);
								linenumber++;

								binaryReset(binaryString);
								getARE(DIRECT_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								updateOffset(currentSymbol , linenumber);
								linenumber++;
								break;

							}
							else{
								convertAddressToBinary(currentSymbol->baseAddress,binaryString);
								getARE(DIRECT_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								linenumber++;

								binaryReset(binaryString);

								convertAddressToBinary(currentSymbol->offset,binaryString);
								getARE(DIRECT_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								linenumber++;
								break;
							}
						}
						currentSymbol = currentSymbol->next;
					}
				}
				else if(currentNode->destMethod == INDIRECT_REGISTER_ADDRESSING){
					 tokken = cutToLabel(currentNode->destArg);
					 while(currentSymbol != NULL){
						if(strcmp(currentSymbol->name, tokken) == 0){
							isExternal = currentSymbol->isExtern;
							if(isExternal == 1){
								getARE(INDIRECT_REGISTER_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								updateBase(currentSymbol , linenumber);
								linenumber++;

								binaryReset(binaryString);
								
								getARE(INDIRECT_REGISTER_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								updateOffset(currentSymbol , linenumber);
								linenumber++;
								break;

							}
							else{
								convertAddressToBinary(currentSymbol->baseAddress,binaryString);
								getARE(INDIRECT_REGISTER_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								linenumber++;

								binaryReset(binaryString);

								convertAddressToBinary(currentSymbol->offset,binaryString);
								getARE(INDIRECT_REGISTER_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								linenumber++;
								break;
							}
						}
						currentSymbol = currentSymbol->next;
					}
				}
			}

/******************************************* Two argument handle (src) ******************************************************************/			
			if(currentNode->required_args == 2){
				if(currentNode->srcMethod == IMMEDIATE_ADDRESSING){
					convertStrToBinary(currentNode->srcArg , binaryString);
					getARE(IMMEDIATE_ADDRESSING, FALSE, currentNode->fileName, currentNode->lineNumber, binaryString);
					ptrSpecial = convertToHexadecimal(binaryString);
					writeObjLine(objFile, ptrSpecial, linenumber);
					linenumber++;
				}
				else if(currentNode->srcMethod == DIRECT_ADDRESSING){
					while(currentSymbol != NULL){
						if(strcmp(currentSymbol->name, currentNode->srcArg) == 0){
							isExternal = currentSymbol->isExtern;
							if(isExternal == 1){
								getARE(DIRECT_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								updateBase(currentSymbol , linenumber);
								linenumber++;

								binaryReset(binaryString);
								
								getARE(DIRECT_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								updateOffset(currentSymbol , linenumber);
								linenumber++;
								break;

							}
							else{
								convertAddressToBinary(currentSymbol->baseAddress,binaryString);
								getARE(DIRECT_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								linenumber++;

								binaryReset(binaryString);

								convertAddressToBinary(currentSymbol->offset,binaryString);
								getARE(DIRECT_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								linenumber++;
								break;
							}
						}
						currentSymbol = currentSymbol->next;
					}
				}
				else if(currentNode->srcMethod == INDIRECT_REGISTER_ADDRESSING){
					 tokken = cutToLabel(currentNode->srcArg);
					 while(currentSymbol != NULL){
						if(strcmp(currentSymbol->name, tokken) == 0){
							isExternal = currentSymbol->isExtern;
							if(isExternal == 1){
								getARE(INDIRECT_REGISTER_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								updateBase(currentSymbol , linenumber);
								linenumber++;

								binaryReset(binaryString);
								
								getARE(INDIRECT_REGISTER_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								updateOffset(currentSymbol , linenumber);
								linenumber++;
								break;

							}
							else{
								convertAddressToBinary(currentSymbol->baseAddress,binaryString);
								getARE(INDIRECT_REGISTER_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								linenumber++;

								binaryReset(binaryString);

								convertAddressToBinary(currentSymbol->offset,binaryString);
								getARE(INDIRECT_REGISTER_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								linenumber++;
								break;
							}
						}
						currentSymbol = currentSymbol->next;
					}
				}

/******************************************** Two argument hendle (dest) *********************************************************************/
				if(currentNode->destMethod == IMMEDIATE_ADDRESSING){
					convertStrToBinary(currentNode->destArg , binaryString);
					getARE(IMMEDIATE_ADDRESSING, FALSE, currentNode->fileName, currentNode->lineNumber, binaryString);
					ptrSpecial = convertToHexadecimal(binaryString);
					writeObjLine(objFile, ptrSpecial, linenumber);
					linenumber++;
				}
				else if(currentNode->destMethod == DIRECT_ADDRESSING){
					while(currentSymbol != NULL){
						if(strcmp(currentSymbol->name, currentNode->destArg) == 0){
							isExternal = currentSymbol->isExtern;
							if(isExternal == 1){
								getARE(DIRECT_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								updateBase(currentSymbol , linenumber);
								linenumber++;

								binaryReset(binaryString);
								
								getARE(DIRECT_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								updateOffset(currentSymbol , linenumber);
								linenumber++;
								break;

							}
							else{
								convertAddressToBinary(currentSymbol->baseAddress,binaryString);
								getARE(DIRECT_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								linenumber++;

								binaryReset(binaryString);

								convertAddressToBinary(currentSymbol->offset,binaryString);
								getARE(DIRECT_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								linenumber++;
								break;
							}
						}
						currentSymbol = currentSymbol->next;
					}
				}
				else if(currentNode->destMethod == INDIRECT_REGISTER_ADDRESSING){
					 tokken = cutToLabel(currentNode->destArg);
					 while(currentSymbol != NULL){
						if(strcmp(currentSymbol->name, currentNode->destArg) == 0){
							isExternal = currentSymbol->isExtern;
							if(isExternal == 1){
								getARE(INDIRECT_REGISTER_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								updateBase(currentSymbol , linenumber);
								linenumber++;

								binaryReset(binaryString);
								
								getARE(INDIRECT_REGISTER_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								updateOffset(currentSymbol , linenumber);
								linenumber++;
								break;

							}
							else{
								convertAddressToBinary(currentSymbol->baseAddress,binaryString);
								getARE(INDIRECT_REGISTER_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								linenumber++;

								binaryReset(binaryString);

								convertAddressToBinary(currentSymbol->offset,binaryString);
								getARE(INDIRECT_REGISTER_ADDRESSING, isExternal, currentNode->fileName, currentNode->lineNumber, binaryString);
								ptrSpecial = convertToHexadecimal(binaryString);
								writeObjLine(objFile, ptrSpecial, linenumber);
								linenumber++;
								break;
							}
						currentSymbol = currentSymbol->next;
						}
					}
				}
			}
		}
		currentNode = currentNode->next;
	}
	printData(objFile);
	printEntry(entFile);
	printExtern(extFile);
/*	secondRunErrors = printErrorList();
	if(secondRunErrors == 1){
		fclose(objFile);
		fclose(entFile);
		fclose(extFile);
	}
	else{
		finalize(objFile, entFile, extFile, objFileName, extFileName, entFileName, secondRunErrors);
	}*/
}

int printErrorList(){
	ErrorList *currentError;
	currentError = errorHead;
	if (currentError == NULL){/* no error in error list */
		return 1;
	}
	if (currentError != NULL){
		printf("Assembly failed: errors found during parsing. \n");
	}
	while (currentError != NULL){
		printf("%s found in %s:%d\n", currentError->errorName, currentError->fileName, currentError->lineNum);
		currentError = currentError->next;
	}
	return 0;
}
