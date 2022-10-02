#include "Control.h"
#include "MockingString.h"
#include <conio.h>



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

# if 0
	for (size_t nIdx = 0; nIdx < g_stAryOfStudentName.GetSize(); ++nIdx)
	{
		cout << "学生名: " << g_stAryOfStudentName[nIdx].strStudentName << endl;
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

#if 0
	for (size_t nIdx = 0; nIdx < g_stAryOfCourseName.GetSize(); ++nIdx)
	{
		cout << "课程名: " << g_stAryOfCourseName[nIdx].strCourseName << endl;
		cout << "ID: " << g_stAryOfCourseName[nIdx].uiCourseID << endl;
		cout << "Hash: " << g_stAryOfCourseName[nIdx].nNameHash << endl;
		cout << endl;
	}
#endif
	
	return(fOk);
}

/*
// 一棵BST树存储 ID作为结点值, 存储tagStudentInfo结构体
// 学生ID => 学生姓名 √
// 学生ID => 学生选修的课程信息 √
// 学生姓名 => 学生ID √
typedef struct tagStudentInfo
{
	UINT uiID;							// 学生ID
	CMyString strName;					// 学生姓名
	CList<StudentCourse> lstOfCourse;	// 学生课程和成绩链表
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
		g_stSrhStudentNameTree.InsertNode(stSrhStuName, nHash);
	}
	else
	{
		// ID发生了重复则找到那个结点直接挂再链表上
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
		// 将学生名和ID插入链表
		stSrhCoName.lstCourseName.InsertTail(stCoNameId);
	}
	else
	{
		// ID发生了重复则找到那个结点直接挂再链表上
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
	puts("插入功能子菜单: ");
	// ID和名字
	puts("1. 新增学生");
	puts("2. 新增课程");
	puts("3. 新增记录");
	printf("命令: ");
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

void Control::Delete()
{
	int iCmd = 0;
	BOOLEAN fOk = TRUE;

	/*
	puts("删除功能子菜单: ");
	// ID
	puts("1. 删除学生");
	puts("2. 删除课程");
	puts("3. 删除选课记录");
	puts("4. 返回上一级");
	printf("命令: ");
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

	/*
	puts("查询功能子菜单: ");
	// ID
	puts("1. 通过学生ID查询学生姓名");
	puts("2. 通过学生姓名查询学生ID");
	puts("3. 通过课程ID查询课程名");
	puts("4. 通过课程名查询课程ID");
	puts("5. 通过课程ID查询选修了该课程的学生以及成绩");
	puts("6. 通过学生ID查询该学生选修的课程信息");
	printf("命令: ");
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
		stInfo.uiID = uiID;
		stInfo.strName = pszNameBuf;
		// 填充stSearchIDByStudentName结构体
		fOk = SetSearchIDByStudentNameStructure(pszNameBuf, uiID);
		{
			if (fOk)
			{
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
		// 获取新同学ID值
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

		// 填充stSearchIDByStudentName结构体
		//SetSearchIDByCourseNameStructure(&stSrhCoInfo, pszNameBuf, uiID);

		//stSrhCoInfo.uiCourseID = uiID;
		//stSrhCoInfo.strCourseName = pszNameBuf;
		//// 把结构体插入动态数组中
		//fOk = g_stAryOfCourseName.InsertTail(stSrhCoInfo);
		if (fOk)
		{
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

}

void Control::DelStudent()
{
	UINT uiID = 0;
	BOOLEAN fOk = FALSE;

	cout << "输入学生ID: ";
	scanf("%u", &uiID);
	ClearBuffer();
	// 删除该节点
	fOk = g_stStudentInfoTree.DeleteNode(uiID);
	if (fOk)
	{
		//fOk = g_stAryOfStudentName.Delete()
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

	cout << "输入课程ID: ";
	scanf("%u", &uiID);
	ClearBuffer();
	// 删除该节点
	fOk = g_stCourseInfoTree.DeleteNode(uiID);
	if (fOk)
	{
		ClearScreenAndOutput("删除课程成功!");
	}
	else
	{
		ClearScreenAndOutput("删除失败, 该课程不存在!");
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
	size_t nID = 0;
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
	stSrhStuName.lstStudentName.TraverseList();
	// 通过学生名查询ID号
	if (!fOk)
	{
		ClearScreenAndOutput("未查到该学生信息, 请确认学生姓名是否有误");
	}
	else
	{
		stSrhStuName.lstStudentName.FindStuId(szNameBuf, nID);
		char szBuf[LINEMAX] = { 0 };
		sprintf(szBuf, "学生信息:\r\n学生ID: %u\r\n学生名: %s\r\n", nID, szNameBuf);
		ClearScreenAndOutput(szBuf, FALSE);
	}
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

}

void Control::QueryAllStuAndPointsByID()
{

}

void Control::QueryCoInfoByID()
{

}
