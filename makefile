assembler: assembler.o preAssembler.o firstPass.o utils.o structures.o linkList.o globalVariables.o secondPass.o writers.o writersUtils.o
	gcc -g -Wall -ansi -pedantic assembler.o preAssembler.o firstPass.o utils.o structures.o linkList.o globalVariables.o secondPass.o writers.o writersUtils.o -lm -o assembler

assembler.o: assembler.c
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o

preAssembler.o: preAssembler.c 
	gcc -c -Wall -ansi -pedantic preAssembler.c -o preAssembler.o

firstPass.o: firstPass.c globalVariables.h
	gcc -c -Wall -ansi -pedantic firstPass.c -o firstPass.o

secondPass.o: secondPass.c globalVariables.h
	gcc -c -Wall -ansi -pedantic secondPass.c -o secondPass.o	

utils.o: utils.c globalVariables.h
	gcc -c -Wall -ansi -pedantic utils.c -o utils.o

structures.o: structures.c globalVariables.h
	gcc -c -Wall -ansi -pedantic structures.c -o structures.o

linkList.o: linkList.c 
	gcc -c -Wall -ansi -pedantic linkList.c -o linkList.o

globalVariables.o: globalVariables.c globalVariables.h
	gcc -c -Wall -ansi -pedantic globalVariables.c -o globalVariables.o

writers.o: writers.c 
	gcc -c -Wall -ansi -pedantic writers.c -o writers.o

writersUtils.o: writersUtils.c 
	gcc -c -Wall -ansi -pedantic writersUtils.c -o writersUtils.o