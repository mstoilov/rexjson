#include <iostream>
#include <fstream>
#include <stdexcept>

#include "rexjson/rexjsonproperty.h"



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

std::ostream& operator<<(std::ostream& os, const Person& p)
{
	os << "Name: " << p.name_ << ", DOB: " << p.day_ << "/" << p.month_ << "/" << p.year_ << ", Weight: " << p.weight_;
	return os;
}


int main(int argc, const char *argv[])
{
	try {
		std::stringstream iss;
		Person joe("Joe Dough", 31, 10, 1970, 74.5);

		std::cout << joe << std::endl;
		std::cout << joe.props["name"].get() << std::endl;
		std::cout << joe.props.navigate("health.weight").get() << std::endl;

		if ( joe.props.navigate("day").access() & rexjson::property_access::access_write )
			joe.props.navigate("day").set(11);
		else
			std::cout << "Property da is not writable" << std::endl;

		joe.props.enumerate(joe.props, "joe", [](const std::string& path, rexjson::property& prop) { std::cout << path << " : " << prop.get() << std::endl; });
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
