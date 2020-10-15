#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "msgs.h"

sem_ptr mutex_sem;
char *client_file_names[99]; //file names used by clients.
int client_count = 0;        // to get reply ports and track number of clients 
int server_port = 20;

void client(){

   FILE *fp;
   //  message type: [{0/1}{filename/data}{reply_port}]
   //  0: file transfer requested      
   //  1: data transfer requested
   //  2: file transfer complete
   int client_msg[10] = {-1};
   int rcv_msg[10] = {-1};   //  rcv message 
   char data_buff[64];
   int send_port = server_port;
 
   P(mutex_sem);
   int reply_port = client_count++;
   V(mutex_sem);

   fp = fopen(client_file_names[reply_port],"r");

   while(1){
     
     client_msg[0] = 0; //request for file
     memcpy(client_msg+1,client_file_names[reply_port],32); //filename  
     client_msg[9] = reply_port; //reply port 

     send(send_port,client_msg);
     //sleep(1);
     rcv(reply_port,rcv_msg);

    if(rcv_msg[0] == -1){
       printf("ERROR: Terminating client %d since file name length exceeded\n",reply_port);
       while(1){
       	  yield(); //will keep yielding to others since they cannot transfer file 
       }
     }
     else if(rcv_msg[0] == 0){ 

     	     continue; //will force next iteration and request again.
       }
     else{ 
     
     printf("CLIENT %d MSG: OK TO TRANSFER FILE: %s\n",reply_port,client_file_names[reply_port]);
     //request for data to be written
     
     while(!feof(fp)){
     fgets(data_buff,32,fp); 
     //printf("CLIENT %d MSG: Data to write = %s\n",reply_port,data_buff);

     client_msg[0] = 1; //request for data
     memcpy(client_msg+1,data_buff,32); //filename  
     client_msg[9] = reply_port; //reply port 

     send(send_port,client_msg); //send

     rcv(reply_port,rcv_msg);

     if(rcv_msg[0] == -1){
     	printf("ERROR: Terminating client %d since file size exceeded\n",reply_port); 
    	while(1){
       	  yield(); //will keep yielding to others since they cannot transfer file
       }
    
    }
   } 

    printf("CLIENT %d MSG: FILE TRANSFER COMPLETE\n",reply_port);
    client_msg[0] = 2;
    send(send_port,client_msg);


    fclose(fp);

    while(1){
     yield();
    }
  }
 }
} 

void server(){

   int rcv_msg[10] = {-1};
   //  message type: [{-1,0/1}{filename/data}{reply_port}]
   //  -1: terminate client beacause file name is not proper.      
   //   1: client can send data.
   //   0: terminate client since its filesize exceeded.
   int server_msg[10] = {-1}; 
   char rcv_filename[16];
   char data_buff[64];
   int send_port;
   int concurrent_count = 0;
   FILE *fp[99];
   
   P(mutex_sem);
   
   int rcv_port = server_port;

   V(mutex_sem);

  while(1){
   rcv(rcv_port,rcv_msg);

   send_port = rcv_msg[9];

   if(rcv_msg[0] == 0){  //client is requesting write access
      
      memcpy(rcv_filename,rcv_msg+1,32);
      //printf("file name rcved = %s\n",rcv_filename);
  
      if(strlen(rcv_filename)>15){

      	printf("SERVER MSG: file name rcved = %s cannot be handled\n",rcv_filename);
      	server_msg[0] = -1;
      	send(send_port,server_msg);
        //tell it to terminate client as file cannot be handled.
      }
      
      concurrent_count++;

      strcat(rcv_filename,".server");
      //printf("SERVER MSG: new file = %s\n",rcv_filename);  
      fp[send_port] = fopen(rcv_filename,"w");
      //printf(" SERVER MSG: increment conc_count = %d\n", concurrent_count);

      if(concurrent_count <= 3){ //max 3 concurrent transfers
       server_msg[0] = 1;
       send(send_port,server_msg);
      }
      else{

       concurrent_count--;
       //printf(" SERVER MSG: Too many clients decrement conc_count = %d\n", concurrent_count);
       server_msg[0] = 0;
       send(send_port,server_msg);

      }
    }
    else{

      if(rcv_msg[0] == 1){ //client is requesting data transfer
        
        memcpy(data_buff,rcv_msg+1,32);
        //printf("SERVER MSG: Data to be written is = %s\n",data_buff);
        fputs(data_buff,fp[send_port]);
        fseek(fp[send_port],0,SEEK_CUR);

        if(ftell(fp[send_port])>1048576){ 
         printf("SERVER MSG: file name = %s exceeded 1MB\n",rcv_filename);
         concurrent_count--;        
         server_msg[0] = -1;
      	 send(send_port,server_msg);
      	}
 
        server_msg[0] = 1;
      	send(send_port,server_msg);

      }
      else{
        
        concurrent_count--;
        //printf(" SERVER MSG: CLIENT %d finished decrement conc_count = %d\n",send_port,concurrent_count);

      } 
     }
   }
}
// argc counts the number of arguments on the cmd line and argv[]. If there 
// are no arguments argc=1. 
int main(int argc, char* argv[]){ 

   mutex_sem = CreateSem(1);
   Create_ports();

   start_thread(server);	

  if(argc >=2){

    int num_of_clients = atoi(argv[1]);
    
    printf("Number of clients is: %d\n", num_of_clients);  

  	printf("The client file names are:\n");
    for(int i=0; i<num_of_clients; i++){
        
        client_file_names[i] = (char *)malloc(strlen(argv[i+2])*sizeof(char));  
        client_file_names[i] = argv[i+2];
        start_thread(client);
    	printf("%s ",client_file_names[i]);
    } 
    printf("\n");
  }  
  else{

    printf("No arguments entered\n");

  } 

  run();
 
 return 0;
}
