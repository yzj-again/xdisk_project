#pragma once
#include "xcom_task.h"
#include <fstream>
class XFileServerTask :
    public XComTask
{
public:
    // 接收到消息的回调
    void ReadCB(const XMsg* msg) override;
private:
    // 处理目录获取的消息，返回目录列表
    void GetDir(const XMsg* msg);
    // 处理客户端上传请求
    void Upload(const XMsg* msg);
    int file_size_;
    /// 当前路径
    std::string current_dir_ = "./";
    /// 写入上传的文件
    std::ofstream ofs_;
};

