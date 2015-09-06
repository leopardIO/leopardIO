#include "Session.h"
using namespace NodeServer;

Session::Session(): _sessionID_( 0 ),_is_recycler_( false )
{
}
Session::~Session()
{
}
string Session::FormMessage(uint32_t proto_length , uint32_t  proto_type , string &proto_str,string & content_str)
{	
	string ret_string ;
	HeadStructMessage header ;
	header.length = proto_length;
	header.type = proto_type;
	char header_buf[17];
	memset(header_buf , 0 ,17);
	memcpy(header_buf , &header , 17);
	ret_string +=  header_buf;
	ret_string += proto_str;
	ret_string += content_str;
	return ret_string; 			
}
