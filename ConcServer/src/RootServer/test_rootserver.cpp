/*************************************************************************
  > File Name: test_rootserver.cpp
  > Author: cooperz
  > Mail: zbzcsn@qq.com
  > Created Time: Sat 01 Aug 2015 08:52:50 AM UTC
 ************************************************************************/

#include <sys/syscall.h>
#include <pthread.h>
#include <glog/logging.h>
#include <glog/raw_logging.h>

#include "IOServerPool.h"
#include "RootServer.h"
#include "SessionService.h"

#include<iostream>
using namespace std;
using namespace cjtech::RootServer;

SessionService* g_session_service;

int main(int argn, char** argv)
{
    //boost
	if(argn != 2)
	{
			return -1;
		}
	   google::LogToStderr();//只输出到标准错误输出
    FLAGS_colorlogtostderr=true;
    google::InitGoogleLogging(argv[0]);
    //printf("child thread lwpid = %u\n", syscall(SYS_gettid));
    //printf("child thread tid = %u\n", pthread_self()); 
        
     IOServerPool io_service_pool(2);
     io_service_pool.start();
    RootServer s(7004, io_service_pool);
    s.run();
    g_session_service = SessionService::getInstance(atoi(argv[1]));
    g_session_service->Run();
    /*
    while(true)
    {
        sleep(1000);
    }*/
    return 0;
}
