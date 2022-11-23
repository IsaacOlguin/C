#include "cspinlock.h"

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
    cspinlock_t *aux = (cspinlock_t *)malloc(sizeof(aux));

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