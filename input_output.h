#include "symbol_tree.h"
#include "operations_tree.h"

//memory start
unsigned int start;
//Will have the output per byte for the program
unsigned int output[100000];
//write counter;
int byteCounter;

//contains the line read
char line[256];
//number of strings per line
int maxStrings;
//number of caracters per string
int stringSize;
//tracks current word count;
int wordCount;
//array of symbols to be replaces with addresses
SymbolData* symbols[1000];
int symbolsToBeReplaced;

SymbolData* symbolsRelative[1000];
int symbolsRelativeToBeReplaced;

//devide string with delimiter and return words
extern char** SplitWords(char* line, char* delim);
extern void analyzeLine(char* line, int* lineCounter, SymbolTree* symbTree, OperationTree* opTree);
extern Mode getMode(char* operands, char* nextComp);
extern void UpperCase(char* string);
extern void restLineAnalyisis(OperationTree* opTree, char** comp,int sectionCount);
extern void saveOperand(Mode m, char* operand, int output[]);
extern void output_Console(SymbolTree* symbolTree, SymbolData* symbols[], SymbolData* symbolsRelative[]);
extern void output_Files(SymbolTree* symbolTree, SymbolData* symbols[], SymbolData* symbolsRelative[]);