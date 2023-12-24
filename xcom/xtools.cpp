#define _CRT_SECURE_NO_WARNINGS
#include "xtools.h"
#ifdef _WIN32
#include <io.h>
#endif // _WIN32

XCOM_API std::string GetDirData(std::string path) {
    std::string data = "";
#ifdef _WIN32
    // �洢�ļ���Ϣ
    _finddata_t file;
    std::string dirpath = path + "/*.*";
    // Ŀ¼������
    intptr_t dir = _findfirst(dirpath.c_str(), &file);
    if (dir < 0) {
        return data;
    }
    do
    {
        if (file.attrib & _A_SUBDIR) continue;// �Ⱥ�����Ŀ¼
        char buf[1024] = { 0 };
        sprintf(buf, "%s,%u;", file.name, file.size);
        data += buf;
    } while (_findnext(dir, &file) == 0);
#else

#endif // _WIN32
    // ȥ����β�ֺ�
    if (!data.empty())
    {
        data = data.substr(0, data.size() - 1);
    }

    return data;
}