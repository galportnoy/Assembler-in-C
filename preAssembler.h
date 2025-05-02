#ifndef preAssembler_h
#define preAssembler_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkList.h"

#define BITS 20
#define MEMORY 8192
#define MAX_LINE_LENGTH 82
#define S_MACRO "macro"
#define E_MACRO "endm"


typedef link_list macro_table;

/*this function check if the file exist in order to open it*/
int isFileExists(const char *path);

/*this function check if the given word is a start of declartion of macro*/
int checkMacro(char * firstWord);

/*this function copy from assembly file to a new file the text without macro declartion and open the macro on the program*/
void copyFile(const char *path);

/*this function check if the given files exists and open new file in order to write in it the new code*/
void preAssemblerHandler(char *fileName);

/* This function end the program.*/
void stop();

/*this function send the file to the macros proccess*/
void preAssembler(char* fileName);

#endif
