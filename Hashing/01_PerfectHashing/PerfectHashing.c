/*
PERFECT HASHING
    We can see a perfect Hashing as an injective function. e.i. a function one to one. 
    It means that per each element corresponds a specific space in the hash table (which is actually an array).
    This array has a fixed-size and thus this Data Structure has limited applications.

    The most important property is that its worst-case time is or order O(1) [constant time].

    The following develop application implements an array of lenght 52 (number of letters of upper and lower case from the English alphabet).

    GOAL:
        Implement a Perfect hashing with a simple example to learn by heart that the worst-case running time is O(1).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH_HASH_TABLE 52
#define OPTION_INSERT 1

void inicializeHashTable(char **alphabet);
void printHashTable(char **alphabet);//, char *strHeader, char *strBody
char getLetter(void);
void insertLetterIntoHashTable(char **alphabet, char letter);
int getIndexInHashTable(char letter);
void printLetterFromHashTable(char **alphabet, char letter);

int main() {
    printf("Willkommen zu Perfekt Hashing\n\n");

    int flag = 0;
    char *alphabet = NULL;

    //char strHeader = (char *)malloc(3*LENGTH_HASH_TABLE*sizeof(char));
    //char strBody = (char *)malloc(3*LENGTH_HASH_TABLE*sizeof(char));
    char car = '\0';

    while(flag >= 0 && flag <= 3) {
        printf("1) Insert a letter (from the English alphabet)\n");
        printf("2) Search (&print) a letter\n");
        printf("3) Print HashTable\n");
        printf("Type your choice: ");
        scanf("%i", &flag);

        switch(flag) {
            case OPTION_INSERT:
                car = getLetter();
                insertLetterIntoHashTable(&alphabet, car);
                break;
            case 2:
                car = getLetter();
                printLetterFromHashTable(&alphabet, car);
                break;
            case 3:
                printHashTable(&alphabet);
                break;
        }       
        printf("\n\n\n");
    }

    if(alphabet != NULL) free(alphabet);

    return 0;
}

void inicializeHashTable(char **alphabet) {
    int i;

    *alphabet = (char *)malloc(LENGTH_HASH_TABLE*sizeof(char));

    for (i = 0; i < LENGTH_HASH_TABLE; i++ ) {
        *(*alphabet+i) = ' ';
    }
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
    if (car >= 65 && car <= 90) {
        return car%65;
    } else if(car >= 97 && car <=122) { 
        return ((car%97)+26);
    } else {
        printf("\n\t\t=== ERROR AT LOOKING UP THE INDEX OF <<%c>> ===\n\n", car);
        return -1;
    }
}

void printHashTable(char **alphabet) {
    /*
    , char *strHeader, char *strBody
    printf("%s\n", strHeader);
    printf("%s\n\n", strBody);
    */
   int i;

    printf("\n");
    if( *alphabet == NULL ) {
        printf(">>>>The hash table is empty\n");
        return ;
    }

   for ( i = 0; i < LENGTH_HASH_TABLE; i++ ) {
       printf("\t%2i\t%c\n", i, *(*alphabet+i));
   }
}

void insertLetterIntoHashTable(char **alphabet, char letter) {
    int index;

    if( *alphabet == NULL ) { 
        //printf("Alphabet is NULL. I'll create the memory space\n");
        inicializeHashTable(&(*alphabet));
        //printf("I've created the memory space.\n");
    }
    
    index = getIndexInHashTable(letter);

    if ( index >= 0 && index <= 52) {
        *(*alphabet+index) = letter;
    }
}

void printLetterFromHashTable(char **alphabet, char letter) {
    int index;

    printf("\n");
    if( *alphabet == NULL ) {
        printf(">>>>The hash table is empty\n");
        return ;
    }
    
    index = getIndexInHashTable(letter);

    if ( *(*alphabet+index) == letter) {
        printf(">>>>The letter %c is at index %i\n", letter, *(*alphabet + index));
    } else {
        printf(">>>>The letter is not in the Hash table\n");
    }

    return ;
}
