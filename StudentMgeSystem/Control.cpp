#include "Control.h"
#include "MockingString.h"

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

	return(fOk);
}


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
		fOk = g_stSrhStudentNameTree.InsertNode(stSrhStuName, nHash);
	}
	else
	{
		// ID�������ظ����ҵ��Ǹ����ֱ�ӹ���������
		pTreeNode->m_data.lstStudentName.InsertTail(stStuNameId);
	}

	return(fOk);
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
		// ���γ�����ID��������
		stSrhCoName.lstCourseName.InsertTail(stCoNameId);
		fOk = g_stSrhCourseNameTree.InsertNode(stSrhCoName, nHash);
	}
	else
	{
		// ID�������ظ����ҵ��Ǹ����ֱ�ӹ���������
		pTreeNode->m_data.lstCourseName.InsertTail(stCoNameId);
	}

	return(fOk);
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
	PAUSEANDCLS();
}


void Control::Insert()
{
	int iCmd = 0;
	BOOLEAN fOk = TRUE;

	do
	{
		system("cls");
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
				break;
			}
			default:
			{
				ClearScreenAndOutput("������1-4֮�������");
				break;
			}
		}
		system("cls");
	} while (iCmd != 4);

	return;
}

void Control::Delete()
{
	int iCmd = 0;
	BOOLEAN fOk = TRUE;

	do
	{
		system("cls");
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

	system("cls");
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
		// ���stSearchIDByStudentName�ṹ��
		fOk = SetSearchIDByStudentNameStructure(pszNameBuf, uiID);
		{
			if (fOk)
			{
				stInfo.uiID = uiID;
				stInfo.strName = pszNameBuf;
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
		// ��ȡ�¿γ�IDֵ
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
		// �ѽ����������
		fOk = SetSearchIDByCourseNameStructure(pszNameBuf, uiID);
		if (fOk)
		{
			stInfo.uiCourseID = uiID;
			stInfo.strCourseName = pszNameBuf;
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
	UINT uiStudentID = 0;
	UINT uiCourseID = 0;
	BOOLEAN fFind = FALSE;
	pstStuInfo pstStudentInfo = nullptr;
	pstCourse pstCourseInfo = nullptr;
	char szBuf[LINEMAX] = { 0 };

	// ��ȡͬѧIDֵ
	do
	{
		cout << "����ѧ��ID: ";
		scanf("%u", &uiStudentID);
		ClearBuffer();
		// ȷ����ID�Ƿ����, ID��������Ҫ��������
		pstStudentInfo = g_stStudentInfoTree.FindNodeDataPtrByID(uiStudentID);
		if (!pstStudentInfo)
		{
			ClearScreenAndOutput("ѧ��������, �ٴ�������ѧ��ID");
		}
	} while (!pstStudentInfo);
	// ��ȡѡ��γ̵�IDֵ
	do
	{
		cout << "������ѡ�γ�ID: ";
		scanf("%u", &uiCourseID);
		ClearBuffer();
		// ȷ����ID�Ƿ����, ID������Ҫ��������
		pstCourseInfo = g_stCourseInfoTree.FindNodeDataPtrByID(uiCourseID);
		if (!pstCourseInfo)
		{
			ClearScreenAndOutput("�γ̲�����, �ٴ�������γ�ID");
		}
	} while (!pstCourseInfo);
	
	// ����ѧ����Ϣѡ����Ϣ
	// ȷ��ѧ��û��ѡ�����ſ�
	if (!pstStudentInfo->lstOfCourse.ChkStudentChoseCourse(pstCourseInfo->strCourseName.GetString(),
		pstCourseInfo->strCourseName.GetBufLen()))
	{
		// ûѡ�ι����ſ�����Ӽ�¼
		StudentCourse stStuCourse;
		UINT uiPoint = 0;
		cout << "������γ̳ɼ�(0-100): ";
		scanf("%u", &uiPoint);
		ClearBuffer();
		// ���γ���Ϣ, ����ѡ�β���
		stStuCourse.uiPoint = uiPoint;
		stStuCourse.strCourseName = pstCourseInfo->strCourseName;
		pstStudentInfo->lstOfCourse.InsertTail(stStuCourse);

		// �ڿγ��������ѡ���˸ÿγ̵�ѧ��
		pstCourse pstCo = g_stCourseInfoTree.FindNodeDataPtrByID(uiCourseID);
		if (!pstCo)
		{
			ClearScreenAndOutput("��ӿγ̼�¼����! ���ڴγ���");
			return;
		}
		StuChosenCourse stChnCo;
		// ͨ��ID��ѯ��ѧ��������
		stChnCo.strName = pstStudentInfo->strName;
		stChnCo.uiPoint = uiPoint;
		stChnCo.nStuID = pstStudentInfo->uiID;
		pstCo->lstOfStu.InsertTail(stChnCo);
		ClearScreenAndOutput("��ӿγ̳ɹ�!");
	}
	else
	{
		// �Ѿ�ѡ�����ſ�����ʾ�޷��ٴ�ѡ��
		ClearScreenAndOutput("�Ѿ�ѡ�����ſ���,�޷��ٴ�ѡ��");
	}
}

void Control::DelStudent()
{
	UINT uiID = 0;
	BOOLEAN fOk = FALSE;
	PCSTR pstrStuName = nullptr;
	stStuInfo stInfo;
	size_t nHash = 0;

	cout << "����ѧ��ID: ";
	scanf("%u", &uiID);
	ClearBuffer();
	// ͨ��IDѰ�Ҹý���Ƿ����
	fOk = g_stStudentInfoTree.FindNodeByID(uiID, stInfo);
	if (fOk)
	{
		// ���ָ��ѧ�����ֵ�ָ��
		pstrStuName = stInfo.strName.GetString();
		// �������ֵ�hashֵ
		nHash = NameHashCalc(pstrStuName, stInfo.strName.GetBufLen() - 1);
		// ͨ��hash�ҵ�g_stSrhStudentNameTree�ϵĽ�㲢ɾ��
		fOk = g_stSrhStudentNameTree.DeleteNode(nHash);
		if (fOk)
		{
			CArray<char *> CourseNameAry;
			// ɾ��ѧ��AVL���ϵ�ѧ����Ϣ
			fOk = g_stStudentInfoTree.DeleteNodeStu(uiID, CourseNameAry);
			if (fOk)
			{
				BOOLEAN fFind = FALSE;
				size_t nHash = 0;
				// �ڿγ�AVL����ѧ�������ϵĽ��ɾ��
				for (size_t nCourseIdx = 0; nCourseIdx < CourseNameAry.GetSize(); ++nCourseIdx)
				{
					nHash = NameHashCalc(CourseNameAry[nCourseIdx], strlen(CourseNameAry[nCourseIdx]));
					// ͨ���γ���, ɾ��g_stCourseInfoTree�ϵ�ѧ��ѡ��������
					fOk = g_stCourseInfoTree.DeleteStuChosenCourseFromCourseInfoTree(
						CourseNameAry[nCourseIdx], nHash, pstrStuName);
				}
			}
		}
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
	stCourse stInfo;
	size_t nHash = 0;
	PSTR pstrCoName = nullptr;
	cout << "����γ�ID: ";
	scanf("%u", &uiID);
	ClearBuffer();

	// ͨ��IDѰ�Ҹý���Ƿ����
	fOk = g_stCourseInfoTree.FindNodeByID(uiID, stInfo);
	if (fOk)
	{
		// ���ָ��ѧ�����ֵ�ָ��
		pstrCoName = stInfo.strCourseName.GetString();
		// �������ֵ�hashֵ
		nHash = NameHashCalc(pstrCoName, stInfo.strCourseName.GetBufLen() - 1);
		// ͨ��hash�ҵ�g_stSrhCourseNameTree�ϵĽ�㲢ɾ��
		fOk = g_stSrhCourseNameTree.DeleteNode(nHash);
		if (fOk)
		{
			CArray<char *> StudentNameAry;
			// ɾ���γ�AVL���ϵĽ��
			fOk = g_stCourseInfoTree.DeleteNodeCo(uiID, StudentNameAry);
			if (fOk)
			{
				BOOLEAN fFind = FALSE;
				size_t nHash = 0;
				// ��ѧ��AVL���Ŀγ������ϵĽ��ɾ��
				for (size_t nStuIdx = 0; nStuIdx < StudentNameAry.GetSize(); ++nStuIdx)
				{
					nHash = NameHashCalc(StudentNameAry[nStuIdx], strlen(StudentNameAry[nStuIdx]));
					// ͨ���γ���, ɾ��g_stCourseInfoTree�ϵ�ѧ��ѡ��������
					fOk = g_stStudentInfoTree.DeleteCoursePointRecFromStudentInfoTree(
						StudentNameAry[nStuIdx], nHash, pstrCoName);
				}
			}
		}
	}
	if (fOk)
	{
		ClearScreenAndOutput("ɾ���γ̳ɹ�!");
	}
	else
	{
		ClearScreenAndOutput("ɾ��ʧ��, �ÿγ̲�����!");
	}
}


void Control::DelRecord()
{
	UINT uiStudentID = 0;
	UINT uiCourseID = 0;
	pstStuInfo pstStudentInfo = nullptr;
	pstCourse pstCourseInfo = nullptr;
	BOOLEAN fOk = FALSE;

	// ��ȡͬѧIDֵ
	do
	{
		cout << "����ѧ��ID: ";
		scanf("%u", &uiStudentID);
		ClearBuffer();
		// ȷ����ID�Ƿ����, ID��������Ҫ��������
		pstStudentInfo = g_stStudentInfoTree.FindNodeDataPtrByID(uiStudentID);
		if (!pstStudentInfo)
		{
			ClearScreenAndOutput("ѧ��������, �ٴ�������ѧ��ID");
		}
	} while (!pstStudentInfo);
	// ��ȡѡ��γ̵�IDֵ
	do
	{
		cout << "������ѡ�γ�ID: ";
		scanf("%u", &uiCourseID);
		ClearBuffer();
		// ȷ����ID�Ƿ����, ID������Ҫ��������
		pstCourseInfo = g_stCourseInfoTree.FindNodeDataPtrByID(uiCourseID);
		if (!pstCourseInfo)
		{
			ClearScreenAndOutput("�γ̲�����, �ٴ�������γ�ID");
		}
	} while (!pstCourseInfo);

	//
	// ���˿γ�ID��ѧ��ID�Լ������Ϣ���Ѿ���ȡ
	// ����ʱֻ��ȷ������γ̺�ѧ���Ǵ��ڵģ�����
	// ѧ����û��ѡ�����ſ��ǲ�ȷ����
	//

	// �����ѡ�μ�¼��������ɾ��ѧ����Ϣ�����ڵ�ѡ�μ�¼, ����������򷵻�false
	fOk = pstStudentInfo->lstOfCourse.DeleteCourseRecordFromStudentTreeCourseList(pstCourseInfo->strCourseName);
	if (fOk)
	{
		// ����ɾ���γ���Ϣ�����ڵ�ѧ����¼
		fOk = pstCourseInfo->lstOfStu.DeleteStudentRecordFromCourseTreeStudentList(pstStudentInfo->strName);
		if (fOk)
		{
			ClearScreenAndOutput("�ɹ�ɾ��ѡ�μ�¼");
			return;
		}
	}
	ClearScreenAndOutput("����, û�и�ѡ�μ�¼");
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
	// ͨ��ѧ������ѯID��
	if (fOk)
	{
		CArray<size_t> cIDAry;
		// ��hash���ж����Ӧ��ѧ��, ͨ��ѧ��������һ��ȷ��
		fOk = stSrhStuName.lstStudentName.FindStuId(szNameBuf, cIDAry);
		if (fOk)
		{
			char szBuf[LINEMAX] = { 0 };

			cout << "ѧ����Ϣ: " << endl;
			for (size_t nIdx = 0; nIdx < cIDAry.GetSize(); ++nIdx)
			{
				sprintf(szBuf, "ѧ��ID: %u\r\nѧ����: %s\r\n", cIDAry[nIdx], szNameBuf);
				cout << szBuf << endl;
			}
			PAUSEANDCLS();
			return;
		}
	}

	ClearScreenAndOutput("δ�鵽��ѧ����Ϣ, ��ȷ��ѧ�������Ƿ�����");
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
	size_t nID = 0;
	stCourse stInfo;
	BOOLEAN fOk = FALSE;
	char szNameBuf[LINEMAX] = { 0 };
	stSearchIDByCourseName stSrhCoName;
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
	// ͨ��HASHֵ���Ҷ�Ӧ�γ���Ϣ
	fOk = g_stSrhCourseNameTree.FindNodeByID(nHash, stSrhCoName);
	// ͨ���γ�����ѯID��
	if (fOk)
	{
		// ��hash���ж����Ӧ�Ŀγ�, ͨ���γ�����һ��ȷ��
		fOk = stSrhCoName.lstCourseName.FindCoId(szNameBuf, nID);
		if (fOk)
		{
			char szBuf[LINEMAX] = { 0 };
			sprintf(szBuf, "�γ���Ϣ:\r\n�γ�ID: %u\r\n�γ���: %s\r\n", nID, szNameBuf);
			ClearScreenAndOutput(szBuf, FALSE);
			return;
		}
	}

	ClearScreenAndOutput("δ�鵽�ÿγ���Ϣ, ��ȷ�Ͽγ����Ƿ�����");
}

void Control::QueryAllStuAndPointsByID()
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
		sprintf(szBuf, "�γ���Ϣ: \r\nID: %u\r\n�γ���: %s\r\n", uiID, stInfo.strCourseName);
		cout << szBuf;
		printf("ѧ��ID\tѧ����\t�ɼ�\r\n");
		stInfo.lstOfStu.TraverseList();
		PAUSEANDCLS();
	}
}

void Control::QueryCoInfoByID()
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
		cout << szBuf;
		stInfo.lstOfCourse.TraverseList();
		PAUSEANDCLS();
	}
}
