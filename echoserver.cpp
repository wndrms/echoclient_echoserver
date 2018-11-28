#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <time.h>

int main(int argc, char *argv[]) {
	int server_socket, client_socket;
	char message[BUFSIZ];
	
	int socketlist[10];
	int currsocket = 0;
	bool check = false;
	
	fd_set fd_reads;

	if(argc != 2 && argc != 3){
		printf("echoserver <port> [-b]\n");
		exit(1);
	}
	if(argc == 3){
		if(strcmp("-b", argv[2]) == 0) check = true;
		else{
			printf("echoserver <port> [-b]\n");
			exit(1);
		}
	}

	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(server_socket == -1){
		printf("server socket fail\n");
		exit(1);
	}
	printf("server socket open\n");
	
	struct sockaddr_in server_addr, client_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family	= AF_INET;
	server_addr.sin_port	= htons(atoi(argv[1]));
	server_addr.sin_addr.s_addr	= htonl(INADDR_ANY);

	if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
		printf("bind error\n");
		exit(1);
	}
	printf("bind success\n");

	if(listen(server_socket, 5) == -1){
		printf("listen fail\n");
		exit(1);
	}
	printf("listen success\n");
	int cyc = 0;
	while(1){
		time_t startTime, endTime;
		int client_addr_size = sizeof(client_addr);
		client_socket = accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t *)(&client_addr_size));
		if(client_socket == -1){
			printf("client socket fail\n");
			exit(1);
		}
		else if(currsocket >= 10){
			printf("socket full!\n");
			continue;
		}
		else{
			int num_check = 1;
			for(int k=0; k < currsocket; k++){
				if(socketlist[k] == client_socket) num_check = -1;
			}
			if(num_check == 1){
				socketlist[currsocket] = client_socket;
				currsocket++;
				printf("%d\n", currsocket);
				printf("connected client: %d \n", client_socket);
				printf("Start...\n");
			}
		}
		for(int i=0; i < currsocket; i++){
			printf("현재 %d번째 client 입니다\n", i+1);
			printf("Message Receives from %d client...\n", socketlist[i]);
			while(1){
				int rcv = recv(socketlist[i], message, sizeof(message)-1, 0);
				if(rcv == 0) break;
				else if(rcv == -1){
					printf("Receive Error\n");
					break;
				}
				printf("Receive Message : %s\n", message);
				if(check){
					for(int j=0; j < currsocket; j++){
						send(socketlist[j], message, (int)strlen(message), 0);
					}
				}
				else send(socketlist[i], message, (int)strlen(message), 0);
			}
			
		}
	}
	close(server_socket);
	close(client_socket);
	return 0;
}
