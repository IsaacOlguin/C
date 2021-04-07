#include <stdlib.h>
#include "BinaryHeap.h"
#include <stdio.h>

NodeBinaryHeap *createNewNodeBinaryHeap() {
    NodeBinaryHeap *node = (NodeBinaryHeap *)malloc(sizeof(NodeBinaryHeap));

    node->level = 0;
    node->value = -1;
    node->parent = NULL;
    node->leftChild = NULL;
    node->rightChild = NULL;

    return node;
}

NodeBinaryHeap *createNewNodeBinaryHeapWithInitialValues(int value, int level) {
    NodeBinaryHeap *node = (NodeBinaryHeap *)malloc(sizeof(NodeBinaryHeap));

    node->level = level;
    node->value = value;
    node->parent = NULL;
    node->leftChild = NULL;
    node->rightChild = NULL;

    return node;
}


NodeBinaryHeap *insertNodeIntoBinaryHeap(NodeBinaryHeap *lastInsertedElement, int value){
    if ( lastInsertedElement == NULL ) {
        //There are no elements and thus, we must create the root element
        return createNewNodeBinaryHeapWithInitialValues(value, 0);
    }

    //There are at least one node in the heap, then... find "last inserted element's successor"
    NodeBinaryHeap *successor = findSuccessorOf(lastInsertedElement);
    successor->value = value;

    fulfillHeapProperty(successor);

    return successor;
}

NodeBinaryHeap *findSuccessorOf(NodeBinaryHeap *currentElement) {
    if(currentElement == NULL) return createNewNodeBinaryHeap();

    if(currentElement->parent == NULL) { //Current node is root node
        if(currentElement->leftChild == NULL) {
            NodeBinaryHeap *child = createNewNodeBinaryHeapWithInitialValues(-1, currentElement->level+1);
            child->parent = currentElement;
            currentElement->leftChild = child;
            return child;
        }
    } 
    //Current node IS NOT root node
    if ( currentElement == (currentElement->parent)->leftChild && (currentElement->parent)->rightChild == NULL ) { 
        //CurrentElement is LEFT-child of its parent and RIGHT-child is NULL. Then, RIGHT-child is successor
        NodeBinaryHeap *successor = createNewNodeBinaryHeapWithInitialValues(-1, currentElement->level);
        successor->parent = currentElement->parent;
        currentElement->parent->rightChild = successor;
        return successor;
    } else if ( currentElement == (currentElement->parent)->rightChild ) {
        //printf("Last element is right child\n");
        return findSuccessorPerLevelsOf(currentElement, currentElement->level, GoingUp, currentElement);
    }
}

NodeBinaryHeap *findSuccessorPerLevelsOf(NodeBinaryHeap *currentElement, int level, int movement, NodeBinaryHeap *lastElement) {
    switch (movement)
    {
        case GoingUp:
            if( currentElement->parent == NULL ) { //This is root node
                if( lastElement == currentElement->rightChild ) {
                    return findSuccessorPerLevelsOf(currentElement->leftChild, level+1, GoingDown, currentElement);
                }
            } else if( currentElement == currentElement->parent->leftChild ) { //Current element is LEFT child
                return findSuccessorPerLevelsOf(currentElement->parent, level, GoingDown, currentElement);
            } else if( currentElement == currentElement->parent->rightChild ) { //Current element is RIGHT child
                return findSuccessorPerLevelsOf(currentElement->parent, level, GoingUp, currentElement);
            }

            break;
        case GoingDown:
            if(currentElement == NULL) {
                NodeBinaryHeap *successor = createNewNodeBinaryHeapWithInitialValues(-1, lastElement->level+1); 
                successor->parent = lastElement;
                lastElement->leftChild = successor;

                return successor;
            } /*else if( currentElement->parent == NULL ) { //This is root node
                printf("Mi padre es NULL!\n");
            }*/ else if( lastElement == currentElement->leftChild ) {
                if ( currentElement->level == level-1 ) { //I come from my left-child and my right child is the successor
                    if ( currentElement->rightChild == NULL ) { 
                        NodeBinaryHeap *successor = createNewNodeBinaryHeapWithInitialValues(-1, level); 
                        successor->parent = currentElement;
                        currentElement->rightChild = successor;
                        return successor;
                    }
                    return currentElement->rightChild;
                } else {
                    return findSuccessorPerLevelsOf(currentElement->rightChild, level, GoingDown, currentElement);
                }
            } else if ( currentElement->level == level-1 ) {
                if ( currentElement->leftChild != NULL ) return currentElement->leftChild;
                
                NodeBinaryHeap *successor = createNewNodeBinaryHeapWithInitialValues(-1, currentElement->level+1);
                successor->parent = currentElement;
                currentElement->leftChild = successor;
                return successor;
            } else {
                return findSuccessorPerLevelsOf(currentElement->leftChild, level, GoingDown, currentElement);
            }
            break;
    }
}

void fulfillHeapProperty(NodeBinaryHeap *lastInsertedElement) {
    if( lastInsertedElement->parent != NULL ) {
        /* This is not the root node and hence, we must evaluate whether we have to move upwards the value */
        if( lastInsertedElement->value < lastInsertedElement->parent->value ) {
            int aux = lastInsertedElement->value;
            lastInsertedElement->value = lastInsertedElement->parent->value;
            lastInsertedElement->parent->value = aux;

            fulfillHeapProperty(lastInsertedElement->parent);
        }
    }
}

void printInOrderBinaryHeap(NodeBinaryHeap *rootBinaryHeap) {
    if( rootBinaryHeap != NULL ) {
        printf("%i[%i] ", rootBinaryHeap->value, rootBinaryHeap->level);
        printInOrderBinaryHeap(rootBinaryHeap->leftChild);
        printInOrderBinaryHeap(rootBinaryHeap->rightChild);

        if(rootBinaryHeap->level == 0) printf("\n\n");
    }
}

void printInOrderBinaryHeapPretty(NodeBinaryHeap *rootBinaryHeap) {
    if( rootBinaryHeap != NULL ) {
        int i;
        int level = rootBinaryHeap->level;
        for(i=0; i < level; i++) 
            printf("\t");
        printf("%i[%i]\n", rootBinaryHeap->value, rootBinaryHeap->level);

        printInOrderBinaryHeapPretty(rootBinaryHeap->leftChild);
        printInOrderBinaryHeapPretty(rootBinaryHeap->rightChild);

        if(rootBinaryHeap->level == 0) printf("\n\n");
    }
}
