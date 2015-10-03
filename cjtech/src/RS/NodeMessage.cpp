/*************************************************************************
  > File Name: NodeMessage.cpp
  > Author: cooperz
  > Mail: zbzcsn@qq.com
  > Created Time: Sat 08 Aug 2015 01:37:43 AM UTC
 ************************************************************************/

#include "NodeMessage.h"

#include<iostream>

using namespace std;

namespace cjtech
{
    namespace RootServer
    {
        NodeMessage::NodeMessage()
        {

        }

        NodeMessage::~NodeMessage()
        {
            //~inner_msg();
        }

        char* NodeMessage::GetInnerMsgLoc()
        {
            return _inner_msg_c_;
        }

        size_t NodeMessage::GetInnerMsgLen()
        {
            return _inner_msg_header_len_;
        }

        char* NodeMessage::GetFileBodyLoc()
        {
            return _file_body_c_;
        }

        size_t NodeMessage::GetFileBodyLen()
        {
            return _file_body_len_;
        }

        size_t* NodeMessage::GetInnerMsgHeaderLoc()
        {
            return &_inner_msg_header_len_;
        }

        size_t NodeMessage::GetInnerMsgHeaderLen()
        {
            return _proto_len_;
        }

        size_t NodeMessage::GetOutLen()
        {
            return _out_len_;
        }

        char* NodeMessage::GetOutLoc()
        {
            return _data_out_;
        }

        bool NodeMessage::InnerMsgAlloc()
        {
            if(_inner_msg_header_len_>=1024*1024)
                return false;
            _inner_msg_c_ = (char*)malloc(sizeof(char)*_inner_msg_header_len_);
            _file_body_len_ = inner_msg.result_length();
            //return inner_msg.ParseFromArray( _inner_msg_c_, _inner_msg_header_len_);
            return true;
        }

        bool NodeMessage::ParserProtoBuf()
        {
            return inner_msg.ParseFromArray( _inner_msg_c_, _inner_msg_header_len_);
        }

        bool NodeMessage::FileAlloc()
        {
            if(!ParserProtoBuf())
                return false;
            _file_body_len_ = inner_msg.result_length();
            _file_body_c_ = (char*)malloc(sizeof(char)*_file_body_len_);
            return true;
        }

        void NodeMessage::ClearFileBody()
        {
            if(_file_body_c_ != NULL)
            {
                free(_file_body_c_);
                _file_body_c_ = NULL;
            }
            _file_body_len_ = 0;
        }

        void NodeMessage::SetFileBody(const char* msg , size_t len)
        {
            _file_body_c_ = (char*)malloc(len);
            memcpy(_file_body_c_ , msg , len);
            _file_body_len_ = len;
        }

        void NodeMessage::SetWriteBuf()
        {
            size_t protofbuf_len = inner_msg.ByteSize();
            write_len_ = _proto_len_ + _file_body_len_ + protofbuf_len;
            write_buf_ = (char*)malloc(write_len_);
            char* buf_ptr = write_buf_;
            memcpy( buf_ptr, &protofbuf_len, sizeof(protofbuf_len));
            buf_ptr = buf_ptr+sizeof(protofbuf_len);
            inner_msg.SerializeToArray( buf_ptr, protofbuf_len);
            buf_ptr = buf_ptr+protofbuf_len;
            memcpy( buf_ptr, _file_body_c_, _file_body_len_);
            cout<<"write_len_"<<write_len_<<endl;
        }

        void NodeMessage::SetBufMsg2Node()
        {
            size_t protofbuf_len = out_msg.ByteSize();
            _out_len_ = sizeof(size_t) + _file_body_len_ + protofbuf_len;
            _data_out_ = (char*)malloc(_out_len_);
            char* buf_ptr = write_buf_;
            memcpy( buf_ptr, &_out_len_, sizeof(_out_len_));
            buf_ptr = buf_ptr+sizeof(_out_len_);
            out_msg.SerializeToArray( buf_ptr, protofbuf_len);
            buf_ptr = buf_ptr+protofbuf_len; 
            memcpy( buf_ptr, write_buf_, write_len_);
        }

        void NodeMessage::SetPB2Node(std::string picture_name,std::string picture_length,
                long int start_time, long int end_time)
        {
        }

        void NodeMessage::SetTaskID(int id)
        {
            out_msg.set_task_id(id);
        }
    }
}
