#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif
#include <map>
enum
{
	EQUIP_AD_UIN,
	EQUIP_AD_ID,			//唯一标示
	EQUIP_AD_TYPE,		//进阶类型PHASE
	EQUIP_AD_SLOT,
	EQUIP_AD_LV,
	EQUIP_AD_MAX,
};

class EquipBase
{
public:
	EquipBase();
	~EquipBase();
	
	int		nTypeID;
	char*	szName;
	char*	szDesc;
	int		nSpr;
	int		nStar;
	BYTE	bSlot;
	int		nLevel;
	int		nNext;
	int		nSellGold;
};

class EquipConfig	: public CSingleton<EquipConfig>
{
public:
	EquipConfig();
	~EquipConfig();

	void		LoadFile(char*	szFile);
	EquipBase*	GetEquipBase(int nType);

private:
	xnList*		pListEquip;
	std::map<int ,std::map<int,xnList*> > mapEquip[3];
};