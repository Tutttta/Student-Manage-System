// 平衡二叉搜索树实现
#pragma once
#include "CStack.hpp"
#include "CQueue.hpp"
#include "MD5.h"

// 结束符标记
#define ENDMARK "j4Sx"

template <typename T>
class CAVLTree
{
public:
	// 搜索树的结点结构
	typedef struct tagTreeNode
	{
		tagTreeNode() = default;
		tagTreeNode(T data, size_t nID) :
			m_data(data), m_pLeftChild(nullptr), 
			m_pRightChild(nullptr), m_pParent(nullptr), m_nHeight(1),
			m_nID(nID)
		{ }
		size_t m_nID;						// 结点的值
		T m_data;							// 用户传入的数据
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
	bool FindNodeByID(size_t nID);
	// 通过结点值(nID)找到结点所携带的信息
	bool FindNodeByID(size_t nID, T &rDataToFind);
	// 通过结点值(nID)找到指向结点所携带的信息的指针
	T *FindNodeDataPtrByID(size_t nID);
	// 从课程AVL树上删除选择了该课程的所有学生记录
	bool DeleteStuChosenCourseFromCourseInfoTree(const char *pcszCourseName,
		size_t nCourseNameHash, const char *pcszStuName);
	// 从学生AVL树上删除选修了该课程的所有记录
	bool DeleteCoursePointRecFromStudentInfoTree(size_t nStudentID, const char *pcszCoName);
	// 通过学生名在g_stSrhStudentNameTree中找到对应的ID号
	// 由于学生可能重名所以可能有多个ID号存在
	bool FindStudentIDByStudentName(const char *pcszStudentName,
		size_t nHash,
		CArray<size_t> &cIDAry);
	// 通过课程名在stSearchIDByCourseName中找到对应的ID号
	unsigned int FindCourseIDByCourseName(const char *pcszCourseName,
		size_t nHash,
		bool &fSuccess);

	// 通过结点值(nID)找到指向结点的指针
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
	// 通过课程名来找到结点
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
	// 通过ID来查找对应结点指针
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
	
	// 插入结点
	bool InsertNode(T data, size_t nID);
	// 删除结点
	bool DeleteNode(const T &rcdata);
	// 通过ID删除学生结点
	bool DeleteNode(const size_t nHash, const size_t nID, bool fCourse = TRUE);
	// 通过ID删除学生结点并返回该学生所选课程
	bool DeleteNodeStu(const size_t nID, CArray<char *> &cCourseNameAry, bool fCourse = true);
	// 通过ID删除课程结点并返回该选择该课程的学生ID数组
	bool DeleteNodeCo(const size_t nID, size_t *&prStuID, size_t &nBufSize, bool fCourse = true);
	// 创建一个具有nDataArySize个元素的树
	bool CreateTree(const T *pDataAry, size_t nDataArySize);
	// 执行前序遍历
	void PreOrderTraverse() const;
	// 执行中序遍历
	void InOrderTraverse() const;
	// 执行后续遍历
	void PostOrderTraverse() const;
	// 执行层序遍历打印课程信息
	void LevelOrderTraverseCourseInfo() const;
	// 执行层序遍历打印学生信息
	void LevelOrderTraverseStudentInfo() const;
public:
	// IO部分
	// 对g_stSrhCourseNameTree做的计算所需空间大小的函数
	unsigned long long CalcAllHeapMemoryAcquiredCourse() const;
	// 从g_stSrhCourseNameTree读取数据并按一定格式保存
	unsigned char *AllocHeapMemoryAndSetSrhCourseData(unsigned long long &rullSetSize);
	// 对g_stSrhStudentNameTree做的计算所需空间大小的函数
	unsigned long long CalcAllHeapMemoryAcquiredStudent() const;
	// 从g_stSrhStudentNameTree读取数据并按一定格式保存
	unsigned char *AllocHeapMemoryAndSetSrhStudentData(unsigned long long &rullSetSize);


	// 对g_stCourseInfoTree做的计算所需空间大小的函数
	unsigned long long CalcAllHeapMemoryAcquiredCourseX() const;
	// 从g_stCourseInfoTree读取数据并按一定格式保存
	unsigned char *AllocHeapMemoryAndSetCourseDataX(unsigned long long &rullSetSize);
	// 对g_stStudentInfoTree做的计算所需空间大小的函数
	unsigned long long CalcAllHeapMemoryAcquiredStudentX() const;
	// 从g_stStudentInfoTree读取数据并按一定格式保存
	unsigned char *AllocHeapMemoryAndSetStudentDataX(unsigned long long &rullSetSize);
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
public:
	// 释放树
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

	// 计算存储总共需要的内存大小
	ullSize = CalcAllHeapMemoryAcquiredCourseX();
	if (!ullSize)
	{
		rullSetSize = 0;
		return(nullptr);
	}
	// 分配空间
	pucBuf = new unsigned char[ullSize];
	if (!pucBuf)
	{
		rullSetSize = 0;
		return(nullptr);
	}
	pucTmpBuf = pucBuf;
	// 开始遍历填充内容
	pCurNode = m_pRootNode;
	//  Insert -> Tail   .....   Head -> Delete
	queue.InsertFromTail(pCurNode);

	while (!queue.IsEmpty())
	{
		queue.GetHeadValue(pCurNode);
		// 队列头部取数据
		if (queue.DeleteFromHead())
		{
			stStu = *(stStuInfo *)(&pCurNode->m_data);
			// 存储ID值
			*((unsigned int *)pucBuf) = stStu.uiID;
			pucBuf += 4;
			ullPtr += 4;
			// 存储学生名的长度
			nLen = stStu.strName.GetBufLen() - 1;
			*((unsigned int *)pucBuf) = nLen;
			pucBuf += 4;
			ullPtr += 4;
			// 存储课程名
			memcpy(pucBuf, stStu.strName.GetString(), nLen);
			pucBuf += nLen;
			ullPtr += nLen;
			// 存储链表信息
			uiListSize = stStu.lstOfCourse.SetStudentListDataToBufX(pucBuf);
			ullPtr += uiListSize;
			pucBuf += uiListSize;
			// 存储结束标记
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

	// 计算存储总共需要的内存大小
	ullSize = CalcAllHeapMemoryAcquiredCourseX();
	if (!ullSize)
	{
		rullSetSize = 0;
		return(nullptr);
	}
	// 分配空间
	pucBuf = new unsigned char[ullSize];
	if (!pucBuf)
	{
		rullSetSize = 0;
		return(nullptr);
	}
	pucTmpBuf = pucBuf;
	// 开始遍历填充内容
	pCurNode = m_pRootNode;
	//  Insert -> Tail   .....   Head -> Delete
	queue.InsertFromTail(pCurNode);

	while (!queue.IsEmpty())
	{
		queue.GetHeadValue(pCurNode);
		// 队列头部取数据
		if (queue.DeleteFromHead())
		{
			stCo = *(stCourse *)(&pCurNode->m_data);
			// 存储ID值
			*((unsigned int *)pucBuf) = stCo.uiCourseID;
			pucBuf += 4;
			ullPtr += 4;
			// 存储课程名的长度
			nLen = stCo.strCourseName.GetBufLen() - 1;
			*((unsigned int *)pucBuf) = nLen;
			pucBuf += 4;
			ullPtr += 4;
			// 存储课程名
			memcpy(pucBuf, stCo.strCourseName.GetString(), nLen);
			pucBuf += nLen;
			ullPtr += nLen;
			// 存储链表信息
			uiListSize = stCo.lstOfStu.SetCourseListDataToBufX(pucBuf);
			ullPtr += uiListSize;
			pucBuf += uiListSize;
			// 存储结束标记
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
		// 队列头部取数据
		if (queue.DeleteFromHead())
		{
			stStu = *(stStuInfo *)&pCurNode->m_data;
			// 4字节是结束符标记, 4字节是字符串长度大小
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
		// 队列头部取数据
		if (queue.DeleteFromHead())
		{
			stCo = *(stCourse *)&pCurNode->m_data;
			// 4字节是结束符标记, 4字节是字符串长度大小
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

	// 计算存储总共需要的内存大小
	ullSize = CalcAllHeapMemoryAcquiredStudent();
	if (!ullSize)
	{
		rullSetSize = 0;
		return(nullptr);
	}
	// 分配空间
	pucBuf = new unsigned char[ullSize];
	if (!pucBuf)
	{
		rullSetSize = 0;
		return(nullptr);
	}
	pucTmpBuf = pucBuf;
	// 开始遍历填充内容
	pCurNode = m_pRootNode;
	//  Insert -> Tail   .....   Head -> Delete
	queue.InsertFromTail(pCurNode);

	while (!queue.IsEmpty())
	{
		queue.GetHeadValue(pCurNode);
		// 队列头部取数据
		if (queue.DeleteFromHead())
		{
			stSrhIDByStuName = *(stSearchIDByStudentName *)&pCurNode->m_data;
			// 存储Hash值
			*((unsigned int *)pucBuf) = stSrhIDByStuName.nNameHash;
			pucBuf += 4;
			ullPtr += 4;
			// 存储链表信息
			uiListSize = stSrhIDByStuName.lstStudentName.SetStudentListDataToBuf(pucBuf);
			ullPtr += uiListSize;
			pucBuf += uiListSize;
			// 存储结束标记
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
		// 队列头部取数据
		if (queue.DeleteFromHead())
		{
			stSrhIDByStu = *(stSearchIDByStudentName *)&pCurNode->m_data;
			// 这4个字节是Hash值占据的内容大小, 另外4字节是结束符标记
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

	// 计算存储总共需要的内存大小
	ullSize = CalcAllHeapMemoryAcquiredCourse();
	if (!ullSize)
	{
		rullSetSize = 0;
		return(nullptr);
	}
	// 分配空间
	pucBuf = new unsigned char[ullSize];
	if (!pucBuf)
	{
		rullSetSize = 0;
		return(nullptr);
	}
	pucTmpBuf = pucBuf;
	// 开始遍历填充内容
	pCurNode = m_pRootNode;
	//  Insert -> Tail   .....   Head -> Delete
	queue.InsertFromTail(pCurNode);

	while (!queue.IsEmpty())
	{
		queue.GetHeadValue(pCurNode);
		// 队列头部取数据
		if (queue.DeleteFromHead())
		{
			stSrhIDByCoName = *(stSearchIDByCourseName *)(&pCurNode->m_data);
			// 存储Hash值
			*((unsigned int *)pucBuf) = stSrhIDByCoName.nNameHash;
			pucBuf += 4;
			ullPtr += 4;
			// 存储链表信息
			uiListSize = stSrhIDByCoName.lstCourseName.SetCourseListDataToBuf(pucBuf);
			ullPtr += uiListSize;
			pucBuf += uiListSize;
			// 存储结束标记
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
		// 队列头部取数据
		if (queue.DeleteFromHead())
		{
			stSrhIDByCo = *(stSearchIDByCourseName *)&pCurNode->m_data;
			// 这4个字节是Hash值占据的内容大小, 另外4字节是结束符标记
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
		// 队列头部取数据
		if (queue.DeleteFromHead())
		{
			pStudentInfo = (pstStuInfo)&pCurNode->m_data;
			cout << "学生名: " << pStudentInfo->strName.GetString() << endl;
			cout << "学生ID: " << pStudentInfo->uiID << endl;
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
	// 通过HASH值查找对应课程信息
	fSuccess = g_stSrhCourseNameTree.FindNodeByID(nHash, stSrhCoName);
	// 通过课程名查询ID号
	if (fSuccess)
	{
		// 该hash下有多个对应的课程, 通过课程名进一步确认
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

	// 通过HASH值查找对应学生信息
	fSuccess = g_stSrhStudentNameTree.FindNodeByID(nHash, stSrhStuName);
	// 通过学生名查询ID号
	if (fSuccess)
	{
		CArray<size_t> cAry;
		// 该hash下有多个对应的学生, 通过学生进一步确认
		fSuccess = stSrhStuName.lstStudentName.FindStuId(pcszStudentName, cAry);
		if (fSuccess && cAry.GetSize() > 0)
		{
			// 可能会有同名的学生
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
	// 通过学生名找到AVL学生结点
	pNode = FindNodePtrByID(nStudentID);
	if (!pNode)
	{
		return(false);
	}
	pstStu = (pstStuInfo)&pNode->m_data;
	// 通过修了该课程的学生名删除AVL课程链表中的选课学生链表结点
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
	// 通过课程名找到AVL课程结点
	pNode = FindNodeByCourseName(pcszCourseName, nCourseNameHash, fOk);
	if (!pNode)
	{
		return(false);
	}
	pstCo = (pstCourse)&pNode->m_data;
	// 通过修了该课程的学生名删除AVL课程链表中的选课学生链表结点
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

	// 空树直接返回false
	if (!m_pRootNode)
	{
		return(false);
	}
	// 查找要删除的结点, 没有找到直接返回nullptr
	pDelNode = FindNodeByID(nID, fOk);
	if (!fOk)
	{
		return(false);
	}
	pCoInfo = (pstCourse)&pDelNode->m_data;
	pCoInfo->lstOfStu.GetStudentListNamesAry(prStuID, nBufSize);

	// 如果是根节点则直接删除后成功
	if (m_pRootNode == pDelNode)
	{
		delete m_pRootNode;
		m_pRootNode = nullptr;
		return(true);
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
bool CAVLTree<T>::DeleteNodeStu(const size_t nID, 
	CArray<char *> &cCourseNameAry, 
	bool fCourse/* = true*/)
{
	pTreeNode pDelNode = nullptr;
	pTreeNode pParentNodeOfDelNode = nullptr;
	pTreeNode pCurNode = nullptr;
	pstStuInfo pStuInfo = nullptr;
	bool fOk = false;

	// 空树直接返回false
	if (!m_pRootNode)
	{
		return(false);
	}
	// 查找要删除的结点, 没有找到直接返回nullptr
	pDelNode = FindNodeByID(nID, fOk);
	if (!fOk)
	{
		return(false);
	}
	pStuInfo = (pstStuInfo)&pDelNode->m_data;
	pStuInfo->lstOfCourse.GetCourseListNamesAry(cCourseNameAry);

	// 如果是根节点则直接删除后成功
	if (m_pRootNode == pDelNode)
	{
		delete m_pRootNode;
		m_pRootNode = nullptr;
		return(true);
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
			// 如果做左子节点存在右子树
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

	// 空树直接返回false
	if (!m_pRootNode)
	{
		return(false);
	}
	// 查找要删除的结点, 没有找到直接返回false
	pDelNode = FindNodeByID(nHash, fOk);
	if (!fOk)
	{
		return(false);
	}
	// Srh用
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
	// 如果是根节点则直接删除后成功
	if (m_pRootNode == pDelNode)
	{
		delete m_pRootNode;
		m_pRootNode = nullptr;
		return(true);
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
		// 队列头部取数据
		if (queue.DeleteFromHead())
		{
			pCourseInfo = (pstCourse)&pCurNode->m_data;
			cout << "课程名: " << pCourseInfo->strCourseName.GetString() << endl;
			cout << "课程ID: " << pCourseInfo->uiCourseID << endl;
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
bool CAVLTree<T>::InsertNode(T data, size_t nID)
{
	pTreeNode pCurNode = m_pRootNode;
	pTreeNode pParentNode = nullptr;
	pTreeNode pNewNode = nullptr;

	do
	{
		// 分配新结点
		pNewNode = new TreeNode(data, nID);
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
		// 判断插入的位置并插入新结点
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