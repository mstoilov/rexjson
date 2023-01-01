#include "rpctestserver.h"


rpc_test_server::rpc_test_server() 
{
    // auto a = rexjson::make_rpc_wrapper(this, &rpc_test_server::get_sequence, "rexjson::value rpc_test_server::get_sequence(size_t count)");
    // add("get_sequence", rexjson::make_rpc_wrapper(this, &rpc_test_server::get_sequence, "rexjson::value rpc_test_server::get_sequence(size_t count)"));
}

rpc_test_server::~rpc_test_server() 
{

}

