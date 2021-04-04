#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

ElemLinkedList *addNewElement(ElemLinkedList *heap, int value, int typeInsertion) {
    switch (typeInsertion)
    {
        case SingleLinkedList:
            if(heap == NULL) { 
                heap = (ElemLinkedList *)malloc(sizeof(ElemLinkedList));
                heap->value = value;
                heap->nextElem = NULL;
                heap->prevElem = NULL;
                return heap;
            } else {
                ElemLinkedList *aux = (ElemLinkedList *)malloc(sizeof(ElemLinkedList));
                aux->value = value;
                aux->nextElem = heap;
                aux->prevElem = NULL;
                return aux;
            }

            break;

        case DoubleLinkedList:
            break;

        case CircularLinkedList:
            break;
        
        default:
            break;
    }
}

void printLinkedList(ElemLinkedList *heap) {
    if( heap == NULL ) { 
        printf("NULL \n\n");
        return;
    }
    printf("%i -> ", heap->value);
    printLinkedList(heap->nextElem);
}