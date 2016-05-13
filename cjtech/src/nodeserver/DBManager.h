/*************************************************************************
	> File Name: HeadStructMessage.h
	> Author: zhonghx
	> Mail: zhonghongxia@foxmail.com
	> Created Time: Sun 29 Aug 2015 11:43:37 PM CST
 ************************************************************************/

#ifndef _DBManager_H
#define _DBManager_H
#include <iostream>
#include <string.h>
#include <string>
#include <mysql/mysql.h>
#include <stdio.h>
#include <map>
#include <vector>

using std::cout;
using std::endl;
using std::map;
using std::multimap;
using std::string;
using std::vector;


namespace NodeServer
{

    struct Picture
    {
        int picture_id_;
        string pic_name_;
    };

    struct MJproduct
    {
        int picture_id_;
        string mjproduct_id_;
    };

    struct ltstr  
    {  
        bool operator()(const char* s1, const char* s2) const  
        {  
            return strcmp(s1, s2) < 0;  
        }  
    };  

    class DBManager
    {
        public :
            string GetMJID(vector<int> pic_id);
            vector<int> GetPicID(char* match_name ,size_t length);
            const char* Query(const char * name);
            void init(	const char * sql_host_name = "localhost",
                    const char * sql_user_name = "root",
                    const char * sql_passwd = "root" ,
                    const char * db_name = "newmj");

        private:
            map<const char * ,const char * ,ltstr> _picture_map_;		
            map<const char * ,const char * ,ltstr>::iterator _iter_;
            multimap<string , struct Picture> _pic_struct_map_;
            map<int , struct MJproduct> _mj_struct_map_;
    };
}
#endif
