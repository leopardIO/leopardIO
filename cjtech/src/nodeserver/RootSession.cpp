#include "RootSession.h"
#include "SearchSession.h"

using std::cout;
using std::cin;
using std::endl;
using boost::asio::ip::tcp;


extern NodeServer::IOServicePool *g_io_service_pool;
extern NodeServer::SessionManager* g_session_manager; 
namespace NodeServer
{
	RootSession::~RootSession()
	{
		if(_p_socket_temp_ != NULL)
			delete _p_socket_temp_;
	}
	RootSession::RootSession(short port)
		: _acceptor_(g_io_service_pool->GetIoService(), tcp::endpoint(tcp::v4(), port))		
	{
			
	}
	void RootSession::Run()
	{
        cout<<"root session run"<<endl;
		_p_socket_temp_ = new tcp::socket(g_io_service_pool->GetIoService());
		_acceptor_.async_accept(*_p_socket_temp_,
				boost::bind(&RootSession::H_Read_Header, this,  
					boost::asio::placeholders::error));				
	}
	void RootSession::H_Read_Header(const boost::system::error_code& error)
	{
		if (!error)
		{
		//async_read 保证将数据读取完毕后才会调用回调函数
            cout<<"sizeof head strutct size"<<sizeof(struct  HeadStructMessage)<<endl;
			boost::asio::async_read(*_p_socket_temp_,
				boost::asio::buffer(&_header_,sizeof(struct  HeadStructMessage)),
				boost::bind(&RootSession::H_New_Session, this,
				boost::asio::placeholders::error));
		}
		else
		{
#ifdef DEBUG
			cout<<"error in rootserver"<<endl;
#endif
			g_session_manager->Recycle(GetSessionID());
		}

	}
	void RootSession::H_New_Session(const boost::system::error_code& error)
	{
		if (!error)
		{
		//根据任务类型创建新的session，然后再继续监听接下来的连接的请求
			switch(_header_.type)
			{
				case SEARCH_REQUEST://到来的是查询请求
                    cout<<"search request recved"<<endl;
					g_session_manager->CreateSession<SearchSession>(_p_socket_temp_ , _header_);

				break;				
				default :
                    cout <<"header.type not recongnize :"<<_header_.type<<endl;
				break;				
			}
			delete _p_socket_temp_;
			_p_socket_temp_ = new tcp::socket(g_io_service_pool->GetIoService());
			_acceptor_.async_accept( *_p_socket_temp_,
				boost::bind(&RootSession::H_Read_Header, this,  
					boost::asio::placeholders::error));	
		}
		else
		{
#ifdef DEBUG
			cout<<"error in rootserver H_New_Session"<<endl;
#endif
			g_session_manager->Recycle(GetSessionID());
		}

	}

}

