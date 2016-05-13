#include "SessionService.h"

#include<iostream>

#include "ClientSession.h"
#include "NodeSession.h"
#include "ClientMessage.h"
#include "NodeMessage.h"

using namespace std;
using namespace cjtech::RootServer;

SessionService* SessionService::_pSessionService_ = NULL;
namespace cjtech
{
    namespace RootServer
    {
        SessionService* SessionService::getInstance(int num )
        {
            if(_pSessionService_ == NULL)
            {
                _pSessionService_ = new SessionService(num);
            }
            return _pSessionService_;
        }

        SessionService::SessionService( int node_num)
        {
            node_manager_ = new NodeManager(node_num);
            _task_id_ = 0;
        }
        
        SessionService::~SessionService()
        {

        }

        void SessionService::RequsetHandler( ClientSession* cli_session,
                ClientMessage* req_msg)
        {
            _mtx_.lock();
            uint64_t taskid = _task_id_;
            NodeMessage* node_msg = new NodeMessage();
            _data_switcher_.Json2PB( req_msg, node_msg);
            node_msg->SetTaskID(taskid);
            node_msg->SetBufMsg2Node();
            NodeSession* temp_node_session = node_manager_->GetNodeByRotate();
            _taskid_clises_[taskid] = cli_session;
            _taskid_ndeses_[taskid] = temp_node_session;
			_task_id_++;
            _mtx_.unlock();
            temp_node_session->TrySendMsg(node_msg);
        }

        void SessionService::NodeHandler( NodeSession* node_session,
                NodeMessage* req_msg)
        {
            uint64_t task_id = req_msg->inner_msg.task_id();
            ClientSession* cli_session = _taskid_clises_[task_id];
            ClientMessage* cli_msg = new ClientMessage();
            _data_switcher_.PB2Json( req_msg, cli_msg);
            //cout<<"cli msg :"<<cli_msg->GetOutLoc()<<endl;;
            //cout<<"cli len :"<<cli_msg->GetOutLen()<<endl;;
            
            cli_session->try_send_msg(cli_msg);
        }
        
        void SessionService::Run()
        {
            node_manager_->run();
        }
    }
}
