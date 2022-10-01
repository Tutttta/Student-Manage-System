#pragma once
/*++

Copyright (c) Ki0pler. All rights reserved.

You may only use this code if you agree to the terms of the GNU General Public License.
If you do not agree to the terms, do not use the code.
Any question about the code. Contact me with the email ki0pler@outlook.com


Module Name:

	CAVLTree.hpp

Abstract:

   This module provides an implementation of a balanced binary search tree.

--*/
#include "CStack.hpp"
#include "CQueue.hpp"

template <typename T>
class CAVLTree
{
public:
	// �������Ľ��ṹ
	typedef struct tagTreeNode
	{
		tagTreeNode() = default;
		tagTreeNode(T data) :
			m_data(data), m_pLeftChild(nullptr), 
			m_pRightChild(nullptr), m_pParent(nullptr), m_nHeight(1)
		{ }
		T m_data;
		struct tagTreeNode *m_pLeftChild;   // ����ָ��
		struct tagTreeNode *m_pRightChild;  // �Һ���ָ��
		struct tagTreeNode *m_pParent;		// �����ָ��
		size_t m_nHeight;					// ���Ĭ�ϸ߶�
	} TreeNode, *pTreeNode;
public:
	CAVLTree() : m_pRootNode(nullptr) {}
	~CAVLTree();
	// Ѱ�ҽ��, Ѱ��ֵΪrcDataToFind�Ľ�㲢����������ͨ����������
	bool FindNode(TreeNode &rNode, const T rcDataToFind);
	bool FindNode(pTreeNode *ppNode, const T rcDataToFind);
	// �ж�ֵΪrcDataToFind�Ľ���Ƿ����
	bool FindNode(const T rcDataToFind);
	// ������
	bool InsertNode(T data);
	// ɾ�����
	bool DeleteNode(const T &rcdata);
	// ����һ������nDataArySize��Ԫ�ص���
	bool CreateTree(const T *pDataAry, size_t nDataArySize);
	// ִ��ǰ�����
	void PreOrderTraverse() const;
	// ִ���������
	void InOrderTraverse() const;
	// ִ�к�������
	void PostOrderTraverse() const;
	// ִ�в������
	void LevelOrderTraverse() const;
private:
	// Ϊת��AVAL�������߶�
	void AdjustHeight(pTreeNode pNode);
	// ����
	void LeftRotate(pTreeNode pNode);
	// ����
	void RightRotate(pTreeNode pNode);
	// ��ȡ��ǰ���߶�
	size_t GetHeight(const pTreeNode pNode) const
	{
		return(pNode ? (pNode->m_nHeight) : 0);
	}
	// �ڲ��õ�max����
	size_t MaxNodeValue(size_t nLeft, size_t nRight) 
	{
		return((nLeft >= nRight) ? nLeft : nRight);
	}
	// ���½��㵱ǰ���߶�
	size_t CalcHeight(pTreeNode pNode);
	// �ͷ���
	void ReleaseTree();
private:
	pTreeNode m_pRootNode;
};

template <typename T>
CAVLTree<T>::~CAVLTree()
{
	ReleaseTree();
}

template <typename T>
bool CAVLTree<T>::FindNode(pTreeNode *ppNode, const T rcDataToFind)
{
	pTreeNode pCurNode = m_pRootNode;
	T CurNodeData;

	if (!pCurNode)
	{
		return(false);
	}

	while (pCurNode)
	{
		// ��ȡ��ǰ�ڵ�����
		CurNodeData = pCurNode->m_data;
		// �������ݴ�С�ڶ��������ƶ�
		if (CurNodeData > rcDataToFind)
		{
			pCurNode = pCurNode->m_pLeftChild;
		}
		else if (CurNodeData < rcDataToFind)
		{
			pCurNode = pCurNode->m_pRightChild;
		}
		else
		{
			// �ҵ����ȡ������
			*ppNode = pCurNode;
			return(true);
		}
	}

	return(false);
}

template <typename T>
void CAVLTree<T>::RightRotate(pTreeNode pNode)
{
	pTreeNode pCurNode = nullptr;
	pTreeNode pCurLNode = nullptr;
	pTreeNode pCurLRNode = nullptr;
	pTreeNode pParentNode = nullptr;

	if (!pNode)
	{
		return;
	}
	pCurNode = pNode;
	pCurLNode = pCurNode->m_pLeftChild;
	pCurLRNode = pCurLNode->m_pRightChild;
	pParentNode = pCurNode->m_pParent;

	// �����ǰ���ĸ���㲻����˵���Ǹ��ڵ�
	if (!pParentNode)
	{
		// ���¸����ָ��
		m_pRootNode = pCurLNode;
	}
	else
	{
		// �����ƽ����ĸ�����������¸�����ָ��
		if (pCurNode == pParentNode->m_pLeftChild)
		{
			// �����ǰ�������������
			pParentNode->m_pLeftChild = pCurLNode;
		}
		else
		{
			// �����ǰ����������ҽ��
			pParentNode->m_pRightChild = pCurLNode;
		}
	}
	
	pCurLNode->m_pParent = pParentNode;
	
	pCurLNode->m_pRightChild = pCurNode;
	pCurNode->m_pParent = pCurLNode;

	pCurNode->m_pLeftChild = pCurLRNode;
	if (pCurLRNode)
	{
		pCurLRNode->m_pParent = pCurNode;
	}

	CalcHeight(pCurNode);
	CalcHeight(pCurLNode);
}

template <typename T>
size_t CAVLTree<T>::CalcHeight(pTreeNode pNode)
{
	size_t nLeftChildHeight = 0;
	size_t nRightChildHeight = 0;

	if (!pNode)
	{
		return(0);
	}
	nLeftChildHeight = GetHeight(pNode->m_pLeftChild);
	nRightChildHeight = GetHeight(pNode->m_pRightChild);
	pNode->m_nHeight = MaxNodeValue(nLeftChildHeight, nRightChildHeight) + 1;

	return(pNode->m_nHeight);
}

template <typename T>
void CAVLTree<T>::LeftRotate(pTreeNode pNode)
{
	pTreeNode pParentNode = nullptr;
	pTreeNode pCurNode = nullptr;
	pTreeNode pCurRNode = nullptr;
	pTreeNode pCurRLNode = nullptr;

	if (!pNode)
	{
		return;
	}

	pCurNode = pNode;
	pCurRNode = pCurNode->m_pRightChild;
	pCurRLNode = pCurRNode->m_pLeftChild;
	pParentNode = pNode->m_pParent;
	// �����ƽ�����Ǹ��ڵ�
	if (!pParentNode)
	{
		// ���¸��ڵ�
		m_pRootNode = pCurRNode;
	}
	else
	{
		// �ж���ƽ�������丸�ڵ�����ӻ����Һ���
		// �����մ˽�������
		if (pCurNode == pParentNode->m_pLeftChild)
		{
			pParentNode->m_pLeftChild = pCurRNode;
		}
		else
		{
			pParentNode->m_pRightChild = pCurRNode;
		}
	}

	pCurNode->m_pParent = pCurRNode;
	pCurNode->m_pRightChild = pCurRLNode;
	if (pCurRLNode)
	{
		pCurRLNode->m_pParent = pCurNode;
	}
	pCurRNode->m_pLeftChild = pCurNode;
	pCurRNode->m_pParent = pParentNode;
	// ���¼���߶�
	CalcHeight(pCurNode);
	CalcHeight(pCurRNode);

	return;
}

template <typename T>
void CAVLTree<T>::AdjustHeight(pTreeNode pNode)
{
	int iLeftChildHeight = 0;
	int iRightChildHeight = 0;
	
	while (pNode)
	{
		// ���㵱ǰ���ĸ߶�
		CalcHeight(pNode);
		// �ж���ǰ����ƽ������
		iLeftChildHeight = GetHeight(pNode->m_pLeftChild);
		iRightChildHeight = GetHeight(pNode->m_pRightChild);
		
		if (iRightChildHeight - iLeftChildHeight > 1)
		{
			pTreeNode pRightNode = pNode->m_pRightChild;
			// RR����
			if (GetHeight(pRightNode->m_pLeftChild) <= GetHeight(pRightNode->m_pRightChild))
			{
				LeftRotate(pNode);
			}
			else
			{
				// RL����
				RightRotate(pRightNode);
				LeftRotate(pNode);
			}
		}
		else if (iLeftChildHeight - iRightChildHeight > 1)
		{
			pTreeNode pLeftNode = pNode->m_pLeftChild;
			if (GetHeight(pLeftNode->m_pLeftChild) >= GetHeight(pLeftNode->m_pRightChild))
			{
				RightRotate(pNode);
			}
			else
			{
				LeftRotate(pLeftNode);
				RightRotate(pNode);
			}
		}

		pNode = pNode->m_pParent;
	}
}

template <typename T>
bool CAVLTree<T>::DeleteNode(const T &rcdata)
{
	pTreeNode pDelNode = nullptr;
	pTreeNode pParentNodeOfDelNode = nullptr;
	pTreeNode pCurNode = nullptr;
	bool fOk = false;

	// ����ֱ�ӷ���false
	if (!m_pRootNode)
	{
		return(false);
	}
	// ����Ǹ��ڵ���ֱ��ɾ����ɹ�
	if (m_pRootNode->m_data == rcdata)
	{
		delete m_pRootNode;
		m_pRootNode = nullptr;
		return(true);
	}
	// ����Ҫɾ���Ľ��, û���ҵ�ֱ�ӷ���false
	fOk = FindNode(&pDelNode, rcdata);
	if (!fOk)
	{
		return(false);
	}
	// �ҵ����ڵ�
	pParentNodeOfDelNode = pDelNode->m_pParent;
	// �ý��û���ӽڵ�
	if (!pDelNode->m_pLeftChild && !pDelNode->m_pRightChild)
	{
		if (pDelNode == pParentNodeOfDelNode->m_pLeftChild)
		{
			pCurNode = pParentNodeOfDelNode->m_pLeftChild;
			pParentNodeOfDelNode->m_pLeftChild = nullptr;
		}
		else
		{
			pCurNode = pParentNodeOfDelNode->m_pRightChild;
			pParentNodeOfDelNode->m_pRightChild = nullptr;
		}
	}
	else if (pDelNode->m_pLeftChild && pDelNode->m_pRightChild)
	{
		pTreeNode pDelLeftChild = nullptr;
		pTreeNode pDelLeftAndMostRightChild = nullptr;
		pTreeNode pParentNode = nullptr;

		pDelLeftChild = pDelNode->m_pLeftChild;
		pDelLeftAndMostRightChild = pDelLeftChild->m_pRightChild;
		// �ҵ���������������ҽ��
		while (pDelLeftAndMostRightChild)
		{
			pParentNode = pDelLeftAndMostRightChild;
			pDelLeftAndMostRightChild = pDelLeftAndMostRightChild->m_pRightChild;
		}
		if (!pParentNode)
		{
			// ������ӽ��û��������, ֱ�Ӱ����ӽ���ֵ����Ҫɾ���Ľ��, Ȼ��ɾ�����ӽ��
			// ��ȡ��Ҫɾ���Ľ��
			pCurNode = pDelNode->m_pLeftChild;
			pDelNode->m_data = pDelLeftChild->m_data;
			pDelNode->m_pLeftChild = pDelLeftChild->m_pLeftChild;
			if (pDelLeftChild->m_pLeftChild)
			{
				pDelLeftChild->m_pLeftChild->m_pParent = pDelNode;
			}
		}
		else
		{ 
			// ��������ӽڵ����������
			pCurNode = pParentNode;
			pDelNode->m_data = pParentNode->m_data;
			
			if (pParentNode->m_pLeftChild)
			{
				pParentNode->m_pParent->m_pRightChild = pParentNode->m_pLeftChild;
			}
			else
			{
				pParentNode->m_pParent->m_pRightChild = nullptr;
			}
		}
	}
	else
	{
		// �ý�����1������
		if (pDelNode->m_pLeftChild)
		{
			// ��ɾ��������������
			if (pDelNode == pParentNodeOfDelNode->m_pLeftChild)
			{
				// ��ɾ�������������
				pDelNode->m_pParent->m_pLeftChild = pDelNode->m_pLeftChild;
				pDelNode->m_pLeftChild->m_pParent = pDelNode->m_pParent;
			}
			else
			{
				// ��ɾ����������ҽ��
				pDelNode->m_pParent->m_pRightChild = pDelNode->m_pLeftChild;
				pDelNode->m_pLeftChild->m_pParent = pDelNode->m_pParent;
			}
		}
		else
		{
			// ��ɾ��������������
			if (pDelNode == pParentNodeOfDelNode->m_pLeftChild)
			{
				// ��ɾ�������������
				pCurNode = pDelNode->m_pParent->m_pLeftChild;
				pDelNode->m_pParent->m_pLeftChild = pDelNode->m_pRightChild;
				pDelNode->m_pRightChild->m_pParent = pDelNode->m_pParent;
			}
			else
			{
				// ��ɾ����������ҽ��
				pCurNode = pDelNode->m_pParent->m_pRightChild;
				pDelNode->m_pParent->m_pRightChild = pDelNode->m_pRightChild;
				pDelNode->m_pRightChild->m_pParent = pDelNode->m_pParent;
			}
		}
	}
	AdjustHeight(pCurNode->m_pParent);
	if (pCurNode)
	{
		delete pCurNode;
		pCurNode = nullptr;
	}

	return(true);
}

template <typename T>
void CAVLTree<T>::LevelOrderTraverse() const
{
	CQueue<pTreeNode> queue;
	pTreeNode pCurNode = nullptr;

	if (!m_pRootNode)
	{
		return;
	}

	pCurNode = m_pRootNode;
	//  Insert -> Tail   .....   Head -> Delete
	queue.InsertFromTail(pCurNode);

	while (!queue.IsEmpty())
	{
		queue.GetHeadValue(pCurNode);
		// ����ͷ��ȡ����
		if (queue.DeleteFromHead())
		{
			cout << pCurNode->m_data << " ";
			if (pCurNode->m_pLeftChild)
			{
				queue.InsertFromTail(pCurNode->m_pLeftChild);
			}
			if (pCurNode->m_pRightChild)
			{
				queue.InsertFromTail(pCurNode->m_pRightChild);
			}	
		}
	}
	cout << endl;

	return;
}

template <typename T>
void CAVLTree<T>::PreOrderTraverse() const
{
	CStack<pTreeNode> stack;
	pTreeNode pCurTreeNode = nullptr;

	if (!m_pRootNode)
	{
		return;
	}
	pCurTreeNode = m_pRootNode;

	stack.Push(pCurTreeNode);
	while (!stack.IsEmpty())
	{
		pCurTreeNode = stack.GetTop();
		stack.Pop();
		if (pCurTreeNode)
		{
			cout << pCurTreeNode->m_data << " ";
			stack.Push(pCurTreeNode->m_pRightChild);
			stack.Push(pCurTreeNode->m_pLeftChild);
		}
	}
	cout << endl;
	//CStack<pTreeNode> stack;
	//pTreeNode pNode = m_pRootNode;

	//if (!pNode)
	//{
	//	return;
	//}

	//while (true)
	//{
	//	// �����Ӷ�ѹ��ջ��
	//	while (pNode)
	//	{
	//		cout << pNode->m_data << " ";
	//		stack.Push(pNode);
	//		pNode = pNode->m_pLeftChild;
	//	}

	//	// ջ���˾��˳�ѭ��
	//	if (stack.IsEmpty())
	//	{
	//		return;
	//	}

	//	do
	//	{
	//		// ����ջ��Ԫ��
	//		pNode = stack.GetTop();
	//		if (!pNode)
	//		{
	//			break;
	//		}
	//		stack.Pop();
	//		// �ҵ�ջ��Ԫ���Һ���
	//		pNode = pNode->m_pRightChild;
	//	} while (!pNode);
	//}
	//
	//return;
}


template <typename T>
void CAVLTree<T>::InOrderTraverse() const
{
	CStack<pTreeNode> stack;
	pTreeNode pCurNode = nullptr;

	if (!m_pRootNode)
	{
		return;
	}
	pCurNode = m_pRootNode;

	while (pCurNode || !stack.IsEmpty())
	{
		if (pCurNode)
		{
			stack.Push(pCurNode);
			pCurNode = pCurNode->m_pLeftChild;
		}
		else
		{
			pCurNode = stack.GetTop();
			stack.Pop();
			cout << pCurNode->m_data << " ";
			pCurNode = pCurNode->m_pRightChild;
		}
	}
	cout << endl;
}

template <typename T>
void CAVLTree<T>::PostOrderTraverse() const
{
	CStack<pTreeNode> stack;
	CStack<T> tmpStack;
	pTreeNode pCurTreeNode = nullptr;

	pCurTreeNode = m_pRootNode;
	stack.Push(pCurTreeNode);

	while (!stack.IsEmpty())
	{
		pCurTreeNode = stack.GetTop();
		stack.Pop();

		if (pCurTreeNode)
		{
			tmpStack.Push(pCurTreeNode->m_data);
			stack.Push(pCurTreeNode->m_pLeftChild);
			stack.Push(pCurTreeNode->m_pRightChild);
		}
	}
	tmpStack.ReverseStack();
	tmpStack.PrintStack();
}


template <typename T>
void CAVLTree<T>::ReleaseTree()
{
	CQueue<pTreeNode> queue;
	pTreeNode pCurNode = nullptr;

	if (!m_pRootNode)
	{
		return;
	}

	pCurNode = m_pRootNode;
	//  Insert -> Tail   .....   Head -> Delete
	queue.InsertFromTail(pCurNode);

	while (!queue.IsEmpty())
	{
		queue.GetHeadValue(pCurNode);
		// ����ͷ��ȡ����
		if (queue.DeleteFromHead())
		{
			if (pCurNode->m_pLeftChild)
			{
				queue.InsertFromTail(pCurNode->m_pLeftChild);
			}
			if (pCurNode->m_pRightChild)
			{
				queue.InsertFromTail(pCurNode->m_pRightChild);
			}
			delete pCurNode;
			pCurNode = nullptr;
		}
	}
	m_pRootNode = nullptr;

	return;
}

template <typename T>
bool CAVLTree<T>::CreateTree(const T *pDataAry, size_t nDataArySize)
{
	bool fOk = false;

	for (size_t nIdx = 0; nIdx < nDataArySize; ++nIdx)
	{
		fOk = InsertNode(pDataAry[nIdx]);

		if (!fOk)
		{

		}
	}
}

template <typename T>
bool CAVLTree<T>::FindNode(const T rcDataToFind)
{
	TreeNode CurTreeNode(rcDataToFind);

	return(FindNode(CurTreeNode, rcDataToFind));
}

template <typename T>
bool CAVLTree<T>::FindNode(TreeNode &rNode, const T rcDataToFind)
{
	pTreeNode pCurNode = m_pRootNode;
	T CurNodeData;

	if (!pCurNode)
	{
		return(false);
	}

	while (pCurNode)
	{
		// ��ȡ��ǰ�ڵ�����
		CurNodeData = pCurNode->m_data;
		// �������ݴ�С�ڶ��������ƶ�
		if (CurNodeData > rcDataToFind)
		{
			pCurNode = pCurNode->m_pLeftChild;
		}
		else if (CurNodeData < rcDataToFind)
		{
			pCurNode = pCurNode->m_pRightChild;
		}
		else 
		{
			// �ҵ����ȡ������
			rNode = *pCurNode;
			return(true);
		}
	}
	
	return(false);
}



template <typename T>
bool CAVLTree<T>::InsertNode(T data)
{
	pTreeNode pCurNode = m_pRootNode;
	pTreeNode pParentNode = nullptr;
	pTreeNode pNewNode = nullptr;

	do
	{
		// �����½��
		pNewNode = new TreeNode(data);
		if (!pNewNode)
		{
			break;
		}
		// �������û���κν��, �嵽���ڵ㷵��
		if (!pCurNode)
		{
			m_pRootNode = pNewNode;

			return(true);
		}

		// �����н��, ����BST��С�Ҵ�Ĺ���
		while (pCurNode)
		{
			// ����ǰ���
			pParentNode = pCurNode;
			if (pCurNode->m_data > data)
			{
				pCurNode = pCurNode->m_pLeftChild;
			}
			else if (pCurNode->m_data < data)
			{
				pCurNode = pCurNode->m_pRightChild;
			}
			else
			{
				return(false);
			}
		}
		// �жϲ����λ�ò������½��
		if (pParentNode->m_data > data)
		{
			pParentNode->m_pLeftChild = pNewNode;
			pNewNode->m_pParent = pParentNode;
		}
		else
		{
			pParentNode->m_pRightChild = pNewNode;
			pNewNode->m_pParent = pParentNode;
		}
		AdjustHeight(pNewNode);
		
	} while (false);

	return(true);
}