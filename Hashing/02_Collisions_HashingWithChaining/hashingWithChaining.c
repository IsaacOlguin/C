/*
When a Hass table is implemented, the size of such table is limited and the possibilities are infinite. When an index of two differents is equal, it is called collision. There are ways to solve this problem and one of them is implement "Hashing with chaining", which basically implements a hash table an a linked list for each of the indexes. When a new element points to an index which is not empty, it is added into the linked list.
INSERT:
    Insertion is of order of O(1), since it is added immediately.
FIND/SEARCH:
    Searching the desired element has a worst-case of linear order. O(N) which is actually the number of elements of the linked list. In the worst scenario, searching requires to traverse the whole list.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH_HASH_TABLE 10
#define OPTION_INSERT 1
#define OPTION_SEARCH 2
#define OPTION_PRINT_HT 3

typedef struct node {
    char character;
    int counter;
    struct node *nextNode;
} Node;

Node *initializeHashTable();
void printHashTable(Node *hashTable);
char getLetter(void);
void insertLetterIntoHashTable(Node *hashTable, char letter);
int getIndexInHashTable(char letter);
/*void printLetterFromHashTable(Node **hashTable, char letter);*/
void printLinkedList(Node *node);
void insertIntoLinkedList(Node *linkedList, Node *newNode);

int main() {
    printf("Willkommen zu Collisions - Hashing with Chaining\n\n");

    int flag = 0, index;
    Node *hashTable = NULL;

    if( hashTable == NULL ) {
        hashTable = initializeHashTable();
    } else {
        printf("I already have a memory location\n");
    }

    char car = '\0';

    while(flag >= 0 && flag <= 3) {
        printf("1) Insert a character\n");
        printf("2) Search (&print) a letter\n");
        printf("3) Print HashTable\n");
        printf("Type your choice: ");
        scanf("%i", &flag);

        switch(flag) {
            case OPTION_INSERT:
                car = getLetter();
                insertLetterIntoHashTable(hashTable, car);
                break;
            case OPTION_SEARCH:
                car = getLetter();
                //printLetterFromHashTable(&hashTable, car);
                break;
            case OPTION_PRINT_HT:
                printHashTable(hashTable);
                break;
        }       
        printf("\n\n\n");
    }

    if(hashTable != NULL){
        free(hashTable);
    }

    return 0;
}

Node *initializeHashTable() {
    int i;

    Node *hashTable = (Node *)malloc(LENGTH_HASH_TABLE*sizeof(Node));

    for(i = 0; i < LENGTH_HASH_TABLE; i++) {
        (*(hashTable+i)).character = ' ';
        (*(hashTable+i)).counter = 0;
        (*(hashTable+i)).nextNode = NULL;
    }

    return hashTable;
}

char getLetter(void) { 
    char car;
    while ( !(car >= 65 && car <= 90) && !(car >= 97 && car <=122) ){
        scanf("%c", &car);
        printf(">>Please insert the desired letter to perform the operation: ");
        scanf("%c", &car);

        if ( !(car >= 65 && car <= 90) && !(car >= 97 && car <=122) )
            printf("\n\t\tI have received <<%c[%i]>>. It IS NOT a valid input. Try it again.\n\n", car, car);
    }
    return car;
}

int getIndexInHashTable(char car) {
    if ((car >= 65 && car <= 90) || (car >= 97 && car <=122)) {
        return car%LENGTH_HASH_TABLE;
    } else {
        printf("\n\t\t=== ERROR AT LOOKING UP THE INDEX OF <<%c>> ===\n\n", car);
        return -1;
    }
}

void printHashTable(Node *hashTable) {
    int i;

    printf("\n");
    if( hashTable == NULL ) {
        printf(">>>>The hash table is empty\n");
        return ;
    }

    for ( i = 0; i < LENGTH_HASH_TABLE; i++ ) {
        printf("[%i] ", i);
        if((hashTable+i)->character == ' ') {
            printf("-> NULL\n");
        } else { 
            printLinkedList((hashTable+i));
        }
   }
}

void printLinkedList(Node *node) {
    if(node->nextNode == NULL) {
        printf("%c -> NULL\n", node->character);
        return ;
    } else {
        printf("%c -> ", node->character);
        printLinkedList(node->nextNode);
    }    
}

void insertLetterIntoHashTable(Node *hashTable, char letter) {
    int index = getIndexInHashTable(letter);

    if ( index >= 0 && index < LENGTH_HASH_TABLE ) {

        if( (hashTable+index)->character == ' ' ) {
            (hashTable+index)->character = letter;
        } else {
            //printf("En el indice seleccionado ya existe algo\n");
            Node *aux = (Node *)malloc(sizeof(Node));
            aux->counter = (hashTable+index)->counter+1;
            aux->character = letter;
            aux->nextNode = NULL;

            insertIntoLinkedList((hashTable+index), aux);
        }
    }
}

void insertIntoLinkedList(Node *linkedList, Node *newNode) {
    if( (*linkedList).nextNode == NULL ) {
        (*linkedList).nextNode = newNode;
    } else {
        insertIntoLinkedList(linkedList->nextNode, newNode);
    }
}

/*
void printLetterFromHashTable(Node **hashTable, char letter) {
    int index;

    printf("\n");
    if( *hashTable == NULL ) {
        printf(">>>>The hash table is empty\n");
        return ;
    }
    
    index = getIndexInHashTable(letter);

    
    if ( *(*hashTable+index) == letter) {
        printf(">>>>The letter %c is at index %i\n", letter, *(*hashTable + index));
    } else {
        printf(">>>>The letter is not in the Hash table\n");
    }
    

    return ;
}
*/