/* 
 * File:   main.h
 * Author: Claude Abounegm
 */

#ifndef PROJECT3_H
#define PROJECT3_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#include "signal_mutex.h"
#include "queue.h"
#include "rand_ts.h"

typedef struct _student
{
    int id;
    sem_t* waitSem;
} student;

// some codes for console colors
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define BLU   "\x1B[34m"
#define RESET "\x1B[0m"

// prints to the console the desired message with [TA]: at the beginning
// of the format, and with the desired colors.
#define TA_PRINT_COLOR(color1, color2, format, args...) \
            printf(color1 "[TA]: " color2 format RESET "\n", ##args)
#define TA_PRINT(format, args...) \
            TA_PRINT_COLOR(GRN, RESET, format, ##args)

// prints to the console the desired message with [Student {id}]: at the beginning
// of the format, and with the desired colors.
#define STUDENT_PRINT_COLOR(color1, color2, format, args...) \
            printf(color1 "[Student %d]: " color2 format RESET "\n", s->id, ##args)
#define STUDENT_PRINT(format, args...) \
            STUDENT_PRINT_COLOR(RED, RESET, format, ##args)

#define GET_RANDOM_WAIT(min, max) \
           (min + (rand_ts_r(&rSeed) % (max - min + 1)))

// the number of chairs outside the office
#define AVAILABLE_CHAIRS 3

// a student programs between 6 and 15 seconds
#define MIN_PROGRAMMING_TIME 6
#define MAX_PROGRAMMING_TIME 15

// the time the TA takes to help a student
#define MIN_HELP_TIME 5
#define MAX_HELP_TIME 10

// START: function signatures

/* 
 * Gets a random programming time between MIN_PROGRAMMING_TIME
 * and MAX_PROGRAMMING_TIME.
 */
int getRandomProgrammingTime();

/* 
 * Gets a random wait time between MIN_HELP_TIME
 * and MAX_HELP_TIME.
 */
int getRandomWaitTime();

// The function which handles each student thread.
void* studentFn(void* arg);

// The function which handles the TA thread.
void* taFn(void* arg);
// END: function signatures

// START: GLOBAL VARS
rand_ts rSeed;
student* students;

// semaphores and signal mutex
signal_mutex wakeupSignal;
sem_t taHelping;
sem_t chairs_sems[AVAILABLE_CHAIRS];

// a thread safe queue
queue chairsQueue;
// END: GLOBAL VARS

#endif
