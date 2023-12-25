#pragma once
#include <QObject>
#include<string>
class XDiskClient :public QObject
{
    Q_OBJECT

public:
    // ����ģʽ Ψһ����
    static XDiskClient* Get() {
        static XDiskClient xdc;
        return &xdc;
    }
    /////////////////////////////////////////////////////////////////////////
    ///@brief ��ȡĿ¼�µ��ļ��б�ֻ��������Ϣ�������
    void GetDir();
    /////////////////////////////////////////////////////////////////////////
    ///@brief ��ʼ���������̳߳�
    bool Init();
    /////////////////////////////////////////////////////////////////////////
    ///@brief �ϴ��ļ�����
    ///@para filepath �����ļ�·��
    void Upload(std::string filepath);
    std::string get_server_ip() const { return server_ip_; }
    void set_server_ip(std::string serverIP) { this->server_ip_ = serverIP; }
    int get_server_port() const { return server_port_; }
    void set_server_port(int serverPort) { this->server_port_ = serverPort; }
    std::string get_server_path() const { return server_path_; }
    void set_server_path(std::string serverPath) { this->server_path_ = serverPath; }

signals:
    // ����ź�
    void SDir(std::string dirs);
private:
    /// ������IP
    std::string server_ip_ = "";
    /// ������·��
    std::string server_path_ = "";
    /// �������˿ں�
    int server_port_ = 0;
    XDiskClient() {};
};

