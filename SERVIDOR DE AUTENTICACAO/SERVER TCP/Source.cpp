// arquivo de servidor

#include <mysql_connection.h>
#include <cppconn\driver.h>
#include <cppconn\prepared_statement.h>
#include <cppconn\resultset.h>

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <random>
#include <string>

#define COMANDO_AUTENTICAR_REQ 1
#define COMANDO_AUTENTICAR_RES 2

bool login(std::string& login,std::string& senha) {

	/*std::string login;
	std::string senha;

	std::cout << "Informe email ou apelido" << std::endl;
	std::getline(std::cin, login);
	
	std::cout << "Informe senha" << std::endl;
	std::getline(std::cin, senha);*/

	sql::Driver * driver;
	driver = get_driver_instance();
	sql::Connection* con;
	con = driver->connect("localhost","root","root");
	con->setSchema("jogomulti");

	sql::PreparedStatement* ps;
	ps = con->prepareStatement("SELECT * FROM usuarios WHERE usua_email = ? ");
	ps->setString(1, login.c_str());
	//ps->setString(2, senha.c_str());

	sql::ResultSet* rs;
	rs = ps->executeQuery();

	bool r = rs->next();

	con->close();
	
	return r;
}

int main() {

	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

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
