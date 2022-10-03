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
	// ����������ļ�ָ����ΪNULL, ��Ĭ�ϴӱ�׼�����л�ȡ
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