#include "IO.h"
#include "MockingString.h"

IO::IO(const char *pcszFilePathName, eIOType eMode /*= READONLY*/) : 
	m_strFileName(pcszFilePathName), m_eMode(eMode)
{ }


IO::IO(const CMyString &strFileName, eIOType eMode /*= READONLY*/) :
	m_strFileName(strFileName), m_eMode(eMode)
{ }


ULONGLONG IO::IOGetFileSize()
{
	char *pcszFileName = nullptr;
	ULONGLONG ullFileSize = 0;
	FILE *pFile = nullptr;
	BOOLEAN fOk = false;

	pcszFileName = m_strFileName.GetString();
	do
	{
		if (!pcszFileName)
		{
			break;
		}
		// 以读取方式打开文件
		pFile = fopen(pcszFileName, "rb");
		if (!pFile)
		{
			break;
		}
		// 获取文件大小
		fseek(pFile, 0, SEEK_END);
		ullFileSize = ftell(pFile);
	} while (false);
	
	if (pFile)
	{
		fclose(pFile);
		pFile = nullptr;
	}

	return(ullFileSize);
}


char ** IO::GetLineFromFile(size_t &rnSizeOfStringAry)
{
#define BASEAMOUNT 1000
#define INCREMENT 500
	char *pcszFileName = nullptr;
	FILE *pFile = nullptr;
	BOOLEAN fOk = FALSE;
	char **ppstrLineAry = nullptr;
	char **ppTmpStrLineAry = nullptr;
	size_t nArySize = BASEAMOUNT;
	size_t nCnt = 0;
	char szBuf[LINEMAX] = { 0 };
	char *pRet = nullptr;

	pcszFileName = m_strFileName.GetString();
	do
	{
		if (!pcszFileName)
		{
			break;
		}
		// 以读取方式打开文件
		pFile = fopen(pcszFileName, "rb");
		if (!pFile)
		{
			break;
		}
		// 分配二维数组
		ppstrLineAry = new PSTR[nArySize];
		if (!ppstrLineAry)
		{
			break;
		}
		
		do
		{
			pRet = GetsEx(szBuf, sizeof(szBuf), pFile);
			// 如果出了错
			if (!pRet)
			{
				break;
			}
			int iLenOfLine = 0;
			// 获取读取行的长度
			iLenOfLine = strlen(szBuf);
			// 如果空间不够了要动态更新空间
			if (nCnt >= nArySize)
			{
				nArySize += INCREMENT;
				// 分配新的临时空间
				ppTmpStrLineAry = new PSTR[nArySize];
				if (!ppTmpStrLineAry)
				{
					break;
				}
				memset(ppTmpStrLineAry, 0, nArySize * sizeof(ppTmpStrLineAry[0]));
				memcpy(ppTmpStrLineAry, ppstrLineAry, (nArySize - INCREMENT) * sizeof(ppstrLineAry[0]));
				ppstrLineAry = ppTmpStrLineAry;
			}
			// 分配堆空间
			ppstrLineAry[nCnt] = new char[iLenOfLine + 1];
			if (!ppstrLineAry[nCnt])
			{
				break;
			}
			memset(ppstrLineAry[nCnt], 0, (iLenOfLine + 1) * sizeof(char));
			strncpy_s(ppstrLineAry[nCnt], iLenOfLine + 1, szBuf, iLenOfLine);
			memset(szBuf, 0, LINEMAX * sizeof(char));
			++nCnt;
		} while (!feof(pFile));

		rnSizeOfStringAry = nCnt;
		fOk = TRUE;
	} while (false);

	if (pFile)
	{
		fclose(pFile);
		pFile = nullptr;
	}

	// 失败了需要释放堆空间
	if (!fOk)
	{
		Release2DiAry(ppstrLineAry, nArySize);
		ppstrLineAry = nullptr;
	}

	return(ppstrLineAry);
}


BOOLEAN IO::FileExists(const char *pcszFileName)
{
	FILE *pFile = nullptr;

	if (!pcszFileName)
	{
		return(FALSE);
	}
	// 尝试打开文件
	pFile = fopen(pcszFileName, "rb");
	if (!pFile)
	{
		return(FALSE);
	}
	fclose(pFile);
	pFile = nullptr;

	return(TRUE);
}


void IO::Release2DiAry(PPSTR ppStrAry, size_t nSize)
{
	if (!ppStrAry || !nSize)
	{
		return;
	}
	// 释放指针数组所指向的地址空间
	for (size_t nIdx = 0; nIdx < nSize; ++nIdx)
	{
		if (ppStrAry[nIdx])
		{
			delete[] ppStrAry[nIdx];
			ppStrAry[nIdx] = nullptr;
		}
	}
	delete[] ppStrAry;
	ppStrAry = nullptr;

	return;
}
