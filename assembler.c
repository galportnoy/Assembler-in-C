#include "assembler.h"

/*
	This program is an a assembler translate to binary code an a assembly program. 
	The program runs in two passes and collect all of the relevant information to the machine language.
	
	Authors:Gal Portnoy and Omer Shlomo.
 	Date: 29/03/22

*/


int main(int argc, char *argv[]){
	FILE *fp;
	char line[MAX_LINE_LENGHT];
	int i, j;
	resetLine(line);

	for (i = 1; i < argc; i++){
		initLists();
		initCounters();
		preAssembler(argv[i]);
		fileName = file_name(argv[i]);
		fp = fopen(fileName, "r");
		rewind(fp);  /*used as safety to make sure pointer is at top of file */
		/*runs twice , first pass and second pass*/
		for (j = 1; j <= 2; j++){
			lineNumber = 1;
			if (fp == NULL){
				printf("%s %s\n", IOError, argv[i]);
			}
			else{
				while (fgets(line, MAX_LINE_LENGHT, fp) != NULL){
					if (j == 1){
						firstPass(line);
						lineNumber++;
					}
					else{
						secondPass(line);
						lineNumber++;
					}
                }
				if(j==1){
					updateSymbolList();
					fixLabelName();
					fixOperationLabel();
					updateDataAddress();
				}
				else{
					writeOutputFiles();
				}
				rewind(fp);
			}
        }
	fclose(fp);
    }
    return 0;
}
