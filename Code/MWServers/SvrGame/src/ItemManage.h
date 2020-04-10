#pragma once
#include "ItemConfig.h"
#include <map>
#include <list>
#include "xnlist.h"

enum ItemManageType{
	ITEM_MANAGE_TYPE_ITEMOBJ,

};

using namespace std;
class ItemObj;
class DBControl;
class UserData;
class ItemManage{
public:
	ItemManage();
	static ItemObj*	CreateItemObjByString(char* szInput,int nSize);
	static ItemObj*	CreateItemByID(int nID,bool bBind = false);

	bool	DBInit(DBControl* pDB);

	int		AddItemObj(ItemObj* pObj,bool bSaveDB = false);
	bool	AddItemObj(ItemObj* pObj,int nIndex,bool bSaveDB = false);

	ItemObj* GetItemObj(int nIndex);

	ItemObj* RemoveItemObj(int nIndex,bool bSaveDB);


	//map<int,list<ItemObj*> >		m_TypeMap;	//key为物品ID

	bool	SetItemOwner(UserData* pUser,ItemObj* pObj,BYTE byLogType = 0);		//设置该物品的所有者

	static ItemObj*	GetItemByIndex(DWORD dwUin,int nIndex);
private:
	int		m_nNextItemId;
	DBControl*		m_pDB;

	void	_dbInsertItem(int nIndex,ItemObj* pItem);
	void	_dbDeleteItem(int nIndex);

	static void	_addEquipEx( ItemObj* pObj );
	static void	_setEquipExAttr( ItemObj* pObj,int nSite,int nType,int nVal );
	map<int,ItemObj*>	m_TotalMap;
};