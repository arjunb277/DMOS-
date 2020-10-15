#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "q.h"

int count = 1; //thread_id 
q_item ReadyQ;
q_item Curr_Thread;


void start_thread(void (*function)(void)){
   
   void *s = malloc(8192);
   q_item t = NewItem();
   init_TCB(t,function,s,8192);
   t->thread_id = count;
   count++;
   AddQueue(&ReadyQ,t);
}

void run(){  

   Curr_Thread = DelQueue(&ReadyQ);
   ucontext_t parent;     // get a place to store the main context, for faking
   getcontext(&parent);   // magic sauce
   swapcontext(&parent, &(Curr_Thread->context));  // start the first thread
}


void yield(){  

   q_item Prev_Thread;
   AddQueue(&ReadyQ, Curr_Thread); 
   Prev_Thread = Curr_Thread;
   Curr_Thread = DelQueue(&ReadyQ);
   swapcontext(&(Prev_Thread->context), &(Curr_Thread->context));
   //swap the context, from Prev_Thread to the thread pointed to Curr_Thread
}

static int get_id(q_item tcb){
   
   printf("%d\n",tcb->thread_id);

}