//server.h
#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>     //socket
#include <unistd.h>            //Gethostname
#include <arpa/inet.h>
#include <string.h>            //memset
#include <string.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

#include "RootSession.h"
#include "IOServicePool.h"
#include "HeadStructMessage.h"

typedef int SOCKET;
typedef struct sockAddr_in SOCKADDR_IN;
typedef struct sockAddr SOCKADDR;

 
using std::cout;
using std::cin;
using std::endl;
using boost::asio::ip::tcp;

extern 	SessionManager* g_session_manager;  

/*************************************************************************
 *
 *整个系统发送的报文的格式
 *+--------------------+--------------+------------------------+
 *|		       	       |		      |						   |												
 *+--------------------+--------------+------------------------+
 * length&type of proto     proto body        file body
 *
 ************************************************************************/

namespace NodeServer{

	class RootSession
	{
	public:
		RootSession(short port);
		virtual ~RootSession();
		void H_Read_Header(const boost::system::error_code& error);
		void Run();
	private:
		typedef struct  HeadStructMessage Header;
		tcp::acceptor _acceptor_;
		tcp::socket *_p_socket_temp_;
		Header _header_	;
		
	};
}


#endif
