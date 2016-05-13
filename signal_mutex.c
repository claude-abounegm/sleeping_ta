/* 
 * File:   signal_mutex.c
 * Author: Claude Abounegm
 */

#include "signal_mutex.h"

void sig_initialize(signal_mutex* signal)
{
    signal->waiting = 0;
    signal->waitForSignal = 1;

    signal->mutex = malloc(sizeof (pthread_mutex_t));
    pthread_mutex_init(signal->mutex, NULL);

    signal->cond = malloc(sizeof (pthread_cond_t));
    pthread_cond_init(signal->cond, NULL);
}

int sig_is_waiting(signal_mutex* signal)
{
    int value;
    pthread_mutex_lock(signal->mutex);
    value = signal->waiting;
    pthread_mutex_unlock(signal->mutex);

    return value;
}

void sig_wait_for_signal(signal_mutex* signal)
{
    pthread_mutex_lock(signal->mutex);
    signal->waiting = 1;
    // at this point the mutex is locked
    while (signal->waitForSignal)
    {
        // while waiting, the mutex unlocks
        pthread_cond_wait(signal->cond, signal->mutex);
        // after waiting, the mutex locks
    }

    // change the signal to 1
    signal->waitForSignal = 1;
    signal->waiting = 0;
    
    // the mutex was locked after returning from the cond_wait, so we unlock it
    pthread_mutex_unlock(signal->mutex);
}

int sig_post_signal(signal_mutex* signal)
{
    int signaled = 0;
    // lock the mutex
    pthread_mutex_lock(signal->mutex);
    
    if (signal->waiting)
    {
        // we want the thread to stop waiting
        signal->waitForSignal = 0;
        
        // signal the thread to stop waiting
        pthread_cond_signal(signal->cond);
        
        signaled = 1;
    }
    
    // unlock the mutex
    pthread_mutex_unlock(signal->mutex);
    
    return signaled;
}

void sig_destroy(signal_mutex* signal)
{
    pthread_mutex_destroy(signal->mutex);
    free(signal->mutex);

    pthread_cond_destroy(signal->cond);
    free(signal->cond);
}