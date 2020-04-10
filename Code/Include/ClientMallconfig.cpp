#include "ClientMallconfig.h"
#include <stdio.h>

CreateSingleton(MallSellConfig);
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
			pSM->bType	= atoi(pIni->ReadString(szTmp,"0"));
			m_pList->Add(pSM);
		}
	}
	pIni->Free();
	return true;
}

SellMallItem* MallSellConfig::GetItem( int nItemID ,BYTE bType)
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		SellMallItem* pSM	= (SellMallItem*)m_pList->Items[i];
		if(pSM->itemID == nItemID && pSM->bType == bType)
			return pSM;
	}
	return NULL;
}