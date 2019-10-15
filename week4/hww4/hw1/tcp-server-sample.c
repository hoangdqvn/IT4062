
#include <stdio.h>          /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

#define PORT 5550   /* Port that will be opened */ 
#define BACKLOG 2   /* Number of allowed connections */
#define BUFF_SIZE 1024
/*
* Check valid port number
* @param int port
* @return 1 if valid port number, else return 0
*/
int validPortNumber(int port) {
	return (port > 0) && (port <= 65535);
}
/*
* Check valid messages
* @param char* mess
* @return 1 if valid, else return 0
*/
int validMessages(char* mess) {
	int i;
	for(i = 0; i < strlen(mess); i++) {
		if((isalpha(mess[i]) == 0) && (isdigit(mess[i]) == 0)) {
			return 0;
		}
	}
	return 1;
}
/*
* get char from messages
* @param char* mess, char* str
* @return void
*/
void charFromMessage(char* mess, char* str) {
	int i;
	int j = 0;
	for(i = 0; i < strlen(mess); i++) {
		if(isalpha(mess[i]) != 0) {
			str[j++] = mess[i];
		}
	}
}
/*
* get number from messages
* @param char* mess, char* str
* @return void
*/
void numberFromMessage(char* mess, char* str) {
	int i;
	int j = 0;
	for(i = 0; i < strlen(mess); i++) {
		if(isdigit(mess[i]) != 0) {
			str[j++] = mess[i];
		}
	}
}
/*
* main function
* @param int argc, char** argv
* @return void
*/
int main(int argc, char **argv)
{
 	int port_number;
 	if(argc != 2) {
 		perror(" Error Parameter! Please input only port number\n ");
 		exit(0);
 	}
 	if((port_number = atoi(argv[1])) == 0) {
 		perror(" Please input port number\n");
 		exit(0);
 	}
 	if(!validPortNumber(port_number)) {
 		perror("Invalid Port Number!\n");
 		exit(0);
 	}

	int listen_sock, conn_sock; /* file descriptors */
	char recv_data[BUFF_SIZE];
	int bytes_sent, bytes_received;
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in client; /* client's address information */
	int sin_size;
	
	//Step 1: Construct a TCP socket to listen connection request
	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  /* calls socket() */
		perror("\nError: ");
		return 0;
	}
	
	//Step 2: Bind address to socket
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;         
	server.sin_port = htons(port_number);   /* Remember htons() from "Conversions" section? =) */
	server.sin_addr.s_addr = htonl(INADDR_ANY);  /* INADDR_ANY puts your IP address automatically */   
	if(bind(listen_sock, (struct sockaddr*)&server, sizeof(server))==-1){ /* calls bind() */
		perror("\nError: ");
		return 0;
	}     
	
	//Step 3: Listen request from client
	if(listen(listen_sock, BACKLOG) == -1){  /* calls listen() */
		perror("\nError: ");
		return 0;
	}
	
	//Step 4: Communicate with client
	while(1){
		//accept request
		sin_size = sizeof(struct sockaddr_in);
		if ((conn_sock = accept(listen_sock,( struct sockaddr *)&client, (unsigned int*)&sin_size)) == -1) 
			perror("\nError: ");
  
		printf("You got a connection from %s\n", inet_ntoa(client.sin_addr) ); /* prints client's IP */
		
		//start conversation
		while(1){
			//receives message from client
			char number[BUFF_SIZE];
 			char character[BUFF_SIZE];
			bytes_received = recv(conn_sock, recv_data, BUFF_SIZE-1, 0); //blocking
			if (bytes_received <= 0){
				printf("\nConnection closed");
				break;
			}
			else{
				recv_data[bytes_received - 1] = '\0';
				if(!validMessages(recv_data)) {
					char error[100] = "Message has invalid character!!";
					bytes_received = strlen(error);
					bytes_sent = send(conn_sock, error, bytes_received, 0);
					if (bytes_sent <= 0){
						printf("\nConnection closed");
						break;
					}
					continue;
				} else {
					memset(number,0,strlen(number));
					memset(character,0,strlen(character));
					numberFromMessage(recv_data, number);// get number from message save in number variable
					charFromMessage(recv_data, character);// get char from message save in character
					char responseMessage[2000] = "";
					strcat(responseMessage, character); 
					strcat(responseMessage, "\n");
					strcat(responseMessage, number);
					strcat(responseMessage, "\0"); //response
					bytes_received = strlen(responseMessage);
					bytes_sent = send(conn_sock, responseMessage, bytes_received, 0);
					if (bytes_sent <= 0){
						printf("\nConnection closed");
						break;
					}
				}
			}
		}//end conversation
		close(conn_sock);	
	}
	
	close(listen_sock);
	return 0;
}