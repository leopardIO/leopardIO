/*************************************************************************
  > File Name: NodeMessage.cpp
  > Author: cooperz
  > Mail: zbzcsn@qq.com
  > Created Time: Sat 08 Aug 2015 01:37:43 AM UTC
 ************************************************************************/

#include "NodeMessage.h"

#include<iostream>
#include<openssl/md5.h>
#include<glog/logging.h>
#include<glog/raw_logging.h>


using namespace std;

void md5cout(char* data, int len)
{
    MD5_CTX x;
    MD5_Init(&x);
    unsigned char d[16];
    MD5_Update (&x, (char *)data, len);
    MD5_Final(d, &x);
    char out[35];
    int i;
    for (i = 0; i < 16; i++)
    {
        sprintf (out + (i*2), "%02X", d[i]);
    }
    out[32] = 0;
    cout<<"the md5 is : "<< out <<endl;
}

namespace cjtech
{
    namespace RootServer
    {
        NodeMessage::NodeMessage()
        {
            send_or_not_ = false;
        }

        NodeMessage::~NodeMessage()
        {
            //~inner_msg();
        }

        char* NodeMessage::GetInnerMsgLoc()
        {
            return _pb_c_;
        }

        uint32_t NodeMessage::GetInnerMsgLen()
        {
            //cout<<" struct head len : "<< _inner_msg_.len << endl;
            //cout<<" struct head type : "<< _inner_msg_.type << endl;
            return _inner_msg_.len;
        }

        char* NodeMessage::GetFileBodyLoc()
        {
            return _file_body_c_;
        }

        size_t NodeMessage::GetFileBodyLen()
        {
            return _file_body_len_;
        }

        char* NodeMessage::GetInnerMsgHeaderLoc()
        {
            return (char*)&_inner_msg_;
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
            //cout<<" struct head len : "<< _inner_msg_.len << endl;
            //cout<<" struct head type : "<< _inner_msg_.type << endl;
            if(_inner_msg_.len>=1024*1024)
                return false;
            _pb_c_ = (char*)malloc(sizeof(char)*_inner_msg_.len);
            //_file_body_len_ = inner_msg.result_length();
            //return inner_msg.ParseFromArray( _inner_msg_c_, _inner_msg_header_len_);
            return true;
        }

        bool NodeMessage::ParserProtoBuf()
        {
            return inner_msg.ParseFromString( 
                    string(_pb_c_, _inner_msg_.len));
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
            //cout<<"write_len_"<<write_len_<<endl;
        }

        void NodeMessage::SetBufMsg2Node()
        {
            uint32_t cmd = 0xa0;
          //  size_t protofbuf_len = out_msg.ByteSize();
			string tmp ;
			out_msg.SerializeToString(&tmp);
			size_t protofbuf_len = tmp.length();

/*            LOG(INFO)<<"NodeMsg::shows Data from NS pblen::"<< protofbuf_len<<" picname::"<< out_msg.picture_name()\
                <<" piclien::"<< out_msg.picture_length()<<" taskid::"<< out_msg.task_id()\
                <<" stime::"<< out_msg.start_time() <<" etime::"<<out_msg.end_time()<<" retrain::"<<out_msg.retrain();
  */          _out_len_ = 2*sizeof(uint32_t) + write_len_ + protofbuf_len;
            uint32_t pb_len_ = protofbuf_len;
            LOG(INFO)<<"NodeMessage::the msg send to Node length:"<<_out_len_<<endl;
            _data_out_ = (char*)malloc(_out_len_);
            memset(_data_out_, 0, sizeof(char)*_out_len_);
            if(_data_out_ == NULL)
                LOG(FATAL)<<"NodeMessage ::malloc err data out is NULL";
            //cout<<"before _data_out_ ::"<<_data_out_<<endl;
            char* buf_ptr = _data_out_;
            memcpy( buf_ptr, &pb_len_, sizeof(uint32_t));
            buf_ptr +=sizeof(uint32_t);
            memcpy( buf_ptr, &cmd, sizeof(uint32_t));
            buf_ptr += sizeof(uint32_t);
			memcpy( buf_ptr, tmp.c_str(),tmp.length());
			buf_ptr += tmp.length(); 
			memcpy(buf_ptr , write_buf_ , write_len_);
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
