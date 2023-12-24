// xcom.cpp : 定义 DLL 的导出函数。
//

#include "framework.h"
#include "xcom.h"


// 这是导出变量的一个示例
XCOM_API int nxcom=0;

// 这是导出函数的一个示例。
XCOM_API int fnxcom(void)
{
    return 0;
}

// 这是已导出类的构造函数。
Cxcom::Cxcom()
{
    return;
}
