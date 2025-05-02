#ifndef MAMAN14_ERRORS_H
#define MAMAN14_ERRORS_H

/*
*   symbols that presented when error deducted while the assembler works
*/

#define IOError "IOError: Cannot open file " /*failes to open file with fopen*/
#define DuplicateSymbolError "SymbolError: symbol already declared" /*use in the same name for a lable*/
#define SymbolNotFoundError "SymbolError: symbol not found in table" /*cannot find the symbol in the initilized symbol table */
#define IllegalAddressingMethod "Illegal addressing Method" /*cannot find the operand on of four adress method*/
#define NumberConversionFailed "Number conversion failed"
#define UnknownCommandInLine "Unknown command after label"/*not legal operation*/
#define UnknownSymbol "UnknownWord: word is not a command or a guidance"/*first wors in line is ilegal*/
#define IllegalLabelError "Illegal label"/*Illegal label syntax*/
#define MissingCommaError "expected a \'  "
#define AdditionalSymbolError "Too many symbol for command"/*Illegal number of lables in operation*/
#define IllegalDataFormat " Expected a number after ',' "/*Illegal use in immideate adress*/
#define IllegalCommandFormat "Illegal command format"/*Illegal use in operation*/
#define IllegalString "Illegal string" /*Illegal string defenition*/
#define IllegalNumberError "Illegal Number" /*Illegal number defenition*/
#endif

