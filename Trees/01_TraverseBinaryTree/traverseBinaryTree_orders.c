/*
    For the sake of simplicity, lets consider that only natural numbers can be inserted into the tree
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int value;
    int rank;
    struct node *leftChild;
    struct node *rightChild;
} Node;

void insertNewNode(Node *tree, int newValue);
void printPreOrder(Node *tree);
void printInOrder(Node *tree);
void printPostOrder(Node *tree);
Node *getMemoryForNode();

int main() {
    int flag = 0, newNumber = -1;
    Node tree;
    tree.value = -1;
    tree.rank = 0;
    tree.leftChild = NULL;
    tree.rightChild = NULL;

    printf("Willkommen zu TraverseTree!\n");
    while(0 <= flag && flag <= 4) {        
        printf("Choose one of the options listed below: \n");
        printf("\t1. Insert a node\n");
        printf("\t2. Print PRE-order\n");
        printf("\t3. Print IN-order\n");
        printf("\t4. Print POST-order\n");
        printf("\t*. Exit\n");
        scanf("%d", &flag);

        printf("\n\n");
        switch(flag){
            case 1:
                printf("\tSelected option --> Insert a node\n");
                printf("\t\tPlease give me the new number: ");
                scanf("%i", &newNumber);
                printf("\nI have received the number <<%i>>\n", newNumber);
                insertNewNode(&tree, newNumber);
                break;
            case 2:
                printf("\tSelected option --> Print PRE-order\n");
                printPreOrder(&tree);
                printf("\n");
                break;
            case 3:
                printf("\tSelected option --> Print IN-order\n");
                printInOrder(&tree);
                printf("\n");
                break;
            case 4:
                printf("\tSelected option --> Print POST-order\n");
                printPostOrder(&tree);
                printf("\n");
                break;
        }
    }

	//free(tree);
    
    return 0;
}

void insertNewNode(Node *tree, int newValue){
    if(tree->value == -1) { //This means that the tree is empty, so this is the first element
	    tree->value = newValue;
	    tree->rank = 0;
    } else {
        //This is no a new value, so we increment rank (# of children and decide if we send it to the left or to the right)
        tree->rank += 1;

        //printf("Pase1 [%i>%i]\n", tree->value, newValue);
        
        if( (tree->value) > newValue ) {
            //It must go to the left
            
            if( tree->leftChild == NULL ) {
                Node *leaf = getMemoryForNode();
                leaf->value = newValue;
                leaf->rank = 0;
                leaf->leftChild = NULL;
                leaf->rightChild = NULL;
                tree->leftChild = leaf;
            } else {
                insertNewNode(tree->leftChild, newValue);
            }
        } else {
            //It must go to the right
            if( tree->rightChild == NULL ) {
                Node *leaf = getMemoryForNode();
                leaf->value = newValue;
                leaf->rank = 0;
                leaf->leftChild = NULL;
                leaf->rightChild = NULL;
                tree->rightChild = leaf;
            } else {
                insertNewNode(tree->rightChild, newValue);
            }
        }
    }
}

void printPreOrder(Node *tree) {
    if(tree != NULL) {
        printf("%i[%i] ", tree->value, tree->rank);
        printPreOrder(tree->leftChild);
        printPreOrder(tree->rightChild);
    }
}

void printInOrder(Node *tree) {
    if(tree != NULL) {
        printPreOrder(tree->leftChild);
        printf("%i[%i] ", tree->value, tree->rank);
        printPreOrder(tree->rightChild);
    }
}

void printPostOrder(Node *tree) {
    if(tree != NULL) {
        printPreOrder(tree->leftChild);
        printPreOrder(tree->rightChild);
        printf("%i[%i] ", tree->value, tree->rank);
    }
}


Node *getMemoryForNode() {
	Node *aux = (Node *)malloc(sizeof(Node));
	aux->leftChild = NULL;
	aux->rightChild = NULL;

	return aux;
}
