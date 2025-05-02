#ifndef secondPass_h
#define secondPass_h

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "errors.h"
#include "globalVariables.h"
#include "globals.h"
#include "structures.h"

/*
*	this function works line after line after the firstpass stored the relvant info
*	it checks if the file end and if its not guidnce line its skip this line
*	if its a guidnce line as entry guidnce it stores the info in entry guidnce table in order to check the lables 
*	inside the symbol table. after it check if errors occoured
*/
void secondPass(char *line);

#endif