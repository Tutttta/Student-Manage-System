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

ostream &operator<<(ostream &os, const tagStudentCourse &stInfo)
{
	cout << "课程名: " << stInfo.strCourseName << endl;
	cout << "成绩: " << stInfo.uiPoint << endl;

	return(os);
}

ostream &operator<<(ostream &os, const tagStuChosenCourse &stInfo)
{
	if (stInfo.nStuID < 1000)
	{
		printf("%03u\t%s\t", stInfo.nStuID, stInfo.strName);
	}
	else
	{
		printf("%u\t%s\t", stInfo.nStuID, stInfo.strName);
	}
	cout << stInfo.uiPoint << endl;

	return(os);
}
