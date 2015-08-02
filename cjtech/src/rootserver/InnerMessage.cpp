/*************************************************************************
	> File Name: InnerMessage.cpp
	> Author: zhonghx
	> Mail: zhonghongxia@foxmail.com
	> Created Time: Sun 02 Aug 2015 02:09:03 AM UTC
 ************************************************************************/

#include<iostream>
#include "InnerMessage.h"
#include <string.h>
#include<string>
using std ::cout;
using std ::cin;
using std ::endl;
using std ::string;
using leopardIO::ProInnerMessage;
InnerMessage::InnerMessage()
    :pro_header_(NULL),
    content_(NULL)

{
    memset(header_buf_ , 0 ,header_length);
}
int  InnerMessage::NewProHeader()
{
    pro_header_ = new char[pro_header_length() +1];
    if(pro_header_ != NULL)
    {
        memset(pro_header_ , 0 , pro_header_length() +1);
        return 0; 
    }
    else return -1;
}
int  InnerMessage::NewContent()

{
    content_ = new char[content_length_+1];
    if(content_ != NULL)
    {
        memset(content_ , 0 ,content_length_+1);
        return 0; 
    }
    else return -1;
}

InnerMessage::~InnerMessage()
{
    if(pro_header_ !=NULL)
       delete []pro_header_;
    if(content_ !=NULL)
       delete []content_;
}
void InnerMessage ::ParseProto()
{
    ProInnerMessage proinnermessage;
    string str(pro_header_);
    proinnermessage.ParseFromString(str);
    session_id_ = proinnermessage.session_id(); 
    message_type_=proinnermessage.message_type();
    content_type_ =proinnermessage.content_type();    
    content_length_=proinnermessage.content_length();
    start_time_=proinnermessage.start_time();
    end_time_=proinnermessage.end_time();
}
