#pragma once

#include <Windows.h>
#include <map>
#include <list>
#include "ItemBase.h"
#include "xnExtstr.h"
#include "singleton.h"

#define SAFE_DELETE(p) { if(p) { delete (p); (p) = 0; } }

enum EMALL_TYPE
{
	MALL_MEDICINE,		//��ҩ
	MALL_UPGRADE,		//��װ
	MALL_HORSE,			//����
	MALL_SLAVE,			//����
	MALL_SPACE,			//�ռ�
	MALL_CONFORT,		//���
	MALL_ASSISTANT,		//����
	MALL_FUDAI,			//����
	MALL_BOON,			//������
	MALL_HIDE,			//������

	Y_MALL_MEDICINE,	//��ҩ
	Y_MALL_UPGRADE,		//��װ
	Y_MALL_HORSE,		//����
	Y_MALL_SLAVE,		//����
	Y_MALL_SPACE,		//�ռ�
	Y_MALL_CONFORT,		//���
	Y_MALL_ASSISTANT,	//����
	Y_MALL_FUDAI,		//����
	Y_MALL_BOON,		//������
	Y_MALL_HIDE,		//������

	BG_MALL_MEDICINE,	//��ҩ
	BG_MALL_UPGRADE,	//��װ
	BG_MALL_HORSE,		//����
	BG_MALL_SLAVE,		//����
	BG_MALL_SPACE,		//�ռ�
	BG_MALL_CONFORT,	//���
	BG_MALL_ASSISTANT,	//����
	BG_MALL_FUDAI,		//����
	BG_MALL_BOON,		//������
	BG_MALL_HIDE,		//������

	WW_MALL_EXITEM,

	MALL_MAX,
};

struct MallShopItemData
{
	WORD	itemID;
	BYTE	type;
	int		price;		//�Զ���۸��ʱ��ʹ��
	BYTE	bGoldType;
	bool	bCanSell;	//�Ƿ�����������������ͣ�
	bool	bCanGive;	//�Ƿ��������
	WORD	wLimitCnt;	//���ƹ���������0��ʾû�����ƣ�
};

struct SellMallItem
{
	WORD	itemID;
	BYTE	bBindType;		//0,1,10
	int		price;
	int		nSellCnt;
};

class MallSellConfig : public CSingleton<MallSellConfig>
{
public:
	MallSellConfig();
	~MallSellConfig();

	bool Load(const char * szFile);
	SellMallItem* GetItem(int nItemID);

private:
	xnList* m_pList;
};

struct MallShopId
{
	MallShopId(WORD itemId, BYTE goldType)
	{
		itemId_ = itemId;
		goldType_ = goldType;
	}

	WORD	itemId_;
	BYTE	goldType_;

	friend bool operator<(const MallShopId & id1, const MallShopId & id2)
	{
		if(id1.itemId_ != id2.itemId_)
		{
			return id1.itemId_ < id2.itemId_;
		}
		return id1.goldType_ < id2.goldType_;
	}
};

class CMallShopItemList
{
public:
	CMallShopItemList(ItemBase * pItemBase);
	~CMallShopItemList(void);

	bool Load(const char * szFile);
	bool GetItem(int nItemID, BYTE goldType, MallShopItemData & xMallShopItemData);
	void SendMallShopItems(DWORD dwUin);

	void UpdateItemBase(bool bSave = true);

	inline int GetVersion(void) { return m_nVersion; }

private:
	void _LoadShopItem(xnExtstr * pExtstr, int nType);

	int m_nVersion;
	std::list<MallShopItemData*> m_listMallShopItemData;
	std::map<MallShopId, MallShopItemData*> m_mapMallShopItemData;
	ItemBase * m_pItemBase;
};

class CMallConfig : public CSingleton<CMallConfig>
{
public:
	CMallConfig(void);
	~CMallConfig(void);

	void Init(ItemBase * pItemBase);
	bool Load(const char * szFile);
	bool GetItem(int nVersion, int nItemID, BYTE goldType, MallShopItemData & xMallShopItemData);
	void SendMallShopItems(DWORD dwUin,int type=0);

	inline int GetVersion(void) { return m_nLastVersion; }

private:
	void _Clear(void);
	void _LoadShopItem(xnExtstr * pExtstr, int nType);

	int m_nLastVersion;
	std::list<CMallShopItemList*> m_listMallShopItemList;

	CRITICAL_SECTION m_xMallConfigCS;
	ItemBase * m_pItemBase;
};