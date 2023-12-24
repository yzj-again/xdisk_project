#pragma once
#include "xcom_task.h"
typedef void (*DirCBFunc)(std::string dirs);
class XDirTask :
    public XComTask
{
public:
    // �ͻ���Ҫ�շ���˻ص���Ϣ
     // ���յ���Ϣ�Ļص�����ҵ��������
    void ReadCB(const XMsg* msg) override;
    // ���ӳɹ�����Ϣ�ص�
    void ConnectedCB() override;
    // ��ȡĿ¼����õĻص�
    DirCBFunc DirCB = nullptr;
    std::string get_server_path() const { return server_path_; };
    void set_server_path(std::string serverPath) { this->server_path_ = serverPath; }
private:
    std::string server_path_ = "";
};

