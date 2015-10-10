#include "SearchSession.h"
#include<iostream>
#include "NodeUtil.h"
#include "SessionManager.h"

using namespace std;

extern NodeServer::SessionManager *g_session_manager;
extern NodeServer::IOServicePool* g_io_service_pool;
extern NodeServer::SessionManager* g_session_manager; 
namespace NodeServer
{
	SearchSession::SearchSession(tcp::socket *temp_socket, struct HeadStructMessage header )
		: _socket_(g_io_service_pool->GetIoService()) 
	{
        _header_ = header;
        int socketDup = dup(temp_socket->native());
        _socket_.assign(boost::asio::ip::tcp::v4(), socketDup);
	}
	SearchSession::~SearchSession()
	{

	}

	void SearchSession::Start()
	{
#ifdef DEBUG
		cout<<"SearchSession::start pb len = "<<_header_.length<<endl;
#endif
		_proto_buf_ptr_ = new char[_header_.length];
		memset(_proto_buf_ptr_ , 0 ,_header_.length);
		boost::asio::async_read(_socket_,
				boost::asio::buffer(_proto_buf_ptr_,(_header_.length)),
				boost::bind(&SearchSession::H_ReadContent, this,
					boost::asio::placeholders::error));		
	}

	void SearchSession::H_ReadContent(const boost::system::error_code& error)
	{
		if( !error)
		{
#ifdef DEBUG
		    cout<<"SearchSession::pb recved : "<< _header_.length <<endl;
#endif
			SearchProtoMessage search_proto_message;
			if(!search_proto_message.ParseFromArray(_proto_buf_ptr_,_header_.length))
			{
				//解析出错了
#ifdef DEBUG
		        cout<<"SearchSession::parse error"<<endl;
#endif
				//g_session_manager->Recycle(GetSessionID());
                return ;
			}				
			//获取到picture的length				
			uint32_t picture_length = search_proto_message.picture_length();
#ifdef DEBUG
		    cout<<"SearchSession::pic len : "<< picture_length <<endl;
#endif
			//获取到picture的名字
			_file_name_ = search_proto_message.picture_name();
			//读取picture的内容
            _content_buf_ptr_ = new char[search_proto_message.picture_length()];
            uint32_t task_id = search_proto_message.task_id();
			boost::asio::async_read(_socket_,
				boost::asio::buffer(_content_buf_ptr_,picture_length),
				boost::bind(&SearchSession::H_MatchPicture, this,
					boost::asio::placeholders::error, task_id, picture_length));	
			
		}
		else
		{
#ifdef DEBUG
            std::cout <<boost::system::system_error(error).what() << std::endl;
#endif
		}
	}

	void SearchSession::H_MatchPicture( const boost::system::error_code& error,
            uint32_t task_id, uint32_t pic_len)
	{
		if(!error)
		{
			cout<<"Match:"<<endl;
			/*
			 * todo:error handle
			 * attention;
			 * */
			string filepath = 
				_WriteToFile_(_content_buf_ptr_, pic_len);
			cout<<"Write to file path:"<<filepath<<endl;
			string result = g_pic_matcher->match( filepath.c_str(), FEATUREPATH, TRANDIR);
			cout<<"Match result:"<<result<<endl;
            int index = result.find("&&", 0);
            string match_name = result.substr( 0, index - 1);
            int index_pit = result.find(".",0);
            string only_name = result.substr( 0, index_pit - 1);
            string tracker = "/mj/tracker/" + only_name + ".zip";
            char* name_c = (char*)match_name.c_str();
            int match_pic_id = g_db_manager->GetPicID(name_c);
            string mj_id = g_db_manager->GetMJID(match_pic_id);
			string response = g_db_manager->Query(_file_name_.c_str());
			SearchResultMessage  search_result;
            search_result.set_picture_name(_file_name_);
            search_result.set_result_length(response.length());
            search_result.set_task_id(task_id);
            search_result.set_mj_id(mj_id);
            search_result.set_trackerurl(tracker);
            search_result.set_status(1);
            string proto_str_tmp ;
            search_result.SerializeToString(&proto_str_tmp);
            string response_msg =
               Session::FormMessage(proto_str_tmp.length(),SEARCH_REQUEST_RESULT_FROM_NODE,proto_str_tmp,response); 
			boost::asio::async_write(_socket_,
				boost::asio::buffer(response_msg,response_msg.length()),
				boost::bind(&SearchSession::H_AfterSendResult, this,
					boost::asio::placeholders::error));	
			
			

		}
		else
		{

		}
	}

	void SearchSession::H_AfterSendResult( const boost::system::error_code& error)
	{
		if(!error)
		{
			cout<<"Send back OK!"<<endl;
            Start();
		}
		else
		{
			
		}
	}

	string SearchSession::_RandomNum_()
	{
		int randnum = 0;
		stringstream rand_num_sstr;
		randnum = rand();
		rand_num_sstr<<randnum;
		return rand_num_sstr.str();
	}

	string SearchSession::_WriteToFile_(char* msg, int len)
	{
		string rand_str = _RandomNum_();
		string filepath = string(TMP_PATH) + rand_str;
		cout<<"file path:"<<filepath<<endl;
		int fd = open( filepath.c_str() , O_CREAT | O_RDWR , 0666);
		if(fd<=0)
			perror("open error\n");
		int wnum = ::write(fd , msg, len);
		if(wnum <=0)
		{
			//cout<<"write error"<<endl;
			perror("write error\n");
			return "";
		}
        close(fd);
		return filepath;
	}
}

