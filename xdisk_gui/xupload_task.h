#pragma once
#include "xcom_task.h"
#include <fstream>
class XUploadTask :
    public XComTask
{
public:
    // 1.重载函数
    // 接收到消息的回调，由业务类重载
    void ReadCB(const XMsg* msg) override;
    // 连接成功的消息回调
    void ConnectedCB() override;
    std::string get_file_path() const { return file_path_; };
    void set_file_path(std::string filePath) { this->file_path_ = filePath; }
private:
    /// 需要上传的文件路径
    std::string file_path_ = "";
    /// 读取文件
    std::ifstream ifs_;
    /// 文件大小，不支持大于2G
    int file_size_ = 0;
};

