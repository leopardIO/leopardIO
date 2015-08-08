/*************************************************************************
  > File Name: RootSession.cpp
  > Author: cooperz
  > Mail: zbzcsn@qq.com
  > Created Time: Sat 08 Aug 2015 01:39:38 AM UTC
 ************************************************************************/
#include "RootSession.h"
#include<iostream>
#include "../../common/includeopencv/interface.h"
#include "RootMessage.h"
#include "DBToMap.h"
#include "NodeUtil.h"

using namespace std;

namespace cjtech
{
    namespace NodeServer
    {
        RootSession::RootSession(boost::asio::io_service& io_service):
            _socket_(io_service)
        {

        }

        RootSession::~RootSession()
        {

        }

        tcp::socket& RootSession::GetSocket()
        {
            return _socket_;
        }

        void RootSession::Start()
        {
#ifdef DEBUG
            cout<<"rootsession::start"<<endl;
#endif
            _msg_ = new RootMessage();
            boost::asio::async_read(_socket_,
                    boost::asio::buffer(_msg_->GetInnerMsgHeaderLoc(),
                        _msg_->GetInnerMsgHeaderLen()),
                    boost::bind(&RootSession::HandleProtobufHeaderLen, this,
                        boost::asio::placeholders::error));
        }

        void RootSession::HandleProtobufHeaderLen(const boost::system::error_code& error)
        {
            bool alloc = _msg_->InnerMsgAlloc();
            if(!error && alloc)
            {
#ifdef DEBUG
                cout<<"rootsession::protobuf head recved"<<endl;
#endif
                boost::asio::async_read(_socket_, boost::asio::buffer(_msg_->GetInnerMsgLoc(), 
                            _msg_->GetInnerMsgLen()),
                        boost::bind(&RootSession::HandleProtobufBody, this, 
                            boost::asio::placeholders::error));
            }
            else
            {

            }
        }

        void RootSession::HandleProtobufBody( const boost::system::error_code& error)
        {
            bool alloc = _msg_->FileAlloc();
            if( !error && alloc)
            {
                boost::asio::async_read(_socket_, boost::asio::buffer(_msg_->GetFileBodyLoc(), 
                            _msg_->GetFileBodyLen()),
                        boost::bind(&RootSession::HandleFileBody, this, 
                            boost::asio::placeholders::error));
            }
            else
            {

            }
        }

        void RootSession::HandleFileBody( const boost::system::error_code& error)
        {
            if(!error)
            {
                cout<<"Match:"<<endl;
                /*
                 * todo:error handle
                 * attention;
                 * */
                string filepath = 
                    _WriteToFile(_msg_->GetFileBodyLoc(), _msg_->GetFileBodyLen());
                cout<<"Write to file path:"<<filepath<<endl;
                extern Matcher* g_pic_matcher;
                string result = g_pic_matcher->match( filepath.c_str(), FEATUREPATH, TRANDIR);
                cout<<"Match result:"<<result<<endl;
                string filename;
                size_t filename_end_pos = result.find("&&");
                if(filename_end_pos != string::npos)
                {
                    filename = result.substr(0, filename_end_pos-1);
                }
                cout<<"Query fileName:"<<filename<<endl;
                extern DBToMap* g_db_cache;
                string response = g_db_cache->query(filename.c_str());
                cout<<"Response:"<<response<<endl;
                _msg_->ClearFileBody();
                _msg_->SetFileBody(response.c_str(), response.length());
                _msg_->SetWriteBuf();
            }
            else
            {

            }
        }

        void RootSession::SendbackResult( const boost::system::error_code& error)
        {
            if(!error)
            {
                boost::asio::async_write(_socket_, 
                        boost::asio::buffer(_msg_->write_buf_,_msg_->write_len_),  
                        boost::bind(NULL, this, 
                            boost::asio::placeholders::error));
            }
            else
            {
                
            }
        }

        string RootSession::_RandomNum()
        {
            int randnum = 0;
            stringstream rand_num_sstr;
            randnum = rand();
            rand_num_sstr<<randnum;
            return rand_num_sstr.str();
        }

        string RootSession::_WriteToFile(char* msg, int len)
        {
            string rand_str = _RandomNum();
            string filepath = string(TMP_PATH) + rand_str;
            int fd = open( filepath.c_str() , O_RDWR);
            int wnum = write(fd , msg, len);
            if(wnum <=0)
            {
                cout<<"write error"<<endl;
                return "";
            }
            return filepath;
        }
    }
}
