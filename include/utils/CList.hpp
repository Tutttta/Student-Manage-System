// 双向循环链表实现类
#pragma once
#include <iostream>
#include "CMyString.h"
#include "CArray.hpp"

using namespace std;

template <typename T>
class CList
{
public:
	static const size_t INVALID_IDX = -1;
	// CList的结点结构
	struct Node
	{
		Node(T val) :m_val(val) {}
		Node(size_t nValid) : m_fInvalid(nValid) { }
		Node() { }
		bool  m_fInvalid = false;
		T	  m_val;            //数据
		Node* m_prev = nullptr; //前驱结点的地址
		Node* m_next = nullptr; //后继结点的地址
	};
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
	CList& InsertHead(T val);
	// 新结点插入尾部
	CList& InsertTail(T val);
	// 在指定索引插入值
	CList& Insert(T val, size_t nIdx); 
	// 清空链表并释放空间
	void   Clear();
	// 获取链表元素数量
	size_t Size() const;
	// 判断链表是否为空
	bool   IsEmpty() const;
	// 获取链表结点数量
	size_t GetSize() const;
	// 遍历打印双向链表
	void TraverseList() const;
	// 将课程结点中的学生信息链表的学生们保存在动态数组内
	void GetCourseListNamesAry(CArray<char *> &CourseNameAry);
	// 将学生结点中的课程链表的课程保存
	void GetStudentListNamesAry(size_t *&prStuID, size_t &nBufSize);

	/*
		g_stSrhCourseNameTree和g_stSrhStudentNameTree存储格式
		|  nHash  |_________ ____________
		|   nID   | nStrLen | string ... |
		|   nID   | nStrLen | string ... |
		|             ...                |_________
		|   nID   | nStrLen | string ... | ENDMARK |
	*/
	// 获取SrhCourse中链表内容总大小
	unsigned int GetCourseListSize() const;
	// 获取SrhStudent中链表内容总大小
	unsigned int GetStudentListSize() const;
	// 将链表数据按照存储格式保存到pucBuf中并返回写入的数据长度
	unsigned int SetCourseListDataToBuf(unsigned char *pucBuf);
	// 将链表数据按照存储格式保存到pucBuf中并返回写入的数据长度
	unsigned int SetStudentListDataToBuf(unsigned char *pucBuf);
	/*
		g_stCourseInfoTree和g_stStudentInfoTree存储格式
		 _________________________________________________
		| CourseID | StrLen |        ... CoStr ...        |
		|   StuID  |  Point | StuStrLen |... StuStrName...|
		|   StuID  |  Point | StuStrLen |... StuStrName...|
		|                        ...                      |
		|   StuID  |  Point | StuStrLen |... StuStrName...|_________
		|   StuID  |  Point | StuStrLen |... StuStrName...| ENDMARK |
	*/
	// 获取Course中链表内容总大小
	unsigned int GetCourseListSizeX() const;
	// 获取Student中链表内容总大小
	unsigned int GetStudentListSizeX() const;
	// 将链表数据按照存储格式保存到pucBuf中并返回写入的数据长度
	unsigned int SetCourseListDataToBufX(unsigned char *pucBuf);
	// 将链表数据按照存储格式保存到pucBuf中并返回写入的数据长度
	unsigned int SetStudentListDataToBufX(unsigned char *pucBuf);
	
	// 删除双向链表头部
	CList& DeleteHead();
	// 删除双向链表尾部
	CList& DeleteTail();
	// 删除指定索引指向的结点
	CList& Delete(size_t nIdx);  
	// 通过ID删除对应结点
	bool DeleteByID(size_t nID, bool fCourse = FALSE);
	// 根据课程名删除对应链表结点
	bool DeleteCourseRecordFromStudentTreeCourseList(CMyString &strCourseName);
	bool DeleteCourseRecordFromStudentTreeCourseList(const char *pcszCourseName);
	bool DeleteStudentRecordFromCourseTreeStudentList(CMyString &strStudentName);
	bool DeleteStudentRecordFromCourseTreeStudentList(const char *pcszStudentName);
	
	// 从stSearchIDByCourseName中的stCourseNameAndID链表内通过课程名搜索对应ID
	unsigned int FindCourseIDByCourseNameInList(const char *pcszCourseName, bool &fOk);
	// 确认学生是否选过该课程
	bool ChkStudentChoseCourse(char *pcszCoName, size_t nBufSize);
	// 通过课程名来查找其ID
	bool FindCoId(const char *pcszName, size_t &rnID);
	// 通过学生的姓名来查找其ID
	bool FindStuId(const char *pcszName, CArray<size_t> &cAry);

	// 获取学生结点中课程链表结点指针
	Node *FindStudentChoseCourseNode(char *pszCourseName, size_t nBufSize)
	{
		Node *pCurNode = nullptr;
		pStudentCourse pstStuCo = nullptr;

		if (!m_pHeadGuard || !m_pTailGuard)
		{
			return(nullptr);
		}
		pCurNode = m_pHeadGuard->m_next;
		while (pCurNode != m_pTailGuard)
		{
			pstStuCo = (pStudentCourse)&pCurNode->m_val;
			if (!_strnicmp(pstStuCo->strCourseName.GetString(), pszCourseName, nBufSize - 1))
			{
				return(pCurNode);
			}
			pCurNode = pCurNode->m_next;
		}

		return(nullptr);
	}
	// 获取课程结点中学生链表结点指针
	Node *FindCourseChosenStudentNode(char *pcszStuName, size_t nBufSize)
	{
		Node *pCurNode = nullptr;
		pstStuChosenCourse pstStuChosenCo = nullptr;

		if (!m_pHeadGuard || !m_pTailGuard)
		{
			return(nullptr);
		}
		pCurNode = m_pHeadGuard->m_next;
		while (pCurNode != m_pTailGuard)
		{
			pstStuChosenCo = (pstStuChosenCourse)&pCurNode->m_val;
			if (!_strnicmp(pstStuChosenCo->strName.GetString(), pcszStuName, nBufSize - 1))
			{
				return(pCurNode);
			}
			pCurNode = pCurNode->m_next;
		}

		return(nullptr);
	}
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
bool CList<T>::DeleteByID(size_t nID, bool fCourse/* = FALSE*/)
{
	Node *pCurNode = nullptr;
	Node *pPreNode = nullptr;
	Node *pNextNode = nullptr;

	if (!m_pHeadGuard || !m_pTailGuard)
	{
		return(false);
	}
	pCurNode = m_pHeadGuard->m_next;
	while (pCurNode != m_pTailGuard)
	{
		if (fCourse)
		{
			pstCourseNameAndID pstCoNameID = (pstCourseNameAndID)&pCurNode->m_val;
			if (pstCoNameID->uiCourseID == nID)
			{
				break;
			}
		}
		else
		{
			pstStudentNameAndID pstStuNameID = (pstStudentNameAndID)&pCurNode->m_val;
			if (pstStuNameID->uiStudentID == nID)
			{
				break;
			}
		}
		pCurNode = pCurNode->m_next;
	}

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

	return(true);
}

template <typename T>
unsigned int CList<T>::GetStudentListSizeX() const
{
	unsigned int uiListTotalSize = 0;
	Node *pCurNode = nullptr;
	StudentCourse stStuCo;

	if (!m_pHeadGuard || !m_pTailGuard)
	{
		return(0);
	}
	pCurNode = m_pHeadGuard->m_next;
	while (pCurNode != m_pTailGuard)
	{
		stStuCo = (StudentCourse)pCurNode->m_val;
		// 这4个字节是存储到文件时用到的字符串长度
		uiListTotalSize += sizeof(stStuCo.nCourseID) + sizeof(stStuCo.uiPoint)
			+ (stStuCo.strCourseName.GetBufLen() - 1) + 4;
		pCurNode = pCurNode->m_next;
	}

	return(uiListTotalSize);
}


template <typename T>
unsigned int CList<T>::SetStudentListDataToBufX(unsigned char *pucBuf)
{
	Node *pCurNode = nullptr;
	StudentCourse stStuCo;
	unsigned int uiSaveSize = 0;
	size_t nStrLen = 0;

	if (!m_pHeadGuard || !m_pTailGuard)
	{
		return(0);
	}
	pCurNode = m_pHeadGuard->m_next;
	while (pCurNode != m_pTailGuard)
	{
		stStuCo = (StudentCourse)pCurNode->m_val;
		// 这4个字节是存储到文件时用到的字符串长度
		// 保存课程ID
		*(size_t *)pucBuf = stStuCo.nCourseID;
		uiSaveSize += 4;
		pucBuf += 4;
		// 保存课程分数
		*(size_t *)pucBuf = stStuCo.uiPoint;
		uiSaveSize += 4;
		pucBuf += 4;
		// 保存修课程名的长度
		nStrLen = stStuCo.strCourseName.GetBufLen() - 1;
		*(size_t *)pucBuf = nStrLen;
		uiSaveSize += 4;
		pucBuf += 4;
		// 保存字符串
		memcpy(pucBuf, stStuCo.strCourseName.GetString(), nStrLen);
		uiSaveSize += nStrLen;
		pucBuf += nStrLen;
		pCurNode = pCurNode->m_next;
	}

	return(uiSaveSize);
}


template <typename T>
unsigned int CList<T>::GetCourseListSizeX() const
{
	unsigned int uiListTotalSize = 0;
	Node *pCurNode = nullptr;
	StuChosenCourse stChosenCo;

	if (!m_pHeadGuard || !m_pTailGuard)
	{
		return(0);
	}
	pCurNode = m_pHeadGuard->m_next;
	while (pCurNode != m_pTailGuard)
	{
		stChosenCo = (StuChosenCourse)pCurNode->m_val;
		// 这4个字节是存储到文件时用到的字符串长度
		uiListTotalSize += sizeof(stChosenCo.nStuID) + sizeof(stChosenCo.uiPoint)
			+ (stChosenCo.strName.GetBufLen() - 1) + 4;
		pCurNode = pCurNode->m_next;
	}

	return(uiListTotalSize);
}


template <typename T>
unsigned int CList<T>::SetCourseListDataToBufX(unsigned char *pucBuf)
{
	Node *pCurNode = nullptr;
	StuChosenCourse stChosenCo;
	unsigned int uiSaveSize = 0;
	size_t nStrLen = 0;

	if (!m_pHeadGuard || !m_pTailGuard)
	{
		return(0);
	}
	pCurNode = m_pHeadGuard->m_next;
	while (pCurNode != m_pTailGuard)
	{
		stChosenCo = (StuChosenCourse)pCurNode->m_val;
		// 这4个字节是存储到文件时用到的字符串长度
		// 保存学生ID
		*(size_t *)pucBuf = stChosenCo.nStuID;
		uiSaveSize += 4;
		pucBuf += 4;
		// 保存学生的课程分数
		*(size_t *)pucBuf = stChosenCo.uiPoint;
		uiSaveSize += 4;
		pucBuf += 4;
		// 保存学生选修课程名的长度
		nStrLen = stChosenCo.strName.GetBufLen() - 1;
		*(size_t *)pucBuf = nStrLen;
		uiSaveSize += 4;
		pucBuf += 4;
		// 保存字符串
		memcpy(pucBuf, stChosenCo.strName.GetString(), nStrLen);
		uiSaveSize += nStrLen;
		pucBuf += nStrLen;
		pCurNode = pCurNode->m_next;
	}

	return(uiSaveSize);
}


template <typename T>
unsigned int CList<T>::SetStudentListDataToBuf(unsigned char *pucBuf)
{
	Node *pCurNode = nullptr;
	stStudentNameAndID stStuNameID;
	unsigned int uiSaveSize = 0;
	size_t nStrLen = 0;

	if (!m_pHeadGuard || !m_pTailGuard)
	{
		return(0);
	}
	pCurNode = m_pHeadGuard->m_next;
	while (pCurNode != m_pTailGuard)
	{
		stStuNameID = (stStudentNameAndID)pCurNode->m_val;
		// 这4个字节是存储到文件时用到的字符串长度
		// 保存ID
		*(size_t *)pucBuf = stStuNameID.uiStudentID;
		uiSaveSize += 4;
		pucBuf += 4;
		// 保存字符串长度
		nStrLen = stStuNameID.strStudentName.GetBufLen() - 1;
		*(size_t *)pucBuf = nStrLen;
		uiSaveSize += 4;
		pucBuf += 4;
		// 保存字符串
		memcpy(pucBuf, stStuNameID.strStudentName.GetString(), nStrLen);
		uiSaveSize += nStrLen;
		pucBuf += nStrLen;
		pCurNode = pCurNode->m_next;
	}

	return(uiSaveSize);
}


template <typename T>
unsigned int CList<T>::GetStudentListSize() const
{
	Node *pCurNode = nullptr;
	stStudentNameAndID stStuNameID;
	unsigned int uiListNodeSize = 0;

	if (!m_pHeadGuard || !m_pTailGuard)
	{
		return(0);
	}
	pCurNode = m_pHeadGuard->m_next;
	while (pCurNode != m_pTailGuard)
	{
		stStuNameID = (stStudentNameAndID)pCurNode->m_val;
		// 这4个字节是存储到文件时用到的字符串长度
		uiListNodeSize += sizeof(stStuNameID.uiStudentID) +
			(stStuNameID.strStudentName.GetBufLen() - 1) + 4;
		pCurNode = pCurNode->m_next;
	}

	return(uiListNodeSize);
}


template <typename T>
unsigned int CList<T>::SetCourseListDataToBuf(unsigned char *pucBuf)
{
	Node *pCurNode = nullptr;
	stCourseNameAndID stCoNameID;
	unsigned int uiSaveSize = 0;
	size_t nStrLen = 0;

	if (!m_pHeadGuard || !m_pTailGuard)
	{
		return(0);
	}
	pCurNode = m_pHeadGuard->m_next;
	while (pCurNode != m_pTailGuard)
	{
		stCoNameID = (stCourseNameAndID)pCurNode->m_val;
		// 这4个字节是存储到文件时用到的字符串长度
		// 保存ID
		*(size_t *)pucBuf = stCoNameID.uiCourseID;
		uiSaveSize += 4;
		pucBuf += 4;
		// 保存字符串长度
		nStrLen = stCoNameID.strCourseName.GetBufLen() - 1;
		*(size_t *)pucBuf = nStrLen;
		uiSaveSize += 4;
		pucBuf += 4;
		// 保存字符串
		memcpy(pucBuf, stCoNameID.strCourseName.GetString(), nStrLen);
		uiSaveSize += nStrLen;
		pucBuf += nStrLen;
		pCurNode = pCurNode->m_next;
	}

	return(uiSaveSize);
}

template <typename T>
unsigned int CList<T>::GetCourseListSize() const
{
	Node *pCurNode = nullptr;
	stCourseNameAndID stCoNameID;
	unsigned int uiListNodeSize = 0;

	if (!m_pHeadGuard || !m_pTailGuard)
	{
		return(0);
	}
	pCurNode = m_pHeadGuard->m_next;
	while (pCurNode != m_pTailGuard)
	{
		stCoNameID = (stCourseNameAndID)pCurNode->m_val;
		// 这4个字节是存储到文件时用到的字符串长度
		uiListNodeSize += sizeof(stCoNameID.uiCourseID) + 
			(stCoNameID.strCourseName.GetBufLen() - 1) + 4;
		pCurNode = pCurNode->m_next;
	}

	return(uiListNodeSize);
}

template <typename T>
bool CList<T>::FindCoId(const char *pcszName, size_t &rnID)
{
	Node *pCurNode = nullptr;
	pstCourseNameAndID pstCoNameAndId = nullptr;

	if (!m_pHeadGuard || !m_pTailGuard)
	{
		return(false);
	}
	pCurNode = m_pHeadGuard->m_next;
	while (pCurNode != m_pTailGuard)
	{
		pstCoNameAndId = (pstCourseNameAndID)&pCurNode->m_val;
		if (!strcmp(pcszName, pstCoNameAndId->strCourseName.GetString()))
		{
			rnID = pstCoNameAndId->uiCourseID;
			return(true);
		}
		pCurNode = pCurNode->m_next;
	}
	cout << endl;

	return(false);
}


template <typename T>
bool CList<T>::ChkStudentChoseCourse(char *pcszCoName, size_t nBufSize)
{
	Node *pCurNode = nullptr;
	pStudentCourse pstStuCo = nullptr;

	if (!m_pHeadGuard || !m_pTailGuard)
	{
		return(false);
	}
	pCurNode = m_pHeadGuard->m_next;
	while (pCurNode != m_pTailGuard)
	{
		pstStuCo = (pStudentCourse)&pCurNode->m_val;
		if (!_strnicmp(pstStuCo->strCourseName.GetString(), pcszCoName, nBufSize - 1))
		{
			return(true);
		}
		pCurNode = pCurNode->m_next;
	}

	return(false);
}


template <typename T>
unsigned int CList<T>::FindCourseIDByCourseNameInList(const char *pcszCourseName, bool &fOk)
{
	pstCourseNameAndID pstCoNameID = nullptr;
	Node *pCurNode = nullptr;
	fOk = false;

	if (!m_pHeadGuard || !m_pTailGuard || !pcszCourseName)
	{
		return(0);
	}
	pCurNode = m_pHeadGuard->m_next;
	while (pCurNode != m_pTailGuard)
	{
		pstCoNameID = (pstCourseNameAndID)&pCurNode->m_val;
		if (!_stricmp(pstCoNameID->strCourseName.GetString(), pcszCourseName))
		{
			fOk = true;
			return(pstCoNameID->uiCourseID);
		}
		pCurNode = pCurNode->m_next;
	}

	return(0);
}


template <typename T>
bool CList<T>::FindStuId(const char *pcszName, CArray<size_t> &cAry)
{
	Node *pCurNode = nullptr;
	pstStudentNameAndID pstStuNameAndId = nullptr;

	if (!m_pHeadGuard || !m_pTailGuard)
	{
		return(false);
	}
	pCurNode = m_pHeadGuard->m_next;
	while (pCurNode != m_pTailGuard)
	{
		pstStuNameAndId = (pstStudentNameAndID)&pCurNode->m_val;
		if (!strcmp(pcszName, pstStuNameAndId->strStudentName.GetString()))
		{
			cAry.InsertHead(pstStuNameAndId->uiStudentID);
		}
		pCurNode = pCurNode->m_next;
	}
	// 如果数组内有内容则代表查找成功
	if (cAry.GetSize())
	{
		return(true);
	}

	return(false);
}


template <typename T>
bool CList<T>::DeleteCourseRecordFromStudentTreeCourseList(const char *pcszCourseName)
{
	Node *pCurNode = nullptr;
	Node *pPreNode = nullptr;
	Node *pNextNode = nullptr;
	bool fOk = false;
	
	// 通过课程名, 找到链表结点
	pCurNode = FindStudentChoseCourseNode((char *)pcszCourseName,
		strlen(pcszCourseName));
	if (!pCurNode)
	{
		return(false);
	}
	// 找到了该结点执行删除操作
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

	return(true);
}

template <typename T>
bool CList<T>::DeleteStudentRecordFromCourseTreeStudentList(const char *pcszStudentName)
{
	Node *pCurNode = nullptr;
	Node *pPreNode = nullptr;
	Node *pNextNode = nullptr;
	stCourse stCo;
	bool fOk = false;

	// 通过学生名, 找到链表结点
	pCurNode = FindCourseChosenStudentNode((char *)pcszStudentName,
		strlen(pcszStudentName));
	if (!pCurNode)
	{
		return(false);
	}
	// 找到了该结点执行删除操作
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

	return(true);
}

template <typename T>
bool CList<T>::DeleteStudentRecordFromCourseTreeStudentList(CMyString &strStudentName)
{
	Node *pCurNode = nullptr;
	Node *pPreNode = nullptr;
	Node *pNextNode = nullptr;
	stCourse stCo;
	bool fOk = false;

	// 通过学生名, 找到链表结点
	pCurNode = FindCourseChosenStudentNode(strStudentName.GetString(), 
		strStudentName.GetBufLen());
	if (!pCurNode)
	{
		return(false);
	}
	// 找到了该结点执行删除操作
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

	return(true);
}

template <typename T>
bool CList<T>::DeleteCourseRecordFromStudentTreeCourseList(CMyString &strCourseName)
{
	Node *pCurNode = nullptr;
	Node *pPreNode = nullptr;
	Node *pNextNode = nullptr;
	bool fOk = false;

	// 找到课程名对应的链表结点
	pCurNode = FindStudentChoseCourseNode(strCourseName.GetString(),
		strCourseName.GetBufLen());
	if (!pCurNode)
	{
		return(false);
	}
	// 找到了该结点执行删除操作
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

	return(true);
}

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
CList<T>& CList<T>::InsertHead(T val)
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
CList<T>& CList<T>::InsertTail(T val)
{
	return(Insert(val, m_nSize));
}

template <typename T>
CList<T>& CList<T>::Insert(T val, size_t nIdx)
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
void  CList<T>::GetStudentListNamesAry(size_t *&prStuID, size_t &nBufSize)
{
	Node *pCurNode = nullptr;
	pstStuChosenCourse pstStuChosenCo = nullptr;

	if (!m_pHeadGuard || !m_pTailGuard)
	{
		return;
	}
	prStuID = new size_t[m_nSize];
	if (!prStuID)
	{
		nBufSize = 0;
		return;
	}
	pCurNode = m_pHeadGuard->m_next;
	while (pCurNode != m_pTailGuard)
	{
		pstStuChosenCo = &pCurNode->m_val;
		prStuID[nBufSize++] = pstStuChosenCo->nStuID;
		pCurNode = pCurNode->m_next;
	}
}


template <typename T>
void CList<T>::GetCourseListNamesAry(CArray<char *> &CourseNameAry)
{
	Node *pCurNode = nullptr;
	pStudentCourse pStuCo = nullptr;

	if (!m_pHeadGuard || !m_pTailGuard)
	{
		return;
	}
	pCurNode = m_pHeadGuard->m_next;
	while (pCurNode != m_pTailGuard)
	{
		pStuCo = (pStudentCourse)&pCurNode->m_val;
		CourseNameAry.InsertHead(pStuCo->strCourseName.GetString());
		pCurNode = pCurNode->m_next;
	}
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
		cout << pCurNode->m_val << endl;
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
size_t CList<T>::GetSize() const
{ 
	return(m_nSize); 
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

