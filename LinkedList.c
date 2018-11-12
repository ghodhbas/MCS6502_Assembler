#include <stdlib.h>
#include <stdio.h>
#include "LinkedList.h"


LinkedList* createLinkedList() {
  LinkedList* ll;
  ll = (LinkedList *)malloc( sizeof( LinkedList ));
  ll->head = 0;
  ll->tail = 0;
  return( ll );
}

Node* createNewLLNode( int data ) {
  Node* newnode = (Node*)malloc( sizeof( Node ));
  newnode->data = data;
  newnode->next = 0;
  newnode->prev = 0;
  return( newnode );
}

void insertInLinkedList( LinkedList* ll, int data ) {
  Node* newnode = createNewLLNode( data );

  if ( ll->head == 0 ) { // first item
    ll->head = newnode;
    ll->tail = newnode;
  } else {
    newnode->next = ll->head;
    ll->head->prev = newnode;
    ll->head = newnode;
  }
}

void printNodeData( Node* n ) {
  printf( "%d ", n->data );
}

void printLinkedList( LinkedList* ll ) {
  Node* current;
  if ( ll->head != 0 ) {
    printf( "Data: " );
    current = ll->head;
    while( current != 0 ) {
      printNodeData( current );
      current = current->next;
    }
    printf( "\n" );
  }
}
