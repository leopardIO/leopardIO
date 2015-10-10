/*************************************************************************
	> File Name: SearchSession.h
	> Author: cooperz
	> Mail: zbzcsn@qq.com
	> Created Time: Sat 08 Aug 2015 01:39:35 AM UTC
 ************************************************************************/

#ifndef _ROOTSESSION_H
#define _ROOTSESSION_H

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include "SearchResultMessage.pb.h"
#include "SearchProtoMessage.pb.h"
#include "HeadStructMessage.h"
#include "DBManager.h" 
#include "Session.h"
#include "IOServicePool.h"
#include "NodeUtil.h"
#include "../../common/includeopencv/interface.h"
#define DEBUG 1
using boost::asio::ip::tcp;  
extern	Matcher* g_pic_matcher;
extern  NodeServer::DBManager* g_db_manager;
namespace NodeServer{

	class SearchSession : public  Session 
	{
		public:
			SearchSession( tcp::socket *temp_socket, struct HeadStructMessage header);
			virtual ~SearchSession();
			void Start();
            void H_ReadContent(const boost::system::error_code& error) ;
            void H_MatchPicture( const boost::system::error_code& error,
                    uint32_t task_id,
                    uint32_t picture_len); 
			void H_AfterSendResult(const boost::system::error_code& error);
			void PicMatch(const boost::system::error_code& error);

			void QueryMqsql(/*  */);
			void SetMsgBack(/*  */);
		private:
			std::string _RandomNum_();
			std::string _WriteToFile_(char* msg, int len);

		private:		
			char * _proto_buf_ptr_;
			char * _content_buf_ptr_;
			string _file_name_;

			typedef struct HeadStructMessage Header;
			tcp::socket _socket_;   
			Header _header_ ;
			
	};
}


#endif
