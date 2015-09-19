/*************************************************************************
  > File Name: NodeSession.cpp
  > Author: cooperz
  > Mail: zbzcsn@qq.com
  > Created Time: Sat 08 Aug 2015 01:39:38 AM UTC
 ************************************************************************/
#include "NodeSession.h"
#include <iostream>
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
            _on_recv_msg_ = new NodeMessage();
            boost::asio::async_read(_socket_,
                    boost::asio::buffer(_on_recv_msg_->GetInnerMsgHeaderLoc(),
                        _on_recv_msg_->GetInnerMsgHeaderLen()),
                    boost::bind(&NodeSession::HandleProtobufHeaderLen, this,
                        boost::asio::placeholders::error));
        }

        void NodeSession::HandleProtobufHeaderLen(const boost::system::error_code& error)
        {
            cout<<*(_on_recv_msg_->GetInnerMsgHeaderLoc())<<endl;
            bool alloc = _on_recv_msg_->InnerMsgAlloc();
            if(!error && alloc)
            {
#ifdef DEBUG
                cout<<"rootsession::protobuf head recved"<<endl;
#endif
                boost::asio::async_read(_socket_, boost::asio::buffer(_on_recv_msg_->GetInnerMsgLoc(), 
                            _on_recv_msg_->GetInnerMsgLen()),
                        boost::bind(&NodeSession::HandleProtobufBody, this, 
                            boost::asio::placeholders::error));
            }
            else
            {

            }
        }

        void NodeSession::HandleProtobufBody( const boost::system::error_code& error)
        {
            bool alloc = _on_recv_msg_->FileAlloc();
            if( !error && alloc)
            {
                boost::asio::async_read(_socket_, boost::asio::buffer(_on_recv_msg_->GetFileBodyLoc(), 
                            _on_recv_msg_->GetFileBodyLen()),
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
                cout<<"Match:"<<_on_recv_msg_->GetFileBodyLen()<<endl;
                /*
                 * todo:error handle
                 * attention;
                 * */
                boost::asio::async_write(_socket_, 
                        boost::asio::buffer(_on_recv_msg_->write_buf_,_on_recv_msg_->write_len_),  
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

        void NodeSession::DoWrite(const boost::system::error_code& error)
        {
            int out_que_size = _node_write_buf_.size();
            while( _node_write_buf_.front()->send_or_not_)
            {
                delete _node_write_buf_.front();
                _node_write_buf_.pop_front();
            }
            if( out_que_size>=1)
            {
                async_write(_socket_,boost::asio::buffer(
                            _node_write_buf_.front()->GetOutLoc(),
                            _node_write_buf_.front()->GetOutLen()),
                        boost::bind(&NodeSession::DoWrite, this,
                            boost::asio::placeholders::error ));

                _node_write_buf_.front()->send_or_not_ = true;
            }
        }

        void NodeSession::TrySendMsg()
        {
            boost::mutex::scoped_lock locker(_mtx_);
            int out_que_size = _node_write_buf_.size();
            if(0 < out_que_size){
                async_write(_socket_,boost::asio::buffer(
                            _node_write_buf_.front()->GetOutLoc(),
                            _node_write_buf_.front()->GetOutLen()),
                        boost::bind(&NodeSession::DoWrite, this,
                            boost::asio::placeholders::error ));
                _node_write_buf_.front()->send_or_not_ = true;
            }
        }

        void NodeSession::addOutMsg(NodeMessage* out_msg)
        {
            boost::mutex::scoped_lock locker(_mtx_);
            _node_write_buf_.push_back(out_msg);
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
