#include "xthread_pool.h"
#include "xthread.h"
#include <thread>
#include <chrono>
#include <iostream>
#include "xtask.h"

// �ַ��߳�
void XThreadPool::Dispatch(XTask* task) {
    // ��ѯ
    if (!task) return;
    int tid = (last_thread_ + 1) % thread_count_;
    last_thread_ = tid;
    XThread* t = threads_[tid];
    t->AddTask(task);
    // �����߳�
    t->ActivateThread();
    // 
}
// ��ʼ�������̲߳������߳�
void XThreadPool::InitThread(int thread_count) {
    this->thread_count_ = thread_count;
    this->last_thread_ = -1;
    for (int i = 0; i < thread_count; i++) {
        XThread* t = new XThread();
        t->id = i + 1;
        std::cout << "create thread" << std::endl;
        // �����߳�
        t->StartThread();
        threads_.push_back(t);// û���̳߳�ͻ
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}