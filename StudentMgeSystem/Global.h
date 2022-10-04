#ifndef _GLOBAL_332D42FF_245E_49F2_894D_289A86388F92_H
#define _GLOBAL_332D42FF_245E_49F2_894D_289A86388F92_H
// 用于定义全局变量, 宏定义, 全局函数等
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

// 常用类型关键字宏定义
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

// 一行最大的字符数
#define LINEMAX 256

// 课程数据文件名
#define COURSEFILENAME "courses.txt"
// 学生姓名数据文件名
#define STUDENTFILENAME "names.txt"
// g_stSrhCourseNameTree保存的文件名
#define SRHCOURSEFILENAME "srhcourse.dat"
// g_stSrhStudentNameTree保存的文件名
#define SRHSTUDENTFILENAME "srhstudent.dat"
// g_stStudentInfoTree保存的文件名
#define SAVESTUDENTFILENAME "student.dat"
// g_stCourseInfoTree保存的文件名
#define SAVECOURSEFILENAME "course.dat"

// 暂停和清屏用的函数宏
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


// 学生信息中用于记录其选修课程的结构体
typedef struct tagStudentCourse
{
	friend ostream &operator<<(ostream &os, const tagStudentCourse &stInfo);
	size_t nCourseID;			// 课程ID
	UINT uiPoint;				// 课程对应的分数
	CMyString strCourseName;	// 课程名字
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
	UINT uiStudentID;					// 学生ID
	CMyString strStudentName;			// 学生名
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
	friend ostream &operator<<(ostream &os, const tagStuChosenCourse &stInfo);
	size_t nStuID;			// 学生ID
	UINT uiPoint;			// 学生的成绩
	CMyString strName;		// 学生姓名
} StuChosenCourse, *pstStuChosenCourse;


// 课程信息结构体, 其会被插入到AVL搜索树中
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

#endif 