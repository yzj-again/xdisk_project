#ifndef XTHREAD_H
#define XTHREAD_H
#include <list>
#include <mutex>
class XTask;
class XThread
{
public:
    // 启动线程
    void StartThread();

    // 线程的入口函数
    void EnterThread();

    // 
    void Notify(int fd, short which);
    // 安装线程，初始化event_base和管道监听事件
    bool SetupThread();
    // 线程激活
    void ActivateThread();
    // 添加处理的任务，一个线程可以同时处理多个任务，共用一个event_base
    void AddTask(XTask* t);
    XThread();
    ~XThread();
    // 线程编号
    int id = 0;
private:
    int notify_send_fd_ = 0;
    struct event_base* base_;
    // 任务列表
    std::list<XTask*> tasks_;
    // 线程安全考虑
    std::mutex tasks_mutex_;
};
#endif // !XTHREAD_H
