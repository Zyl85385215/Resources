#pragma once

#include "singleton.h"
#include "xnlist.h"
#include "ShipConfig.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif

enum
{					//��Ʒ�Ͳ��϶���������
	ITEM_AD_UIN,
	ITEM_AD_ID,			//Ψһ��ʾ
	ITEM_AD_TYPE,		//��������PHASE
	ITEM_AD_NUM,			//����

	ITEM_AD_MAX,
};

class ItemBase
{
public:
	ItemBase();
	~ItemBase();
	
	int		nTypeID;
	char*	szName;
	char*	szDesc;
	int		nSpr;
	int		nStar;
	bool	bCanUsed;
	
	BYTE	byEffType;
	WORD	wEffVal;
};

class ItemConfig	: public CSingleton<ItemConfig>
{
public:
	ItemConfig();
	~ItemConfig();

	void		LoadFile(char*	szFile);
	
	ItemBase*	GetItemBase(int nTypeID);

private:
	xnList*		pListItems;			//��ʹ����Ʒ
	//xnList*		pListMaterials;
};