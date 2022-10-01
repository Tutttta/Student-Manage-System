#pragma once
#include "Global.h"
#include "UI.h"
#include "IO.h"

class Control
{
public:
	Control() = default;

	void Run();

	BOOLEAN InitSystem();
	
	BOOLEAN ChkInitFileExists(PSTR pszFilePathName, size_t nSize, PCSTR pcszFilePathName = STUDENTFILENAME);
private:
	UI m_UI;
	IO m_IO;
};

