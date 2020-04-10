#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#include "stdio.h"
#include <string>
#else
#include <string>
#include <Windows.h>
#endif
#include <map>
#include <list>
using namespace std;

#define EQUIP_MAX (ITEM_SLOT_EQUIP_END-ITEM_SLOT_EQUIP_START-1)
#define EQUIP_SLOT_TO_INDEX(x) (x-ITEM_SLOT_EQUIP_WEAPON)
#define EQUIP_INDEX_TO_SLOT(x) (x+ITEM_SLOT_EQUIP_WEAPON)


const int	nEffectSuccess = 28;
const int	nEffectFail = 29;
const int	nEffectCorrect = 75;
const int	nEffectWrong = 76;
const int nWarRankCntFlag = 19;
const int nBuyWarTimesFlag = 31;

const int	nItemColor[5][3] = {
	{255,255,255},
	{0,255,0},
	{0,204,255},
	{188,52,186},
	{188,52,186},
};
const int	nItemLimitColor[2][3] ={
	{230,20,16},
	{0,255,0},
};
enum
{					//物品和材料都在这里面
	ITEM_AD_UIN,
	ITEM_AD_ID,			//唯一标示
	ITEM_AD_TYPE,		//进阶类型PHASE
	ITEM_AD_NUM,			//数量
	ITEM_AD_SLOT,		//所在位置
	ITEM_AD_POS,		//包裹格子
	ITEM_AD_LIMITTIME,	//限制时间
	ITEM_AD_BIND,
	ITEM_AD_MAX,
};
enum ITEMEXData{
	ITEMEXDATA_EQUIPEX,
	ITEMEXDATA_MAX,
};
enum
{
	EQUIPEX_AD_ID,
	EQUIPEX_AD_UIN,
	EQUIPEX_AD_ITEMID,
	EQUIPEX_AD_LV,			//后八位等级
	EQUIPEX_AD_ADDTYPE1,
	EQUIPEX_AD_ADDRANDOM1,
	EQUIPEX_AD_ADDTYPE2,
	EQUIPEX_AD_ADDRANDOM2,
	EQUIPEX_AD_STONEPOS,	//000...00111 表示开孔3个
	EQUIPEX_AD_STONE12,	//宝石位12	前16位1,后16位2
	EQUIPEX_AD_STONE34, //宝石位34	前16位3,后16位4
	EQUIPEX_AD_EATLV, //精炼	后八位等级,前24位经验
	EQUIPEX_AD_MAX,
};
enum ITEM_USE_TYPE{
	ITEM_USE_NULL,
	ITEM_USE_ADDHP,
	ITEM_USE_ADDMP,
	ITEM_USE_ADDSTATE,
	ITEM_USE_ADDPOW,
	ITEM_USE_ADDPHP,
};
enum ITEM_SLOT{
	ITEM_SLOT_BAG,
	ITEM_SLOT_DEPOT,
	ITEM_SLOT_SELL,		//回购
	ITEM_SLOT_EQUIP_START = 30,
	ITEM_SLOT_EQUIP_WEAPON,
	ITEM_SLOT_EQUIP_CLOTH,
	ITEM_SLOT_EQUIP_LEG,
	ITEM_SLOT_EQUIP_HEAD,
	ITEM_SLOT_EQUIP_FOOT,
	ITEM_SLOT_EQUIP_SHOULDER,
	ITEM_SLOT_EQUIP_NECK,
	ITEM_SLOT_EQUIP_FINGER,
	ITEM_SLOT_EQUIP_BACK,
	ITEM_SLOT_EQUIP_SKIN,
	ITEM_SLOT_EQUIP_END,


};
enum ITEMTYPE{
	ITEMTYPE_ITEMCANNTUSE,
	ITEMTYPE_ITEMCANUSE,
	ITEMTYPE_EQUIP,
	ITEMTYPE_GIFT,
	ITEMTYPE_PETSKILL,
	ITEMTYPE_STONE,
	ITEMTYPE_FAST,		//药品等
	ITEMTYPE_SKILLBOOK,
	ITEMTYPE_RIDE,
	ITEMTYPE_MAX,
};
enum ITEM_SCRPITFLAG{
	ITEM_SCRPITFLAG_ONUSE,
	ITEM_SCRPITFLAG_ONCHECK,
	ITEM_SCRPITFLAG_SP,		//种子收获

};
const char* const szLuaItemFlag_SP = "OnItemSP";
const char* const szLuaItemFlag_OnUse = "OnItemUse";

enum EQUIP_ATTR{
	EQUIP_ATTR_HP,
	EQUIP_ATTR_MP,
	EQUIP_ATTR_ATK_W,
	EQUIP_ATTR_ATK_N,
	EQUIP_ATTR_DEF_W,
	EQUIP_ATTR_DEF_N,
	EQUIP_ATTR_HIT,
	EQUIP_ATTR_MISS,
	EQUIP_ATTR_CRIT,
	EQUIP_ATTR_MAX,
};
enum ITEM_FLAG{
	ITEM_FLAG_SELL,
	ITEM_FLAG_AAAAOE,
	ITEM_FLAG_BANK,
	ITEM_FLAG_DESTROY,
	ITEM_FLAG_LOG,
	ITEM_FLAG_COUNT,		//是否消耗次数
	ITEM_FLAG_LVUP,
	ITEM_FLAG_BORE,			//打孔
	ITEM_FLAG_INLAY,		//镶嵌
	ITEM_FLAG_RESOLVE,		//分解
	ITEM_FLAG_COMPOSE,		//合成
	ITEM_FLAG_COST,			//是否消耗品
};
enum ITEM_BINDTYPE{
	ITEM_BINDTYPE_NULL,		//不绑定
	ITEM_BINDTYPE_BIND_GET,	//拾取绑定
	ITEM_BINDTYPE_BING_EQUIP,	//装备
};

enum MAKE_SKILL_TYPE{
	MAKE_SKILL_TYPE_NULL,
	MAKE_SKILL_TYPE_DIG,		//挖矿
	MAKE_SKILL_TYPE_COLLECT,	//采集
	MAKE_SKILL_TYPE_FISH,		//钓鱼
	MAKE_SKILL_TYPE_FORGE,		//锻造
	MAKE_SKILL_TYPE_SEWING,		//裁缝
	MAKE_SKILL_TYPE_STONE,		//珠宝
	MAKE_SKILL_TYPE_METALLURGY,	//炼金
	MAKE_SKILL_TYPE_FOOD,		//烹饪
	MAKE_SKILL_TYPE_MAX,
};


const int nJJCStartState = 6201;

class ItemExBase
{
public:
	ItemExBase();
	~ItemExBase();
	
	WORD		nTypeID;		//物品ID
	char*	szName;
	char*	szDesc;
	int		nSpr;
	int		nStar;		//品质
	BYTE	byType;		//物品类型
	int		nNum;		//堆叠类型
	BYTE	byLv;		//等级限制

	int		nPrice;
	BYTE	bySlot;		//装备部位
	BYTE	byBind;		//绑定类型
	int		nDura;		//耐久、宠物技能ID、怒气技能id
	int		nScrpitFlag;

	int		nUseType;
	int		nUseVal;

	char*	szLuaName;
	xnList* pAddAttr;	//前缀
	int		nJob;
	int		nFlag;
	int		nParam[EQUIP_ATTR_MAX];
	int		nSkin;
	BYTE	bySexLimit;

	WORD	wCDPos;
	DWORD	dwCDTime;
	DWORD	dwTimeLimit;
	int		nCast;

	int		nGroup;
	inline bool	GetItemFlag(int nType){return nFlag&(1<<nType);};
};

class ItemConfig	: public CSingleton<ItemConfig>
{
public:
	ItemConfig();
	~ItemConfig();

	void		LoadFile(char*	szFile);
	
	ItemExBase*	GetItemBase(WORD nTypeID);

private:
	xnList*		pListItems;			//可使用物品
};
class EquipAttrBase
{
public:
	EquipAttrBase();
	int nID;
	int nType;
	int nBase[EQUIP_ATTR_MAX];
	int nRandom[EQUIP_ATTR_MAX];
};
class EquipAttrConfig	: public CSingleton<EquipAttrConfig>
{
public:
	EquipAttrConfig();
	~EquipAttrConfig();

	void		LoadFile(char*	szFile);

	EquipAttrBase*	GetEquipAttr(int nTypeID);

private:
	xnList*		pListItems;			//可使用物品
};
class EquipStarBase
{
public:
	int nPer;
	int nSuccess;
	int nExCount;
	int	nGold;
};
const int nEquipStarExCount = 15;
class EquipStarConfig	: public CSingleton<EquipStarConfig>
{
public:
	EquipStarConfig();
	~EquipStarConfig();

	void		LoadFile(char*	szFile);

	EquipStarBase*	GetEquipStar(int nTypeID);
	
	int			aExPer[nEquipStarExCount];
	int			aExSuccess[nEquipStarExCount];
	int			nItemID;
	int			nItemCount;
	
	int			GetMaxLv(){return pListItems->Count;};
private:
	xnList*		pListItems;	
};

const int nMaxStoneCount = 4;		//镶嵌宝石数
const int nStoneCostItem = 20001;	//打孔消耗物品
const int nItemBoreCostCount[nMaxStoneCount] = {1,2,3,4}; //打孔消耗物品个数
const int nItemBoreCostMoney[nMaxStoneCount] = {10000,50000,100000,200000}; //打孔消耗钱

const int nStoneComposeMoney = 10000;	//宝石合成消耗
const int nStoneComposeCount = 2;	//宝石合成消耗

class EquipEatBase
{
public:
	int exp;
	int nDamage;
	int nDefense;
};
class EquipEatConfig	: public CSingleton<EquipEatConfig>
{
public:
	EquipEatConfig();
	~EquipEatConfig();

	void		LoadFile(char*	szFile);

	EquipEatBase*	GetEquipEat(int nTypeID);

	int			m_nEatEquipVal[4];
	int			GetAddParam(int nSlot);	//0加伤害 1加防御
private:
	int			m_nSlot;
	xnList*		pListItems;	
};

const int nTransferMoney = 100000;
const int nTransferItemID = 20005;
const int nTransferItemCount = 1;


class CommonItemTag{
public:
	int nItemID;
	int nItemCount;
	int nPer;
};
class ItemReplaceTag{
public:
	ItemReplaceTag();
	~ItemReplaceTag();
	xnList* pListCost;
	xnList* pListGet;
	int nType;
};

class ItemReplaceConfig	: public CSingleton<ItemReplaceConfig>
{
public:
	ItemReplaceConfig();
	~ItemReplaceConfig();

	void		LoadFile(char*	szFile);
	ItemReplaceTag*	GetItemReplaceTag(int nIndex);

private:
	xnList*		pList;
};

const int nMaxResolveTypeCount = 10;
class EquipResolveConfig: public CSingleton<EquipResolveConfig>
{
public:
	EquipResolveConfig();
	~EquipResolveConfig();
	
	void		LoadFile(char*	szFile);

	int			nLv[nMaxResolveTypeCount];
	int			nCloth[4][nMaxResolveTypeCount];
	int			nIron[4][nMaxResolveTypeCount];
	int			nClothID;
	int			nIronID;
	int			GetLvType(int nLv);	
};
#ifdef _ISSERVER
typedef string tempstring;
#else
class tempstring{
public:
	tempstring(){szChar=strdup("");};
	~tempstring(){delete szChar;};
	tempstring(char* szc){szChar = strdup(szc);};
	tempstring(const tempstring& another){
		//if(szChar) delete szChar;
		szChar = strdup(another.szChar);
	};
	bool operator == (const tempstring& temp){return !strcmp(szChar,temp.szChar);};
	tempstring& operator = (const tempstring& szc){//if(szChar) delete szChar;
			szChar = strdup(szc.szChar);
			return *this;
	};
	bool operator <(const tempstring& temp) const{return strcmp(szChar,temp.szChar)<0;};
	char* c_str() const{return szChar;};
private:
	char* szChar;
};
#endif

class MakeSkillTag{
public:
	MakeSkillTag();
	~MakeSkillTag();
	xnList* pListCost;
	CommonItemTag tagGet;
	int nID;
	int nType;
	int nLv;
	int nJob;
	int nMake1;
	int nMake2;
	int nVigor;
	int nGold;
	tempstring szFilter;
};


class MakeSkillConfig: public CSingleton<MakeSkillConfig>
{
public:
	MakeSkillConfig();
	~MakeSkillConfig();

	void		LoadFile(char*	szFile);

	map<int,map<tempstring,map<int,MakeSkillTag*> > >	m_mapInfo;
	map<int,MakeSkillTag*>	m_mapIDInfo;
	xnList*		m_pList[5];

	void		GetMakeList(int nType,int nJob,int nLv,xnList* pList);
	void		GetMakeList(int nType,int nJob,tempstring& szFilter,xnList* pList);
	MakeSkillTag*	GetMakeItem(int nID);
};
struct EquipGroupVal 
{
	BYTE byCount;
	int	nState;
	char szDesc[32];
};
class EquipGroupInfo{
public:
	int nID;
	char szName[32];
	list<int>	nItemID;
	list<EquipGroupVal> groupVal;
};

class EquipGroupConfig: public CSingleton<EquipGroupConfig>
{
public:
	~EquipGroupConfig();
	void		LoadFile(char*	szFile);
	map<int,EquipGroupInfo*>	m_mapInfo;
	EquipGroupInfo*	GetItem(int nID);

};