#include "chashmap.h"
using namespace std;

//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int numElementosPrint = 0;

HM* alloc_hashmap(size_t n_buckets) {
    HM* aux = (HM*)malloc(sizeof(*aux));
    if(!aux) {
        std::cerr << "Error when allocating memory" << std::endl;
    } else {
        aux->numBuckets = n_buckets;
#ifdef DEBUG_PRINT
        std::cerr << "It has been allocated a HM of [" << aux->numBuckets << "] of size" << std::endl;
#endif
        try {
            aux->buckets = (List **)malloc(n_buckets*sizeof(*(aux->buckets)));
            for(long unsigned int i = 0; i < n_buckets; i++){
                (aux->buckets)[i] = (List *)malloc(sizeof(*(aux->buckets)[i]));
            }

            aux->lock = (pthread_mutex_t **)malloc(n_buckets*sizeof(pthread_mutex_t *));
            for(int i = 0; i < n_buckets; i++){
                (aux->lock)[i] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
                pthread_mutex_init((aux->lock)[i], NULL);
            }
        } catch(exception& exc) {
            fprintf(stderr, "[Exception] %s\n", exc.what());
        }

        //aux->mutex = cspin_alloc();
        //int rc = pthread_mutex_init(&(aux->lock), NULL); 
        //assert(rc == 0);
    }
    return aux;
}

void free_hashmap(HM* hm){
    free(hm);
}

void print_hashmap(HM* hm){
    int numTotalElems = 0;
    int numBuckets = hm->numBuckets;
    //fprintf(stderr, "NumBuckets[%i]\n", numBuckets);
    for(int i = 0; i < numBuckets; i++){
#ifdef DEBUG_PRINT
        //fprintf(stderr, "Bucket%i", i+1);
#endif
        try {
            //fprintf(stderr, "Bucket%i", i+1);
            fprintf(stdout, "Bucket%i", i+1);
            std::vector<Node_HM> sentinel = (hm->buckets[i])->sentinel;
            for (std::vector<Node_HM>::const_iterator i = sentinel.begin(); i != sentinel.end(); ++i) {
                if(!(*i).deleted) {
                    //std::cerr << " - " << (*i).m_val ;
                    //fprintf(stderr, " - %li", (*i).m_val);
                    fprintf(stdout, " - %li", (*i).m_val);
                    numTotalElems++;
                }
            }
            //fprintf(stderr, "\n");
            fprintf(stdout, "\n");
            //fprintf(stderr, "NumTotalElems [%i]\n", numTotalElems);

        } catch(exception& exc) {
            fprintf(stderr, "[Exception] %s\n", exc.what());
        }
    }

}

int insert_item(HM* hm, long val) {
    //cspin_lock(hm->mutex);
    //pthread_mutex_lock(&(hm->lock));

    int res = 0;
    int index = val%hm->numBuckets;
    pthread_mutex_lock((hm->lock)[index]);

#ifdef DEBUG_PRINT
    fprintf(stderr, "Inserting [%li] at bucket[%i]\n", val, index);
#endif
    try {
        Node_HM nodeAux;
        nodeAux.m_val = val;
        nodeAux.deleted = false;
        (((hm->buckets)[index])->sentinel).push_back(nodeAux);
    } catch(exception& exc) {
        fprintf(stderr, "[Exception] %s\n", exc.what());
        res = -1;
    }
    //cspin_unlock(hm->mutex);
    //pthread_mutex_unlock(&(hm->lock));
    pthread_mutex_unlock((hm->lock)[index]);
    return res;
}

int remove_item(HM* hm, long val) { 
    //cspin_lock(hm->mutex);
    //pthread_mutex_lock(&(hm->lock));
    int indexBucket = val%hm->numBuckets;
    int indexItem = 0;

    pthread_mutex_lock((hm->lock)[indexBucket]);

#ifdef DEBUG_PRINT
    fprintf(stderr, "Deleting [%li] at bucket[%i]\n", val, indexBucket);
#endif

    try {
        if( ((hm->buckets[indexBucket])) != NULL ) {
            std::vector<Node_HM> sentinel = (hm->buckets[indexBucket])->sentinel;
            for (std::vector<Node_HM>::const_iterator i = sentinel.begin(); i != sentinel.end(); ++i, indexItem++) {
                if( !((*i).deleted) && (*i).m_val == val){
                    //fprintf(stderr, "CONDITIONS FULFILLED - Deleting [%li] at bucket[%i] at index [%i]\n", val, indexBucket, indexItem);
                    ((hm->buckets[indexBucket])->sentinel).at(indexItem).deleted = true;//!(((hm->buckets[indexBucket])->sentinel).at(indexItem).deleted ^ true);//true;//;
                    break;
                }
            }
        } else {
            fprintf(stderr, "Sentinel is NULL\n");
        }
    } catch(exception& exc) {
        fprintf(stderr, "[Exception] %s\n", exc.what());
        pthread_mutex_unlock((hm->lock)[indexBucket]);
        return -1;
    }
    //cspin_unlock(hm->mutex);
    //pthread_mutex_unlock(&(hm->lock));
    pthread_mutex_unlock((hm->lock)[indexBucket]);
    return 0;
}

int lookup_item(HM* hm, long val) { 
    //pthread_mutex_lock(&(hm->lock));
    int numBuckets = hm->numBuckets;
    int index = val%numBuckets;

    pthread_mutex_lock((hm->lock)[index]);
    
#ifdef DEBUG_PRINT
    fprintf(stderr, "LookingUp [%li] at bucket[%i]\n", val, index);
#endif
    try {
        std::vector<Node_HM> sentinel = (hm->buckets[index])->sentinel;
        for (std::vector<Node_HM>::const_iterator i = sentinel.begin(); i != sentinel.end(); ++i) {
            if( !((*i).deleted) && (*i).m_val == val) {
                //pthread_mutex_unlock(&(hm->lock));
                pthread_mutex_unlock((hm->lock)[index]);
                return 0;
            }
        }
    } catch(exception& exc) {
        fprintf(stderr, "[Exception] %s\n", exc.what());
        //pthread_mutex_unlock(&(hm->lock));
        pthread_mutex_unlock((hm->lock)[index]);
        return -1;
    }
    //pthread_mutex_unlock(&(hm->lock));
    pthread_mutex_unlock((hm->lock)[index]);
    return 1;
}



//##########################################################

//acquire the lock
int cspin_lock(cspinlock_t *slock) {
#ifdef DEBUG_PRINT
    std::cerr << "Locking cspinlock_t with address [" << slock << "] and value [" << slock->lock << "]" << std::endl;
#endif
    while(slock->lock);
    return (slock->lock = 1);
}

//if the lock can not be acquired, return immediately
int cspin_trylock(cspinlock_t *slock){
#ifdef DEBUG_PRINT
    std::cerr << "Trying locking cspinlock_t with address [" << slock << "] and value [" << slock->lock << "]" << std::endl;
#endif
    if(slock->lock) {
        return 0;
    }
    return (slock->lock = 1);
}

//release the lock
int cspin_unlock(cspinlock_t *slock) {
#ifdef DEBUG_PRINT
    std::cerr << "Unlocking cspinlock_t" << std::endl;
#endif
    return (slock->lock = 0);
}

//allocate a lock
cspinlock_t* cspin_alloc() {
    cspinlock_t *aux = (cspinlock_t *)malloc(sizeof(*aux));

    if(aux == NULL) {
#ifdef DEBUG_PRINT
        std::cerr << "Error allocating cspin" << std::endl;
#endif
        aux = NULL;
    } else {
        aux->lock = 0;
    }

    return aux;
}

//free a lock
void cspin_free(cspinlock_t* slock) {
#ifdef DEBUG_PRINT
    std::cerr << "Free cspinlock_t" << std::endl;
#endif
    free(slock);
    return ;
}


//#############################################################
