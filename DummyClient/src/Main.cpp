#pragma comment(lib, "ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <Windows.h>
#include <iostream>

int main()
{
	WSADATA data;
	if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
	{
		std::cout << WSAGetLastError() << std::endl;
		return -1;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(SOCKADDR_IN));
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6000);
	connect(sock, (SOCKADDR*)&addr, sizeof(addr));
	std::cout << WSAGetLastError() << std::endl;

	char buffer[128] = "hello world";
	send(sock, buffer, (int)strlen(buffer), 0);

	char recvBuffer[128];
	int recvSize = recv(sock, recvBuffer, 128, 0);
	recvBuffer[recvSize] = '\0';
	std::cout << recvBuffer << std::endl;

	getchar();
	WSACleanup();
}