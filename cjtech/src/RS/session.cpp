/*************************************************************************
  > File Name: session.cpp
  > Author: cooperz
  > Mail: zbzcsn@qq.com
  > Created Time: Sat 25 Jul 2015 05:19:21 PM UTC
 ************************************************************************/
#include "session.h"
#include<iostream>

using namespace std;
using boost::asio::ip::tcp;  
namespace cjtech{
    namespace nodeserver{
        session::session(boost::asio::io_service& io_service)
            : _socket_(io_service)
        {

        }

        session::~session()
        {
            //        boost::mutex::scoped_lock lock(cout_mtx);
        }

        tcp::socket& session::socket()
        {
            return _socket_;
        }
        void session::start()
        {
            _msg_ = new TcpMessage();
            boost::asio::async_read(_socket_, boost::asio::buffer(_msg_->GetJsonLenLoa(), 
                        _msg_->GetJsonHeaderLen()),
                    boost::bind(&session::h_json_header_len, this, 
                        boost::asio::placeholders::error));
        }
        void session::h_json_header_len(const boost::system::error_code& error)
        {        
            if(!error)
            {
                _msg_->TranJsonLenCharToInt();
                boost::asio::async_read(_socket_, boost::asio::buffer(_msg_->GetJsonBodyLoc(), 
                            _msg_->GetJsonBodyLen()),
                        boost::bind(&session::h_json_body, this, 
                            boost::asio::placeholders::error));
            }
            else 
            {
                delete this;
            }
        }
        void session::h_json_body(const boost::system::error_code& error)
        {
            if (!error)
            {
                _msg_->ParserJson();
                _msg_->TranFileLenToInt();
                boost::asio::async_read(_socket_, boost::asio::buffer(_msg_->GetFileBodyLoc(), 
                            _msg_->GetFileBodyLen()),
                        boost::bind(&session::h_file_body, this, 
                            boost::asio::placeholders::error));
            }
            else
            {
                delete this;
            }
        }

        void session::h_file_body(const boost::system::error_code& error)
        {
            if (!error)
            {	
                /*todo:handle file body
                *g_session_manager(_msg_);
                */
                _msg_ = NULL;
                this->start();
            }
            else
            {
                delete this;
            }
        }

        void session::h_pic_match(const boost::system::error_code& error)
        {
            if (!error)
            {	
                //进行match的调用
                //
                //
                //
                //            
                query_db();
                //
                //
                //
                //
                sent_result_back();
            }
            else
            {
                delete this;
            }
        }

        void session::query_db(/*  */)
        {
            //进行数据库的操作  
            //返回的结果记录在my_msg_		
        }	

        void session::sent_result_back(/*  */)
        { 
            boost::asio::async_write(_socket_, boost::asio::buffer(_msg_->data(),_msg_->msg_len()),
                    boost::bind(&session::h_json_header_len, this, 
                        boost::asio::placeholders::error));
        }
    }
}
