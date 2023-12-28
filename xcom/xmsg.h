#ifndef XMSG_H
#define XMSG_H

#ifdef _WIN32
#ifdef XCOM_EXPORTS
#define XCOM_API __declspec(dllexport)
#else
#define XCOM_API __declspec(dllimport)
#endif
#else
#define XCOM_API
#endif // _WIN32
enum MsgType
{
    MSG_NONE = 0,
    MSG_GETDIR,// ����Ŀ¼
    MSG_DIRLIST,// ����Ŀ¼�б�
    MSG_UPLOAD_INFO,// �����ϴ��ļ�
    MSG_UPLOAD_ACCEPT,// �����׼���Ž����ļ�
    MSG_MAX_TYPE,// ������֤������ȷ
};
// ��Ϣ����ֽ���
#define MSG_MAX_SIZE 1000000 
// ��Ϣͷ
struct XCOM_API XMsgHead
{
    MsgType type;
    int size = 0;
};
// Լ��ÿ����Ϣ����������ݣ�û�����ݲ�ʹ��
struct XCOM_API XMsg :public XMsgHead {
    char* data = 0;// �洢��Ϣ����
    int recved = 0;// �Ѿ����յ���Ϣ�ֽ���
};
#endif // !XMSG_H

