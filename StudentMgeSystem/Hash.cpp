#include "Hash.h"
#include "MD5.h"

size_t NameHashCalc(const char *pcszName, size_t nSize)
{
	size_t nHash = 0;
	MD5_CTX Md5;
	unsigned char szMd5[32] = { 0 };

	if (!pcszName || !nSize)
	{
		return(0);
	}
	MD5Init(&Md5);
	// 计算md5值
	MD5Update(&Md5, (unsigned char *)pcszName, nSize);
	MD5Final(&Md5, (unsigned char *)szMd5);

	// 计算md5算出来的Hash值
	for (size_t nIdx = 0; nIdx < 16; ++nIdx)
	{
		nHash += szMd5[nIdx];
	}

	return(nHash);
}