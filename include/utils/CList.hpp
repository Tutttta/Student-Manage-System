#pragma once
/*++

Copyright (c) Ki0pler. All rights reserved.

You may only use this code if you agree to the terms of the GNU General Public License.
If you do not agree to the terms, do not use the code.
Any question about the code. Contact me with the email ki0pler@outlook.com


Module Name:

	CList.pp

Abstract:

   This module provides the implementation of doubly circular list implementation.

--*/
#include "assert.h"
#include <iostream>

using namespace std;



template <typename T>
class CList
{
public:
	struct Node
	{
		Node(T val) :m_val(val) {}
		Node() { }

		T	  m_val;            //数据
		Node* m_prev = nullptr; //前驱结点的地址
		Node* m_next = nullptr; //后继结点的地址
	};
	static const size_t INVALID_IDX = -1;
public:
	// 默认构造函数
	CList();
	// 拷贝构造函数
	CList(const CList& lst);
	// 移动构造函数
	CList(CList&& lst);
	// 析构函数
	virtual ~CList();
	// 等号赋值运算符
	CList& operator=(const CList& lst);
	// 不需要移动等号运算符重载
	CList& operator=(CList&& lst) = delete;
	// 重载[]运算符
	int& operator[](size_t nIdx);
	// 新结点插入头部
	CList& InsertHead(int val);
	// 新结点插入尾部
	CList& InsertTail(int val);
	// 在指定索引插入值
	CList& Insert(int val, size_t nIdx); 
	// 遍历打印双向链表
	void TraverseList() const;
	// 删除双向链表头部
	CList& DeleteHead();
	// 删除双向链表尾部
	CList& DeleteTail();
	// 删除指定索引指向的结点
	CList& Delete(size_t nIdx);           
	// 清空链表并释放空间
	void   Clear();
	// 通过值来寻找对应结点的索引
	int    Find(int val) const; 
	// 获取链表元素数量
	size_t Size() const;
	// 判断链表是否为空
	bool   IsEmpty() const;

private:
	// 通过索引获取执行结点的指针
	Node* IdxToNode(size_t nIdx)
	{
		Node *pCurNode = nullptr;

		if (!m_pHeadGuard || !m_pTailGuard)
		{
			return(nullptr);
		}
		pCurNode = m_pHeadGuard->m_next;
		while (pCurNode != m_pTailGuard)
		{
			if (!nIdx)
			{
				return(pCurNode);
			}
			pCurNode = pCurNode->m_next;
			--nIdx;
		}

		return(nullptr);
	}
	// 初始化双向链表
	void InitList();
private:
	Node* m_pHeadGuard = nullptr; // 头哨兵
	Node* m_pTailGuard = nullptr; // 尾哨兵
	size_t m_nSize = 0;           // 个数
};

template <typename T>
CList<T>::CList() :
	m_pHeadGuard(nullptr), m_pTailGuard(nullptr), m_nSize(0)
{
	InitList();
}

template <typename T>
CList<T>::CList(const CList &lst)
{
	Node *pNewNode = nullptr;
	Node *pCurNode = lst.m_pHeadGuard->m_next;
	// 初始化当前链表
	InitList();

	while (pCurNode != lst.m_pTailGuard)
	{
		InsertTail(pCurNode->m_val);
		pCurNode = pCurNode->m_next;
	}
}

template <typename T>
CList<T>::CList(CList &&lst)
{
	m_nSize = lst.m_nSize;
	m_pHeadGuard = lst.m_pHeadGuard;
	m_pTailGuard = lst.m_pTailGuard;

	lst.m_nSize = 0;
	lst.m_pTailGuard = nullptr;
	lst.m_pHeadGuard = nullptr;
}

template <typename T>
CList<T>& CList<T>::operator=(const CList& lst)
{
	// 清空当前链表
	Clear();
	// 初始化当前链表
	InitList();
	Node *pNewNode = nullptr;
	Node *pCurNode = lst.m_pHeadGuard->m_next;

	while (pCurNode != lst.m_pTailGuard)
	{
		InsertTail(pCurNode->m_val);
		pCurNode = pCurNode->m_next;
	}

	return(*this);
}

template <typename T>
CList<T>::~CList()
{
	Clear();
}

template <typename T>
CList<T>& CList<T>::InsertHead(int val)
{
	Node *pNewNode = nullptr;
	Node *pNodeAfterHeadNode = nullptr;

	do
	{
		// 分配新结点
		pNewNode = new Node(val);
		if (!pNewNode)
		{
			break;
		}
		pNewNode->m_val = val;
		// 保存头结点后的那个结点
		pNodeAfterHeadNode = m_pHeadGuard->m_next;
		// 新结点和head_guard互指
		m_pHeadGuard->m_next = pNewNode;
		pNewNode->m_prev = m_pHeadGuard;
		// 新结点和后结点互指
		pNewNode->m_next = pNodeAfterHeadNode;
		pNodeAfterHeadNode->m_prev = pNewNode;
		++m_nSize;
	} while (false);

	return(*this);
}

template <typename T>
CList<T>& CList<T>::InsertTail(int val)
{
	return(Insert(val, m_nSize));
}

template <typename T>
CList<T>& CList<T>::Insert(int val, size_t nIdx)
{
	Node *pCurNode = m_pHeadGuard->m_next;
	Node *pEndNode = m_pTailGuard;
	Node *pNewNode = nullptr;
	size_t iCurPos = 0;

	// 如果插入第一个位置则头插法
	if (!nIdx)
	{
		return(InsertHead(val));
	}

	// 把结点挪动到nIdx的指定位置
	while ((iCurPos < nIdx) && (pCurNode != pEndNode))
	{
		pCurNode = pCurNode->m_next;
		++iCurPos;
	}

	do
	{
		// 如果还没有到插入位置, 但结点已经到头说明插入位置太远
		if ((iCurPos < nIdx) && (pCurNode == pEndNode))
		{
			break;
		}
		// 分配新结点
		pNewNode = new Node(val);
		if (!pNewNode)
		{
			break;
		}
		// 保存当前节点
		pEndNode = pCurNode;
		// 当前节点前一个结点
		pCurNode = pCurNode->m_prev;
		// 执行插入操作
		pNewNode->m_next = pEndNode;
		pEndNode->m_prev = pNewNode;
		pNewNode->m_prev = pCurNode;
		pCurNode->m_next = pNewNode;
		++m_nSize;
	} while (false);

	return(*this);
}

template <typename T>
void CList<T>::TraverseList() const
{
	Node *pCurNode = nullptr;

	if (!m_pHeadGuard || !m_pTailGuard)
	{
		return;
	}
	pCurNode = m_pHeadGuard->m_next;
	while (pCurNode != m_pTailGuard)
	{
		cout << pCurNode->m_val << " ";
		pCurNode = pCurNode->m_next;
	}
	cout << endl;

	return;
}

template <typename T>
CList<T>& CList<T>::DeleteHead()
{
	return(Delete(0));
}

template <typename T>
CList<T>& CList<T>::DeleteTail()
{
	return(Delete(m_nSize - 1));
}

template <typename T>
CList<T>& CList<T>::Delete(size_t nIdx)
{
	Node *pCurNode = nullptr;
	Node *pPreNode = nullptr;
	Node *pNextNode = nullptr;

	pCurNode = IdxToNode(nIdx);
	if (pCurNode)
	{
		pPreNode = pCurNode->m_prev;
		pNextNode = pCurNode->m_next;

		pPreNode->m_next = pNextNode;
		pNextNode->m_prev = pPreNode;

		if (pCurNode)
		{
			delete pCurNode;
			pCurNode = nullptr;
		}
		--m_nSize;
	}

	return(*this);
}

template <typename T>
int& CList<T>::operator[](size_t nIdx)
{
	Node *pCurNode = nullptr;

	pCurNode = IdxToNode(nIdx);

	return(pCurNode->m_val);
}

template <typename T>
void CList<T>::Clear()
{
	Node *pCurNode = m_pHeadGuard;
	Node *pNextNode = nullptr;

	while (pCurNode)
	{
		// 保存下一个结点
		pNextNode = pCurNode->m_next;
		if (pCurNode)
		{
			pCurNode->m_prev = nullptr;
			pCurNode->m_next = nullptr;
			delete pCurNode;
			pCurNode = nullptr;
		}
		pCurNode = pNextNode;
	}
	m_pHeadGuard = m_pTailGuard = nullptr;
}

//int CList::Find(int val) const
//{
//	Node *pCurNode = m_pHeadGuard->m_next;
//	int iIdx = 0;
//
//	while (pCurNode != m_pTailGuard)
//	{
//		if (val == pCurNode->m_val)
//		{
//			return(iIdx);
//		}
//		++iIdx;
//		pCurNode = pCurNode->m_next;
//	}
//
//	return(INVALID_IDX);
//}

template <typename T>
size_t CList<T>::Size() const
{
	return(m_nSize);
}

//template <typename T>
//CList<T>::Node *CList<T>::IdxToNode(size_t nIdx)
//{
//	
//}

template <typename T>
bool CList<T>::IsEmpty() const
{
	return(!m_nSize);
}

template <typename T>
void CList<T>::InitList()
{
	// 如果存在任一一个为Null则释放后重新申请
	if (!m_pHeadGuard || !m_pTailGuard)
	{
		if (m_pTailGuard)
		{
			delete m_pTailGuard;
			m_pTailGuard = nullptr;
		}
		if (m_pHeadGuard)
		{
			delete m_pHeadGuard;
			m_pHeadGuard = nullptr;
		}
	}
	// 重新分配头尾结点
	do
	{
		if (!m_pHeadGuard)
		{
			m_pHeadGuard = new Node(INVALID_IDX);
		}
		if (!m_pTailGuard)
		{
			m_pTailGuard = new Node(INVALID_IDX);
		}
	} while (!m_pHeadGuard || !m_pTailGuard);
	// 头尾互指
	m_pHeadGuard->m_next = m_pTailGuard;
	m_pHeadGuard->m_prev = nullptr;
	m_pTailGuard->m_prev = m_pHeadGuard;
	m_pTailGuard->m_next = nullptr;
	m_nSize = 0;

	return;
}

