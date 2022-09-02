#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <vector>
#include <sstream>

#define TEMP_FILENAME ".\\temp\\tempfile.temp"
#define GZIP_EXE "gzip.exe"

#define TEXT_FILE 1
#define BIN_FILE 2

#define IMAGE_EXTENTIONS_NUMBER 5

class Request {
	public:
		Request(std::string);
		std::string parserequest(void);
		std::string getRaw(void);
		std::string getelement(std::string);
		int checkfiletype(std::string);
		
	private:
		// Functions
		std::vector<std::string> parselines(void);
		std::string getdocumentname(std::string);
		
		
		// Varribles
		std::vector<std::string> vect;
		std::string raw;
		
		// Constants
		const char* imgaeextentions[IMAGE_EXTENTIONS_NUMBER] = {".JPG", ".JPE", ".BMP", ".GIF", ".PNG"};
};

#endif
