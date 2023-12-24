#ifndef XSERVER_TASK_H
#define XSERVER_TASK_H
#include "xtask.h"

typedef void (*ListenCBFunc)(int sock, struct sockaddr* addr, int socklen, void* arg);
class XCOM_API XServerTask :
    public XTask
{
public:
    virtual bool Init();
    // º¯ÊýÖ¸Õë 
    ListenCBFunc ListenCB = nullptr;
    int get_server_port() const { return server_port_; }
    void set_server_port(int serverPort) { this->server_port_ = serverPort; }
private:
    int server_port_;
};
#endif // !XSERVER_TASK_H
