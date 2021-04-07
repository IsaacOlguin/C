#include <stdio.h>
#include <stdlib.h>
#include "BinaryHeap.h"

int main() {
    printf(" =====> BINARY HEAP <=====\n");

    int array[] = { 3, 5, 8, 10, 2, 1, 9, 6, 617, 295, 20, 45, 55, 65, 76, 93, 111, 120, 129, 16, 25, 12, 32, 88, 90, 145, 163, 241, 367, 183, 17, 7 };
    //int array[] = { 3, 5, 8, 10, 2 };
    int i, sizeArray;

    NodeBinaryHeap *rootBinaryHeap = NULL, *lastInsertedElement = NULL;

    sizeArray = sizeof array / sizeof array[0];
    printf("Lenght of the array <<%i>>\n", sizeArray);

    for( i = 0; i < sizeArray; i++ ) {
        //printf("Indice <<%i>>\n", i);
        lastInsertedElement = insertNodeIntoBinaryHeap(lastInsertedElement, array[i]);
        if(i==0) rootBinaryHeap = lastInsertedElement;
    }

    printf("\nPrinting Binary-Heap in InOrder-form\n");
    printInOrderBinaryHeap(rootBinaryHeap);
    printf("\nPrinting Binary-Heap in tree-form\n");
    printInOrderBinaryHeapPretty(rootBinaryHeap);

    if(rootBinaryHeap != NULL) free(rootBinaryHeap);
    if(lastInsertedElement != NULL) free(lastInsertedElement);

    return 0;
}