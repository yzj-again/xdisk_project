#ifndef XTASK_H
#define XTASK_H

#ifdef _WIN32
#ifdef XCOM_EXPORTS
#define XCOM_API __declspec(dllexport)
#else
#define XCOM_API __declspec(dllimport)
#endif
#else
#define XCOM_API
#endif // _WIN32
class XCOM_API XTask
{
public:
    /// 初始化任务
    virtual bool Init() = 0;
    int get_thread_id() const { return threadID_; }
    void set_thread_id(int threadID) { this->threadID_ = threadID; }
    int get_sock() const { return sock_; }
    void set_sock(int sock) { sock_ = sock; }
    struct event_base* get_base() const { return base_; }
    void set_base(struct event_base* base) { this->base_ = base; }
private:
    int sock_ = 0;
    int threadID_ = 0;
    struct event_base* base_ = 0;
};
#endif // !XTASK_H
