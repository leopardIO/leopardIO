/*************************************************************************
	> File Name: NodeManager.h
	> Author: cooperz
	> Mail: zbzcsn@qq.com
	> Created Time: Sun 23 Aug 2015 02:23:39 AM UTC
 ************************************************************************/

#ifndef _NODEMANAGER_H
#define _NODEMANAGER_H

#include <iostream>
#include <string.h>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

namespace cjtech
{
    namespace RootServer
    {
        struct NodeInfo
        {
            std::string node_ip_;
            std::string node_port_;
            uint32_t node_cpu_;
            uint32_t node_net_io_;
        };

        class NodeManager
        {
            public:
                NodeManager();
                virtual ~NodeManager();
                void UpdateNodeResource( const char* node_ip, const char* port,
                        uint32_t cpu, uint32_t net_io);
                void GetNodeByRotate( char** node_ip, char ** node_port);
                void GetNodeByResource( char** node_ip, char** node_port);
            private:
                std::vector<boost::shared_ptr<NodeInfo>> node_list_;
                std::map< std::string, std::map< std::string , 
                    boost::shared_ptr<NodeInfo>>> node_info_map;
        };
    }
}

#endif
