#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include "Telnet.h"
#include "Request.h"

#define LOG_ON 1
#define LOG_OFF 0

#define SERVER_NAME "UHTTP"

class Httpserver {
	public:
		Httpserver(std::string, int, std::string, int, std::string);
		int start(void);
		~Httpserver();
		
	private:
		// Functions
		inline std::string getdate(void);
		inline std::string gettime(void);
		void log(std::string);
		std::string createHeader(int, int);
		std::string readFile(std::string);
		std::string error404(void);
		char* readbin(std::string);
		size_t getbinsize(std::string);
		
		// Varribles
		std::string ip;
		int port;
		std::string homedir;
		int iflog;
		std::fstream file;
};

#endif
