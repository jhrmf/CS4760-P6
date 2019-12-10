#ifndef SHAREDMEM_H
#define SHAREDMEM_H

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <pthread.h>

/*holy cow look at all these headers!*/

int getSeconds(){                               //function for retrieving the second in our time stored in shared memory
    key_t key = 66;
    int secID = shmget(key, 1024, 0444);
    char *tempTime = (char*) shmat(secID, (void*)0, 0);
    int seconds = atoi(tempTime);
    shmdt(tempTime);
    return seconds;
}
float getNano(){                            //function for retrieving the nanosecond our in time stored in shared memory
    key_t key = 67;
    int nanoID = shmget(key, 1024, 0444);
    char *tempTime = (char*) shmat(nanoID, (void*)0, 0);
    float nano = (float)(atoi(tempTime)) / 1000000000;
    shmdt(tempTime);
    return nano;
}

struct myTime{                                                                             //struct for our virtual time
    unsigned int seconds;
    unsigned int nanoseconds;

};

struct frames{                                                                         //struct for the resource table
    unsigned int dirtyBit :1;
    unsigned int validBit :1;
    unsigned int address :8;
};

struct PCB{
    int simPid;
    struct frames pageTable[32];
};

struct WHATDO{                                         //struct for information for parent regarding request and release
    int writeOrRead;
    int element;
    int page;
};

struct DEAD{
    int count;
};

struct myTime virtual;                                                             //struct for holding the virtual time
struct PCB *table;                                                          //struct for holding resource table
struct WHATDO *iShould;                                                      //struct for holding information for parent
struct DEAD *deadChildren;

const int getRandom(const int max, const int min){            //function for getting a random number between max and min
    int randomNumber = ((rand() % (max + 1 - min)) + min);
    return randomNumber;
}

#endif