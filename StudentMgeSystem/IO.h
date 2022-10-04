#pragma once
#include "Global.h"
#include "CAVLTree.hpp"

class IO
{
public:
	// IO����
	typedef enum _eIOType
	{
		READONLY = 1,
		CREATE
	} eIOType;
public:
	IO() = default;
	IO(const char *pcszFilePathName, eIOType eMode = READONLY);
	IO(const CMyString &strFileName, eIOType eMode = READONLY);
	// ����IOģʽ
	inline void SetMode(eIOType eIOMode)
	{
		m_eMode = eIOMode;
	}
	// ��ȡ�ļ���С
	ULONGLONG IOGetFileSize();
	// ���ļ��а��ж�ȡ����
	char **GetLineFromFile(size_t &rnSizeOfStringAry);
	// �����ļ���
	void UpdateFileName(const char *pcszFilePathName)
	{
		if (!pcszFilePathName)
		{
			return;
		}
		m_strFileName = pcszFilePathName;
	}
	// �����ļ���
	void UpdateFileName(CMyString &strFileName)
	{
		UpdateFileName(strFileName.GetString());
	}
	// �ж��ļ��Ƿ����
	BOOLEAN FileExists(const char *pcszFileName);


	// �洢����AVL���õĺ���
	template <typename T>
	BOOLEAN RestoreSrhAVLTreeInfo(CAVLTree<T> &crAVLTree, BOOLEAN fSrhCourseInfo = TRUE);
	// ��ȡ����AVL���õĺ���
	template <typename T>
	BOOLEAN LoadSrhAVLTreeInfo(CAVLTree<T> &crAVLTree, BOOLEAN fSrhCourseInfo = TRUE);

	// �洢AVL���õĺ���
	template <typename T>
	BOOLEAN RestoreAVLTreeInfo(CAVLTree<T> &crAVLTree, BOOLEAN fCourseInfo = TRUE);
	// �洢AVL���õĺ���
	template <typename T>
	BOOLEAN LoadAVLTreeInfo(CAVLTree<T> &crAVLTree, BOOLEAN fCourseInfo = TRUE);
private:
	// �ͷ�ָ������Ŀռ�
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
			// ��ȡg_stSrhCourseNameTree�����ݲ����浽�ڴ���
			pucBuf = crAVLTree.AllocHeapMemoryAndSetSrhCourseData(ullHeapDataSize);
			if (!pucBuf)
			{
				break;
			}
			pcszFileName = SRHCOURSEFILENAME;
		}
		else
		{
			// ��ȡg_stSrhStudentNameTree�����ݱ��浽�ڴ���
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

	// ȷ���ǿγ���Ϣ����ѧ����Ϣ
	if (!fSrhCourseInfo)
	{
		pcszFileName = SRHSTUDENTFILENAME;
	}

	do
	{
		// ��srh*.dat�����ļ�
		pFile = fopen(pcszFileName, "rb");
		if (!pFile)
		{
			break;
		}
		// �����ļ�
		UpdateFileName(pcszFileName);
		// ����ļ���С
		ullFileSize = IOGetFileSize();
		if (!ullFileSize)
		{
			break;
		}
		// ����ѿռ�
		pucBuf = new unsigned char[ullFileSize];
		if (!pucBuf)
		{
			break;
		}
		// ��srh*.dat�л�ȡ����
		nLen = fread(pucBuf, ullFileSize, 1, pFile);
		if (nLen != 1)
		{
			break;
		}
		
		// ȷ���Ƕ�ȡ�γ���Ϣ����ѧ����Ϣ
		if (fSrhCourseInfo)
		{
			// �ͷŶ�����
			g_stSrhCourseNameTree.ReleaseTree();
			
			while (ullPtr < ullFileSize)
			{
				stSearchIDByCourseName stSrhIDCourseName;
				
				// ��ȡhashֵ
				stSrhIDCourseName.nNameHash = *(size_t *)pucBuf;
				pucBuf += 4;
				ullPtr += 4;
				while (TRUE)
				{
					stCourseNameAndID stCoNameID;
					// ��ȡ�γ�IDֵ
					stCoNameID.uiCourseID = *(size_t *)pucBuf;
					memcpy(szEndStrMark, pucBuf, 4);
					pucBuf += 4;
					ullPtr += 4;
					// ����������ȡ������ض���һ��ENDMARK���
					if (!strcmp(szEndStrMark, ENDMARK))
					{
						// �õ�hash��������˽�����־˵��������û������
						break;
					}
					// ��ȡ�ַ�������
					nLen = *(size_t *)pucBuf;
					pucBuf += 4;
					ullPtr += 4;
					// ��ȡ�ַ�������
					memset(szBuf, 0, LINEMAX);
					memcpy(szBuf, pucBuf, nLen);
					stCoNameID.strCourseName = szBuf;
					pucBuf += nLen;
					ullPtr += nLen;
					// ���γ���Ϣ���뵽����β��
					stSrhIDCourseName.lstCourseName.InsertTail(stCoNameID);
				}
				// ��������g_stSrhCourseNameTree��������
				if (!g_stSrhCourseNameTree.InsertNode(stSrhIDCourseName, stSrhIDCourseName.nNameHash))
				{
					g_stSrhCourseNameTree.ReleaseTree();
					return(FALSE);
				}
			}
		}
		else
		{
			// �ͷŶ�����
			g_stSrhStudentNameTree.ReleaseTree();
			while (ullPtr < ullFileSize)
			{
				stSearchIDByStudentName stSrhIDStudentName;

				// ��ȡhashֵ
				stSrhIDStudentName.nNameHash = *(size_t *)pucBuf;
				pucBuf += 4;
				ullPtr += 4;
				while (TRUE)
				{
					stStudentNameAndID stStuNameID;
					// ��ȡѧ��IDֵ
					stStuNameID.uiStudentID = *(size_t *)pucBuf;
					memcpy(szEndStrMark, pucBuf, 4);
					pucBuf += 4;
					ullPtr += 4;
					// ����������ȡ������ض���һ��ENDMARK���
					if (!strcmp(szEndStrMark, ENDMARK))
					{
						// �õ�hash��������˽�����־˵��������û������
						break;
					}
					// ��ȡ�ַ�������
					nLen = *(size_t *)pucBuf;
					pucBuf += 4;
					ullPtr += 4;
					// ��ȡ�ַ�������
					memset(szBuf, 0, LINEMAX);
					memcpy(szBuf, pucBuf, nLen);
					stStuNameID.strStudentName = szBuf;
					pucBuf += nLen;
					ullPtr += nLen;
					// ���γ���Ϣ���뵽����β��
					stSrhIDStudentName.lstStudentName.InsertTail(stStuNameID);
				}
				// ��������g_stSrhCourseNameTree��������
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
			// ��ȡg_stCourseInfoTree�����ݲ����浽�ڴ���
			pucBuf = crAVLTree.AllocHeapMemoryAndSetCourseDataX(ullHeapDataSize);
			if (!pucBuf)
			{
				break;
			}
			pcszFileName = SAVECOURSEFILENAME;
		}
		else
		{
			// ��ȡg_stStudentInfoTree�����ݱ��浽�ڴ���
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

	// ȷ���ǿγ���Ϣ����ѧ����Ϣ
	if (!fCourseInfo)
	{
		pcszFileName = SAVESTUDENTFILENAME;
	}

	do
	{
		// ��*.dat�����ļ�
		pFile = fopen(pcszFileName, "rb");
		if (!pFile)
		{
			break;
		}
		// �����ļ�
		UpdateFileName(pcszFileName);
		// ����ļ���С
		ullFileSize = IOGetFileSize();
		if (!ullFileSize)
		{
			break;
		}
		// ����ѿռ�
		pucBuf = new unsigned char[ullFileSize];
		if (!pucBuf)
		{
			break;
		}
		// ��*.dat�л�ȡ����
		nLen = fread(pucBuf, ullFileSize, 1, pFile);
		if (nLen != 1)
		{
			break;
		}

		// ȷ���Ƕ�ȡ�γ���Ϣ����ѧ����Ϣ
		if (fCourseInfo)
		{
			// �ͷŶ�����
			g_stCourseInfoTree.ReleaseTree();

			while (ullPtr < ullFileSize)
			{
				stCourse stCo;

				// ��ȡ�γ�IDֵ
				stCo.uiCourseID = *(size_t *)pucBuf;
				pucBuf += 4;
				ullPtr += 4;
				// ��ȡ�γ�������
				nLen = *(size_t *)pucBuf;
				pucBuf += 4;
				ullPtr += 4;
				// ��ȡ�γ���
				memset(szBuf, 0, sizeof(szBuf));
				memcpy(szBuf, pucBuf, nLen);
				stCo.strCourseName = szBuf;
				pucBuf += nLen;
				ullPtr += nLen;

				while (TRUE)
				{
					StuChosenCourse stChosenCo;
					// ��ȡѧ��IDֵ�����ǽ�������
					stChosenCo.nStuID = *(size_t *)pucBuf;
					memcpy(szEndStrMark, pucBuf, 4);
					pucBuf += 4;
					ullPtr += 4;
					// ����������ȡ������ض���һ��ENDMARK���
					if (!strcmp(szEndStrMark, ENDMARK))
					{
						// �����˽�����־˵��������û������
						break;
					}
					// ��ȡѧ��ѡ�޵ĸÿγ̵ķ���
					stChosenCo.uiPoint = *(size_t *)pucBuf;
					pucBuf += 4;
					ullPtr += 4;
					// ��ȡ�ַ�������
					nLen = *(size_t *)pucBuf;
					pucBuf += 4;
					ullPtr += 4;
					// ��ȡ�ַ�������
					memset(szBuf, 0, sizeof(szBuf));
					memcpy(szBuf, pucBuf, nLen);
					stChosenCo.strName = szBuf;
					pucBuf += nLen;
					ullPtr += nLen;
					// ��ѧ����Ϣ���뵽����β��
					stCo.lstOfStu.InsertTail(stChosenCo);
				}
				// ��������g_stCourseInfoTree��������
				if (!g_stCourseInfoTree.InsertNode(stCo, stCo.uiCourseID))
				{
					g_stCourseInfoTree.ReleaseTree();
					return(FALSE);
				}
			}
		}
		else
		{
			// �ͷŶ�����
			g_stStudentInfoTree.ReleaseTree();

			while (ullPtr < ullFileSize)
			{
				stStuInfo stStu;

				// ��ȡѧ��IDֵ
				stStu.uiID = *(size_t *)pucBuf;
				pucBuf += 4;
				ullPtr += 4;
				// ��ȡѧ��������
				nLen = *(size_t *)pucBuf;
				pucBuf += 4;
				ullPtr += 4;
				// ��ȡ�γ���
				memset(szBuf, 0, sizeof(szBuf));
				memcpy(szBuf, pucBuf, nLen);
				stStu.strName = szBuf;
				pucBuf += nLen;
				ullPtr += nLen;

				while (TRUE)
				{
					StudentCourse stStuCo;
					// ��ȡ�γ�IDֵ�����ǽ�������
					stStuCo.nCourseID = *(size_t *)pucBuf;
					memcpy(szEndStrMark, pucBuf, 4);
					pucBuf += 4;
					ullPtr += 4;
					// ����������ȡ������ض���һ��ENDMARK���
					if (!strcmp(szEndStrMark, ENDMARK))
					{
						// �����˽�����־˵��������û������
						break;
					}
					// ��ȡѧ��ѡ�޵ĸÿγ̵ķ���
					stStuCo.uiPoint = *(size_t *)pucBuf;
					pucBuf += 4;
					ullPtr += 4;
					// ��ȡ�ַ�������
					nLen = *(size_t *)pucBuf;
					pucBuf += 4;
					ullPtr += 4;
					// ��ȡ�ַ�������
					memset(szBuf, 0, sizeof(szBuf));
					memcpy(szBuf, pucBuf, nLen);
					stStuCo.strCourseName = szBuf;
					pucBuf += nLen;
					ullPtr += nLen;
					// ���γ���Ϣ���뵽����β��
					stStu.lstOfCourse.InsertTail(stStuCo);
				}
				// ��������g_stStudentInfoTree��������
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

