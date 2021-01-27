#include <iostream>
#include <fstream>
#include "rexjson/rexjson++.h"

int main(int argc, const char *argv[])
{
	std::stringstream iss;
	try {

		if (argc > 1) {
			std::ifstream file(argv[1]);
			iss << file.rdbuf();
		} else {
			iss << std::cin.rdbuf();
		}
		std::cout << rexjson::read(iss.str()).write(true) << std::endl;
	} catch (rexjson::error& e) {
		std::string input = iss.str();
		size_t line = 1, pos = 1;
		for (size_t i = 0; i < e.offset; i++) {
			if (input[i] == '\n') {
				line++;
				pos = 1;
			} else {
				pos++;
			}
		}
		std::cerr << "Unexpected: (" << line << ":" << pos << "):" << e.token << std::endl;
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
