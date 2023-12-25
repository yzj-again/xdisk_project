#pragma once
#include <QObject>
#include<string>
class XDiskClient :public QObject
{
    Q_OBJECT

public:
    // 单键模式 唯一对象
    static XDiskClient* Get() {
        static XDiskClient xdc;
        return &xdc;
    }
    /////////////////////////////////////////////////////////////////////////
    ///@brief 获取目录下的文件列表，只是请求消息到服务端
    void GetDir();
    /////////////////////////////////////////////////////////////////////////
    ///@brief 初始化，包括线程池
    bool Init();
    /////////////////////////////////////////////////////////////////////////
    ///@brief 上传文件请求
    ///@para filepath 本地文件路径
    void Upload(std::string filepath);
    std::string get_server_ip() const { return server_ip_; }
    void set_server_ip(std::string serverIP) { this->server_ip_ = serverIP; }
    int get_server_port() const { return server_port_; }
    void set_server_port(int serverPort) { this->server_port_ = serverPort; }
    std::string get_server_path() const { return server_path_; }
    void set_server_path(std::string serverPath) { this->server_path_ = serverPath; }

signals:
    // 添加信号
    void SDir(std::string dirs);
private:
    /// 服务器IP
    std::string server_ip_ = "";
    /// 服务器路径
    std::string server_path_ = "";
    /// 服务器端口号
    int server_port_ = 0;
    XDiskClient() {};
};

