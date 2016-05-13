/*************************************************************************
	> File Name: HeadStructMessage.h
	> Author: zhonghx
	> Mail: zhonghongxia@foxmail.com
	> Created Time: Mon 24 Aug 2015 07:07:12 PM UTC
 ************************************************************************/

#ifndef _DBManager_H
#define _DBManager_H
#include <iostream>
#include <string.h>
#include <string>
#include <mysql/mysql.h>
#include <stdio.h>
#include <map>

using std::cout;
using std::endl;
using std::map;
using std::string;

struct ltstr  //����Ƚϲ������ģ������С���򷵻ص���1
{  
	bool operator()(const char* s1, const char* s2) const  
	{  
		return strcmp(s1, s2) < 0;  
	}  
};  

class DBManager
{
	public :
		const char* query(const char * name);
		void init(	const char * sql_host_name = "localhost",
					const char * sql_user_name = "root",
					const char * sql_passwd = "root" ,
					const char * db_name = "picdb");
					
	private:
		map<const char * ,const char * ,ltstr> picture_map_;		
		map<const char * ,const char * ,ltstr>::iterator iter_;
};
#endif
