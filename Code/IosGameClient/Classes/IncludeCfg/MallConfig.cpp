
#include "stdafx.h"
#ifdef _ISSERVER
#include "stdio.h"
#define SAFE_FREE(p) { if(p) { free(p); p=NULL; } }
#endif
#ifndef _ISSERVER
#include "UserData.h"
#endif
#include "MallConfig.h"
#include "xnExtstr.h"
#include "ios_item_packet.h"

#include <string>
#include <list>
#ifndef _ISSERVER
extern UserData*	pChar;
#endif
CreateSingleton(MallConfig);
CreateSingleton(ShopConfig);
MallConfig::MallConfig()
{
	m_pItemList = xnList::Create();
}

MallConfig::~MallConfig()
{
	delete (MallItemCfg*)m_pItemList->Delete(0);
}

void MallConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	char sztxt[32];

	int index = 0;
	while (pStr->SetSection(index++) != NULL)
	{
		int nCnt = atoi (pStr->ReadString ("itemcount","0"));
		for (int i=0;i<nCnt;i++)
		{
			MallItemCfg* pCfg = new MallItemCfg;
			pCfg->m_byType = index-1;
			sprintf(sztxt,"itemdiscount%d",i+1);
			pCfg->m_nDiscount = atoi (pStr->ReadString (sztxt,"100"));
			sprintf(sztxt,"itemid%d",i+1);
			pCfg->m_nItemID = atoi (pStr->ReadString (sztxt,"0"));
			sprintf(sztxt,"itemprice%d",i+1);
			pCfg->m_nPrice = atoi (pStr->ReadString (sztxt,"100"));
			sprintf(sztxt,"limit%d",i+1);
			pCfg->m_nLimit = atoi (pStr->ReadString (sztxt,"0"));
			sprintf(sztxt,"flag%d",i+1);
			pCfg->m_nFlag = atoi (pStr->ReadString (sztxt,"-1"));
			m_pItemList->Add(pCfg);
		}
	}
	pStr->Free();
}

MallItemCfg* MallConfig::GetCfg( BYTE byType,int nItemID )
{
#ifndef _ISSERVER
	for (int i=0;i<pChar->m_pItemCenter->m_pMallList->Count;i++)
	{
		MallItemCfg* pItem = (MallItemCfg*)pChar->m_pItemCenter->m_pMallList->Items[i];
		if (pItem->m_byType == byType&&pItem->m_nItemID == nItemID)
		{
			return pItem;
		}
	}
	return NULL;
#endif
	for (int i=0;i<m_pItemList->Count;i++)
	{
		MallItemCfg* pItem = (MallItemCfg*)m_pItemList->Items[i];
		if (pItem->m_byType == byType&&pItem->m_nItemID == nItemID)
		{
			return pItem;
		}
	}
	return NULL;
}

MallItemCfg* MallConfig::GetCfg( int nItemID )
{
#ifndef _ISSERVER
	for (int i=0;i<pChar->m_pItemCenter->m_pMallList->Count;i++)
	{
		MallItemCfg* pItem = (MallItemCfg*)pChar->m_pItemCenter->m_pMallList->Items[i];
		if (pItem->m_nItemID == nItemID)
		{
			return pItem;
		}
	}
	return NULL;
#endif
	for (int i=0;i<m_pItemList->Count;i++)
	{
		MallItemCfg* pItem = (MallItemCfg*)m_pItemList->Items[i];
		if (pItem->m_nItemID == nItemID)
		{
			return pItem;
		}
	}
	return NULL;
}

void MallConfig::GetMallItemByType( xnList* pList,BYTE byType )
{
	for (int i=0;i<m_pItemList->Count;i++)
	{
		MallItemCfg* pItem = (MallItemCfg*)m_pItemList->Items[i];
		if (pItem->m_byType == byType)
		{
			pList->Add(pItem);
		}
	}
}

ShopConfig::ShopConfig()
{
	m_pItemList = xnList::Create();
}

ShopConfig::~ShopConfig()
{
	delete (ShopInfoCfg*)m_pItemList->Delete(0);
}

void ShopConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	char sztxt[32];

	int index = 0;
	while (pStr->SetSection(index++) != NULL)
	{
		ShopInfoCfg* pInfo = new ShopInfoCfg;
		pInfo->nType = atoi(pStr->sectionName);
		m_pItemList->Add(pInfo);
		int nCnt = atoi (pStr->ReadString ("itemcount","0"));
		for (int i=0;i<nCnt;i++)
		{
			ShopItemCfg* pCfg = new ShopItemCfg;
			pCfg->m_nTypeInfo = pInfo->nType;
			sprintf(sztxt,"itemreflushtime%d",i+1);
			pCfg->m_byReflush = atoi (pStr->ReadString (sztxt,"0"));
			sprintf(sztxt,"itemid%d",i+1);
			pCfg->m_nItemID = atoi (pStr->ReadString (sztxt,"0"));
			sprintf(sztxt,"itemprice%d",i+1);
			pCfg->m_nPrice = atoi (pStr->ReadString (sztxt,"100"));
			sprintf(sztxt,"itemlimit%d",i+1);
			pCfg->m_nLimit = atoi (pStr->ReadString (sztxt,"0"));
			sprintf(sztxt,"itemcost%d",i+1);
			pCfg->m_byType = atoi (pStr->ReadString (sztxt,"0"));
			pInfo->pItemList->Add(pCfg);
		}
	}
	pStr->Free();
}

ShopInfoCfg* ShopConfig::GetCfg( int nType )
{
	for (int i=0;i<m_pItemList->Count;i++)
	{
		ShopInfoCfg* pItem = (ShopInfoCfg*)m_pItemList->Items[i];
		if (pItem->nType == nType)
		{
			return pItem;
		}
	}
	return NULL;
}

ShopItemCfg* ShopConfig::GetCfg( int nType,int nItemID )
{
	ShopInfoCfg* pCfg = GetCfg(nType);
	if (pCfg)
	{
		return pCfg->GetItemCfg(nItemID);
	}
	return NULL;
}

ShopInfoCfg::ShopInfoCfg()
{
	pItemList = xnList::Create();
}

ShopInfoCfg::~ShopInfoCfg()
{
	delete (ShopItemCfg*)pItemList->Delete(0);
}

ShopItemCfg* ShopInfoCfg::GetItemCfg( int nItemID )
{
	for (int i=0;i<pItemList->Count;i++)
	{
		ShopItemCfg* pItem = (ShopItemCfg*)pItemList->Items[i];
		if (pItem->m_nItemID == nItemID)
		{
			return pItem;
		}
	}
	return NULL;
}
