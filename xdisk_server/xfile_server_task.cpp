#include "xfile_server_task.h"
#include "xtools.h"
#include <iostream>
void XFileServerTask::ReadCB(const XMsg* msg) {
    switch (msg->type)
    {
    case MSG_GETDIR:// 服务器返回的目录列表 
        std::cout << "MSG_GETDIR" << std::endl;
        GetDir(msg);
        break;
    case MSG_UPLOAD_INFO:
        std::cout << "MSG_UPLOAD_INFO" << std::endl;
        Upload(msg);
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
    current_dir_ = rootpath;
    std::string dir = GetDirData(rootpath);
    XMsg resmsg;
    resmsg.type = MSG_DIRLIST;
    resmsg.size = dir.size() + 1;// 加1发送'\0'
    resmsg.data = (char*)dir.c_str();
    WriteCB(&resmsg);
}
void XFileServerTask::Upload(const XMsg* msg) {
    // 1.获取文件名和文件大小
    if (!msg->data || !msg || msg->size <= 0)return;
    std::string str = msg->data;
    if (str.empty())return;
    int pos = str.find_last_of(',');
    if (pos <= 0)return;
    std::string filename = str.substr(0, pos);
    int sizepos = pos + 1;
    if (sizepos > str.size())return;
    std::string tmp = str.substr(sizepos, str.size() - sizepos);
    file_size_ = atoi(tmp.c_str());
    // 2.打开本地文件
    std::string filepath = current_dir_ + filename;
    ofs_.open(filepath, std::ios::out | std::ios::binary);
    if (!ofs_.is_open()) {
        std::cout << "open file " << filepath << " failed!" << std::endl;
        return;
    }
    std::cout << "open file " << filepath << " success!" << std::endl;
    // 3.回复accept
    XMsg resmsg;
    resmsg.type = MSG_UPLOAD_ACCEPT;
    resmsg.size = 3;// 加1发送'\0'
    resmsg.data = (char*)"ok";
    WriteCB(&resmsg);
}