#include "utils.h"

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))
#define SIZE_SPLIT 5
#define LIMIT_UNABLE_MUTEX_INDEX 100

//ElemHeap *heapList;
pthread_mutex_t mutex[SIZE_SPLIT];// = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexMemory = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexIndex = PTHREAD_MUTEX_INITIALIZER;

ElemHeap **heapListArray = NULL;
int idsArray[] = {0,0,0,0,0};
int counterFlag = 0, flagWithoutMutex = 0;
//int idsArrayCounters[] = {0,0,0,0,0};


int getIndexArray(int dir){
  counterFlag++;
  if(counterFlag < LIMIT_UNABLE_MUTEX_INDEX){ pthread_mutex_lock(&mutexIndex); flagWithoutMutex = 1;}
    
  if(heapListArray == NULL ){heapListArray = (ElemHeap **)sbrk(SIZE_SPLIT*sizeof(ElemHeap*));}
  for(int i = 0; i < SIZE_SPLIT; i++){
    if(idsArray[i] == 0){
      //fprintf(stderr, "New Dir [%i], at pos [%i]\n", dir, i);
      idsArray[i] = dir;
      //idsArrayCounters[i]++;
      //mutex[i] = PTHREAD_MUTEX_INITIALIZER;
      pthread_mutex_init(&mutex[i], NULL);
      if(counterFlag < LIMIT_UNABLE_MUTEX_INDEX){ pthread_mutex_unlock(&mutexIndex); }
      return i;
    } else if(idsArray[i] == dir) {
      //idsArrayCounters[i]++;
      if(counterFlag < LIMIT_UNABLE_MUTEX_INDEX){ pthread_mutex_unlock(&mutexIndex); }
      return i;
    }
  }
  //fprintf(stderr, "ERROR in array's index. Dir not allocated [%i]\n", dir);

  //exit(1);
  if(counterFlag < LIMIT_UNABLE_MUTEX_INDEX){ pthread_mutex_unlock(&mutexIndex); }
  return -1;
}

int getIndexArrayWithOutMutex(int dir){    
  if(heapListArray == NULL ){heapListArray = (ElemHeap **)sbrk(SIZE_SPLIT*sizeof(ElemHeap*));}
  for(int i = 0; i < SIZE_SPLIT; i++){
    if(idsArray[i] == 0){
      fprintf(stderr, "New Dir [%i], at pos [%i]\n", dir, i);
      idsArray[i] = dir;
      //idsArrayCounters[i]++;
      return i;
    } else if(idsArray[i] == dir) {
      //idsArrayCounters[i]++;
      return i;
    }
  }
  fprintf(stderr, "ERROR in array's index. Dir not allocated [%i]\n", dir);
  return -1;
}

void *extend_heap_size(size_t size) {
  void *current_base = sbrk(0);
  void *extended = sbrk(size);
  assert(extended != (void *)-1); // Note: not thread-safe

  /*
   * Modify this function according to your needs
   */

  return extended;
}

void *malloc(size_t size) {
  int idP = pthread_self();
  if(flagWithoutMutex)
    idP = getIndexArrayWithOutMutex(idP);
  else
    idP = getIndexArray(idP);
  //fprintf(stderr, "%3i %3i %3i %3i %3i\n", idsArrayCounters[0], idsArrayCounters[1], idsArrayCounters[2], idsArrayCounters[3], idsArrayCounters[4]);
  ElemHeap *auxElemHeap = NULL;
  void *ptrAux = NULL;
  /*
   * Insert malloc implementation here
   */
  if (size <= 0) return NULL;

  //Size aligment is computed
  size = ALIGN(size);

  if(heapListArray[idP] == NULL){
    auxElemHeap = (ElemHeap *)initElement();
    auxElemHeap->size = size;
  
    auxElemHeap->status = USED;
    auxElemHeap->next = NULL;
    pthread_mutex_lock(&mutexMemory);
    auxElemHeap->currentAddress = sbrk(auxElemHeap->size);
    pthread_mutex_unlock(&mutexMemory);

    heapListArray[idP] = auxElemHeap;
  } else { 
    pthread_mutex_lock(&mutex[idP]);
    ptrAux = findBlock(size);
    //fprintf(stderr, "DirGotten is [%p]\n", ptrAux);

    if(ptrAux == NULL) {
      auxElemHeap = (ElemHeap *)initElement();
      auxElemHeap->status = USED;
      auxElemHeap->next = NULL;
      auxElemHeap->size = size;
      pthread_mutex_lock(&mutexMemory);
      auxElemHeap->currentAddress = sbrk(auxElemHeap->size);
      pthread_mutex_unlock(&mutexMemory);
      insertIntoTheList(heapListArray[idP], auxElemHeap);
    } else {
      pthread_mutex_unlock(&mutex[idP]);
      return ptrAux;
    }
  }
  pthread_mutex_unlock(&mutex[idP]);
  return auxElemHeap->currentAddress;
}

void *calloc(int nitems, size_t nsize) {
  /*
   * Insert calloc implementation here
   */
  int totalMemory = nitems*nsize;
  void *ptrAux = NULL;

  /*
   * Insert malloc implementation here
   */
  if (totalMemory <= 0) return NULL;

  //Size aligment is computed
  totalMemory = ALIGN(totalMemory);

  ptrAux = malloc(totalMemory);
  return ptrAux;
}

void free(void *ptr) {
  /*
   * Insert free implementation here
   */
  int idP = pthread_self();
  if(flagWithoutMutex)
    idP = getIndexArrayWithOutMutex(idP);
  else
    idP = getIndexArray(idP);
  ElemHeap *aux = (ElemHeap *)heapListArray[idP];
  int numIter = 0;
  while(aux != NULL) {
    numIter++;
    if(aux->currentAddress != NULL) {
      if(aux->currentAddress==ptr){
        aux->status = UNUSED;
        return ;
      }

      aux = (ElemHeap *)aux->next;
    }
  }

  return;
}

void *realloc(void *ptr, size_t size) {
  /*
   * Insert realloc implementation here
   */

  void *newPtr = malloc(size);

  if(!newPtr) return NULL;

  int idP = pthread_self();
  if(flagWithoutMutex)
    idP = getIndexArrayWithOutMutex(idP);
  else
    idP = getIndexArray(idP);
  ElemHeap *aux = heapListArray[idP];
  while(aux != NULL) {
    if(aux->currentAddress==ptr){
      size_t newSize = size > aux->size ? aux->size : size;
      memcpy(newPtr, ptr, newSize);
      break;
    }

    aux = aux->next;
  }

  free(ptr);

  return newPtr;
}

ElemHeap *initElement(){
  pthread_mutex_lock(&mutexMemory);
  ElemHeap *ptr = sbrk(ALIGN(sizeof(ElemHeap)));
  pthread_mutex_unlock(&mutexMemory);
  ptr->currentAddress = NULL;
  ptr->next = NULL;
  ptr->size = 0;
  ptr->status = UNUSED;
  return ptr;
}


void *findBlock(size_t size){
  ElemHeap *ptrAuxLoop = NULL, *ptrAuxChosen = NULL;

  if(size <= 0) {
    return NULL; 
  }

  if (heapListArray == NULL) { 
    return NULL; 
  }

    //fprintf(stderr, "findBlock with size [%li] is required\n", size); 

  /*
    if(heapList->status == UNUSED && size <= heapList->size) {
      fprintf(stderr, "heapList[%p]'s UNUSED & hasSize[%li] (We'reLookingFor [%li]) [%p]\n", heapList, heapList->size, size, heapList->currentAddress); 
      heapList->status = USED; // Check if its done at this point. This is changed considering that this function is executing with the goal to allocate new spaces
      //heapList->size = size;
      return heapList->currentAddress;
    }*/
////////////////////////////////////////////////////////////////////////////////////////////////////
    int idP = pthread_self();
    if(flagWithoutMutex)
      idP = getIndexArrayWithOutMutex(idP);
    else
      idP = getIndexArray(idP);
    ptrAuxLoop = (ElemHeap *)heapListArray[idP];

    //fprintf(stderr, "Verifica que tenga espacio directo sin hacer merge\n");
    while(ptrAuxLoop != NULL) {
      /*
      if(ptrAuxLoop->status != USED){
        fprintf(stderr, "ptrAuxLoop[%p]'s US/UN[%i] & hasSize[%li] (We'reLookingFor [%li]) [%p]\n", ptrAuxLoop, ptrAuxLoop->status, ptrAuxLoop->size, size, ptrAuxLoop->currentAddress); 
      }*/
      if(ptrAuxLoop->status == UNUSED && size <= ptrAuxLoop->size){
        if(ptrAuxChosen == NULL) {
          ptrAuxChosen = ptrAuxLoop;
        } else if(ptrAuxLoop->size < ptrAuxChosen->size) {
          //fprintf(stderr, "ptrAuxLoop[%li] < ptrAuxChosen[%li] * [%li]\n", ptrAuxLoop->size, ptrAuxChosen->size, size); 
          ptrAuxChosen = ptrAuxLoop;
        }
      }
      ptrAuxLoop = ptrAuxLoop->next;
    }

    if(ptrAuxChosen != NULL){
      //fprintf(stderr, ".Chosen [%li]\n", ptrAuxChosen->size);
      ptrAuxChosen->status = USED;
      return ptrAuxChosen->currentAddress;
    }

////////////////////////////////////////////////////////////////////////////////////////////////////
    //fprintf(stderr, "Verifica que haya elementos consecutivos para hacer merge\n");

    ptrAuxLoop = (ElemHeap *)heapListArray[idP];
    size_t freeSpace = 0;
    ElemHeap *ptrAuxPolicySum = NULL;

    while(ptrAuxLoop != NULL) {
      if(ptrAuxLoop->status == UNUSED) {
        //fprintf(stderr, "AddPtrLL [%p]'s St[%i] & hasSize[%li] (LookingFor [%li]) [%p]\n", ptrAuxLoop, ptrAuxLoop->status, ptrAuxLoop->size, size, ptrAuxLoop->currentAddress); 
        //freeSpace = ptrAuxLoop->size;
        freeSpace = sizeof(ptrAuxLoop);
        //fprintf(stderr, "freeSize [%li] < desiredSize [%li]\n", freeSpace, size); 
        ptrAuxChosen = ptrAuxLoop->next;
        while(ptrAuxChosen != NULL && freeSpace <= size && ptrAuxChosen->status == UNUSED){
          //fprintf(stderr, "-AddPtrLL [%p]'s St[%i] & hasSize[%li] (LookingFor [%li]) [%p]\n", ptrAuxChosen, ptrAuxChosen->status, ptrAuxChosen->size, size, ptrAuxChosen->currentAddress); 
          freeSpace += ptrAuxChosen->size;
          ptrAuxPolicySum = ptrAuxChosen;
          ptrAuxChosen = ptrAuxChosen->next;
        }
        //fprintf(stderr, "freeSize [%li] < desiredSize [%li]\n", freeSpace, size); 

        if(size<=freeSpace) {
//#ifdef DEBUG_PRINT_INFO_PTR
          //fprintf(stderr, "enough space [%li<%li] aligned is [%li]\n", size, freeSpace, ALIGN(freeSpace));
//#endif
          ptrAuxLoop->size = ALIGN(freeSpace);
          ptrAuxLoop->next = ptrAuxPolicySum->next;
          ptrAuxLoop->status = USED;

          return ptrAuxLoop->currentAddress;
        } else {
          freeSpace = 0;
          if(ptrAuxChosen != NULL)
            ptrAuxLoop = ptrAuxChosen->next;
          else 
            return NULL;
        }
      } else {
        ptrAuxLoop = ptrAuxLoop->next;
      }
    }
    return NULL;
}

void insertIntoTheList(ElemHeap *heap, ElemHeap *element){
  if(heap->next == NULL) {
    element->next = NULL;
    heap->next = element;
  } else {
    insertIntoTheList(heap->next, element);
  }
}