
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#define BUFF_SIZE 255

char number[BUFF_SIZE];
char alphabet[BUFF_SIZE];
int error = 0;
void processRecvBuff(char *receive){	//process message from client with ASCII
	int countNumber = 0, countAlphabet = 0;
	memset(number,'\0',sizeof(number));
	memset(alphabet,'\0', sizeof(alphabet));
	for (int i = 0; i < strlen(receive); i++)
	{
		if(receive[i] > 47 && receive[i] < 58){
			number[countNumber] = receive[i];
			countNumber ++;
		}
		else 
			if((receive[i] > 64 && receive[i] < 91) || (receive[i] > 96 && receive[i] < 123)){
				alphabet[countAlphabet] = receive[i];
				countAlphabet ++;
			}
			else {
				error = 1;
				break;
			}
	}
}

int main(int argc, char *argv[])
{
	int sockfd, rcvSize;
	unsigned int len;
	char buff[BUFF_SIZE];
	char recvBuff[BUFF_SIZE];
	char done[BUFF_SIZE];
	struct sockaddr_in servaddr, cliaddr;
	short serv_PORT;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <UDP SERVER PORT>\n", argv[0]);
		exit(1);
	}
	serv_PORT = atoi(argv[1]);
//Step 1: Construct socket
	if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		perror("Error: ");
		return 0;
	}
//Step 2: Bind address to socket
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(serv_PORT);

	if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("Error: ");
		return 0;
	}
	printf("Server started!\n");
//Step 3: Communicate with client
	for ( ; ; )
	{
		memset(buff,'\0', sizeof(buff));
		memset(done,'\0', sizeof(done));
		len = sizeof(cliaddr);
		rcvSize = recvfrom(sockfd, recvBuff, BUFF_SIZE, 0,(struct sockaddr *) &cliaddr, &len); //receive message from client
		if(rcvSize < 0)
		{
			perror("Error: ");
			return 0;
		}
		recvBuff[rcvSize] = '\0';
		printf("Receive from client: %s\n", recvBuff);

		processRecvBuff(recvBuff);
		if(error == 1){
			strcpy(buff,"error");
			sendto(sockfd, buff, rcvSize, 0,(struct sockaddr *) &cliaddr, len); //error message
			strcpy(done,"ok");
			sendto(sockfd, done, rcvSize, 0,(struct sockaddr *) &cliaddr, len);//send end signal
			error = 0;
		}
		else{
			if(number[0] != '\0')
				sendto(sockfd, number, rcvSize, 0,(struct sockaddr *) &cliaddr, len);
			if(alphabet[0] != '\0')
				sendto(sockfd, alphabet, rcvSize, 0,(struct sockaddr *) &cliaddr, len);
			strcpy(done,"ok");
			sendto(sockfd, done, rcvSize, 0,(struct sockaddr *) &cliaddr, len);//send end signal
		}
	}
	return 0;
}
