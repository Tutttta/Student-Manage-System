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
		// �Զ�ȡ��ʽ���ļ�
		pFile = fopen(pcszFileName, "rb");
		if (!pFile)
		{
			break;
		}
		// ��ȡ�ļ���С
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
		// �Զ�ȡ��ʽ���ļ�
		pFile = fopen(pcszFileName, "rb");
		if (!pFile)
		{
			break;
		}
		// �����ά����
		ppstrLineAry = new PSTR[nArySize];
		if (!ppstrLineAry)
		{
			break;
		}
		
		do
		{
			pRet = GetsEx(szBuf, sizeof(szBuf), pFile);
			// ������˴�
			if (!pRet)
			{
				break;
			}
			int iLenOfLine = 0;
			// ��ȡ��ȡ�еĳ���
			iLenOfLine = strlen(szBuf);
			// ����ռ䲻����Ҫ��̬���¿ռ�
			if (nCnt >= nArySize)
			{
				nArySize += INCREMENT;
				// �����µ���ʱ�ռ�
				ppTmpStrLineAry = new PSTR[nArySize];
				if (!ppTmpStrLineAry)
				{
					break;
				}
				memset(ppTmpStrLineAry, 0, nArySize * sizeof(ppTmpStrLineAry[0]));
				memcpy(ppTmpStrLineAry, ppstrLineAry, (nArySize - INCREMENT) * sizeof(ppstrLineAry[0]));
				ppstrLineAry = ppTmpStrLineAry;
			}
			// ����ѿռ�
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

	// ʧ������Ҫ�ͷŶѿռ�
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
	// ���Դ��ļ�
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
	// �ͷ�ָ��������ָ��ĵ�ַ�ռ�
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
