/* 
 * File:   queue.c
 * Author: Claude Abounegm
 */

#include "queue.h"

void queue_init(queue* q, int size)
{
    q->front = 0;
    q->count = 0;
    q->cap = size;
    q->arr = (int*) malloc(sizeof (int)*size);
    pthread_mutex_init(&q->lock, NULL);
}

void queue_free(queue* q)
{
    q->front = 0;
    q->count = 0;
    q->cap = 0;
    free(q->arr);
    pthread_mutex_destroy(&q->lock);
}

char queue_enqueue(queue* q, int value, int* _index)
{
    int retVal = -1;

    pthread_mutex_lock(&q->lock);
    if (q->count != q->cap)
    {
        int index = (q->front + q->count) % q->cap;
        q->arr[index] = value;
        retVal = ++q->count;
        *_index = index;
    }
    pthread_mutex_unlock(&q->lock);

    return retVal;
}

int* queue_dequeue(queue* q)
{
    int* value = NULL;

    pthread_mutex_lock(&q->lock);
    if (q->count != 0)
    {
        value = &q->arr[q->front];
        q->front = (q->front + 1) % q->cap;
        --q->count;
    }
    pthread_mutex_unlock(&q->lock);

    return value;
}