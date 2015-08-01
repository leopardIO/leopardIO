/*************************************************************************
  > File Name: message.cpp
  > Author: cooperz
  > Mail: zbzcsn@qq.com
  > Created Time: Sun 26 Jul 2015 07:33:04 AM UTC
 ************************************************************************/
#include "ClientMessage.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <sys/time.h>
#include "JsonMap.h"

using std::cout;
using std::endl;


using namespace std;
using std :: cout;
using std :: endl;

namespace cjtech
{
    namespace nodeserver
    {
        bool TcpMessage::ParserJson()
        {
            JsonMap json_map = JsonMap(_json_body_);
            _json_map_ = json_map.getMap();
            return true;
        }    
        bool TcpMessage::ParserProtoBuf()
        {
            return true;
        }
        void TcpMessage::TranJsonLenCharToInt()
        {
            string str_json_len = string( _json_header_len_c_, header_len);
            _json_body_len_ = atoi(str_json_len.c_str());
            _json_body_c_ = (char*)malloc(sizeof(char)*_json_body_len_);
            return;
        }
        int TcpMessage::GetJsonBodyLen()
        {
            return _json_body_len_;
        }
        void TcpMessage::TranFileLenToInt()
        {
            string file_len_str = _json_map_["Content-Length"];
            int file_len = atoi(file_len_str.c_str());
            _file_body_len_ = file_len;
            _file_c_ = (char*)malloc(sizeof(char)*file_len);
        }
    }
}

