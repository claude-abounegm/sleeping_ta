/* 
 * File:   queue.h
 * Author: Claude Abounegm
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct queue
{
    int front;
    int count;
    int cap;
    int* arr;
    
    pthread_mutex_t lock;
} queue;

void queue_init(queue* q, int size);
void queue_free(queue* q);
char queue_enqueue(queue* q, int value);
int* queue_dequeue(queue* q);

#endif
