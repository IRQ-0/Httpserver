#include "Httpserver.h"

Httpserver::Httpserver(std::string ip, int port, std::string homedir, int log, std::string logpath) {
	this->ip = ip;
	this->port = port;
	this->homedir = homedir;
	this->iflog = log;
	if (log == LOG_ON) {
		std::string fullpath = logpath + "\\" + getdate() + ".log";
		this->file.open(fullpath, std::ios::app);
		if (file.is_open() != TRUE) {
			std::cout << "Cannot create log file" << std::endl;
			exit(1);
		}
	}
}

Httpserver::~Httpserver() {
	this->file.flush();
	this->file.close();
}

int Httpserver::start(void) {
	try {
		Telnet telnet(this->ip, this->port, AS_SERVER);
		socketinfo sockinfo;
		log("Starting server...");
		while (1) {
			client cli = telnet.acceptClient(&sockinfo);
			if (cli == NULL) {
				log("Accecp client error");
			}
			log("Client connected from: " + sockinfo.clientsrcadress + " port: " + std::to_string(sockinfo.clientsrcport));
			std::string buffer;
			buffer.clear();
			while (1) {
				buffer = telnet.recvData(cli, 10000);
				if (buffer.length() <= 0) {
					break;
				}
				Request req(buffer);
				std::string filename = req.parserequest();
				
				if (strcmp(filename.c_str(), "/") == 0) {
					filename = std::string("index.html");
				}
				std::string doc = readFile(filename);
				
				int filetype = req.checkfiletype(filename);
				
				if (filetype == BIN_FILE) {
					int fsize = getbinsize(filename);
					std::string header = createHeader(fsize, filetype);
					int length = header.length();
					send(cli, header.c_str(), length, 0);
					send(cli, readbin(filename), fsize, 0);
					
				} else {
					std::string all = createHeader(doc.length(), filetype) + doc;
					telnet.sendData(all, cli);
				}
				telnet.close(cli);
			}
			telnet.close(cli);
			log("Client disconnected");
		}
	} catch (char err[1000]) {
		log("Socket creating error: " + std::string(err));
		exit(1);
	}
}

size_t Httpserver::getbinsize(std::string filename) {
	FILE *f;
	std::string buffer;
	buffer = this->homedir + std::string("\\") + filename.substr(1);	
	f = fopen(buffer.c_str(), "rb");
	if (!f) {
		return 0;
	}
	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fclose(f);
	
	return size;
}

char* Httpserver::readbin(std::string filename) {
	FILE *f;
	std::string buffer;
	buffer = this->homedir + std::string("\\") + filename.substr(1);	
	f = fopen(buffer.c_str(), "rb");
	if (!f) {
		return nullptr;
	}
	int size = (size_t)getbinsize(filename);
	char* filetab = (char*)malloc(size * sizeof(char*));
	if (filetab == nullptr) {
		return nullptr;
	}
	fread(filetab, 1, size, f);
	fclose(f);
	
	return filetab;
	
}

std::string Httpserver::error404(void) {
	std::string err = std::string("<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\r\n") + 
	std::string("<html>\r\n") +
	std::string("<head>\r\n") +
	std::string("<title>404 Not Found</title>\r\n") +
	std::string("</head>\r\n") +
	std::string("<body>\r\n") +
	std::string("<h1>Not Found</h1>\r\n") +
	std::string("<p>The requested URL was not found on this server.</p>\r\n") +
	std::string("<hr>\r\n") +
	std::string("<address> " + std::string(SERVER_NAME) + " </address>\r\n") +
	std::string("</body>\r\n") +
	std::string("</html>\r\n");
	
	return err;
}



std::string Httpserver::readFile(std::string filename) {
	std::fstream doc;
	std::string buffer;
	doc.open(this->homedir + "\\" + filename, std::ios::in);
	if (doc.is_open() != TRUE) {
		log("Cannot find file" + filename);
		return error404();
	}
	std::stringstream stream;
    stream << doc.rdbuf();
    doc.close();
    
    return stream.str();
	
	
}

std::string Httpserver::createHeader(int size, int doctype) {
	std::string header = std::string("HTTP/1.1 200 \r\n");
	
    header += std::string("Date: ") + getdate() + std::string(" ") + gettime() + std::string("\r\n");
    
    header += std::string("Server: ") + std::string(SERVER_NAME) + ("\r\n");
    
	header += std::string("Accept-Ranges: bytes\r\n");
	
	header += std::string("Content-Type: ");
	if (doctype == BIN_FILE) {
		header += std::string("image/jpeg\r\n");
	} else {
		header += std::string("text/html\r\n");
	}
	
	std::ostringstream ss;
    ss << size;
	header += std::string("Content-Length: ") + ss.str() + std::string("\r\n");
	
	header += std::string("Keep-Alive: timeout=5, max=100\r\n");
	
	header += std::string("Connection: Keep-Alive\r\n");
	
	header += std::string("\r\n");
						 
	return header;
}

inline void Httpserver::log(std::string data) {
	std::string datetime;
	datetime = "[" + getdate() + " " + gettime() + "]";
	std::string all = datetime + "     " + data;
	if (this->iflog == LOG_ON) {
		file << all << std::endl << std::flush;
	}
}


inline std::string Httpserver::getdate(void) {
	std::time_t time = std::time(NULL);
	std::tm* now = std::localtime(&time);
	char datebuffer[1000];
	sprintf(datebuffer, "%d-%d-%d", (now->tm_year + 1900), (now->tm_mon + 1), now->tm_mday);
	std::string date = std::string(datebuffer);
	
	return date;
}

inline std::string Httpserver::gettime(void) {
	std::time_t time = std::time(NULL);
	std::tm* now = std::localtime(&time);
	char datebuffer[1000];
	sprintf(datebuffer, "%d:%d:%d", now->tm_hour, now->tm_min, now->tm_sec);
	std::string date = std::string(datebuffer);
	
	return date;
}
