#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <atomic>
#include <pthread.h>
#include <vector>
#include <algorithm>    // std::sort

#define PAD 64

/*
Hashmap: list of buckets
bucket1 -> sentinel -> node1 -> node2 -> NULL
bucket2 -> sentinel -> node3 -> NULL
...
bucketN -> sentinel -> NULL
*/
//##################################################################
//##################################################################
//define a node in the hashmap
typedef struct Node_HM_t
{
	long m_val; //value of the node
	bool deleted;
} Node_HM;
//typedef struct Node_HM_t Node_HM;
//##################################################################
//##################################################################
//defining a bucket in the hashmap
typedef struct List_t
{
	std::vector<Node_HM> sentinel; //list of nodes in a bucket
} List;
//typedef struct List_t List;
//##################################################################
//##################################################################
typedef struct cspinlock {
    std::atomic_int lock;
}cspinlock_t;

//defining the hashmap
typedef struct hm_t
{
        List** buckets; //list of buckets in the hashmap
		int numBuckets;
		std::atomic_int numElems;
		cspinlock_t **mutex;
		pthread_mutex_t **lock;
} HM;
//typedef struct hm_t HM;
//##################################################################
//##################################################################
//allocate a hashmap with given number of buckets
HM* alloc_hashmap(size_t n_buckets);
//##################################################################
//##################################################################
//free a hashamp
void free_hashmap(HM* hm);
//##################################################################
//##################################################################
//insert val into the hm and return 0 if successful
//return 1 otherwise, e.g., could not allocate memory
int insert_item(HM* hm, long val);
//##################################################################
//##################################################################
//remove val from the hm, if it exist and return 0 if successful
//return 1 if item is not found
int remove_item(HM* hm, long val);
//##################################################################
//##################################################################
//check if val exists in hm, return 0 if found, return 1 otherwise
int lookup_item(HM* hm, long val);
//##################################################################
//##################################################################
//print all elements in the hashmap as follows:
//Bucket 1 - val1 - val2 - val3 ...
//Bucket 2 - val4 - val5 - val6 ...
//Bucket N -  ...
void print_hashmap(HM* hm);

//##################################################################
//##################################################################
//##################################################################

//acquire the lock
int cspin_lock(cspinlock_t *slock);
//if the lock can not be acquired, return immediately
int cspin_trylock(cspinlock_t *slock);
//release the lock
int cspin_unlock(cspinlock_t *slock);
//allocate a lock
cspinlock_t* cspin_alloc();
//free a lock
void cspin_free(cspinlock_t* slock);
//##################################################################
//##################################################################
//##################################################################
