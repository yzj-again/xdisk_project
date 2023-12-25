#include "xserver_task.h"
#include <iostream>
#include <string.h>
#include <event2/event.h>
#include <event2/listener.h>

static void SListenCB(struct evconnlistener* evc, evutil_socket_t client_socket, struct sockaddr* client_addr, int socklen, void* arg);


bool XServerTask::Init() {
    // ���տͻ��˹���������
    if (server_port_ <= 0) {
        std::cerr << "XServerTask::Init() failed!" << "server_port_ is not set" << std::endl;
        return false;
    }
    // �����ӿ�
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(server_port_);
    // ���ûص�����
    auto evc = evconnlistener_new_bind(get_base(), SListenCB, this, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
        10,// listen ��С
        (sockaddr*)&sin,
        sizeof(sin)
    );
    if (!evc) {
        std::cout << "listen_port" << server_port_ << "failed!" << std::endl;
        return false;
    }
    std::cout << "listen port " << server_port_ << " success!" << std::endl;
    return true;
}

void SListenCB(struct evconnlistener* evc, evutil_socket_t client_socket, struct sockaddr* client_addr, int socklen, void* arg) {
    std::cout << "SlistenCB" << std::endl;
    auto task = (XServerTask*)arg;
    if (task->ListenCB) {
        task->ListenCB(client_socket, client_addr, socklen, arg);
    }
    else
    {
        std::cerr << "please set callback function ListenCB" << std::endl;
    }




    /*

    sockaddr_in* addr = (sockaddr_in*)client_addr;
    char ip[16] = { 0 };
    evutil_inet_ntop(AF_INET, &addr->sin_addr, ip, sizeof(ip));
    std::cout << "client ip is " << ip << std::endl;
    event_base* base = (event_base*)arg;
    // ����buffevent������ ��Ҫ��base�´������������� ����event����read��write��
    // -1��ȷ��
    // BEV_OPT_CLOSE_ON_FREE�ر�bevʱ�ر�socket��������socket
    bufferevent* bev = bufferevent_socket_new(base, client_socket, BEV_OPT_CLOSE_ON_FREE);
    if (!bev) {
        std::cerr << "bufferevent_socket_new_failed!" << std::endl;
        return;
    }
    // ��Ӽ���¼�
    // �����ڲ�Ȩ�޲���
    bufferevent_enable(bev, EV_READ | EV_WRITE);
    // ���ó�ʱʱ�� ���΢��(�����֮һ��) ����ʱ��д��ʱ
    timeval time1 = { 10,0 };
    bufferevent_set_timeouts(bev, &time1, 0);// ���Ƕ���ʱ
    // ���ûص�����
    bufferevent_setcb(bev, readCB, writeCB, eventCB, base);
    */
}
