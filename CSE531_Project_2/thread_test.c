#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "threads.h"

int global_count=0; 

void f1(){

        int local_count=0;
	while(1){
		 printf("Running function f1 from thread %d\n",get_id(Curr_Thread));
                 local_count++;
                 global_count++;
                 printf("Local count for f1 is %d and global count is %d\n",local_count,global_count);
                 sleep(1);
		 yield();
	        }
          }
 
void f2(){

        int local_count_2 = 0;
	while(1){
		 printf("Running function f2 from thread %d\n",get_id(Curr_Thread));
                 local_count_2++;
                 global_count++;
                 printf("Local count for f2 is %d and global count is %d\n",local_count_2,global_count);
                 sleep(1);
		 yield();	        
                }
          }

int main(){

	ReadyQ = newQueue();
	start_thread(&f1);
	start_thread(&f1);
	start_thread(&f2);
        start_thread(&f2);
	run();
	return 0;

}
