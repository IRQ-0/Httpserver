#include <iostream>
#include "Request.h"
#include "Httpserver.h"

int main(int argc, char** argv) {
	Httpserver httpserver("127.0.0.1", 80, "home", LOG_ON, "logs");
	httpserver.start();
	
	
	return 0;
}
