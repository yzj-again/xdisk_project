#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <signal.h>
#endif // _WIN32
#include"xthread_pool.h"
#include "xserver_task.h"
#include "xfile_server_task.h"
#include <thread>
#include <chrono>


static void ListenCB(int sock, struct sockaddr* addr, int socklen, void* arg);

int main(int argc, char* argv[]) {
#ifdef _WIN32
    // 初始化socket库
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#else
    // 使用断开连接socket，会发出此信号，造成程序退出
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        return 1;
#endif // _WIN32
    // 获取端口和线程池数量
    int server_port = 20010;
    int thread_count = 10;
    if (argc > 1)
        server_port = atoi(argv[1]);
    if (argc > 2)
        thread_count = atoi(argv[2]);
    if (argc == 1)
        std::cout << "xdisk_server port thread_count" << std::endl;
    // 初始化主线程池
    XThreadPool::Get()->InitThread(thread_count);


    XThreadPool server_pool;
    server_pool.InitThread(1);
    auto task = new XServerTask();
    task->set_server_port(server_port);
    task->ListenCB = ListenCB;
    server_pool.Dispatch(task);

    while (1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}

void ListenCB(int sock, struct sockaddr* addr, int socklen, void* arg) {
    auto task = new XFileServerTask();
    task->set_sock(sock);
    XThreadPool::Get()->Dispatch(task);
    std::cout << "ListenCB in main" << std::endl;
}