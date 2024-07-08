#include "log.h"

void Log(const char* msg) {
	std::cout << msg << std::endl;
}

void Log(const std::string msg) {
	std::cout << msg << std::endl;
}

void Log(const int& msg) {
	std::cout << msg << std::endl;
}

void Log(const long& msg) {
	std::cout << msg << std::endl;
}

void Log(const float& msg) {
	std::cout << msg << std::endl;
}

void Log(const double& msg) {
	std::cout << msg << std::endl;
}
