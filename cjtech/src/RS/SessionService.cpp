/*************************************************************************
	> File Name: SessionService.cpp
	> Author: cooperz
	> Mail: zbzcsn@qq.com
	> Created Time: Sat 29 Aug 2015 11:28:54 PM CST
 ************************************************************************/
#include "SessionService.h"

#include<iostream>
using namespace std;

namespace cjtech
{
    namespace RootServer
    {
        SessionService::SessionService()
        {

        }
        
        SessionService::~SessionService()
        {

        }

        void SessionService::RequsetHandler( ClientSession* cli_session,
                ClientMessage* req_msg)
        {
            /*1找到为其服务的NodeSession，2将消息从json转到对应的protobuf.
             *3注意满负载时，需要直接回绝客户端*/

        }

        void SessionService::NodeHandler( NodeSession* node_session,
                NodeMessage* req_msg)
        {

        }
    }
}
