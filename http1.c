#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdlib.h>
#define port "80"				
#define webroot "/home/Utkarsh/httpproject"			// Directory 
int filesize(int fd)
{
	struct stat stat_struct;
	if(fstat(fd, &stat_struct) == -1)
		return(1);
	return (int)stat_struct.st_size;
}
void send_new(int fd,char *msg)
{
	int len = strlen(msg);
	if(send(fd,msg,len,0) == -1)
	{
		printf("Error during send\n");
	}
}
int recv_new(int fd,char *buffer)		
{
	#define EndOfLine "\r\n"
	char *p = buffer;			
	int eol_matched=0;			
	while(recv(fd,p,1,0)!=0)		
	{
		if( *p == EndOfLine[eol_matched])	
		{	
			++eol_matched;		
			if(eol_matched==2)	
			{
				*(p+1-2) = '\0';	
				return(strlen(buffer));	
			}
		}
		else
		{
			eol_matched=0;			
		}
		p++;					
	}
	return(0);
}
			
int connection(int fd)
{
	char request[500],resource[500],*ptr;		
	int fd1,length;
	if(recv_new(fd,request) == 0)
	{
		printf("Recieve failed\n");
	}
	ptr = strstr(request," HTTP/");
	if(ptr == NULL)
	{
		printf("Invalid  HTTP request\n");
	}
	else
	{
		*ptr=0;
		ptr=NULL;
	
		
		if( strncmp(request,"GET ",4) == 0)
		{
			ptr=request+4;
		}
		if(strncmp(request,"HEAD ",5) == 0)
			ptr=request+5;
		if(ptr==NULL)
		{
			printf("Unknown Request !!! \n");
		}
		else
		{
			if( ptr[strlen(ptr) - 1] == '/' )
			{
				strcat(ptr,"index.html");
			}
			strcpy(resource,webroot);
			strcat(resource,ptr);
			fd1 = open(resource,O_RDONLY,0);
			printf("Opening \"%s\"\n",resource);
			if(fd1 == -1)
			{
				printf("404 File not found Error\n");
				send_new(fd,"HTTP/1.0 404 Not Found\r\n");
				send_new(fd,"Server : Utkarsh/Private\r\n\r\n");}
			else
			{
				printf("200 OK!!!\n");
				send_new(fd,"HTTP/1.0 200 OK\r\n");
				send_new(fd,"Server : Utkarsh/Private\r\n\r\n");
				if( ptr == request+4 ) // get
				{
					if( (length = filesize(fd1)) == -1 )
						printf("Error getting size \n");
					if( (ptr = (char *)malloc(length) ) == NULL )
						printf("Error allocating memory!!\n");
					read(fd1,ptr,length);
			
					if(send(fd,ptr,length,0) == -1)
					{
						printf("Send err!!\n");
					}
					free(ptr);
				}
			}
			close(fd);
		}
	}
	shutdown(fd,SHUT_RDWR);
}			
int main()
{
	int sockfd,newfd;
	int err;
	struct addrinfo *res,*p,hints;
	struct sockaddr_storage their_addr;
	socklen_t addr_size;
	int yes=1;
	char ip[INET6_ADDRSTRLEN];
	memset(&hints,0,sizeof(hints));
	hints.ai_family=AF_UNSPEC;
	hints.ai_flags=AI_PASSIVE;
	hints.ai_socktype=SOCK_STREAM;
	printf("Server is open for listening on port 80\n");
	if( (err = getaddrinfo(NULL,port,&hints,&res) ) == -1)
	{
		printf("Err in getaddrinfo : %s\n",gai_strerror(err));
		return(1);
	}
	for(p=res;p!=NULL;p=p->ai_next)
	{		
		if( ( sockfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol) ) == -1)
		{
			printf("Socket error !!!\n");
			continue;
		}
		if( setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1)
		{
			printf("Setsockopt err!!\n");
			return(1);
		}
		if( bind(sockfd,p->ai_addr,p->ai_addrlen) == -1)
		{
			printf("Binding err\n");
			close(sockfd);			
			continue;
		}
		
		break;
	}
	if( listen(sockfd,15) == -1)
	{
		printf("Error in listen\n");
		return(1);
	}
	
	while(1)
	{
	
		char y;	
		addr_size = sizeof(their_addr);
		if( ( newfd = accept(sockfd, (struct sockaddr *)&their_addr,&addr_size) ) == -1)
		{
			printf("Error in accept!\n");
			return(1);
		}
		for(p=res;p!=NULL;p=p->ai_next)
		{
			void *addr;
			
			if(p->ai_family == AF_INET)
			{
				struct sockaddr_in *ip;
				ip = (struct sockaddr_in *)p->ai_addr;
				addr = &(ip->sin_addr);
			}
			if(p->ai_family == AF_INET6)
			{
				struct sockaddr_in6 *ip;
				ip = (struct sockaddr_in6 *)p->ai_addr;
				addr = &(ip->sin6_addr);
			}
			
			inet_ntop(p->ai_family,addr,ip,sizeof(ip));
			printf("Got connection from %s\n",ip);
		}
		connection(newfd);
	}
	freeaddrinfo(res);
	close(newfd);
	close(sockfd);
	return(0);
}