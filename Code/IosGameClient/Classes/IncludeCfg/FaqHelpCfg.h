#pragma once
#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif
#include <vector>
#include <string>

class FaqHelpMsg
{
public:
	char*	szFaqName;
	char*	szFaqDesc;
};

class FaqHelpObj
{
public:
	FaqHelpObj();
	~FaqHelpObj();

	int		nFaqID;
	char*	szName;
	int		nFaqCount;

	xnList* pFaqHelpMsgList;
};

class FaqHelpCfg : public CSingleton<FaqHelpCfg>
{
public:
	FaqHelpCfg();
	~FaqHelpCfg();

	void	LoadFile(char* pFileName);
	xnList* m_pList;
};
