#define _CRT_SECURE_NO_WARNINGS
#include "CMyString.h"
#include <stdarg.h>

void CMyString::Init(const char* sz, size_t nLen, size_t *pnRef, bool fConstrct)
{
	if (!sz || (!fConstrct && !pnRef))
	{
		m_sz = nullptr;
		m_pnReferenceCount = nullptr;
		m_nBufSize = 0;
	}
	else
	{
		if (fConstrct)
		{
			// 由于是buffer的长度, 所以算上最后的空字符
			m_nBufSize = nLen + 1;
			try
			{
				m_sz = new char[m_nBufSize];
				if (nullptr == m_sz)
				{
					m_nBufSize = 0;
					m_pnReferenceCount = nullptr;
					throw CExceptionAllocMemFailed();
				}
				else
				{
					strncpy_s(m_sz, m_nBufSize, sz, m_nBufSize);
				}
				m_pnReferenceCount = new size_t(1);
				if (!m_pnReferenceCount)
				{
					delete[] m_sz;
					m_sz = nullptr;
					throw CExceptionAllocMemFailed();
				}
			}
			catch (const CException &e)
			{
				e.reason();
			}
		}
		else
		{
			m_nBufSize = nLen;
			m_sz = (char *)sz;
			m_pnReferenceCount = pnRef;
			++(*m_pnReferenceCount);
		}
	}
}

void CMyString::reverse(char *pszString)
{
	int iLen = 0;
	int iPtr0 = 0, iPtr1 = 0;

	if (nullptr == pszString)
	{
		return;
	}

	iLen = strlen(pszString);
	iPtr1 = iLen - 1;

	while (iPtr0 < iPtr1)
	{
		pszString[iPtr0] ^= pszString[iPtr1];
		pszString[iPtr1] = pszString[iPtr0] ^ pszString[iPtr1];
		pszString[iPtr0] ^= pszString[iPtr1];
		--iPtr1;
		++iPtr0;
	}
}

CMyString::CMyString(const char* sz /*= nullptr*/)
{
	Init(sz, (!sz ? 0 : strlen(sz)));
}


CMyString::CMyString(const CMyString& obj)
{
	Init(obj.m_sz, obj.m_nBufSize, obj.m_pnReferenceCount, false);
}

CMyString::CMyString(CMyString &&obj)
{
	m_sz = obj.m_sz;
	m_nBufSize = obj.m_nBufSize;
	m_pnReferenceCount = obj.m_pnReferenceCount;

	obj.m_sz = nullptr;
	obj.m_nBufSize = 0;
	obj.m_pnReferenceCount = nullptr;
}

CMyString::~CMyString()
{
	Release();
}

void CMyString::Release()
{
	if (m_pnReferenceCount)
	{
		if (*m_pnReferenceCount != 1)
		{
			--(*m_pnReferenceCount);
			return;
		}
	}
	if (m_pnReferenceCount)
	{
		delete m_pnReferenceCount;
		m_pnReferenceCount = nullptr;
	}
	if (m_sz)
	{
		delete[] m_sz;
		m_sz = nullptr;
	}
	m_nBufSize = 0;
}

size_t CMyString::Len()
{
	return(strlen(m_sz));
}

char& CMyString::At(size_t nIdx)
{
	return(m_sz[nIdx]);
}

void CMyString::Copy(const char* sz)
{
	int iLen = 0;
	char *pTmp = nullptr;

	if (!m_sz || !sz)
	{
		return;
	}
	iLen = strlen(sz);
	// 容量够的情况
	if (m_nBufSize > iLen)
	{
		memset(m_sz, 0, m_nBufSize);
		strncpy_s(m_sz, m_nBufSize, sz, iLen);
	}
	else
	{
		// 容量不够需要释放重新分配堆空间
		pTmp = new char[iLen + 1];
		if (!pTmp)
		{
			return;
		}
		delete[] m_sz;
		m_sz = nullptr;
		m_sz = pTmp;
		m_nBufSize = iLen + 1;
		strncpy_s(m_sz, m_nBufSize, sz, iLen + 1);
	}
}

void CMyString::Copy(const CMyString& str)
{
	Copy(str.m_sz);
}

CMyString& CMyString::Append(const CMyString& str)
{
	this->Append(str.m_sz);
	return(*this);
}

CMyString& CMyString::Append(const char* sz)
{
	char *pTmp = nullptr;
	size_t nLen = 0;
	size_t nOriginLen = 0;

	if (!m_sz || !sz)
	{
		return(*this);
	}

	nLen = strlen(sz);
	nOriginLen = strlen(m_sz);
	if ((nLen + nOriginLen) < m_nBufSize)
	{
		// 长度足够无需分配空间
		strncpy_s(m_sz + nOriginLen, m_nBufSize - nOriginLen, sz, nLen + 1);
	}
	else
	{
		pTmp = new char[nLen + nOriginLen + 1];
		if (nullptr == pTmp)
		{
			return(*this);
		}
		strncpy_s(pTmp, nLen + nOriginLen + 1, m_sz, m_nBufSize);
		delete[] m_sz;
		m_sz = nullptr;
		strncpy_s(pTmp + nOriginLen, nLen + 1, sz, nLen + 1);
		m_sz = pTmp;
		m_nBufSize = nLen + nOriginLen + 1;
	}

	return(*this);
}

CMyString& CMyString::Append(int n)
{
	char szNum[32] = { 0 };
	sprintf(szNum, "%d", n);

	return(Append(szNum));
}

CMyString& CMyString::Append(float f)
{
	char szNum[32] = { 0 };
	sprintf(szNum, "%f", f);

	return(Append(szNum));
}

CMyString& CMyString::Append(double dbl)
{
	char szNum[32] = { 0 };
	sprintf(szNum, "%lf", dbl);

	return(Append(szNum));
}

CMyString& CMyString::Append(short s)
{
	char szNum[32] = { 0 };
	sprintf(szNum, "%hd", s);

	return(Append(szNum));
}

int CMyString::Find(const char* sz)
{
	int iPtr0 = 0, iPtr1 = 0;

	if (!sz || !m_sz)
	{
		return(-1);
	}

	for (iPtr0 = 0; m_sz[iPtr0] && sz[iPtr1]; ++iPtr0)
	{
		if (m_sz[iPtr0] == sz[iPtr1])
		{
			++iPtr1;
		}
		else
		{
			iPtr0 = iPtr0 - iPtr1;
			iPtr1 = 0;
		}
	}
	if (!sz[iPtr1] && sz[0])
	{
		return(iPtr0 - iPtr1);
	}

	return(-1);
}

int CMyString::Find(const CMyString& str)
{
	return(Find(str.m_sz));
}

int CMyString::ReverseFind(const CMyString& str)
{
	return(ReverseFind(str.m_sz));
}

int CMyString::ReverseFind(const char* sz)
{
	int iIdx = 0;
	int iLen = 0;
	char *pBuf = nullptr;

	if (!sz || !m_sz)
	{
		return(-1);
	}
	iLen = strlen(sz);
	pBuf = new char[iLen + 1];
	strncpy_s(pBuf, iLen + 1, sz, iLen + 1);
	reverse(pBuf);
	reverse(this->m_sz);
	iIdx = this->Find(pBuf);
	if (-1 != iIdx)
	{
		iIdx = Len() - iIdx - iLen + 1;
	}
	reverse(pBuf);
	reverse(this->m_sz);
	delete[] pBuf;
	pBuf = nullptr;

	return(iIdx);
}

CMyString CMyString::Left(size_t nLen)
{
	char *pszBuf = nullptr;

	if (!nLen)
	{
		return(CMyString{ "" });
	}
	pszBuf = new char[nLen + 1];
	if (!pszBuf)
	{
		return(CMyString{ nullptr });
	}
	strncpy_s(pszBuf, nLen + 1, m_sz, nLen);
	pszBuf[nLen] = '\0';

	return(CMyString(pszBuf));
}

CMyString CMyString::Right(size_t nLen)
{
	char *pszBuf = nullptr;

	if (!nLen)
	{
		return(CMyString{ "" });
	}
	pszBuf = new char[nLen + 1];
	if (!pszBuf)
	{
		return(CMyString{ nullptr });
	}
	strncpy_s(pszBuf, nLen + 1, m_sz + Len() - nLen, nLen + 1);

	return(CMyString(pszBuf));
}

CMyString CMyString::Mid(size_t nIdx, size_t nLen)
{
	char *pszBuf = nullptr;

	if (!nLen)
	{
		return(CMyString{ "" });
	}
	pszBuf = new char[nLen + 1];
	if (!pszBuf)
	{
		return(CMyString{ nullptr });
	}
	strncpy_s(pszBuf, nLen + 1, m_sz + nIdx, nLen);
	pszBuf[nLen] = '\0';

	return(CMyString(pszBuf));
}

CMyString CMyString::ValueOf(bool f)
{
	if (f)
	{
		return(CMyString("true"));
	}
	else
	{
		return(CMyString("false"));
	}
}

CMyString CMyString::ValueOf(char c)
{
	char szBuf[2] = { 0 };

	szBuf[0] = c;
	return(CMyString(szBuf));
}

CMyString CMyString::ValueOf(double d)
{
	char szBuf[128] = { 0 };

	sprintf(szBuf, "%lf", d);

	return(CMyString(szBuf));
}

CMyString CMyString::ValueOf(float f)
{
	char szBuf[128] = { 0 };

	sprintf(szBuf, "%f", f);

	return(CMyString(szBuf));
}

CMyString CMyString::ValueOf(int i)
{
	char szBuf[128] = { 0 };

	sprintf(szBuf, "%d", i);

	return(CMyString(szBuf));
}

CMyString CMyString::ValueOf(long i)
{
	char szBuf[128] = { 0 };

	sprintf(szBuf, "%ld", i);

	return(CMyString(szBuf));
}

CMyString CMyString::ValueOf(short i)
{
	char szBuf[128] = { 0 };

	sprintf(szBuf, "%hd", i);

	return(CMyString(szBuf));
}

CMyString CMyString::ValueOf(const char* data)
{
	return(CMyString(data));
}

CMyString CMyString::CopyValueOf(const char* data, int offset, int count)
{
	char *pszBuf = nullptr;


	pszBuf = new char[count + 1];
	if (!pszBuf)
	{
		return(CMyString(""));
	}
	strncpy_s(pszBuf, count + 1, data + offset, count);
	CMyString myStr(pszBuf);
	delete[] pszBuf;
	pszBuf = nullptr;

	return(myStr);
}

CMyString CMyString::StringFromFormat(const char* fmt, ...)
{
	va_list vlist;
	char *szBuf = nullptr;

	va_start(vlist, fmt);
	size_t nLen = _vscprintf(fmt, vlist);
	szBuf = new char[nLen + 1];
	vsprintf(szBuf, fmt, vlist);
	va_end(vlist);

	return(CMyString(szBuf));
}

void CMyString::Formate(const char* szFmt, ...)
{
	va_list vlist;

	va_start(vlist, szFmt);
	vprintf(szFmt, vlist);
	va_end(vlist);
}

ostream &operator<<(ostream &os, const CMyString &obj)
{
	os << obj.m_sz;

	return(os);
}