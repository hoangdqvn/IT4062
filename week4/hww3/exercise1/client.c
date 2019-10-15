#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFF_SIZE 255
int main(int argc, char *argv[])
{
	int sockfd, rcvBytes, sendBytes;
	unsigned int len;
	char buff[BUFF_SIZE];
	struct sockaddr_in servaddr;
	char *serv_IP;
	short serv_PORT;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s <ServerIP> [<EchoPort>]\n", argv[0]);
		exit(1);
	}
	serv_IP = argv[1];
	serv_PORT = atoi(argv[2]);

//Step 1: Construct socket
	if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		perror("Error: ");
		return 0;
	}
//Step 2: Define the address of the server
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(serv_IP);
	servaddr.sin_port = htons(serv_PORT);
//Step 3: Communicate with server
	printf("Enter to exit\n");
	do {
		printf("Send to server: ");
		fgets(buff, BUFF_SIZE, stdin);
		if(buff[0] == '\n') exit(1);
		buff[strlen(buff) - 1] = '\0';
		//scanf("%s", buff);
		

		len = sizeof(servaddr);
		sendBytes = sendto(sockfd, buff, strlen(buff), 0,(struct sockaddr *) &servaddr, len); //send message to server
		if(sendBytes < 0){
			perror("Error 1");
			return 0;
		}
	
		for(;;){
			rcvBytes = recvfrom(sockfd, buff, BUFF_SIZE, 0,(struct sockaddr *) &servaddr, &len); //receive message from server
			if(rcvBytes < 0){
				perror("Error 2");
				return 0;
			}
			buff[rcvBytes] = '\0';
			if(strcmp(buff,"ok") == 0) {
				printf("--------------------\n");
				break;
			}
			printf("Reply from server: %s\n", buff);
		}
	} while(1);
	
	close(sockfd);
	return 0;
}