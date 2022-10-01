#pragma once
/*++

Copyright (c) Ki0pler. All rights reserved.

You may only use this code if you agree to the terms of the GNU General Public License.
If you do not agree to the terms, do not use the code.
Any question about the code. Contact me with the email ki0pler@outlook.com


Module Name:

	CArray.hpp

Abstract:

   This module provides a template implementation of dynamic arrays.

--*/
template <typename T>
class CArray
{
public:
	// 构造函数
	CArray();
	CArray(const T &a);
	// 拷贝构造函数
	CArray(const CArray &ary);
	// 移动构造函数
	CArray(CArray &&ary);
	// 虚析构函数
	virtual ~CArray();
	// 等号运算符重载
	CArray& operator=(const CArray& ary);
	// 右值等号运算符重载删除
	CArray& operator=(CArray&& ary) = delete;

	CArray& Insert(const T &val, size_t nIdx);		// 指定位置插入
	CArray& InsertHead(const T &val);		// 头部位置插入
	CArray& InsertTail(const T &val);		// 尾部位置插入

	CArray& Delete(size_t nIdx);			// 指定位置删除
	CArray& DeleteHead();					// 头部位置删除
	CArray& DeleteTail();					// 尾部位置删除

	T operator[](size_t nIdx);				// 访问
	void SetValue(size_t nIdx, const T &iVal);		// 修改

	int  Find(const T &val)const;					// 查找

	bool IsEmpty()const;                    // 判空
	void Clear();                           // 清空

	void swap(CArray& x);					// 与目标数组内容交换
	void reverse();							// 反转数组的内容

	void Print();							// 打印
private:
	void CopyAry(const CArray& ary);
	void ReleaseAll(CArray &ary);
	void ReleaseAll();

	void ChkRefCntAndDec();
	void ChkRefCntAndDec(CArray &ary);

	void GiveValue(const CArray &ary);
private:
	T*   m_pBuf = nullptr;					// 元素缓冲区
	size_t m_nSize = 0;						// 元素个数    内容content
	size_t m_nBufSize = 0;					// 缓冲区大小  容量capacity
	size_t *m_pRefCnt = nullptr;			// 引用计数指针
};


template <typename T>
CArray<T>::CArray(const T &a) : m_pBuf(new T[1]{ a }), m_pRefCnt(new size_t(1)), m_nSize(1), m_nBufSize(1)
{
	if (!m_pBuf || !m_pRefCnt)
	{
		m_nBufSize = m_nSize = 0;
		if (m_pRefCnt)
		{
			delete m_pRefCnt;
			m_pRefCnt = nullptr;
		}
		if (m_pBuf)
		{
			delete m_pBuf;
			m_pBuf = nullptr;
		}
	}
}

template <typename T>
CArray<T>::CArray() :
	m_pBuf(new T[1]), m_pRefCnt(new size_t(1)), m_nBufSize(1), m_nSize(1)
{
	if (!m_pBuf || !m_pRefCnt)
	{
		m_nBufSize = m_nSize = 0;
		if (m_pRefCnt)
		{
			delete m_pRefCnt;
			m_pRefCnt = nullptr;
		}
		if (m_pBuf)
		{
			delete m_pBuf;
			m_pBuf = nullptr;
		}
	}
}

template <typename T>
CArray<T>::CArray(const CArray<T> &ary)
{
	CopyAry(ary);
}

template <typename T>
void CArray<T>::CopyAry(const CArray<T> & ary)
{
	if (ary.m_pBuf && *(ary.m_pRefCnt) >= 1 && ary.m_nBufSize > 0)
	{
		GiveValue(ary);
		// 增加引用计数
		++(*m_pRefCnt);
	}
}

template <typename T>
CArray<T>::CArray(CArray<T> &&ary)
{
	ChkRefCntAndDec();
	// 进行移动
	GiveValue(ary);
	// 清空自身
	ary.m_pBuf = nullptr;
	ary.m_pRefCnt = nullptr;
	ary.m_nSize = ary.m_nBufSize = 0;
}

template <typename T>
CArray<T> & CArray<T>::operator=(const CArray<T> &ary)
{
	// TODO: 在此处插入 return 语句
	// 把自身引用计数降低或者释放
	if (!ary.m_nBufSize || !ary.m_pBuf || !ary.m_pRefCnt)
	{
		return(*this);
	}
	// 降低自身引用或者删除
	ChkRefCntAndDec();
	// 拷贝过来
	GiveValue(ary);
	// 增加自身引用计数
	++(*m_pRefCnt);
}

template <typename T>
void CArray<T>::ChkRefCntAndDec()
{
	if (m_pBuf && m_pRefCnt && *m_pRefCnt >= 1 && m_nBufSize > 0)
	{
		if (*m_pRefCnt > 1)
		{
			// 如果有只有2个对象在引用则直接减少引用计数
			--(*m_pRefCnt);
			return;
		}
	}
	// 全部释放
	ReleaseAll();
}

template <typename T>
void CArray<T>::ChkRefCntAndDec(CArray<T> &ary)
{
	if (ary.m_pBuf && ary.m_pRefCnt && *(ary.m_pRefCnt) >= 1 && ary.m_nBufSize > 0)
	{
		if (*(ary.m_pRefCnt) > 1)
		{
			// 如果有只有2个对象在引用则直接减少引用计数
			--(*ary.m_pRefCnt);
			return;
		}
	}
	// 全部释放
	ReleaseAll(ary);
}

template <typename T>
void CArray<T>::GiveValue(const CArray<T> &ary)
{
	m_pBuf = ary.m_pBuf;
	m_pRefCnt = ary.m_pRefCnt;
	m_nSize = ary.m_nSize;
	m_nBufSize = ary.m_nBufSize;
}

template <typename T>
void CArray<T>::ReleaseAll()
{
	if (m_pBuf)
	{
		delete[] m_pBuf;
		m_pBuf = nullptr;
	}
	if (m_pRefCnt)
	{
		delete m_pRefCnt;
		m_pRefCnt = nullptr;
	}
	m_nSize = m_nBufSize = 0;
}

template <typename T>
void CArray<T>::ReleaseAll(CArray<T> &ary)
{
	if (ary.m_pBuf)
	{
		delete[] ary.m_pBuf;
		ary.m_pBuf = nullptr;
	}
	if (ary.m_pRefCnt)
	{
		delete ary.m_pRefCnt;
		ary.m_pRefCnt = nullptr;
	}
	ary.m_nSize = ary.m_nBufSize = 0;
}

template <typename T>
CArray<T>::~CArray()
{
	ChkRefCntAndDec();
}

template <typename T>
CArray<T> &CArray<T>::Insert(const T &val, size_t nIdx)
{
	T *pArr = nullptr;
	size_t nNewSize = m_nBufSize;
	size_t nTmpSize = m_nSize;

	// TODO: 在此处插入 return 语句
	if (!m_nBufSize || !m_pBuf || !m_pRefCnt || nIdx > m_nSize)
	{
		return(*this);
	}
	// 空间不够的情况
	if (m_nSize >= m_nBufSize)
	{
		nNewSize = m_nBufSize * 2;
	}
	// 分配新的空间
	pArr = new T[nNewSize];
	if (!pArr)
	{
		return(*this);
	}
	memset(pArr, 0, sizeof(m_pBuf[0]) * nNewSize);
	// 防止浅拷贝, 重新构造
	T *tmp = new T[m_nBufSize];
	memset(tmp, 0, sizeof(m_pBuf[0]) * m_nBufSize);
	for (int i = 0; i < m_nSize; ++i)
	{
		tmp[i] = m_pBuf[i];
	}
	memcpy(pArr, tmp, sizeof(tmp[0]) * m_nSize);
	// 减少之前的引用
	ChkRefCntAndDec();
	// 更新缓冲区和新的缓冲区大小
	m_pBuf = pArr;
	m_nBufSize = nNewSize;
	m_nSize = nTmpSize;
	m_pRefCnt = new size_t(1);

	for (size_t i = m_nSize; i > nIdx; --i)
	{
		m_pBuf[i] = m_pBuf[i - 1];
	}
	m_pBuf[nIdx] = val;

	++m_nSize;

	return(*this);
}

template <typename T>
CArray<T> & CArray<T>::InsertHead(const T &val)
{
	// TODO: 在此处插入 return 语句
	return(Insert(val, 0));
}

template <typename T>
CArray<T> & CArray<T>::InsertTail(const T &val)
{
	// TODO: 在此处插入 return 语句
	return(Insert(val, m_nSize));
}

template <typename T>
CArray<T> &CArray<T>::Delete(size_t nIdx)
{
	T *pArr = nullptr;
	size_t nTmpSize = m_nSize;
	size_t nTmpBufSize = m_nBufSize;

	// TODO: 在此处插入 return 语句
	if (!m_nBufSize || !m_pBuf ||
		!m_pRefCnt || !m_nSize || nIdx > m_nSize - 1)
	{
		return(*this);
	}
	pArr = new T[m_nBufSize];
	if (!pArr)
	{
		return(*this);
	}
	// m_nBufSize保持不变即可
	memset(pArr, 0, sizeof(m_pBuf[0]) * m_nBufSize);
	// 防止浅拷贝, 重新构造
	T *tmp = new T[m_nBufSize];
	memset(tmp, 0, sizeof(m_pBuf[0]) * m_nBufSize);
	for (int i = 0; i < m_nSize; ++i)
	{
		tmp[i] = m_pBuf[i];
	}
	memcpy(pArr, tmp, sizeof(tmp[0]) * m_nSize);
	// 降低自身引用或者删除自身
	ChkRefCntAndDec();
	// 恢复之前的内容
	m_nSize = nTmpSize;
	m_nBufSize = nTmpBufSize;
	m_pBuf = pArr;
	m_pRefCnt = new size_t(1);
	// 重新分配内存, 少一个
	T *tmp1 = new T[m_nBufSize];
	memset(tmp1, 0, sizeof(m_pBuf[0]) * m_nBufSize);
	for (int i = 0, k = 0; i < m_nSize; ++i)
	{
		if (i == nIdx)
		{
			continue;
		}
		tmp1[k++] = m_pBuf[i];
	}
	delete[] m_pBuf;
	m_pBuf = tmp1;
	--m_nSize;

	return(*this);
}

template <typename T>
CArray<T> & CArray<T>::DeleteHead()
{
	// TODO: 在此处插入 return 语句
	return(Delete(0));
}

template <typename T>
CArray<T> & CArray<T>::DeleteTail()
{
	// TODO: 在此处插入 return 语句
	return(Delete(m_nSize - 1));
}

template <typename T>
T CArray<T>::operator[](size_t nIdx)
{
	// TODO: 在此处插入 return 语句
	return(m_pBuf[nIdx]);
}

template <typename T>
void CArray<T>::SetValue(size_t nIdx, const T &iVal)
{
	T *pArr = nullptr;
	size_t nTmpBufSize = m_nBufSize;
	size_t nTmpSize = m_nSize;
	pArr = new T[m_nBufSize];
	if (!pArr)
	{
		return;
	}
	memset(pArr, 0, sizeof(pArr[0]) * m_nBufSize);
	memcpy(pArr, m_pBuf, sizeof(pArr[0]) * m_nSize);
	pArr[nIdx] = iVal;
	ChkRefCntAndDec();

	m_nBufSize = nTmpBufSize;
	m_nSize = nTmpSize;
	m_pRefCnt = new size_t(1);
	m_pBuf = pArr;
}

template <typename T>
int CArray<T>::Find(const T &val) const
{
	for (int i = 0; i < m_nSize; ++i)
	{
		if (static_cast<T>(val) == static_cast<T>(m_pBuf[i]))
		{
			return(i);
		}
	}
	return(-1);
}

template <typename T>
bool CArray<T>::IsEmpty() const
{
	return(!m_nSize || !m_pBuf);
}

template <typename T>
void CArray<T>::Clear()
{
	ReleaseAll();
}

template <typename T>
void CArray<T>::swap(CArray<T> &x)
{
	CArray<T> cTmp = x;
	*this = x;
	x = cTmp;
}

template <typename T>
void CArray<T>::reverse()
{
	T iTmp = 0;
	size_t iTmpSize = m_nSize;
	size_t iTmpBufSize = m_nBufSize;

	T *pArr = new T[m_nBufSize];
	if (!pArr)
	{
		return;
	}
	memset(pArr, 0, sizeof(pArr[0]) * m_nBufSize);
	memcpy(pArr, m_pBuf, sizeof(pArr[0]) * m_nSize);
	// 降低自身的引用或删除
	ChkRefCntAndDec();

	m_pBuf = pArr;
	m_nSize = iTmpSize;
	m_nBufSize = iTmpBufSize;
	m_pRefCnt = new size_t(1);

	for (int i = m_nSize - 1, j = 0; j < i; --i, ++j)
	{
		iTmp = m_pBuf[i];
		m_pBuf[i] = m_pBuf[j];
		m_pBuf[j] = iTmp;
	}
}

template <typename T>
void CArray<T>::Print()
{
	if (!m_pBuf)
	{
		return;
	}
	for (int i = 0; i < m_nSize; ++i)
	{
		cout << m_pBuf[i] << " ";
	}
	cout << endl;
}


