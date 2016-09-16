// arquivo de servidor

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

int main() {
#ifndef __linux__
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

	SOCKET principalSocket;
	principalSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (principalSocket == SOCKET_ERROR) {
		std::cout << "Ocorreu um erro ao criar socket" << std::endl;
		return 1;
	}

	sockaddr_in enderecoServidor;
	enderecoServidor.sin_family = AF_INET;
	enderecoServidor.sin_port = htons(9999);
	inet_pton(AF_INET, "0.0.0.0", &enderecoServidor.sin_addr);

	int r;
	r = bind(principalSocket, (sockaddr*)&enderecoServidor, sizeof(enderecoServidor));
	r = listen(principalSocket, SOMAXCONN);

	SOCKET clienteSocket;
	sockaddr_in enderecoCliente;
	int enderecoClienteTam = sizeof(sockaddr_in);
	clienteSocket = accept(principalSocket, (sockaddr*)&enderecoCliente, &enderecoClienteTam);

	char buffer;
	recv(clienteSocket, &buffer, 1, 0);
	buffer = toupper(buffer);
	buffer += '\0';

	send(clienteSocket, &buffer, 1, 0);

	WSACleanup();

}
