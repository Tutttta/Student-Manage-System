#ifndef _GLOBAL_332D42FF_245E_49F2_894D_289A86388F92_H
#define _GLOBAL_332D42FF_245E_49F2_894D_289A86388F92_H
// 用于定义全局变量, 宏定义, 全局函数等
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "MD5.h"
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

// 一行最大的字符数
#define LINEMAX 256

#define COURSEFILENAME "courses.txt"
#define STUDENTFILENAME "names.txt"

#define PAUSEANDCLS() system("pause"); \
						system("cls");
#define CLSANDPAUSE() system("cls"); \
						system("pause");

using namespace std;

// 菜单的类型
typedef enum _eMenuType
{
	MAIN,		// 主菜单
	INSERT,		// 插入子菜单
	QUERY,		// 查询子菜单
	STATISTIC,	// 统计子菜单
	DELETE		// 删除子菜单
} eMenuType;


typedef struct tagStudentCourse
{
	CMyString strCourseName;	// 课程名字
	UINT uiPoint;				// 课程对应的分数
} StudentCourse, *pStudentCourse;

// 一棵BST树存储 ID作为结点值, 存储tagStudentInfo结构体
// 学生ID => 学生姓名 √
// 学生ID => 学生选修的课程信息 √
// 学生姓名 => 学生ID √
typedef struct tagStudentInfo
{
	friend ostream &operator<<(ostream &os, const tagStudentInfo &stInfo);
	UINT uiID;							// 学生ID
	CMyString strName;					// 学生姓名
	CList<StudentCourse> lstOfCourse;	// 学生课程和成绩链表
} stStuInfo, *pstStuInfo; 


// 通过学生名查询ID
typedef struct _stStudentNameAndID
{
	friend ostream &operator<<(ostream &os, const _stStudentNameAndID &stInfo);
	CMyString strStudentName;			// 学生名
	UINT uiStudentID;					// 学生ID
} stStudentNameAndID, *pstStudentNameAndID;
// 由于学生名的HASH值可能重复, 所以把重复的用一个链表串起来
typedef struct tagSearchIDByStudentName
{
	friend ostream &operator<<(ostream &os, const tagSearchIDByStudentName &stInfo);
	size_t nNameHash;								// 学生名的hash值
	CList<stStudentNameAndID> lstStudentName;		// 学生名和对应的学生ID链表							// 学生名的hash值
} stSearchIDByStudentName, *pstSearchIDByStudentName;


// 一棵BST树存储 ID作为结点值, 存储tagCource结构体
// ID => 课程名 √
// 课程ID => 选修课程的学生及成绩 √
// 课程名 => ID √
typedef struct tagStuChosenCourse
{
	CMyString strName;		// 学生姓名
	UINT uiPoint;			// 学生的成绩
} StuChosenCourse, *pstStuChosenCourse;
typedef struct tagCourse
{
	friend ostream &operator<<(ostream &os, const tagCourse &stInfo);
	UINT uiCourseID;					// 课程ID
	CMyString strCourseName;			// 课程名
	CList<StuChosenCourse> lstOfStu;	// 选择该课程的学生成绩链表
} stCourse, *pstCourse;


// 通过课程名查询ID
typedef struct _stCourseNameAndID
{
	CMyString strCourseName;			// 课程名
	UINT uiCourseID;					// 课程ID
} stCourseNameAndID, *pstCourseNameAndID;
// 由于课程名的HASH值可能重复, 所以把重复的用一个链表串起来
typedef struct tagSearchIDByCourseName
{
	size_t nNameHash;							// 课程名的hash值
	CList<stCourseNameAndID> lstCourseName;		// 课程名和对应的课程ID链表
} stSearchIDByCourseName, *pstSearchIDByCourseName;

// 学生信息的平衡二叉搜索树
extern CAVLTree<stStuInfo> g_stStudentInfoTree;

// 课程信息的平衡二叉搜索树
extern CAVLTree<stCourse> g_stCourseInfoTree;

// 课程信息的平衡二叉搜索树, 管理通过课程名称查询ID等信息
extern CAVLTree<stSearchIDByCourseName> g_stSrhCourseNameTree;

// 学生信息的平衡二叉搜索树, 管理通过学生名查询ID等信息
extern CAVLTree<stSearchIDByStudentName> g_stSrhStudentNameTree;


stSearchIDByStudentName *BinSrhStuName(pstSearchIDByStudentName pstSrhStuInfo, size_t nLen, size_t nObjHash);

// 清理缓存
void ClearBuffer();
#endif 