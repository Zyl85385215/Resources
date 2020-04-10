#include "mallconfig.h"
#include "player_packet.h"
#include "netfunc.h"
#include "GameWorld.h"

CreateSingleton(CMallConfig);
CreateSingleton(MallSellConfig);

CMallShopItemList::CMallShopItemList( ItemBase * pItemBase )
{
	m_nVersion = 0;
	m_pItemBase = pItemBase;
}

CMallShopItemList::~CMallShopItemList( void )
{
	std::list<MallShopItemData*>::iterator iterator = m_listMallShopItemData.begin();
	for(; iterator != m_listMallShopItemData.end(); iterator++)
	{
		MallShopItemData * pMallShopItemData = *iterator;

		ItemBaseData * pItemBaseData = m_pItemBase->GetItemData(pMallShopItemData->itemID);
		if(pItemBaseData != NULL) pItemBaseData->byGoldType = TRADE_ORDINORY;

		SAFE_DELETE(pMallShopItemData);
	}
	m_listMallShopItemData.clear();
	m_mapMallShopItemData.clear();
}

bool CMallShopItemList::Load( const char * szFile )
{
	xnExtstr * pIni = xnExtstr::Import((char*)szFile);
	if(!pIni) return false;

	pIni->SetSection("Version");
	m_nVersion = atoi(pIni->ReadString("Version","0"));

	for(int i=0; i<MALL_MAX; i++)
	{
		char szMallName[64] = {0};
		sprintf(szMallName, "%d", i);
		if(pIni->SetSection(szMallName)) _LoadShopItem(pIni, i);
	}
	pIni->Free();
	return true;
}

bool CMallShopItemList::GetItem( int nItemID, BYTE goldType, MallShopItemData & xMallShopItemData )
{
	std::map<MallShopId, MallShopItemData*>::iterator iterator = m_mapMallShopItemData.find(MallShopId(nItemID, goldType));
	if(iterator != m_mapMallShopItemData.end())
	{
		MallShopItemData * pMallShopItemData = iterator->second;
		xMallShopItemData = *pMallShopItemData;
		return true;
	}
	return false;
}

void CMallShopItemList::SendMallShopItems( DWORD dwUin )
{
	if (m_mapMallShopItemData.size() <= 0) return;

	char buffer[1024] = {0};
	PLAYER_SENDMALL_ITEM_Data * pSendData = (PLAYER_SENDMALL_ITEM_Data*)((Packet*)buffer)->data;
	pSendData->bFirst = true;
	MallItem * pMallItem = pSendData->items;
	int n = 0;
	int i = 0;

	std::list<MallShopItemData*>::iterator iterator = m_listMallShopItemData.begin();
	for(; iterator != m_listMallShopItemData.end(); iterator++)
	{
		MallShopItemData * pItem = *iterator;
		if (pItem != NULL)
		{
			pMallItem[n].ItemID = pItem->itemID;
			pMallItem[n].Price = pItem->price;
			pMallItem[n].Type = pItem->type;
			pMallItem[n].byGoldType = pItem->bGoldType;
			pMallItem[n].bCanSell = pItem->bCanSell;
			pMallItem[n].bCanGive = pItem->bCanGive;
			pMallItem[n].wLimitCnt = pItem->wLimitCnt;
			i++;
			n++;
			if ( (i % 30) == 0 || i == m_listMallShopItemData.size())
			{
				if (i>30)
					pSendData->bFirst = false;
				Packet * pPacket = NULL;
				pSendData->count = n;
				pSendData->dwVersion = m_nVersion;
				BuildPacketSize(pPacket, PLAYER, SENDMALL_ITEM, buffer, sizeof(PLAYER_SENDMALL_ITEM_Data) + sizeof(MallItem) * n);
				TCP_SendPacketToUin(dwUin, pPacket);
				n = 0;
				if (i == m_listMallShopItemData.size()) break;
			}
		}
	}
	
}

void CMallShopItemList::_LoadShopItem( xnExtstr * pExtstr, int nType )
{
	int cnt = atoi(pExtstr->ReadString("Count","0"));
	int GoldType = atoi(pExtstr->ReadString("GoldType","0"));
	for(int i=0;i<cnt;i++)
	{
		char str[128] = {0};
		MallShopItemData * pItem = new MallShopItemData;

		pItem->bGoldType = GoldType;
		pItem->type = nType;
		sprintf(str,"itemID%d",i+1);
		pItem->itemID = atoi(pExtstr->ReadString(str, "0"));
		sprintf(str,"Price%d",i+1);
		pItem->price = atoi(pExtstr->ReadString(str, "0"));
		sprintf(str,"CanSell%d",i+1);
		pItem->bCanSell = static_cast<bool>(atoi(pExtstr->ReadString(str, "1")));
		sprintf(str,"CanGive%d",i+1);
		pItem->bCanGive = static_cast<bool>(atoi(pExtstr->ReadString(str, "0")));
		sprintf(str,"LimitCnt%d",i+1);
		pItem->wLimitCnt = static_cast<WORD>(atoi(pExtstr->ReadString(str, "0")));
		sprintf(str, "GoldType%d", i+1);
		int nGoldTypeEx = atoi(pExtstr->ReadString(str, "99"));
		if(nGoldTypeEx != 99)
			pItem->bGoldType	= nGoldTypeEx;

		m_listMallShopItemData.push_back(pItem);
		m_mapMallShopItemData[MallShopId(pItem->itemID, pItem->bGoldType)] = pItem;
	}
}

void CMallShopItemList::UpdateItemBase( bool bSave /*= true*/ )
{
	std::list<MallShopItemData*>::iterator iterator = m_listMallShopItemData.begin();
	for(; iterator != m_listMallShopItemData.end(); iterator++)
	{
		MallShopItemData * pItem = *iterator;
		if(!pItem) continue;

		ItemBaseData * pItemBaseData = m_pItemBase->GetItemData(pItem->itemID);
		if(pItemBaseData != NULL && bSave)
		{
			pItemBaseData->byGoldType = TRADE_ORDINORY;
		}
	}
}

//////////////////////////////////////////////////////////////////////////

CMallConfig::CMallConfig( void )
{
	m_nLastVersion = 0;
	m_pItemBase = 0;
	InitializeCriticalSection(&m_xMallConfigCS);
}

CMallConfig::~CMallConfig( void )
{
	_Clear();
	DeleteCriticalSection(&m_xMallConfigCS);
}

void CMallConfig::Init( ItemBase * pItemBase )
{
	m_pItemBase = pItemBase;
}

bool CMallConfig::Load( const char * szFile )
{
	CMallShopItemList * pLastMallShopItemList = 0;
	CMallShopItemList * pMallShopItemList = new CMallShopItemList(m_pItemBase);
	if(!pMallShopItemList->Load(szFile)) return false;

	EnterCriticalSection(&m_xMallConfigCS);
	pLastMallShopItemList = m_listMallShopItemList.size() > 0 ? m_listMallShopItemList.back() : 0;
	LeaveCriticalSection(&m_xMallConfigCS);

	if(pLastMallShopItemList && pLastMallShopItemList->GetVersion() == pMallShopItemList->GetVersion())
	{
		SAFE_DELETE(pMallShopItemList);
		return false;
	}

	EnterCriticalSection(&m_xMallConfigCS);
	m_nLastVersion = pMallShopItemList->GetVersion();
	m_listMallShopItemList.push_back(pMallShopItemList);
	if(pLastMallShopItemList) pLastMallShopItemList->UpdateItemBase(false);
	pMallShopItemList->UpdateItemBase(true);
	LeaveCriticalSection(&m_xMallConfigCS);

	OutMessage(msg_type_normal, "mall config fresh successfully!\n");
	return true;
}

bool CMallConfig::GetItem( int nVersion, int nItemID, BYTE goldType, MallShopItemData & xMallShopItemData )
{
	CMallShopItemList * pMallShopItemList = 0;

	EnterCriticalSection(&m_xMallConfigCS);
	pMallShopItemList = m_listMallShopItemList.back();
	LeaveCriticalSection(&m_xMallConfigCS);

	if(pMallShopItemList && (!nVersion || pMallShopItemList->GetVersion() == nVersion))
		return pMallShopItemList->GetItem(nItemID, goldType, xMallShopItemData);
	return false;
}

void CMallConfig::_Clear( void )
{
	EnterCriticalSection(&m_xMallConfigCS);

	std::list<CMallShopItemList*>::iterator iterator = m_listMallShopItemList.begin();
	for(; iterator != m_listMallShopItemList.end(); iterator++)
	{
		CMallShopItemList * pMallShopItemList = *iterator;
		SAFE_DELETE(pMallShopItemList);
	}
	m_listMallShopItemList.clear();
	m_nLastVersion = 0;

	LeaveCriticalSection(&m_xMallConfigCS);
}

void CMallConfig::SendMallShopItems( DWORD dwUin ,int type)
{
	CMallShopItemList * pMallShopItemList = 0;

	EnterCriticalSection(&m_xMallConfigCS);
	pMallShopItemList = m_listMallShopItemList.back();
	LeaveCriticalSection(&m_xMallConfigCS);

	if(pMallShopItemList)
	{
		pMallShopItemList->SendMallShopItems(dwUin);
		if (!type)
		{
			TCP_SendCmdToUin(dwUin, PLAYER,SENDMALL_OVER);
		}else{
			TCP_SendCmdToUin(dwUin, PLAYER,SENDMALLDAOJU_OVER);
		}
		
	}
}

MallSellConfig::MallSellConfig()
{
	m_pList	= xnList::Create();
}

MallSellConfig::~MallSellConfig()
{
	while(m_pList->Count)
	{
		delete (SellMallItem*)m_pList->Delete(0);
	}
	m_pList->Free();
}

bool MallSellConfig::Load( const char * szFile )
{
	xnExtstr * pIni = xnExtstr::Import((char*)szFile);
	if(!pIni) return false;

	if(pIni->SetSection(0))
	{
		int nCnt	= atoi(pIni->ReadString("cnt",0));
		char szTmp[256] = {0};
		for (int i = 0; i < nCnt; i++)
		{
			SellMallItem* pSM	= new SellMallItem();
			sprintf(szTmp,"ItemID%d",i+1);
			pSM->itemID	= atoi(pIni->ReadString(szTmp,"0"));
			sprintf(szTmp,"price%d",i+1);
			pSM->price	= atoi(pIni->ReadString(szTmp,"0"));
			sprintf(szTmp,"bind%d",i+1);
			pSM->bBindType	= atoi(pIni->ReadString(szTmp,"10"));
			sprintf(szTmp,"type%d",i+1);
			pSM->nSellCnt	= atoi(pIni->ReadString(szTmp,"0"));
			m_pList->Add(pSM);
		}
	}
	return true;
}

SellMallItem* MallSellConfig::GetItem( int nItemID )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		SellMallItem* pSM	= (SellMallItem*)m_pList->Items[i];
		if(pSM->itemID == nItemID )
			return pSM;
	}
	return NULL;
}