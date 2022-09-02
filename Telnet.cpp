#include "Telnet.h"

Telnet::Telnet(std::string ip, int port, int clisrv) {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		throw("Unable to load Winsock DLL");
	}
	this->mainsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(this->mainsocket == INVALID_SOCKET) {
    	WSACleanup();
    	std::ostringstream ss;
    	ss << port;
    	throw("Unable to create socket on (\"" + ip + "\"" + ss.str());
	}
	struct sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(ip.c_str());
	service.sin_port = htons(port);
	if (clisrv == AS_CLIENT) {
		if (connect(this->mainsocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
			WSACleanup();
			std::ostringstream ss;
			ss << port;
			throw("Unable to connect to (\"" + ip + "\", " + ss.str());
		}
	} else {
		if(bind(this->mainsocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
    		closesocket(this->mainsocket);
    		throw("Unable to associate a local address with a socket");
    	}
    	if(listen(this->mainsocket, 1) == SOCKET_ERROR) {
    		closesocket(this->mainsocket);
    		throw("Unable to make socket listening");
		}
	}
}

client Telnet::acceptClient(socketinfo* sockinfo) {
	client acceptSocket = SOCKET_ERROR;
	sockaddr_in sockaddr;
	int len = sizeof(sockaddr);
	while (acceptSocket == SOCKET_ERROR) {
		acceptSocket = accept(this->mainsocket, (SOCKADDR*)&sockaddr, &len);
	}
	if (acceptSocket == INVALID_SOCKET) {
		return NULL;
	}
	sockinfo->clientsrcport = sockaddr.sin_port;
	sockinfo->clientsrcadress = inet_ntoa(sockaddr.sin_addr);
	
	return acceptSocket;
}

client Telnet::getSocket() {
	return this->mainsocket;
}

int Telnet::sendData(std::string str, client sock) {
	int sent = send(sock, str.c_str(), str.length(), 0);
	if (sent == SOCKET_ERROR) {
		return NULL;
	}
	return sent;
}

std::string Telnet::recvData(client sock, int maxsize) {
	char* buffer = new char[maxsize];
	memset(buffer, 0, sizeof(buffer));
	int recived = recv(sock, buffer, maxsize, 0);
	if (recived <= 0) {
		return "";
	}
	return std::string(buffer);
}

void Telnet::close(client cli) {
	closesocket(cli);
}
