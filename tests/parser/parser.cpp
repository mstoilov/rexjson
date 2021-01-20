#include <iostream>
#include <fstream>
#include "rexjson/rexjson++.h"

int main(int argc, const char *argv[])
{
	try {
		std::stringstream iss;

		if (argc > 1) {
			std::ifstream file(argv[1]);
			iss << file.rdbuf();
		} else {
			iss << std::cin.rdbuf();
		}
		std::cout << rexjson::read(iss.str()).write(true) << std::endl;
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
