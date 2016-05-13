/* 
 * File:   Student.h
 * Author: Claude Abounegm
 */

#ifndef STUDENT_H
#define STUDENT_H

typedef struct Student
{
    int id;
    sem_t studentWaiting;
} Student;

#endif /* STUDENT_H */

