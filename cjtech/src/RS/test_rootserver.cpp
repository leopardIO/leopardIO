/*************************************************************************
  > File Name: test_rootserver.cpp
  > Author: cooperz
  > Mail: zbzcsn@qq.com
  > Created Time: Sat 01 Aug 2015 08:52:50 AM UTC
 ************************************************************************/

#include <sys/syscall.h>
#include <pthread.h>


#include "IOServerPool.h"
#include "RootServer.h"

#include<iostream>
using namespace std;
using namespace cjtech::RootServer;

int main(int argn, char** argv)
{
    //boost
    printf("child thread lwpid = %u\n", syscall(SYS_gettid));
    printf("child thread tid = %u\n", pthread_self()); 
    RootServer s(6002, 1);
    s.run();
    while(true)
    {
        sleep(1000);
    }
    return 0;
}
