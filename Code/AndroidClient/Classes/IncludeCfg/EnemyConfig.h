#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif
#include <map>

class PathObj;
struct EnemyBase
{
	EnemyBase()		{pLSkill	= xnList::Create();}
	~EnemyBase();

	int		nID;								//ID
	WORD	wSkin;
	WORD	wSkinEx;
	BYTE	bLvl;
	char*	szName;

	BYTE	bAtkType;			//不动,被动怪,主动怪
	BYTE	bGroup;

	BYTE	bType;				//普通怪,精英怪,BOSS怪,采集怪,技能怪A(可被攻击),技能怪B(不可)
	BYTE	bNoticeGS;
	int		nMaxHp;
	int		nAtkW;
	int		nAtkN;
	int		nDefW;
	int		nDefN;
	int		nAim;
	int		nDodge;
	int		nCrit;
	int		nSpeed;

	WORD	wAtkRange;
	WORD	wFindRange;

	xnList*	pLSkill;
	PathObj*	pPath;
};

class EnemyBaseCfg : public CSingleton<EnemyBaseCfg>
{
public:
	EnemyBaseCfg();
	~EnemyBaseCfg();

	void	LoadFile(char * pFileName);
	EnemyBase* GetEnemyBase(int nID);

private:
	std::map<WORD,EnemyBase*>	mapEnemyBase;
};