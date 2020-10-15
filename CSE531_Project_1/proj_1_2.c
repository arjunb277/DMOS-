/*************CSE 531 Distributed and Multiprocessor Operating System*************/
              /****************PROJECT 1 SUBMISSION*****************/

                  //Submitted by Niharika Manmode ASU ID:1215337045
                  //             Arjun Bhatnagar ASU ID:1215353243

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sem.h"

#define N 3 //number of children threads

int count=0; //count to check if all the children threads have updated the array
semaphore_t mutex; //for synchronization between the children thread
semaphore_t sem_print;
pthread_mutex_t mutex_print; //to protect/lock the condition 
pthread_cond_t print_cond; //condition to signal and wait

// Array increment function called by each child thread
void increment_arr(int *arg)
{
	while(1)
	{   
		P(&mutex);
		(*arg)++;
		if(count==3){ count =0;}
		count++;
		printf("Array updated by thread %d\n", count);
		V(&sem_print);
        V(&mutex);	
	}
}

int main()
{
    int arr[N];               // Declaring an array of N elements
	init_sem(&mutex,1); // Initializing the mutex to one, so that only one thread can acquire it at a time
	init_sem(&sem_print,-3);

	// Initializing the array of N integers to 0 and creating N threads
	for(int j=0; j<N; j++)
	{
		arr[j] = 0;
		start_thread(increment_arr,arr+j);
	}

    while(1){

          P(&sem_print);
          init_sem(&sem_print,-3);
          printf("Hi");
        }
 return 0;
}

