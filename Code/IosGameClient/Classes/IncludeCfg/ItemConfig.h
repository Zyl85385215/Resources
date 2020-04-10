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
{					//��Ʒ�Ͳ��϶���������
	ITEM_AD_UIN,
	ITEM_AD_ID,			//Ψһ��ʾ
	ITEM_AD_TYPE,		//��������PHASE
	ITEM_AD_NUM,			//����
	ITEM_AD_SLOT,		//����λ��
	ITEM_AD_POS,		//��������
	ITEM_AD_LIMITTIME,	//����ʱ��
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
	EQUIPEX_AD_LV,			//���λ�ȼ�
	EQUIPEX_AD_ADDTYPE1,
	EQUIPEX_AD_ADDRANDOM1,
	EQUIPEX_AD_ADDTYPE2,
	EQUIPEX_AD_ADDRANDOM2,
	EQUIPEX_AD_STONEPOS,	//000...00111 ��ʾ����3��
	EQUIPEX_AD_STONE12,	//��ʯλ12	ǰ16λ1,��16λ2
	EQUIPEX_AD_STONE34, //��ʯλ34	ǰ16λ3,��16λ4
	EQUIPEX_AD_EATLV, //����	���λ�ȼ�,ǰ24λ����
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
	ITEM_SLOT_SELL,		//�ع�
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
	ITEMTYPE_FAST,		//ҩƷ��
	ITEMTYPE_SKILLBOOK,
	ITEMTYPE_RIDE,
	ITEMTYPE_MAX,
};
enum ITEM_SCRPITFLAG{
	ITEM_SCRPITFLAG_ONUSE,
	ITEM_SCRPITFLAG_ONCHECK,
	ITEM_SCRPITFLAG_SP,		//�����ջ�

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
	ITEM_FLAG_COUNT,		//�Ƿ����Ĵ���
	ITEM_FLAG_LVUP,
	ITEM_FLAG_BORE,			//���
	ITEM_FLAG_INLAY,		//��Ƕ
	ITEM_FLAG_RESOLVE,		//�ֽ�
	ITEM_FLAG_COMPOSE,		//�ϳ�
	ITEM_FLAG_COST,			//�Ƿ�����Ʒ
};
enum ITEM_BINDTYPE{
	ITEM_BINDTYPE_NULL,		//����
	ITEM_BINDTYPE_BIND_GET,	//ʰȡ��
	ITEM_BINDTYPE_BING_EQUIP,	//װ��
};

enum MAKE_SKILL_TYPE{
	MAKE_SKILL_TYPE_NULL,
	MAKE_SKILL_TYPE_DIG,		//�ڿ�
	MAKE_SKILL_TYPE_COLLECT,	//�ɼ�
	MAKE_SKILL_TYPE_FISH,		//����
	MAKE_SKILL_TYPE_FORGE,		//����
	MAKE_SKILL_TYPE_SEWING,		//�÷�
	MAKE_SKILL_TYPE_STONE,		//�鱦
	MAKE_SKILL_TYPE_METALLURGY,	//����
	MAKE_SKILL_TYPE_FOOD,		//���
	MAKE_SKILL_TYPE_MAX,
};


const int nJJCStartState = 6201;

class ItemExBase
{
public:
	ItemExBase();
	~ItemExBase();
	
	WORD		nTypeID;		//��ƷID
	char*	szName;
	char*	szDesc;
	int		nSpr;
	int		nStar;		//Ʒ��
	BYTE	byType;		//��Ʒ����
	int		nNum;		//�ѵ�����
	BYTE	byLv;		//�ȼ�����

	int		nPrice;
	BYTE	bySlot;		//װ����λ
	BYTE	byBind;		//������
	int		nDura;		//�;á����＼��ID��ŭ������id
	int		nScrpitFlag;

	int		nUseType;
	int		nUseVal;

	char*	szLuaName;
	xnList* pAddAttr;	//ǰ׺
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
	xnList*		pListItems;			//��ʹ����Ʒ
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
	xnList*		pListItems;			//��ʹ����Ʒ
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

const int nMaxStoneCount = 4;		//��Ƕ��ʯ��
const int nStoneCostItem = 20001;	//���������Ʒ
const int nItemBoreCostCount[nMaxStoneCount] = {1,2,3,4}; //���������Ʒ����
const int nItemBoreCostMoney[nMaxStoneCount] = {10000,50000,100000,200000}; //�������Ǯ

const int nStoneComposeMoney = 10000;	//��ʯ�ϳ�����
const int nStoneComposeCount = 2;	//��ʯ�ϳ�����

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
	int			GetAddParam(int nSlot);	//0���˺� 1�ӷ���
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