#pragma once
#include "xcom_task.h"
#include <fstream>
class XFileServerTask :
    public XComTask
{
public:
    // ���յ���Ϣ�Ļص�
    void ReadCB(const XMsg* msg) override;
private:
    // ����Ŀ¼��ȡ����Ϣ������Ŀ¼�б�
    void GetDir(const XMsg* msg);
    // ����ͻ����ϴ�����
    void Upload(const XMsg* msg);
    int file_size_;
    /// ��ǰ·��
    std::string current_dir_ = "./";
    /// д���ϴ����ļ�
    std::ofstream ofs_;
};

