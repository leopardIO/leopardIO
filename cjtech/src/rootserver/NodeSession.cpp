/*************************************************************************
	> File Name: NodeSession.cpp
	> Author: zhonghx
	> Mail: zhonghongxia@foxmail.com
	> Created Time: Sat 01 Aug 2015 07:36:54 AM UTC
 ************************************************************************/

#include<iostream>
#include<malloc.h>
#include<string.h>
#include "NodeSession.h"
using namespace std;

NodeSession::NodeSession(boost::asio::io_service& io_service,
                        char *str, int len,
                        char *ip , char *port)
    :   session_id_(-1),
        io_service_(io_service),
        socket_(io_service)//len 不含有末尾的\0

{
    data_ = new char[len+1]; 
    memset(data_ , 0 ,len +1);
    memcpy(data_ , str , len);
    data_[strlen(data_)]= '\0';
    memcpy(ip_ , ip , strlen(ip));
    ip_[strlen(ip)] = '\0';
    memcpy(port_ , port , strlen(port));
    port_[strlen(port)] = '\0';
} 
NodeSession::~NodeSession()
{
    if(data_ != NULL )
        delete []data_;
    socket_.close();
}

void NodeSession::Start()
{

    tcp::resolver resolver(io_service_);
    tcp::resolver::query query(ip_, port_); //输入ip（或域名）和端口号
    tcp::resolver::iterator iterator = resolver.resolve(query); 
    boost::asio::async_connect(socket_, iterator,
            boost::bind(&NodeSession::SendRequstToNode, this,
            boost::asio::placeholders::error)); //所有的操作都采用异步的方式

}
void NodeSession:: SendRequstToNode(const boost::system::error_code& error)
{ 
    if(!error)
    {
        boost::asio::async_write(socket_, boost::asio::buffer(data_,sizeof(data_)+1),
            boost::bind(&NodeSession::ReadResultHeader, this, 
            boost::asio::placeholders::error));
    }
    else
    {
        //调用sessionmanager销毁这个对象        
    }
}
void NodeSession::ReadResultHeader(const boost::system::error_code& error)
{
    if (!error)
    {
      boost::asio::async_read(socket_,
          boost::asio::buffer(inner_message_.header_buf(), InnerMessage::header_length), //读取数据报头
          boost::bind(&NodeSession::ReadResultProto, this,
            boost::asio::placeholders::error));
    }    
    
    else
    {
    
    }

}
void NodeSession::ReadResultProto(const boost::system::error_code& error)
{
    if (!error)
    {
        int length = inner_message_.pro_header_length();
        inner_message_.NewProHeader();
        boost::asio::async_read(socket_,
          boost::asio::buffer(inner_message_.pro_header(),length), //读取数据报头
          boost::bind(&NodeSession::ReadResultContent, this,
            boost::asio::placeholders::error));
        inner_message_.ParseProto();
        inner_message_.NewContent();

    }    
    else
    {
    
    }
}
void NodeSession::ReadResultContent(const boost::system::error_code& error)
{
    if (!error)
    {
        int length = inner_message_.content_length();

        boost::asio::async_read(socket_,
          boost::asio::buffer(inner_message_.content(),length), //读取数据报头
          boost::bind(&NodeSession::SendtoClient, this,
            boost::asio::placeholders::error));
    }    
    else
    {
    }    

}
void NodeSession::SendtoClient(const boost::system::error_code& error)
{

    if(!error)
    {
    //查找到自己的sessionid,然后将任务丢给session回写
    
    }
    else
    {}
}
