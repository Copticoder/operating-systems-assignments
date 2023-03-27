/*
Write a multi-threaded C program that gives readers priority over writers concerning a shared (global) variable. Essentially, if any readers are waiting, then they have priority over writer threads -- writers can only write when there are no readers. This program should adhere to the following constraints:

    Multiple readers/writers must be supported (5 of each is fine)
    Readers must read the shared variable X number of times
    Writers must write the shared variable X number of times
    Readers must print:
        The value read
        The number of readers present when value is read
    Writers must print:
        The written value
        The number of readers present were when value is written (should be 0)
    Before a reader/writer attempts to access the shared variable it should wait some random amount of time
        Note: This will help ensure that reads and writes do not occur all at once
    Use pthreads, mutexes, and condition variables to synchronize access to the shared variable

*/
#include<stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include <unistd.h>


unsigned short NUM_READERS = 5; // number of readers
unsigned short NUM_WRITERS = 5; // number of writers
unsigned short GlobalVariable = 0; // shared variable
unsigned short num_readers_atm=0; // number of readers currently reading
unsigned short num_writers_atm=0; // number of writers currently writing
pthread_t readers[5]; // array of reader threads
pthread_t writers[5]; // array of writer threads
pthread_mutex_t GlobalVariable_mutex; // mutex lock for the shared variable
pthread_cond_t readers_cond; // condition variable for the readers on the shared variable 
pthread_cond_t writers_cond; // condition variable for the writers on the shared variable
unsigned short reading_writing_times=0; // number of times each reader will read the variable and each writer will write the variable
unsigned int sleep_time=100; // the maximum amount of time a thread will sleep before reading or writing the variable
void reader(); // the reader threads will begin reading the variable in this function
void writer(unsigned short value); // the writer threads will begin writing the variable in this function

int main(void){
    printf("Hello World\n");
    // initialize the mutex lock
    pthread_mutex_init(&GlobalVariable_mutex, NULL);
    // initialize the condition variables
    pthread_cond_init(&readers_cond, NULL);
    pthread_cond_init(&writers_cond, NULL);
    


}

// void reader(){

// }

// void reader(unsigned short value){

// }

