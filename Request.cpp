#include "Request.h"

Request::Request(std::string raw) {
	this->raw = raw;
}

std::string Request::parserequest(void) {
	std::string line;
	this->vect = parselines();
	for (int i = 0; i < this->vect.size(); i++) {
		line.clear();
		line = this->vect.at(i);
		if (line.find("GET") != std::string::npos) {
			return getdocumentname(line);
		}
	}
}

std::string Request::getelement(std::string elementname) {
	std::string line;
	for (int i = 0; i < this->vect.size(); i++) {
		line.clear();
		line = this->vect.at(i);
		std::string::size_type n;
		n = line.find(':');
		if (n == std::string::npos) {
			continue;
		}
		if (strcmp(line.substr(0, n).c_str(), elementname.c_str()) == 0) {
			return line.substr(n + 2, std::string::npos);
		}
	}
	
	return "";
}

std::vector<std::string> Request::parselines(void) {
	std::vector<std::string> vect;
	std::string buffer;
	std::istringstream f(this->raw);
	while (std::getline(f, buffer)) {
        vect.push_back(buffer);
    }
    
    return vect;
}

int Request::checkfiletype(std::string filename) {
	std::string::size_type n;
	std::string buffer = "";
	while (1) {
		n = filename.find('.', n + 1);
		if (n == std::string::npos) {
			break;
		}
		buffer = filename.substr(n);
	}
	
	if (buffer == "") {
		return TEXT_FILE;
	}
	std::string temp;
	temp.clear();
	for (int i = 0; i < buffer.length(); i++) {
		temp.push_back(toupper(buffer.at(i)));
	}
	
	filename = temp;
	for (int i = 0; i < IMAGE_EXTENTIONS_NUMBER; i++) {
		if (strcmp(temp.c_str(), imgaeextentions[i]) == 0) {
			return BIN_FILE;
		}
	}
	
	return TEXT_FILE;
}

std::string Request::getRaw(void) {
	return this->raw;
}

std::string Request::getdocumentname(std::string line) {
	std::string::size_type nstart, nend;
	nstart = line.find(' ');
	nend = line.find(' ', nstart + 1);
	
	return (line.substr(nstart, nend - 3)).substr(1);
}

/*
int Request::pack(char* out) {
	std::fstream filein;
	filein.open(TEMP_FILENAME, std::ios::out);
	if (!filein.is_open()) {
		std::cout << "Cannot create file" << std::endl;
		exit(1);
	}
	filein << this->raw;
	std::string command = std::string(GZIP_EXE) + " -k " + std::string(TEMP_FILENAME);
	system(command.c_str());
	FILE *f = fopen(std::string(std::string(TEMP_FILENAME) + ".gz").c_str(), "rb");
	if (!f) {
		std::cout << "Cannot find packed temp file" << std::endl;
		exit(1);
	}
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* tab = (char*)malloc(size * sizeof(char));
	fclose(f);
	DeleteFileA(std::string(std::string(TEMP_FILENAME) + ".gz").c_str());
	memcpy(out, tab, size);
}
*/
