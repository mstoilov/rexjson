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

void print_string(std::string str)
{
	std::cout << str << std::endl;
}

rexjson::value str_to_value(const std::string& str)
{
	return rexjson::value(str);
}

class Person
{
public:
	Person(const std::string& name, size_t day, size_t month, size_t year, float weight)
		: name_(name), day_(day), month_(month), year_(year), weight_(weight)
	{
	}

	rexjson::array get_sequence(size_t count)
	{
		rexjson::array ret;
		for (size_t i = 0; i < count; i++)
			ret.push_back(i);
		return ret;
	}

	friend std::ostream& operator<<(std::ostream& os, const Person& p);


public:
	std::string name_;
	size_t day_;
	size_t month_;
	size_t year_;
	float weight_;

	rexjson::property props = rexjson::property_map({
		{"name", {rexjson::property(&name_, rexjson::property_get<std::string>, rexjson::property_set<std::string>)}},
		{"day", {&day_, rexjson::property_get<decltype(day_)>, rexjson::property_set<decltype(day_)>}},
		{"month", {&month_, rexjson::property_get<decltype(month_)>}},
		{"year", {&year_, rexjson::property_get<decltype(year_)>}},
		{"health", rexjson::property_map({
			{"weight", {&weight_, rexjson::property_get<decltype(weight_)>}},
		})}
	});

};

// rexjson::value get_property(std::string propname)
// {
	// return g_props.navigate(propname).get();
// }

// rexjson::value set_property(std::string propname, rexjson::value v)
// {
// 	g_props.navigate(propname).set(v);
// 	return g_props.navigate(propname).get();
// }

std::ostream& operator<<(std::ostream& os, const Person& p)
{
	os << "Name: " << p.name_ << ", DOB: " << p.day_ << "/" << p.month_ << "/" << p.year_ << ", Weight: " << p.weight_;
	return os;
}

void register_rpc_methods(rpc_test_server& server)
{
	server.add("get_sequence", rexjson::make_rpc_wrapper(get_sequence, "std::vector<int> get_sequence(size_t count)"));
	server.add("get_sequence_vector", rexjson::make_rpc_wrapper(get_sequence_vector, "std::vector<int> get_sequence_vector(size_t count)"));
	server.add("get_echo_vector", rexjson::make_rpc_wrapper(get_echo_vector, "std::vector<int> get_echoadd_vector(const std::vector<int>& v, const int& add)"));
	server.add("get_echoadd_vector", rexjson::make_rpc_wrapper(get_echoadd_vector, "std::vector<int> get_echoadd_vector(const std::vector<int>& v, const int& add)"));
	server.add("get_echoarrayadd_vector", rexjson::make_rpc_wrapper(get_echoarrayadd_vector, "std::vector<int> get_echoarrayadd_vector(const rexjson::array& v, const int& add)"));
	server.add("get_add_result", rexjson::make_rpc_wrapper(get_add_result, "int get_add_result(rexjson::object& v)"));
	server.add("print_string", rexjson::make_rpc_wrapper(print_string, "std::string print_string(std::string str)"));
	server.add("str_to_value", rexjson::make_rpc_wrapper(str_to_value, "rexjson::value str_to_value(const std::string& str)"));
}


int main(int argc, const char *argv[])
{
	try {
		Person joe("Joe Dough", 25, 3, 1971, 72.5);
		Person martin("Martin Stoilov", 26, 2, 1972, 70.5);
		rpc_test_server server;
		server.add_property("joe", joe.props);
		server.add_property("martin", martin.props);

		std::stringstream iss;
		register_rpc_methods(server);

		joe.props.enumerate(joe.props, "joe", [](const std::string& path, rexjson::property& prop) { std::cout << path << " : " << prop.get() << std::endl; });
		joe.props.enumerate(martin.props, "martin", [](const std::string& path, rexjson::property& prop) { std::cout << path << " : " << prop.get() << std::endl; });

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
