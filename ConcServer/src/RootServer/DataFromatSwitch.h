#ifndef _DATAFROMATSWITCH_H
#define _DATAFROMATSWITCH_H

#include "ClientMessage.h"
#include "NodeMessage.h"
#include <sys/time.h>
#include <stdlib.h>
#include <sstream>
#include <glog/logging.h>
#include <glog/raw_logging.h>
#include "JsonMap.h"

typedef std::string STDSTR;

namespace cjtech{
    namespace RootServer{
        class DataFromatSwitch
        {
            public:
                DataFromatSwitch();
                virtual ~DataFromatSwitch();
            public:
                template<class T, class W> 
                    void Json2PB(T* json_data, W* pb_data)
                    {
                        struct timeval tv_begin;
                        gettimeofday(&tv_begin, NULL);
                        STDSTR pic_name = json_data->findInJsonMap("picture_name");
                        STDSTR pic_len = json_data->findInJsonMap("picture_length");
                        STDSTR retrian = json_data->findInJsonMap("retrain");
						LOG(INFO)<<"Json2PB::picture_name : "<<pic_name<<std::endl;
						LOG(INFO)<<"Json2PB::picture_len : "<<atoi(pic_len.c_str())<<std::endl;
                        long int start_time = tv_begin.tv_sec;
                        long int end_time = 0;
                        pb_data->out_msg.set_picture_name(pic_name);
                        pb_data->out_msg.set_picture_length(atoi(pic_len.c_str()));
                        pb_data->out_msg.set_start_time(0);
                        pb_data->out_msg.set_end_time(0);
                        if(retrian == std::string("1"))
                        {
                            pb_data->out_msg.set_retrain(1);
                        }
                        else 
                        {
                            pb_data->out_msg.set_retrain(-1);
                        }
                        pb_data->write_buf_ = json_data->GetFileBodyLoc();
                        pb_data->write_len_ = atoi(pic_len.c_str());
                        json_data->ClearFileLoc();
                    };

                template<class W, class T> 
                    void PB2Json(W* pb_data, T* json_data)
                    {
                        std::string pic_name = pb_data->inner_msg.picture_name();
                        int result_length  = pb_data->inner_msg.result_length();
                        std::string mj_id = pb_data->inner_msg.mj_id();
                        std::string trackerUrl = pb_data->inner_msg.trackerurl();
                        int status = pb_data->inner_msg.status();
                        int retrian = pb_data->inner_msg.retrain();
                        std::string status_str;
                        if(status == 1)
                        {
                            status_str = "1";
                        }
                        else
                        {
                            status_str = "-1";
                        }
                        std::stringstream len_str; 
                        len_str<<result_length;
                        std::stringstream retrain_str;
                        retrain_str<<retrian;
                        std::string len = len_str.str();
                        std::string out_str = "{\"pic_name\":\"" + pic_name + "\",\"result_length\":\"" + len + "\"," 
                            + "\"mjProductId\":\"" + mj_id + "\",\"trackUrl\":\""+trackerUrl+"\",\"status\":"+
                            status_str + ",\"retrain\":\""+retrain_str.str()+"\"}\r\n";
                        //std::cout<<"the out meg 2 client is like this : "<<out_str<<std::endl;
                        json_data->out_buf_ = (char*)malloc(out_str.length());
                        memcpy(json_data->out_buf_,out_str.c_str(),out_str.length());
                        json_data->SetOutLen(out_str.length());
                    };
        };
    }
}
#endif
