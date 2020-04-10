#ifndef _NA_DATATRANS_H
#define _NA_DATATRANS_H

#include "item_packet.h"
#include "NAuction_Data.h"


struct NADATA_ItemInfo
{
	NADATA_ItemInfo()
	{
		pIBase	= NULL;
		pLvlUp	= NULL;
		pBind	= NULL;
		pExp	= NULL;
		pJudge	= NULL;
		pGems	= NULL;
		pTrump	= NULL;
		pPlusInfo = NULL;
		pCarve  = NULL;
		nGemsSize	= 0;
		byInfo	= 0;
		nTotalSize	= 0;
	}
	~NADATA_ItemInfo()
	{
		if(pIBase)	delete pIBase;
		if(pLvlUp)	delete pLvlUp;
		if(pBind)	delete pBind;
		if(pExp)	delete pExp;
		if(pJudge)	delete pJudge;
		if(pGems)	free(pGems);
		if(pTrump)	delete pTrump; 
		if(pPlusInfo) delete(pPlusInfo);
		if(pCarve)  delete pCarve;
	}
	
	NADATA_ItemBase* pIBase;
	ItemExtendInfo* pLvlUp;
	ItemBindInfo* pBind;
	ItemExpExtend* pExp;
	ExtendData* pJudge;
	void* pGems;
	TrumpSPData*	pTrump;
	ItemPlusUpInfo * pPlusInfo;
	ItemCarveInfo*	pCarve;
	int nGemsSize;

	//不用初始化
	int nTotalSize;
	BYTE	byInfo;
};

class NA_DataTrans
{
public:
	//NA_DataTrans();
	//~NA_DataTrans();
	//3层
	static NADATA_ItemBase*		GetItemBase(PNA_ITEM_DATA pItem);
	static bool					GetGemsData(PNA_ITEM_DATA pItem, OUT void** ppGems, OUT int& nSize);
	static ItemExtendInfo*		GetLvlUpData(PNA_ITEM_DATA pItem);
	static ItemBindInfo*		GetBindData(PNA_ITEM_DATA pItem);
	static ItemExpExtend*		GetExpData(PNA_ITEM_DATA pItem);
	static ExtendData*			GetJudgeData(PNA_ITEM_DATA pItem);
	static ItemPlusUpInfo *		GetPlusInfo( PNA_ITEM_DATA pItem );
	static TrumpSPData *		GetTrumpData( PNA_ITEM_DATA pItem );

	static PNA_ITEM_DATA		CreatItemData(NADATA_ItemInfo* pInfo, OUT int& nSize);
	static NADATA_ItemInfo*		CreatItemInfo(PNA_ITEM_DATA pData);
	//2层
	static PNA_OBJ_DATA			CreatNAObjDataEx(PNA_ITEM_DATA* ppItem,int* pnLen,int nCnt,OUT int& nSize,DWORD dwKey=0);
	static PNA_OBJ_DATA			CreatNAObjData(NADATA_ItemInfo** ppItem,int nCnt,OUT int& nSize,DWORD dwKey=0);
	static bool					SetNAObjKey(PNA_OBJ_DATA pNAObj,DWORD dwKey);
	static DWORD				GetNAObjKey(PNA_OBJ_DATA pNAObj);
	static PNA_ITEM_DATA		GetNAObjItem(PNA_OBJ_DATA pNAObj,int nPos);

	static NADATA_ItemBase*		GetItemBase(PNA_OBJ_DATA pNAObj,int nPos);
	static bool					GetGemsData(OUT void** ppGems, OUT int& nSize,PNA_OBJ_DATA pNAObj,int nPos=0);
	static ItemExtendInfo*		GetLvlUpData(PNA_OBJ_DATA pNAObj,int nPos);
	static ItemBindInfo*		GetBindData(PNA_OBJ_DATA pNAObj,int nPos);
	static ItemExpExtend*		GetExpData(PNA_OBJ_DATA pNAObj,int nPos);
	static ExtendData*			GetJudgeData(PNA_OBJ_DATA pNAObj,int nPos);
	static ItemPlusUpInfo *		GetPlusInfo( PNA_OBJ_DATA pNAObj,int nPos);
	
	static TrumpSPData *		GetTrumpData( PNA_OBJ_DATA pNAObj,int nPos);
	
	//1层
	static PNA_PACKET_DATA		CreatPacketData(PNA_OBJ_DATA* ppNAObj,int nCnt);
	static int					GetPacketObjCnt(PNA_PACKET_DATA pPackData);
	static PNA_OBJ_DATA			GetPacketObj(PNA_PACKET_DATA pPackData,int nPos);
private:
	static bool					_CheckBit(const BYTE& byVal,BYTE byPos);
	static int					_GetItemSize(NADATA_ItemInfo* pInfo);
	static bool					_SetItemData(NADATA_ItemInfo* pInfo, BYTE* pData);

	
};

#endif