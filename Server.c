#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>
#include<sys/ipc.h>

#include<sys/shm.h>
#include<malloc.h>
#include<fcntl.h>           						/* For O_* constants */
#include<sys/stat.h>        						/* For mode constants */
#include<semaphore.h>
#include<sys/mman.h>

#define PORT 8088
#define MAXLINE 1024

void send_response(int);
void receive_request(int);


unsigned char snd_data[256];
unsigned char rcv_data[256];

int sockproxy;
char buffer[MAXLINE];
struct sockaddr_in servaddr,proxyaddr;
int c,len,valread,i,n,datalen;
int new_socket;


int main()
{
	

// Creating Socket File Description
	
	sockproxy = socket(AF_INET,SOCK_STREAM,0);

	if(sockproxy<0)
	{
		perror("\n Socket Creation Failed ");
		exit(1);
	}
	else
		printf("\n Socket for Proxy Server 1 created Successfully ");
	
	sleep(2);

	memset(&servaddr,0,sizeof(servaddr));
	memset(&proxyaddr,0,sizeof(proxyaddr));

// Filling Server Address 

	servaddr.sin_family = AF_INET;  // IPv4
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

// Binding the Socket with Server address


	if(bind(sockproxy,(const struct sockaddr*)&servaddr,sizeof(servaddr))<0)
	{
		perror("\n Binding To Proxy Server 1 Failure ");
		exit(EXIT_FAILURE);
	}
	else
		printf("\n Binding Successfully to Proxy Server 1 "); 
	
	sleep(2);

// Listening Server

	listen(sockproxy,5);
	
	sleep(2);


	

	if((new_socket = accept(sockproxy,(struct sockaddr*)&sockproxy,(socklen_t*)&len))<0)
	{
		printf("\n Could not accept ");
	}
	else 
		printf("\n Connection Accepted ");

	sleep(2);

	
	{
		printf("-------------------------------------------------------------\n");
		printf("\n Recieving Request from  Proxy Server 1\n");
		receive_request(new_socket);
		sleep(1);
		send_response(new_socket);	
		sleep(5);
		
		printf("-------------------------------------------------------------\n");
		sleep(5);
	}
	return 0;
}

void send_response(int new_socket) {
	snd_data[0] = 0x00;				// Transition Identifier High Bit
	snd_data[1] = 0x00;				// Transition Identifier Low Bit
	snd_data[2] = 0x00;				// Protocol Identifier High Bit
	snd_data[3] = 0x00;				// Protocol Identifier Low Bit
	snd_data[4] = 0x00;				// Length High Bit
	snd_data[5] = 0x09;				// Length Low Bit
	snd_data[6] = 0x01;				// Unit Identifier
	snd_data[7] = 0x03;				// Function Code
	snd_data[8] = 0x06;				// Data Count
	snd_data[9] = 0x00;				// Data 1 High bit
	snd_data[10] = 0x9c;				// Data 1 Low bit
	snd_data[11] = 0x00;				// Data 2 High bit
	snd_data[12] = 0x9c;				// Data 2 Low bit
	snd_data[13] = 0x00;				// Data 3 High bit
	snd_data[14] = 0x9c;				// Data 3 Low bit
	send(new_socket, snd_data, 15, 0);
       	printf("\nsent data    [");
        for(i = 0; i < 15 ; i++) {
           	printf(" %02x ", snd_data[i]);
       	}
        printf("]\n");
        memset(snd_data, 0, sizeof(snd_data));
}

void receive_request(int new_socket) {
	
	recv(new_socket, rcv_data, 256, 0);
	int n = rcv_data[5] + 6;					
	printf("\n Recieved Data [");
	for(i = 0; i < n ; i++)
	{
		printf(" %02x ",rcv_data[i]);
	}
	printf("]\n");
	
        memset(rcv_data, 0, sizeof(rcv_data));
}



