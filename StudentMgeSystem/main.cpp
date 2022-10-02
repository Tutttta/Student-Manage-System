#include <iostream>
//#include "IO.h"
#include "Control.h"
#include "MD5.h"
//#include "Global.h"
using namespace std;

void Swap(int *left, int *right)
{
	int iTmp = *left;
	*left = *right;
	*right = iTmp;
}

size_t NameHashCalc(PCSTR szMd5)
{
	size_t nHash = 0;


	// 计算md5算出来的Hash值
	for (size_t nIdx = 0; nIdx < 16; ++nIdx)
	{
		nHash += szMd5[nIdx];
	}

	return(nHash);
}

int main()
{
	Control c;

	c.Run();

	return(0);
}