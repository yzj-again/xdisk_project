#pragma once
#include "xcom_task.h"
class XFileServerTask :
    public XComTask
{
public:
    // ���յ���Ϣ�Ļص�
    void ReadCB(const XMsg* msg) override;
private:
    // ����Ŀ¼��ȡ����Ϣ������Ŀ¼�б�
    void GetDir(const XMsg* msg);
};

