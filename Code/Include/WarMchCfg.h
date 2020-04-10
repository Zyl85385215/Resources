#pragma once

#include "singleton.h"
#include "xnlist.h"
#include "xnExtstr.h"

class WMClientInfo
{
public:
	WMClientInfo();
	~WMClientInfo();
	BYTE	bLv;
	DWORD	dwExp;
	int		nSPPoint;
	xnList*	pList;
	xnList* pLUsed;
	DWORD	dwCurHp;
	DWORD	dwDure;
};


class WMCfgItem
{
public:
	DWORD	dwItemID;
	WORD	wNum;
};

class WMCfgLvObj
{
public:
	WMCfgLvObj();
	~WMCfgLvObj();
	BYTE	bSkin;
	DWORD	dwItemID;
	DWORD	dwHP;
	DWORD	dwSpeed;
	WORD	wState;
	DWORD	dwAtk;
	DWORD	dwDef;
	DWORD	dwNeedExp;
	DWORD	dwNeedGold;
	DWORD	dwDure;
	xnList*	pListItem;
};

class CfgWarMachine	: public CSingleton<CfgWarMachine>
{
public:
	CfgWarMachine();
	~CfgWarMachine();

	void			LoadFile(char*	szFile);
	WMCfgLvObj*		GetWMCObj(BYTE bLv);
	WORD			GetAtkSkill(BYTE bPos);
	WORD			GetDefSkill(BYTE bPos);
	bool			CheckCanLearn(WORD	wSkill);
	WMCfgLvObj*		m_ppWMLvl[37];
	xnList*			m_pLSkillAtk;
	xnList*			m_pLSkillDef;

};