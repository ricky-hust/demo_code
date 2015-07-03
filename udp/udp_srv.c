#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define PORT 9999
#define BUFLEN 512
#define NPACK 10

int main(void)
{
	struct sockaddr_in si_me,si_others;
	int s,i,slen=sizeof(si_others);
	char buf[BUFLEN];
	struct timeval tv; 

	if((s=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) == -1) {
		perror("socket");
		exit(1);
	}

	memset((char *)&si_me,0,sizeof(si_me));
	si_me.sin_family=AF_INET;
	si_me.sin_port=htons(PORT);
	si_me.sin_addr.s_addr=htonl(INADDR_ANY);
	if(bind(s,(struct sockaddr *)&si_me,sizeof(si_me)) == -1) {
		perror("bind");
		exit(1);
	}

	fd_set rset;
	FD_ZERO(&rset);
	FD_SET(s,&rset);
	for(i=0;i<NPACK;i++) {
		int ret;
		//FD_ZERO(&rset);
		//FD_SET(s,&rset);
		tv.tv_sec=10;
		tv.tv_usec=0;
		ret=select(s+1,&rset,NULL,NULL,&tv);
		if(ret == -1) {
			if(errno == EINTR)
				continue;
			else {
				perror("select");
				exit(1);
			}
		}
		if(ret == 0) {
			printf("No data within ten seconds\n");
			continue;
		}
		if(FD_ISSET(s,&rset)) {
			int n=recvfrom(s,buf,BUFLEN,0,(struct sockaddr *)&si_others,&slen);
			if(n == -1) {
				perror("recvfrom");
				exit(1);
			}
			printf("Received packet from %s:%d\nData:%s\n\n",
					inet_ntoa(si_others.sin_addr),ntohs(si_others.sin_port),buf);
		}
	}
	close(s);
	return 0;
}
