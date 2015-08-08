/*************************************************************************
  > File Name: server.cpp
  > Author: cooperz
  > Mail: zbzcsn@qq.com
  > Created Time: Sat 25 Jul 2015 05:44:23 PM UTC
 ************************************************************************/

#include<iostream>
using namespace std;

#include "nodeserver.h"

#include <sys/socket.h>
#include <netinet/in.h>     //socket
#include <unistd.h>            //gethostname
#include <arpa/inet.h>
#include <string.h>            //memset
#include <string.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "threadpool.h"
#include "session.h"

using std::cout;
using std::cin;
using std::endl;
using boost::asio::ip::tcp;

namespace cjtech{
    namespace nodeserver{
        server::server(short port, int thread_cnt)
            : io_service_pool_(thread_cnt)
              , acceptor_(io_service_pool_.get_io_service(), tcp::endpoint(tcp::v4(), port))
        {
            session* new_session = new session(io_service_pool_.get_io_service());
            acceptor_.async_accept(new_session->socket(),
                    boost::bind(&server::handle_accept, this, 
                        new_session, boost::asio::placeholders::error));
        }
        virtual server::~server()
        {
        }

        void server::handle_accept(session* new_session, 
                const boost::system::error_code& error)
        {
            if (!error)
            {
                new_session->start();
            }
            else
            {
                delete new_session;
            }

            new_session = new session(io_service_pool_.get_io_service());
            acceptor_.async_accept(new_session->socket(),
                    boost::bind(&server::handle_accept, this, 
                        new_session, boost::asio::placeholders::error));
        }

        void server::run()
        {
            io_service_pool_.start();
            io_service_pool_.join();
        }
    }
}

