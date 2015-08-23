#include "RootSession.h"

using std::cout;
using std::cin;
using std::endl;
using boost::asio::ip::tcp;




namespace cjtech{
    namespace RootSession{
        RootSession::RootSession(short port)
            : _acceptor_(g_io_service_pool->Get_io_service(), tcp::endpoint(tcp::v4(), port))
        {
			_acceptor_.async_accept(_socket_temp_,
                    boost::bind(&RootSession::handle_accept, this,  
                        boost::asio::placeholders::error));			
        }

        void RootSession::handle_accept(const boost::system::error_code& error)
        {
            if (!error)
            {
                boost::asio::async_read(_socket_temp_,
				boost::asio::buffer(_msg_->GetInnerMsgHeaderLoc(),
					_msg_->GetInnerMsgHeaderLen()),
				boost::bind(&SearchSession::HandleProtobufHeaderLen, this,
					boost::asio::placeholders::error));
            }
            else
            {
#ifdef DEBUG
                cout<<"error in rootserver"<<endl;
#endif
                delete new_session;
            }
#ifdef DEBUG
            cout<<"accept new task"<<endl;
#endif
            new_session = new RootSession(io_service_pool_.Get_io_service());
            _acceptor_.async_accept(new_session->GetSocket(),
                    boost::bind(&RootSession::handle_accept, this, new_session, 
                        boost::asio::placeholders::error));
        }

        void RootSession::run()
        {
            io_service_pool_.start();
            io_service_pool_.join();
        }
    }
}
