#include "chashmap.h"
#include <unistd.h>

using namespace std;

HM* alloc_hashmap(size_t n_buckets) {
    HM* aux = (HM*)malloc(sizeof(*aux));
    if(!aux) {
        std::cerr << "Error when allocating memory" << std::endl;
    } else {
        aux->numBuckets = n_buckets;

        aux->buckets = (List **)malloc(n_buckets*sizeof(*(aux->buckets)));
        for(long unsigned int i = 0; i < n_buckets; i++){
            (aux->buckets)[i] = (List *)malloc(sizeof(*(aux->buckets)[i]));
        }

        aux->mutex = (cspinlock_t **)malloc(n_buckets*sizeof(cspinlock_t *));
        for(long unsigned int i = 0; i < n_buckets; i++){
            (aux->mutex)[i] = cspin_alloc();
        }
    }
    return aux;
}

void free_hashmap(HM* hm){
    int numBuckets = hm->numBuckets;
    for(int i = 0; i < numBuckets; i++){
        cspin_free(hm->mutex[i]);
    }
    free(hm);
}

void print_hashmap(HM* hm){
    int numBuckets = hm->numBuckets;
    for(int i = 0; i < numBuckets; i++){
        fprintf(stdout, "Bucket%i", i+1);
        std::vector<Node_HM> &sentinel = (hm->buckets[i])->sentinel;
        for (std::vector<Node_HM>::const_iterator i = sentinel.begin(); i != sentinel.end(); ++i) {
            fprintf(stdout, " - %li", (*i).m_val);
        }
        fprintf(stdout, "\n");
    }
}

int insert_item(HM* hm, long val) {
    int res = 0;
    int index = val%hm->numBuckets;

    cspin_lock((hm->mutex)[index]);

    Node_HM nodeAux;
    nodeAux.m_val = val;
    nodeAux.deleted = false;
    (((hm->buckets)[index])->sentinel).push_back(nodeAux);

    cspin_unlock((hm->mutex)[index]);
    return res;
}

int remove_item(HM* hm, long val) { 
    int indexBucket = val%hm->numBuckets;
    int indexItem = 0;

    cspin_lock((hm->mutex)[indexBucket]);

    std::vector<Node_HM> &sentinel = (hm->buckets[indexBucket])->sentinel;
    for (std::vector<Node_HM>::const_iterator i = sentinel.begin(); i != sentinel.end(); ++i, indexItem++) {
        if( (*i).m_val == val){
            sentinel.erase(sentinel.begin()+indexItem);
            break;
        }
    }

    cspin_unlock((hm->mutex)[indexBucket]);
    return 0;
}

int lookup_item(HM* hm, long val) { 
    int numBuckets = hm->numBuckets;
    int index = val%numBuckets;

    cspin_lock((hm->mutex)[index]);

    std::vector<Node_HM> &sentinel = (hm->buckets[index])->sentinel;
    for (std::vector<Node_HM>::const_iterator i = sentinel.begin(); i != sentinel.end(); ++i) {
        if( (*i).m_val == val) {
            cspin_unlock((hm->mutex)[index]);
            return 0;
        }
    }

    cspin_unlock((hm->mutex)[index]);
    return 1;
}



//##########################################################

//acquire the lock
int cspin_lock(cspinlock_t *slock) {
    usleep(5);
    while((slock->lock).fetch_add(1, memory_order_relaxed) != 0){
        usleep(20);
    }
    return 0;
}

//if the lock can not be acquired, return immediately
int cspin_trylock(cspinlock_t *slock){
    if(slock->lock) {
        return 0;
    }
    return ( slock->lock );
}

//release the lock
int cspin_unlock(cspinlock_t *slock) {
    if((slock->lock) > 0) {
        slock->lock = 0;
        return 0;
    }
    return 1;
}

//allocate a lock
cspinlock_t* cspin_alloc() {
    cspinlock_t *aux = (cspinlock_t *)malloc(sizeof(*aux));

    if(aux == NULL) {
        aux = NULL;
    } else {
        aux->lock = 0;
    }

    return aux;
}

//free a lock
void cspin_free(cspinlock_t* slock) {
    if(slock != NULL) free(slock);
    return ;
}


//#############################################################
