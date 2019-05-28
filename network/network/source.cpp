#include <stdio.h>
#include <WinSock2.h>
#include <stdlib.h>

#pragma comment(lib,"ws2_32")

int main() {
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2, 2), &wsaData)!=0)
		return 1;

	SOCKET server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET) {
		printf("server socket error\n");
		exit(1);
	}

	SOCKADDR_IN server_addr = {};
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(10000);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(server_socket, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
		printf("bind error\n");
		exit(1);
	}

	if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
		printf("listen error\n");
		exit(1);
	}
	

	SOCKET client_socket;
	SOCKADDR_IN client_addr;
	int addr_len;
	printf("Wait Client\n");
	addr_len = sizeof(client_addr);
	client_socket = accept(server_socket, (SOCKADDR*)&client_addr, &addr_len);

	char buffer[1024] = {};
	recv(client_socket, buffer, 1024, 0);
	printf("Recv Msg : %s\n", buffer);

	FILE *fp;
	char message[1024] = {};
	char tmp[100];
	fp = fopen(buffer, "r");
	if (fp == NULL) {
		strcpy(message, "404 NOT FOUND");
	}
	else {
		while (!feof(fp)) {
			fgets(tmp, sizeof(tmp), fp);
			strcat(message, tmp);
		}
	}
	send(client_socket, message, strlen(message), 0);
	printf("Send Msg : %s\n", message);

	fclose(fp);
	closesocket(client_socket);
	closesocket(server_socket);

	WSACleanup();
	return 0;
}