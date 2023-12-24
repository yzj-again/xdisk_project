#ifndef XCOM_TASK_H
#define XCOM_TASK_H
#include "xtask.h"
#include "xmsg.h"
#include <string>
class XCOM_API XComTask :
    public XTask
{
public:
    virtual bool Init();
    std::string get_server_ip() const { return server_ip_; };
    void set_server_ip(std::string serverIP) { this->server_ip_ = serverIP; }
    int get_server_port() const { return server_port_; }
    void set_server_port(int serverPort) { this->server_port_ = serverPort; }
    virtual void EventCB(short what);
    // ��ȡ���ݻص�����
    virtual void ReadCB();
    // ���յ���Ϣ�Ļص�����ҵ��������
    virtual void ReadCB(const XMsg* msg);
    // ������Ϣ
    virtual bool WriteCB(const XMsg* msg);
    // ���ӳɹ�����Ϣ�ص�
    virtual void ConnectedCB() {};
protected:
    // ��ȡ����
    char read_buf_[4096] = { 0 };
private:
    struct bufferevent* bev_;
    /// ������IP
    std::string server_ip_ = "";
    /// �������˿ں�
    int server_port_ = 0;
    // ���ݰ�����
    XMsg msg_;
};
#endif // !XCOM_TASK_H
