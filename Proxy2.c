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

#define PORT3 8000
#define PORT2 8080
#define MAXLINE 1024


void receive_request(int);
void forward_response(int);
void receive_response(int);


unsigned char snd_data[256];
unsigned char rcv_data[256];
unsigned char rcv_data_proxy[256];

struct sockaddr_in servaddr,cliaddr;
int sockfd,sock2,new_socket_p,new_socket_c,valread,len;
int valread_c,i;


int main(int argc,char const*argv[])
{
	
	
// Socket Creation
	sock2 = socket(AF_INET,SOCK_STREAM,0);
	
	if(sock2<0)
	{
		printf("\n Socket creation error \n");
		
	}
	else 
		printf("\n Socket Created Successfully ");


	cliaddr.sin_family = AF_INET; // IPv4
	cliaddr.sin_port = htons(PORT3);
	cliaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if(bind(sock2,(const struct sockaddr*)&cliaddr,sizeof(cliaddr))<0)
	{
		printf("\n Binding Failure with Client ");
		return -1;
	}
	else
		printf("\n Binding Successful with Client");
	
	sleep(2);


	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	
	if(sockfd<0)
	{
		printf("\n Socket Creation with Proxy Server 1 error \n");
		return -1;
	}
	else
		printf("\n Socket successfully Created to Proxy Server 1 ");
	
	sleep(2);

//	memset(&proxysecondaddr,'0',sizeof(proxysecondaddr));
	
	
	
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_port = htons(PORT2);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	
/*	if(inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr)<=0)
	{
		printf("\n Invalid address / Address not Supported \n");
		return -1;
	}   */
	 
	

	if(bind(sockfd,(const struct sockaddr*)&servaddr,sizeof(servaddr))<0)
	{
		printf("\n Binding Failure with Proxy Server 1 ");
		return -1;
	}
	else
		printf("\n Binding Successful with Proxy Server 1 "); 
	
	
	
	
	

	
//	listen(sock,5);

/*	if((new_socket_p = accept(sock,(struct sockaddr*)&servaddr,(socklen_t*)&len))<0)
	{
		printf("\n Could not accept with proxy 1 ");
	}
	else 
		printf("\n Connection Accepted with proxy "); */

	sleep(2);

		
	listen(sock2,5);
	
	if((new_socket_c = accept(sock2,(struct sockaddr*)&cliaddr,(socklen_t*)&len))<0)
	{
		printf("\n Could not accept with Client ");
	}
	else 
		printf("\n Connection Accepted with Client ");  
	
	{
		printf("-------------------------------------------------------------\n");
		printf("\n Request Received From Client");		
		receive_request(new_socket_c);
		sleep(3);
		len = sizeof(servaddr);
		printf("\n Response Received From Proxy Server 1");
		recvfrom(sockfd,rcv_data,256,0,(struct sockaddr*)&servaddr,&len);				
		printf("\n Recieved Data [");
		for(i = 0; i< 15 ; i++)
		{
			printf(" %02x ",rcv_data[i]);
		}
		printf("]\n");
		sleep(1);
		printf("\n Response Forwardad To Client\n");		
		forward_response(new_socket_c);
		sleep(5);
		printf("-------------------------------------------------------------\n");
		sleep(5);
	}		
	
	return 0;
}
	


void receive_request(int sock) {
	recv(sock, rcv_data, 256, 0);					
	int n = rcv_data[5] + 6;
	printf("\n Recieved Data [");
	for(i = 0; i< n ; i++)
	{
		printf(" %02x ",rcv_data[i]);
	}
	printf("]\n");
	
       
}

void receive_response(int sock) {
	
	int valread = recvfrom(sock,rcv_data,256,0,(struct sockaddr*)&servaddr,&len);				
	printf("\n Recieved Data [");
	for(i = 0; i< 15 ; i++)
	{
		printf(" %02x ",rcv_data[i]);
	}
	printf("]\n");
	
       
}


void forward_response(int sock){
	
	send(sock,rcv_data,256,0);
	int n = rcv_data[5] +6;
	printf("\n Forwarded Data [");
	for(i = 0;i < 15;i++)
	{
		printf(" %02x ",rcv_data[i]);
	}
	printf("]\n");
}
		


