#ifndef firstPass_h
#define firstPass_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globalVariables.h"


/*
*	this function works line after line after the preassembler opened all macros
*	it checks if the line is comment or end of file
*	after it reads the assembly program and dedect if there are syntax errors
*	if all ok its stores and update the operation / symbol / data tables in order to make it binary later
*/
void firstPass(char *line);

void print_symbleTable();
void print_symbleList();
void print_commandList();
void print_dataList();
#endif