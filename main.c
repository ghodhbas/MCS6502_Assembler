#include <stdio.h>
#include <stdlib.h>
#include "input_output.h"

int main(int argc, char *argv[]){
    //
    if(argv[2] != NULL){
        sscanf(argv[2],"%x",&start);
    }

    byteCounter=start;
    //initialize operations tree
    OperationTree* opTree = InitOperationTree();
    //create synbol tree
    SymbolTree* symbolTree = createSymbolTree();
    
    //open file
    FILE* fileptr = fopen(argv[1],"r");
    if(fileptr == NULL){
        printf("Error Opening File");
        exit(1);
    } 
    
    int lineCounter = 0;
    //read lines
    while ( fgets ( line, sizeof line, fileptr ) != NULL )
    {   
        //analyze line and save output to memory
        analyzeLine(line, &lineCounter, symbolTree,opTree);

    }
    fclose(fileptr);

    //output
    output_Console(symbolTree,symbols,symbolsRelative);
    output_Files(symbolTree,symbols, symbolsRelative);
    
    return 1;
}
