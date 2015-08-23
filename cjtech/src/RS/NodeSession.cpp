/*************************************************************************
  > File Name: NodeSession.cpp
  > Author: cooperz
  > Mail: zbzcsn@qq.com
  > Created Time: Sat 08 Aug 2015 01:39:38 AM UTC
 ************************************************************************/
#include "NodeSession.h"
#include<iostream>
#include "../../common/includeopencv/interface.h"
#include "NodeMessage.h"

using namespace std;

namespace cjtech
{
    namespace RootServer
    {
        NodeSession::NodeSession(boost::asio::io_service& io_service):
            _socket_(io_service)
        {

        }

        NodeSession::~NodeSession()
        {

        }

        tcp::socket& NodeSession::GetSocket()
        {
            return _socket_;
        }

        void NodeSession::Start()
        {
#ifdef DEBUG
            cout<<"rootsession::start"<<endl;
#endif
            _msg_ = new NodeMessage();
            boost::asio::async_read(_socket_,
                    boost::asio::buffer(_msg_->GetInnerMsgHeaderLoc(),
                        _msg_->GetInnerMsgHeaderLen()),
                    boost::bind(&NodeSession::HandleProtobufHeaderLen, this,
                        boost::asio::placeholders::error));
        }

        void NodeSession::HandleProtobufHeaderLen(const boost::system::error_code& error)
        {
            cout<<*(_msg_->GetInnerMsgHeaderLoc())<<endl;
            bool alloc = _msg_->InnerMsgAlloc();
            if(!error && alloc)
            {
#ifdef DEBUG
                cout<<"rootsession::protobuf head recved"<<endl;
#endif
                boost::asio::async_read(_socket_, boost::asio::buffer(_msg_->GetInnerMsgLoc(), 
                            _msg_->GetInnerMsgLen()),
                        boost::bind(&NodeSession::HandleProtobufBody, this, 
                            boost::asio::placeholders::error));
            }
            else
            {

            }
        }

        void NodeSession::HandleProtobufBody( const boost::system::error_code& error)
        {
            bool alloc = _msg_->FileAlloc();
            if( !error && alloc)
            {
                boost::asio::async_read(_socket_, boost::asio::buffer(_msg_->GetFileBodyLoc(), 
                            _msg_->GetFileBodyLen()),
                        boost::bind(&NodeSession::HandleFileBody, this, 
                            boost::asio::placeholders::error));
            }
            else
            {

            }
        }

        void NodeSession::HandleFileBody( const boost::system::error_code& error)
        {
            if(!error)
            {
                cout<<"Match:"<<_msg_->GetFileBodyLen()<<endl;
                /*
                 * todo:error handle
                 * attention;
                 * */
                boost::asio::async_write(_socket_, 
                        boost::asio::buffer(_msg_->write_buf_,_msg_->write_len_),  
                        boost::bind(&NodeSession::SendbackResult, this, 
                            boost::asio::placeholders::error));
            }
            else
            {

            }
        }

        void NodeSession::SendbackResult( const boost::system::error_code& error)
        {
            if(!error)
            {
                cout<<"Send back OK!"<<endl;
            }
            else
            {
                
            }
        }

        string NodeSession::_RandomNum()
        {
            int randnum = 0;
            stringstream rand_num_sstr;
            randnum = rand();
            rand_num_sstr<<randnum;
            return rand_num_sstr.str();
        }

        string NodeSession::_WriteToFile(char* msg, int len)
        {
            string store_name;
            return store_name;
        }
    }
}
