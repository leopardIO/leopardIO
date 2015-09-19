/*************************************************************************
  > File Name: NodeMessage.h
  > Author: cooperz
  > Mail: zbzcsn@qq.com
  > Created Time: Sat 08 Aug 2015 01:37:25 AM UTC
 ************************************************************************/

#ifndef _ROOTMESSAGE_H
#define _ROOTMESSAGE_H

#include "ProInnerMessage.pb.h"

/*************************************************************************
 *
 *_____________________________________________________________
 *          |              |
 *__________|______________|____________________________________
 *proto len   proto body        file body
 *
 ************************************************************************/

namespace cjtech
{
    namespace RootServer
    {
        class NodeMessage
        {
            public:
                NodeMessage();
                virtual ~NodeMessage();
                char* GetInnerMsgLoc();
                size_t GetInnerMsgLen();
                char* GetFileBodyLoc();
                size_t GetFileBodyLen();
                size_t* GetInnerMsgHeaderLoc();
                size_t GetInnerMsgHeaderLen();
                size_t GetOutLen();
                char* GetOutLoc();
                void ClearFileBody();
                void SetFileBody(const char* msg, size_t len);
                void SetWriteBuf();
                bool InnerMsgAlloc();
                bool ParserProtoBuf();
                bool FileAlloc();
            public:
                leopardIO::ProInnerMessage inner_msg;
                char* write_buf_;
                size_t write_len_;
                bool send_or_not_;
            private:
                char* _inner_msg_c_;
                char* _file_body_c_;
                std::size_t _file_body_len_;
                std::size_t _inner_msg_header_len_;
                enum{ _proto_len_ = sizeof(_inner_msg_header_len_)};
                std::size_t _out_len_;
                char* _data_out_;
        };
    }
}

#endif
