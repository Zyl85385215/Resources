#pragma once
#include "ItemConfig.h"
#include "AmzDataServer.h"
#include <map>
#include <list>
#include "ios_item_packet.h"
#include "ZNetPacket.h"
#include "LuaClassObject.h"
#include "LuaStateManagerPlus.h"
using namespace std;
enum ITEM_LOG{
	ITEM_LOG_NULL,
	ITEM_LOG_BAG,			//�ű�
	ITEM_LOG_SHOP,		//�̵�
	ITEM_LOG_DROP,		//����
	ITEM_LOG_ADD,		//�������
	ITEM_LOG_TEST,		//����
	ITEM_LOG_TURN,		//���˴�ת��
	ITEM_LOG_QUEST,		//������
	ITEM_LOG_NASALE,		//�����й���
	ITEM_LOG_NASALE_LONGTIME,		//�����г�ʱ
	ITEM_LOG_MCOPY,		//��������
	ITEM_LOG_ACTIVE,		//�����ȡ
	ITEM_LOG_MAKE,		//��������
	ITEM_LOG_VIP,		//VIP���
	ITEM_LOG_MALL,		//�̳�
	ITEM_LOG_INVITE,		//�����뽱��
	ITEM_LOG_ACHIEVENT,		//�ɾͽ���
	ITEM_LOG_ONLINEAWARD,		//���߽���
	ITEM_LOG_LVUPAWARD,		//��������
	ITEM_LOG_LOGINGIFT,		//�ۼƵ�½
	ITEM_LOG_RECHANGE_AWARD,		//�׳影��
	ITEM_LOG_SIGUP_MOUTH,		//ÿ��ǩ��
	ITEM_LOG_MENTOR,		//ʦͽ
	ITEM_LOG_DEGREEAWARD,		//��Ծ�Ƚ���
	ITEM_LOG_MERRY,		//���
	ITEM_LOG_EXPCOPY,		//Զ������
	ITEM_LOG_KILLENEMY,		//ɱ��
	ITEM_LOG_REMOVESTONE,		//�Ƴ���ʯ
	ITEM_LOG_COMPSTONE,		//��ʯ�ϳ�
	ITEM_LOG_NASALE_NEW,	//�����м���
};
class UserData;
class ItemCenter;
class ItemManage;
class ItemObj:public CLuaClassObject<ItemObj> 
{
	friend class ItemCenter;
	friend class ItemManage;
public:
	ItemObj(AmzDataBase* pData,ItemCenter* pCenter);
	~ItemObj();
	static void			CreateScriptClass(CLuaState * pLuaState);
	static void			ReleaseScriptClass(CLuaState * pLuaState);
	ItemExBase*		m_pBase;
	bool			SetExData(int nType,AmzDataBase* pData);
	int				GetItemID();
	int				GetDataVal(BYTE nType);
	int				GetDataExVal(int nExType,BYTE nType);
	void			SetDataVal(BYTE nType,int nData);
	void			SetDataValEx(BYTE nType,int nData,bool bSaveDB = true);
	void			SetExDataVal(int nExType,BYTE nType,int nData);
	void			RecalcParam();
	int				GetIndex(){return m_pData->GetVal(ITEM_AD_ID);};
	int				m_nParam[EQUIP_ATTR_MAX];

	void			SendItemInfo(DWORD dwUin,int nKey = 0);

	void			GetByteData(char* pData);					//����Ʒ��Ϣ�����ַ���
	int				GetByteSize();								//��Ҫ�Ĵ�С

	int				GetTransferCost();

	bool			IsEquiped();
	int				GetScore(int nJob);
	int				GetScore();

	void			ChgDura(int nChg);

	void			Release();
private:
	AmzDataBase*	m_pData;
	AmzDataBase*	m_pDataEx[ITEMEXDATA_MAX];
	ItemCenter*		m_pCenter;
	void			_recalcAttr(int nType,int nVal);
	ItemExBase*		_getEquipStoneByPos(int nPos);

};


class ItemCenter :public CLuaClassObject<ItemCenter> 
{
	friend class ItemObj;
public:
	ItemCenter(UserData* pUser);
	~ItemCenter();
	static void			CreateScriptClass(CLuaState * pLuaState);
	static void			ReleaseScriptClass(CLuaState * pLuaState);
	void			Init();

	void			CheckEquipRank();
	ItemObj*		GetItemByIndex(int nID);
	int				GetItemCountByIDEx(int nType,int nSite = (1<<ITEM_SLOT_BAG|1<<ITEM_SLOT_DEPOT),int nBind = -1);	//nBind -1ȫѡ
	int				GetItemCountByID(int nType);
	void			SetBagSize(int nBagSize);
	bool			GetItemByType(int nType,list<ItemObj*>& itemlist,int nSite = (1<<ITEM_SLOT_BAG|1<<ITEM_SLOT_DEPOT),bool bBind = false);	//ֻ�Ա����Ͳֿ���Ч

	int				GetBlankBag();
	bool			CheckItemBlankBagByID(int nItemID,int nCount);
	bool			AddItemByID(int nType,int nCount = 1,int byBind = false,int nLogType = ITEM_LOG_NULL);
	bool			RemoveItemByIndex(int nID,int nCount = 1,int nLogType = ITEM_LOG_NULL); //nCount == -1ȫ����
	bool			RemoveItemByID(int nType,int nCount = 1,int nSite = (1<<ITEM_SLOT_BAG),int nLogType = ITEM_LOG_NULL );//ֻ�Ա����Ͳֿ���Ч
	bool			RemoveItemByIDEx(int nType,int nCount = 1,int nSite = (1<<ITEM_SLOT_BAG),int nLogType = ITEM_LOG_NULL,int nBind = -1 );//ֻ�Ա����Ͳֿ���Ч nBind -1ȫѡ

	bool			UseItem(ItemObj* pObj);
	bool			EquipTakeOn(ItemObj* pObj);
	bool			MoveItemToPos(ItemObj* pObj,int nPos);
	ItemObj*		GetEquipByIndex(int nIndex){
		if(nIndex>=EQUIP_MAX) return NULL;
		return m_pEquip[nIndex];
	};
	bool			ItemPacketProcess(ZPacket* pPacket);

	bool			SetItemBank(ItemObj* pItem);

	bool			TryStarLvUp(ItemObj* pItem);
	bool			TryAddStonePos(ItemObj* pObj);
	bool			TryInlayStone(ItemObj* pObj,ItemObj* pStone);
	bool			TryRemoveStone(ItemObj* pObj,int nPos);
	bool			TryComposeStone(int nID,int nCount);
	bool			TryEatEquip(ItemObj* pObj,ItemObj* bEat);
	bool			TryTransferEquip(ItemObj* pEquip,ItemObj* pEat);
	bool			TryResolveEquip(ItemObj* pEquip);

	ItemObj*		m_pEquip[EQUIP_MAX];


	bool			TryBuyMallItem(BYTE byType,int nItemID,int nCount);
	bool			TryBuyShopItem(int nType,int nItemID,int nCount);
	bool			TrySellItem(ItemObj* pObj);
	bool			TryFastSell(int nType);
	bool			TryAddBagSize();

	void			PileBag();
	void			PileBagByID();
	void			SortBag();


	UserData*		GetUser(){return m_pUser;};

	bool			CheckReplaceItem(int nIndex);
	bool			ReplaceItem(int nIndex);

	void			SendToClient();

	void			FixEquip(ItemObj* pObj,bool bRmb = false);
	void			FixAllUsedEquip(bool bRmb = false);

	void			OnItemScrpit(ItemObj* pObj,BYTE byType);

	void			SendMallItem();

	ItemObj*		CutItemByIndex(ItemObj* pObj,int nCount);

	bool			AddItem(ItemObj* pObj);

	bool			AddItemByMail(int nItemID,int nItemCnt,bool bBind);

	void			UpdateSec();

	bool			AddBagSizeByRMB();
	int				GetBagSize(){return m_nBagSize;};

	xnList*         m_pSysMsgedList;
	void            CheckSLvSysMsg(int nStarLv,ItemObj* pObj);

	int             GetStarState();
	int             GetStoneState();

	ItemObj*		GetPileItemInBagByID(int nID,bool bBind);
	ItemObj*		GetPileItemInBankByID(int nID,bool bBind);
	bool            PileItemBySite(ItemObj* pItem,int nSite);

	void            SetEnemyDrop(bool bEnemyDrop);
private: 

	map<int,ItemObj*>	m_mapItem;			//������Ʒ
	UserData*		m_pUser;
	
	//��Ʒ����
	int				m_nBagSize;
	ItemObj**		m_pItemInBag;
	//�ֿ�
	int				m_nBankSize;
	ItemObj**		m_pItemInBank;
	//�ع�
	list<ItemObj*>	m_listSellItem;			//������Ʒ


	int				_getBlankBagIndex();
	int				_getBlankBankIndex();
	ItemObj*		_addNewItem(int nType,int nCount,bool bBind = false);
	bool			_removeItem(ItemObj* pObj);
	inline bool			_checkEquip(ItemObj* pObj);
	bool			_setItemPos(ItemObj* pObj,int nSite,int nPos,bool bSaveDB =true);
	bool			_exchangeItemPos(ItemObj* pObj1,ItemObj* pObj2,bool bSaveDB = true);
	void			_addEquipEx(ItemObj* pObj);
	void			_setEquipExAttr(ItemObj* pObj,int nSite,int nType,int nVal);
	void			_addEquipStar(ItemObj* pObj,bool bSuccess = false);
	int				_addEquipStartemp(int nLv);
	ItemExBase*		_getEquipStoneByPos(ItemObj* pObj,int nPos);
	bool			_checkStoneConflict(ItemExBase* pBase1,ItemExBase* pBase2);
	bool			_inlayStone(ItemObj* pObj,int nItemID,int nPos);
	bool			_removeStone(ItemObj* pObj,int nPos);
	bool			_addSellItem(ItemObj* pObj);
	bool			_removeSellItem(ItemObj* pObj,bool bDelete = false);

	bool			_useItemEff(ItemObj* pObj);

	bool			_addBagSize(int nSize);

	void			_sendBagSize();
	void			_sendBlankBagRlt();

	int				_pileItem(ItemObj* pObjTo,ItemObj* pObjFrom);	//-1ʧ��,0toδ��,1to��from��ʧ,2to��from����

private:
	bool            m_bEnemyDrop;//����������
};

