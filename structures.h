#ifndef structures_h
#define structures_h

/*
*	this data structure holds the operations are used in the assembly program
*/
typedef struct commandsList
{
	char operationLabel[100];
    int address;
    int code;
    int funct;
    int required_args;
    int srcMethod;
    int destMethod;
    char srcArg [100];
    char destArg [100];
    char fileName [100];
    int lineNumber;
    struct commandsList* next;
}CommandsList;

/*
*	this data structure holds the data are used in the assembly program
*/
typedef struct dataList
{
    int address;
    int code;
    char fileName [100];
    int lineNumber;
    struct dataList* next;
}DataList;

/*
*	this data structure holds the symbols are used in the assembly program
*/
typedef struct symbolList
{
    char name [100];
    int address;
    int isGuidance; /* 0 if not 1 if yes 2 if unknown*/
    int isEntry;
    int isExtern;
    char filename [100];
    int baseAddress;
    int offset;
    char *attribute;
    struct symbolList* next;
}SymbolList;

/*
*	this data structure holds the errors founded in the assembly program
*/
typedef struct errorList
{
    char fileName [100];
    int lineNum;
    char errorName [100];
    struct errorList *next;
}ErrorList;

SymbolList * getSymbol(char *symbol);
void addError(char *error, char *filename, int lineNum);

#endif