#include <stdlib.h>
#include <unistd.h>
#include "threads.h"


struct Semaphore_t
{
	int count;
	q_item TCB_ptr;
};

typedef struct Semaphore_t * sem_ptr;
// Creating Semaphore
sem_ptr CreateSem(int InputValue)
{
	sem_ptr sem = (sem_ptr) malloc(sizeof(struct Semaphore_t));
	sem->count = InputValue;
	sem->TCB_ptr = newQueue();
	//printf("Sem Count: %d\n", sem->count);
	return sem;
}

// Taking Semaphore
void P(sem_ptr sem)
{
	sem->count--;             // decrementing the semaphore
	if(sem->count<0)
	{
		// block the thread in queue associated with semaphore i.e., delete it from Ready queue and add it to semQ
		//q_item curr;
		//Curr_Thread=DelQueue(&ReadyQ);

		q_item Prev_Thread;
		AddQueue(&(sem->TCB_ptr), Curr_Thread);
		Prev_Thread = Curr_Thread;
		Curr_Thread = DelQueue(&ReadyQ);
		swapcontext(&(Prev_Thread->context), &(Curr_Thread->context));

	}
}

// Giving Semaphore
void V(sem_ptr sem)
{
	sem->count++;
	if(sem->count<=0)
	{
		// put thread back to ReadyQ from semQ
		//q_item temp;
		AddQueue(&ReadyQ, DelQueue(&(sem->TCB_ptr)));
		//AddQueue(&ReadyQ, temp);   // but add it to its head?
	}
	yield();
}
