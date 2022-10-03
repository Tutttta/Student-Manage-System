#include "MockingString.h"

PSTR GetsEx(PSTR pBuf, size_t nBufSize, FILE *pFile /*= stdin*/)
{
	PSTR pszBufPtr = nullptr;
	size_t nIdx = 0;
	size_t nLen = 0;

	if (!pBuf || !nBufSize)
	{
		return(NULL);
	}
	// 如果传入了文件指针且为NULL, 则默认从标准输入中获取
	if (!pFile)
	{
		pFile = stdin;
	}
	pszBufPtr = fgets(pBuf, nBufSize, pFile);
	if (!pszBufPtr)
	{
		return(NULL);
	}
	nLen = strlen(pszBufPtr);
	if (pszBufPtr[nLen - 1] == '\n')
	{
		pszBufPtr[nLen - 1] = '\0';
		if (pszBufPtr[nLen - 2] == '\r')
		{
			pszBufPtr[nLen - 2] = '\0';
		}
	}
	else
	{
		if (pFile == stdin)
		{
			while (getchar() != '\n');
		}
	}
	
	return(pszBufPtr);
}


void ClearBuffer()
{
	while (getchar() != '\n');
}