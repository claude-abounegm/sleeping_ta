/* 
 * File:   rand_ts.h
 * Author: Claude Abounegm
 *
 */

#ifndef RAND_TS_H
#define RAND_TS_H

#include <pthread.h>
#include <stdlib.h>

typedef struct rand_ts 
{
    unsigned int seed;
    pthread_mutex_t lock;
} rand_ts;

void rand_ts_initialize(rand_ts* r, unsigned int seed);
int rand_ts_r(rand_ts* r);
void rand_ts_destroy(rand_ts* r);

#endif