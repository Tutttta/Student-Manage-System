#pragma once
/*++

Copyright (c) Ki0pler. All rights reserved.

You may only use this code if you agree to the terms of the GNU General Public License.
If you do not agree to the terms, do not use the code.
Any question about the code. Contact me with the email ki0pler@outlook.com


Module Name:

	CMyString.h
	CMyString.cpp

Abstract:

   This module provides a simple String implementation.

--*/
#include <iostream>

using namespace std;

class CException
{
public:
	CException() { }
	virtual void reason() const = 0;
};

class CExceptionAllocMemFailed : public CException
{
public:
	CExceptionAllocMemFailed() { }
	void reason() const
	{
		cout << "�����ڴ�ʧ��" << endl;
	}
};

class CExceptionOutOfRange : public CException
{
public:
	CExceptionOutOfRange() { }
	void reason() const
	{
		cout << "����Խ��" << endl;
	}
};


class CMyString
{
	friend ostream &operator<<(ostream &os, const CMyString &obj);
public:
	// Ĭ�Ϲ��캯��
	CMyString() : m_sz(nullptr), m_nBufSize(0), m_pnReferenceCount(nullptr) { }
	// ���캯��
	CMyString(const char* sz);
	// �ƶ����캯��
	CMyString(CMyString &&obj);
	// �������캯��
	CMyString(const CMyString& obj);
	// ��������
	~CMyString();

	void Release();

	//��ȡ����
	size_t Len();
	char& At(size_t nIdx);

	//����
	void Copy(const char* sz);
	void Copy(const CMyString& str);

	//ƴ��
	CMyString& Append(const CMyString& str);
	CMyString& Append(const char* sz);
	CMyString& Append(int n);
	CMyString& Append(float f);
	CMyString& Append(double dbl);
	CMyString& Append(short s);

	//����
	int Find(const CMyString& str);
	int Find(const char* sz);
	int ReverseFind(const CMyString& str);
	int ReverseFind(const char* sz);

	//�ִ���ȡ
	CMyString Left(size_t nLen);
	CMyString Right(size_t nLen);
	CMyString Mid(size_t nIdx, size_t nLen);


	static CMyString ValueOf(bool);				// true == > "true"
	static CMyString ValueOf(char c);			// a == > "a"
	static CMyString ValueOf(double d);			// 3.5 == > "3.5"
	static CMyString ValueOf(float f);			// 4.5f == > "4.5"
	static CMyString ValueOf(int i);			// 123 == > "123"
	static CMyString ValueOf(long i);			// 123 == > "123"
	static CMyString ValueOf(short i);			// 123 == > "123"
	static CMyString ValueOf(const char* data); // char[] = "123" == > string "123"
	static CMyString CopyValueOf(const char* data, int offset, int count); // char[] = "123" == > string "123"
	static CMyString StringFromFormat(const char* fmt, ...);  // ("%d", 123) == > string "123"

	//��ʽ��
	void Formate(const char* szFmt, ...);

	inline char *GetString() { return(m_sz); }
	inline size_t GetBufLen() { return(m_nBufSize); }
	inline void Print() { cout << m_sz << endl; }

	// ���������
	char &operator[](int n)
	{
		try
		{
			if (n < 0 || n >(m_nBufSize - 1))
			{
				throw CExceptionOutOfRange();
			}
			return(m_sz[n]);
		}
		catch (const CException &e)
		{
			e.reason();
		}
	}
	bool operator==(const CMyString &obj)
	{
		return(!strcmp(obj.m_sz, m_sz));
	}
	bool operator!=(const CMyString &obj)
	{
		return(!(*this == obj));
	}
	bool operator>(const CMyString &obj)
	{
		return(strcmp(m_sz, obj.m_sz) > 0);
	}
	bool operator<(const CMyString &obj)
	{
		return(!(*this > obj));
	}

	CMyString operator+(const CMyString &obj)
	{
		size_t nLen = 0;

		// �����ֵ�����ݲ�����, ���൱���ͷ�һ���Լ�
		if (!obj.m_sz)
		{
			Release();
			return(*this);
		}

		if (!m_sz)
		{
			// ����Լ��ǲ����ڵ�, ������Ŀ������ü���
			// �����൱�ڿ�������, ��ʽ���������ü���
			return(obj);
		}
		else
		{
			// ������߶�������ֱ��ƴ��
			char *pszNewBuf = new char[obj.m_nBufSize + m_nBufSize + 1];
			if (!pszNewBuf)
			{
				return(CMyString());
			}
			strcpy_s(pszNewBuf, obj.m_nBufSize + m_nBufSize + 1, m_sz);
			strcat_s(pszNewBuf, obj.m_nBufSize + m_nBufSize + 1, obj.m_sz);
			CMyString c;
			c.m_sz = pszNewBuf;
			c.m_nBufSize = obj.m_nBufSize + m_nBufSize + 1;
			c.m_pnReferenceCount = new size_t(1);

			return(c);
		}
	}

	CMyString &operator=(const CMyString &obj)
	{
		if (m_sz)
		{
			// �����ͷ��Լ�һ��
			Release();
		}
		// ָ��������
		m_sz = obj.m_sz;
		m_nBufSize = obj.m_nBufSize;
		m_pnReferenceCount = obj.m_pnReferenceCount;
		(*m_pnReferenceCount)++;

		return(*this);
	}

	CMyString &operator+=(const CMyString &obj)
	{
		*this = *this + obj;

		return(*this);
	}

	

private:
	void Init(const char* sz = nullptr, size_t nLen = 0,
		size_t *pnRef = nullptr, bool fConstrct = true);
public:
	void reverse(char *pszString);

private:
	char*   m_sz;				//�ַ���������
	size_t  m_nBufSize;			//�ڴ��ܴ�С 
	size_t *m_pnReferenceCount;	// ���ü���
};

