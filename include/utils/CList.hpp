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

		T	  m_val;            //����
		Node* m_prev = nullptr; //ǰ�����ĵ�ַ
		Node* m_next = nullptr; //��̽��ĵ�ַ
	};
	static const size_t INVALID_IDX = -1;
public:
	// Ĭ�Ϲ��캯��
	CList();
	// �������캯��
	CList(const CList& lst);
	// �ƶ����캯��
	CList(CList&& lst);
	// ��������
	virtual ~CList();
	// �ȺŸ�ֵ�����
	CList& operator=(const CList& lst);
	// ����Ҫ�ƶ��Ⱥ����������
	CList& operator=(CList&& lst) = delete;
	// ����[]�����
	int& operator[](size_t nIdx);
	// �½�����ͷ��
	CList& InsertHead(int val);
	// �½�����β��
	CList& InsertTail(int val);
	// ��ָ����������ֵ
	CList& Insert(int val, size_t nIdx); 
	// ������ӡ˫������
	void TraverseList() const;
	// ɾ��˫������ͷ��
	CList& DeleteHead();
	// ɾ��˫������β��
	CList& DeleteTail();
	// ɾ��ָ������ָ��Ľ��
	CList& Delete(size_t nIdx);           
	// ��������ͷſռ�
	void   Clear();
	// ͨ��ֵ��Ѱ�Ҷ�Ӧ��������
	int    Find(int val) const; 
	// ��ȡ����Ԫ������
	size_t Size() const;
	// �ж������Ƿ�Ϊ��
	bool   IsEmpty() const;

private:
	// ͨ��������ȡִ�н���ָ��
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
	// ��ʼ��˫������
	void InitList();
private:
	Node* m_pHeadGuard = nullptr; // ͷ�ڱ�
	Node* m_pTailGuard = nullptr; // β�ڱ�
	size_t m_nSize = 0;           // ����
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
	// ��ʼ����ǰ����
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
	// ��յ�ǰ����
	Clear();
	// ��ʼ����ǰ����
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
		// �����½��
		pNewNode = new Node(val);
		if (!pNewNode)
		{
			break;
		}
		pNewNode->m_val = val;
		// ����ͷ������Ǹ����
		pNodeAfterHeadNode = m_pHeadGuard->m_next;
		// �½���head_guard��ָ
		m_pHeadGuard->m_next = pNewNode;
		pNewNode->m_prev = m_pHeadGuard;
		// �½��ͺ��㻥ָ
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

	// ��������һ��λ����ͷ�巨
	if (!nIdx)
	{
		return(InsertHead(val));
	}

	// �ѽ��Ų����nIdx��ָ��λ��
	while ((iCurPos < nIdx) && (pCurNode != pEndNode))
	{
		pCurNode = pCurNode->m_next;
		++iCurPos;
	}

	do
	{
		// �����û�е�����λ��, ������Ѿ���ͷ˵������λ��̫Զ
		if ((iCurPos < nIdx) && (pCurNode == pEndNode))
		{
			break;
		}
		// �����½��
		pNewNode = new Node(val);
		if (!pNewNode)
		{
			break;
		}
		// ���浱ǰ�ڵ�
		pEndNode = pCurNode;
		// ��ǰ�ڵ�ǰһ�����
		pCurNode = pCurNode->m_prev;
		// ִ�в������
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
		// ������һ�����
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
	// ���������һһ��ΪNull���ͷź���������
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
	// ���·���ͷβ���
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
	// ͷβ��ָ
	m_pHeadGuard->m_next = m_pTailGuard;
	m_pHeadGuard->m_prev = nullptr;
	m_pTailGuard->m_prev = m_pHeadGuard;
	m_pTailGuard->m_next = nullptr;
	m_nSize = 0;

	return;
}

