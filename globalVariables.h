#ifndef globalVeriables_h
#define globalVeriables_h

#include "structures.h"

CommandsList *   commandHead,    * commandTail;
DataList *       dataHead,       * dataTail;
SymbolList *     symbolHead,     * symbolTail;
ErrorList *      errorHead,      * errorTail;

int ic;
int dc;
int lineNumber;
int secondRunErrors;
char *fileName;

char *COMMANDS[16];
char *GUIDANCE[4];
char *REGISTER[16];
char *REGISTER_ADDRESS[16];

char *funct2[2];
char *funct5[4];
char *funct9[3];
#endif

