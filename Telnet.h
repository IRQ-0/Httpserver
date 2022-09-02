#ifndef TELNET_H
#define TELNET_H
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <string>
#include <sstream>

#define AS_CLIENT 1
#define AS_SERVER 0

#define CONNECTED 1
#define DISCONNECTED 0

typedef SOCKET client;

struct socketinfo {
	std::string clientsrcadress;
	int clientsrcport;
};

class Telnet {
	public:
		Telnet(std::string ip, int port, int method);
		client acceptClient(socketinfo*);
		client getSocket();
		int sendData(std::string, client);
		std::string recvData(client, int);
		void close(client);
	private:
		SOCKET mainsocket;
};

#endif
