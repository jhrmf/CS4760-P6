#include "sharedMem.h"


struct frameTable{
    int tableKey;
    int tableNum;
    int process;
};

struct myTime updateClock(struct myTime virtual){     //function for updating the system clock's seconds and nanoseconds
    char temp1[10], temp2[11];                                               //two temp strings for holding time
    key_t secKey = 66;                                         //the key for the shared memory holding the seconds is 66
    key_t nanoKey = 67;                                    //the key for the shared memory holding the nanoseconds is 67

    int secID = shmget(secKey, 1024, 0666|IPC_CREAT);    //access the shared memory with file write and read permissions
    int nanoID = shmget(nanoKey, 1024, 0666|IPC_CREAT); //same as above, secID is for seconds, nanoID is for nanoseconds


    virtual.nanoseconds = virtual.nanoseconds + 40000;                       //increment nanoseconds by 40000 every call
    if(virtual.nanoseconds >= 1000000000){                          //if the nanoseconds exceeds or is equal to a second
        virtual.seconds++;                                                             //increment the seconds counter
        virtual.nanoseconds = virtual.nanoseconds - 1000000000;             //decrement the nanosecond count by a second
    }

    sprintf(temp1, "%d", virtual.seconds);                                               //store the seconds in a string
    sprintf(temp2, "%d", virtual.nanoseconds);                                       //store the nanoseconds in a string
    char *secStr = (char *) shmat(secID, (void *) 0, 0);                 //ptr to the shared memory location for seconds
    strcpy(secStr, temp1);                                              //copy the seconds to the shared memory location
    shmdt(secStr);                                                              //detach from the shared memory location
    char *nanoStr = (char *) shmat(nanoID, (void *) 0, 0);           //ptr to the shared memory location for nanoseconds
    strcpy(nanoStr, temp2);                                         //copy the nanoseconds to the shared memory location
    shmdt(nanoStr);                                                             //detach from the shared memory location
    return virtual;                                           //return the new time stored in the virtual time structure
}

int checkTerminate(int check){
    int deadId = shmget(71, 1024, 0666 | IPC_CREAT);
    deadChildren = shmat(deadId, NULL, 0);
    int temp = deadChildren->count;
    shmdt(deadChildren);
    if(check != temp){
        printf("%d processes have died\n", temp);
    }
    return temp;
}

void displayTable(struct frameTable ft[], int size){
    int n;
    printf("Frame Table\n");
    printf("__________________________________\n");
    for(n = 0; n < 256; n++){
        if(n <= size){
            printf("%-3ld     ", ft[n].tableKey);
            printf("%-3ld     ", ft[n].tableNum);
            printf("%-3ld     ", ft[n].process);
            printf("\n");
        }else{
            printf("%-3lc     ", '+');
            printf("%-3lc     ", '+');
            printf("%-3lc     ", '+');
            printf("\n");
        }
    }
    printf("__________________________________\n");
}


int main(int argc, char *argv[]) {

    int opt = 0;
    int maxProcesses = 18;                                                       //default max child processes is set to 5
    char fileName[100] = "logFile";                                            //default log file name is set to logFile
    virtual.nanoseconds = 0;                                                            //nanosecond counter is set to 0
    virtual.seconds = 0;                                                                    //second counter is set to 0
    key_t dataKey = 65;
    int tableCount = 1;
    int deadProcesses = 0;
    int dataId = shmget(dataKey, 1024, 0666 | IPC_CREAT);
    int frameIndex = 0;
    struct frameTable ft[256];
    int method = 1;
    int pageFaultCount = 0;
    int memoryRequestCount = 0;
    float memoryAccessSpeed = 0;
    float startOfAccess = 0;
    int displayPage = 0;
    while ((opt = getopt(argc, argv,"hl")) != -1) {                                              //GET OPT WOOOOO
        switch (opt) {                                          //start a switch statement for the commandline arguments
            case 'h' :                                                                              //print help message
                printf("Welcome to Log Parse!\n");
                printf("Invocation: ./logParse [-h] [-s maxChildren] [-l logFile] [-t timer]\n");
                printf("h : Display a help message to the user and exit \n");
                printf("l : Run program with LRU\n");
                exit(0);
                break;
            case 'l' :                                                                                 //set verbose off
                method = 0;
                break;
            default:                                                              //user inputted something unrecognized
                perror("Command line argument not recognized.");
                exit(EXIT_FAILURE);
        }
    }


    int childCount;                                      //this is for a loop below to hold the count of child processes
    pid_t childPid;                                            //of course we need one of these guys for child processes
    //remove("logFile");                                //remove whatever the default log file name just in case it exists
    int i, processNumber = 0;


    //FILE *logptr = fopen(fileName, "w");                                               //initialize file ptr for writing

    float randomProcessMili = getRandom(2, 499);
    int checkSpace;
    FILE *logptr = fopen(fileName, "w");
    do {
        int miliCheck = virtual.nanoseconds / 1000000;                  //get current millisecond for process time check
        if (miliCheck == randomProcessMili &&processNumber < maxProcesses) {            //if its time to fork a process

            char temp[2];                                                                 //arguement for process number
            sprintf(temp, "%d", processNumber);                                                 //set the process number
            char *envp[] = {temp, NULL};

            processNumber++;                                                                      //increment the number
            childPid = fork();                                                                    //fork a child process

            if (childPid == 0) {                                                                           //run user file
                execve("./user", NULL, envp);
            }
            randomProcessMili = getRandom(2, 499);                                       //get a new random process time

        } else {

            deadProcesses = checkTerminate(deadProcesses);
            if (deadProcesses == maxProcesses) {
                printf("Ending\n");
                break;
            }

            key_t read = 69;                                                   //the key for the shared memory for messaging
            if (shmget(read, 1024, 0444) != -1) {                        //if the memory location has something stored there
                displayPage++;
                if(displayPage == 200){
                    displayTable(ft, frameIndex);
                    displayPage = 0;
                }
                startOfAccess = (float)((float)virtual.seconds + (float)((float)virtual.nanoseconds/1000000000));
                char msg[150] = "\0";
                char tmp[100] = "\0";
                int decisionId = shmget(read, 1024, 0666);
                iShould = shmat(decisionId, NULL, 0);
                int elementForUse = iShould->element;                                           //get the process number
                int pageForUse = iShould->page;                                                //get the resource number
                table = shmat(dataId, NULL, 0);
                pageForUse = pageForUse / 1024;

                if (table[elementForUse].pageTable[pageForUse].validBit != 1) {
                    pageFaultCount++;

                    table[elementForUse].pageTable[pageForUse].validBit = 1;
                    if (iShould->writeOrRead == 1) {
                        //write
                        strcpy(msg, "Process ");    //this is all printing and formatting
                        sprintf(tmp, "%d", elementForUse);
                        strcat(msg, tmp);
                        strcat(msg, " requested write of page ");
                        strcpy(tmp, "\0");
                        sprintf(tmp, "%d", pageForUse);
                        strcat(msg, tmp);
                        strcat(msg, " at time ");
                        strcpy(tmp, "\0");
                        sprintf(tmp, "%f", ((float)virtual.seconds + (float)((float)virtual.nanoseconds/1000000000)));
                        strcat(msg, tmp);
                        strcat(msg, "\n");
                        fputs(msg, logptr);
                        table[elementForUse].pageTable[pageForUse].dirtyBit = 1;
                        virtual.nanoseconds += 14000000;
                        memoryRequestCount++;
                    } else {
                        strcpy(msg, "Process ");    //this is all printing and formatting
                        sprintf(tmp, "%d", elementForUse);
                        strcat(msg, tmp);
                        strcat(msg, " requested read of page ");
                        strcpy(tmp, "\0");
                        sprintf(tmp, "%d", pageForUse);
                        strcat(msg, tmp);
                        strcat(msg, " at time ");
                        strcpy(tmp, "\0");
                        sprintf(tmp, "%f", ((float)virtual.seconds + (float)((float)virtual.nanoseconds/1000000000)));
                        strcat(msg, tmp);
                        strcat(msg, "\n");
                        fputs(msg, logptr);
                        virtual.nanoseconds += 14000000;
                        memoryRequestCount++;
                        //read
                    }
                    if (frameIndex < 256) {
                        table[elementForUse].pageTable[pageForUse].address = frameIndex;
                        ft[frameIndex].tableKey = frameIndex;
                        ft[frameIndex].process = elementForUse;
                        ft[frameIndex].tableNum = pageForUse;
                        strcpy(msg, "Process ");    //this is all printing and formatting
                        sprintf(tmp, "%d", elementForUse);
                        strcat(msg, tmp);
                        strcat(msg, " has been approved. Page ");
                        strcpy(tmp, "\0");
                        sprintf(tmp, "%d", pageForUse);
                        strcat(msg, tmp);
                        strcat(msg, " has been written into frame at address ");
                        strcpy(tmp, "\0");
                        sprintf(tmp, "%d", frameIndex);
                        strcat(msg, tmp);
                        strcat(msg, "\n");
                        fputs(msg, logptr);
                        frameIndex++;
                    } else {
                        strcpy(msg, "Process ");    //this is all printing and formatting
                        sprintf(tmp, "%d", elementForUse);
                        strcat(msg, tmp);
                        strcat(msg, " seeking write of table ");
                        strcpy(tmp, "\0");
                        sprintf(tmp, "%d", pageForUse);
                        strcat(msg, tmp);
                        strcat(msg, " but frame is max | Clearing victim of address ");
                        strcpy(tmp, "\0");
                        sprintf(tmp, "%d", table[ft[0].process].pageTable[pageForUse].address);
                        strcat(msg, tmp);
                        strcat(msg, "\n");
                        fputs(msg, logptr);
                        int n;
                        table[ft[0].process].pageTable[pageForUse].validBit = 0;
                        for (n = 0; n < 256; n++) {
                            if (n == 255) {
                                ft[n].tableNum = pageForUse;
                            } else {
                                ft[n].tableNum = ft[n + 1].tableNum;
                            }
                        }
                        virtual.nanoseconds += 10;
                    }
                    memoryAccessSpeed = (float)(((float)virtual.seconds + (float)((float)virtual.nanoseconds/1000000000))
                                                - startOfAccess) / (float)memoryRequestCount;
                } else {
                    memoryRequestCount++;
                    if (method != 1) {                                       //LRU
                        int least = table[elementForUse].pageTable[pageForUse].address;
                        int tempPage;
                        int n;
                        for (n = least; n <= frameIndex; n++) {
                            if (n == least) {
                                tempPage = ft[n].tableNum;
                            } else {
                                 ft[n].tableNum = ft[n + 1].tableNum;
                              }
                        }
                        ft[frameIndex].tableNum = tempPage;
                        table[elementForUse].pageTable[pageForUse].address = frameIndex;
                        strcpy(msg, "Process ");    //this is all printing and formatting
                        sprintf(tmp, "%d", elementForUse);
                        strcat(msg, tmp);
                        strcat(msg, " has caused the relocation of memory at address ");
                        strcpy(tmp, "\0");
                        sprintf(tmp, "%d", least);
                        strcat(msg, tmp);
                        strcat(msg, " to the end of the frame");
                        strcat(msg, "\n");
                        fputs(msg, logptr);
                    }
                  }

                shmdt(table);
                shmdt(iShould);
                shmctl(decisionId, IPC_RMID, NULL);
            }

          }
        int temp = virtual.seconds;
        virtual = updateClock(virtual);                                                   //update the virtual user time

        if(virtual.seconds == 40){
            break;
        }

    } while (1);
    //PRINT STATISTICS
    printf("Average Memory Access Time: %f ms\n", memoryAccessSpeed*1000000);
    printf("Total Memory Requests: %d \n", memoryRequestCount);
    printf("Page faults / Memory Requests: %f\n", (float)((float)pageFaultCount/(float)memoryRequestCount));
    shmctl(shmget(66, 1024, 0444), IPC_RMID, NULL);                               //delete the shared memory for seconds
    shmctl(shmget(67, 1024, 0444), IPC_RMID, NULL);                           //delete the shared memory for nanoseconds
    shmctl(shmget(65, 1024, 0444), IPC_RMID, NULL);                             //delete the shared memory for resources
    shmctl(shmget(69, 1024, 0444), IPC_RMID, NULL);                             //delete the shared memory for resources
    shmctl(shmget(71, 1024, 0444), IPC_RMID, NULL);                               //delete the shared memory for seconds
    exit(0);                                                                    // end the process, cause its the parent
}
