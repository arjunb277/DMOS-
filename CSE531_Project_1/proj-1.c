/*************CSE 531 Distributed and Multiprocessor Operating System*************/
              /****************PROJECT 1 SUBMISSION*****************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sem.h"

#define N 3 //number of child threads

int count=0; //count to check if all the children threads have updated the array
semaphore_t mutex; //for synchronization between the children thread
pthread_mutex_t mutex_print; //to protect/lock the condition 
pthread_cond_t print_cond; //condition to signal and wait
semaphore_t mutex_update;

// Array increment function called by each child thread
void increment_arr(int *arg)
{
	while(1)
	{   
		P(&mutex);
		pthread_mutex_lock(&mutex_print);
		if(count == N)
		 {
            pthread_cond_signal(&print_cond);
            count=0;
		 }
		count++;
		pthread_mutex_unlock(&mutex_print);
		//usleep(10000);
                
		printf("Array updated by thread %d\n", count);
		(*arg)++;
		V(&mutex);	
	}
}

int main()
{
    int arr[N];               // Declaring an array of N elements
	init_sem(&mutex, 1);  // Initializing the mutex to one, so that only one thread can acquire it at a time
    init_sem(&mutex_update, 1);
	int i = 0;

	// Initializing the array of N integers to 0 and creating N threads
	for(int j=0; j<N; j++)
	{
		arr[j] = 0;
		start_thread(increment_arr,arr+j);
	}

    while(1){
        pthread_mutex_lock(&mutex_print);
        while(count != N)
        {
          pthread_cond_wait(&print_cond,&mutex_print);
          printf("count = %d\n", count);
          printf("Array = ");
          
          while(i<N){         //printing the updated array
            printf("%d ",arr[i]);
            i++;	
          }
          i=0;
          printf("\n");
        
        pthread_mutex_unlock(&mutex_print);
      }
 return 0;
}

