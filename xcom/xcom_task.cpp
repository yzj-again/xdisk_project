#include "xcom_task.h"
#include <iostream>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <string.h>
static void SReadCB(struct bufferevent* bev, void* ctx);
static void SWriteCB(struct bufferevent* bev, void* ctx);
static void SEventCB(struct bufferevent* bev, short what, void* ctx);
bool XComTask::Init() {
    // ����socket ���ֿͻ��˻��Ƿ���˴�����socket
    int comsock = this->get_sock();
    if (comsock <= 0) {
        comsock = -1;
    }
    /// ��bufferevent �������� 
    // -1�Զ�����socket
    // 1.����bufferevent������
    bev_ = bufferevent_socket_new(get_base(), comsock, BEV_OPT_CLOSE_ON_FREE);
    if (!bev_) {
        std::cerr << "bufferevent_socket_new() failed!" << std::endl;
        return false;
    }
    // 2.���ûص�����
    bufferevent_setcb(bev_, SReadCB, SWriteCB, SEventCB, this);
    bufferevent_enable(bev_, EV_READ | EV_WRITE);
    // ��ʱʱ��
    timeval tv = { 10,0 };
    bufferevent_set_timeouts(bev_, &tv, &tv);
    // 3.���ӷ�����
    if (get_server_ip().empty()) {
        return true;
    }
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(get_server_port());
    evutil_inet_pton(AF_INET, get_server_ip().c_str(), &sin.sin_addr.s_addr);
    int ret = bufferevent_socket_connect(bev_, (sockaddr*)&sin, sizeof(sin));
    if (ret != 0) {
        return false;
    }
    return true;
}
void SReadCB(struct bufferevent* bev, void* ctx) {
    auto task = (XComTask*)ctx;
    task->ReadCB();
}
void SWriteCB(struct bufferevent* bev, void* ctx) {

}
void SEventCB(struct bufferevent* bev, short what, void* ctx) {
    auto task = (XComTask*)ctx;
    task->EventCB(what);

}
void XComTask::EventCB(short what) {
    if (what & BEV_EVENT_CONNECTED) {
        std::cout << "BEV_EVENT_CONNECTED" << std::endl;
        // ֪ͨ���ӳɹ�
        ConnectedCB();
    }
    // �˳�Ҫ����������
    if (what & BEV_EVENT_ERROR || what & BEV_EVENT_TIMEOUT) {
        std::cout << "BEV_EVENT_ERROR or BEV_EVENT_TIMEOUT" << std::endl;
        bufferevent_free(bev_);
    }
    if (what & BEV_EVENT_EOF) {
        std::cout << "BEV_EVENT_EOF" << std::endl;
        bufferevent_free(bev_);
    }
}

bool XComTask::WriteCB(const XMsg* msg) {

    if (!bev_ || !msg || !msg->data || msg->size <= 0) {
        return false;
    }
    // 1.д����Ϣͷ
    int ret = bufferevent_write(bev_, msg, sizeof(XMsgHead));
    if (ret != 0)return false;
    // 2.д����Ϣ����
    ret = bufferevent_write(bev_, msg->data, msg->size);
    if (ret != 0)return false;
    return true;
}


void XComTask::ReadCB(const XMsg* msg) {
    std::cout << "recv Msg type = " << msg->type << " Msg size = " << msg->size << std::endl;
}
void XComTask::ReadCB() {
    // ������ϢXMsgHead
    // ȷ����Ե����ʱ�ܶ�ȡ����������
    for (;;) {
        // 1.����ͷ����Ϣ
        if (!msg_.data) {
            int headsize = sizeof(XMsgHead);
            int len = bufferevent_read(bev_, &msg_, headsize);
            if (len <= 0) {
                return;
            }
            if (len != headsize) {
                std::cerr << "msg head receive error" << std::endl;
                return;
            }
            // ��֤��Ϣͷ����Ч��
            if (msg_.type >= MSG_MAX_TYPE || msg_.size <= 0 || msg_.size > MSG_MAX_SIZE) {
                std::cerr << "msg head is error" << std::endl;
                return;
            }
            msg_.data = new char[msg_.size];

        }
        int readsize = msg_.size - msg_.recved;
        if (readsize <= 0) {
            // ���������Ϣ
            delete msg_.data;
            memset(&msg_, 0, sizeof(msg_));
            return;
        }
        int len = bufferevent_read(bev_, msg_.data + msg_.recved, msg_.size);
        if (len <= 0) {
            return;
        }
        msg_.recved += len;
        if (msg_.recved == msg_.size) {
            // ������Ϣ
            std::cout << "recved msg" << msg_.size << std::endl;
            ReadCB(&msg_);
            delete msg_.data;
            memset(&msg_, 0, sizeof(msg_));
        }
    }
}
