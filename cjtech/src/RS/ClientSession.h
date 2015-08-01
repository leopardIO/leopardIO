#ifndef SESSION_H
#define SESSION_H
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include "ClientMessage.h"

using boost::asio::ip::tcp;  

namespace cjtech{
    namespace nodeserver{
        class session
        {
            public:
                session(boost::asio::io_service& io_service);
                virtual ~session();
                tcp::socket& socket();
                void start();
                void h_json_header_len(const boost::system::error_code& error);
                void h_json_body(const boost::system::error_code& error);
                void h_file_body(const boost::system::error_code& error);
                void h_pic_match(const boost::system::error_code& error);
                void query_db(/*  */);
                void sent_result_back(/*  */);
            private:
                TcpMessage* _msg_;
                tcp::socket _socket_;   
        };
    }
}

#endif
