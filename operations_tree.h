typedef enum{
    IMMEDIATE,
    ACCUMULATOR,
    ZEROPAGE,
    ZEROPAGEX,
    ZEROPAGEY,
    ABSOLUTE,
    ABSOLUTEX,
    ABSOLUTEY,
    INDIRECT,
    INDIRECTX,
    INDIRECTY,
    UNIQUE,
    RELATIVE,
}Mode;

typedef struct operationData{
    char* operationName;
    int opCode[13]; //accessed by Mode Enum depending on the case  opCode[Mode]
}OperationData;


typedef struct OperationNode {
    struct OperationNode* left;
    struct OperationNode* right;
    OperationData* operationData;
}OperationNode;


typedef struct OperationTree {
    OperationNode* root;
    int size;
}OperationTree;


extern OperationTree* InitOperationTree();
extern OperationTree* createOperationTree();
extern OperationNode* createOperationNode(OperationData* op);
extern OperationData* createOperationData(char* operationName, int* opcode);
extern void insertInOperationTree( OperationTree* tree, char* operationName, int* opcode);
extern void resetArray(int array[]);
extern OperationNode* lookUpOperation(OperationTree* tree, char* name);
