
#include <stdio.h>          /* These are the usual header files */
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 5550
#define BUFF_SIZE 8192
#define NUMBEROFDOTSINIPV4 3 //number dots in ipv4
#define NUMBEROFDOTSINIPV6 5 //number dots in ipv6
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
#define MAXLINE 4096
#define EXISTEDFILE "Error: File is existent on server"
#define BUFF_SEND 1024

// global value
char **tokens;
struct in_addr ipv4addr;
struct hostent *host;
struct in_addr **addr_list;
struct in_addr **alias_list;

/*
* Check valid number in range 0 -> 255
* @param char* value
* @return boolean
*/
int validNumber(char *value)
{
    if(!strcmp(value, "0")) {
        return 1;
    }
    return (atoi(value) > 0) && (atoi(value) <= 255);
}

/*
* Split string
* @param char* string
* @param const char a_delim
* @return boolean
* source https://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c
*/
char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}
/*
* Check dots in string equals dots in ip address
* @param char* string
* @return boolean(0,1)
*/
int checkDots(char *str)
{
    tokens = str_split(str, '.');
    if (tokens)
    {
        int i;
        for (i = 0; *(tokens + i); i++)
        {
            // count number elements in array
        }
        if((i-1) == NUMBEROFDOTSINIPV4) {
            return 1;
        }
    }
    return 0;
}
/*
* Check valid Ip
* @param char* string
* @return 1 if valid ip, 0 if invalid ip
*/
int checkIP(char *str)
{
    if(checkDots(str)) {
        if (tokens) {
            int i;
            for (i = 0; *(tokens + i); i++)
            {
                if(!validNumber(*(tokens + i))) {
                    return 0;
                }
                free(*(tokens + i));
            }
            free(tokens);
            return 1;
        }
    }
    return 0;
}
/*
* Check valid port number
* @param int port
* @return 1 if valid port number, else return 0
*/
int validPortNumber(int port) {
	return (port > 0) && (port <= 65535);
}
/*
* Check valid Ip
* @param char* ip
* @return 1 has found ip address, else return 0
*/
int hasIPAddress(char *ip) {
    int i;
    inet_pton(AF_INET, ip, &ipv4addr);
    host = gethostbyaddr(&ipv4addr, sizeof(ipv4addr), AF_INET);
    if (host != NULL)
    {
        return 1;
    }
    return 0;
}
/*
* Check valid File Name
* @param char* file Name
* @void
*/
void toValidFileName(char *fileName) {
    int i;
    for(i = 0; fileName[i] != '\0'; i++) {
        if(fileName[i] == '\n') {
            fileName[i] = '\0';
        }
    }
}
/*
* TO name of file
* @param char* file Name, char* name
* @void name
*/
void toNameOfFile(char *fileName, char* name ) {
    char** tokens = str_split(fileName, '/');
    int i;
    for (i = 0; *(tokens + i); i++)
    {
        // count number elements in array
    }
    strcpy(name, *(tokens + i -1));
}
/*
* Main function
* @param int argc, char** argv
* @return 1
*/
int main(int argc, char **argv){
	char *temp = malloc(sizeof(argv[1]) * strlen(argv[1]));
	strcpy(temp, argv[1]);
	if(argc == 2) {
		printf("Missing server port number!\n"); // Check if missing port number
		exit(0);
	}
	if(argc < 2) {
		printf("Missing server port number and ip address!\n"); // check if missing port number and server ip address
		exit(0);
	}
	if(!checkIP(argv[1])) {
		printf("Invalid Ip Address!\n"); // Check valid Ip Address
		exit(0);
	}
	if(!validPortNumber(atoi(argv[2]))) {
		printf("Invalid Port Number!\n"); // Check Valid Port Number
		exit(0);
	}

	if(!hasIPAddress(temp)) {
		printf("Not found information Of IP Address [%s]\n", temp); // Find Ip Address
		exit(0);
	}
	
	int client_sock;
	char buff[1000];
	struct sockaddr_in server_addr; /* server's address information */
	int msg_len, bytes_sent, bytes_received;
    char sendline[MAXLINE], recvline[MAXLINE];
    FILE *fileptr;
	
	//Step 1: Construct socket
	client_sock = socket(AF_INET,SOCK_STREAM,0);
	int ser_port = atoi(argv[2]);
	//Step 2: Specify server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(ser_port);
	server_addr.sin_addr.s_addr = inet_addr(temp);
	
	//Step 3: Request to connect server
	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0){
		printf("\nError!Can not connect to sever! Client exit imediately! ");
		return 0;
	}
		
	//Step 4: Communicate with server			
	while(1){
		//send message
        printf("Input File name: ");
        bzero(sendline,MAXLINE);
        fgets(sendline, MAXLINE, stdin);
		if(sendline[0] == '\0') {
            printf("Please input file name\n");
            continue;
        }
        char fileName[50];
        toValidFileName(sendline); //checkif null
        printf("fileName: %s\n", sendline); //print file name
        if ((fileptr = fopen(sendline, "rb")) == NULL){
            printf("Error: File not found\n");
            fclose(fileptr);
            continue;
        }
        toNameOfFile(sendline, fileName); // to name of file ex: /quang/abc.txt -> abc.txt

        long filelen;
        fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
        filelen = ftell(fileptr);             // Get the current byte offset in the file       
        rewind(fileptr);    // pointer to start of file
        

        bytes_sent = send(client_sock, fileName, strlen(fileName), 0); //send name of file
        if(bytes_sent <= 0){ // check if false
            printf("\nConnection closed!\n");
            break;
        }

        bytes_received = recv(client_sock, buff, MAXLINE, 0); // recv result of check file  name
        if(bytes_received <= 0){
            printf("\nError!Cannot receive data from sever!\n");
            break;
        }
        
        buff[bytes_received] = '\0';
        if(strcmp(buff, EXISTEDFILE) == 0) { // if file is existed
            printf("%s\n", buff);
            continue;
        }

        bytes_sent = send(client_sock, &filelen, 20, 0); //send file size
        if(bytes_sent <= 0){
            printf("\nConnection closed!\n");
            break;
        }

        int sumByte = 0; //count size byte send
        while(1) {
            int numberByteSend = BUFF_SEND;
            if((sumByte + BUFF_SEND) > filelen) {// if over file size
                numberByteSend = filelen - sumByte; 
            }
            char* buffer = (char *) malloc((numberByteSend) * sizeof(char));
            fread(buffer, numberByteSend, 1, fileptr); // read buffer with size 
            sumByte += numberByteSend; //increase byte send
            bytes_sent = send(client_sock, buffer, numberByteSend, 0);
            if(bytes_sent <= 0){
                printf("\nConnection closed!\n");
                break;
            }
            free(buffer);
            if(sumByte >= filelen) {
                break;
            }
        }
        
        fclose(fileptr); //close file

        bytes_received = recv(client_sock, buff, MAXLINE, 0); // recv result
        if(bytes_received <= 0){
            printf("\nError!Cannot receive data from sever!\n");
            break;
        }

        printf("%s \n", buff); //print result
	}
	
	//Step 4: Close socket
	close(client_sock);
	return 0;
}
