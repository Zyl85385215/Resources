#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif

struct _mbInfo
{
	int nID;
	int nNum;
	int nPer;
};

struct MentoringBase
{
	BYTE	byLv;
	char	szName[20];			//名字
	int		nNeedval;			//所需声望
	int		nPuplecnt;			//徒弟数量
	xnList* pList;
};

class MentoringCfg	: public CSingleton<MentoringCfg>
{
public:
	MentoringCfg();
	~MentoringCfg();
	void	LoadFile(char* pFile);
	MentoringBase*	GetCurBase(int nNum);
	MentoringBase*	GetNextBase(int nNum);

	xnList* m_pList;
	int	m_nExpaddid;			//组队经验加成ID
	int	m_nDaylyexpcoe;		//每日领取经验系数
	int	m_nDaylygoldcoe;		//每日领取金币系统
	int	m_nDaylypower;		//每日领取体力
	int	m_nDaylyhonour;		//每日领取师德
	int	m_nGraduateexp;		//毕业领取经验
	int	m_nGraduategold;		//毕业领取金币
	int	m_nGraduatehonour;	//毕业领取师德
};

struct MarrySKObj
{
	char*	pName;
	char*	pIcon;
	BYTE	byType;
	xnList* pLvList;
};

struct MarryAward
{
	int nNeed;
	int nExp;
	int nGold;
	int nPoint;
};

class MarrySkillCfg	: public CSingleton<MarrySkillCfg>
{
public:
	MarrySkillCfg();
	~MarrySkillCfg();
	void	LoadFile(char* pFile);

	xnList* m_pList;
	xnList*	m_pAwardList;
};