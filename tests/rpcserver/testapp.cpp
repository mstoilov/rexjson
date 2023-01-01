#include <iostream>
#include <fstream>
#include <stdexcept>

#include "rexjson/rexjsonproperty.h"
#include "rexjson/rexjsonpropertyex.h"
#include "rpctestserver.h"



rexjson::array get_sequence(size_t count)
{
	rexjson::array ret;
	for (size_t i = 0; i < count; i++)
		ret.push_back(i);
	return ret;
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

	void register_rpc_methods(rpc_test_server& server, const std::string& prefix)
	{
		server.add(prefix, "get_sequence", rexjson::make_rpc_wrapper(this, &Person::get_sequence, "Person::get_sequence(size_t count)"));
	}


	friend std::ostream& operator<<(std::ostream& os, const Person& p);


public:
	std::string name_;
	size_t day_;
	size_t month_;
	size_t year_;
	float weight_;

	rexjson::property health = rexjson::property_map({
		{"weight", &weight_},
	});
	rexjson::property props = rexjson::property_map({
		{"name", &name_},
		{"day", &day_},
		{"month", &month_},
		{"year", &year_},
		{"health", {health}}
	});

};

std::ostream& operator<<(std::ostream& os, const Person& p)
{
	os << "Name: " << p.name_ << ", DOB: " << p.day_ << "/" << p.month_ << "/" << p.year_ << ", Weight: " << p.weight_;
	return os;
}



void register_rpc_methods(rpc_test_server& server)
{
	server.add("get_sequence", rexjson::make_rpc_wrapper(get_sequence, "get_sequence(size_t count)"));
}

int main(int argc, const char *argv[])
{
	try {
		rpc_test_server server;
		std::stringstream iss;
		Person joe("Joe Dough", 31, 10, 1970, 74.5);
		rexjson::property propname(&joe.name_);
		rexjsonex::property propyear(
			&joe.year_,
			rexjsonex::property_get<size_t>,
			rexjsonex::property_set<size_t>
		);

		rexjsonex::property propday(
			&joe.day_,
			rexjsonex::property_get<size_t>,
			rexjsonex::property_set<size_t>
		);

		rexjsonex::property propmonth(
			&joe.month_,
			rexjsonex::property_get<size_t>,
			rexjsonex::property_set<size_t>
		);

		rexjsonex::property props = rexjsonex::property_map({
			{"joe", rexjsonex::property_map({
				{"year", rexjsonex::property(&joe.year_, rexjsonex::property_get<size_t>, rexjsonex::property_set<size_t>)},
				{"month", {&joe.month_, rexjsonex::property_get<size_t>, rexjsonex::property_set<size_t>}},
				{"day", rexjsonex::property(&joe.day_, rexjsonex::property_get<size_t>, rexjsonex::property_set<size_t>)},
				})
			}
		});

		joe.register_rpc_methods(server, "joe.");
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
		std::cout << ret["result"] << std::endl;
//      std::cout << rexjson::read(iss.str()).write(true) << std::endl;
		propname.set_prop("Jony Hong");
		std::cout << joe << std::endl;

		std::cout << propname.get_prop() << std::endl;
		std::cout << joe.props.navigate("health.weight").get_prop() << std::endl;
		propyear.set(rexjson::value(1972));
		std::cout << propyear.get() << "/" << propmonth.get() << "/" << propday.get() << std::endl;
		std::cout << props["joe"]["year"].get() << "/" << props["joe"]["month"].get() << "/" << props["joe"]["day"].get() << std::endl;
		std::cout << props.navigate("joe.year").get() << "/" << props.navigate("joe.month").get() << "/" << props.navigate("joe.day").get() << std::endl;

		joe.props.enumerate_children("joe", [](const std::string& path, rexjson::property& prop) { std::cout << path << " : " << prop.get_prop() << std::endl; });

		props.enumerate(props, "joeboe", [](const std::string& path, rexjsonex::property& prop) { std::cout << path << " : " << prop.get() << std::endl; });

	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
