// ˫��ѭ������ʵ����
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
	// CList�Ľ��ṹ
	struct Node
	{
		Node(T val) :m_val(val) {}
		Node(size_t nValid) : m_fInvalid(nValid) { }
		Node() { }
		bool  m_fInvalid = false;
		T	  m_val;            //����
		Node* m_prev = nullptr; //ǰ�����ĵ�ַ
		Node* m_next = nullptr; //��̽��ĵ�ַ
	};
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
	CList& InsertHead(T val);
	// �½�����β��
	CList& InsertTail(T val);
	// ��ָ����������ֵ
	CList& Insert(T val, size_t nIdx); 
	// ��������ͷſռ�
	void   Clear();
	// ��ȡ����Ԫ������
	size_t Size() const;
	// �ж������Ƿ�Ϊ��
	bool   IsEmpty() const;
	// ��ȡ����������
	size_t GetSize() const;
	// ������ӡ˫������
	void TraverseList() const;
	// ���γ̽���е�ѧ����Ϣ�����ѧ���Ǳ����ڶ�̬������
	void GetCourseListNamesAry(CArray<char *> &CourseNameAry);
	// ��ѧ������еĿγ�����Ŀγ̱���
	void GetStudentListNamesAry(size_t *&prStuID, size_t &nBufSize);

	/*
		g_stSrhCourseNameTree��g_stSrhStudentNameTree�洢��ʽ
		|  nHash  |_________ ____________
		|   nID   | nStrLen | string ... |
		|   nID   | nStrLen | string ... |
		|             ...                |_________
		|   nID   | nStrLen | string ... | ENDMARK |
	*/
	// ��ȡSrhCourse�����������ܴ�С
	unsigned int GetCourseListSize() const;
	// ��ȡSrhStudent�����������ܴ�С
	unsigned int GetStudentListSize() const;
	// ���������ݰ��մ洢��ʽ���浽pucBuf�в�����д������ݳ���
	unsigned int SetCourseListDataToBuf(unsigned char *pucBuf);
	// ���������ݰ��մ洢��ʽ���浽pucBuf�в�����д������ݳ���
	unsigned int SetStudentListDataToBuf(unsigned char *pucBuf);
	/*
		g_stCourseInfoTree��g_stStudentInfoTree�洢��ʽ
		 _________________________________________________
		| CourseID | StrLen |        ... CoStr ...        |
		|   StuID  |  Point | StuStrLen |... StuStrName...|
		|   StuID  |  Point | StuStrLen |... StuStrName...|
		|                        ...                      |
		|   StuID  |  Point | StuStrLen |... StuStrName...|_________
		|   StuID  |  Point | StuStrLen |... StuStrName...| ENDMARK |
	*/
	// ��ȡCourse�����������ܴ�С
	unsigned int GetCourseListSizeX() const;
	// ��ȡStudent�����������ܴ�С
	unsigned int GetStudentListSizeX() const;
	// ���������ݰ��մ洢��ʽ���浽pucBuf�в�����д������ݳ���
	unsigned int SetCourseListDataToBufX(unsigned char *pucBuf);
	// ���������ݰ��մ洢��ʽ���浽pucBuf�в�����д������ݳ���
	unsigned int SetStudentListDataToBufX(unsigned char *pucBuf);
	
	// ɾ��˫������ͷ��
	CList& DeleteHead();
	// ɾ��˫������β��
	CList& DeleteTail();
	// ɾ��ָ������ָ��Ľ��
	CList& Delete(size_t nIdx);  
	// ͨ��IDɾ����Ӧ���
	bool DeleteByID(size_t nID, bool fCourse = FALSE);
	// ���ݿγ���ɾ����Ӧ������
	bool DeleteCourseRecordFromStudentTreeCourseList(CMyString &strCourseName);
	bool DeleteCourseRecordFromStudentTreeCourseList(const char *pcszCourseName);
	bool DeleteStudentRecordFromCourseTreeStudentList(CMyString &strStudentName);
	bool DeleteStudentRecordFromCourseTreeStudentList(const char *pcszStudentName);
	
	// ��stSearchIDByCourseName�е�stCourseNameAndID������ͨ���γ���������ӦID
	unsigned int FindCourseIDByCourseNameInList(const char *pcszCourseName, bool &fOk);
	// ȷ��ѧ���Ƿ�ѡ���ÿγ�
	bool ChkStudentChoseCourse(char *pcszCoName, size_t nBufSize);
	// ͨ���γ�����������ID
	bool FindCoId(const char *pcszName, size_t &rnID);
	// ͨ��ѧ����������������ID
	bool FindStuId(const char *pcszName, CArray<size_t> &cAry);

	// ��ȡѧ������пγ�������ָ��
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
	// ��ȡ�γ̽����ѧ��������ָ��
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
		// ��4���ֽ��Ǵ洢���ļ�ʱ�õ����ַ�������
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
		// ��4���ֽ��Ǵ洢���ļ�ʱ�õ����ַ�������
		// ����γ�ID
		*(size_t *)pucBuf = stStuCo.nCourseID;
		uiSaveSize += 4;
		pucBuf += 4;
		// ����γ̷���
		*(size_t *)pucBuf = stStuCo.uiPoint;
		uiSaveSize += 4;
		pucBuf += 4;
		// �����޿γ����ĳ���
		nStrLen = stStuCo.strCourseName.GetBufLen() - 1;
		*(size_t *)pucBuf = nStrLen;
		uiSaveSize += 4;
		pucBuf += 4;
		// �����ַ���
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
		// ��4���ֽ��Ǵ洢���ļ�ʱ�õ����ַ�������
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
		// ��4���ֽ��Ǵ洢���ļ�ʱ�õ����ַ�������
		// ����ѧ��ID
		*(size_t *)pucBuf = stChosenCo.nStuID;
		uiSaveSize += 4;
		pucBuf += 4;
		// ����ѧ���Ŀγ̷���
		*(size_t *)pucBuf = stChosenCo.uiPoint;
		uiSaveSize += 4;
		pucBuf += 4;
		// ����ѧ��ѡ�޿γ����ĳ���
		nStrLen = stChosenCo.strName.GetBufLen() - 1;
		*(size_t *)pucBuf = nStrLen;
		uiSaveSize += 4;
		pucBuf += 4;
		// �����ַ���
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
		// ��4���ֽ��Ǵ洢���ļ�ʱ�õ����ַ�������
		// ����ID
		*(size_t *)pucBuf = stStuNameID.uiStudentID;
		uiSaveSize += 4;
		pucBuf += 4;
		// �����ַ�������
		nStrLen = stStuNameID.strStudentName.GetBufLen() - 1;
		*(size_t *)pucBuf = nStrLen;
		uiSaveSize += 4;
		pucBuf += 4;
		// �����ַ���
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
		// ��4���ֽ��Ǵ洢���ļ�ʱ�õ����ַ�������
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
		// ��4���ֽ��Ǵ洢���ļ�ʱ�õ����ַ�������
		// ����ID
		*(size_t *)pucBuf = stCoNameID.uiCourseID;
		uiSaveSize += 4;
		pucBuf += 4;
		// �����ַ�������
		nStrLen = stCoNameID.strCourseName.GetBufLen() - 1;
		*(size_t *)pucBuf = nStrLen;
		uiSaveSize += 4;
		pucBuf += 4;
		// �����ַ���
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
		// ��4���ֽ��Ǵ洢���ļ�ʱ�õ����ַ�������
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
	// ����������������������ҳɹ�
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
	
	// ͨ���γ���, �ҵ�������
	pCurNode = FindStudentChoseCourseNode((char *)pcszCourseName,
		strlen(pcszCourseName));
	if (!pCurNode)
	{
		return(false);
	}
	// �ҵ��˸ý��ִ��ɾ������
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

	// ͨ��ѧ����, �ҵ�������
	pCurNode = FindCourseChosenStudentNode((char *)pcszStudentName,
		strlen(pcszStudentName));
	if (!pCurNode)
	{
		return(false);
	}
	// �ҵ��˸ý��ִ��ɾ������
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

	// ͨ��ѧ����, �ҵ�������
	pCurNode = FindCourseChosenStudentNode(strStudentName.GetString(), 
		strStudentName.GetBufLen());
	if (!pCurNode)
	{
		return(false);
	}
	// �ҵ��˸ý��ִ��ɾ������
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

	// �ҵ��γ�����Ӧ��������
	pCurNode = FindStudentChoseCourseNode(strCourseName.GetString(),
		strCourseName.GetBufLen());
	if (!pCurNode)
	{
		return(false);
	}
	// �ҵ��˸ý��ִ��ɾ������
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
CList<T>& CList<T>::InsertHead(T val)
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
size_t CList<T>::GetSize() const
{ 
	return(m_nSize); 
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

