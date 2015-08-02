/*************************************************************************
	> File Name: InnerMessage.h
	> Author: zhonghx
	> Mail: zhonghongxia@foxmail.com
	> Created Time: Sun 02 Aug 2015 02:08:45 AM UTC
 ************************************************************************/

#ifndef _INNERMESSAGE_H
#define _INNERMESSAGE_H
#include <string.h>
#include <stdlib.h>
#include "ProInnerMessage.pb.h"
class InnerMessage
{
public:
    InnerMessage();
    virtual ~InnerMessage();
    int session_id()
    {
        return session_id_;
    }
    int message_type()
    {
        return message_type_;
    }
    int content_length()
    {
        return content_length_;
    }
    char * pro_header()
    {
        return pro_header_;
    }
    char *content()
    {
        return content_;
    }
    char * header_buf()
    {
        return header_buf_;
    }
    int pro_header_length()
    {
        return atoi(header_buf_);
    }
    void set_session_id( int id)
    {
        session_id_ = id;
    }
    void set_message_type(int type)
    {
        message_type_ = type;
    } 
    void set_content_type(int type)
    {
        content_type_ = type;
    }
    void set_content_length(int len)
    {
        content_length_ = len;
    }
    int NewProHeader();
    int NewContent();
    void ParseProto();
    enum{header_length= 8};

private:
    char header_buf_[header_length];
    int session_id_; 
    int message_type_;
    int content_type_;
    int content_length_;
    char *pro_header_;
    char *content_;
    float start_time_;
    float end_time_;
};
#endif
