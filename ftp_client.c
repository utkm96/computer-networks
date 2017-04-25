#include<string.h>
#include<sys/ioctl.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<stdio.h>
#include<net/if_arp.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb,h>
int main()
{
int serverd,c,s;
char fname[50],sip[25],op[1000];
struct sockaddr_in cli_addr;
struct hostent *host_ent;
FILE *fp;
printf(‘enter thost_ent server ip address”);

scanf(“%s”,sip);
host_ent=gethostbyname(sip0);

serverd=socket(AF_INET,SOCK_STREAM,0);
if(serverd!=1)
printf(“socket created successfully”);
else
printf(“socket is not created’);
cli_addr.sin_family=AF_INET;
cli_addr.sin_port=htons(2500);
cli_addr.sin_addr=*((struct in_addr*)host_ent->h_addr);
c=connect(serverd,(struct sockaddr*)&cli_addr,sizeof(cli_addr));
if(c==0)
printf(“connected to server”);
else
printf(“connection failed”);
printf(“enter file name’);
scanf(“%s”,fname);
send(serverd,fname,sizeof(fname),0);
fp=fopen(fname,”r”);
fopen(op,1000,1,fp);
send(serverd,op,sizeof(op),0);
fclose(fp);
close(serverd);
return 0;
}