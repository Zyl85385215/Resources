#pragma once


#include "AmzDataBase.h"
#include "xnlist.h"
#include "ItemConfig.h"
#include <list>
#include "Interface.h"
class ItemObj
{
public:
	static	ItemObj* Create(AmzDataBase* pData);

	ItemObj(AmzDataBase* pData);
	~ItemObj();
	int		GetVal(BYTE bEnum);
	bool		Update();

	ItemExBase*		pItemBase;
	int				m_nParam[EQUIP_ATTR_MAX];

	void			RecalcParam();
	bool			SetExData(int nType,AmzDataBase* pData);
	int				GetExData(int nType,BYTE bEnum);

	void			GetBaseData(int* nBase);		//��û�������+ǰ׺����
	int				GetStarPer(int nLv);			//����Ǽ��ӳɰٷֱ�
	void			GetAllParam(int* nBase);		//��������
	int				GetStarLv();
	int				GetStarLvEx();
	ItemExBase*		GetStoneByPos(int nPos);
	 
	int				GetScore();
	
	int				GetTransferCost();

	bool			CheckAddPer();			//�Ƿ�ǿ����

	bool			IsEquip();
private:
	AmzDataBase*		pAmzData;
	AmzDataBase*		m_pDataEx[ITEMEXDATA_MAX];
	void			_recalcAttr(int nType,int nVal,int* nParam);

};
class _itemInfoCallback{
public:
	int		nKey;
	void	(*callBack)(ItemObj* pObj);
};
class ItemCenter: public PacketListener
{
public:
	ItemCenter(AmzDataManager* pDC);
	~ItemCenter();

	void		Update();
	void		OnItemRemove(ItemObj* pObj);

	bool	        GetItemByID(int	nID,std::list<ItemObj*>& itemlist);
	ItemObj*	    GetItemByIndex(int	nID);
	int				GetItemCountByID(int nID,int nSlot = (1<<ITEM_SLOT_BAG));
	bool			UseItem(ItemObj* pObj,bool bCast = true);
	bool			SetItemBank(ItemObj* pObj);
	bool			DestroyItem(ItemObj* pObj);
	bool			EquipInlay(ItemObj* pEquip,ItemObj* pStone);
	bool			EquipRemoveStone(ItemObj* pEquip,int nPos);
	bool			ComposeStone(int nID,int nCount);
	bool			EquipEat(ItemObj* pEquip,ItemObj* pEat);
	bool			EquipTransfer(ItemObj* pEquip,ItemObj* pEat);

	virtual bool		PacketProcess(ZPacket* pPacket);
	bool		m_bChgNumber;
	xnList*				m_pListItems;
	void		GetItemInBagByPos(xnList* pList);
	void		GetItemsByKind(int nType,xnList* pList);
	void		GetItemInBagByType(xnList* pList,int nType);
	void		GetItemInBagBySlot(xnList* pList,int nType);
	void		GetItemInBagByFilter(xnList* pList,bool (*nFun)(ItemObj* pObj,void* param),void* param = NULL);
	bool		CheckEquip( ItemObj* pObj );
	ItemObj*	GetEquipBySlot(int nSlot);
	void        GetItemInBankByPos( xnList* pList );
	bool		EquipStarUp(ItemObj* pObj);
	bool		EquipBore(ItemObj* pObj);
	void		_updateMenu();
	bool		BuyMallItem(BYTE byType,int nItemID,int nCount);
	bool		SellItem(ItemObj* pObj);
	bool		BuyShopItem(int nType,int nItemID,int nCount);
	bool		FastSell(int nType);
	void		SortBag();
	void		GoForFix();
	void		GoForBuy(int nType = 0);

	void				GetItemInfoFormServer(int dwUin,int nItemID,void (*ItemInfoCallback)(ItemObj* pObj) );//�ӷ���˻����Ʒ,��Է�����
	ItemObj*			CreateItemByData(char* pData,int nSize);												//���ַ����д���itemobj


	ItemObj*			GetTempItem(int dwUin,int nItemID);
	void				AddTempItem(ItemObj* pItem);
	void				RemoveTempItem(int dwUin,int nItemID);

	int					GetWeaponSkin();
	int                 GetClothSkin();

	ItemObj*	GetItemByTypeID(int	nTypeID,int nSlot = (1<<ITEM_SLOT_BAG));
	void				GetMallItemByType(xnList* pList,BYTE byType);
	bool				m_ifNeedFix;
	int					m_nBagSize;	
	xnList*				m_pMallList;

	int                 m_nBagCurCount;
	int                GetBagCurCount(); //===zjk

	bool	CheckSkin(int nSkin);	//�Ƿ�ʱװ

	void				UseMakeSkill(MakeSkillTag* pTag);
	void				MakeSkillResult(BYTE byResult);
	void				LearnMakeSkill(BYTE byMakeSkill);

	int					GetEquipCountInGroup(int nGroup);
	bool				JudgeChangeCountryItem(int nItemID);

private:

	AmzDataManager*		pDataCenter;

	xnList*				m_pListTempItems;			//��ʱ��Ҫ��ʾ����Ʒ��Ϣ

	_itemInfoCallback*	_getItemInfoCallback(int nKey);
	xnList*				m_pListCallback;
};

void ShowItemInfoOnComp(DComponent* pComp,int nType,int nNum = 1);