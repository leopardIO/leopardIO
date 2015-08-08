/*************************************************************************
	> File Name: SessionManager.h
	> Author: zhonghx
	> Mail: zhonghongxia@foxmail.com
	> Created Time: Sat 08 Aug 2015 01:51:27 AM UTC
 ************************************************************************/

#ifndef _SESSIONMANAGER_H
#define _SESSIONMANAGER_H
#include <map>
#include <vector>
#include <string>
#include <utility>  
#include "SessionFactory.h"
using std::map;
using std::vector;
using std::string;
using std::pair;
class SessionManager
{
public:
    SessionManager( SessionFactory &s);
    ~SessionManager();
    void AddNodeAddr(string ip ,string port);
    void GetNodeAddr(string &ip , string & port);
private:
    vector<pair<string,string> >  ip_port_;
    int pos_;
    SessionFactory &sessionfactory_;

};
#endif
