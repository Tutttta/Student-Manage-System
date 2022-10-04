#pragma once
#include "Global.h"
#include "CAVLTree.hpp"

class IO
{
public:
	// IO类型
	typedef enum _eIOType
	{
		READONLY = 1,
		CREATE
	} eIOType;
public:
	IO() = default;
	IO(const char *pcszFilePathName, eIOType eMode = READONLY);
	IO(const CMyString &strFileName, eIOType eMode = READONLY);
	// 设置IO模式
	inline void SetMode(eIOType eIOMode)
	{
		m_eMode = eIOMode;
	}
	// 获取文件大小
	ULONGLONG IOGetFileSize();
	// 从文件中按行读取内容
	char **GetLineFromFile(size_t &rnSizeOfStringAry);
	// 更新文件名
	void UpdateFileName(const char *pcszFilePathName)
	{
		if (!pcszFilePathName)
		{
			return;
		}
		m_strFileName = pcszFilePathName;
	}
	// 更新文件名
	void UpdateFileName(CMyString &strFileName)
	{
		UpdateFileName(strFileName.GetString());
	}
	// 判定文件是否存在
	BOOLEAN FileExists(const char *pcszFileName);


	// 存储搜索AVL树用的函数
	template <typename T>
	BOOLEAN RestoreSrhAVLTreeInfo(CAVLTree<T> &crAVLTree, BOOLEAN fSrhCourseInfo = TRUE);
	// 读取搜索AVL树用的函数
	template <typename T>
	BOOLEAN LoadSrhAVLTreeInfo(CAVLTree<T> &crAVLTree, BOOLEAN fSrhCourseInfo = TRUE);

	// 存储AVL树用的函数
	template <typename T>
	BOOLEAN RestoreAVLTreeInfo(CAVLTree<T> &crAVLTree, BOOLEAN fCourseInfo = TRUE);
	// 存储AVL树用的函数
	template <typename T>
	BOOLEAN LoadAVLTreeInfo(CAVLTree<T> &crAVLTree, BOOLEAN fCourseInfo = TRUE);
private:
	// 释放指针数组的空间
	void Release2DiAry(PPSTR ppStrAry, size_t nSize);
private:
	CMyString m_strFileName;
	eIOType m_eMode;
};


template <typename T>
BOOLEAN 
IO::RestoreSrhAVLTreeInfo(CAVLTree<T> &crAVLTree, 
	BOOLEAN fSrhCourseInfo /*= TRUE*/)
{
	unsigned long long ullHeapDataSize = 0;
	unsigned char *pucBuf = nullptr;
	FILE *pFile = nullptr;
	PCSTR pcszFileName = nullptr;
	BOOLEAN fOk = FALSE;
	size_t nWritten = 0;
	do
	{
		if (fSrhCourseInfo)
		{
			// 读取g_stSrhCourseNameTree的内容并保存到内存中
			pucBuf = crAVLTree.AllocHeapMemoryAndSetSrhCourseData(ullHeapDataSize);
			if (!pucBuf)
			{
				break;
			}
			pcszFileName = SRHCOURSEFILENAME;
		}
		else
		{
			// 读取g_stSrhStudentNameTree的内容保存到内存中
			pucBuf = crAVLTree.AllocHeapMemoryAndSetSrhStudentData(ullHeapDataSize);
			if (!pucBuf)
			{
				break;
			}
			pcszFileName = SRHSTUDENTFILENAME;
		}
		pFile = fopen(pcszFileName, "wb");
		if (!pFile)
		{
			break;
		}
		nWritten = fwrite(pucBuf, ullHeapDataSize, 1, pFile);
		if (nWritten != 1)
		{
			break;
		}
		fflush(pFile);
		fOk = TRUE;
	} while (FALSE);

	if (pFile)
	{
		fclose(pFile);
		pFile = nullptr;
	}
	if (pucBuf)
	{
		delete[] pucBuf;
		pucBuf = nullptr;
	}

	return(fOk);
}


template <typename T>
BOOLEAN IO::LoadSrhAVLTreeInfo(CAVLTree<T> &crAVLTree, 
	BOOLEAN fSrhCourseInfo /*= TRUE*/)
{
	unsigned long long ullPtr = 0;
	unsigned long long ullFileSize = 0;
	FILE *pFile = nullptr;
	PCSTR pcszFileName = SRHCOURSEFILENAME;
	BOOLEAN fOk = FALSE;
	unsigned char *pucBuf = nullptr;
	char szBuf[LINEMAX] = { 0 };
	size_t nLen = 0;
	char szEndStrMark[5] = { 0 };

	// 确认是课程信息还是学生信息
	if (!fSrhCourseInfo)
	{
		pcszFileName = SRHSTUDENTFILENAME;
	}

	do
	{
		// 打开srh*.dat数据文件
		pFile = fopen(pcszFileName, "rb");
		if (!pFile)
		{
			break;
		}
		// 更新文件
		UpdateFileName(pcszFileName);
		// 获得文件大小
		ullFileSize = IOGetFileSize();
		if (!ullFileSize)
		{
			break;
		}
		// 分配堆空间
		pucBuf = new unsigned char[ullFileSize];
		if (!pucBuf)
		{
			break;
		}
		// 从srh*.dat中获取数据
		nLen = fread(pucBuf, ullFileSize, 1, pFile);
		if (nLen != 1)
		{
			break;
		}
		
		// 确定是读取课程信息还是学生信息
		if (fSrhCourseInfo)
		{
			// 释放二叉树
			g_stSrhCourseNameTree.ReleaseTree();
			
			while (ullPtr < ullFileSize)
			{
				stSearchIDByCourseName stSrhIDCourseName;
				
				// 获取hash值
				stSrhIDCourseName.nNameHash = *(size_t *)pucBuf;
				pucBuf += 4;
				ullPtr += 4;
				while (TRUE)
				{
					stCourseNameAndID stCoNameID;
					// 获取课程ID值
					stCoNameID.uiCourseID = *(size_t *)pucBuf;
					memcpy(szEndStrMark, pucBuf, 4);
					pucBuf += 4;
					ullPtr += 4;
					// 链表内容拿取结束后必定有一个ENDMARK标记
					if (!strcmp(szEndStrMark, ENDMARK))
					{
						// 拿到hash后就遇到了结束标志说明链表中没有内容
						break;
					}
					// 获取字符串长度
					nLen = *(size_t *)pucBuf;
					pucBuf += 4;
					ullPtr += 4;
					// 获取字符串内容
					memset(szBuf, 0, LINEMAX);
					memcpy(szBuf, pucBuf, nLen);
					stCoNameID.strCourseName = szBuf;
					pucBuf += nLen;
					ullPtr += nLen;
					// 将课程信息插入到链表尾部
					stSrhIDCourseName.lstCourseName.InsertTail(stCoNameID);
				}
				// 将结点插入g_stSrhCourseNameTree二叉树中
				if (!g_stSrhCourseNameTree.InsertNode(stSrhIDCourseName, stSrhIDCourseName.nNameHash))
				{
					g_stSrhCourseNameTree.ReleaseTree();
					return(FALSE);
				}
			}
		}
		else
		{
			// 释放二叉树
			g_stSrhStudentNameTree.ReleaseTree();
			while (ullPtr < ullFileSize)
			{
				stSearchIDByStudentName stSrhIDStudentName;

				// 获取hash值
				stSrhIDStudentName.nNameHash = *(size_t *)pucBuf;
				pucBuf += 4;
				ullPtr += 4;
				while (TRUE)
				{
					stStudentNameAndID stStuNameID;
					// 获取学生ID值
					stStuNameID.uiStudentID = *(size_t *)pucBuf;
					memcpy(szEndStrMark, pucBuf, 4);
					pucBuf += 4;
					ullPtr += 4;
					// 链表内容拿取结束后必定有一个ENDMARK标记
					if (!strcmp(szEndStrMark, ENDMARK))
					{
						// 拿到hash后就遇到了结束标志说明链表中没有内容
						break;
					}
					// 获取字符串长度
					nLen = *(size_t *)pucBuf;
					pucBuf += 4;
					ullPtr += 4;
					// 获取字符串内容
					memset(szBuf, 0, LINEMAX);
					memcpy(szBuf, pucBuf, nLen);
					stStuNameID.strStudentName = szBuf;
					pucBuf += nLen;
					ullPtr += nLen;
					// 将课程信息插入到链表尾部
					stSrhIDStudentName.lstStudentName.InsertTail(stStuNameID);
				}
				// 将结点插入g_stSrhCourseNameTree二叉树中
				if (!g_stSrhStudentNameTree.InsertNode(stSrhIDStudentName, stSrhIDStudentName.nNameHash))
				{
					g_stSrhStudentNameTree.ReleaseTree();
					return(FALSE);
				}
			}
		}
		fOk = TRUE;
	} while (FALSE);

	if (pFile)
	{
		fclose(pFile);
		pFile = nullptr;
	}

	return(fOk);
}


template <typename T>
BOOLEAN IO::RestoreAVLTreeInfo(CAVLTree<T> &crAVLTree, BOOLEAN fCourseInfo /*= TRUE*/)
{
	unsigned long long ullHeapDataSize = 0;
	unsigned char *pucBuf = nullptr;
	FILE *pFile = nullptr;
	PCSTR pcszFileName = nullptr;
	BOOLEAN fOk = FALSE;
	size_t nWritten = 0;
	do
	{
		if (fCourseInfo)
		{
			// 读取g_stCourseInfoTree的内容并保存到内存中
			pucBuf = crAVLTree.AllocHeapMemoryAndSetCourseDataX(ullHeapDataSize);
			if (!pucBuf)
			{
				break;
			}
			pcszFileName = SAVECOURSEFILENAME;
		}
		else
		{
			// 读取g_stStudentInfoTree的内容保存到内存中
			pucBuf = crAVLTree.AllocHeapMemoryAndSetStudentDataX(ullHeapDataSize);
			if (!pucBuf)
			{
				break;
			}
			pcszFileName = SAVESTUDENTFILENAME;
		}
		pFile = fopen(pcszFileName, "wb");
		if (!pFile)
		{
			break;
		}
		nWritten = fwrite(pucBuf, ullHeapDataSize, 1, pFile);
		if (nWritten != 1)
		{
			break;
		}
		fflush(pFile);
		fOk = TRUE;
	} while (FALSE);

	if (pFile)
	{
		fclose(pFile);
		pFile = nullptr;
	}
	if (pucBuf)
	{
		delete[] pucBuf;
		pucBuf = nullptr;
	}

	return(fOk);
}


template <typename T>
BOOLEAN IO::LoadAVLTreeInfo(CAVLTree<T> &crAVLTree, BOOLEAN fCourseInfo /*= TRUE*/)
{
	unsigned long long ullPtr = 0;
	unsigned long long ullFileSize = 0;
	FILE *pFile = nullptr;
	PCSTR pcszFileName = SAVECOURSEFILENAME;
	BOOLEAN fOk = FALSE;
	unsigned char *pucBuf = nullptr;
	char szBuf[LINEMAX] = { 0 };
	size_t nLen = 0;
	char szEndStrMark[5] = { 0 };

	// 确认是课程信息还是学生信息
	if (!fCourseInfo)
	{
		pcszFileName = SAVESTUDENTFILENAME;
	}

	do
	{
		// 打开*.dat数据文件
		pFile = fopen(pcszFileName, "rb");
		if (!pFile)
		{
			break;
		}
		// 更新文件
		UpdateFileName(pcszFileName);
		// 获得文件大小
		ullFileSize = IOGetFileSize();
		if (!ullFileSize)
		{
			break;
		}
		// 分配堆空间
		pucBuf = new unsigned char[ullFileSize];
		if (!pucBuf)
		{
			break;
		}
		// 从*.dat中获取数据
		nLen = fread(pucBuf, ullFileSize, 1, pFile);
		if (nLen != 1)
		{
			break;
		}

		// 确定是读取课程信息还是学生信息
		if (fCourseInfo)
		{
			// 释放二叉树
			g_stCourseInfoTree.ReleaseTree();

			while (ullPtr < ullFileSize)
			{
				stCourse stCo;

				// 获取课程ID值
				stCo.uiCourseID = *(size_t *)pucBuf;
				pucBuf += 4;
				ullPtr += 4;
				// 获取课程名长度
				nLen = *(size_t *)pucBuf;
				pucBuf += 4;
				ullPtr += 4;
				// 获取课程名
				memset(szBuf, 0, sizeof(szBuf));
				memcpy(szBuf, pucBuf, nLen);
				stCo.strCourseName = szBuf;
				pucBuf += nLen;
				ullPtr += nLen;

				while (TRUE)
				{
					StuChosenCourse stChosenCo;
					// 获取学生ID值或者是结束符号
					stChosenCo.nStuID = *(size_t *)pucBuf;
					memcpy(szEndStrMark, pucBuf, 4);
					pucBuf += 4;
					ullPtr += 4;
					// 链表内容拿取结束后必定有一个ENDMARK标记
					if (!strcmp(szEndStrMark, ENDMARK))
					{
						// 遇到了结束标志说明链表中没有内容
						break;
					}
					// 获取学生选修的该课程的分数
					stChosenCo.uiPoint = *(size_t *)pucBuf;
					pucBuf += 4;
					ullPtr += 4;
					// 获取字符串长度
					nLen = *(size_t *)pucBuf;
					pucBuf += 4;
					ullPtr += 4;
					// 获取字符串内容
					memset(szBuf, 0, sizeof(szBuf));
					memcpy(szBuf, pucBuf, nLen);
					stChosenCo.strName = szBuf;
					pucBuf += nLen;
					ullPtr += nLen;
					// 将学生信息插入到链表尾部
					stCo.lstOfStu.InsertTail(stChosenCo);
				}
				// 将结点插入g_stCourseInfoTree二叉树中
				if (!g_stCourseInfoTree.InsertNode(stCo, stCo.uiCourseID))
				{
					g_stCourseInfoTree.ReleaseTree();
					return(FALSE);
				}
			}
		}
		else
		{
			// 释放二叉树
			g_stStudentInfoTree.ReleaseTree();

			while (ullPtr < ullFileSize)
			{
				stStuInfo stStu;

				// 获取学生ID值
				stStu.uiID = *(size_t *)pucBuf;
				pucBuf += 4;
				ullPtr += 4;
				// 获取学生名长度
				nLen = *(size_t *)pucBuf;
				pucBuf += 4;
				ullPtr += 4;
				// 获取课程名
				memset(szBuf, 0, sizeof(szBuf));
				memcpy(szBuf, pucBuf, nLen);
				stStu.strName = szBuf;
				pucBuf += nLen;
				ullPtr += nLen;

				while (TRUE)
				{
					StudentCourse stStuCo;
					// 获取课程ID值或者是结束符号
					stStuCo.nCourseID = *(size_t *)pucBuf;
					memcpy(szEndStrMark, pucBuf, 4);
					pucBuf += 4;
					ullPtr += 4;
					// 链表内容拿取结束后必定有一个ENDMARK标记
					if (!strcmp(szEndStrMark, ENDMARK))
					{
						// 遇到了结束标志说明链表中没有内容
						break;
					}
					// 获取学生选修的该课程的分数
					stStuCo.uiPoint = *(size_t *)pucBuf;
					pucBuf += 4;
					ullPtr += 4;
					// 获取字符串长度
					nLen = *(size_t *)pucBuf;
					pucBuf += 4;
					ullPtr += 4;
					// 获取字符串内容
					memset(szBuf, 0, sizeof(szBuf));
					memcpy(szBuf, pucBuf, nLen);
					stStuCo.strCourseName = szBuf;
					pucBuf += nLen;
					ullPtr += nLen;
					// 将课程信息插入到链表尾部
					stStu.lstOfCourse.InsertTail(stStuCo);
				}
				// 将结点插入g_stStudentInfoTree二叉树中
				if (!g_stStudentInfoTree.InsertNode(stStu, stStu.uiID))
				{
					g_stStudentInfoTree.ReleaseTree();
					return(FALSE);
				}
			}
		}
		fOk = TRUE;
	} while (FALSE);

	if (pFile)
	{
		fclose(pFile);
		pFile = nullptr;
	}

	return(fOk);
}

