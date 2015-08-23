/*************************************************************************
	> File Name: RootSession.h
	> Author: cooperz
	> Mail: zbzcsn@qq.com
	> Created Time: Sat 08 Aug 2015 01:39:35 AM UTC
 ************************************************************************/

#ifndef _ROOTSESSION_H
#define _ROOTSESSION_H

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#define DEBUG 2

using boost::asio::ip::tcp;  

namespace cjtech{
    namespace RootServer{
        class NodeMessage;
        class NodeSession
        {
            public:
                NodeSession(boost::asio::io_service& io_service);
                virtual ~NodeSession();
                tcp::socket& GetSocket();
                void Start();
                void HandleProtobufHeaderLen(const boost::system::error_code& error);
                void HandleProtobufBody(const boost::system::error_code& error);
                void HandleFileBody(const boost::system::error_code& error);
                void SendbackResult(const boost::system::error_code& error);
                void PicMatch(const boost::system::error_code& error);
                void QueryMqsql(/*  */);
                void SetMsgBack(/*  */);
            private:
                std::string _RandomNum();
                std::string _WriteToFile(char* msg, int len);
                NodeSession( const NodeSession& root_session);
                NodeSession& operator=(const NodeSession);
            private:
                NodeMessage* _msg_;
                tcp::socket _socket_;   
        };
    }
}

#endif
