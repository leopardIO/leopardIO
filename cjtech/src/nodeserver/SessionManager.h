#ifndef _SessionManager_H
#define _SessionManager_H


#include <map>
#include <list>
#include<string>
#include <boost/thread/mutex.hpp>
#include <boost/asio.hpp>
#include "Singleton.h"
using namespace std;

class Session;
class SessionManager: public Singleton<SessionManager>
{
   friend class Singleton<SessionManager>;
public:
    //Add a session to SessionManager.
    int Add( Session* const );
    //Get a session base on id.
    Session* Get( int ) const;
    //Find is there have a session's ID is id.
    bool Find( int ) const;
    //SessionManager will not manager the session.
    bool Remove( int );
    //Add a session to RecycleAll list.
    void RecycleAll( int );
    //destructor session in RecycleAll list.
    void RecycleAll( void );
	void ClearAllSession();
//  template<typename T>    T* CreateSession(); 
//  template<typename T>    T* CreateSession(boost::asio::io_service& io_service);    
    void RecycleAllSession( Session* );
private:
    SessionManager();
    ~SessionManager();
private:
    typedef std::map<int, Session*> SessionMap;
    typedef std::list<Session*> SessionList;

    int GenerateID( void );

    int _sessionID_;
    SessionMap _map_;
    SessionList _recycle_all_list_;
	boost::mutex _mutex_;
};





#endif

