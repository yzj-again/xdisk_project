#include "xdisk_client.h"
#include <iostream>
#include "xdir_task.h"
#include "xthread_pool.h"
static void DirCB(std::string dirs);
/////////////////////////////////////////////////////////////////////////
///@brief 获取目录下的文件列表，只是请求消息到服务端
///@para path 目录路径
void XDiskClient::GetDir() {
    auto task = new XDirTask();
    task->set_server_ip(server_ip_);
    task->set_server_port(server_port_);
    task->set_server_path(server_path_);
    task->DirCB = DirCB;
    std::cout << "GetDir" << get_server_ip() << std::endl;
    // 现在不能操作，task没有初始化，task现在没有event_base

    XThreadPool::Get()->Dispatch(task);
}
bool XDiskClient::Init() {
    // 客户端线程数量固定
    XThreadPool::Get()->InitThread(10);
    return true;
}
void DirCB(std::string dirs) {
    XDiskClient::Get()->SDir(dirs);
}