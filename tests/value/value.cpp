#include <iostream>
#include <fstream>
#include "rexjson/rexjson++.h"

int main(int argc, const char *argv[])
{
	try {
		rexjson::value v5(5);
		std::cout << v5.write(true, true, 8, 8) << std::endl;
		{
			rexjson::value v("five");
			std::cout << v.write(true, true, 8, 8) << std::endl;
		}
		{
			rexjson::value v(std::string("std::string five"));
			std::cout << v.write(true, true, 8, 8) << std::endl;
		}
		{
			rexjson::value v(true);
			std::cout << v.write(true, true, 8, 8) << std::endl;
		}
		{
			int i = 7;
			rexjson::value v(i);
			std::cout << v.write(true, true, 8, 8) << std::endl;
		}
		{
			unsigned int i = 7;
			rexjson::value v(i);
			std::cout << v.write(true, true, 8, 8) << std::endl;
		}
		{
			long int i = 7;
			rexjson::value v(i);
			std::cout << v.write(true, true, 8, 8) << std::endl;
		}
		{
			unsigned long int i = 7;
			rexjson::value v(i);
			std::cout << v.write(true, true, 8, 8) << std::endl;
		}
		{
			double d = 7;
			rexjson::value v(d);
			std::cout << v.write(true, true, 8, 8) << std::endl;
		}
		{
			float d = 7;
			rexjson::value v(d);
			std::cout << v.write(true, true, 8, 8) << std::endl;
		}
		{
			rexjson::value v(v5);
			std::cout << v.write(true, true, 8, 8) << std::endl;
		}
		{
			rexjson::object o;
			rexjson::value v(o);
			std::cout << v.write(false, true, 8, 8) << std::endl;
		}
		{
			rexjson::value o;
			o.read("{\"v\" : 5}");
			rexjson::value v(o);
			std::cout << v.write(false, true, 8, 8) << std::endl;
		}
		{
			rexjson::value o;
			o.read("[1, 2, 3, \"four\", 5]");
			rexjson::value v(o);
			std::cout << v.write(false, true, 8, 8) << std::endl;
			std::cout << v[2] << std::endl;
		}
		{
			rexjson::value v(5);
			int i;
			float f;
			std::string s;
			v.get<int>(i);
			std::cout << "v.get<int>(i) : " << i << std::endl;
			v = 5.2;
			v.get<float>(f);
			std::cout << "v.get<float>(f) : " << f << std::endl; 
			v = "this is a string";
			v.get<std::string>(s);
			std::cout << "v.get<string>(s) : " << s << std::endl; 
		}


	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
