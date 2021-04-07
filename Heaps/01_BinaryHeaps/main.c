#include <stdio.h>
#include <stdlib.h>
#include "BinaryHeap.h"

int main() {
    printf(" =====> BINARY HEAP <=====\n");

    NodeBinaryHeap *root = NULL;

    int array[] = { 21, 5, 93, 07, 03, 04, 13};
    int i, sizeArray;

    NodeBinaryHeap *rootBinaryHeap = NULL, *lastInsertedElement = NULL;

    sizeArray = sizeof array / sizeof array[0];
    printf("Lenght of the array <<%i>>\n", sizeArray);

    for( i = 0; i < sizeArray; i++ ) {
        printf("Indice <<%i>>\n", i);
        lastInsertedElement = insertNodeIntoBinaryHeap(lastInsertedElement, array[i]);
        if(i==0) rootBinaryHeap = lastInsertedElement;
    }

    printInOrderBinaryHeap(rootBinaryHeap);

    if(root != NULL) free(rootBinaryHeap);

    return 0;
}