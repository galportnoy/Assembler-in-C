#include "assembler.h"

/*
*	this function checks if its an external lable or internal lable and turn on the right bit in the binary line
*/
void getARE(int addressMethod, int isExtern, char *filename, int linenumber, int binary[])
{
	int i;
	int start, end;
	start = 16;
	end = 18;
	for (i = start; i <= end; i++){
		binary[i] = 0;
	}
	if (addressMethod == IMMEDIATE_ADDRESSING || addressMethod == DIRECT_REGISTER_ADDRESSING)
	{
		binary[start + 2] = 1;
	}
	else if (addressMethod == DIRECT_ADDRESSING || addressMethod == INDIRECT_REGISTER_ADDRESSING){
		if (isExtern == TRUE){
			binary[start] = 1;
		}
		else{
			binary[start + 1] = 1;
		}
	}
	else{
		secondRunErrors = TRUE;
		addError(IllegalAddressingMethod, filename, linenumber);
	}
}

/*
*	this function checks the adressing methods and turn on the right bit in the binary line
*/
void getAddressingMethodInBinary(int method, char *filename, int linNumber, int binary[], int src)
{
	int i, start, end;

	if (src == TRUE){
		start = 6;
		end = 7;
	}
	else{
		start = 0;
		end = 1;
	}

	for (i = start; i <= end; i++){
		binary[i] = 0;
	}

	switch (method)
	{
		case 0:
			break;
		case 1:
			binary[start] = 1;
			break;
		case 2:
			binary[end] = 1;
			break;
		case 3:
			binary[start] = 1;
			binary[end] = 1;
			break;
		case NOT_USED_INT:
			break;

		default:
			addError(IllegalAddressingMethod, filename, lineNumber);
			secondRunErrors = TRUE;
			break;
	}
}

/*
*	this function checks the opcode of operation and turn on the right bit in the binary line
*/
void translateOpCode(int decimal, int binary[])
{
	/*convertToBinary(decimal, 0, 15, binary, FALSE);*/
	int i;
	int start = 0, end = 15;
	i = start;
	while(i <= end){
		if(i == decimal){
			binary[i] = 1;
			break;
		}
		else{
			i++;
		}
	}
	
}

/*
*	this function checks if its an external lable or internal lable and turn on the right bit in the binary line
*/
void convertToBinary(int num, int binary[], int isData)
{
	int isNegative, start = 0 , end = 15, temp;
	isNegative = (num < 0 ? TRUE : FALSE);
	if (num < 0)
	{
		num = abs(num);
	}
	
	num = intToBinary(num);
	while(start <= end){
		temp = num % 10;
		binary[start] = temp;
		num = num /10;
		start++;
	}
	if (isNegative == TRUE)
	{
		convertToTwosComplement(binary);
	}
}

/*e
*	this function trnslate a negative number to binary line in the 2 complinet method
*/
void convertToTwosComplement(int binary[])
{
	int i, max, carry;
	max = 16;
	/* get 1's complement */
	for (i = 0; i < max; i++)
	{
		if (binary[i] == 1)
		{
			binary[i] = 0;
		}
		else
		{
			binary[i] = 1;
		}
	}
	/* add one to get 2's complement */
	carry = 1;
	i = 0;
	while (i < max && carry == 1)
	{
		if (binary[i] == 0)
		{
			binary[i] = 1;
			carry = 0;
		}
		else
		{
			binary[i] = 0;
		}
		i++;
	}
}



void getBasename(char *orig, char *resolved)
{
	int i, j;
	int gotName;
	i = 0;
	j = 0;
	gotName = FALSE;
	while (orig[i] != '\0' && orig[i] != '\n')
	{
		if ((orig[i] == '.') && (gotName == TRUE))
		{
			break;
		}
		else if (orig[i] == '\\' || orig[i] == '/' || orig[i] == '.')
		{
			i++;

		}
		else
		{
			resolved[j] = orig[i];
			i++;
			j++;
			gotName = TRUE;
		}

	}
	resolved[j++] = '\0';

}

void printData(FILE *fp){
	DataList *current;
	int binary[WORD_LENGTH];
	char *ptrSpecial;
	current = dataHead;
	while (current != NULL){
		binaryReset(binary);
		convertToBinary(current->code, binary, TRUE);
		getARE(IMMEDIATE_ADDRESSING, FALSE, current->fileName, current->lineNumber, binary);
		ptrSpecial = convertToHexadecimal(binary);
		writeObjLine(fp, ptrSpecial , current->address);
		current = current->next;
	}
}

void printEntry(FILE *fp){
	SymbolList *current;
	current = symbolHead;
	while (current != NULL){
		if (current->isEntry == TRUE){
			writeEntLine(fp, current->name, current->baseAddress , current->offset);
		}
		current = current->next;
	}
}

void printExtern(FILE *fp){
	SymbolList *current;
	current = symbolHead;
	while (current != NULL){
		if (current->isExtern == TRUE){
			writeExtLine(fp, current->name, current->baseAddress , "BASE");
			writeExtLine(fp, current->name, current->offset, "OFFSET");
			fprintf(fp , "\n");
		}
		current = current->next;
	}
}


void finalize(FILE *obj, FILE *ent, FILE *ext, char *objName, char *extName, char *entName, int hasErrors)
{
	fclose(obj);
	fclose(ent);
	fclose(ext);
	if (hasErrors == 0)
	{
		remove(objName);
		remove(extName);
		remove(entName);
	}
}

void updateBase(SymbolList *label , int lineNumber){
	label->baseAddress = lineNumber;
}

void updateOffset(SymbolList *label , int lineNumber){
	label->offset = lineNumber;
}

void writeObjLine(FILE *fp, char *specialBase, int address)
{
	int i;
	fprintf(fp, "%04d\t", address);
	for (i = 0; i < 14; i++)
	{
		fprintf(fp, "%c", specialBase[i]);
	}
	fprintf(fp, "\n");
}

void writeExtLine(FILE *fp, char *name, int address , char *word)
{
	fprintf(fp, "%s %s %04d\n", name, word, address);
}

void writeEntLine(FILE *fp, char *name, int baseAddress, int offset)
{
	fprintf(fp, "%s,%04d,%04d\n", name, baseAddress , offset);
}


char* convertToHexadecimal(int binary[]){
	char specialBase[] = "A0-B0-C0-D0-E0";
	char* ptr;
	int i , j , sum, k, baseIndex;
	int numArr[4];
	char c;
	ptr = specialBase;
	baseIndex = 1;
	for(i=WORD_LENGTH-1; i >= 0 ;){
		j=0;
		sum = 0;
		while(j < 4){
			numArr[j] = binary[i];
			j++;
			i--;
		}
		for(j=3, k=0 ; j >=0 ; j-- , k++){
			if(numArr[k] == 1){
				sum += pow(2 , j);
			}
		}
		if(sum > 9){
			c = ifHexa(sum);
			specialBase[baseIndex] = c;
		}
		else{
			specialBase[baseIndex] = sum + '0';
		}
		baseIndex +=3;
	}

	return ptr;
}

char ifHexa(int sum){
	switch (sum)
			{
			case 10:
				return 'a';
			case 11:
				return 'b';
			case 12:
				return 'c';
			
			case 13:
				return 'd';
			case 14:
				return 'e';
			case 15:
				return 'f';
			default:
				break;
			}
	return sum;
}


void putFunct(int funct , int binary[]){
	int start = 12 , end = 15;
	int num , temp;
	num = intToBinary(funct);
	while(start <= end){
		temp = num % 10;
		binary[start] = temp;
		num = num /10;
		start++;
	}
}

int intToBinary(int num){
	if(num < 2){
		return num;
	}
	return intToBinary(num/2) * 10 + num %2;
}

void putRegister(int binary[] , char *srcArg, int status){
	int i, start, end , num , temp;
	if (status == TRUE){
		start = 8;
		end = 11;
	}
	else{
		start = 2;
		end = 5;
	}

	for (i = start; i <= end; i++){
		binary[i] = 0;
	}
	for(i=0 ; i < 16 ; i++){
		if(strcmp(srcArg , REGISTER[i]) == 0 || strcmp(srcArg , REGISTER_ADDRESS[i]) == 0){
			num = intToBinary(i);
		}
	}
	while(start <= end){
		temp = num % 10;
		binary[start] = temp;
		num = num /10;
		start++;
	}
}

void binaryReset(int binary[]){
	int i;
	for(i=0; i < WORD_LENGTH ; i++){
		binary[i] = 0;
	}
}

void convertStrToBinary(char *arg , int binary[]){
	int num , start = 0 ,end = 15 , temp ,isNegative;
	arg++;
	num = atoi(arg);
	isNegative = (num < 0 ? TRUE : FALSE);
	if (num < 0)
	{
		num = abs(num);
	}
	num = intToBinary(num);
	while(start <= end){
		temp = num % 10;
		binary[start] = temp;
		num = num /10;
		start++;
	}
	if (isNegative == TRUE)
	{
		convertToTwosComplement(binary);
	}
}

void convertAddressToBinary(int baseAddress, int binary[]){
	int num, start = 0 ,end = 16 , temp;
	num = intToBinary(baseAddress);
	while(start <= end){
		temp = num % 10;
		binary[start] = temp;
		num = num /10;
		start++;
	}
}


char* cutToLabel(char *arg){
	int i=0;
	char word[20];
	char *ptr;
	while(arg[i] != '['){
		word[i] = arg[i];
		i++;
	}
	word[i] = '\0';
	ptr = word;
	return ptr;
}

char* cutToRgister(char *arg){
	int i=0, j=0;
	char word[20];
	char *ptr;
	while(arg[i] != '['){
		i++;
	}
	while(arg[i] != '\0'){
		word[j] = arg[i];
		i++;
		j++;
	}
	word[j] = '\0';
	ptr = word;
	return ptr;
}
