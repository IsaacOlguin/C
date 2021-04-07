#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

typedef struct node_binary_heap {
    int value;
    int level;
    struct node_binary_heap *leftChild;
    struct node_binary_heap *rightChild;
    struct node_binary_heap *parent;
} NodeBinaryHeap;

enum TypesMovementsTraversingTheHeap { GoingUp, GoingDown };

NodeBinaryHeap *createNewNodeBinaryHeap();
NodeBinaryHeap *createNewNodeBinaryHeapWithInitialValues(int value, int level);
NodeBinaryHeap *insertNodeIntoBinaryHeap(NodeBinaryHeap *lastInsertedElement, int value);
NodeBinaryHeap *findSuccessorOf(NodeBinaryHeap *currentNode);
NodeBinaryHeap *findSuccessorPerLevelsOf(NodeBinaryHeap *currentElement, int level, int movement, NodeBinaryHeap *lastElement);
void printInOrderBinaryHeap(NodeBinaryHeap *rootBinaryHeap);
void printInOrderBinaryHeapPretty(NodeBinaryHeap *rootBinaryHeap);
void fulfillHeapProperty(NodeBinaryHeap *lastInsertedElement);

#endif //BINARY_HEAP_H