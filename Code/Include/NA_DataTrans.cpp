#include "NA_DataTrans.h"


PNA_ITEM_DATA NA_DataTrans::CreatItemData( NADATA_ItemInfo* pInfo, OUT int& nSize )
{
	nSize = _GetItemSize(pInfo);

	BYTE* pItem = (BYTE*)malloc(nSize);
	_SetItemData(pInfo,pItem);
	return pItem;
}

NADATA_ItemInfo* NA_DataTrans::CreatItemInfo(PNA_ITEM_DATA pData)
{
	NADATA_ItemInfo* pInfo = new NADATA_ItemInfo();
	BYTE* pSeek = pData;
	pInfo->nTotalSize = *(DWORD*)pSeek;
	pSeek+=4;
	pSeek+=2;
	pInfo->byInfo = *pSeek;
	pSeek++;
	pInfo->pIBase = new NADATA_ItemBase;
	memcpy(pInfo->pIBase,pSeek,sizeof(NADATA_ItemBase));
	//pInfo->pIBase = (NADATA_ItemBase*)pSeek;
	pSeek+=sizeof(NADATA_ItemBase);
	if(_CheckBit(pInfo->byInfo,0))
	{
		pInfo->pLvlUp = new ItemExtendInfo;
		memcpy(pInfo->pLvlUp,pSeek,sizeof(ItemExtendInfo));
		//pInfo->pLvlUp = (ItemExtendInfo*)pSeek;
		pSeek+=sizeof(ItemExtendInfo);
	}
	if(_CheckBit(pInfo->byInfo,1))
	{
		pInfo->pBind = new ItemBindInfo;
		memcpy(pInfo->pBind,pSeek,sizeof(ItemBindInfo));
		//pInfo->pBind = (ItemBindInfo*)pSeek;
		pSeek+=sizeof(ItemBindInfo);
	}
	if(_CheckBit(pInfo->byInfo,2))
	{
		pInfo->pExp = new ItemExpExtend;
		memcpy(pInfo->pExp,pSeek,sizeof(ItemExpExtend));
		//pInfo->pExp = (ItemExpExtend*)pSeek;
		pSeek+=sizeof(ItemExpExtend);
	}
	if(_CheckBit(pInfo->byInfo,3))
	{
		pInfo->pJudge = new ExtendData;
		memcpy(pInfo->pJudge,pSeek,sizeof(ExtendData));
		//pInfo->pJudge = (ExtendData*)pSeek;
		pSeek+=sizeof(ExtendData);
	}
	if(_CheckBit(pInfo->byInfo,4))
	{
		pInfo->nGemsSize = *(int*)pSeek;
		pSeek+=4;
		pInfo->pGems = malloc(pInfo->nGemsSize);
		memcpy(pInfo->pGems,pSeek,pInfo->nGemsSize);
		pSeek+=pInfo->nGemsSize;
		//pInfo->pGems = pSeek;
	}
	if(_CheckBit(pInfo->byInfo,5))
	{
		pInfo->pTrump = new TrumpSPData();
		memcpy(pInfo->pTrump,pSeek,sizeof(TrumpSPData));
		pSeek+=sizeof(TrumpSPData);
	}
	if(_CheckBit(pInfo->byInfo,6))
	{
		pInfo->pPlusInfo = new ItemPlusUpInfo;
		memcpy(pInfo->pPlusInfo,pSeek,sizeof(ItemPlusUpInfo));
		pSeek+=sizeof(ItemPlusUpInfo);
	}
	if(_CheckBit(pInfo->byInfo,7))
	{
		pInfo->pCarve = new ItemCarveInfo();
		memcpy(pInfo->pCarve,pSeek,sizeof(ItemCarveInfo));
		pSeek+=sizeof(ItemCarveInfo);
	}
	return pInfo;
}

NADATA_ItemBase* NA_DataTrans::GetItemBase( PNA_ITEM_DATA pItem )
{
	NADATA_ItemBase* pOut = (NADATA_ItemBase*)(pItem+7);
	return pOut;
}

NADATA_ItemBase* NA_DataTrans::GetItemBase( PNA_OBJ_DATA pNAObj,int nPos )
{
	PNA_ITEM_DATA pItem = GetNAObjItem(pNAObj,nPos);
	if(!pItem)
		return NULL;
	return GetItemBase(pItem);
}
bool NA_DataTrans::GetGemsData( PNA_ITEM_DATA pItem, OUT void** ppGems, OUT int& nSize )
{
	BYTE byPos = pItem[6];
	if(!_CheckBit(byPos,4))
		return false;
	int nSeek = 7+sizeof(NADATA_ItemBase);
	if(_CheckBit(byPos,0)) nSeek+=sizeof(ItemExtendInfo);
	if(_CheckBit(byPos,1)) nSeek+=sizeof(ItemBindInfo);
	if(_CheckBit(byPos,2)) nSeek+=sizeof(ItemExpExtend);
	if(_CheckBit(byPos,3)) nSeek+=sizeof(ExtendData);
	BYTE* pOut = pItem+nSeek;
	nSize = *((int*)pOut);
	*ppGems = pOut+sizeof(int);
	return true;
}

bool NA_DataTrans::GetGemsData( OUT void** ppGems, OUT int& nSize,PNA_OBJ_DATA pNAObj,int nPos )
{
	PNA_ITEM_DATA pItem = GetNAObjItem(pNAObj,nPos);
	if(!pItem)
		return false;
	return GetGemsData(pItem,ppGems,nSize);
}

ItemExtendInfo* NA_DataTrans::GetLvlUpData( PNA_ITEM_DATA pItem )
{
	BYTE byPos = pItem[6];
	if(!_CheckBit(byPos,0))
		return NULL;
	ItemExtendInfo* pOut = (ItemExtendInfo*)(pItem+7+sizeof(NADATA_ItemBase));
	return pOut;
}

ItemExtendInfo* NA_DataTrans::GetLvlUpData( PNA_OBJ_DATA pNAObj,int nPos )
{
	PNA_ITEM_DATA pItem = GetNAObjItem(pNAObj,nPos);
	if(!pItem)
		return NULL;
	return GetLvlUpData(pItem);
}
ItemBindInfo* NA_DataTrans::GetBindData( PNA_ITEM_DATA pItem )
{
	BYTE byPos = pItem[6];
	if(!_CheckBit(byPos,1))
		return NULL;
	int nSeek = 7+sizeof(NADATA_ItemBase);
	if(_CheckBit(byPos,0)) nSeek+=sizeof(ItemExtendInfo);
	ItemBindInfo* pOut = (ItemBindInfo*)(pItem+nSeek);
	return pOut;
}

ItemBindInfo* NA_DataTrans::GetBindData( PNA_OBJ_DATA pNAObj,int nPos )
{
	PNA_ITEM_DATA pItem = GetNAObjItem(pNAObj,nPos);
	if(!pItem)
		return NULL;
	return GetBindData(pItem);
}
ItemExpExtend* NA_DataTrans::GetExpData( PNA_ITEM_DATA pItem )
{
	BYTE byPos = pItem[6];
	if(!_CheckBit(byPos,2))
		return NULL;
	int nSeek = 7+sizeof(NADATA_ItemBase);
	if(_CheckBit(byPos,0)) nSeek+=sizeof(ItemExtendInfo);
	if(_CheckBit(byPos,1)) nSeek+=sizeof(ItemBindInfo);
	ItemExpExtend* pOut = (ItemExpExtend*)(pItem+nSeek);
	return pOut;
}

ItemExpExtend* NA_DataTrans::GetExpData( PNA_OBJ_DATA pNAObj,int nPos )
{
	PNA_ITEM_DATA pItem = GetNAObjItem(pNAObj,nPos);
	if(!pItem)
		return NULL;
	return GetExpData(pItem);
}
ExtendData* NA_DataTrans::GetJudgeData( PNA_ITEM_DATA pItem )
{
	BYTE byPos = pItem[6];
	if(!_CheckBit(byPos,3))
		return NULL;
	int nSeek = 7+sizeof(NADATA_ItemBase);
	if(_CheckBit(byPos,0)) nSeek+=sizeof(ItemExtendInfo);
	if(_CheckBit(byPos,1)) nSeek+=sizeof(ItemBindInfo);
	if(_CheckBit(byPos,2)) nSeek+=sizeof(ItemExpExtend);
	ExtendData* pOut = (ExtendData*)(pItem+nSeek);
	return pOut;
}

ExtendData* NA_DataTrans::GetJudgeData( PNA_OBJ_DATA pNAObj,int nPos )
{
	PNA_ITEM_DATA pItem = GetNAObjItem(pNAObj,nPos);
	if(!pItem)
		return NULL;
	return GetJudgeData(pItem);
}

TrumpSPData * NA_DataTrans::GetTrumpData( PNA_ITEM_DATA pItem )
{
	BYTE byPos = pItem[6];
	if (!_CheckBit(byPos,5))
		return NULL;
	int nSeek = 7+sizeof(NADATA_ItemBase);
	if(_CheckBit(byPos,0)) nSeek+=sizeof(ItemExtendInfo);
	if(_CheckBit(byPos,1)) nSeek+=sizeof(ItemBindInfo);
	if(_CheckBit(byPos,2)) nSeek+=sizeof(ItemExpExtend);
	if(_CheckBit(byPos,3)) nSeek+=sizeof(ExtendData);
	if(_CheckBit(byPos,4)) 
	{
		//nSeek+=sizeof(int);
		int nGemSize = *(int*)((BYTE *)pItem+nSeek);
		nSeek+=(nGemSize+4);
	}
	TrumpSPData* pOut = (TrumpSPData*)(pItem+nSeek);
	return pOut;
}

TrumpSPData * NA_DataTrans::GetTrumpData( PNA_OBJ_DATA pNAObj,int nPos)
{
	PNA_ITEM_DATA pItem = GetNAObjItem(pNAObj,nPos);
	if(!pItem)
		return NULL;
	return GetTrumpData(pItem);
}

ItemPlusUpInfo * NA_DataTrans::GetPlusInfo( PNA_ITEM_DATA pItem )
{
	BYTE byPos = pItem[6];
	if (!_CheckBit(byPos,6))
		return NULL;
	int nSeek = 7+sizeof(NADATA_ItemBase);
	if(_CheckBit(byPos,0)) nSeek+=sizeof(ItemExtendInfo);
	if(_CheckBit(byPos,1)) nSeek+=sizeof(ItemBindInfo);
	if(_CheckBit(byPos,2)) nSeek+=sizeof(ItemExpExtend);
	if(_CheckBit(byPos,3)) nSeek+=sizeof(ExtendData);
	if(_CheckBit(byPos,4)) 
	{
		//nSeek+=sizeof(int);
		int nGemSize = *(int*)((BYTE *)pItem+nSeek);
		nSeek+=(nGemSize+4);
	}
	if(_CheckBit(byPos,5)) nSeek+=sizeof(TrumpSPData);
	ItemPlusUpInfo* pOut = (ItemPlusUpInfo*)(pItem+nSeek);
	return pOut;
}

ItemPlusUpInfo * NA_DataTrans::GetPlusInfo( PNA_OBJ_DATA pNAObj,int nPos)
{
	PNA_ITEM_DATA pItem = GetNAObjItem(pNAObj,nPos);
	if(!pItem)
		return NULL;
	return GetPlusInfo(pItem);
}

bool NA_DataTrans::_CheckBit( const BYTE& byVal,BYTE byPos )
{
	return ((byVal>>byPos)&1);
}

PNA_OBJ_DATA NA_DataTrans::CreatNAObjDataEx( PNA_ITEM_DATA* ppItem,int* pnLen,int nCnt,OUT int& nSize,DWORD dwKey/*=0*/ )
{
	nSize = 11;
	for (int i = 0; i < nCnt; i++)
		nSize+=pnLen[i];

	BYTE* pNAObj = (BYTE*)malloc(nSize);
	BYTE* pCpy = pNAObj;
	*(DWORD*)pCpy = nSize;
	pCpy+=4;//数据块长度
	*(BYTE*)pCpy = NEW_AUCTION_VERSION;
	pCpy+=2;//保留
	*(DWORD*)pCpy = dwKey;
	pCpy+=4;	//编号
	*pCpy = nCnt;
	pCpy++;	//物品数
	for (i = 0; i < nCnt; i++)
	{
		memcpy(pCpy,ppItem[i],pnLen[i]);
		pCpy+=pnLen[i];
	}

	return pNAObj;
}

bool NA_DataTrans::SetNAObjKey( PNA_OBJ_DATA pNAObj,DWORD dwKey )
{
	BYTE* pCtrl = pNAObj;
	pCtrl+=6;
	*(DWORD*)pCtrl = dwKey;
	return true;
}

DWORD NA_DataTrans::GetNAObjKey(PNA_OBJ_DATA pNAObj)
{
	BYTE* pCtrl = pNAObj;
	pCtrl+=6;
	return *(DWORD*)pCtrl;
}

PNA_ITEM_DATA NA_DataTrans::GetNAObjItem( PNA_OBJ_DATA pNAObj, int nPos )
{
	BYTE* pCtrl = pNAObj;
	DWORD dwMaxLen = *(DWORD*)pCtrl;
	pCtrl+=4;
	//if(*(BYTE*)pCtrl != NEW_AUCTION_VERSION)
	//	return NULL;

	pCtrl+=6;
	if(nPos >= *pCtrl)
		return NULL;
	pCtrl++;
	while (nPos--)
	{
		DWORD dwLen = *(DWORD*)pCtrl;
		pCtrl+=dwLen;
	}
	if(pCtrl >= pNAObj+dwMaxLen)
		return NULL;

	return pCtrl;
}

PNA_OBJ_DATA NA_DataTrans::CreatNAObjData( NADATA_ItemInfo** ppItem,int nCnt,OUT int& nSize,DWORD dwKey/*=0*/ )
{
	nSize = 11;
	for (int i = 0; i < nCnt; i++)
	{
		nSize += _GetItemSize(ppItem[i]);
	}

	BYTE* pNAObj = (BYTE*)malloc(nSize);
	BYTE* pCtrl = pNAObj;
	*(DWORD*)pCtrl = nSize;
	pCtrl+=4;//数据块长度
	*(BYTE*)pCtrl = NEW_AUCTION_VERSION;
	pCtrl+=2;//保留
	*(DWORD*)pCtrl = dwKey;
	pCtrl+=4;	//编号
	*pCtrl = nCnt;
	pCtrl++;	//物品数
	for (i = 0; i < nCnt; i++)
	{
		_SetItemData(ppItem[i],pCtrl);
		pCtrl+=ppItem[i]->nTotalSize;
	}

	return pNAObj;
}

int NA_DataTrans::_GetItemSize( NADATA_ItemInfo* pInfo )
{
	int nSize = 0;
	nSize+=sizeof(NADATA_ItemBase)+7;
	if(pInfo->pLvlUp)
	{
		nSize+=sizeof(ItemExtendInfo);
		pInfo->byInfo += 0x1; 
	}
	if(pInfo->pBind)	
	{
		nSize+=sizeof(ItemBindInfo);
		pInfo->byInfo += 0x2;
	}
	if(pInfo->pExp)
	{
		nSize+=sizeof(ItemExpExtend);
		pInfo->byInfo += 0x4;
	}
	if(pInfo->pJudge) 
	{
		nSize+=sizeof(ExtendData);
		pInfo->byInfo+= 0x8;
	}
	if(pInfo->pGems)	
	{
		nSize+=sizeof(int)+pInfo->nGemsSize;
		pInfo->byInfo+= 0x10;
	}
	if(pInfo->pTrump)
	{
		nSize+=sizeof(TrumpSPData);
		pInfo->byInfo+= 0x20;
	}
	if (pInfo->pPlusInfo)
	{
		nSize+=sizeof(ItemPlusUpInfo);
		pInfo->byInfo+= 0x40;
	}
	if(pInfo->pCarve)
	{
		nSize+=sizeof(ItemCarveInfo);
		pInfo->byInfo += 0x80;
	}
	pInfo->nTotalSize = nSize;
	return nSize;
}

bool NA_DataTrans::_SetItemData( NADATA_ItemInfo* pInfo, BYTE* pData )
{
	if(!pInfo->nTotalSize)
		return false;

	BYTE* pCtrl  = pData;
	*(DWORD*)pCtrl = pInfo->nTotalSize;
	pCtrl+=4;//数据长度
	pCtrl++;//保留
	*pCtrl = pInfo->pIBase->m_bType;
	pCtrl++;//数据类型
	memcpy(pCtrl,&pInfo->byInfo,sizeof(BYTE));
	pCtrl++;//属性位
	memcpy(pCtrl,pInfo->pIBase,sizeof(NADATA_ItemBase));
	pCtrl+=sizeof(NADATA_ItemBase);
	if(_CheckBit(pInfo->byInfo,0))
	{
		memcpy(pCtrl,pInfo->pLvlUp,sizeof(ItemExtendInfo));
		pCtrl+=sizeof(ItemExtendInfo);
	}
	if(_CheckBit(pInfo->byInfo,1))
	{
		memcpy(pCtrl,pInfo->pBind,sizeof(ItemBindInfo));
		pCtrl+=sizeof(ItemBindInfo);
	}
	if(_CheckBit(pInfo->byInfo,2))
	{
		memcpy(pCtrl,pInfo->pExp,sizeof(ItemExpExtend));
		pCtrl+=sizeof(ItemExpExtend);
	}
	if(_CheckBit(pInfo->byInfo,3))
	{
		memcpy(pCtrl,pInfo->pJudge,sizeof(ExtendData));
		pCtrl+=sizeof(ExtendData);
	}
	if(_CheckBit(pInfo->byInfo,4))
	{
		memcpy(pCtrl,&pInfo->nGemsSize,sizeof(int));
		pCtrl+=sizeof(int);
		memcpy(pCtrl,pInfo->pGems,pInfo->nGemsSize);
		pCtrl+=pInfo->nGemsSize;
	}
	if(_CheckBit(pInfo->byInfo,5))
	{
		memcpy(pCtrl,pInfo->pTrump,sizeof(TrumpSPData));
		pCtrl+=sizeof(TrumpSPData);
	}
	if (_CheckBit(pInfo->byInfo,6))
	{
		memcpy(pCtrl,pInfo->pPlusInfo,sizeof(ItemPlusUpInfo));
		pCtrl+=sizeof(ItemPlusUpInfo);
	}
	if(_CheckBit(pInfo->byInfo,7))
	{
		memcpy(pCtrl,pInfo->pCarve,sizeof(ItemCarveInfo));
		pCtrl+=sizeof(ItemCarveInfo);
	}

	return true;
}

PNA_PACKET_DATA NA_DataTrans::CreatPacketData( PNA_OBJ_DATA* ppNAObj,int nCnt )
{
	int nSize = 8;
	for (int i = 0; i < nCnt; i++)
	{
		nSize+= *(DWORD*)ppNAObj[i];
	}
	PNA_PACKET_DATA pPack = (BYTE*)malloc(nSize);
	BYTE* pCtrl = pPack;
	*(DWORD*)pCtrl = nSize;
	pCtrl+=4;
	pCtrl+=2;
	*(WORD*)pCtrl = nCnt;
	pCtrl+=2;
	for (i = 0; i < nCnt; i++)
	{
		memcpy(pCtrl,ppNAObj[i],*(DWORD*)ppNAObj[i]);
		pCtrl+=*(DWORD*)ppNAObj[i];
	}
	return pPack;
}

int NA_DataTrans::GetPacketObjCnt( PNA_PACKET_DATA pPackData )
{
	return *(WORD*)(pPackData+6);
}

PNA_OBJ_DATA NA_DataTrans::GetPacketObj( PNA_PACKET_DATA pPackData,int nPos )
{
	DWORD dwSize = *(DWORD*)pPackData;
	BYTE* pCtrl = pPackData;
	pCtrl+=8;
	while (nPos--)
	{
		DWORD dwLen = *(DWORD*)pCtrl;
		pCtrl+=dwLen;
		if(pCtrl>=pPackData+dwSize)
			return NULL;
	}
	return pCtrl;
}