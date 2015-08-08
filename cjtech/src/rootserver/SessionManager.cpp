/*************************************************************************
	> File Name: SessionManager.cpp
	> Author: zhonghx
	> Mail: zhonghongxia@foxmail.com
	> Created Time: Sat 08 Aug 2015 01:51:44 AM UTC
 ************************************************************************/

#include<iostream>
#include "SessionManager.h"
using namespace std;
SessionManager::SessionManager(SessionFactory &s):pos_(0), sessionfactory_(s)
{
    
}
SessionManager::~SessionManager()
{

}
void SessionManager::AddNodeAddr(string ip,string  port)
{
    ip_port_.push_back(make_pair(ip,port));
}
void SessionManager::GetNodeAddr(string &ip, string &port )
{
   ip=ip_port_[pos_].first;    
   port=ip_port_[pos_].second;    
   pos_++;
}

