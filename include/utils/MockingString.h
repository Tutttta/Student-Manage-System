#pragma once
#include "Global.h"

// 从标准输入或者文件中获取内容
PSTR GetsEx(PSTR pBuf, size_t nBufSize, FILE *pFile = stdin);

// 清理缓存
void ClearBuffer();
