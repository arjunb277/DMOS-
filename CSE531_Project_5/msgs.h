#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sem.h"

#define N 10                                 // Number of messages                                  
#define NUM_OF_PORTS 100                     // Total number of ports

// Port Structure declaration
struct port_struct{
	int msg[N][10];                          // Message queue with N rows, each row containing 10 integers
	sem_ptr mutex;                           // Semaphore for global variables
	sem_ptr emptycount;                      // Blocking semaphore when message queue is full
	sem_ptr fullcount;                       // Blocking semaphore when message queue is empty
	int s_idx;                               // Message Index for sending
	int r_idx;                               // Message Index for receiving
};
struct port_struct port[NUM_OF_PORTS];       // Array of 100 ports

// Initialisation of port structure
void Create_ports()
{
	int i,j,k;
	for(i=0; i<NUM_OF_PORTS; i++){
		port[i].mutex = CreateSem(1);
		port[i].emptycount = CreateSem(N);
		port[i].fullcount = CreateSem(0);
		port[i].s_idx = 0;
		port[i].r_idx = 0;
		for(j=0;j<N;j++){
			for(k=0;k<10;k++){
				port[i].msg[j][k] = 0;
			}
		}
	}
}

// Send routine to send message to sender port
void send(int port_num, int*buffer)
{
	int i;
	P(port[port_num].emptycount);
	P(port[port_num].mutex);
		                             
		for(i=0;i<10;i++){
			port[port_num].msg[port[port_num].s_idx][i] = buffer[i];
		}
		port[port_num].s_idx = (port[port_num].s_idx+1) % N;

	V(port[port_num].mutex);
	V(port[port_num].fullcount);
	//usleep(1000);
}

// Receive routine to receive message from receiver/reply port
void rcv(int port_num, int*buffer)
{
	int i;
	P(port[port_num].fullcount);
	P(port[port_num].mutex);   
	
		for(i=0;i<10;i++){
			buffer[i] = port[port_num].msg[port[port_num].r_idx][i];
		}
		port[port_num].r_idx = (port[port_num].r_idx+1) % N;

	V(port[port_num].mutex);
	V(port[port_num].emptycount);
	//usleep(1000);
}

