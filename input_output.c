#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "input_output.h"

unsigned int start=0;
//counts bytes 
int byteCounter;
//number of strings per line
int totalStrings = 25;
//number of caracters per string
int stringSize=15;
//tracks current seciton count relative to what last was split;
int sectionCount=0;

int maxStrings = 50;

//last index of symbols
int symbolsToBeReplaced = 0;
int symbolsRelativeToBeReplaced = 0;

/************************************************ MISC FUNCTIONS ***************************************************************************/
//devide string with delimiter and return section forming the string
char** SplitWords(char* line, char* delim){
    sectionCount=0;
    //assume we have 50 words per line
	char** word = malloc( maxStrings * sizeof(char *));
    //allocate memory for every word
    for (int i = 0; i < maxStrings; ++i) {
        word[i] = malloc(stringSize+1);
    }

    //put words in string container
	char* ptr = strtok(line, delim);
	while (ptr != NULL)
	{
		word[sectionCount] = ptr;
		ptr = strtok(NULL, delim);
		sectionCount++;
	}
    //remember section count in a line
	return word;
}

/** Convert string to Upper Case **/
void UpperCase(char* string){
    while ( *string != '\0' ) {
        *string = toupper ( ( unsigned char ) *string );
        ++string;
    }
}


/**************************************************   ANALYZE LINE AND DEAL WITH THE COMPONENTS  ***********************************************/

void analyzeLine(char* line, int* lineCounter, SymbolTree* symbTree, OperationTree* opTree){
     if(strlen(line)==0 || line[0]=='\n')return;
    (*lineCounter)++;
    /* SPLIT LINE TO FIND IF THERE IS A SYMBOL AT THE START*/
    //split line in respect of colon
    char** sections = SplitWords(line,":");

    //there is a symbol in the first position
    if(sectionCount>1){
        //SAVE Symbol and location if not already saved
        UpperCase(sections[0]);
        insertInSymbolTree( symbTree, sections[0], byteCounter);
        //DEAL with the rest of the line
        if(strlen(sections[1])==0 || sections[1][0]=='\n')return;
        char** comp = SplitWords(sections[1], " ");
        restLineAnalyisis(opTree, comp,sectionCount);
        
       
        free(comp);
    }else{
        //DEAL with the line without a sumbol
        char** comp = SplitWords(sections[0], " ");
        restLineAnalyisis(opTree, comp,sectionCount);
        free(comp);
    }  
    free(sections);
}

/**DEAL with the rest of the line**/
void restLineAnalyisis(OperationTree* opTree, char** comp,int sectionCount){
    if(strlen(comp[0])==0)return;
    //there are operands and it's not a comment
        if(sectionCount>1 && comp[1][0]!=';'){
            //comp[0] has operation comp[1 has operands]
            /**OPERATION**/
            //get operation
            if(comp[0]==NULL) return;
            UpperCase(comp[0]);
            OperationNode* operation = lookUpOperation(opTree,comp[0]);
            //decide operation mode -- convert upper case
            if(comp[1]==NULL) return;
            UpperCase(comp[1]);
            if(comp[1][0]==';')return;
            Mode m;
            //check if there is space in the operands
            if(strcmp(comp[0],"BNE")==0 || strcmp(comp[0],"BCS")==0 || strcmp(comp[0],"BEQ")==0 || strcmp(comp[0],"BCC")==0 || strcmp(comp[0],"BPL")==0 || strcmp(comp[0],"BMI")==0 || strcmp(comp[0],"BVC")==0 || strcmp(comp[0],"BVS")==0){
                m=RELATIVE;
            }else if(comp[2] != NULL){
                m = getMode(comp[1],comp[2]);
            }else{
                m = getMode(comp[1],"");
            }
            //handle error
            if(operation->operationData->opCode[m] == -1){
                printf("[ERROR] operation mode doesnt exist for \"%s\" operation with operand %s | MODE = %d\n",operation->operationData->operationName, comp[1],m);
                exit(0);
            }
            output[byteCounter - start] = operation->operationData->opCode[m];
            byteCounter++;
            
            /**OPERAND**/
            //DEAL WITH OPERAND add them to output & increase byte counter and                 if operand symbol  --> if  mode returns unique which is 0. Save symbol
            saveOperand(m, comp[1], output);
        }else{
            //NO OPERANDS DEAL OMLY WITH OPERATION
            /**OPERATION**/
            //get operation
            if(comp[0]==NULL) return;
            if(comp[0]=="") return;
            if(strlen(comp[0])==0)return;
            if(comp[0][0]==';')return;
            UpperCase(comp[0]);
            OperationNode* operation = lookUpOperation(opTree,comp[0]);
            //decide operation mode -- convert upper case
            //handle error
            if(operation->operationData->opCode[UNIQUE] == -1){
                printf("[ERROR] operation mode doesnt exist for \"%s\" operation\n",operation->operationData->operationName );
                exit(0);
            }
            output[byteCounter - start] = operation->operationData->opCode[UNIQUE];
            byteCounter++;
        }
}

/** RETURN OPERATION MODE **/
Mode getMode(char* operands, char* nextComp){
    if(operands[0]=='#') return IMMEDIATE;
    if(operands[0]=='A')  return ACCUMULATOR;
    if(operands[0]=='$'){
        //zeropage
        if(strlen(operands) == 3 || strlen(operands) == 4)return ZEROPAGE;
        if( (strlen(operands)==5) && (operands[strlen(operands)-1] == 'X')) return ZEROPAGEX;
        if( (strlen(operands)==6) && (operands[strlen(operands)-2] == 'X') && operands[strlen(operands)-1] == '\n') return ZEROPAGEX;
        if( (strlen(operands)==5) && (operands[strlen(operands)-1] == 'Y')) return ZEROPAGEY;
        if( (strlen(operands)==6) && (operands[strlen(operands)-2] == 'Y') && operands[strlen(operands)-1] == '\n') return ZEROPAGEY;
        //absolute
        if(strlen(operands) == 5  || strlen(operands)==6 )return ABSOLUTE;
        if( (strlen(operands)==7) && (operands[strlen(operands)-1] == 'X')) return ABSOLUTEX;
        if( (strlen(operands)==8) && (operands[strlen(operands)-2] == 'X') && operands[strlen(operands)-1] == '\n') return ABSOLUTEX;
        if( (strlen(operands)==7) && (operands[strlen(operands)-1] == 'Y')) return ABSOLUTEY;
        if( (strlen(operands)==8) && (operands[strlen(operands)-2] == 'Y') && operands[strlen(operands)-1] == '\n') return ABSOLUTEY;
    }
    //indirect
    if(operands[0]=='('){
        if(strlen(operands)==7  && operands[5]=='X' && operands[6]==')') return INDIRECTX;
        if(strlen(operands)==5  && nextComp!="" && strlen(nextComp)==2 && nextComp[0]=='X' && nextComp[1]==')') return INDIRECTX;
        if(strlen(operands)==8  && operands[5]=='X' && operands[6]==')' && operands[7]=='\n') return INDIRECTX;
        if(strlen(operands)==5  && nextComp!="" && strlen(nextComp)==3 && nextComp[0]=='X' && nextComp[1]==')' && nextComp[1]=='\n') return INDIRECTX;
        if(strlen(operands)==7 && operands[6]=='Y') return INDIRECTY;
        if(strlen(operands)==8 && operands[7]=='\n' && operands[6]=='Y') return INDIRECTY;
        if(strlen(operands)==6 && nextComp!="" && nextComp[0]=='Y') return INDIRECTY;
    }

    return UNIQUE;
}

void saveOperand(Mode m, char* operand, int output[]){
    //IMMEDIATE
    if(m==IMMEDIATE){
        //if operand is in hex format
        if(operand[1]=='$'){
            //convert to hex
            //get 3 char
            char* tmp = operand+2;
            sscanf(tmp,"%x", &output[byteCounter-start]);
            byteCounter++;
        }else{
            char* tmp = operand+1;
            sscanf(tmp,"%d", &output[byteCounter-start]);
            byteCounter++;
        }
        return;
    }

    //ACCUMULATOR
    if(m==ACCUMULATOR) return;
    //ZEROPAGE
    if(m==ZEROPAGE){
        //ignore "enter" if it exists
        if(strlen(operand) == 4 && (operand[strlen(operand)-1] == '\n')){
            operand[strlen(operand)-1]='\0';
        }
        //go past $ sign
        char* tmp = operand+1;
        sscanf(tmp,"%x", &output[byteCounter - start]);
        byteCounter++;
        return;
    }

    //ZEROPAGEX && Y 
    if(m==ZEROPAGEX || m==ZEROPAGEY){
        //consider first 3 chars
        operand[3]='\0';
        //consider last 2 chars
        char* tmp = operand+1;
        sscanf(tmp,"%x", &output[byteCounter - start]);
        byteCounter++;
        return;
    }

    //ABSOLUTE
    if(m==ABSOLUTE){
        //ignore "enter" if it exists
        if(strlen(operand)==6){
            operand[strlen(operand)-1]='\0';
        }
        //ignore $
        char* tmp = operand+1;
        //split to two bytes
        char firstByte[3];
        strncpy(firstByte,tmp,2);
        firstByte[2]='\0';
        //2nd byte
        char secondByte[3];
        strncpy(secondByte,tmp+2,2);
        firstByte[2]='\0';

        sscanf(firstByte,"%x", &output[byteCounter - start]);
        byteCounter++;
        sscanf(secondByte,"%x", &output[byteCounter - start]);
        byteCounter++;
        return;
    }

    //ABSOLUTE X/Y
    if(m==ABSOLUTEX || m==ABSOLUTEY){
        //consider first 5 chars
        operand[5]='\0';
        //consider last 2 chars
        char* tmp = operand+1;
        //split to two bytes
        char firstByte[3];
        strncpy(firstByte,tmp,2);
        firstByte[2]='\0';
        //2nd byte
        char secondByte[3];
        strncpy(secondByte,tmp+2,2);
        firstByte[2]='\0';

        sscanf(firstByte,"%x", &output[byteCounter - start]);
        byteCounter++;
        sscanf(secondByte,"%x", &output[byteCounter - start]);
        byteCounter++;
        return;
    }

    //Indirect
    if(m==INDIRECT || m==INDIRECTX || m==INDIRECTY ){
        //consider first 4 chars
        operand[4]='\0';
        //ignore ( and $
        //consider last 2 chars
        char* tmp = operand+2;
        sscanf(tmp,"%x", &output[byteCounter - start]);
        byteCounter++;
        return;
    }

    //UNIQUE -> Symbol save the symbol and the address
    if(m==UNIQUE){
        char* tmp = operand;
        if(tmp[strlen(tmp)-1]=='\n'){
            tmp[strlen(tmp)-1]='\0';
        }
        UpperCase(tmp);
        symbols[symbolsToBeReplaced]=createSymbolData(tmp, byteCounter);
        symbolsToBeReplaced++;
        //put value that will be looked and replaced later
        output[byteCounter - start]= 0xFFFF;
        byteCounter++;
    }

    if(m==RELATIVE){
        Mode comp = getMode(operand,"");
        //not symbol
        if(comp != UNIQUE){
            saveOperand(comp, operand, output);
        }else{
            //symbol
            char* tmp = operand;
            if(tmp[strlen(tmp)-1]=='\n'){
                tmp[strlen(tmp)-1]='\0';
            }
            UpperCase(tmp);
            symbolsRelative[symbolsRelativeToBeReplaced]=createSymbolData(tmp, byteCounter);
            symbolsRelativeToBeReplaced++;
            //put value that will be looked and replaced later
            output[byteCounter - start]= 0xFFFA;
            byteCounter++;
        }
    }

}


/********************************************************** OUTPUT ********************************************/

void output_Console(SymbolTree* symbolTree, SymbolData* symbols[], SymbolData* symbolsRelative[]){
    //print symbol tree
    printf("STARTHEADER\n");
    if(symbolTree->root != NULL) printf("%s",toStringSymbolTree(symbolTree));
    printf("ENDHEADER\n");

    //output of relatives
    for(int i = 0 ; i<symbolsRelativeToBeReplaced;i++){
        int address = lookUpSymbol(symbolTree,symbolsRelative[i]->SymbolName);
        if(address == -1){
            printf("[ERROR] Symbol Relative %s not found, Please check assembly again\n", symbolsRelative[i]->SymbolName);
            exit(1);
        }

        if(abs(symbolsRelative[i]->address - address)>0xFF){
            printf("[ERROR] Symbol Relative %s  branching out of range, Please check assembly again\n", symbolsRelative[i]->SymbolName);
            exit(1);
        }

       int offset = address - symbolsRelative[i]->address;
        //keep last byte because offset can only be a byte long
        offset = offset & 0xFF;
        
        output[symbolsRelative[i]->address - start]=offset;

    }

    //output of symbols
    for(int i = 0 ; i<symbolsToBeReplaced;i++){
        int address = lookUpSymbol(symbolTree,symbols[i]->SymbolName);
        if(address == -1){
            printf("[ERROR] Symbol %s not found, Please check assembly again\n", symbols[i]->SymbolName);
            exit(1);
        }


        //array for little endian
        int tmpArray[20];
        int k = 0;
        //address bigger than a byte;
        while(address> 0xFF){
            //slide array to make space for extra bytes
            for (int c = byteCounter; c > symbols[i]->address+1; c--){
                output[c - start] = output[c-1-start];
            }
            tmpArray[k] = address % 0x100;
            k++;
            address = address / 0x100;

             //update addresses of other symbols
            for( int j = i+1; j<symbolsToBeReplaced;j++){
                symbols[j]->address= symbols[j]->address+1;
            }
            byteCounter++;
        }

        if(k>0){
            output[symbols[i]->address+k-start]=address;
            while(k!=0){
                output[symbols[i]->address+k-1-start]=tmpArray[k-1];
                k--;
            }
        }else{
            output[symbols[i]->address-start]=address;
        }
    }

    

    //print output
    for(int i =0; i<byteCounter - start;i++){
        if(output[i]>= 0x10){
            printf("%X ",output[i]);
        }else{
            printf("0%X ",output[i]);
        }
    }
    printf("\n");

}


//output to files
void output_Files(SymbolTree* symbolTree, SymbolData* symbols[], SymbolData* symbolsRelative[]){
    FILE *out = fopen("Output.prt", "w");
    if (out == NULL)
    {
        printf("Error opening output file!\n");
        exit(1);
    }
    FILE *outb = fopen("Output_Binary.exc", "wb");
    if (out == NULL)
    {
        printf("Error opening output file!\n");
        exit(1);
    }



    //print symbol tree
    fprintf(out,"STARTHEADER\n");
    if(symbolTree->root != NULL) fprintf(out,"%s",toStringSymbolTree(symbolTree));
    fprintf(out,"ENDHEADER\n");

    //print symbol tree
    fprintf(outb,"STARTHEADER\n");
    if(symbolTree->root != NULL) fprintf(outb,"%s",toStringSymbolTree(symbolTree));
    fprintf(outb,"ENDHEADER\n");

    //output
     //output of relatives
    for(int i = 0 ; i<symbolsRelativeToBeReplaced;i++){
        int address = lookUpSymbol(symbolTree,symbolsRelative[i]->SymbolName);
        if(address == -1){
            printf("[ERROR] Symbol Relative %s not found, Please check assembly again\n", symbolsRelative[i]->SymbolName);
            exit(1);
        }

        if(abs(symbolsRelative[i]->address - address)>0xFF){
            printf("[ERROR] Symbol Relative %s  branching out of range, Please check assembly again\n", symbolsRelative[i]->SymbolName);
            exit(1);
        }

       int offset = address - symbolsRelative[i]->address;

        //keep last byre because offset can only be a byte long
        offset = offset & 0xFF;
        
        output[symbolsRelative[i]->address-start]=offset;

    }

    for(int i = 0 ; i<symbolsToBeReplaced;i++){
        int address = lookUpSymbol(symbolTree,symbols[i]->SymbolName);
        if(address == -1){
            printf("[ERROR] Symbol %s not found, Please check assembly again\n", symbols[i]->SymbolName);
            exit(1);
        }
        //address bigger than a byte;
        while(address> 0xFF){
            //slide array to make space for extra bytes
            for (int c = byteCounter; c > symbols[i]->address+1; c--){
                output[c-start] = output[c-1-start];
            }
            output[symbols[i]->address+1-start] = address % 0x100;
            address = address / 0x100;

             //update addresses of other symbols
            for( int j = i+1; j<symbolsToBeReplaced;j++){
                symbols[j]->address= symbols[j]->address+1;
            }

            byteCounter++;
        }
        output[symbols[i]->address-start] = address;
    }

   

    //print output
    for(int i =0; i<byteCounter - start;i++){
        if(i%36==0)fprintf(out,"\n");
        if(output[i]>= 0x10){
            fprintf(out,"%X ",output[i]);
        }else{
            fprintf(out,"0%X ",output[i]);
        }
    }
    fprintf(out,"\n");

    //print output
    for(int i =0; i<byteCounter - start;i++){
        if(i%36==0)fprintf(outb,"\n");
        if(output[i]>= 0x10){
            fprintf(outb,"%u",output[i]);
        }else{
            fprintf(outb,"0%u",output[i]);
        }
    }
    fprintf(outb,"\n");


    fclose(out);
    fclose(outb);
}
