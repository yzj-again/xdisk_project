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
    // 读取数据回调函数
    virtual void ReadCB();
    // 接收到消息的回调，由业务类重载
    virtual void ReadCB(const XMsg* msg);
    // 发送消息
    virtual bool WriteCB(const XMsg* msg);
    // 连接成功的消息回调
    virtual void ConnectedCB() {};
protected:
    // 读取缓存
    char read_buf_[4096] = { 0 };
private:
    struct bufferevent* bev_;
    /// 服务器IP
    std::string server_ip_ = "";
    /// 服务器端口号
    int server_port_ = 0;
    // 数据包缓存
    XMsg msg_;
};
#endif // !XCOM_TASK_H
