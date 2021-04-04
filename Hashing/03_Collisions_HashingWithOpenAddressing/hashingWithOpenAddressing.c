/*
When a Hass table is implemented, the size of such table is limited and the possibilities are infinite. When an index of two differents is equal, it is called collision. There are ways to solve this problem and one of them is implement "Hashing with chaining", which basically implements a hash table an a linked list for each of the indexes. When a new element points to an index which is not empty, it is added into the linked list.
INSERT:
    Insertion is of order of O(1), since it is added immediately.
FIND/SEARCH:
    Searching the desired element has a worst-case of linear order. O(N) which is actually the number of elements of the linked list. In the worst scenario, searching requires to traverse the whole list.


Example: 
    >> insert numbers 4, 55, 37, 34, 7, 16, 23, 85, 61
    >> output:
Linear Probing:
	[55] [34] [23] [  ] [ 4] [37] [16] [ 7] [85] [61] [  ] 
Quadratic Probing:
	[55] [34] [  ] [  ] [ 4] [16] [61] [ 7] [37] [23] [85] 
Double Hashing:
	[55] [37] [  ] [  ] [ 4] [16] [34] [ 7] [85] [23] [61]
*/

#include <stdio.h>
#include <stdlib.h>

#define THREE_HASHINGS 3
#define LENGTH_HASH_TABLE 11
#define OPTION_INSERT 1
#define OPTION_PRINT_HT 2

#define C1 1
#define C2 3

enum TypesHwOA { LinearProb, QuadraticProb, DoubleHash };

int **initializeHashTable(void);
void getNumber(int *);
void insertNumberIntoHashTables(int **, int);
void insertLinearProbing(int *, int);
void insertQuadraticProbing(int *, int);
void insertDoubleHashing(int *, int);
void printHashTable(int **);

int main() {
    printf("Willkommen zu Collisions - Hashing with Open Addressing\n\n");

    int flag = 0, index;
    int **hashTable = NULL;
    int *number = NULL;

    if( hashTable == NULL ) {
        hashTable = initializeHashTable();
    } else {
        printf("I already have a memory location\n");
    }

    number = (int *)malloc(sizeof(int));

    while(flag >= 0 && flag <= 2) {
        printf("1) Insert a number\n");
        printf("2) Print HashTable\n");
        printf("Type your choice: ");
        scanf("%i", &flag);

        switch(flag) {
            case OPTION_INSERT:
                getNumber(number);
                insertNumberIntoHashTables(hashTable, *number);
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
    if(number != NULL){
        free(number);
    }

    return 0;
}

int **initializeHashTable() {
    int i, j;

    int **hashTable = (int **)malloc(THREE_HASHINGS*sizeof(int *));

    for(i = 0; i < THREE_HASHINGS; i++) {
        (*(hashTable+i)) = (int *)malloc(LENGTH_HASH_TABLE*sizeof(int));

        for(j = 0; j < LENGTH_HASH_TABLE; j++) 
            *((*(hashTable+i))+j) = -1;
    }

    return hashTable;
}

void getNumber(int *number) { 
    int flag = 1;
    while ( flag ){
        printf(">>Please insert the desired number (max 2 digits) to perform the operation: ");
        scanf("%i", number);

        flag = 0;
    }
}

void insertNumberIntoHashTables(int ** hashTable, int number) {
    insertLinearProbing(*(hashTable + LinearProb), number);

    insertQuadraticProbing(*(hashTable + QuadraticProb), number);

    insertDoubleHashing(*(hashTable + DoubleHash), number);
}

int getIndexInHashTable(char car) {
    if ((car >= 65 && car <= 90) || (car >= 97 && car <=122)) {
        return car%LENGTH_HASH_TABLE;
    } else {
        printf("\n\t\t=== ERROR AT LOOKING UP THE INDEX OF <<%c>> ===\n\n", car);
        return -1;
    }
}

void insertLinearProbing(int * linearHash, int number) {
    int flag = 0, i, index;

    for( i = 0; flag == 0 && i < LENGTH_HASH_TABLE+1; i++ ) {
        index = ((number+i)%LENGTH_HASH_TABLE);
        if ( *(linearHash + index) == -1 ) {
            *(linearHash + index) = number;
            flag = 1;
        }
    }
}

void insertQuadraticProbing(int *quadraticHash, int number) {
    int flag = 0, i, index;

    for( i = 0; flag == 0 && i < LENGTH_HASH_TABLE+1; i++ ) {
        index = (((number) + (C1*i) + (C2*(i*i))) %LENGTH_HASH_TABLE);
        //printf("Index [%i]\n", index);
        if ( *(quadraticHash + index) == -1 ) {
            *(quadraticHash + index) = number;
            flag = 1;
        }
    }
}

void insertDoubleHashing(int *doubleHash, int number) {
    int flag = 0, i, index;

    for( i = 0; flag == 0 && i < LENGTH_HASH_TABLE+1; i++ ) {
        index = ( number + i*( 1+(number%(LENGTH_HASH_TABLE-1)) ) ) %LENGTH_HASH_TABLE;
        //printf("Index [%i]\n", index);
        if ( *(doubleHash + index) == -1 ) {
            *(doubleHash + index) = number;
            flag = 1;
        }
    }
}

void printHashTable(int **hashTable) {
    int i,j;

    printf("\n");
    if( hashTable == NULL ) {
        printf(">>>>The hash table is empty\n");
        return ;
    }

    for(j=0; j < THREE_HASHINGS; j++) {
        if(j==LinearProb) printf("Linear Probing:\n\t");
        if(j==QuadraticProb) printf("Quadratic Probing:\n\t");
        if(j==DoubleHash) printf("Double Hashing:\n\t");
        for ( i = 0; i < LENGTH_HASH_TABLE; i++ ) {
            if(*((*(hashTable+j))+i) == -1)
                printf("[  ] ");
            else
                printf("[%2i] ", *((*(hashTable+j))+i));
        }
        printf("\n");
    }
}
