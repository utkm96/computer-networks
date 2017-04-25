#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<pthread.h>
#include<poll.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<signal.h>
#include<sys/sem.h>
#include<poll.h>
#include<pthread.h>
#include<sys/select.h>
#include<sys/un.h>
#define SA (struct sockaddr*)
#include<netinet/ip.h>
#include<netinet/ip_icmp.h>


unsigned short
checksum (unsigned short *buffererfer, int num)
{
  unsigned long sum;
  for (sum = 0; num > 0; num--)
    sum += *buffererfer++;
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return ~sum;
}

int main (int argc, char *argv[])
{
  if (argc != 2)
    {
      printf ("enter destination for tracerouting\n");
      exit (0);
    }
  int sfd = socket (AF_INET, SOCK_RAW, IPPROTO_ICMP);
  char bufferer[4096] = {0};
  struct ip *ip_header = (struct ip *) bufferer;
  int hop = 0;

  int rand = 1;
  const int *val = &rand;
  if (setsockopt (sfd, IPPROTO_IP, ip_headerINCL, val, sizeof (rand)) < 0)
    printf ("Cannot set HDRINCL!\n");

  struct sockaddr_in addr;
  addr.sin_port = htons (7);
  addr.sin_family = AF_INET;
  inet_pton (AF_INET, argv[1], &(addr.sin_addr));


  while (1)
    {
		ip_header->ip_len = 20 + 8;
      ip_header->ip_id = 10000;
      ip_header->ip_off = 0;
      ip_header->ip_hl = 5;
      ip_header->ip_v = 4;
      ip_header->ip_tos = 0;
      
      ip_header->ip_ttl = hop;
      ip_header->ip_p = IPPROTO_ICMP;
      inet_pton (AF_INET, "172.17.22.22", &(ip_header->ip_src));
      inet_pton (AF_INET, argv[1], &(ip_header->ip_dst));
      ip_header->ip_sum = checksum ((unsigned short *) bufferer, 9);

      struct icmp_header *icmphd = (struct icmp_header *) (bufferer + 20);
	  icmphd->un.echo.id = 0;
      icmphd->un.echo.sequence = hop + 1;
      icmphd->checksum = checksum ((unsigned short *) (bufferer + 20), 4);
      icmphd->type = ICMP_ECHO;
      icmphd->code = 0;
      icmphd->checksum = 0;
      
      sendto (sfd, buf, sizeof(struct ip) + sizeof(struct icmphdr), 0, SA & addr, sizeof addr);
      char buffererf[4096] = { 0 };
      struct sockaddr_in addr2;
      socklen_t len = sizeof (struct sockaddr_in);
      recvfrom (sfd, buffer, sizeof(buffer), 0, SA & addr2, &len);
      struct icmp_header *icmphd2 = (struct icmp_header *) (buffererf + 20);
      if (icmphd2->type != 0)
    printf ("hop limit:%d\n Address:%s\n", hop, inet_ntoa (addr2.sin_addr));
      else
    {
      printf ("Reached destination:%s with hop limit:%d\n",
          inet_ntoa (addr2.sin_addr), hop);
      exit (0);
    }

      hop++;
    }

  return 0;
}