#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int client_socket;
	char message[BUFSIZ];
	
	if(argc != 3){
		printf("echoserver <host> <port>\n");
		exit(1);
	}

	client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(client_socket == -1){
		printf("client socket fail\n");
		exit(1);
	}
	printf("client socket open\n");

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family	= AF_INET;
	server_addr.sin_port	= htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr	= inet_addr(argv[1]);

	if(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
		printf("connect fail\n");
		exit(1);
	}
	printf("Connect success!\n");
	
	while(1){
		printf("Send Message : ");
		fgets(message, sizeof(message), stdin);
		
		send(client_socket, message, (int)strlen(message), 0);
		printf("Message Receives ...\n");
		int rcv = recv(client_socket, message, sizeof(message)-1, 0);
		if(rcv == -1){
			printf("Receive Error\n");
			break;
		}
		printf("Receive Message : %s\n", message);
	}

	close(client_socket);
	return 0;
}
		
