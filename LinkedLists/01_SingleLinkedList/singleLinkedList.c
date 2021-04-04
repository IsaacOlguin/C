/*
    gcc singleLinkedList.c ../LinkedList.c -o test
    ./test
*/

#include <stdio.h>
#include <stdlib.h>
#include "../LinkedList.h"

#define LENGTH_ARRAY 10

int main() {
    printf("\t\t=====> SINGLE LINKED LIST <=====\n");
    ElemLinkedList *linkedList = (ElemLinkedList *)malloc(sizeof(ElemLinkedList));

    int array[LENGTH_ARRAY] = { 23, 4, 18, 24, 21, 3, 7, 19, 20, 21};
    int i;

    printf("Total numbers in the array is [%i], which values are: [ ", LENGTH_ARRAY);
    for( i=0; i < LENGTH_ARRAY; i++) {
        printf("%i ", array[i]);
    }
    printf("]\n");

    printf("\n\nInsertion is executed: ...\n");
    linkedList->value = array[0];
    linkedList->nextElem = NULL;
    linkedList->prevElem = NULL;

    for( i=1; i < LENGTH_ARRAY; i++) {
        linkedList = addNewElement(linkedList, array[i], SingleLinkedList);
        //printf("Address of LinkedList at position [%i] is [%p]\n", array[i], linkedList);
    }

    printf("\n\nPrint LinkedList is triggered: ...\n");
    printLinkedList(linkedList);

    if(linkedList != NULL) free(linkedList);

    return 0;
}