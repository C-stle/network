#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32")
#define SERVER_IP "172.26.16.102"

int main() {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 1;

	SOCKET client_socket;
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == INVALID_SOCKET) {
		printf("server socket error\n");
		exit(1);
	}

	SOCKADDR_IN client_addr = {};
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(10000);
	client_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	
	connect(client_socket, (SOCKADDR*)&client_addr, sizeof(client_addr));

	char message[] = "HelloWorld.html";
	send(client_socket, message, strlen(message), 0);

	char buffer[1024] = {};
	recv(client_socket, buffer, 1024, 0);
	printf("Recv Msg\n%s\n", buffer);
	
	closesocket(client_socket);

	WSACleanup();
	return 0;
}