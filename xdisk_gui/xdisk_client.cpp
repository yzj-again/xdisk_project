#include "xdisk_client.h"
#include <iostream>
#include "xdir_task.h"
#include "xthread_pool.h"
static void DirCB(std::string dirs);
/////////////////////////////////////////////////////////////////////////
///@brief ��ȡĿ¼�µ��ļ��б�ֻ��������Ϣ�������
///@para path Ŀ¼·��
void XDiskClient::GetDir() {
    auto task = new XDirTask();
    task->set_server_ip(server_ip_);
    task->set_server_port(server_port_);
    task->set_server_path(server_path_);
    task->DirCB = DirCB;
    std::cout << "GetDir" << get_server_ip() << std::endl;
    // ���ڲ��ܲ�����taskû�г�ʼ����task����û��event_base

    XThreadPool::Get()->Dispatch(task);
}
bool XDiskClient::Init() {
    // �ͻ����߳������̶�
    XThreadPool::Get()->InitThread(10);
    return true;
}
void DirCB(std::string dirs) {
    XDiskClient::Get()->SDir(dirs);
}