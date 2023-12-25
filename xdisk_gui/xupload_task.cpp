#include "xupload_task.h"
#include <iostream>
// ���յ���Ϣ�Ļص�����ҵ��������
void XUploadTask::ReadCB(const XMsg* msg) {

}
// ���ӳɹ�����Ϣ�ص�
void XUploadTask::ConnectedCB() {
    // �����ϴ��ļ�����
    if (get_file_path().empty()) {
        std::cout << "XUploadTask ConnectedCB filepath_ is empty" << std::endl;
        return;
    }

    // 1.�򿪱����ļ�
    // ��ת����β����ȡ�ļ���С
    ifs_.open(get_file_path().c_str(), std::ios::in | std::ios::binary | std::ios::ate);
    if (!ifs_.is_open()) {
        std::cerr << "open file " << file_path_ << "failed!" << std::endl;
        return;
    }
    // 2.��ȡ�ļ���С ��ȡ�ļ����� filename.xxx,1024
    file_size_ = ifs_.tellg();
    ifs_.seekg(0, std::ios::beg);
    std::cout << "open file " << file_path_ << "success!" << std::endl;
    std::string filename = "";
    int pos = file_path_.find_last_of('/');
    if (pos < 0)pos = 0;
    filename = file_path_.substr(pos, file_path_.size() - pos);
    char data[1024] = { 0 };
    sprintf(data, "%s,%d", filename.c_str(), file_size_);
    // 3.�����ϴ��ļ�����
    XMsg msg;
    msg.type = MSG_UPLOAD_INFO;
    msg.data = data;
    msg.size = strlen(data) + 1;
    WriteCB(&msg);

}
