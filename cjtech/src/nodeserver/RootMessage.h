/*************************************************************************
  > File Name: RootMessage.h
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
    namespace NodeServer
    {
        class RootMessage
        {
            public:
                RootMessage();
                virtual ~RootMessage();
                leopardIO::ProInnerMessage inner_msg;
                inline char* GetInnerMsgLoc();
                inline size_t GetInnerMsgLen();
                inline char* GetFileBodyLoc();
                inline size_t GetFileBodyLen();
                inline size_t* GetInnerMsgHeaderLoc();
                inline size_t GetInnerMsgHeaderLen();
                void ClearFileBody();
                void SetFileBody(const char* msg, size_t len);
                void SetWriteBuf();
                bool InnerMsgAlloc();
                bool FileAlloc();
            public:
                char* write_buf_;
                size_t write_len_;
            private:
                enum{ _proto_len_ = 4};
                char* _inner_msg_c_;
                char* _file_body_c_;
                std::size_t _file_body_len_;
                std::size_t _inner_msg_header_len_;
        };
    }
}

#endif
