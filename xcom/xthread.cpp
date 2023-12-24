#include "xthread.h"
#include <thread>
#include <iostream>
#include <event2/event.h>
#include "xtask.h"
#ifdef _WIN32
#else
#include<unistd.h>
#endif // 
// ��̬���������ͻ
// �����߳�����Ļص�����
static void NotifyCB(evutil_socket_t fd, short which, void* arg) {
    XThread* t = (XThread*)arg;
    t->Notify(fd, which);
}

void XThread::Notify(int fd, short which) {
    // ˮƽ������ֻҪû�н�����ɣ����ٴν���
    char buf[2] = { 0 };
#ifdef _WIN32
    int ret = recv(fd, buf, 1, 0);
#else
    // Linux�ǹܵ���������recv
    int ret = read(fd, buf, 1);
#endif // _WIN32
    if (ret <= 0) {
        return;
    }
    std::cout << id << "thread id = " << buf << std::endl;
    XTask* task = NULL;
    // ��ȡ���񣬲���ʼ������
    tasks_mutex_.lock();
    if (tasks_.empty())
    {
        tasks_mutex_.unlock();
        return;
    }
    task = tasks_.front();// �Ƚ��ȳ�
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
    // Linux�ǹܵ���������recv
    int ret = write(this->notify_send_fd_, "c", 1);
#endif // _WIN32
    if (ret <= 0) {
        std::cerr << "XThread::ActivateThread() failed" << std::endl;
    }
}

// �����߳�
void XThread::StartThread() {
    // �����߳�
    SetupThread();
    std::thread th(&XThread::EnterThread, this);
    // �Ͽ������߳���ϵ
    th.detach();
};

// ��װ�̣߳���ʼ��event_base�͹ܵ������¼�
bool XThread::SetupThread() {
    // windows�����socket linux�ùܵ�
#ifdef _WIN32
    // ����һ��socketpair ���Ի���ͨ�ţ� 0��1д
    evutil_socket_t fds[2];
    if (evutil_socketpair(AF_INET, SOCK_STREAM, 0, fds) < 0) {
        std::cout << "evutil_socketpair failed" << std::endl;
        return false;
    }
    // ���÷�����
    evutil_make_socket_nonblocking(fds[0]);
    evutil_make_socket_nonblocking(fds[1]);
#else
    int fds[2];
    // �������ǹܵ� ������send recv 
    if (pipe(fds)) {
        std::cerr << "pipe failed" << std::endl;
        return false;
    }
#endif
    // ��ȡ�󶨵�event�¼��У�д��Ҫ����
    notify_send_fd_ = fds[1];
    // ����libevent������
    // ����������
    event_config* ev_config = event_config_new();
    event_config_set_flag(ev_config, EVENT_BASE_FLAG_NOLOCK);
    this->base_ = event_base_new_with_config(ev_config);
    if (!base_) {
        std::cerr << "event_base_new failed" << std::endl;
        return false;
    }
    event_config_free(ev_config);
    // ��ӹܵ������¼������ڼ����߳�ִ������
    // �߳���ִ���ڼ䲻���˳�
    event* ev = event_new(base_, fds[0], EV_READ | EV_PERSIST, NotifyCB, this);
    event_add(ev, 0);
    return true;
};

// �̵߳���ں���
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