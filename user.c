#include "sharedMem.h"
sem_t mutex, timeMutex, deadMutex;

int writeOrRead(){                                  //function for getting a random number for a request or release
    int chance = getRandom(100, 0);
    if(chance < 30){
        return 0;
    }else{
        return 1;
    }
}

void threadFunc(int element){            //function that holds our semaphore
    int leaveItToChance;                                   //integer that gets a value between 0 and 100 to get a chance
    key_t decisionKey = 69;
    int requestCount = 0;
    do {                                                                   //nice little do while loop for our semaphore
            sem_wait(&mutex);                                                               //enter critical section
            if(requestCount >= 1000) {
                leaveItToChance = getRandom(100, 0);                                            //get the value 0 to 100
                if (leaveItToChance < 50) {                                       //if the value is 0-50 or a 51% chance
                    sem_wait(&deadMutex);
                    int deadId = shmget(71, 1024, 0666 | IPC_CREAT);
                    deadChildren = shmat(deadId, NULL, 0);
                    deadChildren->count = deadChildren->count + 1;
                    shmdt(deadChildren);
                    sem_post(&deadMutex);                                                      //end of critical section
                    sem_post(&mutex);                                                              //end of critical section
                    break;                                                                       //break out of the loop
                }else{
                    requestCount = 0;
                }
            }
            int decisionId = shmget(decisionKey, 1024, 0666|IPC_CREAT);
            int address = getRandom(32000, 0);                                               //get a random resource
            if(writeOrRead() == 1){                                      //if 1, we are gonna say we want to request
                iShould = shmat(decisionId, NULL, 0);                 //set up our information struct for the parent
                iShould->writeOrRead = 1;
                iShould->element = element;
                iShould->page = address;
                shmdt(iShould);
                //request
            }else{                                                          //otherwise the process wants to release
                iShould = shmat(decisionId, NULL, 0);                     //set up our information struct for parent
                iShould->writeOrRead = 0;
                iShould->element = element;
                iShould->page = address;
                shmdt(iShould);
                //release resource
            }
            requestCount++;
            while(shmget(decisionKey, 1024, 0444) != -1){
            }
            sem_post(&mutex);                                                              //end of critical section

    }while(1);

}

int main(int argc, char* argv[]){

    int element = atoi(argv[1]);                                                                   //save process number


    int i;
    key_t dataKey = 65;
    int dataId = shmget(dataKey, 1024, 0666|IPC_CREAT);
    table = shmat(dataId, NULL, 0);
    table[element].simPid = element;
    for(i = 0; i < 32; i++){                                //reinitialize the resource table, getting random max claims
        table[element].pageTable[i].dirtyBit = 0;
        table[element].pageTable[i].validBit = 0;
        table[element].pageTable[i].address = 0;
    }
    shmdt(table);

    sem_init(&mutex, 1, 1);                                                                       //initialize semaphore
    sem_init(&deadMutex, 1, 1);
    threadFunc(element);                                       //call semaphore function
    sem_destroy(&mutex);                                                                         //destroy the semaphore
    sem_destroy(&deadMutex);
    exit(0);
}