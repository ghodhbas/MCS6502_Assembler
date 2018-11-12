#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_tree.h"

/**Creates Binary tree that stores Symbols and their data**/
SymbolTree* createSymbolTree() {
    SymbolTree* tree = (SymbolTree*)malloc( sizeof( SymbolTree ));
    tree->root = NULL;
    tree->size = 0;
    return(tree);
}

/**create a node that will be stored in the opration tree**/
SymbolNode* createSymbolNode(SymbolData* symb){
    SymbolNode* Node = (SymbolNode*)malloc( sizeof( SymbolNode ));
    Node->left=NULL;
    Node->right=NULL;
    Node->SymbolData=symb;
    return Node;
}

/**Creates the data that will stored int the Symbol node**/
SymbolData* createSymbolData(char* SymbolName, int byteCounter){
    SymbolData* symbolData = (SymbolData*)malloc( sizeof( SymbolData));
    strncpy(symbolData->SymbolName, SymbolName, 19); //copy name while avoiding buffer overflow
    symbolData->address = byteCounter;
    return symbolData;
}

/**creates a node with data and store it in the treee**/
void insertInSymbolTree( SymbolTree* tree, char* SymbolName, int byteCounter) {
    //create node
    
    SymbolData* data = createSymbolData(SymbolName,byteCounter);
    SymbolNode* newNode = createSymbolNode(data);
    SymbolNode *current, *prev;
    tree->size++;

    //insert node in right location
    if ( tree->root == NULL ) {
        tree->root = newNode;
        return;
    }
    current = tree->root;
    prev = NULL;
    while( current ) {
        prev = current;
        if ( strcmp(current->SymbolData->SymbolName,SymbolName) < 0 )
            current = current->right;
        else
            current = current->left;
    }
    if ( strcmp(prev->SymbolData->SymbolName, SymbolName) < 0 )
        prev->right = newNode;
    else
        prev->left = newNode;
}

/** Lookup a symbol in the tree with its name and return its address**/
int lookUpSymbol(SymbolTree* tree, char* name){
    SymbolNode *current, *prev;

    current=tree->root;
    prev =NULL;
    while(current){
        prev=current;
        if(strcmp(current->SymbolData->SymbolName,name) == 0){
            return current->SymbolData->address;
        }else if ( strcmp(current->SymbolData->SymbolName,name) < 0 )
            current = current->right;
        else
            current = current->left;
    }

    return -1;
}

char* toStringSymbolTree( SymbolTree* bt) {
    if ( bt->root != NULL ) {
        char* printbuf = (char*)malloc(1000); //50 symbol allowed
        SymbolTreeInOrder( bt->root, printbuf );
        return( printbuf );
    }
}

void SymbolTreeInOrder( SymbolNode* bt, char* p ) {
    char tmpbuf[20];
    if ( bt == NULL ) return;
    SymbolTreeInOrder( bt->left, p );
    strcat( p, bt->SymbolData->SymbolName );
    strcat( p, " " );
    if(bt->SymbolData->address>=0x10){
        sprintf( tmpbuf, "0x%X", bt->SymbolData->address);
    }else{
        sprintf( tmpbuf, "0x0%X", bt->SymbolData->address);
    }
    strcat( p, tmpbuf );
    strcat( p, "\n" );
    SymbolTreeInOrder( bt->right, p );
}
