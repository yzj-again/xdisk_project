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
    // ��Ŀ¼��ȡ���ź�
    // ע���ź�֧�ֵ�����
    qRegisterMetaType<std::string>("std::string");
    QObject::connect(XDiskClient::Get(), SIGNAL(SDir(std::string)), this, SLOT(UpdateDir(std::string)));
}

void XDiskGUI::UpdateServerInfo() {
    // ��ȡ������ֵ�� ������·�� ������IP �������˿�
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

    // 1.���ӷ�����

    // 2.�ȴ����ûص� ����ģʽ
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
    // �û�ѡ��һ���ļ�
    QString filename = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("��ѡ���ϴ��ļ�"));
    if (filename.isEmpty()) {
        return;
    }
    UpdateServerInfo();
    XDiskClient::Get()->Upload(filename.toStdString());
}

XDiskGUI::~XDiskGUI()
{}
