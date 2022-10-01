#ifndef _GLOBAL_332D42FF_245E_49F2_894D_289A86388F92_H
#define _GLOBAL_332D42FF_245E_49F2_894D_289A86388F92_H
// ���ڶ���ȫ�ֱ���, �궨��, ȫ�ֺ�����
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "CList.hpp"
#include "CMyString.h"
#include "CArray.hpp"
#include "CStack.hpp"
#include "CQueue.hpp"
#include "CAVLTree.hpp"

//#include "MockingString.h"

#define UINT unsigned int 
#define ULONGLONG unsigned long long 
#define LONGLONG long long 
#define INT int
#define BOOLEAN bool
#define FALSE false 
#define TRUE true
#define PSTR char *
#define PPSTR char **
#define PCSTR const char *

// һ�������ַ���
#define LINEMAX 256

#define COURSEFILENAME "courses.txt"
#define STUDENTFILENAME "names.txt"

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

typedef struct tagStudentCourse
{
	CMyString strCourseName;	// �γ�����
	UINT uiPoint;				// �γ̶�Ӧ�ķ���
} StudentCourse, *pStudentCourse;

typedef struct tagStuChosenCourse
{
	CMyString strName;		// ѧ������
	UINT uiPoint;			// ѧ���ĳɼ�
} StuChosenCourse, *pstStuChosenCourse;

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

// ͨ��ѧ������ѯID
typedef struct tagSearchIDByStudentName
{
	CMyString strStudentName;			// ѧ����
	UINT uiStudentID;					// ѧ��ID
} stSearchIDByStudentName, *pstSearchIDByStudentName;

// һ��BST���洢 ID��Ϊ���ֵ, �洢tagCource�ṹ��
// ID => �γ��� ��
// �γ�ID => ѡ�޿γ̵�ѧ�����ɼ� ��
// �γ��� => ID ��
typedef struct tagCourse
{
	UINT uiCourseID;					// �γ�ID
	CMyString strCourseName;			// �γ���
	CList<StuChosenCourse> lstOfStu;	// ѡ��ÿγ̵�ѧ���ɼ�����
} stCourse, *pstCourse;

// ͨ��ѧ������ѯID
typedef struct tagSearchIDByCourseName
{
	CMyString strCourseName;			// �γ���
	UINT uiCourseID;					// �γ�ID
} stSearchIDByCourseName, *pstSearchIDByCourseName;

// ѧ����Ϣ��ƽ�����������
extern CAVLTree<stStuInfo> g_stStudentInfoTree;

// �γ���Ϣ��ƽ�����������
extern CAVLTree<stCourse> g_stCourseInfoTree;

// �γ���Ϣ�Ķ�̬����
extern CArray<stSearchIDByCourseName> g_stAryOfCourseName;

// ѧ����Ϣ�Ķ�̬����
extern CArray<stSearchIDByStudentName> g_stAryOfStudentName;

#endif 