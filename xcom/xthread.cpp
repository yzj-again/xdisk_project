#include "xthread.h"
#include <thread>
#include <iostream>
#include <event2/event.h>
#include "xtask.h"
#ifdef _WIN32
#else
#include<unistd.h>
#endif // 
// 静态函数不会冲突
// 激活线程任务的回调函数
static void NotifyCB(evutil_socket_t fd, short which, void* arg) {
    XThread* t = (XThread*)arg;
    t->Notify(fd, which);
}

void XThread::Notify(int fd, short which) {
    // 水平触发，只要没有接收完成，会再次进来
    char buf[2] = { 0 };
#ifdef _WIN32
    int ret = recv(fd, buf, 1, 0);
#else
    // Linux是管道，不能用recv
    int ret = read(fd, buf, 1);
#endif // _WIN32
    if (ret <= 0) {
        return;
    }
    std::cout << id << "thread id = " << buf << std::endl;
    XTask* task = NULL;
    // 获取任务，并初始化任务
    tasks_mutex_.lock();
    if (tasks_.empty())
    {
        tasks_mutex_.unlock();
        return;
    }
    task = tasks_.front();// 先进先出
    tasks_.pop_front();
    tasks_mutex_.unlock();
    task->Init();
}


void XThread::AddTask(XTask* t) {
    if (!t)return;
    t->set_base(this->base_);
    tasks_mutex_.lock();
    tasks_.push_back(t);
    tasks_mutex_.unlock();
}


void XThread::ActivateThread() {
#ifdef _WIN32
    int ret = send(this->notify_send_fd_, "c", 1, 0);
#else
    // Linux是管道，不能用recv
    int ret = write(this->notify_send_fd_, "c", 1);
#endif // _WIN32
    if (ret <= 0) {
        std::cerr << "XThread::ActivateThread() failed" << std::endl;
    }
}

// 启动线程
void XThread::StartThread() {
    // 启动线程
    SetupThread();
    std::thread th(&XThread::EnterThread, this);
    // 断开与主线程联系
    th.detach();
};

// 安装线程，初始化event_base和管道监听事件
bool XThread::SetupThread() {
    // windows用配对socket linux用管道
#ifdef _WIN32
    // 创建一个socketpair 可以互相通信， 0读1写
    evutil_socket_t fds[2];
    if (evutil_socketpair(AF_INET, SOCK_STREAM, 0, fds) < 0) {
        std::cout << "evutil_socketpair failed" << std::endl;
        return false;
    }
    // 设置非阻塞
    evutil_make_socket_nonblocking(fds[0]);
    evutil_make_socket_nonblocking(fds[1]);
#else
    int fds[2];
    // 创建的是管道 不能用send recv 
    if (pipe(fds)) {
        std::cerr << "pipe failed" << std::endl;
        return false;
    }
#endif
    // 读取绑定到event事件中，写入要保存
    notify_send_fd_ = fds[1];
    // 创建libevent上下文
    // 无锁上下文
    event_config* ev_config = event_config_new();
    event_config_set_flag(ev_config, EVENT_BASE_FLAG_NOLOCK);
    this->base_ = event_base_new_with_config(ev_config);
    if (!base_) {
        std::cerr << "event_base_new failed" << std::endl;
        return false;
    }
    event_config_free(ev_config);
    // 添加管道监听事件，用于激活线程执行任务
    // 线程在执行期间不会退出
    event* ev = event_new(base_, fds[0], EV_READ | EV_PERSIST, NotifyCB, this);
    event_add(ev, 0);
    return true;
};

// 线程的入口函数
void XThread::EnterThread() {

    std::cout << id << "XThread::EnterThread() begin" << std::endl;
    if (!base_) {
        std::cerr << "XThread::EnterThread() failed! base_ is NULL" << std::endl;
        std::cerr << "In windows_set WSAStartup failed!" << std::endl;
        return;
    }
    event_base_dispatch(base_);
    event_base_free(base_);
    std::cout << id << "XThread::EnterThread() end" << std::endl;
};
XThread::XThread() {};
XThread::~XThread() {};