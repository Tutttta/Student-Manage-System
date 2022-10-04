#ifndef _GLOBAL_332D42FF_245E_49F2_894D_289A86388F92_H
#define _GLOBAL_332D42FF_245E_49F2_894D_289A86388F92_H
// ���ڶ���ȫ�ֱ���, �궨��, ȫ�ֺ�����
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <conio.h>

#include "MD5.h"
#include "CList.hpp"
#include "CMyString.h"
#include "CArray.hpp"
#include "CStack.hpp"
#include "CQueue.hpp"
#include "CAVLTree.hpp"

// �������͹ؼ��ֺ궨��
#define UINT unsigned int 
#define ULONGLONG unsigned long long 
#define LONGLONG long long 
#define BOOLEAN bool
#define INT int
#define FALSE false 
#define TRUE true
#define PSTR char *
#define PPSTR char **
#define PCSTR const char *

// һ�������ַ���
#define LINEMAX 256

// �γ������ļ���
#define COURSEFILENAME "courses.txt"
// ѧ�����������ļ���
#define STUDENTFILENAME "names.txt"
// g_stSrhCourseNameTree������ļ���
#define SRHCOURSEFILENAME "srhcourse.dat"
// g_stSrhStudentNameTree������ļ���
#define SRHSTUDENTFILENAME "srhstudent.dat"
// g_stStudentInfoTree������ļ���
#define SAVESTUDENTFILENAME "student.dat"
// g_stCourseInfoTree������ļ���
#define SAVECOURSEFILENAME "course.dat"

// ��ͣ�������õĺ�����
#define PAUSEANDCLS() system("pause"); \
						system("cls");
#define CLSANDPAUSE() system("cls"); \
						system("pause");

using namespace std;

// �˵�������
typedef enum _eMenuType
{
	MAIN,		// ���˵�
	INSERT,		// �����Ӳ˵�
	QUERY,		// ��ѯ�Ӳ˵�
	STATISTIC,	// ͳ���Ӳ˵�
	DELETE		// ɾ���Ӳ˵�
} eMenuType;


// ѧ����Ϣ�����ڼ�¼��ѡ�޿γ̵Ľṹ��
typedef struct tagStudentCourse
{
	friend ostream &operator<<(ostream &os, const tagStudentCourse &stInfo);
	size_t nCourseID;			// �γ�ID
	UINT uiPoint;				// �γ̶�Ӧ�ķ���
	CMyString strCourseName;	// �γ�����
} StudentCourse, *pStudentCourse;


// һ��BST���洢 ID��Ϊ���ֵ, �洢tagStudentInfo�ṹ��
// ѧ��ID => ѧ������ ��
// ѧ��ID => ѧ��ѡ�޵Ŀγ���Ϣ ��
// ѧ������ => ѧ��ID ��
typedef struct tagStudentInfo
{
	friend ostream &operator<<(ostream &os, const tagStudentInfo &stInfo);
	UINT uiID;							// ѧ��ID
	CMyString strName;					// ѧ������
	CList<StudentCourse> lstOfCourse;	// ѧ���γ̺ͳɼ�����
} stStuInfo, *pstStuInfo; 


// ͨ��ѧ������ѯID
typedef struct _stStudentNameAndID
{
	friend ostream &operator<<(ostream &os, const _stStudentNameAndID &stInfo);
	UINT uiStudentID;					// ѧ��ID
	CMyString strStudentName;			// ѧ����
} stStudentNameAndID, *pstStudentNameAndID;


// ����ѧ������HASHֵ�����ظ�, ���԰��ظ�����һ����������
typedef struct tagSearchIDByStudentName
{
	friend ostream &operator<<(ostream &os, const tagSearchIDByStudentName &stInfo);
	size_t nNameHash;								// ѧ������hashֵ
	CList<stStudentNameAndID> lstStudentName;		// ѧ�����Ͷ�Ӧ��ѧ��ID����							// ѧ������hashֵ
} stSearchIDByStudentName, *pstSearchIDByStudentName;


// һ��BST���洢 ID��Ϊ���ֵ, �洢tagCource�ṹ��
// ID => �γ��� ��
// �γ�ID => ѡ�޿γ̵�ѧ�����ɼ� ��
// �γ��� => ID ��
typedef struct tagStuChosenCourse
{
	friend ostream &operator<<(ostream &os, const tagStuChosenCourse &stInfo);
	size_t nStuID;			// ѧ��ID
	UINT uiPoint;			// ѧ���ĳɼ�
	CMyString strName;		// ѧ������
} StuChosenCourse, *pstStuChosenCourse;


// �γ���Ϣ�ṹ��, ��ᱻ���뵽AVL��������
typedef struct tagCourse
{
	friend ostream &operator<<(ostream &os, const tagCourse &stInfo);
	UINT uiCourseID;					// �γ�ID
	CMyString strCourseName;			// �γ���
	CList<StuChosenCourse> lstOfStu;	// ѡ��ÿγ̵�ѧ���ɼ�����
} stCourse, *pstCourse;


// ͨ���γ�����ѯID
typedef struct _stCourseNameAndID
{
	CMyString strCourseName;			// �γ���
	UINT uiCourseID;					// �γ�ID
} stCourseNameAndID, *pstCourseNameAndID;


// ���ڿγ�����HASHֵ�����ظ�, ���԰��ظ�����һ����������
typedef struct tagSearchIDByCourseName
{
	size_t nNameHash;							// �γ�����hashֵ
	CList<stCourseNameAndID> lstCourseName;		// �γ����Ͷ�Ӧ�Ŀγ�ID����
} stSearchIDByCourseName, *pstSearchIDByCourseName;

// ѧ����Ϣ��ƽ�����������
extern CAVLTree<stStuInfo> g_stStudentInfoTree;

// �γ���Ϣ��ƽ�����������
extern CAVLTree<stCourse> g_stCourseInfoTree;

// �γ���Ϣ��ƽ�����������, ����ͨ���γ����Ʋ�ѯID����Ϣ
extern CAVLTree<stSearchIDByCourseName> g_stSrhCourseNameTree;

// ѧ����Ϣ��ƽ�����������, ����ͨ��ѧ������ѯID����Ϣ
extern CAVLTree<stSearchIDByStudentName> g_stSrhStudentNameTree;

#endif 