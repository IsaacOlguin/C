#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

enum STATUS { UNUSED, USED };
enum POSITION { NORMAL, HEAD };

typedef struct elem_heap {
  int size;
  int status;
  void *currentAddress;
  struct elem_heap *next;
} ElemHeap;

extern ElemHeap *heapList;

void *extend_heap_size(size_t size);
void *malloc(size_t size);
void *calloc(int nitems, size_t nsize);
void free(void *ptr);
void *realloc(void *ptr, size_t size);
ElemHeap *initElement();
void *findBlock(size_t size);
void insertIntoTheList(ElemHeap *heap, ElemHeap *element);