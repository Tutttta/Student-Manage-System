#pragma once
#include "Global.h"
// 学生信息动态数组专用快速排序
void QuickSortStuAry(CArray<stSearchIDByStudentName> &Ary, int left, int right);
int PartSortStuAry(CArray<stSearchIDByStudentName> &Ary, int left, int right);

// 课程信息动态数组专用快速排序
void QuickSortCoAry(CArray<stSearchIDByCourseName> &Ary, int left, int right);
int PartSortCoAry(CArray<stSearchIDByCourseName> &Ary, int left, int right);
