#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "msgs.h"


sem_ptr mutex_sem;            // For mutual exclusion of global variables
int server_count = 0;         // Total no of servers
int client_count = 0;         // Total no of clients 
int port_index = 0;           // To calculate sender port
int server_port_list[100] = {0};

void client()
{
	int sp,rp;
	int sp_idx;
	int client_num = 0;                        //Sender port and Receiver port 
	int buffer[10] = {0,1,2,3,4,5,6,7,8,9};

	P(mutex_sem);

        sp_idx = rand() % (server_count + 1);  //To select a server port from server list
	    sp = server_port_list[sp_idx];
		rp = 30+(port_index++)-server_count;   // 30 sp's and 70 rp's --> max no. of clients 70 and max no. of servers is 30
		client_num = client_count;
	    client_count++;
		buffer[0] = rp;

	V(mutex_sem);
	while(1){

		if(rp>99){
			printf("ERROR: NO MORE CLIENT PORTS AVAILABLE\n");
			break;
            
		}

		printf("Client %d: Sending to port %d and receiving from port %d\n\n", client_num, sp, rp);
		send(sp, buffer);
		sleep(1);
		rcv(rp, buffer);
		printf("Client %d: Message received from port %d\n", client_num, rp);
		int i;
		for(i=1;i<10;i++){
			printf("Client %d message : %d\n", client_num, buffer[i]);
		}
	}
}

void server()
{
	int sp, rp;                             // Sender port and Receiver port      
	int server_num = 0;                    // The Server number

	P(mutex_sem);

	    sp = port_index;                   //To select SP.
	    port_index++;
		server_port_list[server_count]=sp;
		server_num = server_count;
		server_count++;
		
	V(mutex_sem);

	while(1){

		if(sp>29){
			printf("ERROR: NO MORE SERVER PORTS AVAILABLE\n");
			break;
            
		}

		int buffer[10] = {0};            
		rcv(sp, buffer);
		printf("Server %d: Message received from port %d\n",server_num, sp);
		rp = buffer[0];
		sleep(1);
		int i;
		for(i=1;i<10;i++){
			printf("Server %d message: %d\n", server_num, buffer[i]);
			buffer[i]++;
		}
		printf("Server %d: Sending to port %d\n\n",server_num, rp);
		send(rp,buffer);
	}
}

int main()
{   //2 servers and 3 clients.
	mutex_sem = CreateSem(1);
	Create_ports();
   	start_thread(&server);
	start_thread(&server);
   
   	start_thread(&client);
	start_thread(&client);
    start_thread(&client);
	
	run();
	return 0;
}