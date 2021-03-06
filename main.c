/* 
 * File:   main.c
 * Author: Claude Abounegm
 *
 * The Sleeping Teaching Assistant
 */

#include "main.h"

int main(int argc, char *argv[])
{
    int studentsCount = -1;

    // parse the number of students
    printf("Please enter the number of students: ");
    scanf("%d", &studentsCount);
    if (studentsCount <= 0)
    {
        printf("Not a valid number; aborting.\n");
        return 1;
    }
    
    // we need to initialize the students' array
    students = (student*) malloc(sizeof (student) * studentsCount);

    // set a seed for the random number generator
    rand_ts_initialize(&rSeed, time(NULL));

    // the wakeup signal, to wake up the TA
    sig_initialize(&wakeupSignal);
    sem_init(&taHelping, 0, 0);

    // initialize the queue
    queue_init(&chairsQueue, AVAILABLE_CHAIRS);

    // the threads that are going to be used
    pthread_t* student_threads = (pthread_t*) malloc(sizeof (pthread_t) * studentsCount);
    pthread_t ta_thread;

    int i;

    // initialize the chairs' semaphores
    for (i = 0; i < AVAILABLE_CHAIRS; ++i)
        sem_init(&chairs_sems[i], 0, 0);

    // create the TA thread before the student threads
    pthread_create(&ta_thread, 0, &taFn, NULL);

    // students threads
    for (i = 0; i < studentsCount; ++i)
    {
        student* s = &students[i];
        s->id = (i + 1);
        s->waitSem = NULL;

        // create the student thread, and pass the student struct as a pointer.
        pthread_create(&student_threads[i], 0, &studentFn, s);
    }

    // wait for the threads to end. a.k.a never.
    for (i = 0; i < studentsCount; ++i)
        pthread_join(student_threads[i], NULL);
    pthread_join(ta_thread, NULL);

    // cleanup resources
    for (i = 0; i < AVAILABLE_CHAIRS; ++i)
        sem_destroy(&chairs_sems[i]);

    rand_ts_destroy(&rSeed);
    free(students);
    free(student_threads);
    queue_free(&chairsQueue);
    sig_destroy(&wakeupSignal);
    sem_destroy(&taHelping);

    return 0;
}

void* studentFn(void* arg)
{
    student* s = (student*) arg;

    while (1)
    {
        // we first get a random programming time, and wait for that long.
        int progTime = getRandomProgrammingTime();
        STUDENT_PRINT("I am programming for %d seconds.", progTime);
        sleep(progTime);
        STUDENT_PRINT("I need help from the TA, HELP.");

        // enqueue the id to the queue.
        int index;
        int count = queue_enqueue(&chairsQueue, s->id, &index);

        if (count != -1)
        {
            // if there is only one student in the chair, that means either: 
            //    1. the TA is sleeping, and we are next to work with him.
            //    2. the TA is helping another student.
            // if when we signal wake-up to the TA, the TA wakes up, then we
            // notify the user that we woke up the TA, otherwise, we wait in
            // the chair until the TA is done with the current student.
            if (count == 1 && sig_post_signal(&wakeupSignal))
                STUDENT_PRINT("TA is sleeping; waking him up.");
            else
                STUDENT_PRINT("TA is not available, got chair #%d. %d", count, index);

            // we get the chair semaphore and give it to the student
            s->waitSem = &chairs_sems[index];
            sem_wait(s->waitSem);
            s->waitSem = NULL;

            // the TA gave us a signal that they are helping us
            STUDENT_PRINT_COLOR(BLU, RESET, "The TA is currently helping me.");
            sem_wait(&taHelping);
            STUDENT_PRINT_COLOR(BLU, RESET, "The TA has finished helping me.");
        }
        else
            // if the enqueue-ing returns -1, then the queue is full, so we abort.
            STUDENT_PRINT("There are no chairs available, back to programming!");
    }

    return NULL;
}

void* taFn(void* arg)
{
    while (1)
    {
        int* studentId;
        TA_PRINT("I am sleeping, wake me up when you need me!");
        sig_wait_for_signal(&wakeupSignal);

        // we dequeue the student immediately, as he is not on a chair,
        // and we do not want to take the place of someone else.
        studentId = queue_dequeue(&chairsQueue);
        TA_PRINT("I am awake now :(");

        do
        {
            student* s = &students[*studentId - 1];

            // just in case the semaphore was not set yet, so we try to wait
            // until it's been set.
            while (!s->waitSem);
            // notify the student that their turn is now.
            sem_post(s->waitSem);

            // wait for the help time.
            int waitTime = getRandomWaitTime();
            TA_PRINT("Helping student #%d for %d seconds.", *studentId, waitTime);
            sleep(waitTime);

            // notify the student that we're done helping them.
            sem_post(&taHelping);

            // tiny wait to allow the student to print out debugging
            // statements before the next student does.
            usleep(1000);
        } while ((studentId = queue_dequeue(&chairsQueue)) != NULL);
    }

    return NULL;
}

int getRandomProgrammingTime()
{
    return GET_RANDOM_WAIT(MIN_PROGRAMMING_TIME, MAX_PROGRAMMING_TIME);
}

int getRandomWaitTime()
{
    return GET_RANDOM_WAIT(MIN_HELP_TIME, MAX_HELP_TIME);
}