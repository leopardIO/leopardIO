
#include "ClientSession.h"
#include <iostream>
#include <sys/syscall.h>
#include <pthread.h>
#include <glog/logging.h>
#include <glog/raw_logging.h>
#include <openssl/md5.h>

#include "ClientMessage.h"
#include "SessionService.h"

using namespace std;
using boost::asio::ip::tcp;  

extern cjtech::RootServer::SessionService* g_session_service;

namespace cjtech{
    namespace RootServer{
        boost::mutex g_mtx;
        ClientSession::ClientSession(boost::asio::io_service& io_service)
            : _socket_(io_service)
        {

        }

        ClientSession::~ClientSession()
        {
            //        boost::mutex::scoped_lock lock(cout_mtx);
        }

        tcp::socket& ClientSession::socket()
        {
            return _socket_;
        }

        void ClientSession::start()
        {
            LOG(INFO) <<"clientsession::start";
            pthread_t tid =  pthread_self();
            _on_recv_msg_ = new ClientMessage();
            LOG(INFO)<<"ClientSession::Proto shows jeson size = "<<_on_recv_msg_->GetJsonHeaderLen();
            boost::asio::async_read(_socket_, boost::asio::buffer(_on_recv_msg_->GetJsonLenLoa(), 
                        _on_recv_msg_->GetJsonHeaderLen()),
                    boost::bind(&ClientSession::h_json_header_len, this, 
                        boost::asio::placeholders::error));
        }
		

        void ClientSession::h_json_header_len(const boost::system::error_code& error)
        {        
            if(!error)
            {
                pthread_t tid =  pthread_self();
				LOG(INFO)<<"clientsession::json_header_len_recved::tid"<<tid<<std::endl;
                _on_recv_msg_->TranJsonLenCharToInt();
                boost::asio::async_read(_socket_, boost::asio::buffer(_on_recv_msg_->GetJsonBodyLoc(), 
                            _on_recv_msg_->GetJsonBodyLen()),
                        boost::bind(&ClientSession::h_json_body, this, 
                            boost::asio::placeholders::error));
            }
            else 
            {
                LOG(ERROR)<<"while wait read 12char error happen "<<boost::system::system_error(error).what();
            }
        }

        void ClientSession::h_json_body(const boost::system::error_code& error)
        {
            if (!error)
            {
                _on_recv_msg_->ParserJson();
                _on_recv_msg_->TranFileLenToInt();
                LOG(INFO)<<"CLientSession::Jeson body recved : "<<_on_recv_msg_->GetJsonString()<<std::endl;
                LOG(INFO)<<"File length "<<_on_recv_msg_->GetFileBodyLen();
                if(_on_recv_msg_->GetFileBodyLen() == -1)
                {
                    RecvErrorData();
                    return;
                }
                boost::asio::async_read(_socket_,boost::asio::buffer(_on_recv_msg_->GetFileBodyLoc(),
                            _on_recv_msg_->GetFileBodyLen()),
                        boost::bind(&ClientSession::h_file_body, this, 
                            boost::asio::placeholders::error));
            }
            else
            {
                LOG(ERROR)<< boost::system::system_error(error).what();
            }
        }

        void ClientSession::h_file_body(const boost::system::error_code& error)
        {
            if (!error)
            {	
                /*todo:handle file body
                 *g_ClientSession_manager(_on_recv_msg_);
                 */
                LOG(INFO)<<"ClientSession::file body recved"<<std::endl;
                SessionService* g_session_service = SessionService::getInstance();
                g_session_service->RequsetHandler(this, this->_on_recv_msg_);
                _on_recv_msg_ = NULL;
            }
            else
            {
                LOG(ERROR)<< boost::system::system_error(error).what() << std::endl;
            }

        }
        void ClientSession::send2cliover_restart(const boost::system::error_code& error)
        {
            if(!error)
            {
                delete _cli_write_buf_.front();
                _cli_write_buf_.pop();
                if(_cli_write_buf_.size()!=0)
                {
                    ClientMessage* out_msg = _cli_write_buf_.front();     
                async_write(_socket_,boost::asio::buffer(
                            out_msg->GetOutLoc(),
                            out_msg->GetOutLen()),
                        boost::bind(&ClientSession::send2cliover_restart, this,
                            boost::asio::placeholders::error ));
                }
                else{
                _mtx_.unlock(); 
                    RAW_LOG(INFO , "++++++send msg to client over ++++++start+++again+++++++++++++++++++++++++++++++++");
                    //delete _on_send_msg_; 
                   // _on_send_msg_ = NULL;
                   // _send_2_client_mtx_.unlock();
                    this->start();
                }
            }
            else
            {
                LOG(ERROR)<<"while wait write back2 client error : " << boost::system::system_error(error).what() << std::endl;
            }
        }

        void ClientSession::try_send_msg( ClientMessage* msg )
        {

            _mtx_.lock();  
			_cli_write_buf_.push(msg);
            ClientMessage* out_msg = _cli_write_buf_.front();
            LOG(INFO)<<"This msg will be send to Client... : "<<out_msg->GetOutLoc()
                <<" Msg length : "<<out_msg->GetOutLen()<<endl;
                async_write(_socket_,boost::asio::buffer(
                            out_msg->GetOutLoc(),
                            out_msg->GetOutLen()),
                        boost::bind(&ClientSession::send2cliover_restart, this,
                            boost::asio::placeholders::error ));
        }

        void ClientSession::RecvErrorData()
        {
            string error_msg = string("Json Data is Error : ") + _on_recv_msg_->GetJsonString() ;
            _error_data_ = (char*)malloc(error_msg.length());
            memcpy(_error_data_, error_msg.c_str(), error_msg.length());
                async_write(_socket_,boost::asio::buffer(
                            _error_data_,
                            error_msg.length()),
                        boost::bind(&ClientSession::Error_Back2Cli, this,
                            boost::asio::placeholders::error ));
        }
        void ClientSession::Error_Back2Cli(const boost::system::error_code& error)
        {
            free(_error_data_);
        }
    }
}
