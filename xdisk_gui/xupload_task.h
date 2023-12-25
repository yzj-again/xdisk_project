#pragma once
#include "xcom_task.h"
#include <fstream>
class XUploadTask :
    public XComTask
{
public:
    // 1.���غ���
    // ���յ���Ϣ�Ļص�����ҵ��������
    void ReadCB(const XMsg* msg) override;
    // ���ӳɹ�����Ϣ�ص�
    void ConnectedCB() override;
    std::string get_file_path() const { return file_path_; };
    void set_file_path(std::string filePath) { this->file_path_ = filePath; }
private:
    /// ��Ҫ�ϴ����ļ�·��
    std::string file_path_ = "";
    /// ��ȡ�ļ�
    std::ifstream ifs_;
    /// �ļ���С����֧�ִ���2G
    int file_size_ = 0;
};

