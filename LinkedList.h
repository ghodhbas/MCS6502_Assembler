#include <stdbool.h>

typedef struct Node {
  struct Node* next;
  struct Node* prev;
  int data;
} Node;


typedef struct {
  Node* head;
  Node* tail;
} LinkedList;

extern LinkedList* createLinkedList();
extern void insertInLinkedList( LinkedList*, int );
extern void printLinkedList( LinkedList* );