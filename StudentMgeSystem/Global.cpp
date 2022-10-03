#include "Global.h"
#include "CAVLTree.hpp"

CAVLTree<stStuInfo> g_stStudentInfoTree;

CAVLTree<stCourse> g_stCourseInfoTree;

CAVLTree<stSearchIDByCourseName> g_stSrhCourseNameTree;

CAVLTree<stSearchIDByStudentName> g_stSrhStudentNameTree;

ostream &operator<<(ostream &os, const tagStudentInfo &stInfo)
{
	os << "ѧ����: " << stInfo.strName << endl;
	os << "ID: " << stInfo.uiID << endl;

	os << endl;

	return(os);
}

ostream &operator<<(ostream &os, const tagCourse &stInfo)
{
	os << "�γ���: " << stInfo.strCourseName << endl;
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
	cout << "ѧ��ID: " << stInfo.uiStudentID << endl;
	cout << "ѧ����: " << stInfo.strStudentName << endl;

	return(os);
}

ostream &operator<<(ostream &os, const tagStudentCourse &stInfo)
{
	cout << "�γ���: " << stInfo.strCourseName << endl;
	cout << "�ɼ�: " << stInfo.uiPoint << endl;

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
