/*************************************************************************
  > File Name: DBManager.h
  > Author: cooperz
  > Mail: zbzcsn@qq.com
  > Created Time: Sat 08 Aug 2015 09:17:25 AM UTC
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


namespace NodeServer{

    struct Picture
    {
        int picture_id_;
        int vedio_id_;
        string web_url_;
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
            const char* Query(const char * name);
            void init(	const char * sql_host_name = "localhost",
                    const char * sql_user_name = "root",
                    const char * sql_passwd = "1314159" ,
                    const char * db_name = "picdb");

        private:
            map<const char * ,const char * ,ltstr> _picture_map_;		
            map<const char * ,const char * ,ltstr>::iterator _iter_;
            map<int , struct Picture> _pic_struct_map_;
            map<int , struct MJproduct> _mj_struct_map_;
    };
}
#endif
