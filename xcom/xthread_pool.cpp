#include "xthread_pool.h"
#include "xthread.h"
#include <thread>
#include <chrono>
#include <iostream>
#include "xtask.h"

// 分发线程
void XThreadPool::Dispatch(XTask* task) {
    // 轮询
    if (!task) return;
    int tid = (last_thread_ + 1) % thread_count_;
    last_thread_ = tid;
    XThread* t = threads_[tid];
    t->AddTask(task);
    // 激活线程
    t->ActivateThread();
    // 
}
// 初始化所有线程并启动线程
void XThreadPool::InitThread(int thread_count) {
    this->thread_count_ = thread_count;
    this->last_thread_ = -1;
    for (int i = 0; i < thread_count; i++) {
        XThread* t = new XThread();
        t->id = i + 1;
        std::cout << "create thread" << std::endl;
        // 启动线程
        t->StartThread();
        threads_.push_back(t);// 没有线程冲突
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}