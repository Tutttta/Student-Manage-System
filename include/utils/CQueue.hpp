#pragma once
/*++

Copyright (c) Ki0pler. All rights reserved.

You may only use this code if you agree to the terms of the GNU General Public License.
If you do not agree to the terms, do not use the code.
Any question about the code. Contact me with the email ki0pler@outlook.com


Module Name:

	CQueue.hpp

Abstract:

   This module provides the basic implementation of the template bidirectional circular queue

--*/

template <typename T>
class CQueue
{
public:
	typedef struct tagQueueNode
	{
		tagQueueNode(T data) : 
			m_data(data), m_pPrev(nullptr), m_pNext(nullptr) {}
		T m_data;
		struct tagQueueNode *m_pPrev;
		struct tagQueueNode *m_pNext;
	} QNode, *pQNode;

	// Ĭ�Ϲ��캯��
	CQueue();
	// ���ι��캯��
	CQueue(const T *pDataAry, size_t nSizeOfData);
	// ��������
	~CQueue();
	// ��ͷ����������
	bool InsertFromHead(const T &refData);
	// ��β����������
	bool InsertFromTail(const T &refData);
	
	bool CreateQueue(const T *pDataAry, size_t nSizeOfData, bool bInsertFromHead = true);

	// ��ͷ����������
	bool DeleteFromHead(T &refDelData);
	bool DeleteFromHead();
	// ��β����������
	bool DeleteFromTail(T &refDelData);
	bool DeleteFromTail();

	// ��ͷ����ʼ����
	void TraverseFromHead() const;
	// ��β����ʼ����
	void TraverseFromTail() const;
	// ��ȡͷ������
	bool GetHeadValue(T &data) const
	{
		if (!m_pHeadNode)
		{
			return(false);
		}
		data = m_pHeadNode->m_data;

		return(true);
	}
	bool GetTailValue(T &data) const
	{
		if (!m_pTailNode)
		{
			return(false);
		}
		data = m_pTailNode->m_data;

		return(true);
	}

	// ��ȡ���г���
	inline size_t GetLength() const
	{
		return(m_nQueueLen);
	}
	// �ж������Ƿ�Ϊ��
	inline bool IsEmpty() const
	{
		return(!m_nQueueLen);
	}
	// �������Ϊ�ն���
	void CleanQueue()
	{
		ReleaseQueue();
	}

private:
	// �ͷŶ���
	void ReleaseQueue();
private:
	// ͷ���
	pQNode m_pHeadNode;
	pQNode m_pTailNode;
	size_t m_nQueueLen;
};

template <typename T>
bool CQueue<T>::CreateQueue(const T *pDataAry, size_t nSizeOfData, bool bInsertFromHead/* = true*/)
{
	bool fOk = false;

	// ��ǰ���ڶ������ͷ�
	if (m_pHeadNode || m_pTailNode || m_nQueueLen)
	{
		ReleaseQueue();
	}

	if (pDataAry && nSizeOfData)
	{
		// ͷ�巨������
		for (size_t nIdx = 0; nIdx < nSizeOfData; ++nIdx)
		{
			if (bInsertFromHead)
			{
				fOk = InsertFromHead(pDataAry[nIdx]);
			}
			else
			{
				fOk = InsertFromTail(pDataAry[nIdx]);
			}
			if (!fOk)
			{
				// �����һ������ʧ�����ͷ����н��
				ReleaseQueue();
				return(false);
			}
		}
	}

	return(true);
}

template <typename T>
void CQueue<T>::TraverseFromTail() const
{
	pQNode pCurNode = nullptr;
	pQNode pEndNode = nullptr;

	// ����ֱ�ӷ���
	if (!m_pTailNode)
	{
		return;
	}
	// ֻ��һ�����, ��ӡ����
	if (1 == m_nQueueLen)
	{
		cout << m_pTailNode->m_data << endl;
		return;
	}
	pCurNode = m_pTailNode;
	pEndNode = m_pTailNode->m_pPrev;

	while (pCurNode != pEndNode)
	{
		cout << pCurNode->m_data << " ";
		pCurNode = pCurNode->m_pNext;
	}
	cout << pCurNode->m_data << endl;

	return;
}

template <typename T>
bool CQueue<T>::DeleteFromTail(T &refDelData)
{
	pQNode pPrevNode = nullptr;
	pQNode pNextNode = nullptr;

	// û��Ԫ����ֱ�ӷ���false
	if (!m_pHeadNode || !m_pTailNode)
	{
		return(false);
	}
	// ֻ��һ�����
	if (1 == m_nQueueLen)
	{
		delete m_pHeadNode;
		m_pTailNode = m_pHeadNode = nullptr;
		m_nQueueLen = 0;
		return(true);
	}
	// ����2�����, ��ȡ��ǰ����
	pNextNode = m_pTailNode->m_pNext;
	pPrevNode = m_pTailNode->m_pPrev;
	// ɾ�����
	pNextNode->m_pPrev = pPrevNode;
	pPrevNode->m_pNext = pNextNode;
	// ��������
	refDelData = m_pTailNode->m_data;
	// �����ɾ���Ľ����ͷβͬʱָ��Ľ��
	if (m_pHeadNode == m_pTailNode)
	{
		m_pHeadNode = pPrevNode;
	}

	delete m_pTailNode;
	m_pTailNode = nullptr;

	m_pTailNode = pNextNode;

	// ���ͽ������, ͷָ��ָ����һ�����
	--m_nQueueLen;

	return(true);
}

template <typename T>
bool CQueue<T>::DeleteFromTail()
{
	T data;

	return(DeleteFromTail(data));
}

template <typename T>
bool CQueue<T>::InsertFromTail(const T &refData)
{
	pQNode pNewNode = nullptr;
	pQNode pTailPrev = nullptr;
	bool fOk = true;

	do
	{
		// ����һ���µĽ��
		pNewNode = new QNode(refData);
		if (!pNewNode)
		{
			fOk = false;
			break;
		}
		// ˫��ѭ�������в������κ�Ԫ��
		if (!m_nQueueLen)
		{
			// ֱ�Ӵ���һ����Ԫ�غ�������
			// ͷβָ�붼ָ���½ڵ�
			m_pHeadNode = m_pTailNode = pNewNode;
			pNewNode->m_pPrev = pNewNode;
			pNewNode->m_pNext = pNewNode;

			break;
		}
		// ˫������к�������һ��Ԫ��
		// ����β���ĺ���
		pTailPrev = m_pTailNode->m_pPrev;

		// β�����½�㻥ָ
		pNewNode->m_pNext = m_pTailNode;
		m_pTailNode->m_pPrev = pNewNode;
		// β���ĺ�����½�㻥ָ
		pNewNode->m_pPrev = pTailPrev;
		pTailPrev->m_pNext = pNewNode;
		// ����ͷ����ָ���½��
		m_pTailNode = pNewNode;
	} while (false);

	// ������ɹ������Ӷ��н������
	if (fOk)
	{
		m_nQueueLen++;
	}

	return(fOk);
}

template <typename T>
bool CQueue<T>::DeleteFromHead()
{
	T data;
	return(DeleteFromHead(data));
}


template <typename T>
CQueue<T>::CQueue() :
	m_pHeadNode(nullptr), m_pTailNode(nullptr), m_nQueueLen(0)
{

}

template <typename T>
CQueue<T>::~CQueue()
{
	ReleaseQueue();
}


template <typename T>
bool CQueue<T>::DeleteFromHead(T &refDelData)
{
	pQNode pPrevNode = nullptr;
	pQNode pNextNode = nullptr;

	// û��Ԫ����ֱ�ӷ���false
	if (!m_pHeadNode || !m_pTailNode)
	{
		return(false);
	}
	// ֻ��һ�����
	if (1 == m_nQueueLen)
	{
		delete m_pTailNode;
		m_pTailNode = m_pHeadNode = nullptr;
		m_nQueueLen = 0;
		return(true);
	}
	// ����2�����, ��ȡ��ǰ����
	pNextNode = m_pHeadNode->m_pNext;
	pPrevNode = m_pHeadNode->m_pPrev;
	// ɾ�����
	pNextNode->m_pPrev = pPrevNode;
	pPrevNode->m_pNext = pNextNode;
	// ��������
	refDelData = m_pHeadNode->m_data;
	// �����ɾ���Ľ����ͷβͬʱָ��Ľ��
	if (m_pHeadNode == m_pTailNode)
	{
		m_pTailNode = pNextNode;
	}

	delete m_pHeadNode;
	m_pHeadNode = nullptr;

	m_pHeadNode = pPrevNode;
	
	// ���ͽ������, ͷָ��ָ����һ�����
	--m_nQueueLen;

	return(true);
}

template <typename T>
void CQueue<T>::TraverseFromHead() const
{
	pQNode pCurNode = nullptr;
	pQNode pEndNode = nullptr;

	// ����ֱ�ӷ���
	if (!m_pHeadNode)
	{
		return;
	}
	// ֻ��һ�����, ��ӡ����
	if (1 == m_nQueueLen)
	{
		cout << m_pHeadNode->m_data << endl;
		return;
	}
	pCurNode = m_pHeadNode;
	pEndNode = m_pHeadNode->m_pPrev;
	
	while (pCurNode != pEndNode)
	{
		cout << pCurNode->m_data << " ";
		pCurNode = pCurNode->m_pNext;
	}
	cout << pCurNode->m_data << endl;

	return;
}

template <typename T>
void CQueue<T>::ReleaseQueue()
{
	pQNode pCurNode = nullptr;
	pQNode pTmpNode = nullptr;

	// һ����㶼û��, ֱ�ӷ���
	if (!m_nQueueLen)
	{
		return;
	}
	// ֻ��һ���������, �ͷź󷵻�
	if (1 == m_nQueueLen)
	{
		delete m_pHeadNode;
		m_pHeadNode = m_pTailNode = nullptr;
		m_nQueueLen = 0;
		return;
	}
	// ������2�����, ��ȡͷָ��ָ�������һ�����
	pCurNode = m_pHeadNode->m_pNext;
	while (pCurNode != m_pHeadNode)
	{
		// �ͷų���ͷ���������н��
		// ������һ�����
		pTmpNode = pCurNode->m_pNext;
		delete pCurNode;
		pCurNode = nullptr;
		pCurNode = pTmpNode;
	}
	// �ͷ�ͷ���
	if (pCurNode)
	{
		delete pCurNode;
		pCurNode = nullptr;
	}
	// ͷָ���λ���ڶ�ָ��nullptr
	m_pHeadNode = m_pTailNode = nullptr;
	m_nQueueLen = 0;

	return;
}

template <typename T>
bool CQueue<T>::InsertFromHead(const T &refData)
{
	pQNode pNewNode = nullptr;
	pQNode pHeadNext = nullptr;
	bool fOk = true;

	do
	{
		// ����һ���µĽ��
		pNewNode = new QNode(refData);
		if (!pNewNode)
		{
			fOk = false;
			break;
		}
		// ˫��ѭ�������в������κ�Ԫ��
		if (!m_nQueueLen)
		{
			// ֱ�Ӵ���һ����Ԫ�غ�������
			// ͷβָ�붼ָ���½ڵ�
			m_pHeadNode = m_pTailNode = pNewNode;
			pNewNode->m_pPrev = pNewNode;
			pNewNode->m_pNext = pNewNode;

			break;
		}
		// ˫������к�������һ��Ԫ��
		// ����ͷ����ǰ���
		pHeadNext = m_pHeadNode->m_pNext;
		
		// ͷ����ǰ�����½�㻥ָ
		pNewNode->m_pNext = pHeadNext;
		pHeadNext->m_pPrev = pNewNode;
		// ͷ�����½�㻥ָ
		pNewNode->m_pPrev = m_pHeadNode;
		m_pHeadNode->m_pNext = pNewNode;
		// ����ͷ����ָ���½��
		m_pHeadNode = pNewNode;
	} while (false);

	// ������ɹ������Ӷ��н������
	if (fOk)
	{
		m_nQueueLen++;
	}
	
	return(fOk);
}

template <typename T>
CQueue<T>::CQueue(const T *pDataAry, size_t nSizeOfData) : 
	m_pHeadNode(nullptr), m_pTailNode(nullptr), m_nQueueLen(0)
{
	
}
