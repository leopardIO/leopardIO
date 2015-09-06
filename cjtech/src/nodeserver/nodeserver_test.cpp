/*************************************************************************
  > File Name: test_rootserver.cpp
  > Author: cooperz
  > Mail: zbzcsn@qq.com
  > Created Time: Sat 01 Aug 2015 08:52:50 AM UTC
 ************************************************************************/
//#include "IOServerPool.h"
//
#include<iostream>
#include "RootSession.h"
#include "DBManager.h"
#include "../../common/includeopencv/interface.h"
#include "NodeUtil.h"


using namespace std;
using namespace cjtech::RootSession;

int main(int argn, char** argv)
{
	Matcher* g_pic_matcher;
    DBManager* g_db_manager;
	IOServerPool* g_io_service_pool;
	SessionManager* g_session_manager; 

	g_pic_matcher = new Matcher();
    if((access (INDEXPATH,F_OK)) == 0)
    {
        g_pic_matcher->loadIndex(INDEXPATH);
    }
    else
    {
        g_pic_matcher->train(TRANDIR, FEATUREPATH, INDEXPATH);
    }
    g_db_manager = new DBManager();
    g_db_manager->init();
	
	g_io_service_pool = new IOServerPool(1);//传入的线程池的大小
	g_session_manager = new SessionManager();
    RootSession s(6002, 1);
    s.run();
    while(true)
    {
        sleep(1000);
    }
    return 0;
}
