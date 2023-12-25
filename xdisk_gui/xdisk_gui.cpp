#include "xdisk_gui.h"
#include "xdisk_client.h"
#include <QMessageBox>
#include <QFileDialog>
#include <string>

XDiskGUI::XDiskGUI(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    XDiskClient::Get()->Init();
    // 绑定目录获取的信号
    // 注册信号支持的类型
    qRegisterMetaType<std::string>("std::string");
    QObject::connect(XDiskClient::Get(), SIGNAL(SDir(std::string)), this, SLOT(UpdateDir(std::string)));
}

void XDiskGUI::UpdateServerInfo() {
    // 获取到三个值， 服务器路径 服务器IP 服务器端口
    std::string ip = ui.ipEdit->text().toStdString();
    std::string root = ui.serverPathEdit->text().toStdString();
    int port = ui.portBox->value();
    XDiskClient::Get()->set_server_port(port);
    XDiskClient::Get()->set_server_ip(ip);
    XDiskClient::Get()->set_server_path(root);
}
void XDiskGUI::Refresh() {
    UpdateServerInfo();
    XDiskClient::Get()->GetDir();

    // 1.连接服务器

    // 2.等待设置回调 门面模式
}

void XDiskGUI::UpdateDir(std::string dirs) {
    QString str = dirs.c_str();
    str = str.trimmed();
    if (str.isEmpty()) {
        return;
    }
    QStringList filelist = str.split(';');
    ui.fileListWidget->setRowCount(filelist.size());
    for (int i = 0; i < filelist.size(); i++) {
        QStringList fileinfo = filelist[i].split(',');
        if (fileinfo.size() != 2) {
            continue;
        }
        ui.fileListWidget->setItem(i, 0, new QTableWidgetItem(fileinfo[0]));
        ui.fileListWidget->setItem(i, 1, new QTableWidgetItem(tr("%1Byte").arg(fileinfo[1])));
    }
}

void XDiskGUI::Upload() {
    // 用户选择一个文件
    QString filename = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("请选择上传文件"));
    if (filename.isEmpty()) {
        return;
    }
    UpdateServerInfo();
    XDiskClient::Get()->Upload(filename.toStdString());
}

XDiskGUI::~XDiskGUI()
{}
