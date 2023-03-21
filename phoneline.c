#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

static int next_id = 0; //represents caller id, incremented each time a thread is called
static sem_t connected_lock; //binary semaphore for connected
static sem_t next_id_lock; //binary semaphore for next_id
static sem_t operators; //counting semaphore for operators
static int NUM_OPERATORS = 3; //number of operators available
static int NUM_LINES = 5; //number of lines available
static int connected = 0; //represents the number of threads connected to a line

//thread function
void * phonecall(void* vargp){
    sem_wait(&next_id_lock);
    int callerId = ++next_id; //this will be the caller id of the thread
    sem_post(&next_id_lock);

    //getting connected to a line, if busy then retries every second
    while(1){
        sem_wait(&connected_lock);
        printf("An attempt to connect has been made! Thread id: %i\n", callerId);
        if(connected < NUM_LINES){ //if there is an available line
            printf("Thread %i has available line, call ringing\n", callerId);
            connected++;
            sem_post(&connected_lock);
            break;
        }

        //if there are no lines available
        printf("Thread %i is calling line, busy signal\n", callerId);
        sem_post(&connected_lock);
        sleep(1); //waits for one second before retrying if no lines are available
    }
    
    //speaking to operator
    sem_wait(&operators);
    printf("Thread %i is speaking to operator\n", callerId);
    sleep(3); //takes 3 seconds to complete the purchase
    printf("Thread %i has bought a ticket!\n", callerId);
    sem_post(&operators);
    printf("Thread %i has hung up!\n", callerId);
    
    //decrementing connected when a phone call has concluded
    sem_wait(&connected_lock);
    connected--;
    sem_post(&connected_lock);
}

//main method
int main(int argc, char * argv[]){
    //if no argument is given
    if(argc == 1){
        printf("Please provide the number of phone calls! Exiting...\n");
        exit(0);
    }
    
    //initializating semaphores for connected, operators, and next_id
    sem_init(&connected_lock, 0, 1);
    sem_init(&operators, 0, NUM_OPERATORS); 
    sem_init(&next_id_lock, 0, 1);

    int numOfCalls = atoi(argv[1]); //number of calls to be made (1-240)

    pthread_t tids [numOfCalls]; //array of thread ids

    //creating the threads
    for(int i = 0; i<numOfCalls; i++){
        pthread_create(&tids[i], NULL, phonecall, NULL);
    }

    //waiting for all of the threads to finish
    for(int i = 0; i<numOfCalls; i++){
        pthread_join(tids[i], NULL);
    }

    //destroying semaphores
    sem_destroy(&connected_lock);
    sem_destroy(&operators);
    sem_destroy(&next_id_lock);
}