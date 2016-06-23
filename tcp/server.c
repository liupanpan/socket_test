#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define IDC_SERVER_PORT    6666
#define LENGTH_OF_LISTEN_QUEUE     20
#define BUFFER_SIZE                1024


int main(int argc, char **argv)
{
    // set socket's address information
	int i, j, len;
    static struct timeval send_time[10] = {0};
	static struct timeval receive_time[10] = {0};
    struct sockaddr_in   server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(IDC_SERVER_PORT);

    // create a stream socket
    int server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        printf("Create Socket Failed!\n");
        exit(1);
    }

    //bind
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))
    {
        printf("Server Bind Port: %d Failed!\n", IDC_SERVER_PORT);
        exit(1);
    }

    // listen
    if (listen(server_socket, LENGTH_OF_LISTEN_QUEUE))
    {
        printf("Server Listen Failed!\n");
        exit(1);
    }

    while(1)
    {
        struct sockaddr_in client_addr;
        socklen_t          length = sizeof(client_addr);
        int new_server_socket = accept(server_socket, (struct sockaddr*)&client_addr, &length);
        if (new_server_socket < 0)
        {
            printf("Server Accept Failed!\n");
            break;
        }
		for(i = 0; i < 100; i++){
			gettimeofday(&receive_time[i], NULL);
		    char* buffer=(char*)malloc(sizeof(struct timeval));
			
			len = recv(new_server_socket, buffer, sizeof(struct timeval), 0);

		    memcpy(&send_time[i], buffer, sizeof(struct timeval));
			
			free(buffer);
		}

		for(j = 0; j < 100; j++)
		{
			long int tmp = send_time[j].tv_sec - receive_time[j].tv_sec;
			printf("\n>>>>>%ld>>>>>%ld>>>>>\n", tmp, tmp==0 ? ( send_time[j].tv_usec - receive_time[j].tv_usec):(tmp*1000000 + send_time[j].tv_usec - receive_time[j].tv_usec));
			//printf("\n>>>%d>>>>receive time is %ld>>>>>%ld>>>>\n", j, receive_time[j].tv_sec, receive_time[j].tv_usec);
			//printf("\n>>>%d>>>>>>>send time is %ld>>>>>%ld>>>>\n", j, send_time[j].tv_sec, send_time[j].tv_usec);
		}
    }
    close(server_socket);

    return 0;
}
