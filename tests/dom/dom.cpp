#include <iostream>

#include "rexjson/rexjson++.h"

int main(int argc, const char *argv[])
{
    char text1[]="{\n\"name\" : \"Jack (\\\"Bee\\\") Nimble \u0434\u0432\u0435\", \n\"format\": {\"type\":       \"rect\", \n\"width\":      1920, \n\"height\":     1080, \n\"interlace\":  false,\"frame rate\": 24\n}\n}";
    char text2[]="[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";
    char text3[]="[\n    [0.1, -1, 0],\n    [1, 0, 0],\n    [0, 0, 1]\n ]\n";
    char text4[]="{\n       \"Image\": {\n          \"Width\":  800,\n          \"Height\": 600,\n          \"Title\":  \"View from 15th Floor\",\n     \"Thumbnail\": {\n              \"Url\":    \"http:/*www.example.com/image/481989943\",\n               \"Height\": 125,\n              \"Width\":  \"100\"\n           },\n            \"IDs\": [116, 943, 234, 38793]\n       }\n }";
    char text5[]="[\n    {\n     \"precision\": \"zip\",\n   \"Latitude\":  37.7668,\n   \"Longitude\": -122.3959,\n     \"Address\":   \"\",\n  \"City\":      \"SAN FRANCISCO\",\n     \"State\":     \"CA\",\n    \"Zip\":       \"94107\",\n     \"Country\":   \"US\"\n     },\n    {\n     \"precision\": \"zip\",\n   \"Latitude\":  37.371991,\n     \"Longitude\": -122.026020,\n   \"Address\":   \"\",\n  \"City\":      \"SUNNYVALE\",\n     \"State\":     \"CA\",\n    \"Zip\":       \"94085\",\n     \"Country\":   \"US\", \"tst\" : null\n     }\n     ]";

    std::cout << "*** text1 ***\n" << rexjson::read(text1).write(false) << std::endl << std::endl;
    std::cout << "*** text2 ***\n" << rexjson::read(text2).write(false) << std::endl << std::endl;
    std::cout << "*** text3 ***\n" << rexjson::read(text3).write(false) << std::endl << std::endl;
    std::cout << "*** text4 ***\n" << rexjson::read(text4, sizeof(text4) - 1).write(true, true, 8) << std::endl << std::endl;
    std::cout << "*** text5 ***\n";
    rexjson::output(false).write(rexjson::read(text5), std::cout);
    std::cout << std::endl << std::endl;

    rexjson::value val(43);
    rexjson::value v = rexjson::object();
    v["prop1"] = "Value \\\"one\\\"";
    v["prop2"] = "Value \u0434\u0432\u0435 two \u0434\u0432\u0435";
    v["prop3"] = rexjson::array();
    v["prop3"].push_back().read("\"Value 3\"");
    v["prop3"].push_back() = "Value 4";
    v["prop3"].push_back(true);
    v["prop3"].push_back(false);
    v["prop3"].push_back(44);
    v["prop3"].push_back(rexjson::read("{}"));
    v["prop3"][5]["prop1"] = 2.3332;
    v["prop3"][5]["prop2"] = rexjson::value::null;
    v["prop3"][5]["prop3"] = rexjson::object();
    v["prop3"][5]["prop3"]["prop1"] = rexjson::object();
    v["prop3"][5]["prop3"]["prop2"] = rexjson::value::null;
    v["prop3"].push_back(rexjson::value().read("[1,2,3]"));
    v["prop3"].push_back(23.33);
    v["prop3"].push_back(rexjson::array());
    v["prop4"] = true;
    v["prop5"] = 3.345;
    v["prop7"].read("[1, 2, true, false, 0.25, {\"prop1\": true}]");
    std::cout << "*** New Text ***\n" << v.write(true, true, 8, 8) << std::endl;
    return 0;
}
