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


int NUM_READERS_WRITERS = 5; // number of readers
int sharedvar = 0; // shared variable
int num_readers_atm=0; // number of readers currently reading
pthread_t readers[5]; // array of reader threads
pthread_t writers[5]; // array of writer threads
pthread_mutex_t sharedvar_mutex; // mutex lock for the shared variable
pthread_cond_t readers_cond; // condition variable for the readers on the shared variable 
pthread_cond_t writers_cond; // condition variable for the writers on the shared variable


/*
number of readers or writers currently using the shared variable.
if 0 then no readers or writers are using the shared variable. 
if > 0 then readers are reading.
if < 0 then writers are writing.
This is considered a state machine.
*/
int resource_count=0; 



int reading_writing_times=0; // number of times each reader will read the variable and each writer will write the variable
unsigned int sleep_time=100; // the maximum amount of time a thread will sleep before reading or writing the variable
void *reader(int id); // the reader threads will begin reading the variable in this function
void *writer(int value); // the writer threads will begin writing the variable in this function
void read_x_times(int id); // the reader threads will read the variable x times in this function
void write_x_times(int value); // the writer threads will write the variable x times in this function

typedef struct {
    int id;
    int value;
} writer_args; 

int main(void){
    // initialize the mutex lock
    pthread_mutex_init(&sharedvar_mutex, NULL);
    // create the structs for passing arguments to the reader and writer threads
    // writer_args writer_args;
    // initialize the condition variables
    pthread_cond_init(&readers_cond, NULL);
    pthread_cond_init(&writers_cond, NULL);
    // create the reader and writer threads
    for(int i=0; i<NUM_READERS_WRITERS; i++){
        int value = rand()%10;
        pthread_create(&readers[i], NULL, (void *)reader,i );
        pthread_create(&writers[i], NULL, (void *)writer, value);
    }

    // join the reader and writer threads
    for(int i=0; i<NUM_READERS_WRITERS; i++){
        pthread_join(readers[i], NULL);
        pthread_join(writers[i], NULL);
    }

}

void *reader(int id){
    // sleep for a random amount of time
    usleep(rand()%sleep_time);
    // lock the mutex lock
    pthread_mutex_lock(&sharedvar_mutex);
    while(resource_count<0){
        // if the resource count is less than 0 then there is one writer currently writing
        // wait for the writer to finish writing
        pthread_cond_wait(&readers_cond, &sharedvar_mutex);
    }
    resource_count+=1; // increment the resource count
    num_readers_atm+=1; // increment the number of readers currently reading
    pthread_mutex_unlock(&sharedvar_mutex); // unlock the mutex lock
    // read the variable
    printf("The value read is %d\n", sharedvar);
    printf("The number of readers present when value is read is %d\n", num_readers_atm);
    printf("Currently reader %d reading...\n", id);

    pthread_mutex_lock(&sharedvar_mutex);
    num_readers_atm-=1; // decrement the number of readers currently reading
    resource_count-=1; // decrement the resource count
    if(resource_count==0){
        // if the resource count is 0 then there are no readers or writers currently using the variable
        // signal the writers that they can write
        pthread_cond_signal(&writers_cond);
    }
    pthread_mutex_unlock(&sharedvar_mutex);

}

void *writer(int value){
    pthread_mutex_lock(&sharedvar_mutex);
    while(resource_count!=0){
        // if the resource count is more than 0 then there are readers currently reading
        // wait for the readers to finish reading 
        pthread_cond_wait(&writers_cond, &sharedvar_mutex);
    }
    resource_count=-1; // make the resouce_count -1 to indicate that there is one writer currently writing
    pthread_mutex_unlock(&sharedvar_mutex);
    // write the variable
    sharedvar=value;
    printf("The written value is %d\n", sharedvar);
    printf("The number of readers present when value is written is %d\n", num_readers_atm);
    // printf("Currently writer %d writing...\n", id);

    pthread_mutex_lock(&sharedvar_mutex);
    // set the resource count to 0
    
    resource_count=0;
    // signal the readers that they can read
    pthread_cond_broadcast(&readers_cond);
    pthread_mutex_unlock(&sharedvar_mutex);

}

