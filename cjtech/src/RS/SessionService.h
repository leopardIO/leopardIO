/*************************************************************************
	> File Name: SessionService.h
	> Author: cooperz
	> Mail: zbzcsn@qq.com
	> Created Time: Sun 23 Aug 2015 07:34:57 AM UTC
 ************************************************************************/

#ifndef _SESSIONSERVICE_H
#define _SESSIONSERVICE_H

#include <map>
#include <boost/thread.hpp>
#include "ClientSession.h"
#include "ClientMessage.h"
#include "NodeSession.h"

namespace cjtech
{
    namespace RootServer
    {
        using boost::asio::ip::tcp;
        class SessionService
        {
            public:
                SessionService();
                virtual ~SessionService();
                void RequsetHandler( ClientSession* cli_session, 
                        ClientMessage* req_msg); 
                void NodeHandler( NodeSession* node_session,
                        NodeMessage* req_msg);
            private:
                boost::mutex _mtx_;  
                uint64_t _task_id_;
                std::map< uint64_t, ClientSession*> _taskid_clises_;
                std::map< uint64_t, NodeSession*> _taskid_ndeses_;
        };
    }
}

#endif
