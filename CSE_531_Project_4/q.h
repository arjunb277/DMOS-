#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "TCB.h"

/*struct q_element
{
    struct TCB_t* tcb_ptr;
    struct q_element *next; // Pointer to next node
    struct q_element *prev; // Pointer to previous node
};*/


q_item NewItem()
{
	q_item item = (q_item)malloc(sizeof(struct TCB_t));
	item->next = NULL;
	item->prev = NULL;
	return item;
}

q_item newQueue()
{
	q_item head = NULL;
	return head;
}

void AddQueue(q_item *head_ptr, q_item item)
{
	if(*head_ptr == NULL)
	{
		(*head_ptr) = item;
		(*head_ptr)->next = item;
		(*head_ptr)->prev = item;
	}
	else
	{
		q_item last = (*head_ptr)->prev;
		last->next = item;
		item->prev = last;
		item->next = (*head_ptr);
		(*head_ptr)->prev = item;
	}
}

q_item DelQueue(q_item *head_ptr)
{
	q_item temp, last, second; 

	if((*head_ptr) == NULL)
	{
		printf("The queue is already empty!\n");
		return NULL;
	}
	else if((*head_ptr)->prev == (*head_ptr))
	{
		temp = (*head_ptr);
		(*head_ptr) = NULL;
		temp->next = NULL;
		temp->prev = NULL;
		return temp;
	}
	else{
		last = (*head_ptr)->prev;
		second = (*head_ptr)->next;
		last->next = second;
		second->prev = last;	
		temp = (*head_ptr);
		temp->next = NULL;
		temp->prev = NULL;
		(*head_ptr) = second;
		return temp;
	   }
}

/*void main()
{
	q_item item, temp;
	q_item head;

	head = newQueue();
	
	for(int i=0; i<2; i++){

	 item = NewItem();
	 AddQueue(&head,item);
	}
     
	temp = head;
	for(int i=0; i<2; i++)
	{
		temp->payload = i;
		temp=temp->next;
	}
	
	
	temp = head;
	for(int i=0; i<2; i++)
	{
		printf("%d \n", temp->payload);
		temp = temp->next;
	}

	AddQueue(&head,DelQueue(&head));
	//AddQueue(&head,DelQueue(&head));
	//AddQueue(&head,DelQueue(&head));
	//AddQueue(&head,DelQueue(&head));

	temp = head;
	for(int i=0; i<2; i++)
	{
		printf("%d \n", temp->payload);
		temp = temp->next;
	}

   /*while(head != NULL){
    temp = DelQueue(&head);
	printf("%d \n", temp->payload);	
 }
 temp=DelQueue(&head);*/
//}	
     



