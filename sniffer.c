#include<stdio.h> 
#include<stdlib.h>    
#include<string.h>    
#include<netinet/ip_icmp.h>   
#include<netinet/udp.h>   
#include<netinet/tcp.h>   
#include<netinet/ip.h>   
#include<sys/socket.h>
#include<arpa/inet.h>
 
void checkPacket(unsigned char* , int);
void ip_out(unsigned char* , int);
void tcp_out(unsigned char* , int);
void udp_out(unsigned char * , int);
void icmp_out(unsigned char* , int);
void PrintData (unsigned char* , int);
 
int raw_socket;
FILE *datafile;
int tcp=0,udp=0,icmp=0,others=0,igmp=0,total=0,i,j;
struct sockaddr_in source,dest;
 
int main()
{
    int saddr_size , data_size;
    struct sockaddr saddr;
    struct in_addr in;
     
    unsigned char *buffer = (unsigned char *)malloc(65536); 
     
    datafile=fopen("data.txt","w");
    if(datafile==NULL) printf("Error file.");
    printf("Starting...\n");
    raw_socket = socket(AF_INET , SOCK_RAW , IPPROTO_TCP);
    if(raw_socket < 0)
    {
        printf("Socket Error\n");
        return 1;
    }
    while(1)
    {
        saddr_size = sizeof saddr;
        data_size = recvfrom(raw_socket , buffer , 65536 , 0 , &saddr , &saddr_size);
        if(data_size <0 )
        {
            printf("Recvfrom error\n");
            return 1;
        }
        checkPacket(buffer , data_size);
    }
    close(raw_socket);
    printf("Finished");
    return 0;
}
 
void checkPacket(unsigned char* buffer, int size)
{
    struct iphdr *iph = (struct iphdr*)buffer;
    ++total;
    switch (iph->protocol) 
    {
        case 1:   ++icmp;
            break;
         
        case 2:  ++igmp;
            break;
         
        case 6:  ++tcp;
            tcp_out(buffer , size);
            break;
         
        case 17: ++udp;
            udp_out(buffer , size);
            break;
         
        default: 
            ++others;
            break;
    }
    printf("TCP : %d   UDP : %d   ICMP : %d   IGMP : %d   Total PAckets : %d\r",tcp,udp,icmp,igmp,total);
}
 
void ip_out(unsigned char* Buffer, int Size)
{
    unsigned short iphdrlen;
         
    struct iphdr *iph = (struct iphdr *)Buffer;
    iphdrlen =iph->ihl*4;
     
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
     
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
     
    printf("\n");
    printf("IP Header\n");
	
    printf("   Source IP        : %s\n",inet_ntoa(source.sin_addr));
    printf("   Destination IP   : %s\n",inet_ntoa(dest.sin_addr));
    printf("   IP Version        : %d\n",(unsigned int)iph->version);
    printf("   IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
    
    printf("   TTL      : %d\n",(unsigned int)iph->ttl);
    printf("   Protocol : %d\n",(unsigned int)iph->protocol);
    printf("   Checksum : %d\n",ntohs(iph->check));
}
 
void tcp_out(unsigned char* Buffer, int Size)
{
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)Buffer;
    iphdrlen = iph->ihl*4;
     
    struct tcphdr *tcph=(struct tcphdr*)(Buffer + iphdrlen);
             
    printf("\n\nTCP Packet\n");    
         
    ip_out(Buffer,Size);
         
    printf("\n");
    printf("TCP Header\n");
    printf("   Source Port      : %u\n",ntohs(tcph->source));
    printf("   Destination Port : %u\n",ntohs(tcph->dest));
    printf("   Sequence Number    : %u\n",ntohl(tcph->seq));
    printf("   Acknowledge Number : %u\n",ntohl(tcph->ack_seq));
    
    printf("\n");
    printf("  DATA  ");
    printf("\n");
         
    printf("IP Header\n");
    PrintData(Buffer,iphdrlen);
         
    printf("TCP Header\n");
    PrintData(Buffer+iphdrlen,tcph->doff*4);
         
    printf("Data Payload\n");  
    PrintData(Buffer + iphdrlen + tcph->doff*4 , (Size - tcph->doff*4-iph->ihl*4) );
                         
    printf("\n-------------");
}
 
void udp_out(unsigned char *Buffer , int Size)
{
     
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)Buffer;
    iphdrlen = iph->ihl*4;
     
    struct udphdr *udph = (struct udphdr*)(Buffer + iphdrlen);
     
    printf("\n\nUDP Packet\n");
     
    ip_out(Buffer,Size);           
     
    printf("\nUDP Header\n");
    printf("   Source Port      : %d\n" , ntohs(udph->source));
    printf("   Destination Port : %d\n" , ntohs(udph->dest));
    printf("   UDP Length       : %d\n" , ntohs(udph->len));
    printf("   UDP Checksum     : %d\n" , ntohs(udph->check));
     
    printf("\n");
    printf("IP Header\n");
    PrintData(Buffer , iphdrlen);
         
    printf("UDP Header\n");
    PrintData(Buffer+iphdrlen , sizeof udph);
         
    printf("Data Payload\n");  
    PrintData(Buffer + iphdrlen + sizeof udph ,( Size - sizeof udph - iph->ihl * 4 ));
     
    printf("\n-------");
}
 

 
void PrintData (unsigned char* data , int Size)
{
     
    for(i=0 ; i < Size ; i++)
    {
        
         
        if(i%16==0) printf("   ");
            printf(" %02X",(unsigned int)data[i]);
                 
        if( i==Size-1) 
        {
            for(j=0;j<15-i%16;j++) printf("   "); 
             
            printf("         ");
             
            for(j=i-i%16 ; j<=i ; j++)
            {
                if(data[j]>=32 && data[j]<=128) printf("%c",(unsigned char)data[j]);
                else printf(".");
            }
            printf("\n");
        }
    }
}
