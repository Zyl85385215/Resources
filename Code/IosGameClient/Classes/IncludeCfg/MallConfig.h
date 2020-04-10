#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif


class MallItemCfg;
const int nBindMall = 6;
class MallConfig	: public CSingleton<MallConfig>
{
public:
	MallConfig();
	~MallConfig();

	void		LoadFile(char*	szFile);
	MallItemCfg*	GetCfg(BYTE byType,int nItemID);
	MallItemCfg*	GetCfg(int nItemID);
	xnList*		m_pItemList;
	void		GetMallItemByType(xnList* pList,BYTE byType);
private:
};
class ShopItemCfg{
public:
	int		m_nTypeInfo;
	int		m_nItemID;
	int		m_nPrice;
	int		m_nLimit;
	BYTE	m_byReflush;
	BYTE	m_byType;
};

class ShopInfoCfg{
public:
	ShopInfoCfg();
	~ShopInfoCfg();
	int		nType;
	xnList*	pItemList;
	ShopItemCfg*	GetItemCfg(int nItemID);
};

const int nWarRankShop = 999;
enum{
	SHOP_TYPE_MONEY,
	SHOP_TYPE_WARRANK,

};
class ShopConfig	: public CSingleton<ShopConfig>
{
public:
	ShopConfig();
	~ShopConfig();

	void		LoadFile(char*	szFile);
	ShopItemCfg*	GetCfg(int nType,int nItemID);
	ShopInfoCfg*	GetCfg(int nType);
	xnList*		m_pItemList;
private:
};