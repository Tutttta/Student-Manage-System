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
	// 搜索树的结点结构
	typedef struct tagTreeNode
	{
		tagTreeNode() = default;
		tagTreeNode(T data) :
			m_data(data), m_pLeftChild(nullptr), 
			m_pRightChild(nullptr), m_pParent(nullptr), m_nHeight(1)
		{ }
		T m_data;
		struct tagTreeNode *m_pLeftChild;   // 左孩子指针
		struct tagTreeNode *m_pRightChild;  // 右孩子指针
		struct tagTreeNode *m_pParent;		// 父结点指针
		size_t m_nHeight;					// 结点默认高度
	} TreeNode, *pTreeNode;
public:
	CAVLTree() : m_pRootNode(nullptr) {}
	~CAVLTree();
	// 寻找结点, 寻找值为rcDataToFind的结点并将结点的引用通过参数传回
	bool FindNode(TreeNode &rNode, const T rcDataToFind);
	bool FindNode(pTreeNode *ppNode, const T rcDataToFind);
	// 判定值为rcDataToFind的结点是否存在
	bool FindNode(const T rcDataToFind);
	// 插入结点
	bool InsertNode(T data);
	// 删除结点
	bool DeleteNode(const T &rcdata);
	// 创建一个具有nDataArySize个元素的树
	bool CreateTree(const T *pDataAry, size_t nDataArySize);
	// 执行前序遍历
	void PreOrderTraverse() const;
	// 执行中序遍历
	void InOrderTraverse() const;
	// 执行后续遍历
	void PostOrderTraverse() const;
	// 执行层序遍历
	void LevelOrderTraverse() const;
private:
	// 为转成AVAL树调整高度
	void AdjustHeight(pTreeNode pNode);
	// 左旋
	void LeftRotate(pTreeNode pNode);
	// 右旋
	void RightRotate(pTreeNode pNode);
	// 获取当前结点高度
	size_t GetHeight(const pTreeNode pNode) const
	{
		return(pNode ? (pNode->m_nHeight) : 0);
	}
	// 内部用的max函数
	size_t MaxNodeValue(size_t nLeft, size_t nRight) 
	{
		return((nLeft >= nRight) ? nLeft : nRight);
	}
	// 重新结算当前结点高度
	size_t CalcHeight(pTreeNode pNode);
	// 释放树
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
		// 获取当前节点数据
		CurNodeData = pCurNode->m_data;
		// 根据数据大小在二叉树内移动
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
			// 找到后获取并返回
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

	// 如果当前结点的父结点不存在说明是根节点
	if (!pParentNode)
	{
		// 更新根结点指向
		m_pRootNode = pCurLNode;
	}
	else
	{
		// 如果不平衡结点的父结点存在则更新父结点的指向
		if (pCurNode == pParentNode->m_pLeftChild)
		{
			// 如果当前结点类型是左结点
			pParentNode->m_pLeftChild = pCurLNode;
		}
		else
		{
			// 如果当前结点类型是右结点
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
	// 如果不平衡结点是根节点
	if (!pParentNode)
	{
		// 更新根节点
		m_pRootNode = pCurRNode;
	}
	else
	{
		// 判定不平衡结点是其父节点的左孩子还是右孩子
		// 并按照此进行左旋
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
	// 重新计算高度
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
		// 计算当前结点的高度
		CalcHeight(pNode);
		// 判定当前结点的平衡因子
		iLeftChildHeight = GetHeight(pNode->m_pLeftChild);
		iRightChildHeight = GetHeight(pNode->m_pRightChild);
		
		if (iRightChildHeight - iLeftChildHeight > 1)
		{
			pTreeNode pRightNode = pNode->m_pRightChild;
			// RR型旋
			if (GetHeight(pRightNode->m_pLeftChild) <= GetHeight(pRightNode->m_pRightChild))
			{
				LeftRotate(pNode);
			}
			else
			{
				// RL型旋
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

	// 空树直接返回false
	if (!m_pRootNode)
	{
		return(false);
	}
	// 如果是根节点则直接删除后成功
	if (m_pRootNode->m_data == rcdata)
	{
		delete m_pRootNode;
		m_pRootNode = nullptr;
		return(true);
	}
	// 查找要删除的结点, 没有找到直接返回false
	fOk = FindNode(&pDelNode, rcdata);
	if (!fOk)
	{
		return(false);
	}
	// 找到父节点
	pParentNodeOfDelNode = pDelNode->m_pParent;
	// 该结点没有子节点
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
		// 找到结点左子树的最右结点
		while (pDelLeftAndMostRightChild)
		{
			pParentNode = pDelLeftAndMostRightChild;
			pDelLeftAndMostRightChild = pDelLeftAndMostRightChild->m_pRightChild;
		}
		if (!pParentNode)
		{
			// 如果左子结点没有右子树, 直接把左子结点的值给需要删除的结点, 然后删除左子结点
			// 获取需要删除的结点
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
			// 如果做左子节点存在右子树
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
		// 该结点存在1个子树
		if (pDelNode->m_pLeftChild)
		{
			// 被删除结点存在左子树
			if (pDelNode == pParentNodeOfDelNode->m_pLeftChild)
			{
				// 被删结点自身是左结点
				pDelNode->m_pParent->m_pLeftChild = pDelNode->m_pLeftChild;
				pDelNode->m_pLeftChild->m_pParent = pDelNode->m_pParent;
			}
			else
			{
				// 被删结点自身是右结点
				pDelNode->m_pParent->m_pRightChild = pDelNode->m_pLeftChild;
				pDelNode->m_pLeftChild->m_pParent = pDelNode->m_pParent;
			}
		}
		else
		{
			// 被删除结点存在右子树
			if (pDelNode == pParentNodeOfDelNode->m_pLeftChild)
			{
				// 被删结点自身是左结点
				pCurNode = pDelNode->m_pParent->m_pLeftChild;
				pDelNode->m_pParent->m_pLeftChild = pDelNode->m_pRightChild;
				pDelNode->m_pRightChild->m_pParent = pDelNode->m_pParent;
			}
			else
			{
				// 被删结点自身是右结点
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
		// 队列头部取数据
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
	//	// 把左孩子都压入栈内
	//	while (pNode)
	//	{
	//		cout << pNode->m_data << " ";
	//		stack.Push(pNode);
	//		pNode = pNode->m_pLeftChild;
	//	}

	//	// 栈空了就退出循环
	//	if (stack.IsEmpty())
	//	{
	//		return;
	//	}

	//	do
	//	{
	//		// 弹出栈顶元素
	//		pNode = stack.GetTop();
	//		if (!pNode)
	//		{
	//			break;
	//		}
	//		stack.Pop();
	//		// 找到栈顶元素右孩子
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
		// 队列头部取数据
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
		// 获取当前节点数据
		CurNodeData = pCurNode->m_data;
		// 根据数据大小在二叉树内移动
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
			// 找到后获取并返回
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
		// 分配新结点
		pNewNode = new TreeNode(data);
		if (!pNewNode)
		{
			break;
		}
		// 如果树内没有任何结点, 插到根节点返回
		if (!pCurNode)
		{
			m_pRootNode = pNewNode;

			return(true);
		}

		// 树内有结点, 根据BST左小右大的规则
		while (pCurNode)
		{
			// 保存前结点
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
		// 判断插入的位置并插入新结点
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