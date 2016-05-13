#include "rand_ts.h"

void rand_ts_initialize(rand_ts* r, unsigned int seed)
{
    r->seed = seed;
    pthread_mutex_init(&r->lock, NULL);
}

int rand_ts_r(rand_ts* r)
{
    int val;
    pthread_mutex_lock(&r->lock);
    val = rand_r(&r->seed);
    pthread_mutex_unlock(&r->lock);

    return val;
}

void rand_ts_destroy(rand_ts* r)
{
    r->seed = 0;
    pthread_mutex_destroy(&r->lock);
}