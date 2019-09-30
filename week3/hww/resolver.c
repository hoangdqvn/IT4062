#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h> 
#include <arpa/inet.h>

int get_ip(char *  , char *);
int get_hostname(char *, char *);
int checkHostnameOrIp(char *);

int main(int argc , char *argv[])
{
	if(argc <2)     
    {
    	printf("Please provide a hostname or ip");         
    	exit(1);
    }
    char *temp = argv[1];
     if(checkHostnameOrIp(temp) == 0)
    {
        char *hostname = argv[1];
        char ip[100];
        if(!get_ip(hostname , ip))
        {
            printf("Official IP: %s\n", ip);
        }
    }
    else
    {
        char *ip = argv[1];
        char hostname[100];
        if(!get_hostname(hostname, ip))
        {
            printf("Official name: %s\n", hostname);
        }
    }
} 

int get_ip(char * hostname , char* ip) 
{
	struct hostent *he;     
    struct in_addr **addr_list;     
    int i;     
    if ((he = gethostbyname(hostname)) == NULL)     
    {
    	//herror("gethostbyname");         
    	printf("Not found information\n");
    	ip = NULL;
    	return 1;
    }     
    addr_list = (struct in_addr **) he->h_addr_list;
    for(i = 0; addr_list[i] != NULL; i++)
    {
    	strcpy(ip , inet_ntoa(*addr_list[i]) );
    }
    return 0;
}

int get_hostname(char *hostname, char *ip)
{
	struct in_addr addr;
	struct hostent* esu;
    inet_aton(ip, &addr);
    //printf("%s\n", addr);
    esu = gethostbyaddr(((const char*)&addr),sizeof(addr), AF_INET);
    if(esu == NULL)
    {
    	printf("Not found information\n");
    	hostname = NULL;
    	return 1;
    }
    strcpy(hostname, esu->h_name);
    return 0;
    //printf("%s\n", esu->h_name);
    //return 0;
}

int checkHostnameOrIp(char *info)
{
	if(info[1] > 47 && info[1] < 58)
		return 1; //is IP
	else
		return 0; // is Hostname
}