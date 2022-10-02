#include "Sort.h"


int PartSortStuAry(CArray<stSearchIDByStudentName> &Ary, int left, int right)
{
	size_t key = Ary[left].nNameHash;
	while (left < right) {
		while (left < right && Ary[right].nNameHash >= key)
		{
			right--;
		}
		if (left != right) {
			stSearchIDByStudentName temp(Ary[left]);
			Ary[left] = Ary[right];
			Ary[right] = temp;
		}
		while (left < right && Ary[left].nNameHash <= key)
		{
			left++;
		}
		if (left != right) {
			stSearchIDByStudentName temp = Ary[left];
			Ary[left] = Ary[right];
			Ary[right] = temp;
		}
	}

	return(left);
}

void QuickSortStuAry(CArray<stSearchIDByStudentName> &Ary, int left, int right) {
	int iFixedPos = 0;

	if (left < right)
	{
		iFixedPos = PartSortStuAry(Ary, left, right);
		QuickSortStuAry(Ary, left, iFixedPos - 1);
		QuickSortStuAry(Ary, iFixedPos + 1, right);
	}
}


void QuickSortCoAry(CArray<stSearchIDByCourseName> &Ary, int left, int right)
{
	CStack<int> st;

	st.Push(left);
	st.Push(right - 1);
	while (!st.IsEmpty()) {
		int _end = st.GetTop();
		st.Pop();
		int _begin = st.GetTop();
		st.Pop();
		int div = PartSortCoAry(Ary, _begin, _end + 1);
		if (_begin < div) {
			st.Push(_begin);
			st.Push(div);
		}
		if (div + 1 < _end) {
			st.Push(div + 1);
			st.Push(_end);
		}
	}
}

int PartSortCoAry(CArray<stSearchIDByCourseName> &Ary, int left, int right)
{
	int begin = left;
	int end = right - 1;
	stSearchIDByCourseName temp;
	size_t key = Ary[left].nNameHash;
	while (begin < end) {
		while (begin < end && Ary[end].nNameHash >= key)
		{
			end--;
		}
		while (begin < end && Ary[begin].nNameHash <= key)
		{
			begin++;
		}
		if (begin != end) {
			temp = Ary[begin];
			Ary[begin] = Ary[end];
			Ary[end] = temp;
		}
	}
	temp = Ary[begin];
	Ary[begin] = Ary[left];
	Ary[left] = temp;

	return(begin);//返回关键字的下标
}