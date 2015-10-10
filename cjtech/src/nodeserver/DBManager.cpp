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
        string str = "select name, url, id, vedio_id, keepword1 from picture;";
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

            struct Picture pic_struct;
            pic_struct.picture_id_ = atoi(row[2]);
            pic_struct.vedio_id_ = atoi(row[3]);
            pic_struct.web_url_ = row[4];
            _pic_struct_map_[pic_struct.picture_id_] = pic_struct;
            row = mysql_fetch_row(result);
        }
        mysql_close(&mysql);	
        cout<<"DB get success !"<<endl;//最后要改成log


        MYSQL mysql_two;
        MYSQL_RES *result_two = NULL;
        mysql_init(&mysql_two);
        if(NULL == mysql_real_connect(&mysql_two, sql_host_name, sql_user_name, 
                    sql_passwd,db_name, 3306, NULL, 0))
        {
            fprintf(stderr, "error: %s",mysql_error(&mysql_two)); 
            cout<<"127.0.0.1 connect error!"<<endl; //#最后要改成是log格式
        }
        string str_two = "select id, picture_id from mjproduct;";
        mysql_query(&mysql_two, str.c_str());
        result_two = mysql_store_result(&mysql_two);
        MYSQL_ROW row2 = NULL;
        row2 = mysql_fetch_row(result_two);
        while(NULL != row2)
        {
            int pic_id = atoi(row[1]);
            struct MJproduct mj;
            mj.picture_id_ = pic_id;
            mj.mjproduct_id_ = row[0];
            _mj_struct_map_[pic_id] = mj;
        }
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
