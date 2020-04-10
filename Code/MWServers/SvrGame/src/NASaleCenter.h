#pragma once


#include <Windows.h>
#include "xnlist.h"

#include "ItemConfig.h"
#include "ZNetPacket.h"

class DBControl;
class ItemObj;
class UserData;
class	NASaleObj
{
public:
	DWORD	dwNAIndex;
	DWORD	dwFUin;
	BYTE	byType;			//0ºƒ €, 1 ’π∫
	DWORD	dwStartTickSec;	//
	int		nPrice;

	WORD	wItemID;
	WORD	wItemCnt;
	DWORD	dwItemInfo;

	ItemExBase* pBase;
};


//≈ƒ¬Ù––
class	NASaleCenter
{
public:
	NASaleCenter();
	~NASaleCenter();

	void		DB_Init(DBControl* pDB);
	void		UpdateMinute();

	bool		RecvPacket(ZPacket* pPacket,DWORD	dwUin);

	DWORD		AddNewSale(ItemObj* pItem, DWORD dwUin, int nPrice);
	bool		RqBuyItem(UserData* pBuyer,	DWORD	dwNAIndex,WORD wCnt);

	bool		RqGetMyItem(UserData* pBuyer,	DWORD	dwNAIndex);

	DWORD		RequestItem(WORD wItem, DWORD dwUin, int nPrice,WORD wCnt);
	bool		AnswerItem(UserData* pAnswer, DWORD dwNAIndex,ItemObj* pItem,WORD wCnt);

	int			RqViewItem(BYTE bNAType,BYTE bType, BYTE bJob,BYTE bLvSel, int nStartPos, xnList* pOutList);
	int			RqViewMyItem(BYTE bNAType,DWORD dwUin, xnList* pOutList);
	int			RqViewMyItemTimeOut(BYTE bNAType,DWORD dwUin, xnList* pOutList);

	void		CalcMySale(UserData* pBuyer);

	xnList*		m_pListSales;		//
	xnList*		m_pListRequests;
	DWORD		m_dwMaxNAIndex;
	DBControl*			m_pDB;

	DWORD		m_dwFilterMaxCnt;

private:
	void		_removeDB(DWORD dwNaIndex);
	void		_insertDB(NASaleObj* pNAObj);
	void		_updateDB(NASaleObj* pNAObj);
};