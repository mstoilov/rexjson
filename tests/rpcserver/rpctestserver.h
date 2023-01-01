#ifndef __RPCTESTSERVER_H__
#define __RPCTESTSERVER_H__


#include "rexjson/rexjson++.h"
#include "rexjson/rexjsonrpc.h"


class rpc_test_server : public rexjson::rpc_server<rpc_test_server>
{
public:
    rpc_test_server();
    virtual ~rpc_test_server();

    // rexjson::array get_sequence(size_t count);


};


#endif
