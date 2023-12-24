#include "xdir_task.h"
#include <iostream>
void XDirTask::ConnectedCB() {
    XMsg msg;
    msg.type = MSG_GETDIR;
    msg.size = get_server_path().size() + 1;
    std::string serverPath = get_server_path();
    msg.data = new char[serverPath.length() + 1];
    strcpy(msg.data, serverPath.c_str());
    // msg.data = (char*)get_server_path().c_str();
    WriteCB(&msg);
}
void XDirTask::ReadCB(const XMsg* msg) {
    // ���յ�����˷��͵�Ŀ¼
    switch (msg->type)
    {
    case MSG_DIRLIST:// ���������ص�Ŀ¼�б� 
        std::cout << "MSG_DIRLIST" << std::endl;
        DirCB(msg->data);
        break;
    default:
        break;
    }
}