#include <ucontext.h>
#include <string.h>


struct TCB_t 
{
    int thread_id;
    ucontext_t context;
    struct TCB_t *next; // Pointer to next node
    struct TCB_t *prev; // Pointer to previous node
};

typedef struct TCB_t* q_item;

void init_TCB(q_item tcb, void *function, void *stackP, int stack_size){

  memset(tcb, '\0', sizeof(struct TCB_t));    
  getcontext(&tcb->context);              
  tcb->context.uc_stack.ss_sp = stackP;
  tcb->context.uc_stack.ss_size = (size_t) stack_size;
  makecontext(&tcb->context, function, 0);
}
