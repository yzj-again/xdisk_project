#include "xserver_task.h"
#include <iostream>
#include <string.h>
#include <event2/event.h>
#include <event2/listener.h>

static void SListenCB(struct evconnlistener* evc, evutil_socket_t client_socket, struct sockaddr* client_addr, int socklen, void* arg);


bool XServerTask::Init() {
    // 接收客户端过来的连接
    if (server_port_ <= 0) {
        std::cerr << "XServerTask::Init() failed!" << "server_port_ is not set" << std::endl;
        return false;
    }
    // 监听接口
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(server_port_);
    // 设置回调函数
    auto evc = evconnlistener_new_bind(get_base(), SListenCB, this, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
        10,// listen 大小
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
    // 创建buffevent上下文 但要在base下创建，方便清理 创建event对象（read和write）
    // -1不确定
    // BEV_OPT_CLOSE_ON_FREE关闭bev时关闭socket不用清理socket
    bufferevent* bev = bufferevent_socket_new(base, client_socket, BEV_OPT_CLOSE_ON_FREE);
    if (!bev) {
        std::cerr << "bufferevent_socket_new_failed!" << std::endl;
        return;
    }
    // 添加监控事件
    // 设置内部权限参数
    bufferevent_enable(bev, EV_READ | EV_WRITE);
    // 设置超时时间 秒和微妙(百万分之一秒) 读超时和写超时
    timeval time1 = { 10,0 };
    bufferevent_set_timeouts(bev, &time1, 0);// 我们读超时
    // 设置回调函数
    bufferevent_setcb(bev, readCB, writeCB, eventCB, base);
    */
}
