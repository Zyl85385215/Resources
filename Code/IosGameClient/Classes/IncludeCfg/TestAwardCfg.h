#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif

struct AwardObj
{
	int nMoney;
	int nExp;
	xnList* pItemList;
};

class TestAwardCfg: public CSingleton<TestAwardCfg>
{
public:
	TestAwardCfg();
	~TestAwardCfg();
	void LoadFile(char* pFile);
	AwardObj* GetAwardByStar(int nStar);
	AwardObj* GetAwardByRank(int nPlace);

	int	m_byStartH;
	int	m_byStartM;
	int m_byEndH;
	int m_byEndM;
	int m_nACount;
	xnList* m_pTestAward;
	xnList* m_pRankAward;
};