/*************************************************************************
	> File Name: HeadStructMessage.h
	> Author: zhonghx
	> Mail: zhonghongxia@foxmail.com
 ************************************************************************/

#include "ClientSession.h"
#include "SearchSession.h"
#include <glog/logging.h>  
#include <glog/raw_logging.h> 
#include "NodeUtil.h"
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>

using std::cout;
using std::cin;
using std::endl;
using boost::asio::ip::tcp;

extern boost::mutex g_match_lock; 
extern NodeServer::IOServicePool *g_io_service_pool;
extern NodeServer::SessionManager* g_session_manager; 
extern Matcher* g_pic_matcher; 
namespace NodeServer
{
	ClientSession::~ClientSession()
	{
	}

    ClientSession::ClientSession( tcp::socket *temp_socket)
        :_socket_(g_io_service_pool->GetIoService())
    {
        int socketDup = dup(temp_socket->native());  
        _socket_.assign(boost::asio::ip::tcp::v4(), socketDup); 
    }
	void ClientSession::Start()
	{
        LOG(INFO)<< "ClientSession :: Started ";
        LOG(INFO)<< "ClientSession :: begin read header ... ";
			boost::asio::async_read(_socket_,
				boost::asio::buffer(&_header_,sizeof(struct  HeadStructMessage)),
				boost::bind(&ClientSession::H_Read_Proto, this,
				boost::asio::placeholders::error));
    }
	void ClientSession::H_Read_Header(const boost::system::error_code& error)
	{
		if (!error)
		{
		//async_read 保证将数据读取完毕后才会调用回调函数
            LOG(INFO)<<"ClientSession :: Send to Root OK!";
            LOG(INFO)<< "ClientSession :: begin read header again... ";
			boost::asio::async_read(_socket_,
				boost::asio::buffer(&_header_,sizeof(struct  HeadStructMessage)),
				boost::bind(&ClientSession::H_Read_Proto, this,
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
    void ClientSession::H_Read_Proto(const boost::system::error_code& error)
    {
        if(!error)
        {
            LOG(INFO)<< "ClientSession :: Header shows : proto length : "
                <<_header_.length;
            LOG(INFO)<< "ClientSession :: Read proto ing... : " ;
            _proto_buf_ptr_ = new char[_header_.length ];
            memset(_proto_buf_ptr_ , 0 ,_header_.length ); 
            boost::asio::async_read(_socket_,boost::asio::buffer(_proto_buf_ptr_,(_header_.length)), 
                    boost::bind(&ClientSession::H_Read_File, this,
                        boost::asio::placeholders::error));
        }
        else
        {
        
        }
    }
    void ClientSession::H_Read_File(const boost::system::error_code& error)
    {
        if(!error)
        {
            LOG(INFO)<< "ClientSession :: Read proto SUCCESSFUL " ;
            switch (_header_.type)
            {
                case SEARCH_REQUEST:
                    {
                        SearchProtoMessage search_proto_message;  
                        if(!search_proto_message.ParseFromArray(_proto_buf_ptr_,_header_.length))
                        {
                            LOG(ERROR)<< "ClientSession :: parse error ";

                            //g_session_manager->Recycle(GetSessionID());
                            return ;
                        }				
                        else
                        {
                            if(search_proto_message.retrain()== -1)
                            {
                        LOG(INFO)<< "ClientSession :: Proto show it is SEARCH_REQUEST " ;
                                if(_proto_buf_ptr_ != NULL)
                                {
                                    delete []_proto_buf_ptr_;
                                    _proto_buf_ptr_ = NULL;
                                }
                                //获取到picture的length				
                                _pic_len_ = search_proto_message.picture_length();
                                LOG(INFO)<< "ClientSession :: Proto shows : picture_length : "
                                    << _pic_len_  <<" picture name : "
                                    <<search_proto_message.picture_name();
                                if(_pic_len_ > 512000 )//设置接收的图片大小不超过500K
                                {
                                    LOG(ERROR)<< "ClientSession :: too large picture size ";
                                    /*
                                     *todo 
                                     *这时返回ROOT错误查询，让ROOT关闭此连接
                                     *
                                     * */
                                }
                                _content_buf_ptr_ = new char[ _pic_len_  ];
                                memset(_content_buf_ptr_ , 0 ,_pic_len_ );
                                _pic_len_ = search_proto_message.picture_length();
                                boost::asio::async_read(_socket_,
                                    boost::asio::buffer(_content_buf_ptr_,_pic_len_),
                                    boost::bind(&ClientSession::H_New_Search_Session, this,
                                        boost::asio::placeholders::error , search_proto_message , _content_buf_ptr_));	
                            }
                            else if(search_proto_message.retrain()== 1)
                            {
                        LOG(INFO)<< "ClientSession :: Proto show it is RETRAIN_REQUEST " ;
                                g_match_lock.lock();
                                remove(INDEXPATH);
                                EmptyDir(FEATUREPATH) ;
                                delete g_pic_matcher;
                                g_pic_matcher = new Matcher();
                                g_pic_matcher->train(TRANDIR, FEATUREPATH, INDEXPATH);
                                g_match_lock.unlock();
                                SearchResultMessage ack;
                                ack.set_status(0);
                                ack.set_retrain(1);
                                ack.set_mj_id("");
                                ack.set_task_id(0);
                                ack.set_picture_name("");
                                ack.set_result_length(0);
                                ack.set_trackerurl("");
                                string tmp;
                                ack.SerializeToString(&tmp);
                                HeadStructMessage header;
                                header.type = SEARCH_REQUEST_RESULT_FROM_NODE;
                                header.length = tmp.length();
                                string send((char *)&header , sizeof(HeadStructMessage));
                                send += tmp;
                                boost::asio::async_write(_socket_,
                                    boost::asio::buffer(send ,send.length()),
                                    boost::bind(&ClientSession::H_Read_Header, this,
                                        boost::asio::placeholders::error ));	
                            }
                        }
                        break; 
                    }
                    /*  
                case 0xa2:
                    {
                        LOG(INFO)<< "ClientSession :: Proto show it is TRAIN_REQUEST " ;
                        TrainPictureProtoMessage train_proto_message;  
                        if(!train_proto_message.ParseFromArray(_proto_buf_ptr_,_header_.length))
                        {
                            LOG(ERROR)<< "ClientSession :: train parse error ";
                            return ;
                        }				
                        else
                        {
                            if(_proto_buf_ptr_ != NULL)
                            {
                                delete []_proto_buf_ptr_;
                                _proto_buf_ptr_ = NULL;
                            }
                            g_match_lock.lock();
                            g_pic_matcher->train(TRANDIR, FEATUREPATH, INDEXPATH);
                            g_match_lock.unlock();
                            TrainPictureProtoMessageACK ack;
                            ack.set_session_id(train_proto_message.session_id());
                            ack.set_status(1);
                            string tmp;
                            ack.SerializeToString(&tmp);
                            HeadStructMessage header;
                            header.type = 0xa3;
                            header.length = tmp.length();
                            string send((char *)&header , sizeof(HeadStructMessage));
                            send += tmp;
                            boost::asio::async_write(_socket_,
                                boost::asio::buffer(send ,send.length()),
                                boost::bind(&ClientSession::H_Read_Header, this,
                                    boost::asio::placeholders::error ));	
                        }
                        break; 
                    }
                    */
                default : 
                    LOG(INFO)<< "ClientSession :: header.type not recongnize : " <<_header_.type;
                break;
            }
       ;}
        else
        {
        
        }
    
    }

	void ClientSession::H_New_Search_Session(const boost::system::error_code& error ,
            SearchProtoMessage msg,char * content_buf_ptr)
	{
		if (!error)
		{
		//根据任务类型创建新的session，然后再继续监听接下来的连接的请求
            string  path = _WriteToFile_(content_buf_ptr, _pic_len_) ; 
            LOG(INFO)<<"ClientSession :: file path "<<path; 
			SearchSession * session = g_session_manager->CreateSession<SearchSession>( msg , path ,GetSessionID());
            if(content_buf_ptr != NULL)
            {
                delete []content_buf_ptr;
                content_buf_ptr = NULL;
            }
            session->Start();
            WritePacket();
		}
		else
		{
            LOG(ERROR)<< "ClientSession :: Read file content Error"<<error.message();
			g_session_manager->Recycle(GetSessionID());
		}

	}
    void ClientSession::WritePacket()
    {
        boost::asio::async_write(_socket_,
            boost::asio::buffer(_sring_for_RS_.c_str() , _sring_for_RS_.length()),
                boost::bind(&ClientSession::H_Read_Header, this,
                    boost::asio::placeholders::error));	
        
    }
	string ClientSession::_WriteToFile_(const char* msg, int len)
	{
		string rand_str = _RandomNum_();
		string filepath = string(TMP_PATH) + rand_str;
		int fd = open( filepath.c_str() , O_CREAT | O_RDWR , 0666);
		if(fd<=0)
        {
			LOG(ERROR)<<"ClientSession :: create file error";
                perror("Open Erro ");
                return "";
        }
		int wnum = ::write(fd , msg, len);
		if(wnum <=0)
		{
			//cout<<"write error"<<endl;
			LOG(ERROR)<<"ClientSession :: write to file error";
			perror("write error\n");
			return "";
		}
        close(fd);
		return filepath;
	}
	string ClientSession::_RandomNum_()
	{
		long int  randnum = 0;
		stringstream rand_num_sstr;
        uint64_t t = time(NULL);
        srand(t);
		randnum = rand();
		long int  time_use=0;
		struct timeval start;
		gettimeofday(&start,NULL); 
		time_use = start.tv_sec*1000000 + start.tv_usec;

//		rand_num_sstr<<randnum;
		rand_num_sstr<<time_use;
		return rand_num_sstr.str();
	}
	int ClientSession:: EmptyDir(string destDir)
	{

		DIR *dp;
		struct dirent *entry;
		struct stat statbuf;
		if ((dp = opendir(destDir.c_str())) == NULL)
		{
			fprintf(stderr, "cannot open directory: %s\n", destDir.c_str());
			return -1;
		}
        char pwd[200];
        getcwd(pwd,200);
		chdir (destDir.c_str());
		while ((entry = readdir(dp)) != NULL)
		{
			lstat(entry->d_name, &statbuf);
			if (S_ISREG(statbuf.st_mode))
			{
				remove(entry->d_name);
			}
		}
        chdir (pwd);   

		return 0;
	}
}

