#ifndef XTHREAD_POOL_H
#define XTHREAD_POOL_H
#ifdef _WIN32
#ifdef XCOM_EXPORTS
#define XCOM_API __declspec(dllexport)
#else
#define XCOM_API __declspec(dllimport)
#endif
#else
#define XCOM_API
#endif // _WIN32
#include<vector>
// ֻ����һ�����ͣ�������ͷ�ļ�
class XThread;
class XTask;
class XCOM_API XThreadPool
{
public:
    ///////////////////////////////////////////////////////////////////////////
    /// @brief ��ȡ XThreadPool �ľ�̬���� ����̬������
    /// @return XThreadPool ��̬�����ָ��
    ///////////////////////////////////////////////////////////////////////////
    static XThreadPool* Get() {
        static XThreadPool p;
        return &p;
    }// ����ģʽ ֻάϵһ������
    ///////////////////////////////////////////////////////////////////////////
    /// @brief ��ʼ�������̲߳������̣߳������� event_base ,�����߳��п�ʼ������Ϣ
    ///////////////////////////////////////////////////////////////////////////
    void InitThread(int thread_count);
    ///////////////////////////////////////////////////////////////////////////
    /// @brief �ַ������߳���ִ�У������ task �� Init ���������ʼ��
    ///        �������ѯ�ַ����̳߳��еĸ����߳�
    /// @param task ����ӿڶ���XTask ��Ҫ�û��Լ��̳в����� InitThread ����
    /////////////////////////////////////////////////////////////////////////// 
    void Dispatch(XTask* task);
    XThreadPool() {};// ���캯��˽�л�
private:
    /// �߳�����
    int thread_count_ = 0;
    /// ��һ�ηַ����̣߳�������ѯ
    int last_thread_ = -1;
    /// �̳߳��̶߳���
    std::vector<XThread*>threads_;
};

#endif // !XTHREAD_POOL_H