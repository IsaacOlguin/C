/*
To compile the code, it's required to execute the following command line (with the header "-lpthread")
    >> gcc multithreadWithError.c -lpthread -o test

What this code basically does is:
    - It shows how can be used multithreads and,
    - How its required to used a strategy to not overlap values of global variables

OUTPUT:
    >> Job 1 has started
    >> Job 2 has started
    >> Job 2 has finished
    >> Job 2 has finished
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

pthread_t tid[2];
int counter;

void *trythis(void *arg) { 
    unsigned long i = 0;
    counter += 1;
    printf("\nJob %d has started\n", counter);

    for( i = 0; i < (0xFFFFFFFF); i++)
        ;
    printf("\nJob %d has finished\n", counter);

    return NULL;
}

int main(void) {
    int i = 0;
    int error;

    while ( i < 2 ) {
        error = pthread_create( &(tid[i]), NULL, &trythis, NULL);
        if (error != 0) {
            printf("\nThread can't be created: [%s]", strerror(error));
        }
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    return 0;
}