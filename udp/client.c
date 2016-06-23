#include <sys/types.h>
#include <sys/socket.h>                     
#include <stdio.h>
#include <netinet/in.h>                        
#include <arpa/inet.h>                    
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <pthread.h>

#define MYPORT  6666
#define BUFFER_SIZE 1024


int main()
{
    int sock_cli = socket(AF_INET, SOCK_DGRAM, 0);
	int i, len;
    struct timeval send_time;
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("connect");
		exit(1);
	}

    for(i = 0; i < 100; i++){
	    gettimeofday(&send_time, NULL);	
		printf("\n>>>%d>>>>send time is %ld>>>>>%ld>>>>\n", i, send_time.tv_sec, send_time.tv_usec);
	    char* buffer = (char*)malloc(sizeof(send_time));
	    memcpy(buffer, &send_time, sizeof(send_time));
			
		len = sendto(sock_cli, buffer, sizeof(send_time), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));

		free(buffer);
    }
    close(sock_cli);
    printf("Send over!!!\n");

    return 0;
}
