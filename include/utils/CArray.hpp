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
#define BASESIZE 1000
#define INCREMENT 500
public:
	// 构造函数
	CArray();
	// 析构函数
	~CArray();
	// 不允许等号运算符重载
	CArray& operator=(const CArray& ary) = delete;
	// 不允许右值等号运算符重载
	CArray& operator=(CArray&& ary) = delete;
	// 不允许构造函数
	CArray(const CArray &ary) = delete;

	bool Insert(const T &val, size_t nIdx);	// 指定位置插入
	bool InsertHead(const T &val);			// 头部位置插入
	bool InsertTail(const T &val);			// 尾部位置插入

	bool Delete(size_t nIdx, T &val);		// 指定位置删除
	bool DeleteHead(T &val);				// 头部位置删除
	bool DeleteTail(T &val);				// 尾部位置删除

	bool Delete(size_t nIdx);
	bool DeleteHead();
	bool DeleteTail();

	bool DeleteByID(size_t nID);			// 通过ID删除信息

	T &operator[](size_t nIdx);					// 访问
	void SetValue(size_t nIdx, const T &iVal);	// 修改
	T *GetBufPtr() const { return(m_pBuf); }
	size_t GetSize() const { return(m_nSize); } // 返回现有大小容量

	int  Find(const T &val)const;				// 查找
	bool FindStuByID(size_t nID) const;			// 通过ID查找学生信息
	bool FindStuID(size_t nHash, size_t &rnID); // 通过Hash值查找学生ID

	bool IsEmpty()const;                    // 判空
	void Clear();                           // 清空

	void swap(CArray& x);					// 与目标数组内容交换
	void reverse();							// 反转数组的内容
private:
	void ReleaseAll(CArray &ary);
	void ReleaseAll();

	void GiveValue(const CArray &ary);

	bool IncBufSize(size_t nIncSize = INCREMENT);
private:
	T*   m_pBuf = nullptr;					// 元素缓冲区
	size_t m_nSize = 0;						// 元素个数    内容content
	size_t m_nBufSize = 0;					// 缓冲区大小  容量capacity
};

template <typename T>
bool CArray<T>::FindStuByID(size_t nID) const
{

	return(true);
}

template <typename T>
bool CArray<T>::FindStuID(size_t nHash, size_t &rnID)
{
	pstSearchIDByStudentName pSrhStuInfo = nullptr;
	pstSearchIDByStudentName pObjInfo = nullptr;
	bool fOk = false;

	if (!nHash)
	{
		return(false);
	}
	pSrhStuInfo = m_pBuf;
	// 二分查找法
	pObjInfo = BinSrhStuName(pSrhStuInfo, m_nSize, nHash);
	if (pObjInfo)
	{
		rnID = pObjInfo->uiStudentID;
		fOk = true;
	}

	return(fOk);
}

template <typename T>
bool CArray<T>::DeleteTail()
{
	T data;
	return(DeleteTail(data));
}

template <typename T>
bool CArray<T>::DeleteHead()
{
	T data;
	return(DeleteHead(data));
}

template <typename T>
bool CArray<T>::IncBufSize(size_t nIncSize/* = INCREMENT*/)
{
	T *pTmpAry = nullptr;

	// 容量够不需要增加内容
	if (m_nBufSize > m_nSize)
	{
		return(true);
	}
	// 容量满了的情况, 进行扩容
	pTmpAry = new T[m_nBufSize + nIncSize];
	if (!pTmpAry)
	{
		// 扩容失败
		return(false);
	}
	memset(pTmpAry, 0, sizeof(pTmpAry[0]) * (m_nBufSize + nIncSize));
	if (!m_pBuf)
	{
		return(false);
	}
	memcpy(pTmpAry, m_pBuf, sizeof(T) * m_nSize);
	// 增加当前容量
	m_nBufSize += nIncSize;
	// 指向新的空间
	m_pBuf = pTmpAry;

	return(true);
}

template <typename T>
CArray<T>::CArray() :
	m_pBuf(new T[BASESIZE]), m_nBufSize(BASESIZE), m_nSize(0)
{
	if (!m_pBuf)
	{
		m_nBufSize = m_nSize = 0;
		return;
	}
	memset(m_pBuf, 0, sizeof(T) * BASESIZE);
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
	ary.m_nSize = ary.m_nBufSize = 0;
}

template <typename T>
CArray<T>::~CArray()
{
	ReleaseAll();
}

template <typename T>
bool CArray<T>::Insert(const T &val, size_t nIdx)
{
	// 如果满了就动态增加内存
	if (!IncBufSize())
	{
		return(false);
	}
	// 如果插入位置大于当前总元素数量, 则默认插在末尾
	if (nIdx > m_nSize)
	{
		nIdx = m_nSize;
	}
	for (size_t nIndex = m_nSize; nIndex > nIdx; ++nIndex)
	{
		m_pBuf[nIndex] = m_pBuf[nIndex - 1];
	}
	m_pBuf[nIdx] = val;
	++m_nSize;

	return(true);
}

template <typename T>
bool CArray<T>::InsertHead(const T &val)
{
	// TODO: 在此处插入 return 语句
	return(Insert(val, 0));
}

template <typename T>
bool CArray<T>::InsertTail(const T &val)
{
	// TODO: 在此处插入 return 语句
	return(Insert(val, m_nSize));
}


template <typename T>
bool CArray<T>::Delete(size_t nIdx, T &val)
{
	// 数组内没有元素或者nIdx指向的位置超过存在元素的范围
	if (nIdx >= m_nSize || m_nSize < 1)
	{
		return(false);
	}
	// 删除数组末尾元素直接删
	if (nIdx == (m_nSize - 1))
	{
		val = m_pBuf[nIdx];
		m_pBuf[nIdx] = 0;
	}
	else
	{
		for (size_t nIndex = nIdx; nIndex < (m_nSize - 1); ++nIndex)
		{
			m_pBuf[nIndex] = m_pBuf[nIndex + 1];
		}
		m_pBuf[m_nSize - 1] = 0;
		
	}
	--m_nSize;

	return(true);
}


template <typename T>
bool CArray<T>::DeleteByID(size_t nID)
{
	Find()
}


template <typename T>
bool CArray<T>::Delete(size_t nIdx)
{
	T data;
	return(Delete(nIdx, data));
}

template <typename T>
bool CArray<T>::DeleteHead(T &val)
{
	// TODO: 在此处插入 return 语句
	return(Delete(0, val));
}

template <typename T>
bool CArray<T>::DeleteTail(T &val)
{
	// TODO: 在此处插入 return 语句
	return(Delete(m_nSize - 1, val));
}

template <typename T>
T &CArray<T>::operator[](size_t nIdx)
{
	// TODO: 在此处插入 return 语句
	return(m_pBuf[nIdx]);
}

template <typename T>
void CArray<T>::SetValue(size_t nIdx, const T &iVal)
{
	
	return;
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


