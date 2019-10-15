/*UDP Echo Server*/
#include <stdio.h>          /* These are the usual header files */
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define PORT 5550  /* Port that will be opened */ 
#define BUFF_SIZE 1024

#define NUMBEROFDOTSINIPV4 3 //number dots in ipv4
#define NUMBEROFDOTSINIPV6 5 //number dots in ipv6
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

// global value
char **tokens;
struct in_addr ipv4addr;
struct hostent *host;
struct in_addr **addr_list;
struct in_addr **alias_list;
int server_sock; /* file descriptors */
char buff[BUFF_SIZE];
int bytes_sent, bytes_received;
struct sockaddr_in server; /* server's address information */
struct sockaddr_in client; /* client's address information */
int sin_size;

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
* Show info Host From Ip Address
* @param char* ip
* @return void
*/
void showInfoFromIP(char *ip, char* str) {
    int i;
    inet_pton(AF_INET, ip, &ipv4addr);
    host = gethostbyaddr(&ipv4addr, sizeof(ipv4addr), AF_INET);
    if (host != NULL)
    {
    	strcat(str, "Official Name: ");
    	strcat(str, host->h_name);
        strcat(str, "\nAlias Name: ");
       
        for (i = 0; host->h_aliases[i] != NULL; i++)
        {
        	strcat(str, host->h_aliases[i]);
        	strcat(str, "\n\0");
        }
    }
    else
    {
    	strcpy(str, "Not found information Of IP Address!\n");
    }
}
/*
* Show info Host From Domain Address
* @param char* domain
* @return void
*/
void showInfoFromDomain(char* domain, char* str) {
    int i;
    host = gethostbyname(domain);
    if (host != NULL) {
    	strcat(str, "Official IP: ");
        addr_list = (struct in_addr **)host->h_addr_list;
        strcat(str, inet_ntoa(*addr_list[0]));
        strcat(str, "\nAlias IP: ");
        for (i = 1; addr_list[i] != NULL; i++)
        {
        	strcat(str, inet_ntoa(*addr_list[i]));
        	strcat(str, "\n\0");
        }
    }
    else {
    	strcpy(str, "Not found information of domain!\n");
    }
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
* Main function
* @param int argc, char** argv
* @return 1 if valid ip, 0 if invalid ip
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

	//Step 1: Construct a UDP socket
	if ((server_sock=socket(AF_INET, SOCK_DGRAM, 0)) == -1 ){  /* calls socket() */
		perror("\nError: ");
		exit(0);
	}
	
	//Step 2: Bind address to socket
	server.sin_family = AF_INET;         
	server.sin_port = htons(port_number);   /* Remember htons() from "Conversions" section? =) */
	server.sin_addr.s_addr = INADDR_ANY;  /* INADDR_ANY puts your IP address automatically */   
	bzero(&(server.sin_zero),8); /* zero the rest of the structure */

  
	if(bind(server_sock,(struct sockaddr*)&server,sizeof(struct sockaddr))==-1){ /* calls bind() */
		perror("\nError: ");
		exit(0);
	}     
	printf("Server running in port %d\n", port_number);
	//Step 3: Communicate with clients
	while(1){
		sin_size=sizeof(client);
    		
		bytes_received = recvfrom(server_sock, buff, BUFF_SIZE-1, 0, (struct sockaddr *) &client,(unsigned int*) &sin_size);
		
		if (bytes_received < 0)
			perror("\nError: ");
		else{
			buff[bytes_received - 1] = '\0';
			printf("[%s:%d]: %s", inet_ntoa(client.sin_addr), ntohs(client.sin_port), buff); // print info of client
			char *temp = malloc(sizeof(argv[1]) * strlen(buff));
	        strcpy(temp, buff);
	        char response[1000] = "";
	        if (checkIP(temp))
	        {
	            showInfoFromIP(buff, response); // save info host of ip in response
	        } 
	        else {
	            showInfoFromDomain(buff, response); // save info host of domain in reponse
	        }
			bytes_sent = sendto(server_sock, response, 2000, 0, (struct sockaddr *) &client, sin_size ); /* send to the client welcome message */
			if (bytes_sent < 0)
				perror("\nError: ");
		}				
	}
	
	close(server_sock);
	return 0;
}