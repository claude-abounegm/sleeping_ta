/* 
 * File:   signal_mutex.h
 * Author: Claude Abounegm
 */

#ifndef SIGNAL_MUTEX_H
#define SIGNAL_MUTEX_H

#include <stddef.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct signal_mutex
{
    pthread_cond_t* cond;
    pthread_mutex_t* mutex;
    int waitForSignal;
    int waiting;
} signal_mutex;

void sig_initialize(signal_mutex* signal);
int sig_is_waiting(signal_mutex* signal);
void sig_wait_for_signal(signal_mutex* signal);
int sig_post_signal(signal_mutex* signal);
void sig_destroy(signal_mutex* signal);

#endif