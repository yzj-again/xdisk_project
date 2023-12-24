#pragma once
#include "xcom_task.h"
typedef void (*DirCBFunc)(std::string dirs);
class XDirTask :
    public XComTask
{
public:
    // 客户端要收服务端回的消息
     // 接收到消息的回调，由业务类重载
    void ReadCB(const XMsg* msg) override;
    // 连接成功的消息回调
    void ConnectedCB() override;
    // 获取目录后调用的回调
    DirCBFunc DirCB = nullptr;
    std::string get_server_path() const { return server_path_; };
    void set_server_path(std::string serverPath) { this->server_path_ = serverPath; }
private:
    std::string server_path_ = "";
};

