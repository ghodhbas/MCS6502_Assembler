


typedef struct SymbolData{
    char SymbolName[20];
    int address; //accessed by Mode Enum depending on the case  opCode[Mode]
}SymbolData;


typedef struct SymbolNode {
    struct SymbolNode* left;
    struct SymbolNode* right;
    SymbolData* SymbolData;
}SymbolNode;


typedef struct SymbolTree {
    SymbolNode* root;
    int size;
}SymbolTree;


extern SymbolTree* createSymbolTree();
extern SymbolNode* createSymbolNode(SymbolData* symb);
extern SymbolData* createSymbolData(char* symbolName, int byteCounter);
extern void insertInSymbolTree( SymbolTree* tree, char* symbolName, int byteCounter);
extern int lookUpSymbol(SymbolTree* tree, char* name);
extern char* toStringSymbolTree( SymbolTree*);
extern void SymbolTreeInOrder( SymbolNode* bt, char* p );