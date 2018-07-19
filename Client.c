#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define PORT 8000

void send_request(int);
void receive_response(int);



unsigned char snd_data[256];
unsigned char rcv_data[256];

int sock,valread,len;
struct sockaddr_in cliaddr;
int new_socket,i;
	

int main(int argc,char const*argv[])
{
	
	
	sock = socket(AF_INET,SOCK_STREAM,0);
	
	sleep(6);
	
	if(sock<0)
	{
		printf("\n Socket creation error \n");
		
	}
	else 
		printf("\n Socket Created Successfully ");

//	memset(&servaddr,'0',sizeof(servaddr));

	cliaddr.sin_family = AF_INET; // IPv4
	cliaddr.sin_port = htons(PORT);
	cliaddr.sin_addr.s_addr = INADDR_ANY;
	
	sleep(2);

	
	if(connect(sock,(struct sockaddr*)&cliaddr,sizeof(cliaddr))<0)
	{
		printf("\n Connection Failed \n ");
		return -1;
	}
	else 
		printf("\n Connection Successfull ");
	
	sleep(2);
	
	 {
		printf("-------------------------------------------------------------\n");
		printf("\n Sending: Request to Proxy Server 2");
		send_request(sock);
		sleep(1);
		receive_response(sock);
		printf("\n Response Recieved From Proxy Server 2");
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
	for(i = 0; i < n ; i++)
	{
		printf(" %02x ",rcv_data[i]);
	}
	printf("]\n");
	
       
}



