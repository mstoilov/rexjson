#include <iostream>
#include <fstream>
#include <stdexcept>
#include "rexjson/rexjson++.h"
#include "rexjson/rexjsonrpc.h"


class rpc_test_server : public rexjson::rpc_server<rpc_test_server>
{

};

int main(int argc, const char *argv[])
{
	try {
		std::stringstream iss;
		rpc_test_server server;

		if (argc > 1) {
			std::ifstream file(argv[1]);
			if (!file.is_open())
				throw std::runtime_error(std::string("File: " + std::string(argv[1]) + " can't be open."));
			iss << file.rdbuf();
		} else {
			iss << std::cin.rdbuf();
		}

		rexjson::value ret = server.call(iss.str());
		std::cout << ret["result"] << std::endl;
//		std::cout << rexjson::read(iss.str()).write(true) << std::endl;
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
