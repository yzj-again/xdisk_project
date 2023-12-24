#ifndef XTHREAD_H
#define XTHREAD_H
#include <list>
#include <mutex>
class XTask;
class XThread
{
public:
    // �����߳�
    void StartThread();

    // �̵߳���ں���
    void EnterThread();

    // 
    void Notify(int fd, short which);
    // ��װ�̣߳���ʼ��event_base�͹ܵ������¼�
    bool SetupThread();
    // �̼߳���
    void ActivateThread();
    // ��Ӵ��������һ���߳̿���ͬʱ���������񣬹���һ��event_base
    void AddTask(XTask* t);
    XThread();
    ~XThread();
    // �̱߳��
    int id = 0;
private:
    int notify_send_fd_ = 0;
    struct event_base* base_;
    // �����б�
    std::list<XTask*> tasks_;
    // �̰߳�ȫ����
    std::mutex tasks_mutex_;
};
#endif // !XTHREAD_H
