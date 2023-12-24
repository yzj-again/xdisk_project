#pragma once
#include "xcom_task.h"
class XFileServerTask :
    public XComTask
{
public:
    // 接收到消息的回调
    void ReadCB(const XMsg* msg) override;
private:
    // 处理目录获取的消息，返回目录列表
    void GetDir(const XMsg* msg);
};

