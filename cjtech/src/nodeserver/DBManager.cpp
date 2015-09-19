#include "DBManager.h"
namespace NodeServer
{

    void DBManager::init(const char * sql_host_name ,const char * sql_user_name ,
                const char * sql_passwd ,const char * db_name)
    {
        MYSQL mysql;
        MYSQL_RES *result = NULL;
        mysql_init(&mysql);
        if(NULL == mysql_real_connect(&mysql, sql_host_name, sql_user_name, 
                    sql_passwd,db_name, 3306, NULL, 0))
        {
            fprintf(stderr, "error: %s",mysql_error(&mysql)); 
            cout<<"127.0.0.1 connect error!"<<endl; //#最后要改成是log格式
        }
        string str = "select name, url from picture;";
        mysql_query(&mysql, str.c_str());
        result = mysql_store_result(&mysql);
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row(result);
        while(NULL != row)
        {
            if(row[1] == NULL)
                _picture_map_[row[0]] = "NOT FOUND";
            else
                cout<<"key : "<<row[0]<<" value : "<<row[1]<<"\" get "<<endl;// for test 
                _picture_map_[row[0]] = row[1];
            row = mysql_fetch_row(result);
        }
        mysql_close(&mysql);	
        cout<<"DB get success !"<<endl;//最后要改成log
    }

    const char * DBManager::Query(const char * name)
    {
        cout<<"Search in db cache :"<<name<<endl;
        _iter_ = _picture_map_.find(name);
        if(_iter_ != _picture_map_.end())
        {
           return _iter_->second;
        }
        else
        {
            //最后要改成log
           return "NOT FOUND IN MAP";
        }
    }
}
