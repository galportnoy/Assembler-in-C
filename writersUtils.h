#ifndef writersUtils_h
#define writersUtils_h

void getARE(int addressMethod, int isExtern, char *filename, int linenumber, int binary[]);
void convertToTwosComplement(int binary[]);
void getAddressingMethodInBinary(int method, char *filename, int linNumber, int binary[], int src);
void convertToTwosComplement(int binary[]);
int getNumberFromArg(char *arg, CommandsList *node);
int getRegisterNumber(char *arg, int direct);
void getBasename(char *orig, char *resolved);
void printData(FILE *fp);
void printEntry(FILE *fp);
void printExtern(FILE *fp);
void finalize(FILE *obj, FILE *ent, FILE *ext, char *objName, char *extName, char *entName, int hasErrors);
void translateOpCode(int decimal, int binary[]);
void writeObjLine(FILE *fp, char *specialBase, int address);
void writeExtLine(FILE *fp, char *name, int address , char *word);
void writeEntLine(FILE *fp, char *name, int baseAddress, int offset);
int handleArgument(CommandsList *node, int isSrcArg, FILE *obj, FILE *ext, int twoRegisters, int hasTwoArgs);
int checkIfTwoRegisters(CommandsList *node);
char* convertToHexadecimal(int binary[]);
char ifHexa(int sum);
int intToBinary(int num);
void putFunct(int funct , int binary[]);
void putRegister(int binary[] , char *srcArg, int status);
void binaryReset(int binary[]);
void convertAddressToBinary(int baseAddress, int binary[]);
char* cutToLabel(char *arg);
char* cutToRgister(char *arg);
void convertStrToBinary(char *arg , int binary[]);
void updateBase(SymbolList *label , int lineNumber);
void updateOffset(SymbolList *label , int lineNumber);
#endif

