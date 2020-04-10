#pragma once

#include <windows.h>
#include "singleton.h"
#include "xnlist.h"

#define SAFE_DELETE(p) { if(p) { delete (p); (p) = 0; } }

struct TransportMapData
{
	WORD	m_wMapIndex;
	BYTE	m_bFlagCount;
	int		m_nMoney;
};

class TransportMapConfig : public CSingleton<TransportMapConfig>
{
public:
	TransportMapConfig(void);
	~TransportMapConfig(void);

	bool		Load(const char * filename);
	int			GetFlagCost(int nMapIndex);
	int			GetMoneyCost(int nMapIndex);

private:
	xnList *	m_pList;
};

class HSCfgObj
{
public:
	BYTE	bType;
	BYTE	bLevel;
	WORD	wSkill;		//客户端用的
	WORD	wState;
	int		nNeedHonor;
	int		nNeedMoney;
	DWORD		nNeedExp;
};

class HomeStatesConfig : public CSingleton<HomeStatesConfig>
{
public:
	HomeStatesConfig();
	~HomeStatesConfig();

	void	Load(const char * filename);
	HSCfgObj*	GetHSInfo(BYTE bType, BYTE bLv);
	
private:
	xnList*		m_pList;
};