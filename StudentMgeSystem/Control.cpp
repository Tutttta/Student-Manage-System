#include "Control.h"
#include "MockingString.h"

void Control::Run()
{
	int iCmd = 0;
	BOOLEAN fOk = TRUE;

	cout << "正在初始化系统" << endl;
	// 初始化系统
	fOk = InitSystem();
	if (!fOk)
	{
		_getch();
		return;
	}
	ClearScreenAndOutput("初始化成功, 按任意键使用");
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
				ClearScreenAndOutput("感谢使用本系统, 再见!");
				break;
			}
			default:
			{
				ClearScreenAndOutput("请输入1-5之间的命令");
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
		// 判定读取文件是否存在, 不存在直接返回
		if (!ChkInitFileExists(szStudentNameFilePath, LINEMAX, STUDENTFILENAME) ||
			!ChkInitFileExists(szCourseNameFilePath, LINEMAX, COURSEFILENAME))
		{
			break;
		}
		// 从文件中读取所有学生姓名
		m_IO.UpdateFileName(szStudentNameFilePath);
		ppStudentsNameAry = m_IO.GetLineFromFile(nLineOfStudentNames);
		if (!ppStudentsNameAry)
		{
			cout << "读取学生信息失败, 请再次尝试" << endl;
			break;
		}
		// 从文件中读取所有课程名称
		m_IO.UpdateFileName(szCourseNameFilePath);
		ppCoursesNameAry = m_IO.GetLineFromFile(nLineOfCourseNames);
		if (!ppCoursesNameAry)
		{
			cout << "读课程信息失败, 请在次尝试" << endl;
			break;
		}
		// 初始化学生信息
		bInitSuccess = InitStudentInfo(ppStudentsNameAry, nLineOfStudentNames);
		if (!bInitSuccess)
		{
			cout << "初始化学生信息失败, 请在次尝试" << endl;
			break;
		}
		// 初始化课程信息
		bInitSuccess = InitCourseInfo(ppCoursesNameAry, nLineOfCourseNames);
		if (!bInitSuccess)
		{
			cout << "初始化课程信息失败, 请在次尝试" << endl;
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

		// 填充树结点信息
		stStudentInfo.uiID = nIdx;
		stStudentInfo.strName = ppstrStudentNameAry[nIdx];
		// 插入树结点，有一个失败则释放树空间返回FALSE
		if (!SetSearchIDByStudentNameStructure(ppstrStudentNameAry[nIdx], nIdx))
		{
			fOk = FALSE;
			g_stSrhStudentNameTree.ReleaseTree();
			break;
		}
		// 插入树结点，有一个失败则释放树空间返回FALSE
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

		// 填充树结点信息
		stCourseInfo.uiCourseID = nIdx;
		stCourseInfo.strCourseName = ppstrCourseNameAry[nIdx];
		// 插入树结点，有一个失败则释放树空间返回FALSE
		if (!SetSearchIDByCourseNameStructure(ppstrCourseNameAry[nIdx], nIdx))
		{
			fOk = FALSE;
			g_stSrhCourseNameTree.ReleaseTree();
			break;
		}
		// 插入树结点，有一个失败则释放树空间返回FALSE
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

	// 判断students.txt是否存在
	if (!m_IO.FileExists(szFilePathName))
	{
		// 如果students.txt不存在, 尝试在当前路径下寻找
		strncpy_s(szFilePathName, LINEMAX, ".\\", sizeof(".\\"));
		strncat_s(szFilePathName, LINEMAX, pcszFileName, iLen);
		if (!m_IO.FileExists(szFilePathName))
		{
			// 依旧不存在
			printf("无法找到%s, 请将其放于与本程序同一目录下并再次尝试", pcszFileName);
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
	// 计算md5值
	MD5Update(&Md5, (unsigned char *)pcszName, nSize);
	MD5Final(&Md5, (unsigned char *)szMd5);

	// 计算md5算出来的Hash值
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
	// 填充学生名和ID结构体
	stStuNameId.strStudentName = pStrName;
	stStuNameId.uiStudentID = nID;

	// 计算名字的Hash值
	nHash = NameHashCalc(pStrName, strlen(pStrName));
	pTreeNode = g_stSrhStudentNameTree.FindNodeByID(nHash, fOk);
	if (!pTreeNode)
	{
		// 没有找到代表未重复, 新创建
		stSrhStuName.nNameHash = nHash;
		// 将学生名和ID插入链表
		stSrhStuName.lstStudentName.InsertTail(stStuNameId);
		fOk = g_stSrhStudentNameTree.InsertNode(stSrhStuName, nHash);
	}
	else
	{
		// ID发生了重复则找到那个结点直接挂再链表上
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
	// 填充学生名和ID结构体
	stCoNameId.strCourseName = pStrName;
	stCoNameId.uiCourseID = nID;

	// 计算名字的Hash值
	nHash = NameHashCalc(pStrName, strlen(pStrName));
	pTreeNode = g_stSrhCourseNameTree.FindNodeByID(nHash, fOk);
	if (!pTreeNode)
	{
		// 没有找到代表未重复, 新创建
		stSrhCoName.nNameHash = nHash;
		// 将课程名和ID插入链表
		stSrhCoName.lstCourseName.InsertTail(stCoNameId);
		fOk = g_stSrhCourseNameTree.InsertNode(stSrhCoName, nHash);
	}
	else
	{
		// ID发生了重复则找到那个结点直接挂再链表上
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
				// 新增学生
				AddStudent();
				break;
			}
			case 2:
			{
				// 新增课程
				AddCourse();
				break;
			}
			case 3:
			{
				// 新增记录
				AddRecord();
				break;
			}
			case 4:
			{
				break;
			}
			default:
			{
				ClearScreenAndOutput("请输入1-4之间的命令");
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
				// 删除学生
				DelStudent();
				break;
			}
			case 2:
			{
				// 删除课程
				DelCourse();
				break;
			}
			case 3:
			{
				// 删除选课记录
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
				ClearScreenAndOutput("请输入1-4之间的命令");
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
	ClearScreenAndOutput("统计功能");
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
		// 获取新同学ID值
		do
		{
			cout << "输入新学生ID: ";
			scanf("%u", &uiID);
			ClearBuffer();
			// 确定该ID是否存在, ID不可重复如果重复需要重新输入
			fFind = g_stStudentInfoTree.FindNodeByID(uiID);
			if (fFind)
			{
				ClearScreenAndOutput("ID重复, 请输入一个不重复的新ID");
			}
		} while (fFind);

		// 获取新同学名字
		do
		{
			cout << "请输入新学生姓名: ";
			pszNameBufx = GetsEx(szNameBufTmp, sizeof(szNameBufTmp));
			if (!pszNameBufx)
			{
				ClearScreenAndOutput("输入名称失败, 请重新输入一个新名称");
			}
		} while (!pszNameBufx);

		// 分配新的堆空间用于存储新同学名称
		nLen = strlen(szNameBufTmp);
		pszNameBuf = new char[nLen + 1];
		if (!pszNameBuf)
		{
			ClearScreenAndOutput("添加新名称失败! 请再次尝试");
			break;
		}
		// 存储结点信息
		strncpy_s(pszNameBuf, nLen + 1, szNameBufTmp, sizeof(szNameBufTmp));
		// 填充stSearchIDByStudentName结构体
		fOk = SetSearchIDByStudentNameStructure(pszNameBuf, uiID);
		{
			if (fOk)
			{
				stInfo.uiID = uiID;
				stInfo.strName = pszNameBuf;
				// 把结点插入二叉树
				fOk = g_stStudentInfoTree.InsertNode(stInfo, uiID);
			}
		}
		if (fOk)
		{
			ClearScreenAndOutput("添加新名称成功!");
		}
		else
		{
			ClearScreenAndOutput("添加新名称失败! 请再次尝试");
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
		// 获取新课程ID值
		do
		{
			cout << "输入新课程ID: ";
			scanf("%u", &uiID);
			ClearBuffer();
			// 确定该ID是否存在, ID不可重复如果重复需要重新输入
			fFind = g_stCourseInfoTree.FindNodeByID(uiID);
			if (fFind)
			{
				ClearScreenAndOutput("ID重复, 请输入一个不重复的新ID");
			}
		} while (fFind);

		// 获取新课程名字
		do
		{
			cout << "请输入新课程名: ";
			pszNameBufx = GetsEx(szNameBufTmp, sizeof(szNameBufTmp));
			if (!pszNameBufx)
			{
				ClearScreenAndOutput("输入名称失败, 请重新输入一个新名称");
			}
		} while (!pszNameBufx);

		// 分配新的堆空间用于存储新同学名称
		nLen = strlen(szNameBufTmp);
		pszNameBuf = new char[nLen + 1];
		if (!pszNameBuf)
		{
			ClearScreenAndOutput("添加新名称失败! 请再次尝试");
			break;
		}
		// 存储结点信息
		strncpy_s(pszNameBuf, nLen + 1, szNameBufTmp, sizeof(szNameBufTmp));
		// 把结点插入二叉树
		fOk = SetSearchIDByCourseNameStructure(pszNameBuf, uiID);
		if (fOk)
		{
			stInfo.uiCourseID = uiID;
			stInfo.strCourseName = pszNameBuf;
			// 把结点插入二叉树
			fOk = g_stCourseInfoTree.InsertNode(stInfo, uiID);
		}
		if (fOk)
		{
			ClearScreenAndOutput("添加新课程成功!");
		}
		else
		{
			ClearScreenAndOutput("添加新课程失败! 请再次尝试");
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

	// 获取同学ID值
	do
	{
		cout << "输入学生ID: ";
		scanf("%u", &uiStudentID);
		ClearBuffer();
		// 确定该ID是否存在, ID不存在需要重新输入
		pstStudentInfo = g_stStudentInfoTree.FindNodeDataPtrByID(uiStudentID);
		if (!pstStudentInfo)
		{
			ClearScreenAndOutput("学生不存在, 再次请输入学生ID");
		}
	} while (!pstStudentInfo);
	// 获取选则课程的ID值
	do
	{
		cout << "输入所选课程ID: ";
		scanf("%u", &uiCourseID);
		ClearBuffer();
		// 确定该ID是否存在, ID存在需要重新输入
		pstCourseInfo = g_stCourseInfoTree.FindNodeDataPtrByID(uiCourseID);
		if (!pstCourseInfo)
		{
			ClearScreenAndOutput("课程不存在, 再次请输入课程ID");
		}
	} while (!pstCourseInfo);
	
	// 更新学生信息选课信息
	// 确保学生没有选过这门课
	if (!pstStudentInfo->lstOfCourse.ChkStudentChoseCourse(pstCourseInfo->strCourseName.GetString(),
		pstCourseInfo->strCourseName.GetBufLen()))
	{
		// 没选课过这门课则添加记录
		StudentCourse stStuCourse;
		UINT uiPoint = 0;
		cout << "请输入课程成绩(0-100): ";
		scanf("%u", &uiPoint);
		ClearBuffer();
		// 填充课程信息, 并将选课插入
		stStuCourse.uiPoint = uiPoint;
		stStuCourse.strCourseName = pstCourseInfo->strCourseName;
		pstStudentInfo->lstOfCourse.InsertTail(stStuCourse);

		// 在课程树上添加选择了该课程的学生
		pstCourse pstCo = g_stCourseInfoTree.FindNodeDataPtrByID(uiCourseID);
		if (!pstCo)
		{
			ClearScreenAndOutput("添加课程记录出错! 请在次尝试");
			return;
		}
		StuChosenCourse stChnCo;
		// 通过ID查询到学生的姓名
		stChnCo.strName = pstStudentInfo->strName;
		stChnCo.uiPoint = uiPoint;
		stChnCo.nStuID = pstStudentInfo->uiID;
		pstCo->lstOfStu.InsertTail(stChnCo);
		ClearScreenAndOutput("添加课程成功!");
	}
	else
	{
		// 已经选过这门课了提示无法再次选择
		ClearScreenAndOutput("已经选过这门课了,无法再次选择");
	}
}

void Control::DelStudent()
{
	UINT uiID = 0;
	BOOLEAN fOk = FALSE;
	PCSTR pstrStuName = nullptr;
	stStuInfo stInfo;
	size_t nHash = 0;

	cout << "输入学生ID: ";
	scanf("%u", &uiID);
	ClearBuffer();
	// 通过ID寻找该结点是否存在
	fOk = g_stStudentInfoTree.FindNodeByID(uiID, stInfo);
	if (fOk)
	{
		// 获得指向学生名字的指针
		pstrStuName = stInfo.strName.GetString();
		// 计算名字的hash值
		nHash = NameHashCalc(pstrStuName, stInfo.strName.GetBufLen() - 1);
		// 通过hash找到g_stSrhStudentNameTree上的结点并删除
		fOk = g_stSrhStudentNameTree.DeleteNode(nHash);
		if (fOk)
		{
			CArray<char *> CourseNameAry;
			// 删除学生AVL树上的学生信息
			fOk = g_stStudentInfoTree.DeleteNodeStu(uiID, CourseNameAry);
			if (fOk)
			{
				BOOLEAN fFind = FALSE;
				size_t nHash = 0;
				// 在课程AVL树的学生链表上的结点删除
				for (size_t nCourseIdx = 0; nCourseIdx < CourseNameAry.GetSize(); ++nCourseIdx)
				{
					nHash = NameHashCalc(CourseNameAry[nCourseIdx], strlen(CourseNameAry[nCourseIdx]));
					// 通过课程名, 删除g_stCourseInfoTree上的学生选课链表结点
					fOk = g_stCourseInfoTree.DeleteStuChosenCourseFromCourseInfoTree(
						CourseNameAry[nCourseIdx], nHash, pstrStuName);
				}
			}
		}
	}
	if (fOk)
	{
		ClearScreenAndOutput("删除学生成功!");
	}
	else
	{
		ClearScreenAndOutput("删除失败, 该学生不存在!");
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
	cout << "输入课程ID: ";
	scanf("%u", &uiID);
	ClearBuffer();

	// 通过ID寻找该结点是否存在
	fOk = g_stCourseInfoTree.FindNodeByID(uiID, stInfo);
	if (fOk)
	{
		// 获得指向学生名字的指针
		pstrCoName = stInfo.strCourseName.GetString();
		// 计算名字的hash值
		nHash = NameHashCalc(pstrCoName, stInfo.strCourseName.GetBufLen() - 1);
		// 通过hash找到g_stSrhCourseNameTree上的结点并删除
		fOk = g_stSrhCourseNameTree.DeleteNode(nHash);
		if (fOk)
		{
			CArray<char *> StudentNameAry;
			// 删除课程AVL树上的结点
			fOk = g_stCourseInfoTree.DeleteNodeCo(uiID, StudentNameAry);
			if (fOk)
			{
				BOOLEAN fFind = FALSE;
				size_t nHash = 0;
				// 在学生AVL树的课程链表上的结点删除
				for (size_t nStuIdx = 0; nStuIdx < StudentNameAry.GetSize(); ++nStuIdx)
				{
					nHash = NameHashCalc(StudentNameAry[nStuIdx], strlen(StudentNameAry[nStuIdx]));
					// 通过课程名, 删除g_stCourseInfoTree上的学生选课链表结点
					fOk = g_stStudentInfoTree.DeleteCoursePointRecFromStudentInfoTree(
						StudentNameAry[nStuIdx], nHash, pstrCoName);
				}
			}
		}
	}
	if (fOk)
	{
		ClearScreenAndOutput("删除课程成功!");
	}
	else
	{
		ClearScreenAndOutput("删除失败, 该课程不存在!");
	}
}


void Control::DelRecord()
{
	UINT uiStudentID = 0;
	UINT uiCourseID = 0;
	pstStuInfo pstStudentInfo = nullptr;
	pstCourse pstCourseInfo = nullptr;
	BOOLEAN fOk = FALSE;

	// 获取同学ID值
	do
	{
		cout << "输入学生ID: ";
		scanf("%u", &uiStudentID);
		ClearBuffer();
		// 确定该ID是否存在, ID不存在需要重新输入
		pstStudentInfo = g_stStudentInfoTree.FindNodeDataPtrByID(uiStudentID);
		if (!pstStudentInfo)
		{
			ClearScreenAndOutput("学生不存在, 再次请输入学生ID");
		}
	} while (!pstStudentInfo);
	// 获取选则课程的ID值
	do
	{
		cout << "输入所选课程ID: ";
		scanf("%u", &uiCourseID);
		ClearBuffer();
		// 确定该ID是否存在, ID存在需要重新输入
		pstCourseInfo = g_stCourseInfoTree.FindNodeDataPtrByID(uiCourseID);
		if (!pstCourseInfo)
		{
			ClearScreenAndOutput("课程不存在, 再次请输入课程ID");
		}
	} while (!pstCourseInfo);

	//
	// 到此课程ID和学生ID以及相关信息都已经获取
	// 但此时只能确定这个课程和学生是存在的，至于
	// 学生有没有选择这门课是不确定的
	//

	// 如果该选课记录存在则先删除学生信息链表内的选课记录, 如果不存在则返回false
	fOk = pstStudentInfo->lstOfCourse.DeleteCourseRecordFromStudentTreeCourseList(pstCourseInfo->strCourseName);
	if (fOk)
	{
		// 接着删除课程信息链表内的学生记录
		fOk = pstCourseInfo->lstOfStu.DeleteStudentRecordFromCourseTreeStudentList(pstStudentInfo->strName);
		if (fOk)
		{
			ClearScreenAndOutput("成功删除选课记录");
			return;
		}
	}
	ClearScreenAndOutput("错误, 没有该选课记录");
}

void Control::QueryStuNameByID()
{
	UINT uiID = 0;
	stStuInfo stInfo;
	BOOLEAN fOk = FALSE;

	cout << "输入学生ID: ";
	scanf("%u", &uiID);
	ClearBuffer();
	// 通过ID号查询AVL树
	fOk = g_stStudentInfoTree.FindNodeByID(uiID, stInfo);
	if (!fOk)
	{
		ClearScreenAndOutput("未查到该学生信息, 请确认ID号是否有误");
	}
	else
	{
		char szBuf[LINEMAX] = { 0 };
		sprintf(szBuf, "学生信息: \r\nID: %u\r\n姓名: %s\r\n", uiID, stInfo.strName);
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
		cout << "名字: ";
		pszTmpPtr = GetsEx(szNameBuf, LINEMAX);
		if (!pszTmpPtr)
		{
			ClearScreenAndOutput("输入名字失败, 请再次尝试!");
			memset(szNameBuf, 0, sizeof(szNameBuf));
		}
	} while (!pszTmpPtr);
	// 计算名称的HASH值
	nHash = NameHashCalc(szNameBuf, strlen(szNameBuf));
	// 通过HASH值查找对应学生信息
	fOk = g_stSrhStudentNameTree.FindNodeByID(nHash, stSrhStuName);
	// 通过学生名查询ID号
	if (fOk)
	{
		CArray<size_t> cIDAry;
		// 该hash下有多个对应的学生, 通过学生姓名进一步确认
		fOk = stSrhStuName.lstStudentName.FindStuId(szNameBuf, cIDAry);
		if (fOk)
		{
			char szBuf[LINEMAX] = { 0 };

			cout << "学生信息: " << endl;
			for (size_t nIdx = 0; nIdx < cIDAry.GetSize(); ++nIdx)
			{
				sprintf(szBuf, "学生ID: %u\r\n学生名: %s\r\n", cIDAry[nIdx], szNameBuf);
				cout << szBuf << endl;
			}
			PAUSEANDCLS();
			return;
		}
	}

	ClearScreenAndOutput("未查到该学生信息, 请确认学生姓名是否有误");
}

void Control::QueryCoNameByID()
{
	UINT uiID = 0;
	stCourse stInfo;
	BOOLEAN fOk = FALSE;

	cout << "输入课程ID: ";
	scanf("%u", &uiID);
	ClearBuffer();
	// 通过ID号查询AVL树
	fOk = g_stCourseInfoTree.FindNodeByID(uiID, stInfo);
	if (!fOk)
	{
		ClearScreenAndOutput("未查到该课程信息, 请确认ID号是否有误");
	}
	else
	{
		char szBuf[LINEMAX] = { 0 };
		sprintf(szBuf, "课程信息: \r\nID: %u\r\n课程名: %s\r\n", stInfo.uiCourseID, stInfo.strCourseName);
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
		cout << "名字: ";
		pszTmpPtr = GetsEx(szNameBuf, LINEMAX);
		if (!pszTmpPtr)
		{
			ClearScreenAndOutput("输入名字失败, 请再次尝试!");
			memset(szNameBuf, 0, sizeof(szNameBuf));
		}
	} while (!pszTmpPtr);
	// 计算名称的HASH值
	nHash = NameHashCalc(szNameBuf, strlen(szNameBuf));
	// 通过HASH值查找对应课程信息
	fOk = g_stSrhCourseNameTree.FindNodeByID(nHash, stSrhCoName);
	// 通过课程名查询ID号
	if (fOk)
	{
		// 该hash下有多个对应的课程, 通过课程名进一步确认
		fOk = stSrhCoName.lstCourseName.FindCoId(szNameBuf, nID);
		if (fOk)
		{
			char szBuf[LINEMAX] = { 0 };
			sprintf(szBuf, "课程信息:\r\n课程ID: %u\r\n课程名: %s\r\n", nID, szNameBuf);
			ClearScreenAndOutput(szBuf, FALSE);
			return;
		}
	}

	ClearScreenAndOutput("未查到该课程信息, 请确认课程名是否有误");
}

void Control::QueryAllStuAndPointsByID()
{
	UINT uiID = 0;
	stCourse stInfo;
	BOOLEAN fOk = FALSE;

	cout << "输入课程ID: ";
	scanf("%u", &uiID);
	ClearBuffer();
	// 通过ID号查询AVL树
	fOk = g_stCourseInfoTree.FindNodeByID(uiID, stInfo);
	if (!fOk)
	{
		ClearScreenAndOutput("未查到该课程信息, 请确认ID号是否有误");
	}
	else
	{
		char szBuf[LINEMAX] = { 0 };
		sprintf(szBuf, "课程信息: \r\nID: %u\r\n课程名: %s\r\n", uiID, stInfo.strCourseName);
		cout << szBuf;
		printf("学生ID\t学生名\t成绩\r\n");
		stInfo.lstOfStu.TraverseList();
		PAUSEANDCLS();
	}
}

void Control::QueryCoInfoByID()
{
	UINT uiID = 0;
	stStuInfo stInfo;
	BOOLEAN fOk = FALSE;

	cout << "输入学生ID: ";
	scanf("%u", &uiID);
	ClearBuffer();
	// 通过ID号查询AVL树
	fOk = g_stStudentInfoTree.FindNodeByID(uiID, stInfo);
	if (!fOk)
	{
		ClearScreenAndOutput("未查到该学生信息, 请确认ID号是否有误");
	}
	else
	{
		char szBuf[LINEMAX] = { 0 };
		sprintf(szBuf, "学生信息: \r\nID: %u\r\n姓名: %s\r\n", uiID, stInfo.strName);
		cout << szBuf;
		stInfo.lstOfCourse.TraverseList();
		PAUSEANDCLS();
	}
}
