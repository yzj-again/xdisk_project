#include "xfile_server_task.h"
#include "xtools.h"
#include <iostream>
void XFileServerTask::ReadCB(const XMsg* msg) {
    switch (msg->type)
    {
    case MSG_GETDIR:// ���������ص�Ŀ¼�б� 
        std::cout << "MSG_GETDIR" << std::endl;
        GetDir(msg);
        break;
    default:
        break;
    }
}

void XFileServerTask::GetDir(const XMsg* msg) {
    if (!msg->data)return;
    std::string rootpath = msg->data;
    if (rootpath.empty()) {
        rootpath = "./";
    }
    std::string dir = GetDirData(rootpath);
    XMsg resmsg;
    resmsg.type = MSG_DIRLIST;
    resmsg.size = dir.size() + 1;// ��1����'\0'
    resmsg.data = (char*)dir.c_str();
    WriteCB(&resmsg);
}