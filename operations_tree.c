#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operations_tree.h"

/**Creates Binary tree that stores operations and their data**/
OperationTree* createOperationTree() {
    OperationTree* tree = (OperationTree*)malloc( sizeof( OperationTree ));
    tree->root = NULL;
    tree->size = 0;
    return(tree);
}

/**create a node that will be stored in the opration tree**/
OperationNode* createOperationNode(OperationData* op){
    OperationNode* Node = (OperationNode*)malloc( sizeof( OperationNode ));
    Node->left=NULL;
    Node->right=NULL;
    Node->operationData=op;
    return Node;
}

/**Creates the data that will stored int the operation node**/
OperationData* createOperationData(char* operationName, int* opcode){
    OperationData* opData = (OperationData*)malloc( sizeof( OperationData));
    opData->operationName = operationName;   
    for(int i=0;i<13;i++){
        if(opcode[i]!=-1){
            opData->opCode[i] = opcode[i];
        }else{
            opData->opCode[i] = -1;
        }
    }
    return opData;
}

/**creates a node with data and store it in the treee**/
void insertInOperationTree( OperationTree* tree, char* operationName, int* opcode) {
    //create node
    OperationData* data = createOperationData(operationName,opcode);
    OperationNode* newNode = createOperationNode(data);
    OperationNode *current, *prev;
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
        if ( strcmp(current->operationData->operationName,operationName) < 0 )
            current = current->right;
        else
            current = current->left;
    }
    if ( strcmp(prev->operationData->operationName, operationName) < 0 )
        prev->right = newNode;
    else
        prev->left = newNode;
}

/** Lookup a node in the tree with its name and return it**/
OperationNode* lookUpOperation(OperationTree* tree, char* name){
    OperationNode *current, *prev;
    if(name[strlen(name)-1] == '\n') name[strlen(name)-1] = '\0';

    current=tree->root;
    prev =NULL;
    while(current){
        prev=current;
        if(strcmp(current->operationData->operationName,name) == 0){
            return current;
        }else if ( strcmp(current->operationData->operationName,name) < 0 )
            current = current->right;
        else
            current = current->left;
    }

    return NULL;
}


/**Build and initialize the tree containing operation and their codes**/
OperationTree* InitOperationTree(){
    OperationTree* operationTree = createOperationTree();
    
    //populate the tree with all operations
    //1 -- 28
    char* name = "JMP";
    int opcodes[13]= {-1 , -1, -1, -1, -1, 0x4c,-1, -1, 0x6c, -1, -1,0x4c,-1};
    insertInOperationTree(operationTree,name, opcodes);

    //2 -- 14
    resetArray(opcodes);
    name = "CLC";
    opcodes[UNIQUE]= 0x18;
    insertInOperationTree(operationTree,name, opcodes);

    //3 -- 7
    resetArray(opcodes);
    name = "BIT";
    opcodes[ZEROPAGE]= 0x24;
    opcodes[ABSOLUTE]= 0x2D;
    insertInOperationTree(operationTree,name, opcodes);

    //4 -- 3
    resetArray(opcodes);
    name = "ASL";
    opcodes[ACCUMULATOR]= 0x0A;
    opcodes[ZEROPAGE]= 0x06 ;
    opcodes[ZEROPAGEX]= 0x16;
    opcodes[ABSOLUTE]= 0x0E;
    opcodes[ABSOLUTEX]= 0x1E;
    insertInOperationTree(operationTree,name, opcodes);

    //5 -- 1
    resetArray(opcodes);
    name = "ADC";
    opcodes[IMMEDIATE]= 0x69;
    opcodes[ZEROPAGE]= 0x65;
    opcodes[ZEROPAGEX]= 0x75;
    opcodes[ABSOLUTE]= 0x6D;
    opcodes[ABSOLUTEX]= 0x7D;
    opcodes[ABSOLUTEY]= 0x79;
    opcodes[INDIRECTX]= 0x61;
    opcodes[INDIRECTY]= 0x71;
    insertInOperationTree(operationTree,name, opcodes);

    //6 == 2
    resetArray(opcodes);
    name = "AND";
    opcodes[IMMEDIATE] = 0x29;
    opcodes[ZEROPAGE]= 0x25;
    opcodes[ZEROPAGEX]= 0x35;
    opcodes[ABSOLUTE]= 0x2D;
    opcodes[ABSOLUTEX]= 0x3D;
    opcodes[INDIRECTX]= 0x21;
    opcodes[INDIRECTY]= 0x31;
    insertInOperationTree(operationTree,name, opcodes);

    //7 -- 10
    resetArray(opcodes);
    name = "BPL";
    opcodes[RELATIVE]= 0x10;
    insertInOperationTree(operationTree,name, opcodes);

    //8 -- 8
    resetArray(opcodes);
    name = "BMI";
    opcodes[RELATIVE] = 0x30;
    insertInOperationTree(operationTree,name, opcodes);

    //9 -- 9
    resetArray(opcodes);
    name = "BNE";
    opcodes[RELATIVE] = 0xD0;
    insertInOperationTree(operationTree,name, opcodes);

    //10 --- 12
    resetArray(opcodes);
    name = "BVC";
    opcodes[RELATIVE] = 0x50;
    insertInOperationTree(operationTree,name, opcodes);

    //11 -- 11  SPECIAL CASE?
    resetArray(opcodes);
    name = "BRK";
    opcodes[UNIQUE] = 0x00;
    insertInOperationTree(operationTree,name, opcodes);

    //12 -- 13
    resetArray(opcodes);
    name = "BVS";
    opcodes[RELATIVE] = 0x70;
    insertInOperationTree(operationTree,name, opcodes);

    //13 -- 21
    resetArray(opcodes);
    name = "DEC";
    opcodes[ZEROPAGE]= 0xC6;
    opcodes[ZEROPAGEX]= 0xD6;
    opcodes[ABSOLUTE]= 0xCE;
    opcodes[ABSOLUTEX]= 0xDE;
    insertInOperationTree(operationTree,name, opcodes);

    //14 --  17
    resetArray(opcodes);
    name = "CLV";
    opcodes[UNIQUE]=0xB8;
    insertInOperationTree(operationTree,name, opcodes);

    //15 -- 15
    resetArray(opcodes);
    name = "CLD";
    opcodes[UNIQUE]= 0xD8;
    insertInOperationTree(operationTree,name, opcodes);

    //16 -- 16
    resetArray(opcodes);
    name = "CLI";
    opcodes[UNIQUE]=0x58;
    insertInOperationTree(operationTree,name, opcodes);

    //17 -- 19
    resetArray(opcodes);
    name = "CPX";
    opcodes[IMMEDIATE]=0xE0;
    opcodes[ZEROPAGE]=0xE4;
    opcodes[ABSOLUTE]=0xEC;
    insertInOperationTree(operationTree,name, opcodes);

    //18 -- 20
    resetArray(opcodes);
    name = "CPY";
    opcodes[IMMEDIATE]=0xC0;
    opcodes[ZEROPAGE]=0xC4;
    opcodes[ABSOLUTE]=0xCC;
    insertInOperationTree(operationTree,name, opcodes);

    //19 -- 24
    resetArray(opcodes);
    name = "EOR";
    opcodes[IMMEDIATE]=0x49;
    opcodes[ZEROPAGE]=0x45;
    opcodes[ZEROPAGEX]=0x55;
    opcodes[ABSOLUTE]=0x4D;
    opcodes[ABSOLUTEX]=0x5D;
    opcodes[ABSOLUTEY]=0x59;
    opcodes[INDIRECTX]=0x41;
    opcodes[INDIRECTY]=0x51;
    insertInOperationTree(operationTree,name, opcodes);

    //20 -- 22
    resetArray(opcodes);
    name = "DEX";
    opcodes[UNIQUE]=0xCA;
    insertInOperationTree(operationTree,name, opcodes);

    //21 -- 23
    resetArray(opcodes);
    name = "DEY";
    opcodes[UNIQUE]=0x88;
    insertInOperationTree(operationTree,name, opcodes);

    //22 -- 26
    resetArray(opcodes);
    name = "INX";
    opcodes[UNIQUE]=0xE8;
    insertInOperationTree(operationTree,name, opcodes);

    //23 -- 25
    resetArray(opcodes);
    name = "INC";
    opcodes[ZEROPAGE]=0xE6;
    opcodes[ZEROPAGEX]=0xF6;
    opcodes[ABSOLUTE]=0xEE;
    opcodes[ABSOLUTEX]=0xFE;
    insertInOperationTree(operationTree,name, opcodes);

    //24 -- 27
    resetArray(opcodes);
    name = "INY";
    opcodes[UNIQUE]=0xC8;
    insertInOperationTree(operationTree,name, opcodes);

    //25 -- 42
    resetArray(opcodes);
    name = "RTI";
    opcodes[UNIQUE]=0x40;
    insertInOperationTree(operationTree,name, opcodes);

    //26 -- 35
    resetArray(opcodes);
    name = "ORA";
    opcodes[IMMEDIATE]=0x09;
    opcodes[ZEROPAGE]=0x05;
    opcodes[ZEROPAGEX]=0x15;
    opcodes[ABSOLUTE]=0x0D;
    opcodes[ABSOLUTEX]=0x1D;
    opcodes[ABSOLUTEY]=0x19;
    opcodes[INDIRECTX]=0x01;
    opcodes[INDIRECTY]=0x11;
    insertInOperationTree(operationTree,name, opcodes);

    //27 -- 31
    resetArray(opcodes);
    name = "LDX";
    opcodes[IMMEDIATE]=0xA2;
    opcodes[ZEROPAGE]=0xA6;
    opcodes[ZEROPAGEY]=0xB6;
    opcodes[ABSOLUTE]=0xAE;
    opcodes[ABSOLUTEY]=0xBE;
    insertInOperationTree(operationTree,name, opcodes);

    //28 -- 29
    resetArray(opcodes);
    name = "JSR";
    opcodes[ABSOLUTE]=0x20;
    opcodes[UNIQUE]=0x20;
    insertInOperationTree(operationTree,name, opcodes);

    //29 -- 30
    resetArray(opcodes);
    name = "LDA";
    opcodes[IMMEDIATE]=0xA9;
    opcodes[ZEROPAGE]= 0xA5 ;
    opcodes[ZEROPAGEX]=0xB5;
    opcodes[ABSOLUTE]=0xAD;
    opcodes[ABSOLUTEX]=0xBD;
    opcodes[ABSOLUTEY]=0xB9;
    opcodes[INDIRECTX]=0xA1;
    opcodes[INDIRECTY]=0xB1;
    insertInOperationTree(operationTree,name, opcodes);

    //30 -- 33
    resetArray(opcodes);
    name = "LSR";
    opcodes[ACCUMULATOR]=0x4A;
    opcodes[ZEROPAGE]=0x46;
    opcodes[ZEROPAGEX]=0x56;
    opcodes[ABSOLUTE]=0x4E;
    opcodes[ABSOLUTEX]=0x5E;
    insertInOperationTree(operationTree,name, opcodes);

    //31 -- 32
    resetArray(opcodes);
    name = "LDY";
    opcodes[IMMEDIATE]=0xA0;
    opcodes[ZEROPAGE]=0xA4;
    opcodes[ZEROPAGEX]=0xB4;
    opcodes[ABSOLUTE]=0xAC;
    opcodes[ABSOLUTEX]=0xBC;
    insertInOperationTree(operationTree,name, opcodes);

    //32 -- 34
    resetArray(opcodes);
    name = "NOP";
    opcodes[UNIQUE]=0xEA;
    insertInOperationTree(operationTree,name, opcodes);

    //33 -- 38
    resetArray(opcodes);
    name = "PLA";
    opcodes[UNIQUE]=0x68;
    insertInOperationTree(operationTree,name, opcodes);

    //34 -- 36
    resetArray(opcodes);
    name = "PHA";
    opcodes[UNIQUE]=0x48;
    insertInOperationTree(operationTree,name, opcodes);

    //35 -- 37
    resetArray(opcodes);
    name = "PHP";
    opcodes[UNIQUE]=0x08;
    insertInOperationTree(operationTree,name, opcodes);

    //36 -- 40
    resetArray(opcodes);
    name = "ROL";
    opcodes[ACCUMULATOR]=0x2A;
    opcodes[ZEROPAGE]=0x26;
    opcodes[ZEROPAGEX]=0x36;
    opcodes[ABSOLUTE]=0x1E;
    opcodes[ABSOLUTEX]=0x3E;
    insertInOperationTree(operationTree,name, opcodes);

    //37 -- 39
    resetArray(opcodes);
    name = "PLP";
    opcodes[UNIQUE]=0x28;
    insertInOperationTree(operationTree,name, opcodes);

    //38 -- 41
    resetArray(opcodes);
    name = "ROR";
    opcodes[ACCUMULATOR]=0x6A;
    opcodes[ZEROPAGE]=0x66;
    opcodes[ZEROPAGEX]=0x76;
    opcodes[ABSOLUTE]=0x6E;
    opcodes[ABSOLUTEX]=0x7E;
    insertInOperationTree(operationTree,name, opcodes);

    //39 -- 49
    resetArray(opcodes);
    name = "STX";
    opcodes[ZEROPAGE]=0x86;
    opcodes[ZEROPAGEY]=0x96;
    opcodes[ABSOLUTE]=0x8E;
    insertInOperationTree(operationTree,name, opcodes);
    
    //40 -- 45
    resetArray(opcodes);
    name = "SEC";
    opcodes[UNIQUE]=0x38;
    insertInOperationTree(operationTree,name, opcodes);

    //41 -- 43
    resetArray(opcodes);
    name = "RTS";
    opcodes[UNIQUE]=0x60;
    insertInOperationTree(operationTree,name, opcodes);

    //42 -- 44
    resetArray(opcodes);
    name = "SBC";
    opcodes[IMMEDIATE]= 0xE9;
    opcodes[ZEROPAGE]= 0xE5;
    opcodes[ZEROPAGEX]= 0xF5;
    opcodes[ABSOLUTE]= 0xED;
    opcodes[ABSOLUTEX]= 0xFD;
    opcodes[ABSOLUTEY]=0xF9;
    opcodes[INDIRECTX]=0xE1;
    opcodes[INDIRECTY]=0xF1;
    insertInOperationTree(operationTree,name, opcodes);

    //43 -- 47
    resetArray(opcodes);
    name = "SEI";
    opcodes[UNIQUE]=0x78;
    insertInOperationTree(operationTree,name, opcodes);

    //44 -- 46
    resetArray(opcodes);
    name = "SED";
    opcodes[UNIQUE] = 0xF8;
    insertInOperationTree(operationTree,name, opcodes);

    //45 -- 48
    resetArray(opcodes);
    name = "STA";
    opcodes[ZEROPAGE]=0x85;
    opcodes[ZEROPAGEX]=0x95;
    opcodes[ABSOLUTE]=0x8D;
    opcodes[ABSOLUTEX]=0x9D;
    opcodes[ABSOLUTEY]=0x99;
    opcodes[INDIRECTX]=0x81;
    opcodes[INDIRECTY]=0x91;
    insertInOperationTree(operationTree,name, opcodes);

    //46 -- 52
    resetArray(opcodes);
    name = "TAY";
    opcodes[UNIQUE]=0xA8;
    insertInOperationTree(operationTree,name, opcodes);

    //47 -- 50
    resetArray(opcodes);
    name = "STY";
    opcodes[ZEROPAGE]=0x84;
    opcodes[ZEROPAGEX]=0x94;
    opcodes[ABSOLUTE]=0x8C;
    insertInOperationTree(operationTree,name, opcodes);

    //48 -- 51
    resetArray(opcodes);
    name = "TAX";
    opcodes[UNIQUE]=0xAA;
    insertInOperationTree(operationTree,name, opcodes);

    //49 -- 54
    resetArray(opcodes);
    name = "TSX";
    opcodes[UNIQUE]=0xBA;
    insertInOperationTree(operationTree,name, opcodes);

    //50 -- 53
    resetArray(opcodes);
    name  = "TYA";
    opcodes[UNIQUE]=0x98;
    insertInOperationTree(operationTree,name, opcodes);

    //51 -- 56
    resetArray(opcodes);
    name = "TXS";
    opcodes[UNIQUE]=0x9A;
    insertInOperationTree(operationTree,name, opcodes);

    //52 -- 4
    resetArray(opcodes);
    name = "BCC";
    opcodes[RELATIVE]=0x90;
    insertInOperationTree(operationTree,name, opcodes);

    //53 -- 5
    resetArray(opcodes);
    name = "BCS";
    opcodes[RELATIVE]=0xB0;
    insertInOperationTree(operationTree,name, opcodes);

    //54 -- 6
    resetArray(opcodes);
    name = "BEQ";
    opcodes[RELATIVE]=0xF0;
    insertInOperationTree(operationTree,name, opcodes);

    //55 -- 18
    resetArray(opcodes);
    name = "CMP";
    opcodes[IMMEDIATE]=0xC9;
    opcodes[ZEROPAGE]=0xC5;
    opcodes[ZEROPAGEX]=0xD5;
    opcodes[ABSOLUTEX]=0xDD;
    opcodes[ABSOLUTEY]=0xC9;
    opcodes[INDIRECTX]=0xC1;
    opcodes[INDIRECTY]=0xD1;
    insertInOperationTree(operationTree,name, opcodes);

    //56 -- 55
    resetArray(opcodes);
    name = "TXA";
    opcodes[UNIQUE]=0x8A;
    insertInOperationTree(operationTree,name, opcodes);

    return operationTree;
}

void resetArray(int* array){
    for(int i=0; i<13;i++){
        array[i]=-1;
    }
}