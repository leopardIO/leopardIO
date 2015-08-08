#ifndef _SessionFactory_H
#define _SessionFactory_H

#include <map>
#include <list>

#include <boost/thread/mutex.hpp>
#include <boost/asio.hpp>
#include "Singleton.h"

class Session;
class SessionFactory: public Singleton<SessionFactory>
{
   friend class Singleton<SessionFactory>;
public:
    //add a session to SessionFactory.
    int add( Session* const );
    //get a session base on id.
    Session* get( int ) const;
    //find is there have a session's ID is id.
    bool find( int ) const;
    //SessionFactory will not manager the session.
    bool remove( int );
    //add a session to recycle list.
    void recycle( int );
    //destructor session in recycle list.
    void recycleAll( void );
	void clearAllSession();
    template<typename T>    T* createSession();
    template<typename T>    T* createSession(boost::asio::io_service& io_service,                   
                                        char *str, int len,                                     
                                        char *ip , char *port);
    void recyclerSession( Session* );
private:
    SessionFactory();
    ~SessionFactory();

private:
    typedef std::map<int, Session*> SessionMap;
    typedef std::list<Session*> SessionList;

    int generateID( void );

    int sessionID_;
    SessionMap map_;
    SessionList recycleList_;
	boost::mutex mutex_;
};

template<typename T>
T* SessionFactory::createSession()
{
    T* session;
    session = new T();
    if ( session != NULL )
    {
        add( session );
    }

    return session;
}
template<typename T>    
T* createSession(boost::asio::io_service& io_service,                   
                char *str, int len,                                     
                char *ip , char *port)
{
    T *session;
    session = new T(io_service,str,len,ip,port);
    if ( session != NULL )
    {
        add( session );
    }

    return session;
    

}



#endif

