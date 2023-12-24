#include "xdisk_gui.h"
#include <QtWidgets/QApplication>
#ifdef _WIN32
#include <windows.h>
#else
#include <signal.h>
#endif // _WIN32
int main(int argc, char* argv[])
{
#ifdef _WIN32
    // 初始化socket库
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#else
    // 使用断开连接socket，会发出此信号，造成程序退出
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        return 1;
#endif // _WIN32
    QApplication a(argc, argv);
    XDiskGUI w;
    w.show();
    return a.exec();
}
