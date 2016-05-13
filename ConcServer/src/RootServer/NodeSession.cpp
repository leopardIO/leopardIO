#include "NodeSession.h"
#include <iostream>
#include<stdlib.h>
#include <glog/logging.h>
#include <glog/raw_logging.h>
#include "../../common/includeopencv/interface.h"
#include "NodeMessage.h"
#include "SessionService.h"
#include <openssl/md5.h>

using namespace std;

namespace cjtech
{
    namespace RootServer
    {
        NodeSession::NodeSession(boost::asio::io_service& io_service,
                tcp::resolver::iterator endpoint_iterator):
            _socket_(io_service)
        {
            boost::asio::async_connect(_socket_, endpoint_iterator,
                    boost::bind(&NodeSession::ConnectAfter, this,
                        boost::asio::placeholders::error));
            LOG(INFO)<<"bind connecct after"<<endl;
        }

        NodeSession::~NodeSession()
        {
            LOG(INFO)<<"+++++++++++++++++++++node session delete"<<endl;
        }

        tcp::socket& NodeSession::GetSocket()
        {
            return _socket_;
        }

        void NodeSession::ConnectAfter(const boost::system::error_code& error)
        {
			LOG(INFO)<<"NodeSession::ConnectAfter ..."<<endl;
            if(!error){
                LOG(INFO)<<"NodeSession begin read header ... ";
                _on_recv_msg_ = new NodeMessage();
                boost::asio::async_read(_socket_,
                        boost::asio::buffer(_on_recv_msg_->GetInnerMsgHeaderLoc(),
                            _on_recv_msg_->GetInnerMsgHeaderLen()),
                        boost::bind(&NodeSession::HandleProtobufHeaderLen, this,
                            boost::asio::placeholders::error));
            }else
            {
                LOG(ERROR)<<"error happen while connect to ns"<<boost::system::system_error(error).what() << endl;
            }
        }

        void NodeSession::HandleProtobufHeaderLen(const boost::system::error_code& error)
        {
            //cout<<"pb head len : "<< *(_on_recv_msg_->GetInnerMsgHeaderLoc())<<endl;
            LOG(INFO)<<"NodeSession::header shows the length of pb : "<<(_on_recv_msg_->GetInnerMsgLen())<<endl;
            bool alloc = _on_recv_msg_->InnerMsgAlloc();
            if(!error && alloc)
            {
				LOG(INFO)<<"rootsession::protobuf head recved"<<endl;
                boost::asio::async_read(_socket_, boost::asio::buffer(_on_recv_msg_->GetInnerMsgLoc(), 
                            _on_recv_msg_->GetInnerMsgLen()),
                        boost::bind(&NodeSession::HandleProtobufBody, this, 
                            boost::asio::placeholders::error));
            }
            else
            {
                LOG(ERROR)<<"error happen while read head from ns"<<endl;
                std::cout << boost::system::system_error(error).what() << std::endl;
            }
        }

        void NodeSession::HandleProtobufBody( const boost::system::error_code& error)
        {
            bool alloc = _on_recv_msg_->FileAlloc();
            if( !error && alloc)
            {
                LOG(INFO)<<"NodeSession::protobuf received ";
                LOG(INFO)<<"NodeSession::Begin read file ... ";
                boost::asio::async_read(_socket_, boost::asio::buffer(_on_recv_msg_->GetFileBodyLoc(), 
                            _on_recv_msg_->GetFileBodyLen()),
                        boost::bind(&NodeSession::SendbackResult, this, 
                            boost::asio::placeholders::error));
            }
            else
            {
                RAW_LOG(ERROR,"NodeSession::HandleProtobufBody find error");
                std::cout << boost::system::system_error(error).what() << std::endl;
            }
        }

        void NodeSession::HandleFileBody( const boost::system::error_code& error)
        {
            if(!error)
            {
                /*
                   cout<<"Match resved"<<_on_recv_msg_->GetFileBodyLen()<<endl;
                 * todo:error handle
                 * attention;
                 * 
                 boost::asio::async_write(_socket_, 
                 boost::asio::buffer(_on_recv_msg_->write_buf_,_on_recv_msg_->write_len_),  
                 boost::bind(&NodeSession::SendbackResult, this, 
                 boost::asio::placeholders::error));
                 */
            }
            else
            {
                RAW_LOG(ERROR,"NodeSession::HandleFileBody find error");
            }
        }

        void NodeSession::SendbackResult( const boost::system::error_code& error)
        {
            if(!error)
            {
                LOG(INFO)<<"NodeSession::Received result from Node"<<endl;
                LOG(INFO)<<"NodeSession::Begin send result back to client..."<<endl;
                SessionService* g_session_service = SessionService::getInstance(); 
                g_session_service->NodeHandler(this, _on_recv_msg_);
                _on_recv_msg_ = new NodeMessage();
                LOG(INFO)<<"NodeSession::Begin read header from Node again..."<<endl;
                boost::asio::async_read(_socket_,
                        boost::asio::buffer(_on_recv_msg_->GetInnerMsgHeaderLoc(),
                            _on_recv_msg_->GetInnerMsgHeaderLen()),
                        boost::bind(&NodeSession::HandleProtobufHeaderLen, this,
                            boost::asio::placeholders::error));
            }
            else
            {
                RAW_LOG(ERROR,"NodeSession::SendbackResult find error");
                std::cout << boost::system::system_error(error).what() << std::endl;
            }
        }

        void NodeSession::Write2ClientOK(const boost::system::error_code& error)
        {
            RAW_LOG(INFO,"Write to Node OK");
            if(!error){
                delete _node_write_buf_.front();
                _node_write_buf_.pop();
                if(_node_write_buf_.size()!=0)
                {
                NodeMessage* msg = _node_write_buf_.front();
                    async_write(_socket_,boost::asio::buffer(
                                _node_write_buf_.front()->GetOutLoc(),
                                _node_write_buf_.front()->GetOutLen()),
                            boost::bind(&NodeSession::Write2ClientOK, this,
                                boost::asio::placeholders::error ));
                    
                }

                else{
					_mtx_.unlock();
                }

            }
            else
            {
                RAW_LOG(ERROR,"SendbackResult 2 client error");
                std::cout << boost::system::system_error(error).what() << std::endl;
            }
        }

        void NodeSession::TrySendMsg(NodeMessage* msg)
        {
            if(msg == NULL)
                return;
			_mtx_.lock();
            RAW_LOG(INFO,"NodeSession try send request to Node Now " );
            _node_write_buf_.push(msg);
            NodeMessage* msg_tmp = _node_write_buf_.front();
            LOG(INFO) <<"================================ length to Node "<<msg_tmp->GetOutLen()<<endl;
		//	_mtx_.unlock();
            async_write(_socket_,boost::asio::buffer(
                        msg_tmp->GetOutLoc(),
                        msg_tmp->GetOutLen()),
                    boost::bind(&NodeSession::Write2ClientOK, this,
                        boost::asio::placeholders::error ));
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
