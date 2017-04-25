#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

int main(){
  int udpSocket, n;
  char buffer[1024];
  struct sockaddr_in serverAddr, clientAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size, client_addr_size;
  int i;

  udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(7891);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero,'\0', sizeof(serverAddr.sin_zero));  

  bind(udpSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr));

  addr_size = sizeof(serverStorage);

  while(1){
    n = recvfrom(udpSocket,buffer,1024,0,(struct sockaddr*)&serverStorage, &addr_size);

    for(i=0;i<n-1;i++)
      buffer[i] = toupper(buffer[i]);

    sendto(udpSocket,buffer,n,0,(struct sockaddr*)&serverStorage,addr_size);
  }

  return 0;
}

