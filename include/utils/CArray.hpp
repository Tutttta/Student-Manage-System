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
	// ���캯��
	CArray();
	// ��������
	~CArray();
	// ������Ⱥ����������
	CArray& operator=(const CArray& ary) = delete;
	// ��������ֵ�Ⱥ����������
	CArray& operator=(CArray&& ary) = delete;
	// �������캯��
	CArray(const CArray &ary) = delete;

	bool Insert(const T &val, size_t nIdx);	// ָ��λ�ò���
	bool InsertHead(const T &val);			// ͷ��λ�ò���
	bool InsertTail(const T &val);			// β��λ�ò���

	bool Delete(size_t nIdx, T &val);		// ָ��λ��ɾ��
	bool DeleteHead(T &val);				// ͷ��λ��ɾ��
	bool DeleteTail(T &val);				// β��λ��ɾ��

	bool Delete(size_t nIdx);
	bool DeleteHead();
	bool DeleteTail();

	bool DeleteByID(size_t nID);			// ͨ��IDɾ����Ϣ

	T &operator[](size_t nIdx);					// ����
	void SetValue(size_t nIdx, const T &iVal);	// �޸�
	T *GetBufPtr() const { return(m_pBuf); }
	size_t GetSize() const { return(m_nSize); } // �������д�С����

	int  Find(const T &val)const;				// ����
	bool FindStuByID(size_t nID) const;			// ͨ��ID����ѧ����Ϣ
	bool FindStuID(size_t nHash, size_t &rnID); // ͨ��Hashֵ����ѧ��ID

	bool IsEmpty()const;                    // �п�
	void Clear();                           // ���

	void swap(CArray& x);					// ��Ŀ���������ݽ���
	void reverse();							// ��ת���������
private:
	void ReleaseAll(CArray &ary);
	void ReleaseAll();

	void GiveValue(const CArray &ary);

	bool IncBufSize(size_t nIncSize = INCREMENT);
private:
	T*   m_pBuf = nullptr;					// Ԫ�ػ�����
	size_t m_nSize = 0;						// Ԫ�ظ���    ����content
	size_t m_nBufSize = 0;					// ��������С  ����capacity
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
	// ���ֲ��ҷ�
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

	// ����������Ҫ��������
	if (m_nBufSize > m_nSize)
	{
		return(true);
	}
	// �������˵����, ��������
	pTmpAry = new T[m_nBufSize + nIncSize];
	if (!pTmpAry)
	{
		// ����ʧ��
		return(false);
	}
	memset(pTmpAry, 0, sizeof(pTmpAry[0]) * (m_nBufSize + nIncSize));
	if (!m_pBuf)
	{
		return(false);
	}
	memcpy(pTmpAry, m_pBuf, sizeof(T) * m_nSize);
	// ���ӵ�ǰ����
	m_nBufSize += nIncSize;
	// ָ���µĿռ�
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
	// ������˾Ͷ�̬�����ڴ�
	if (!IncBufSize())
	{
		return(false);
	}
	// �������λ�ô��ڵ�ǰ��Ԫ������, ��Ĭ�ϲ���ĩβ
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
	// TODO: �ڴ˴����� return ���
	return(Insert(val, 0));
}

template <typename T>
bool CArray<T>::InsertTail(const T &val)
{
	// TODO: �ڴ˴����� return ���
	return(Insert(val, m_nSize));
}


template <typename T>
bool CArray<T>::Delete(size_t nIdx, T &val)
{
	// ������û��Ԫ�ػ���nIdxָ���λ�ó�������Ԫ�صķ�Χ
	if (nIdx >= m_nSize || m_nSize < 1)
	{
		return(false);
	}
	// ɾ������ĩβԪ��ֱ��ɾ
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
	// TODO: �ڴ˴����� return ���
	return(Delete(0, val));
}

template <typename T>
bool CArray<T>::DeleteTail(T &val)
{
	// TODO: �ڴ˴����� return ���
	return(Delete(m_nSize - 1, val));
}

template <typename T>
T &CArray<T>::operator[](size_t nIdx)
{
	// TODO: �ڴ˴����� return ���
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
	// ������������û�ɾ��
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


