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
	// ���캯��
	CArray();
	CArray(const T &a);
	// �������캯��
	CArray(const CArray &ary);
	// �ƶ����캯��
	CArray(CArray &&ary);
	// ����������
	virtual ~CArray();
	// �Ⱥ����������
	CArray& operator=(const CArray& ary);
	// ��ֵ�Ⱥ����������ɾ��
	CArray& operator=(CArray&& ary) = delete;

	CArray& Insert(const T &val, size_t nIdx);		// ָ��λ�ò���
	CArray& InsertHead(const T &val);		// ͷ��λ�ò���
	CArray& InsertTail(const T &val);		// β��λ�ò���

	CArray& Delete(size_t nIdx);			// ָ��λ��ɾ��
	CArray& DeleteHead();					// ͷ��λ��ɾ��
	CArray& DeleteTail();					// β��λ��ɾ��

	T operator[](size_t nIdx);				// ����
	void SetValue(size_t nIdx, const T &iVal);		// �޸�

	int  Find(const T &val)const;					// ����

	bool IsEmpty()const;                    // �п�
	void Clear();                           // ���

	void swap(CArray& x);					// ��Ŀ���������ݽ���
	void reverse();							// ��ת���������

	void Print();							// ��ӡ
private:
	void CopyAry(const CArray& ary);
	void ReleaseAll(CArray &ary);
	void ReleaseAll();

	void ChkRefCntAndDec();
	void ChkRefCntAndDec(CArray &ary);

	void GiveValue(const CArray &ary);
private:
	T*   m_pBuf = nullptr;					// Ԫ�ػ�����
	size_t m_nSize = 0;						// Ԫ�ظ���    ����content
	size_t m_nBufSize = 0;					// ��������С  ����capacity
	size_t *m_pRefCnt = nullptr;			// ���ü���ָ��
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
		// �������ü���
		++(*m_pRefCnt);
	}
}

template <typename T>
CArray<T>::CArray(CArray<T> &&ary)
{
	ChkRefCntAndDec();
	// �����ƶ�
	GiveValue(ary);
	// �������
	ary.m_pBuf = nullptr;
	ary.m_pRefCnt = nullptr;
	ary.m_nSize = ary.m_nBufSize = 0;
}

template <typename T>
CArray<T> & CArray<T>::operator=(const CArray<T> &ary)
{
	// TODO: �ڴ˴����� return ���
	// ���������ü������ͻ����ͷ�
	if (!ary.m_nBufSize || !ary.m_pBuf || !ary.m_pRefCnt)
	{
		return(*this);
	}
	// �����������û���ɾ��
	ChkRefCntAndDec();
	// ��������
	GiveValue(ary);
	// �����������ü���
	++(*m_pRefCnt);
}

template <typename T>
void CArray<T>::ChkRefCntAndDec()
{
	if (m_pBuf && m_pRefCnt && *m_pRefCnt >= 1 && m_nBufSize > 0)
	{
		if (*m_pRefCnt > 1)
		{
			// �����ֻ��2��������������ֱ�Ӽ������ü���
			--(*m_pRefCnt);
			return;
		}
	}
	// ȫ���ͷ�
	ReleaseAll();
}

template <typename T>
void CArray<T>::ChkRefCntAndDec(CArray<T> &ary)
{
	if (ary.m_pBuf && ary.m_pRefCnt && *(ary.m_pRefCnt) >= 1 && ary.m_nBufSize > 0)
	{
		if (*(ary.m_pRefCnt) > 1)
		{
			// �����ֻ��2��������������ֱ�Ӽ������ü���
			--(*ary.m_pRefCnt);
			return;
		}
	}
	// ȫ���ͷ�
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

	// TODO: �ڴ˴����� return ���
	if (!m_nBufSize || !m_pBuf || !m_pRefCnt || nIdx > m_nSize)
	{
		return(*this);
	}
	// �ռ䲻�������
	if (m_nSize >= m_nBufSize)
	{
		nNewSize = m_nBufSize * 2;
	}
	// �����µĿռ�
	pArr = new T[nNewSize];
	if (!pArr)
	{
		return(*this);
	}
	memset(pArr, 0, sizeof(m_pBuf[0]) * nNewSize);
	// ��ֹǳ����, ���¹���
	T *tmp = new T[m_nBufSize];
	memset(tmp, 0, sizeof(m_pBuf[0]) * m_nBufSize);
	for (int i = 0; i < m_nSize; ++i)
	{
		tmp[i] = m_pBuf[i];
	}
	memcpy(pArr, tmp, sizeof(tmp[0]) * m_nSize);
	// ����֮ǰ������
	ChkRefCntAndDec();
	// ���»��������µĻ�������С
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
	// TODO: �ڴ˴����� return ���
	return(Insert(val, 0));
}

template <typename T>
CArray<T> & CArray<T>::InsertTail(const T &val)
{
	// TODO: �ڴ˴����� return ���
	return(Insert(val, m_nSize));
}

template <typename T>
CArray<T> &CArray<T>::Delete(size_t nIdx)
{
	T *pArr = nullptr;
	size_t nTmpSize = m_nSize;
	size_t nTmpBufSize = m_nBufSize;

	// TODO: �ڴ˴����� return ���
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
	// m_nBufSize���ֲ��伴��
	memset(pArr, 0, sizeof(m_pBuf[0]) * m_nBufSize);
	// ��ֹǳ����, ���¹���
	T *tmp = new T[m_nBufSize];
	memset(tmp, 0, sizeof(m_pBuf[0]) * m_nBufSize);
	for (int i = 0; i < m_nSize; ++i)
	{
		tmp[i] = m_pBuf[i];
	}
	memcpy(pArr, tmp, sizeof(tmp[0]) * m_nSize);
	// �����������û���ɾ������
	ChkRefCntAndDec();
	// �ָ�֮ǰ������
	m_nSize = nTmpSize;
	m_nBufSize = nTmpBufSize;
	m_pBuf = pArr;
	m_pRefCnt = new size_t(1);
	// ���·����ڴ�, ��һ��
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
	// TODO: �ڴ˴����� return ���
	return(Delete(0));
}

template <typename T>
CArray<T> & CArray<T>::DeleteTail()
{
	// TODO: �ڴ˴����� return ���
	return(Delete(m_nSize - 1));
}

template <typename T>
T CArray<T>::operator[](size_t nIdx)
{
	// TODO: �ڴ˴����� return ���
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


