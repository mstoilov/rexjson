#include <iostream>
#include <fstream>
#include <stdexcept>
#include <type_traits>

#include "rexjson/rexjsonproperty.h"
#include "rpctestserver.h"

template<typename T>
struct DT {
	DT()
	// {}
	;
};

rexjson::array get_sequence(size_t count)
{
	rexjson::array ret;
	for (size_t i = 0; i < count; i++)
		ret.push_back(i);
	return ret;
}

std::vector<int> get_sequence_vector(size_t count)
{
	std::vector<int> ret;
	for (size_t i = 0; i < count; i++)
		ret.push_back(i+10);
	return ret;
}

std::vector<int> get_echo_vector(const std::vector<int>& v)
{
	std::vector<int> ret;
	for (const auto& i : v)
		ret.push_back(i);
	return ret;
}

std::vector<int> get_echoadd_vector(const std::vector<int>& v, const int& add)
{
	std::vector<int> ret;
	for (const auto& i : v)
		ret.push_back(i + add);
	return ret;
}

std::vector<int> get_echoarrayadd_vector(const rexjson::array& v, const int& add)
{
	std::vector<int> ret;
	for (const auto& i : v)
		ret.push_back(i.get_int() + add);
	return ret;
}

int get_add_result(const rexjson::object& v)
{
	return v["a"].get_int() + v["b"].get_int();
}


void register_rpc_methods(rpc_test_server& server)
{
	server.add("get_sequence", rexjson::make_rpc_wrapper(get_sequence, "std::vector<int> get_sequence(size_t count)"));
	server.add("get_sequence_vector", rexjson::make_rpc_wrapper(get_sequence_vector, "std::vector<int> get_sequence_vector(size_t count)"));
	server.add("get_echo_vector", rexjson::make_rpc_wrapper(get_echo_vector, "std::vector<int> get_echoadd_vector(const std::vector<int>& v, const int& add)"));
	server.add("get_echoadd_vector", rexjson::make_rpc_wrapper(get_echoadd_vector, "std::vector<int> get_echoadd_vector(const std::vector<int>& v, const int& add)"));
	server.add("get_echoarrayadd_vector", rexjson::make_rpc_wrapper(get_echoarrayadd_vector, "std::vector<int> get_echoarrayadd_vector(const rexjson::array& v, const int& add)"));
	server.add("get_add_result", rexjson::make_rpc_wrapper(get_add_result, "int get_add_result(rexjson::object& v)"));
}

int main(int argc, const char *argv[])
{
	try {
		rpc_test_server server;
		std::stringstream iss;

		register_rpc_methods(server);

		if (argc > 1) {
			std::ifstream file(argv[1]);
			if (!file.is_open())
				throw std::runtime_error(std::string("File: " + std::string(argv[1]) + " can't be open."));
			iss << file.rdbuf();
		} else {
			iss << std::cin.rdbuf();
		}

		rexjson::value ret = server.call(iss.str());
		if (ret["error"].get_type() != rexjson::null_type) {
			std::cout << ret["error"]["message"] << std::endl;
		} else {
			std::cout << ret["result"] << std::endl;
		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
