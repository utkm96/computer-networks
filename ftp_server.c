#include<string.h>
#include<sys/ioctl.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<stdio.h>
#include<net/if_arp.h>
int main()
{
int serverd,b,clientd;

struct fname[50],op[1000];
struct sockaddr_in cli_addr,ser_addr;

FILE *fp;
socklen_t clen=sizeof(cli_addr);
serverd=socket(AF_INET,SOCK_STREAM,0);

if(serverd!=-1)
printf(“socket is created”);

else
printf(“socket is not created”);

ser_addr.sin_family=AF_INET;
ser_addr.sin_port=htons(2500);
ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
b=bind(serverd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));

if(b==0)
printf(“successful”);

else
printf(“binding failed’);
listen(serverd,5);
clientd=accept(serverd,(struct sockaddr*)&cli_addr,&clen);
recv(clientd,fname,sizeof(fnmae),0);
fp=open(fname,”w”);
fwrite(op,strlen(op),1,fp);
printf(“transfer complete”);
close(fd);
close(clientd);
fclose(fp);
return 0;
}