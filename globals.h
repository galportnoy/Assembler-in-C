#ifndef globals_h
#define globals_h

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "structures.h"
#include "errors.h"
#include "globalVariables.h"
#include "utils.h"
#include "libgen.h"
#include "limits.h"

#define MAX_LINE_LENGHT 82
#define IC_VALUE 100
#define ERROR INT_MIN
#define LEGAL 1
#define LEGAL_FIRST_WORD 2
#define IS_EXIST 1
#define IS_EXTERN 3
#define DATA 1
#define STRING 2
#define ENTRY 4
#define NOT_USED_INT INT_MIN
#define NOT_USED_STRING "not_used"

#define CODE "code"
#define EXTERNAL "external"
#define ENTRY_STR "entry"
#define DATA_STR "data"
#define NO_LABEL "NO_LABEL"
#define IMMEDIATE_ADDRESSING 0
#define DIRECT_ADDRESSING 1
#define INDIRECT_REGISTER_ADDRESSING 2
#define DIRECT_REGISTER_ADDRESSING 3
#define OPERATION_WORD 4
#define WORD_LENGTH 20
#define BITS_FOR_ARG_VALUE 12
#define NUM_OF_REGISTERS 8
#define OCTAL_LENGTH 5
#define SRC_REG_START 6
#define SRC_REG_END 8
#define DEST_REG_START 9
#define DEST_REG_END 11

typedef enum booleans{
    FALSE = -1 , TRUE = 1
}bool;

#endif