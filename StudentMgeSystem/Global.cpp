#include "Global.h"
#include "CAVLTree.hpp"

CAVLTree<stStuInfo> g_stStudentInfoTree;

CAVLTree<stCourse> g_stCourseInfoTree;

CAVLTree<stSearchIDByCourseName> g_stSrhCourseNameTree;

CAVLTree<stSearchIDByStudentName> g_stSrhStudentNameTree;

ostream &operator<<(ostream &os, const tagStudentInfo &stInfo)
{
	os << "学生名: " << stInfo.strName << endl;
	os << "ID: " << stInfo.uiID << endl;

	os << endl;

	return(os);
}

ostream &operator<<(ostream &os, const tagCourse &stInfo)
{
	os << "课程名: " << stInfo.strCourseName << endl;
	os << "ID: " << stInfo.uiCourseID << endl;

	os << endl;

	return(os);
}

ostream &operator<<(ostream &os, const tagSearchIDByStudentName &stInfo)
{
	os << "hash: " << stInfo.nNameHash << endl;
	stInfo.lstStudentName.TraverseList();
	os << endl;

	return(os);
}

ostream &operator<<(ostream &os, const _stStudentNameAndID &stInfo)
{
	cout << "学生ID: " << stInfo.uiStudentID << endl;
	cout << "学生名: " << stInfo.strStudentName << endl;

	return(os);
}

stSearchIDByStudentName *BinSrhStuName(pstSearchIDByStudentName pstSrhStuInfo, 
	size_t nLen, size_t nObjHash)
{
	size_t nFirst = 0;
	size_t nLast = nLen - 1;
	size_t nMid = 0;
	size_t nAryHash = 0;

	if (!nObjHash || !nLen || !pstSrhStuInfo)
	{
		return(nullptr);
	}
	while (nFirst < nLast)
	{
		nMid = nFirst + (nLast - nFirst) / 2;
		nAryHash = pstSrhStuInfo[nMid].nNameHash;
		if (nAryHash < nObjHash)
		{
			nFirst = nMid + 1;
		}
		else if (nAryHash > nObjHash)
		{
			nLast = nMid - 1;
		}
		else
		{
			return(&pstSrhStuInfo[nMid]);
		}
	}
	
	return(nullptr);
}

void ClearBuffer()
{
	while (getchar() != '\n');
}

