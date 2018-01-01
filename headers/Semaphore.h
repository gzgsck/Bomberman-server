#ifndef PROJECTBOOM_SEMAPHORE_H
#define PROJECTBOOM_SEMAPHORE_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

static struct sembuf buf;

void raiseSemaphore(int semid, int semnum, int value);

void lowerSemaphore(int semid, int semnum, int value);
#endif