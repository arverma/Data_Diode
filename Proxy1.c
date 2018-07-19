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
#define PORT2 8080
#define MAXLINE 1024

void send_request(int);
void receive_response(int);
void forward_response(int);


unsigned char snd_data[256];
unsigned char rcv_data[256];
unsigned char rcv_data_proxy[256];

struct sockaddr_in proxysecond_addr,servaddr,cliaddr;
int sock,sock2,new_socket,valread,len,i;



int main(int argc,char const*argv[])
{
	
	
// Socket Creation

	sock = socket(AF_INET,SOCK_STREAM,0);

	if(sock<0)
	{
		printf("\n Socket Creation for Server error");
		return -1;
	}
	else
		printf("\n Socket successfully Created to Server");
	
	sleep(2);

	memset(&servaddr,'0',sizeof(servaddr));

	
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	
	
	
	if(connect(sock,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
	{
		printf("\n Connection Failure with Server");
		return -1;
	}
	else
		printf("\n Connection Successful with Server");
	
	sleep(2);

	
	sock2 = socket(AF_INET,SOCK_DGRAM,0);
	
	if(sock2<0)
	{
		printf("\n Socket creation with Proxy Server 2	Failure ");
	}
	else
		printf("\n Socket Creation Successful with Proxy Server 2 ");

	
	proxysecond_addr.sin_family = AF_INET;
	proxysecond_addr.sin_port = htons(PORT2);
	proxysecond_addr.sin_addr.s_addr = INADDR_ANY;
	
/*	if(bind(sock2,(struct sockaddr*)&proxysecond_addr,sizeof(proxysecond_addr))<0)
	{
		printf("\n Binding Failed with TCP Proxy 2");
	}
	else
		printf("\n Binding Successful with TCP Proxy 2");    */
	
	{
		printf("-------------------------------------------------------------\n");
		printf("\n Sending: Request to Server");
		send_request(sock);
		sleep(1);
		receive_response(sock);
		printf("\n Response Recieved From Server");
		sleep(7);
		printf("\n\n Response Forwarded to Proxy Server 2");
		
		sendto(sock2,rcv_data,256,0,(const struct sockaddr*)&proxysecond_addr,sizeof(proxysecond_addr)); 
		printf("\n Forwarded Data [");
		int n = rcv_data[5] + 6;
		for(i = 0;i < 15;i++)
		{
			printf(" %02x ",rcv_data[i]);
		}
		printf("]\n");
		sleep(5);
		printf("-------------------------------------------------------------\n");
		sleep(5);
	}
	
	
	return 0;
}

void send_request(int sock) {
	snd_data[0] = 0x00;				// Transition Identifier High Bit
	snd_data[1] = 0x00;				// Transition Identifier Low Bit
	snd_data[2] = 0x00;				// Protocol Identifier High Bit
	snd_data[3] = 0x00;				// Protocol Identifier Low Bit
	snd_data[4] = 0x00;				// Length High Bit
	snd_data[5] = 0x06;				// Length Low Bit
	snd_data[6] = 0x01;				// Unit Identifier
	snd_data[7] = 0x01;				// Function Code
	snd_data[8] = 0x00;				// Starting Address High bit
	snd_data[9] = 0x0F;				// Starting Address Low bit
	snd_data[10] = 0x00;				// Count High bit
	snd_data[11] = 0x03;				// Count Low bit
	
	
	send(sock, snd_data, 12, 0);
       	printf("\nsent data    [");
        for(i = 0; i < 12; i++) {
           	printf(" %02x ", snd_data[i]);
       	}
        printf("]\n");
       
}

void receive_response(int sock) {
	recv(sock, rcv_data, 256, 0);					
	int n = rcv_data[5] + 6;
	printf("\n Recieved Data [");
	for(i = 0; i< 15 ; i++)
	{
		printf(" %02x ",rcv_data[i]);
	}
	printf("]\n");
	close(sock);
	
       
}


void forward_response(int sock){
	
	
	sendto(sock,rcv_data,256,MSG_CONFIRM,(const struct sockaddr *)&proxysecond_addr,len); 
	printf("\n Forwarded Data [");
	int n = rcv_data[5] + 6;
	for(i = 0;i < 15;i++)
	{
		printf(" %02x ",rcv_data[i]);
	}
	printf("]\n");
}

