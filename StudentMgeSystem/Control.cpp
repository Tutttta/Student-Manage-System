#include "Control.h"
#include "MockingString.h"
#include <conio.h>



void Control::Run()
{
	int iCmd = 0;
	BOOLEAN fOk = TRUE;

	cout << "���ڳ�ʼ��ϵͳ" << endl;
	// ��ʼ��ϵͳ
	fOk = InitSystem();
	if (!fOk)
	{
		_getch();
		return;
	}
	ClearScreenAndOutput("��ʼ���ɹ�, �������ʹ��");
	do 
	{
		m_UI.PrintMenu(MAIN);
		scanf("%d", &iCmd);
		ClearBuffer();
		switch (iCmd)
		{
			case 1:
			{
				Insert();
				break;
			}
			case 2:
			{
				Delete();
				break;
			}
			case 3:
			{
				Query();
				break;
			}
			case 4:
			{
				Statistic();
				break;
			}
			case 5:
			{
				ClearScreenAndOutput("��лʹ�ñ�ϵͳ, �ټ�!");
				break;
			}
			default:
			{
				ClearScreenAndOutput("������1-5֮�������");
				break;
			}
		}
	} while (iCmd != 5);

	return;
}

BOOLEAN Control::InitSystem()
{
	char szStudentNameFilePath[LINEMAX] = { 0 };
	char szCourseNameFilePath[LINEMAX] = { 0 };
	PPSTR ppStudentsNameAry = nullptr;
	PPSTR ppCoursesNameAry = nullptr;
	size_t nLineOfStudentNames = 0;
	size_t nLineOfCourseNames = 0;
	BOOLEAN bInitSuccess = FALSE;

	do
	{
		// �ж���ȡ�ļ��Ƿ����, ������ֱ�ӷ���
		if (!ChkInitFileExists(szStudentNameFilePath, LINEMAX, STUDENTFILENAME) ||
			!ChkInitFileExists(szCourseNameFilePath, LINEMAX, COURSEFILENAME))
		{
			break;
		}
		// ���ļ��ж�ȡ����ѧ������
		m_IO.UpdateFileName(szStudentNameFilePath);
		ppStudentsNameAry = m_IO.GetLineFromFile(nLineOfStudentNames);
		if (!ppStudentsNameAry)
		{
			cout << "��ȡѧ����Ϣʧ��, ���ٴγ���" << endl;
			break;
		}
		// ���ļ��ж�ȡ���пγ�����
		m_IO.UpdateFileName(szCourseNameFilePath);
		ppCoursesNameAry = m_IO.GetLineFromFile(nLineOfCourseNames);
		if (!ppCoursesNameAry)
		{
			cout << "���γ���Ϣʧ��, ���ڴγ���" << endl;
			break;
		}
		// ��ʼ��ѧ����Ϣ
		bInitSuccess = InitStudentInfo(ppStudentsNameAry, nLineOfStudentNames);
		if (!bInitSuccess)
		{
			cout << "��ʼ��ѧ����Ϣʧ��, ���ڴγ���" << endl;
			break;
		}
		// ��ʼ���γ���Ϣ
		bInitSuccess = InitCourseInfo(ppCoursesNameAry, nLineOfCourseNames);
		if (!bInitSuccess)
		{
			cout << "��ʼ���γ���Ϣʧ��, ���ڴγ���" << endl;
			break;
		}

		bInitSuccess = TRUE;
	} while (FALSE);
	
	return(bInitSuccess);
}

BOOLEAN Control::InitStudentInfo(PPSTR ppstrStudentNameAry, size_t nSize)
{
	BOOLEAN fOk = TRUE;
	int		iLen = 0;
	PSTR    pszBuf = nullptr;

	if (!ppstrStudentNameAry || !nSize)
	{
		return(FALSE);
	}
	
	for (size_t nIdx = 0; nIdx < nSize; ++nIdx)
	{
		stStuInfo stStudentInfo;
		stSearchIDByStudentName SrhIDByStuName;

		// ����������Ϣ
		stStudentInfo.uiID = nIdx;
		stStudentInfo.strName = ppstrStudentNameAry[nIdx];
		// ��������㣬��һ��ʧ�����ͷ����ռ䷵��FALSE
		if (!SetSearchIDByStudentNameStructure(ppstrStudentNameAry[nIdx], nIdx))
		{
			fOk = FALSE;
			g_stSrhStudentNameTree.ReleaseTree();
			break;
		}
		// ��������㣬��һ��ʧ�����ͷ����ռ䷵��FALSE
		if (!g_stStudentInfoTree.InsertNode(stStudentInfo, nIdx))
		{
			fOk = FALSE;
			g_stStudentInfoTree.ReleaseTree();
			break;
		}
	}

# if 0
	for (size_t nIdx = 0; nIdx < g_stAryOfStudentName.GetSize(); ++nIdx)
	{
		cout << "ѧ����: " << g_stAryOfStudentName[nIdx].strStudentName << endl;
		cout << "ID: " << g_stAryOfStudentName[nIdx].uiStudentID << endl;
		cout << "Hash: " << g_stAryOfStudentName[nIdx].nNameHash << endl;
		cout << endl;
	}
#endif
	
	return(fOk);
}

BOOLEAN Control::InitCourseInfo(PPSTR ppstrCourseNameAry, size_t nSize)
{
	BOOLEAN fOk = TRUE;

	if (!ppstrCourseNameAry || !nSize)
	{
		return(FALSE);
	}

	for (size_t nIdx = 0; nIdx < nSize; ++nIdx)
	{
		stCourse stCourseInfo;
		stSearchIDByCourseName SrhIDByCoName;

		// ����������Ϣ
		stCourseInfo.uiCourseID = nIdx;
		stCourseInfo.strCourseName = ppstrCourseNameAry[nIdx];
		// ��������㣬��һ��ʧ�����ͷ����ռ䷵��FALSE
		if (!SetSearchIDByCourseNameStructure(ppstrCourseNameAry[nIdx], nIdx))
		{
			fOk = FALSE;
			g_stSrhCourseNameTree.ReleaseTree();
			break;
		}
		// ��������㣬��һ��ʧ�����ͷ����ռ䷵��FALSE
		if (!g_stCourseInfoTree.InsertNode(stCourseInfo, nIdx))
		{
			fOk = FALSE;
			g_stCourseInfoTree.ReleaseTree();
			break;
		}
	}

#if 0
	for (size_t nIdx = 0; nIdx < g_stAryOfCourseName.GetSize(); ++nIdx)
	{
		cout << "�γ���: " << g_stAryOfCourseName[nIdx].strCourseName << endl;
		cout << "ID: " << g_stAryOfCourseName[nIdx].uiCourseID << endl;
		cout << "Hash: " << g_stAryOfCourseName[nIdx].nNameHash << endl;
		cout << endl;
	}
#endif
	
	return(fOk);
}

/*
// һ��BST���洢 ID��Ϊ���ֵ, �洢tagStudentInfo�ṹ��
// ѧ��ID => ѧ������ ��
// ѧ��ID => ѧ��ѡ�޵Ŀγ���Ϣ ��
// ѧ������ => ѧ��ID ��
typedef struct tagStudentInfo
{
	UINT uiID;							// ѧ��ID
	CMyString strName;					// ѧ������
	CList<StudentCourse> lstOfCourse;	// ѧ���γ̺ͳɼ�����
} stStuInfo, *pstStuInfo;


*/
BOOLEAN Control::InitSystemData()
{
	
	return(TRUE);
}

BOOLEAN Control::ChkInitFileExists(PSTR pszFilePathName, size_t nSize, PCSTR pcszFileName/* = STUDENTFILENAME*/)
{
	char szFilePathName[LINEMAX] = "..\\data\\";
	int iLen = 0;

	if (!pcszFileName)
	{
		return(FALSE);
	}
	iLen = strlen(pcszFileName);
	strncat_s(szFilePathName, LINEMAX, pcszFileName, iLen);

	// �ж�students.txt�Ƿ����
	if (!m_IO.FileExists(szFilePathName))
	{
		// ���students.txt������, �����ڵ�ǰ·����Ѱ��
		strncpy_s(szFilePathName, LINEMAX, ".\\", sizeof(".\\"));
		strncat_s(szFilePathName, LINEMAX, pcszFileName, iLen);
		if (!m_IO.FileExists(szFilePathName))
		{
			// ���ɲ�����
			printf("�޷��ҵ�%s, �뽫������뱾����ͬһĿ¼�²��ٴγ���", pcszFileName);
			return(FALSE);
		}
	}
	if (pszFilePathName)
	{
		strncpy_s(pszFilePathName, nSize, szFilePathName, strlen(szFilePathName));
	}

	return(TRUE);
}

size_t Control::NameHashCalc(PCSTR pcszName, size_t nSize)
{
	size_t nHash = 0;
	MD5_CTX Md5;
	unsigned char szMd5[32] = { 0 };

	if (!pcszName || !nSize)
	{
		return(0);
	}
	MD5Init(&Md5);
	// ����md5ֵ
	MD5Update(&Md5, (unsigned char *)pcszName, nSize);
	MD5Final(&Md5, (unsigned char *)szMd5);

	// ����md5�������Hashֵ
	for (size_t nIdx = 0; nIdx < 16; ++nIdx)
	{
		nHash += szMd5[nIdx];
	}

	return(nHash);
}

BOOLEAN Control::SetSearchIDByStudentNameStructure(PSTR pStrName, size_t nID)
{
	size_t nHash = 0;
	BOOLEAN fOk = FALSE;
	CAVLTree<stSearchIDByStudentName>::pTreeNode pTreeNode = nullptr;
	stSearchIDByStudentName stSrhStuName;
	stSearchIDByStudentName *pstSrhStuName = nullptr;
	stStudentNameAndID stStuNameId;

	if (!pStrName)
	{
		return(FALSE);
	}
	// ���ѧ������ID�ṹ��
	stStuNameId.strStudentName = pStrName;
	stStuNameId.uiStudentID = nID;

	// �������ֵ�Hashֵ
	nHash = NameHashCalc(pStrName, strlen(pStrName));
	pTreeNode = g_stSrhStudentNameTree.FindNodeByID(nHash, fOk);
	if (!pTreeNode)
	{
		// û���ҵ�����δ�ظ�, �´���
		stSrhStuName.nNameHash = nHash;
		// ��ѧ������ID��������
		stSrhStuName.lstStudentName.InsertTail(stStuNameId);
		g_stSrhStudentNameTree.InsertNode(stSrhStuName, nHash);
	}
	else
	{
		// ID�������ظ����ҵ��Ǹ����ֱ�ӹ���������
		pTreeNode->m_data.lstStudentName.InsertTail(stStuNameId);
	}

	return(TRUE);
}

BOOLEAN Control::SetSearchIDByCourseNameStructure(PSTR pStrName, size_t nID)
{
	size_t nHash = 0;
	BOOLEAN fOk = FALSE;
	CAVLTree<stSearchIDByCourseName>::pTreeNode pTreeNode = nullptr;
	stSearchIDByCourseName stSrhCoName;
	stSearchIDByCourseName *pstSrhCoName = nullptr;
	stCourseNameAndID stCoNameId;

	if (!pStrName)
	{
		return(FALSE);
	}
	// ���ѧ������ID�ṹ��
	stCoNameId.strCourseName = pStrName;
	stCoNameId.uiCourseID = nID;

	// �������ֵ�Hashֵ
	nHash = NameHashCalc(pStrName, strlen(pStrName));
	pTreeNode = g_stSrhCourseNameTree.FindNodeByID(nHash, fOk);
	if (!pTreeNode)
	{
		// û���ҵ�����δ�ظ�, �´���
		stSrhCoName.nNameHash = nHash;
		// ��ѧ������ID��������
		stSrhCoName.lstCourseName.InsertTail(stCoNameId);
	}
	else
	{
		// ID�������ظ����ҵ��Ǹ����ֱ�ӹ���������
		pTreeNode->m_data.lstCourseName.InsertTail(stCoNameId);
	}

	return(TRUE);
}

void Control::ClearScreenAndOutput(PCSTR pszText, BOOLEAN fNextLine /*= TRUE*/)
{
	system("cls");
	if (pszText)
	{
		cout << pszText;
		if (fNextLine)
		{
			cout << endl;
		}
	}
	system("pause");
	system("cls");
}

/*
	puts("���빦���Ӳ˵�: ");
	// ID������
	puts("1. ����ѧ��");
	puts("2. �����γ�");
	puts("3. ������¼");
	printf("����: ");
*/
void Control::Insert()
{
	int iCmd = 0;
	BOOLEAN fOk = TRUE;

	do
	{
		m_UI.PrintMenu(INSERT);
		scanf("%d", &iCmd);
		getchar();
		switch (iCmd)
		{
			case 1:
			{
				// ����ѧ��
				AddStudent();
				break;
			}
			case 2:
			{
				// �����γ�
				AddCourse();
				break;
			}
			case 3:
			{
				// ������¼
				AddRecord();
				break;
			}
			case 4:
			{
				system("cls");
				break;
			}
			default:
			{
				ClearScreenAndOutput("������1-4֮�������");
				break;
			}
		}
	} while (iCmd != 4);

	return;
}

void Control::Delete()
{
	int iCmd = 0;
	BOOLEAN fOk = TRUE;

	/*
	puts("ɾ�������Ӳ˵�: ");
	// ID
	puts("1. ɾ��ѧ��");
	puts("2. ɾ���γ�");
	puts("3. ɾ��ѡ�μ�¼");
	puts("4. ������һ��");
	printf("����: ");
	*/
	do
	{
		m_UI.PrintMenu(DELETE);
		scanf("%d", &iCmd);
		getchar();
		switch (iCmd)
		{
			case 1:
			{
				// ɾ��ѧ��
				DelStudent();
				break;
			}
			case 2:
			{
				// ɾ���γ�
				DelCourse();
				break;
			}
			case 3:
			{
				// ɾ��ѡ�μ�¼
				DelRecord();
				break;
			}
			case 4:
			{
				system("cls");
				break;
			}
			default:
			{
				ClearScreenAndOutput("������1-4֮�������");
				break;
			}
		}
	} while (iCmd != 4);

	return;
}

void Control::Query()
{
	int iCmd = 0;
	BOOLEAN fOk = TRUE;

	/*
	puts("��ѯ�����Ӳ˵�: ");
	// ID
	puts("1. ͨ��ѧ��ID��ѯѧ������");
	puts("2. ͨ��ѧ��������ѯѧ��ID");
	puts("3. ͨ���γ�ID��ѯ�γ���");
	puts("4. ͨ���γ�����ѯ�γ�ID");
	puts("5. ͨ���γ�ID��ѯѡ���˸ÿγ̵�ѧ���Լ��ɼ�");
	puts("6. ͨ��ѧ��ID��ѯ��ѧ��ѡ�޵Ŀγ���Ϣ");
	printf("����: ");
	*/
	do
	{
		m_UI.PrintMenu(QUERY);
		scanf("%d", &iCmd);
		ClearBuffer();
		switch (iCmd)
		{
			case 1:
			{
				QueryStuNameByID();
				break;
			}
			case 2:
			{
				QueryIDByStuName();
				break;
			}
			case 3:
			{
				QueryCoNameByID();
				break;
			}
			case 4:
			{
				QueryIDByCoName();
				break;
			}
			case 5:
			{
				QueryAllStuAndPointsByID();
				break;
			}
			case 6:
			{
				QueryCoInfoByID();
				break;
			}
			case 7:
			{
				system("cls");
				break;
			}
		}
	} while (iCmd != 7);
	
	return;
}

void Control::Statistic()
{
	ClearScreenAndOutput("ͳ�ƹ���");
}

void Control::AddStudent()
{
	UINT uiID = 0;
	CMyString strStuName;
	BOOLEAN fFind = FALSE;
	BOOLEAN fOk = FALSE;
	stStuInfo stInfo = { 0 };
	stSearchIDByStudentName stSrhStuInfo = { 0 };
	char *pszNameBuf = nullptr;
	char *pszNameBufx = nullptr;
	char szNameBufTmp[LINEMAX] = { 0 };
	size_t nLen = 0;

	do
	{
		// ��ȡ��ͬѧIDֵ
		do
		{
			cout << "������ѧ��ID: ";
			scanf("%u", &uiID);
			ClearBuffer();
			// ȷ����ID�Ƿ����, ID�����ظ�����ظ���Ҫ��������
			fFind = g_stStudentInfoTree.FindNodeByID(uiID);
			if (fFind)
			{
				ClearScreenAndOutput("ID�ظ�, ������һ�����ظ�����ID");
			}
		} while (fFind);

		// ��ȡ��ͬѧ����
		do
		{
			cout << "��������ѧ������: ";
			pszNameBufx = GetsEx(szNameBufTmp, sizeof(szNameBufTmp));
			if (!pszNameBufx)
			{
				ClearScreenAndOutput("��������ʧ��, ����������һ��������");
			}
		} while (!pszNameBufx);

		// �����µĶѿռ����ڴ洢��ͬѧ����
		nLen = strlen(szNameBufTmp);
		pszNameBuf = new char[nLen + 1];
		if (!pszNameBuf)
		{
			ClearScreenAndOutput("���������ʧ��! ���ٴγ���");
			break;
		}
		// �洢�����Ϣ
		strncpy_s(pszNameBuf, nLen + 1, szNameBufTmp, sizeof(szNameBufTmp));
		stInfo.uiID = uiID;
		stInfo.strName = pszNameBuf;
		// ���stSearchIDByStudentName�ṹ��
		fOk = SetSearchIDByStudentNameStructure(pszNameBuf, uiID);
		{
			if (fOk)
			{
				// �ѽ����������
				fOk = g_stStudentInfoTree.InsertNode(stInfo, uiID);
			}
		}
		if (fOk)
		{
			ClearScreenAndOutput("��������Ƴɹ�!");
		}
		else
		{
			ClearScreenAndOutput("���������ʧ��! ���ٴγ���");
		}
	} while (FALSE);
	if (!fOk && pszNameBuf)
	{
		delete[] pszNameBuf;
		pszNameBuf = nullptr;
	}

	return;
}

void Control::AddCourse()
{
	UINT uiID = 0;
	CMyString strCoName;
	BOOLEAN fFind = FALSE;
	BOOLEAN fOk = FALSE;
	stCourse stInfo = { 0 };
	stSearchIDByCourseName stSrhCoInfo = { 0 };
	char *pszNameBuf = nullptr;
	char *pszNameBufx = nullptr;
	char szNameBufTmp[LINEMAX] = { 0 };
	size_t nLen = 0;

	do
	{
		// ��ȡ��ͬѧIDֵ
		do
		{
			cout << "�����¿γ�ID: ";
			scanf("%u", &uiID);
			ClearBuffer();
			// ȷ����ID�Ƿ����, ID�����ظ�����ظ���Ҫ��������
			fFind = g_stCourseInfoTree.FindNodeByID(uiID);
			if (fFind)
			{
				ClearScreenAndOutput("ID�ظ�, ������һ�����ظ�����ID");
			}
		} while (fFind);

		// ��ȡ�¿γ�����
		do
		{
			cout << "�������¿γ���: ";
			pszNameBufx = GetsEx(szNameBufTmp, sizeof(szNameBufTmp));
			if (!pszNameBufx)
			{
				ClearScreenAndOutput("��������ʧ��, ����������һ��������");
			}
		} while (!pszNameBufx);

		// �����µĶѿռ����ڴ洢��ͬѧ����
		nLen = strlen(szNameBufTmp);
		pszNameBuf = new char[nLen + 1];
		if (!pszNameBuf)
		{
			ClearScreenAndOutput("���������ʧ��! ���ٴγ���");
			break;
		}
		// �洢�����Ϣ
		strncpy_s(pszNameBuf, nLen + 1, szNameBufTmp, sizeof(szNameBufTmp));

		// ���stSearchIDByStudentName�ṹ��
		//SetSearchIDByCourseNameStructure(&stSrhCoInfo, pszNameBuf, uiID);

		//stSrhCoInfo.uiCourseID = uiID;
		//stSrhCoInfo.strCourseName = pszNameBuf;
		//// �ѽṹ����붯̬������
		//fOk = g_stAryOfCourseName.InsertTail(stSrhCoInfo);
		if (fOk)
		{
			// �ѽ����������
			fOk = g_stCourseInfoTree.InsertNode(stInfo, uiID);
		}
		if (fOk)
		{
			ClearScreenAndOutput("����¿γ̳ɹ�!");
		}
		else
		{
			ClearScreenAndOutput("����¿γ�ʧ��! ���ٴγ���");
		}
	} while (FALSE);
	if (!fOk && pszNameBuf)
	{
		delete[] pszNameBuf;
		pszNameBuf = nullptr;
	}

	return;
}

void Control::AddRecord()
{

}

void Control::DelStudent()
{
	UINT uiID = 0;
	BOOLEAN fOk = FALSE;

	cout << "����ѧ��ID: ";
	scanf("%u", &uiID);
	ClearBuffer();
	// ɾ���ýڵ�
	fOk = g_stStudentInfoTree.DeleteNode(uiID);
	if (fOk)
	{
		//fOk = g_stAryOfStudentName.Delete()
	}
	if (fOk)
	{
		ClearScreenAndOutput("ɾ��ѧ���ɹ�!");
	}
	else
	{
		ClearScreenAndOutput("ɾ��ʧ��, ��ѧ��������!");
	}

	return;
}

void Control::DelCourse()
{
	UINT uiID = 0;
	BOOLEAN fOk = FALSE;

	cout << "����γ�ID: ";
	scanf("%u", &uiID);
	ClearBuffer();
	// ɾ���ýڵ�
	fOk = g_stCourseInfoTree.DeleteNode(uiID);
	if (fOk)
	{
		ClearScreenAndOutput("ɾ���γ̳ɹ�!");
	}
	else
	{
		ClearScreenAndOutput("ɾ��ʧ��, �ÿγ̲�����!");
	}

	return;
}

void Control::DelRecord()
{

}

void Control::QueryStuNameByID()
{
	UINT uiID = 0;
	stStuInfo stInfo;
	BOOLEAN fOk = FALSE;

	cout << "����ѧ��ID: ";
	scanf("%u", &uiID);
	ClearBuffer();
	// ͨ��ID�Ų�ѯAVL��
	fOk = g_stStudentInfoTree.FindNodeByID(uiID, stInfo);
	if (!fOk)
	{
		ClearScreenAndOutput("δ�鵽��ѧ����Ϣ, ��ȷ��ID���Ƿ�����");
	}
	else
	{
		char szBuf[LINEMAX] = { 0 };
		sprintf(szBuf, "ѧ����Ϣ: \r\nID: %u\r\n����: %s\r\n", uiID, stInfo.strName);
		ClearScreenAndOutput(szBuf, FALSE);
	}
}

void Control::QueryIDByStuName()
{
	size_t nID = 0;
	stStuInfo stInfo;
	BOOLEAN fOk = FALSE;
	char szNameBuf[LINEMAX] = { 0 };
	stSearchIDByStudentName stSrhStuName;
	char *pszTmpPtr = nullptr;
	size_t nHash = 0;

	do
	{
		cout << "����: ";
		pszTmpPtr = GetsEx(szNameBuf, LINEMAX);
		if (!pszTmpPtr)
		{
			ClearScreenAndOutput("��������ʧ��, ���ٴγ���!");
			memset(szNameBuf, 0, sizeof(szNameBuf));
		}
	} while (!pszTmpPtr);
	// �������Ƶ�HASHֵ
	nHash = NameHashCalc(szNameBuf, strlen(szNameBuf));
	// ͨ��HASHֵ���Ҷ�Ӧѧ����Ϣ
	fOk = g_stSrhStudentNameTree.FindNodeByID(nHash, stSrhStuName);
	stSrhStuName.lstStudentName.TraverseList();
	// ͨ��ѧ������ѯID��
	if (!fOk)
	{
		ClearScreenAndOutput("δ�鵽��ѧ����Ϣ, ��ȷ��ѧ�������Ƿ�����");
	}
	else
	{
		stSrhStuName.lstStudentName.FindStuId(szNameBuf, nID);
		char szBuf[LINEMAX] = { 0 };
		sprintf(szBuf, "ѧ����Ϣ:\r\nѧ��ID: %u\r\nѧ����: %s\r\n", nID, szNameBuf);
		ClearScreenAndOutput(szBuf, FALSE);
	}
}

void Control::QueryCoNameByID()
{
	UINT uiID = 0;
	stCourse stInfo;
	BOOLEAN fOk = FALSE;

	cout << "����γ�ID: ";
	scanf("%u", &uiID);
	ClearBuffer();
	// ͨ��ID�Ų�ѯAVL��
	fOk = g_stCourseInfoTree.FindNodeByID(uiID, stInfo);
	if (!fOk)
	{
		ClearScreenAndOutput("δ�鵽�ÿγ���Ϣ, ��ȷ��ID���Ƿ�����");
	}
	else
	{
		char szBuf[LINEMAX] = { 0 };
		sprintf(szBuf, "�γ���Ϣ: \r\nID: %u\r\n�γ���: %s\r\n", stInfo.uiCourseID, stInfo.strCourseName);
		ClearScreenAndOutput(szBuf, FALSE);
	}
}

void Control::QueryIDByCoName()
{

}

void Control::QueryAllStuAndPointsByID()
{

}

void Control::QueryCoInfoByID()
{

}
