// ƽ�����������ʵ��
#pragma once
#include "CStack.hpp"
#include "CQueue.hpp"
#include "MD5.h"

// ���������
#define ENDMARK "j4Sx"

template <typename T>
class CAVLTree
{
public:
	// �������Ľ��ṹ
	typedef struct tagTreeNode
	{
		tagTreeNode() = default;
		tagTreeNode(T data, size_t nID) :
			m_data(data), m_pLeftChild(nullptr), 
			m_pRightChild(nullptr), m_pParent(nullptr), m_nHeight(1),
			m_nID(nID)
		{ }
		size_t m_nID;						// ����ֵ
		T m_data;							// �û����������
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
	bool FindNodeByID(size_t nID);
	// ͨ�����ֵ(nID)�ҵ������Я������Ϣ
	bool FindNodeByID(size_t nID, T &rDataToFind);
	// ͨ�����ֵ(nID)�ҵ�ָ������Я������Ϣ��ָ��
	T *FindNodeDataPtrByID(size_t nID);
	// �ӿγ�AVL����ɾ��ѡ���˸ÿγ̵�����ѧ����¼
	bool DeleteStuChosenCourseFromCourseInfoTree(const char *pcszCourseName,
		size_t nCourseNameHash, const char *pcszStuName);
	// ��ѧ��AVL����ɾ��ѡ���˸ÿγ̵����м�¼
	bool DeleteCoursePointRecFromStudentInfoTree(size_t nStudentID, const char *pcszCoName);
	// ͨ��ѧ������g_stSrhStudentNameTree���ҵ���Ӧ��ID��
	// ����ѧ�������������Կ����ж��ID�Ŵ���
	bool FindStudentIDByStudentName(const char *pcszStudentName,
		size_t nHash,
		CArray<size_t> &cIDAry);
	// ͨ���γ�����stSearchIDByCourseName���ҵ���Ӧ��ID��
	unsigned int FindCourseIDByCourseName(const char *pcszCourseName,
		size_t nHash,
		bool &fSuccess);

	// ͨ�����ֵ(nID)�ҵ�ָ�����ָ��
	pTreeNode FindNodePtrByID(size_t nID)
	{
		pTreeNode pCurNode = nullptr;

		pCurNode = m_pRootNode;

		while (pCurNode)
		{
			if (pCurNode->m_nID > nID)
			{
				pCurNode = pCurNode->m_pLeftChild;
			}
			else if (pCurNode->m_nID < nID)
			{
				pCurNode = pCurNode->m_pRightChild;
			}
			else
			{
				return(pCurNode);
			}
		}

		return(nullptr);
	}
	// ͨ���γ������ҵ����
	pTreeNode FindNodeByCourseName(const char *pcszCourseName, size_t nHash, bool &fSuccess)
	{
		pTreeNode pCurNode = nullptr;
		pstCourse pstCo = nullptr;
		unsigned int uiCourseID = 0;
		unsigned int nID = 0;

		pCurNode = m_pRootNode;

		if (!pCurNode)
		{
			return(nullptr);
		}
		
		nID = FindCourseIDByCourseName(pcszCourseName, nHash, fSuccess);
		if (!fSuccess)
		{
			return(nullptr);
		}
		return(FindNodePtrByID(nID));
	}
	// ͨ��ID�����Ҷ�Ӧ���ָ��
	pTreeNode FindNodeByID(size_t nID, bool &fSuccess)
	{
		pTreeNode pCurNode = nullptr;

		pCurNode = m_pRootNode;
		if (!pCurNode)
		{
			return(nullptr);
		}

		while (pCurNode)
		{
			if (pCurNode->m_nID > nID)
			{
				pCurNode = pCurNode->m_pLeftChild;
			}
			else if (pCurNode->m_nID < nID)
			{
				pCurNode = pCurNode->m_pRightChild;
			}
			else
			{
				fSuccess = true;
				return(pCurNode);
			}
		}
		fSuccess = false;

		return(nullptr);
	}
	
	// ������
	bool InsertNode(T data, size_t nID);
	// ɾ�����
	bool DeleteNode(const T &rcdata);
	// ͨ��IDɾ��ѧ�����
	bool DeleteNode(const size_t nHash, const size_t nID, bool fCourse = TRUE);
	// ͨ��IDɾ��ѧ����㲢���ظ�ѧ����ѡ�γ�
	bool DeleteNodeStu(const size_t nID, CArray<char *> &cCourseNameAry, bool fCourse = true);
	// ͨ��IDɾ���γ̽�㲢���ظ�ѡ��ÿγ̵�ѧ��ID����
	bool DeleteNodeCo(const size_t nID, size_t *&prStuID, size_t &nBufSize, bool fCourse = true);
	// ����һ������nDataArySize��Ԫ�ص���
	bool CreateTree(const T *pDataAry, size_t nDataArySize);
	// ִ��ǰ�����
	void PreOrderTraverse() const;
	// ִ���������
	void InOrderTraverse() const;
	// ִ�к�������
	void PostOrderTraverse() const;
	// ִ�в��������ӡ�γ���Ϣ
	void LevelOrderTraverseCourseInfo() const;
	// ִ�в��������ӡѧ����Ϣ
	void LevelOrderTraverseStudentInfo() const;
public:
	// IO����
	// ��g_stSrhCourseNameTree���ļ�������ռ��С�ĺ���
	unsigned long long CalcAllHeapMemoryAcquiredCourse() const;
	// ��g_stSrhCourseNameTree��ȡ���ݲ���һ����ʽ����
	unsigned char *AllocHeapMemoryAndSetSrhCourseData(unsigned long long &rullSetSize);
	// ��g_stSrhStudentNameTree���ļ�������ռ��С�ĺ���
	unsigned long long CalcAllHeapMemoryAcquiredStudent() const;
	// ��g_stSrhStudentNameTree��ȡ���ݲ���һ����ʽ����
	unsigned char *AllocHeapMemoryAndSetSrhStudentData(unsigned long long &rullSetSize);


	// ��g_stCourseInfoTree���ļ�������ռ��С�ĺ���
	unsigned long long CalcAllHeapMemoryAcquiredCourseX() const;
	// ��g_stCourseInfoTree��ȡ���ݲ���һ����ʽ����
	unsigned char *AllocHeapMemoryAndSetCourseDataX(unsigned long long &rullSetSize);
	// ��g_stStudentInfoTree���ļ�������ռ��С�ĺ���
	unsigned long long CalcAllHeapMemoryAcquiredStudentX() const;
	// ��g_stStudentInfoTree��ȡ���ݲ���һ����ʽ����
	unsigned char *AllocHeapMemoryAndSetStudentDataX(unsigned long long &rullSetSize);
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
public:
	// �ͷ���
	void ReleaseTree();
private:
	pTreeNode m_pRootNode;
};


template <typename T>
unsigned char * CAVLTree<T>::AllocHeapMemoryAndSetStudentDataX(
	unsigned long long &rullSetSize)
{
	unsigned char *pucBuf = nullptr;
	unsigned char *pucTmpBuf = nullptr;
	unsigned long long ullSize = 0;
	CQueue<pTreeNode> queue;
	stStuInfo stStu;
	pTreeNode pCurNode = nullptr;
	unsigned long long ullPtr = 0;
	unsigned int uiListSize = 0;
	size_t nLen = 0;

	// ����洢�ܹ���Ҫ���ڴ��С
	ullSize = CalcAllHeapMemoryAcquiredCourseX();
	if (!ullSize)
	{
		rullSetSize = 0;
		return(nullptr);
	}
	// ����ռ�
	pucBuf = new unsigned char[ullSize];
	if (!pucBuf)
	{
		rullSetSize = 0;
		return(nullptr);
	}
	pucTmpBuf = pucBuf;
	// ��ʼ�����������
	pCurNode = m_pRootNode;
	//  Insert -> Tail   .....   Head -> Delete
	queue.InsertFromTail(pCurNode);

	while (!queue.IsEmpty())
	{
		queue.GetHeadValue(pCurNode);
		// ����ͷ��ȡ����
		if (queue.DeleteFromHead())
		{
			stStu = *(stStuInfo *)(&pCurNode->m_data);
			// �洢IDֵ
			*((unsigned int *)pucBuf) = stStu.uiID;
			pucBuf += 4;
			ullPtr += 4;
			// �洢ѧ�����ĳ���
			nLen = stStu.strName.GetBufLen() - 1;
			*((unsigned int *)pucBuf) = nLen;
			pucBuf += 4;
			ullPtr += 4;
			// �洢�γ���
			memcpy(pucBuf, stStu.strName.GetString(), nLen);
			pucBuf += nLen;
			ullPtr += nLen;
			// �洢������Ϣ
			uiListSize = stStu.lstOfCourse.SetStudentListDataToBufX(pucBuf);
			ullPtr += uiListSize;
			pucBuf += uiListSize;
			// �洢�������
			memcpy(pucBuf, ENDMARK, 4);
			ullPtr += 4;
			pucBuf += 4;
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
	rullSetSize = ullPtr;

	return(pucTmpBuf);
}


template <typename T>
unsigned char * CAVLTree<T>::AllocHeapMemoryAndSetCourseDataX(
	unsigned long long &rullSetSize)
{
	unsigned char *pucBuf = nullptr;
	unsigned char *pucTmpBuf = nullptr;
	unsigned long long ullSize = 0;
	CQueue<pTreeNode> queue;
	stCourse stCo;
	pTreeNode pCurNode = nullptr;
	unsigned long long ullPtr = 0;
	unsigned int uiListSize = 0;
	size_t nLen = 0;

	// ����洢�ܹ���Ҫ���ڴ��С
	ullSize = CalcAllHeapMemoryAcquiredCourseX();
	if (!ullSize)
	{
		rullSetSize = 0;
		return(nullptr);
	}
	// ����ռ�
	pucBuf = new unsigned char[ullSize];
	if (!pucBuf)
	{
		rullSetSize = 0;
		return(nullptr);
	}
	pucTmpBuf = pucBuf;
	// ��ʼ�����������
	pCurNode = m_pRootNode;
	//  Insert -> Tail   .....   Head -> Delete
	queue.InsertFromTail(pCurNode);

	while (!queue.IsEmpty())
	{
		queue.GetHeadValue(pCurNode);
		// ����ͷ��ȡ����
		if (queue.DeleteFromHead())
		{
			stCo = *(stCourse *)(&pCurNode->m_data);
			// �洢IDֵ
			*((unsigned int *)pucBuf) = stCo.uiCourseID;
			pucBuf += 4;
			ullPtr += 4;
			// �洢�γ����ĳ���
			nLen = stCo.strCourseName.GetBufLen() - 1;
			*((unsigned int *)pucBuf) = nLen;
			pucBuf += 4;
			ullPtr += 4;
			// �洢�γ���
			memcpy(pucBuf, stCo.strCourseName.GetString(), nLen);
			pucBuf += nLen;
			ullPtr += nLen;
			// �洢������Ϣ
			uiListSize = stCo.lstOfStu.SetCourseListDataToBufX(pucBuf);
			ullPtr += uiListSize;
			pucBuf += uiListSize;
			// �洢�������
			memcpy(pucBuf, ENDMARK, 4);
			ullPtr += 4;
			pucBuf += 4;
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
	rullSetSize = ullPtr;

	return(pucTmpBuf);
}


template <typename T>
unsigned long long CAVLTree<T>::CalcAllHeapMemoryAcquiredStudentX() const
{
	CQueue<pTreeNode> queue;
	stStuInfo stStu;
	pTreeNode pCurNode = nullptr;
	unsigned long long ullTotalSize = 0;
	unsigned int uiSingleNodeSize = 0;

	if (!m_pRootNode)
	{
		return(0);
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
			stStu = *(stStuInfo *)&pCurNode->m_data;
			// 4�ֽ��ǽ��������, 4�ֽ����ַ������ȴ�С
			uiSingleNodeSize = 4 + 4 + (stStu.strName.GetBufLen() - 1) +
				sizeof(stStu.uiID) + stStu.lstOfCourse.GetStudentListSizeX();
			ullTotalSize += uiSingleNodeSize;
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

	return(ullTotalSize);
}


template <typename T>
unsigned long long CAVLTree<T>::CalcAllHeapMemoryAcquiredCourseX() const
{
	CQueue<pTreeNode> queue;
	stCourse stCo;
	pTreeNode pCurNode = nullptr;
	unsigned long long ullTotalSize = 0;
	unsigned int uiSingleNodeSize = 0;

	if (!m_pRootNode)
	{
		return(0);
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
			stCo = *(stCourse *)&pCurNode->m_data;
			// 4�ֽ��ǽ��������, 4�ֽ����ַ������ȴ�С
			uiSingleNodeSize = 4 + 4 + (stCo.strCourseName.GetBufLen() - 1) + 
				sizeof(stCo.uiCourseID) + stCo.lstOfStu.GetCourseListSizeX();
			ullTotalSize += uiSingleNodeSize;
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

	return(ullTotalSize);
}


template <typename T>
unsigned char * CAVLTree<T>::AllocHeapMemoryAndSetSrhStudentData(unsigned long long &rullSetSize)
{
	unsigned char *pucBuf = nullptr;
	unsigned char *pucTmpBuf = nullptr;
	unsigned long long ullSize = 0;
	CQueue<pTreeNode> queue;
	stSearchIDByStudentName stSrhIDByStuName;
	pTreeNode pCurNode = nullptr;
	unsigned long long ullPtr = 0;
	unsigned int uiListSize = 0;

	// ����洢�ܹ���Ҫ���ڴ��С
	ullSize = CalcAllHeapMemoryAcquiredStudent();
	if (!ullSize)
	{
		rullSetSize = 0;
		return(nullptr);
	}
	// ����ռ�
	pucBuf = new unsigned char[ullSize];
	if (!pucBuf)
	{
		rullSetSize = 0;
		return(nullptr);
	}
	pucTmpBuf = pucBuf;
	// ��ʼ�����������
	pCurNode = m_pRootNode;
	//  Insert -> Tail   .....   Head -> Delete
	queue.InsertFromTail(pCurNode);

	while (!queue.IsEmpty())
	{
		queue.GetHeadValue(pCurNode);
		// ����ͷ��ȡ����
		if (queue.DeleteFromHead())
		{
			stSrhIDByStuName = *(stSearchIDByStudentName *)&pCurNode->m_data;
			// �洢Hashֵ
			*((unsigned int *)pucBuf) = stSrhIDByStuName.nNameHash;
			pucBuf += 4;
			ullPtr += 4;
			// �洢������Ϣ
			uiListSize = stSrhIDByStuName.lstStudentName.SetStudentListDataToBuf(pucBuf);
			ullPtr += uiListSize;
			pucBuf += uiListSize;
			// �洢�������
			memcpy(pucBuf, ENDMARK, 4);
			ullPtr += 4;
			pucBuf += 4;
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
	rullSetSize = ullPtr;

	return(pucTmpBuf);
}


template <typename T>
unsigned long long CAVLTree<T>::CalcAllHeapMemoryAcquiredStudent() const
{
	CQueue<pTreeNode> queue;
	stSearchIDByStudentName stSrhIDByStu;
	pTreeNode pCurNode = nullptr;
	unsigned long long ullTotalSize = 0;
	unsigned int uiSingleNodeSize = 0;

	if (!m_pRootNode)
	{
		return(0);
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
			stSrhIDByStu = *(stSearchIDByStudentName *)&pCurNode->m_data;
			// ��4���ֽ���Hashֵռ�ݵ����ݴ�С, ����4�ֽ��ǽ��������
			uiSingleNodeSize = 4 + 4 + stSrhIDByStu.lstStudentName.GetStudentListSize();
			ullTotalSize += uiSingleNodeSize;
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

	return(ullTotalSize);
}


template <typename T>
unsigned char * CAVLTree<T>::AllocHeapMemoryAndSetSrhCourseData(unsigned long long &rullSetSize)
{
	unsigned char *pucBuf = nullptr;
	unsigned char *pucTmpBuf = nullptr;
	unsigned long long ullSize = 0;
	CQueue<pTreeNode> queue;
	stSearchIDByCourseName stSrhIDByCoName;
	pTreeNode pCurNode = nullptr;
	unsigned long long ullPtr = 0;
	unsigned int uiListSize = 0;

	// ����洢�ܹ���Ҫ���ڴ��С
	ullSize = CalcAllHeapMemoryAcquiredCourse();
	if (!ullSize)
	{
		rullSetSize = 0;
		return(nullptr);
	}
	// ����ռ�
	pucBuf = new unsigned char[ullSize];
	if (!pucBuf)
	{
		rullSetSize = 0;
		return(nullptr);
	}
	pucTmpBuf = pucBuf;
	// ��ʼ�����������
	pCurNode = m_pRootNode;
	//  Insert -> Tail   .....   Head -> Delete
	queue.InsertFromTail(pCurNode);

	while (!queue.IsEmpty())
	{
		queue.GetHeadValue(pCurNode);
		// ����ͷ��ȡ����
		if (queue.DeleteFromHead())
		{
			stSrhIDByCoName = *(stSearchIDByCourseName *)(&pCurNode->m_data);
			// �洢Hashֵ
			*((unsigned int *)pucBuf) = stSrhIDByCoName.nNameHash;
			pucBuf += 4;
			ullPtr += 4;
			// �洢������Ϣ
			uiListSize = stSrhIDByCoName.lstCourseName.SetCourseListDataToBuf(pucBuf);
			ullPtr += uiListSize;
			pucBuf += uiListSize;
			// �洢�������
			memcpy(pucBuf, ENDMARK, 4);
			ullPtr += 4;
			pucBuf += 4;
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
	rullSetSize = ullPtr;

	return(pucTmpBuf);
}

template <typename T>
unsigned long long CAVLTree<T>::CalcAllHeapMemoryAcquiredCourse() const
{
	CQueue<pTreeNode> queue;
	stSearchIDByCourseName stSrhIDByCo;
	pTreeNode pCurNode = nullptr;
	unsigned long long ullTotalSize = 0;
	unsigned int uiSingleNodeSize = 0;

	if (!m_pRootNode)
	{
		return(0);
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
			stSrhIDByCo = *(stSearchIDByCourseName *)&pCurNode->m_data;
			// ��4���ֽ���Hashֵռ�ݵ����ݴ�С, ����4�ֽ��ǽ��������
			uiSingleNodeSize = 4 + 4 + stSrhIDByCo.lstCourseName.GetCourseListSize();
			ullTotalSize += uiSingleNodeSize;
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

	return(ullTotalSize);
}

template <typename T>
void CAVLTree<T>::LevelOrderTraverseStudentInfo() const
{
	CQueue<pTreeNode> queue;
	pstStuInfo pStudentInfo = nullptr;
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
			pStudentInfo = (pstStuInfo)&pCurNode->m_data;
			cout << "ѧ����: " << pStudentInfo->strName.GetString() << endl;
			cout << "ѧ��ID: " << pStudentInfo->uiID << endl;
			cout << endl;
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
unsigned int 
CAVLTree<T>::FindCourseIDByCourseName(const char *pcszCourseName, 
	size_t nHash, 
	bool &fSuccess)
{
	stSearchIDByCourseName stSrhCoName;
	size_t nID = 0;

	fSuccess = false;
	// ͨ��HASHֵ���Ҷ�Ӧ�γ���Ϣ
	fSuccess = g_stSrhCourseNameTree.FindNodeByID(nHash, stSrhCoName);
	// ͨ���γ�����ѯID��
	if (fSuccess)
	{
		// ��hash���ж����Ӧ�Ŀγ�, ͨ���γ�����һ��ȷ��
		fSuccess = stSrhCoName.lstCourseName.FindCoId(pcszCourseName, nID);
		if (fSuccess)
		{
			return(nID);
		}
	}

	return(0);
}


template <typename T>
bool
CAVLTree<T>::FindStudentIDByStudentName(const char *pcszStudentName, 
	size_t nHash, 
	CArray<size_t> &cIDAry)
{
	stSearchIDByStudentName stSrhStuName;
	size_t nID = 0;
	bool fSuccess = false;

	// ͨ��HASHֵ���Ҷ�Ӧѧ����Ϣ
	fSuccess = g_stSrhStudentNameTree.FindNodeByID(nHash, stSrhStuName);
	// ͨ��ѧ������ѯID��
	if (fSuccess)
	{
		CArray<size_t> cAry;
		// ��hash���ж����Ӧ��ѧ��, ͨ��ѧ����һ��ȷ��
		fSuccess = stSrhStuName.lstStudentName.FindStuId(pcszStudentName, cAry);
		if (fSuccess && cAry.GetSize() > 0)
		{
			// ���ܻ���ͬ����ѧ��
			cIDAry = cAry;
			return(fSuccess);
		}
	}

	return(fSuccess);
}


template <typename T>
bool CAVLTree<T>::DeleteCoursePointRecFromStudentInfoTree(
	size_t nStudentID, 
	const char *pcszCoName)
{
	bool fOk = false;
	unsigned int uiID = 0;
	pTreeNode pNode = nullptr;
	pstStuInfo pstStu = nullptr;

	if (!pcszCoName)
	{
		return(false);
	}
	// ͨ��ѧ�����ҵ�AVLѧ�����
	pNode = FindNodePtrByID(nStudentID);
	if (!pNode)
	{
		return(false);
	}
	pstStu = (pstStuInfo)&pNode->m_data;
	// ͨ�����˸ÿγ̵�ѧ����ɾ��AVL�γ������е�ѡ��ѧ��������
	fOk = pstStu->lstOfCourse.DeleteCourseRecordFromStudentTreeCourseList(pcszCoName);

	return(fOk);
}

template <typename T>
bool CAVLTree<T>::DeleteStuChosenCourseFromCourseInfoTree(
	const char *pcszCourseName, 
	size_t nCourseNameHash,
	const char *pcszStuName)
{
	bool fOk = false;
	unsigned int uiID = 0;
	pTreeNode pNode = nullptr;
	pstCourse pstCo = nullptr;

	if (!pcszCourseName || !pcszStuName)
	{
		return(false);
	}
	// ͨ���γ����ҵ�AVL�γ̽��
	pNode = FindNodeByCourseName(pcszCourseName, nCourseNameHash, fOk);
	if (!pNode)
	{
		return(false);
	}
	pstCo = (pstCourse)&pNode->m_data;
	// ͨ�����˸ÿγ̵�ѧ����ɾ��AVL�γ������е�ѡ��ѧ��������
	fOk = pstCo->lstOfStu.DeleteStudentRecordFromCourseTreeStudentList(pcszStuName);

	return(fOk);
}


template <typename T>
bool CAVLTree<T>::DeleteNodeCo(const size_t nID, 
	size_t *&prStuID, 
	size_t &nBufSize, 
	bool fCourse/* = true*/)
{
	pTreeNode pDelNode = nullptr;
	pTreeNode pParentNodeOfDelNode = nullptr;
	pTreeNode pCurNode = nullptr;
	pstCourse pCoInfo = nullptr;
	bool fOk = false;

	// ����ֱ�ӷ���false
	if (!m_pRootNode)
	{
		return(false);
	}
	// ����Ҫɾ���Ľ��, û���ҵ�ֱ�ӷ���nullptr
	pDelNode = FindNodeByID(nID, fOk);
	if (!fOk)
	{
		return(false);
	}
	pCoInfo = (pstCourse)&pDelNode->m_data;
	pCoInfo->lstOfStu.GetStudentListNamesAry(prStuID, nBufSize);

	// ����Ǹ��ڵ���ֱ��ɾ����ɹ�
	if (m_pRootNode == pDelNode)
	{
		delete m_pRootNode;
		m_pRootNode = nullptr;
		return(true);
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
			if (fCourse)
			{
				pstCourse pstCo = (pstCourse)&pDelNode->m_data;
				pDelNode->m_nID = pstCo->uiCourseID;
			}
			else
			{
				pstStuInfo pstStu = (pstStuInfo)&pDelNode->m_data;
				pDelNode->m_nID = pstStu->uiID;
			}
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
bool CAVLTree<T>::DeleteNodeStu(const size_t nID, 
	CArray<char *> &cCourseNameAry, 
	bool fCourse/* = true*/)
{
	pTreeNode pDelNode = nullptr;
	pTreeNode pParentNodeOfDelNode = nullptr;
	pTreeNode pCurNode = nullptr;
	pstStuInfo pStuInfo = nullptr;
	bool fOk = false;

	// ����ֱ�ӷ���false
	if (!m_pRootNode)
	{
		return(false);
	}
	// ����Ҫɾ���Ľ��, û���ҵ�ֱ�ӷ���nullptr
	pDelNode = FindNodeByID(nID, fOk);
	if (!fOk)
	{
		return(false);
	}
	pStuInfo = (pstStuInfo)&pDelNode->m_data;
	pStuInfo->lstOfCourse.GetCourseListNamesAry(cCourseNameAry);

	// ����Ǹ��ڵ���ֱ��ɾ����ɹ�
	if (m_pRootNode == pDelNode)
	{
		delete m_pRootNode;
		m_pRootNode = nullptr;
		return(true);
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
			if (fCourse)
			{
				pstCourse pstCo = (pstCourse)&pDelNode->m_data;
				pDelNode->m_nID = pstCo->uiCourseID;
			}
			else
			{
				pstStuInfo pstStu = (pstStuInfo)&pDelNode->m_data;
				pDelNode->m_nID = pstStu->uiID;
			}
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
			if (fCourse)
			{
				pstCourse pstCo = (pstCourse)&pDelNode->m_data;
				pDelNode->m_nID = pstCo->uiCourseID;
			}
			else
			{
				pstStuInfo pstStu = (pstStuInfo)&pDelNode->m_data;
				pDelNode->m_nID = pstStu->uiID;
			}
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
T * CAVLTree<T>::FindNodeDataPtrByID(size_t nID)
{
	pTreeNode pCurNode = nullptr;

	pCurNode = m_pRootNode;

	while (pCurNode)
	{
		if (pCurNode->m_nID > nID)
		{
			pCurNode = pCurNode->m_pLeftChild;
		}
		else if (pCurNode->m_nID < nID)
		{
			pCurNode = pCurNode->m_pRightChild;
		}
		else
		{
			return(&pCurNode->m_data);
		}
	}

	return(nullptr);
}

template <typename T>
bool CAVLTree<T>::DeleteNode(const size_t nHash, const size_t nID, bool fCourse/* = TRUE*/)
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
	// ����Ҫɾ���Ľ��, û���ҵ�ֱ�ӷ���false
	pDelNode = FindNodeByID(nHash, fOk);
	if (!fOk)
	{
		return(false);
	}
	// Srh��
	if (fCourse)
	{
		pstSearchIDByCourseName pstSrhIDCo = (pstSearchIDByCourseName)&pDelNode->m_data;
		if (pstSrhIDCo->lstCourseName.GetSize() != 1)
		{
			fOk = pstSrhIDCo->lstCourseName.DeleteByID(nID, true);
			return(fOk);
		}
	}
	else
	{
		pstSearchIDByStudentName pstSrhIDStu = (pstSearchIDByStudentName)&pDelNode->m_data;
		if (pstSrhIDStu->lstStudentName.GetSize() != 1)
		{
			fOk = pstSrhIDStu->lstStudentName.DeleteByID(nID, false);
			return(fOk);
		}
	}
	// ����Ǹ��ڵ���ֱ��ɾ����ɹ�
	if (m_pRootNode == pDelNode)
	{
		delete m_pRootNode;
		m_pRootNode = nullptr;
		return(true);
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
bool CAVLTree<T>::FindNodeByID(size_t nID, T &rDataToFind)
{
	pTreeNode pCurNode = nullptr;

	pCurNode = m_pRootNode;

	while (pCurNode)
	{
		if (pCurNode->m_nID > nID)
		{
			pCurNode = pCurNode->m_pLeftChild;
		}
		else if (pCurNode->m_nID < nID)
		{
			pCurNode = pCurNode->m_pRightChild;
		}
		else
		{
			rDataToFind = pCurNode->m_data;
			return(true);
		}
	}

	return(false);
}

template <typename T>
bool CAVLTree<T>::FindNodeByID(size_t nID)
{
	pTreeNode pCurNode = nullptr;

	pCurNode = m_pRootNode;

	while (pCurNode)
	{
		if (pCurNode->m_nID > nID)
		{
			pCurNode = pCurNode->m_pLeftChild;
		}
		else if (pCurNode->m_nID < nID)
		{
			pCurNode = pCurNode->m_pRightChild;
		}
		else
		{
			return(true);
		}
	}

	return(false);
}

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
void CAVLTree<T>::LevelOrderTraverseCourseInfo() const
{
	CQueue<pTreeNode> queue;
	pstCourse pCourseInfo = nullptr;
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
			pCourseInfo = (pstCourse)&pCurNode->m_data;
			cout << "�γ���: " << pCourseInfo->strCourseName.GetString() << endl;
			cout << "�γ�ID: " << pCourseInfo->uiCourseID << endl;
			cout << endl;
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
bool CAVLTree<T>::InsertNode(T data, size_t nID)
{
	pTreeNode pCurNode = m_pRootNode;
	pTreeNode pParentNode = nullptr;
	pTreeNode pNewNode = nullptr;

	do
	{
		// �����½��
		pNewNode = new TreeNode(data, nID);
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
			if (pCurNode->m_nID > nID)
			{
				pCurNode = pCurNode->m_pLeftChild;
			}
			else if (pCurNode->m_nID < nID)
			{
				pCurNode = pCurNode->m_pRightChild;
			}
			else
			{
				return(false);
			}
		}
		// �жϲ����λ�ò������½��
		if (pParentNode->m_nID > nID)
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