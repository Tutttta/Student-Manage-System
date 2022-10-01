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

	// 默认构造函数
	CQueue();
	// 含参构造函数
	CQueue(const T *pDataAry, size_t nSizeOfData);
	// 析构函数
	~CQueue();
	// 从头部插入数据
	bool InsertFromHead(const T &refData);
	// 从尾部插入数据
	bool InsertFromTail(const T &refData);
	
	bool CreateQueue(const T *pDataAry, size_t nSizeOfData, bool bInsertFromHead = true);

	// 从头部插入数据
	bool DeleteFromHead(T &refDelData);
	bool DeleteFromHead();
	// 从尾部插入数据
	bool DeleteFromTail(T &refDelData);
	bool DeleteFromTail();

	// 从头部开始遍历
	void TraverseFromHead() const;
	// 从尾部开始遍历
	void TraverseFromTail() const;
	// 获取头部数据
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

	// 获取队列长度
	inline size_t GetLength() const
	{
		return(m_nQueueLen);
	}
	// 判定队列是否为空
	inline bool IsEmpty() const
	{
		return(!m_nQueueLen);
	}
	// 清除队列为空队列
	void CleanQueue()
	{
		ReleaseQueue();
	}

private:
	// 释放队列
	void ReleaseQueue();
private:
	// 头结点
	pQNode m_pHeadNode;
	pQNode m_pTailNode;
	size_t m_nQueueLen;
};

template <typename T>
bool CQueue<T>::CreateQueue(const T *pDataAry, size_t nSizeOfData, bool bInsertFromHead/* = true*/)
{
	bool fOk = false;

	// 当前存在队列先释放
	if (m_pHeadNode || m_pTailNode || m_nQueueLen)
	{
		ReleaseQueue();
	}

	if (pDataAry && nSizeOfData)
	{
		// 头插法插入结点
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
				// 如果有一个插入失败则释放所有结点
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

	// 空树直接返回
	if (!m_pTailNode)
	{
		return;
	}
	// 只有一个结点, 打印返回
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

	// 没有元素则直接返回false
	if (!m_pHeadNode || !m_pTailNode)
	{
		return(false);
	}
	// 只有一个结点
	if (1 == m_nQueueLen)
	{
		delete m_pHeadNode;
		m_pTailNode = m_pHeadNode = nullptr;
		m_nQueueLen = 0;
		return(true);
	}
	// 至少2个结点, 获取其前后结点
	pNextNode = m_pTailNode->m_pNext;
	pPrevNode = m_pTailNode->m_pPrev;
	// 删除结点
	pNextNode->m_pPrev = pPrevNode;
	pPrevNode->m_pNext = pNextNode;
	// 保存数据
	refDelData = m_pTailNode->m_data;
	// 如果被删除的结点是头尾同时指向的结点
	if (m_pHeadNode == m_pTailNode)
	{
		m_pHeadNode = pPrevNode;
	}

	delete m_pTailNode;
	m_pTailNode = nullptr;

	m_pTailNode = pNextNode;

	// 降低结点数量, 头指针指向下一个结点
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
		// 分配一个新的结点
		pNewNode = new QNode(refData);
		if (!pNewNode)
		{
			fOk = false;
			break;
		}
		// 双向循环队列中不存在任何元素
		if (!m_nQueueLen)
		{
			// 直接创建一个新元素后插入队列
			// 头尾指针都指向新节点
			m_pHeadNode = m_pTailNode = pNewNode;
			pNewNode->m_pPrev = pNewNode;
			pNewNode->m_pNext = pNewNode;

			break;
		}
		// 双向队列中含有至少一个元素
		// 保存尾结点的后结点
		pTailPrev = m_pTailNode->m_pPrev;

		// 尾结点和新结点互指
		pNewNode->m_pNext = m_pTailNode;
		m_pTailNode->m_pPrev = pNewNode;
		// 尾结点的后结点和新结点互指
		pNewNode->m_pPrev = pTailPrev;
		pTailPrev->m_pNext = pNewNode;
		// 更新头结点的指向新结点
		m_pTailNode = pNewNode;
	} while (false);

	// 插入结点成功则增加队列结点数量
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

	// 没有元素则直接返回false
	if (!m_pHeadNode || !m_pTailNode)
	{
		return(false);
	}
	// 只有一个结点
	if (1 == m_nQueueLen)
	{
		delete m_pTailNode;
		m_pTailNode = m_pHeadNode = nullptr;
		m_nQueueLen = 0;
		return(true);
	}
	// 至少2个结点, 获取其前后结点
	pNextNode = m_pHeadNode->m_pNext;
	pPrevNode = m_pHeadNode->m_pPrev;
	// 删除结点
	pNextNode->m_pPrev = pPrevNode;
	pPrevNode->m_pNext = pNextNode;
	// 保存数据
	refDelData = m_pHeadNode->m_data;
	// 如果被删除的结点是头尾同时指向的结点
	if (m_pHeadNode == m_pTailNode)
	{
		m_pTailNode = pNextNode;
	}

	delete m_pHeadNode;
	m_pHeadNode = nullptr;

	m_pHeadNode = pPrevNode;
	
	// 降低结点数量, 头指针指向下一个结点
	--m_nQueueLen;

	return(true);
}

template <typename T>
void CQueue<T>::TraverseFromHead() const
{
	pQNode pCurNode = nullptr;
	pQNode pEndNode = nullptr;

	// 空树直接返回
	if (!m_pHeadNode)
	{
		return;
	}
	// 只有一个结点, 打印返回
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

	// 一个结点都没有, 直接返回
	if (!m_nQueueLen)
	{
		return;
	}
	// 只有一个结点的情况, 释放后返回
	if (1 == m_nQueueLen)
	{
		delete m_pHeadNode;
		m_pHeadNode = m_pTailNode = nullptr;
		m_nQueueLen = 0;
		return;
	}
	// 至少有2个结点, 获取头指针指向结点的下一个结点
	pCurNode = m_pHeadNode->m_pNext;
	while (pCurNode != m_pHeadNode)
	{
		// 释放除了头结点外的所有结点
		// 保存下一个结点
		pTmpNode = pCurNode->m_pNext;
		delete pCurNode;
		pCurNode = nullptr;
		pCurNode = pTmpNode;
	}
	// 释放头结点
	if (pCurNode)
	{
		delete pCurNode;
		pCurNode = nullptr;
	}
	// 头指针和位置在都指向nullptr
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
		// 分配一个新的结点
		pNewNode = new QNode(refData);
		if (!pNewNode)
		{
			fOk = false;
			break;
		}
		// 双向循环队列中不存在任何元素
		if (!m_nQueueLen)
		{
			// 直接创建一个新元素后插入队列
			// 头尾指针都指向新节点
			m_pHeadNode = m_pTailNode = pNewNode;
			pNewNode->m_pPrev = pNewNode;
			pNewNode->m_pNext = pNewNode;

			break;
		}
		// 双向队列中含有至少一个元素
		// 保存头结点的前结点
		pHeadNext = m_pHeadNode->m_pNext;
		
		// 头结点的前结点和新结点互指
		pNewNode->m_pNext = pHeadNext;
		pHeadNext->m_pPrev = pNewNode;
		// 头结点和新结点互指
		pNewNode->m_pPrev = m_pHeadNode;
		m_pHeadNode->m_pNext = pNewNode;
		// 更新头结点的指向新结点
		m_pHeadNode = pNewNode;
	} while (false);

	// 插入结点成功则增加队列结点数量
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
