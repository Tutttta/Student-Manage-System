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
		cout << "分配内存失败" << endl;
	}
};

class CExceptionOutOfRange : public CException
{
public:
	CExceptionOutOfRange() { }
	void reason() const
	{
		cout << "索引越界" << endl;
	}
};


class CMyString
{
	friend ostream &operator<<(ostream &os, const CMyString &obj);
public:
	// 默认构造函数
	CMyString() : m_sz(nullptr), m_nBufSize(0), m_pnReferenceCount(nullptr) { }
	// 构造函数
	CMyString(const char* sz);
	// 移动构造函数
	CMyString(CMyString &&obj);
	// 拷贝构造函数
	CMyString(const CMyString& obj);
	// 析构函数
	~CMyString();

	void Release();

	//获取长度
	size_t Len();
	char& At(size_t nIdx);

	//拷贝
	void Copy(const char* sz);
	void Copy(const CMyString& str);

	//拼接
	CMyString& Append(const CMyString& str);
	CMyString& Append(const char* sz);
	CMyString& Append(int n);
	CMyString& Append(float f);
	CMyString& Append(double dbl);
	CMyString& Append(short s);

	//查找
	int Find(const CMyString& str);
	int Find(const char* sz);
	int ReverseFind(const CMyString& str);
	int ReverseFind(const char* sz);

	//字串提取
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

	//格式化
	void Formate(const char* szFmt, ...);

	inline char *GetString() { return(m_sz); }
	inline size_t GetBufLen() { return(m_nBufSize); }
	inline void Print() { cout << m_sz << endl; }

	// 运算符重载
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

		// 如果赋值的内容不存在, 则相当于释放一次自己
		if (!obj.m_sz)
		{
			Release();
			return(*this);
		}

		if (!m_sz)
		{
			// 如果自己是不存在的, 则增加目标的引用计数
			// 这里相当于拷贝构造, 隐式增加了引用计数
			return(obj);
		}
		else
		{
			// 如果两者都存在则直接拼接
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
			// 首先释放自己一次
			Release();
		}
		// 指向新内容
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
	char*   m_sz;				//字符串缓冲区
	size_t  m_nBufSize;			//内存总大小 
	size_t *m_pnReferenceCount;	// 引用计数
};

