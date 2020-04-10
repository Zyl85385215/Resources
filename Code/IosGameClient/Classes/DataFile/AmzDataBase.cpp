#include "AmzDataBase.h"
#include "ios_amzdata_packet.h"
#include "ItemConfig.h"
#include "Interface.h"
#include "UserData.h"
extern UserData* pChar;
int AmzDataBase::operator[]( BYTE bEnum )
{
	if(bEnum >= bEMax)
		return 0;
	return *(int*)(pbData+4*bEnum);
}
AmzDataBase::AmzDataBase( BYTE bEnumMax,BYTE	bType, int nID )
{
	pbData	= new BYTE[bEnumMax*4];
	memset(pbData,0,bEnumMax*4);
	bEMax	= bEnumMax;
	bModelType	= bType;
	nModelID	= nID;
	bStateFlag	= AMZ_STATE_CHGED;
	nExData	= 0;
}

AmzDataBase::~AmzDataBase()
{
	delete pbData;
}

void AmzDataBase::SetVal( BYTE bEnum, int nVal )
{
	(*(int*)(pbData+4*bEnum))	= nVal;
}

int AmzDataBase::GetVal( BYTE bEnum )
{
	if(bEnum >= bEMax)
		return 0;
	return *(int*)(pbData+4*bEnum);
}
AmzDataManager::AmzDataManager()
{
	for (int i = 0; i < AMZ_DATA_MAX; i++)
	{
		m_pListAmzD[i]		= xnList::Create ();
	}

	m_pListCacha	= xnList::Create ();
	m_pListNew		= xnList::Create ();
}

AmzDataManager::~AmzDataManager()
{
	for (int i = 0; i < AMZ_DATA_MAX; i++)
	{
		while(m_pListAmzD[i]->Count)
		{
			delete (AmzDataBase*)m_pListAmzD[i]->Delete (0);
		}
		m_pListAmzD[i]->Free ();
	}

	while(m_pListCacha->Count)
	{
		delete (AmzDataBase*)m_pListCacha->Delete (0);
	}
	m_pListCacha->Free ();

	m_pListNew->Free ();
}

bool AmzDataManager::PacketProcess( ZPacket* pPacket )
{
	if(pPacket->bCmdGroup	!= AMZDATA)
		return false;

	switch(pPacket->bCmd)
	{
	case S_AMZD_INFO:
		{
			SET_DATA(pData,AMZDATA,S_AMZD_INFO,pPacket);
			AmzDataBase*	pAmzData	= GetAmzData (pData->bMType,pData->nMID);
			if(pAmzData == NULL)
			{
				pAmzData	= AddAmzData (pData->nSize/4, pData->bMType,pData->nMID);				
			}
			else
				pAmzData->bStateFlag	= AMZ_STATE_CHGED;

			if(pAmzData->bStateFlag ==  AMZ_STATE_CHGED&& pAmzData->bModelType ==AMZ_DATA_ITEM)
			{//物品的数量变动提示,临时
				int nOldCnt	= pAmzData->GetVal(ITEM_AD_NUM);
				memcpy(pAmzData->pbData,pData->pbData,pData->nSize);
				int nNewCnt	= pAmzData->GetVal(ITEM_AD_NUM);
// 				if(nNewCnt > nOldCnt)
// 				{
// 					ItemExBase* pBase	= ItemConfig::GetInstance().GetItemBase(pAmzData->GetVal(ITEM_AD_TYPE));
// 					if(pBase)
// 						pMainToopTip->AddGetTipObj(pBase->szName,pBase->szDesc,pBase->nSpr,nNewCnt-nOldCnt,3);
// 				}
			}
			else
				memcpy(pAmzData->pbData,pData->pbData,pData->nSize);
		}
		return true;
	case S_AMZD_SIMPLE:
		{
			SET_DATA(pData,AMZDATA,S_AMZD_SIMPLE,pPacket);
			AmzDataBase*	pAmzData	= GetAmzData (pData->bMType,pData->nMID);
			if(pAmzData)
			{
				pAmzData->bStateFlag	= AMZ_STATE_CHGED;
				for (int i = 0; i < pData->bCnt; i++)
				{
					pAmzData->SetVal (pData->pSData[i].bPos,pData->pSData[i].nVal);
				}
			}
		}
		return true;
	case S_AMZD_DELETE:
		{
			SET_DATA(pData,AMZDATA,S_AMZD_DELETE,pPacket);
			RemoveAmzData (pData->bMType,pData->nMID);
		}
		return true;
	}

	return false;
}

AmzDataBase* AmzDataManager::GetAmzData( BYTE bMT, int nMID )
{
	if(bMT >= AMZ_DATA_MAX)
		return NULL;
	for (int i = 0; i < m_pListAmzD[bMT]->Count; i++)
	{
		AmzDataBase* pData	= (AmzDataBase*)m_pListAmzD[bMT]->Items[i];
		if(pData->nModelID	== nMID && pData->bStateFlag != AMZ_STATE_DELETE)
			return pData;
	}

	return NULL;
}

void AmzDataManager::Update()
{
	for (int i = m_pListCacha->Count-1; i >= 0; i--)
	{
		AmzDataBase* pData	= (AmzDataBase*)m_pListCacha->Items[i];
		if(pData->bStateFlag == AMZ_STATE_RELEASE)
		{
			m_pListCacha->Delete (i);
			delete pData;
		}
	}
}

AmzDataBase* AmzDataManager::PopAmzData( BYTE bMT )
{
	for (int i = 0; i < m_pListNew->Count; i++)
	{
		AmzDataBase* pData	= (AmzDataBase*)m_pListNew->Items[i];
		if(pData->bModelType	== bMT)
			return (AmzDataBase*)m_pListNew->Delete (i);
	}
	return NULL;
}

AmzDataBase* AmzDataManager::AddAmzData(BYTE bEMax, BYTE bMT, int nMID )
{
	if(bMT >= AMZ_DATA_MAX)
		return NULL;
	AmzDataBase* pAmzData	= GetAmzData (bMT,nMID);
	if(pAmzData == NULL)
	{
		//if (bMT == AMZ_DATA_CARD)
		{
			CCLOG("add ship:%d",nMID);
		}
		pAmzData	= new AmzDataBase(bEMax,bMT,nMID);
		m_pListNew->Add (pAmzData);
		m_pListAmzD[bMT]->Add (pAmzData);
		pAmzData->bStateFlag	= AMZ_STATE_NEW;
	}
	else if(pAmzData->bStateFlag != AMZ_STATE_NEW)
		pAmzData->bStateFlag	= AMZ_STATE_CHGED;
	return pAmzData;
}

bool AmzDataManager::RemoveAmzData( BYTE bMT, int nMID,bool bRelease )
{
	if(bMT >= AMZ_DATA_MAX)
		return false;
	for (int i = 0; i < m_pListAmzD[bMT]->Count; i++)
	{
		AmzDataBase* pData	= (AmzDataBase*)m_pListAmzD[bMT]->Items[i];
		if(pData->nModelID	== nMID)
		{
			m_pListAmzD[bMT]->Delete (i);
			m_pListNew->Remove (pData);

			if(!bRelease)
			{
				pData->bStateFlag	= AMZ_STATE_DELETE;
			}
			else
				pData->bStateFlag	= AMZ_STATE_RELEASE;
			m_pListCacha->Add (pData);
			

			return true;
		}
	}
	return false;
}

int AmzDataManager::GetNewMID( BYTE bMT )
{
	if(bMT >= AMZ_DATA_MAX)
		return -1;
	int nID = 0;
	for (int i = 0; i < m_pListAmzD[bMT]->Count; i++)
	{
		AmzDataBase* pData	= (AmzDataBase*)m_pListAmzD[bMT]->Items[i];
		if (pData->nModelID>nID)
		{
			nID = pData->nModelID;
		}
	}
	return nID+1;
}

AmzDataBase* AmzDataManager::CreateAmzData( BYTE bEMax,BYTE bMT,char* pData )
{
	AmzDataBase* pBase = new AmzDataBase(bEMax,bMT, 0);
	memcpy(pBase->pbData,pData,bEMax*sizeof(int));
	return pBase;
}

void AmzDataManager::ClearData()
{
	for (int i = 0; i < AMZ_DATA_MAX; i++)
	{
		while(m_pListAmzD[i]->Count)
		{
			delete (AmzDataBase*)m_pListAmzD[i]->Delete (0);
		}
	}

	while(m_pListCacha->Count)
	{
		delete (AmzDataBase*)m_pListCacha->Delete (0);
	}
	m_pListNew->Clear();
}
