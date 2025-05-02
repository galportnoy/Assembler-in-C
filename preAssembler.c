#include "preAssembler.h"
#include "linkList.h"

/*
*	this data structure represent a macro type
*/
struct macro
{
	char macroName[MAX_LINE_LENGTH];
	int start;
	int end;
};


/**
 * This function check whether a file exists or not.
 * It returns 1 if file exists at given path , otherwise returns 0.
 */
 
int isFileExists(const char *path)
{
    /*Try to open file */
    FILE *fptr = fopen(path, "r");

    /* If file does not exists */ 
    if (fptr == NULL)
        return 0;

    fclose(fptr);
    return 1;
}

/**
 * This function copy the conext of a macro by given start and end index from the macro defenition
 */
void CopyMacroContext(FILE *fp_am ,FILE *fp_as , void* macroNode)
{
	int size = ((struct macro*)(macroNode)) -> end - ((struct macro*)(macroNode)) -> start;
	int counter = 0;	
	int currentLocation = ftell(fp_as);
	char c;
	fseek(fp_as , ((struct macro*)macroNode) -> start , SEEK_SET);	
	while(counter!=size)
	{	
		c = fgetc(fp_as);
		fputc(c , fp_am);
		counter++;
	}	
	fseek(fp_as , currentLocation , SEEK_SET);	/*BACK TO THE POINTER*/
}
/**
 * This function compare between the given macro name and the macros stored in the macro table
 */
int CompareMacroNames(const void *firstWord, const void *macroNode)
{
	firstWord = (const char*)firstWord;
	macroNode = (const struct macro*)macroNode;
	if(strcmp((const char*)firstWord, ((const struct macro*)macroNode)->macroName)==0)
	{
		return 0;
	}
	else
		return 1;
}

/**
 * This function check if its a  legal declaration of macro
 */
int isMacro(char * firstWord)
{
	return !(strcmp(firstWord, S_MACRO));		
}

/**
 * This function check if its the end of the macro declartion
 */
int checkEndMacro(char * firstWord)
{
	return !(strcmp(firstWord, E_MACRO));
}

/* This function end the program.*/
void stop(){
    exit(0);
}

/**
 * This function creat a new file and copy all of the assembly program text into the new file without
 * the macro defenitions but open the macros in the text in the new file
 * we asump that the macro defenition always end
 */
void copyFile(const char *path){

    int i = 0;
    FILE *fptr1= fopen(path , "r");	/*CHECK IF OPEN SUCSEED*/
    FILE *fptr2 = NULL;
    char asName[80] = {0};
    char amName[80] = {0};
    char line[MAX_LINE_LENGTH] = {0};	
	macro_table *macroTable = NULL;
	link_list *macro_node = NULL;
	int inmacro = 0;
	const char *delim = " \t\n\v\r\f";
	char * firstWord;
	
	struct macro *macro = NULL;
	
    strcpy(asName, path);
    strcpy(amName,path);
    
    for(i=0; amName[i] != '\0'; i++){

    }
    amName[i-1] = 'm';
    fptr2 = fopen(amName, "w");

    if(fptr2 == NULL){
        printf("Unable to create file.\n");
        exit(0);   
    }
    
    while(fgets(line , MAX_LINE_LENGTH , fptr1))
	{	
		/*struct macro *macro = NULL;*/
		char copy_line[MAX_LINE_LENGTH];
		void *macro_data = NULL;		
		strcpy(copy_line, line);
		firstWord = strtok(copy_line, delim);		

		if(inmacro==1)	/*in case we are in a macro context*/
		{
			if (checkEndMacro(firstWord))
			{
				macro->end = ftell(fptr1) - strlen(line);
				inmacro = 0;
			}
		}
		
		else
		{
			if(firstWord == NULL)
			{
				fprintf(fptr2, "%s", line);	/*put in the .am file*/			
			}
			
			else if (isMacro(firstWord)!=0)	/*in case of declaration of macro*/
			{
				macro = malloc(sizeof(struct macro)); 	/* create new pointer to a macro*/
				if(macro == NULL)
				{
					printf("EROOR");
					exit(1);
				}
				
				inmacro = 1;
				strcpy(macro->macroName, strtok(NULL, delim));	/*get the macro name*/
				macro->start = ftell(fptr1); 
				macro->end = 0;							
				macro_node = CreateNode(macro, NULL);	/*new node to general list*/
				InsertNode(&macroTable, macro_node);
			}

			else if (macroTable!=NULL && (macro_data = LinkListFind(macroTable, firstWord, CompareMacroNames)) != NULL) /* if its macroName in list*/
			{
				CopyMacroContext(fptr2 , fptr1 ,  macro_data);	/*this func is put the macro context in the .am file*/
			}

			
			else	/*if its not macro name or not a macro declaration*/
			{
				fprintf(fptr2, "%s", line);	/*put in the .am file*/
			}
		}
	}
    fclose(fptr1);
    fclose(fptr2);
    FreeLinkList(macroTable);
}



/*
 *	this function check if the given files exists and open new file in order to write in it the new code
 */
void preAssemblerHandler(char *fileName){
	char* newFileName;
	int length = 0;
		length = strlen(fileName);
		newFileName = malloc(sizeof(char)*length + 3);
    	strcpy(newFileName, fileName);
        strcat(newFileName, ".as");
		if(isFileExists(newFileName) == 0){ /* check if file exists */
            printf("\ncan't open %s.\n", newFileName);
        }
        else{
            copyFile(newFileName);
        }
}



/* main pre assembler function */
void preAssembler(char* fileName)
{
	preAssemblerHandler(fileName);
}

