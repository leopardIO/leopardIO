#ifndef ClientSession_H
#define ClientSession_H

#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::tcp;  

namespace cjtech{
    namespace RootServer{
        class ClientMessage;
        class ClientSession
        {
            public:
                ClientSession(boost::asio::io_service& io_service);
                virtual ~ClientSession();
                tcp::socket& socket();
                void start();
                void h_json_header_len(const boost::system::error_code& error);
                void h_json_body(const boost::system::error_code& error);
                void h_file_body(const boost::system::error_code& error);
                void sent_result_back(const boost::system::error_code& error);
                void try_send_msg( ClientMessage* msg);
                void send2cliover_restart(const boost::system::error_code& error);
                void RecvErrorData();
                void Error_Back2Cli(const boost::system::error_code& error);
            private:
                boost::mutex _mtx_;
                tcp::socket _socket_;
                std::queue<ClientMessage*> _cli_write_buf_;
				ClientMessage*  _on_recv_msg_;
                char* _error_data_;
        };
    }
}

#endif
