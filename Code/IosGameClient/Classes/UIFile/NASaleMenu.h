#pragma once

#include "CtrlObj.h"
#include "PacketCenter.h"

class	NASaleClient
{
public:
	DWORD	dwNAIndex;
	BYTE	byType;
	DWORD	dwEndSec;	//
	int		nPrice;

	WORD	wItemID;
	WORD	wItemCnt;
	DWORD	dwItemInfo;

	ItemExBase* pBase;
};

class NASaleMenu	: public PacketListener
{
public:
	NASaleMenu();
	~NASaleMenu();

	void	Update();
	virtual	bool PacketProcess(ZPacket* pPacket);

	void		OpenForm();
	void		OpenMySale();

	void		SearchSellItems(BYTE bJob, BYTE bType, BYTE bLvStep, int nStartPos);
	void		ViewMySales();
	void		RequestSellItem(ItemObj* pItem, int nPrice, int nCount);
	void		RequestBuyItem();
	void		RequestGetMyItem();

	DForm*		m_pForm;
	DForm*		m_pMySaleForm;

	int         m_nPage;
	int         m_nMaxPage;
	int			m_nCurSelPos;
	NASaleClient*	m_pSelSale;
	xnList*		m_pListSale;
	xnList*		m_pListMine;
	int			m_nMySaleMoney;
	int         m_nMySaleCnt;

	void		FilterUpdata(int nPage=0);
	//void		Refresh(xnList* pList);
	int         m_nFilterType[3];
	//xnList*		m_pTypeFilterList;
	//xnList*		m_pJopFilterList;
	//xnList*		m_pRankFilterList;

};

class NASaleFilterMenu
{
public:
	NASaleFilterMenu();
	~NASaleFilterMenu();
	void		OpenForm(int nType,int nX,int nY);

	DForm*		m_pForm;
	int         m_nFilterType;
};

class NASalePriceMenu
{
public:
	NASalePriceMenu();
	~NASalePriceMenu();
	void		OpenForm(ItemObj* pObj);
	void		Close();

	DForm*		m_pForm;

	int         m_nNums;
	int         m_nPrice;

	ItemObj*    m_pSaleItem;
};