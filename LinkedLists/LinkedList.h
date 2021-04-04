#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct element {
    int value;
    struct element *nextElem;
    struct element *prevElem;
} ElemLinkedList;

enum TypeLinkedList { SingleLinkedList, DoubleLinkedList, CircularLinkedList };

ElemLinkedList *addNewElement(ElemLinkedList *heap, int value, int typeInsertion);
void printLinkedList(ElemLinkedList *heap);


#endif // LINKED_LIST_H