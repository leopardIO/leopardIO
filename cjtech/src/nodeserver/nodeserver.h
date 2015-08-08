/*************************************************************************
	> File Name: nodeserver.h
	> Author: cooperz
	> Mail: zbzcsn@qq.com
	> Created Time: Sat 25 Jul 2015 05:44:29 PM UTC
 ************************************************************************/

#ifndef _NODESERVER_H
#define _NODESERVER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include "ioserverpool.h"

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

#define INVALID_SOCKET   -1
#define SOCKET_ERROR     -1
#define TRUE             0
#define FALSE            -1


namespace cjtech
{
    namespace nodeserver
    {
        class session;
        class server
        {
            public:
                server(short port, int thread_cnt);
                virtual ~server();
                void handle_accept(session* new_session, 
                        const boost::system::error_code& error);
                void run();
            private:
                io_service_pool io_service_pool_;
                boost::asio::ip::tcp::acceptor acceptor_;
        };
    }
}

#endif
