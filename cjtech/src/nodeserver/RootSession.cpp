#include "RootSession.h"

using std::cout;
using std::cin;
using std::endl;
using boost::asio::ip::tcp;


namespace RootSession{

	RootSession::~RootSession()
	{
		if(_p_socket_temp_ != NULL)
			delete _p_socket_temp_;
	}
	RootSession::RootSession(short port)
		: acceptor_(g_io_service_pool::GetInstance()->GetIoService(), tcp::endpoint(tcp::v4(), port))		
	{
			
	}
	void RootSession::Run()
	{
		_p_socket_temp_ = new tcp::socket(g_io_service_pool::GetInstance()->GetIoService());
		_acceptor_.async_accept(*_p_socket_temp_,
				boost::bind(&RootSession::H_Read_Header, this,  
					boost::asio::placeholders::error));				
	}
	void RootSession::H_Read_Header(const boost::system::error_code& error)
	{
		if (!error)
		{
		//async_read 保证将数据读取完毕后才会调用回调函数
			boost::asio::async_read(*_p_socket_temp_,
				boost::asio::buffer(&_header_,sizeof(Header)),
				boost::bind(&RootSession::H_New_Session, this,
				boost::asio::placeholders::error));
		}
		else
		{
#ifdef DEBUG
			cout<<"error in rootserver"<<endl;
#endif
			g_session_manager->GetInstance()::Recycle(GetSessionID());
		}

	}
	void RootSession::H_New_Session(const boost::system::error_code& error)
	{
		if (!error)
		{
		//根据任务类型创建新的session，然后再继续监听接下来的连接的请求
			switch(_header_.proto_type)
			{
				case SEARCH_REQUEST://到来的是查询请求
					g_session_manager->GetInstance()::CreatSession<SearchSession>(*_p_socket_temp_ , _header_);
				break;				
				default :
				break;				
			}
			delete _p_socket_temp_;
			_p_socket_temp_ = new tcp::socket(g_io_service_pool::GetInstance()->GetIoService());
			_acceptor_.async_accept( *_p_socket_temp_,
				boost::bind(&RootSession::H_Read_Header, this,  
					boost::asio::placeholders::error));	
		}
		else
		{
#ifdef DEBUG
			cout<<"error in rootserver"<<endl;
#endif
			g_session_manager->GetInstance()::Recycle(GetSessionID());
		}

	}

}

