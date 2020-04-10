#include "stdafx.h"
#include "AmzDataServer.h"
#include "ios_amzdata_packet.h"
#include "DBControl.h"
#include "ZNetGamesvr.h"
#include "EquipConfig.h"
#include "ItemConfig.h"

char*	g_szDbName0[]	= {"uin","id","baseid"};
char*	g_szDbName1[]	= {"uin","id","baseid","num","slot","pos","limittime","bind"};
char*	g_szDbName2[]	= {"id","uin","itemid","lvl","addtype1","addrandom1","addtype2","addrandom2","stonepos","stone12","stone34","eatlv"};
char*	g_szTableName[]	= {RPG_SLAVE_TABLE,RPG_ITEM_TABLE,RPG_EQUIPEX_TABLE};
int		g_sznValCnt[]	= {3,ITEM_AD_MAX,EQUIPEX_AD_MAX};

//CARD_AD_MAX

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
	nExData		= 0;
	bStateFlag	= AMZ_STATE_CHGED;
	nLinkCnt	= 0;
}

AmzDataBase::~AmzDataBase()
{
	delete[] pbData;
}

void AmzDataBase::SetVal( BYTE bEnum, int nVal,bool bSaveDB )
{
	(*(int*)(pbData+4*bEnum))	= nVal;
	if(bStateFlag == AMZ_STATE_NORMAL)
		bStateFlag	= bSaveDB?AMZ_STATE_CHGED:AMZ_STATE_CHGED_NOTSAVEDB;
	else if (bStateFlag == AMZ_STATE_CHGED_NOTSAVEDB&&bSaveDB)
	{
		bStateFlag = AMZ_STATE_CHGED;
	}
}

int AmzDataBase::GetVal( BYTE bEnum )
{
	if(bEnum >= bEMax)
		return 0;
	return *(int*)(pbData+4*bEnum);
}

AmzDataManager::AmzDataManager(DWORD dwUin, DBControl* pDB)
{
	for (int i = 0; i < AMZ_DATA_MAX; i++)
	{
		m_pListAmzD[i]		= xnList::Create ();
	}

	m_pListCacha	= xnList::Create ();

	m_dwUin		= dwUin;
	m_pDB	= pDB;
}

AmzDataManager::~AmzDataManager()
{
	for (int i = 0; i < AMZ_DATA_MAX; i++)
	{
		while(m_pListAmzD[i]->Count)
		{
			AmzDataBase*	pADB	= (AmzDataBase*)m_pListAmzD[i]->Delete (0);
			pADB->nLinkCnt--;
			if(pADB->nLinkCnt <= 0)
				delete pADB;
		}
		m_pListAmzD[i]->Free ();
	}

	while(m_pListCacha->Count)
	{
		delete (AmzDataBase*)m_pListCacha->Delete (0);
	}
	m_pListCacha->Free ();
}

int AmzDataManager::GetAmzTCnt( BYTE bMT )
{
	if(bMT >= AMZ_DATA_MAX)
		return 0;
	return m_pListAmzD[bMT]->Count;
}
AmzDataBase* AmzDataManager::GetAmzData( BYTE bMT, int nMID )
{
	if(bMT >= AMZ_DATA_MAX)
		return NULL;
	for (int i = 0; i < m_pListAmzD[bMT]->Count; i++)
	{
		AmzDataBase* pData	= (AmzDataBase*)m_pListAmzD[bMT]->Items[i];
		if(pData->nModelID	== nMID && pData->bStateFlag != AMZ_STATE_DELETE && pData->bStateFlag != AMZ_STATE_RELEASE)
			return pData;
	}

	return NULL;
}

void AmzDataManager::Update()
{
	for(int n = 0; n < AMZ_DATA_MAX; n++)
	{
		for (int i = 0; i < m_pListAmzD[n]->Count; i++)
		{
			AmzDataBase* pAmzData	= (AmzDataBase*)m_pListAmzD[n]->Items[i];
			switch(pAmzData->bStateFlag)
			{
			case AMZ_STATE_DBGET:
				{
					BuildPacketEx(pPacket,AMZDATA,S_AMZD_INFO,buf,256);	
					SET_DATA(pSend,AMZDATA,S_AMZD_INFO,pPacket);
					pSend->bMType	= pAmzData->bModelType;
					pSend->nMID		= pAmzData->nModelID;
					pSend->nSize	= pAmzData->bEMax*4;
					memcpy(pSend->pbData,pAmzData->pbData,pSend->nSize);
					pPacket->wSize	+= pSend->nSize;
					TCP_SendPacketToUin(m_dwUin,pPacket);

					pAmzData->bStateFlag	= AMZ_STATE_NORMAL;
				}
				break;
			case AMZ_STATE_CHGED:
				{
					BuildPacketEx(pPacket,AMZDATA,S_AMZD_INFO,buf,256);	
					SET_DATA(pSend,AMZDATA,S_AMZD_INFO,pPacket);
					pSend->bMType	= pAmzData->bModelType;
					pSend->nMID		= pAmzData->nModelID;
					pSend->nSize	= pAmzData->bEMax*4;
					memcpy(pSend->pbData,pAmzData->pbData,pSend->nSize);
					pPacket->wSize	+= pSend->nSize;
					TCP_SendPacketToUin(m_dwUin,pPacket);

					pAmzData->bStateFlag	= AMZ_STATE_NORMAL;
					
					_saveAmzDB(pAmzData);
					//m_pDB->ExecuteSQL(false,);
				}
				break;
			case AMZ_STATE_CREATE:
				{
					BuildPacketEx(pPacket,AMZDATA,S_AMZD_INFO,buf,1024);	
					SET_DATA(pSend,AMZDATA,S_AMZD_INFO,pPacket);
					pSend->bMType	= pAmzData->bModelType;
					pSend->nMID		= pAmzData->nModelID;
					pSend->nSize	= pAmzData->bEMax*4;
					memcpy(pSend->pbData,pAmzData->pbData,pSend->nSize);
					pPacket->wSize	+= pSend->nSize;
					TCP_SendPacketToUin(m_dwUin,pPacket);

					pAmzData->bStateFlag	= AMZ_STATE_NORMAL;
					_insertAmzDB(pAmzData);
				}
				break;
			case AMZ_STATE_CHGED_NOTSAVEDB:
				{
					BuildPacketEx(pPacket,AMZDATA,S_AMZD_INFO,buf,256);	
					SET_DATA(pSend,AMZDATA,S_AMZD_INFO,pPacket);
					pSend->bMType	= pAmzData->bModelType;
					pSend->nMID		= pAmzData->nModelID;
					pSend->nSize	= pAmzData->bEMax*4;
					memcpy(pSend->pbData,pAmzData->pbData,pSend->nSize);
					pPacket->wSize	+= pSend->nSize;
					TCP_SendPacketToUin(m_dwUin,pPacket);

					pAmzData->bStateFlag	= AMZ_STATE_NORMAL;
				}
				break;
			}
		}
	}

	for (int i = m_pListCacha->Count-1; i >= 0; i--)
	{
		AmzDataBase* pAmzData	= (AmzDataBase*)m_pListCacha->Items[i];
		switch(pAmzData->bStateFlag)
		{
		case AMZ_STATE_DELETE:
		case AMZ_STATE_RELEASE:
			{
				BuildPacketEx(pPacket,AMZDATA,S_AMZD_DELETE,buf,256);	
				SET_DATA(pSend,AMZDATA,S_AMZD_DELETE,pPacket);
				pSend->bMType	= pAmzData->bModelType;
				pSend->nMID		= pAmzData->nModelID;
				TCP_SendPacketToUin(m_dwUin,pPacket);
				_removeAmzDB(pAmzData);
				m_pListCacha->Delete (i);
				if(pAmzData->nLinkCnt <= 0)
					delete pAmzData;
			}
			break;
		}
	}
}

AmzDataBase* AmzDataManager::AddAmzData(BYTE bEMax, BYTE bMT, int nMID )
{
	if(bMT >= AMZ_DATA_MAX)
		return NULL;
	AmzDataBase* pAmzData	= GetAmzData (bMT,nMID);
	if(pAmzData == NULL)
	{
		pAmzData	= new AmzDataBase(bEMax,bMT,nMID);
		pAmzData->nLinkCnt++;
		m_pListAmzD[bMT]->Add (pAmzData);
		pAmzData->bStateFlag	= AMZ_STATE_CREATE;
	}
	return pAmzData;
}

bool AmzDataManager::AddAmzData( AmzDataBase* pBase )
{
	if(!pBase){
		return false;
	}
	if(m_pListAmzD[pBase->bModelType]->IndexOf(pBase) != -1)
		return false;
	pBase->nLinkCnt++;
	m_pListAmzD[pBase->bModelType]->Add (pBase);
	pBase->bStateFlag	= AMZ_STATE_CREATE;
	return true;
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
			pData->nLinkCnt--;
			m_pListAmzD[bMT]->Delete (i);

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

#define LINK_DBNAME(a)	\
	g_szDbName##a;

void AmzDataManager::DBInit()
{
	for (int i = 0; i < AMZ_DATA_MAX; i++)
	{
		QueryRlt xRlt;
		
		if(!m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM %s WHERE uin = %d",g_szTableName[i] ,m_dwUin))  //and Slot not in (%d,%d,%d,%d)
			continue;
		
		MySqlQuery* pCmd = xRlt.GetRlt();
		if (pCmd != NULL)
		{
			char**	ppTableName	= _getDBValsName(i);

			while(pCmd->FetchRow())
			{	
				AmzDataBase* pAmzD	= AddAmzData(g_sznValCnt[i],i,pCmd->GetInt("id"));
				if(!pAmzD)
					continue;
				pAmzD->bStateFlag	= AMZ_STATE_DBGET;
				pAmzD->llKey	= _atoi64(pCmd->GetStr("objkey"));
				if(pAmzD->llKey == 0)
				{
					LONGLONG temp = m_dwUin;
					pAmzD->llKey	= (temp<< 32 | pAmzD->nModelID);

					m_pDB->ExecuteSQL(FALSE,"update %s set objkey=%I64d where uin=%d and id=%d",g_szTableName[i],pAmzD->llKey,m_dwUin,pAmzD->nModelID);
				}
				for (int n = 0; n < g_sznValCnt[i]; n++)
				{
					pAmzD->SetVal(n,pCmd->GetInt(ppTableName[n]));
				}
			}
		}
		continue;
	}
}

void AmzDataManager::_saveAmzDB( AmzDataBase* pData )
{
	char	szSql[2048];
	sprintf(szSql,"update %s set ",g_szTableName[pData->bModelType]);

	char**	ppTableName	= _getDBValsName(pData->bModelType);
	for (int i = 0; i < g_sznValCnt[pData->bModelType]; i++)
	{
		if(i != 0)
			strcat(szSql,",");
		sprintf(szSql+strlen(szSql),"%s=%d ",ppTableName[i],pData->GetVal(i));
	}
	sprintf(szSql+strlen(szSql),"where uin=%d and id=%d",m_dwUin,pData->nModelID);

	m_pDB->ExecuteSQL(FALSE,szSql);
}

void AmzDataManager::_insertAmzDB( AmzDataBase* pData )
{
	char	szSql[2048];
	sprintf(szSql,"insert into %s (",g_szTableName[pData->bModelType]);
	char**	ppTableName	= _getDBValsName(pData->bModelType);
	for (int i = 0; i < g_sznValCnt[pData->bModelType]; i++)
	{
		if(i != 0)
			strcat(szSql,",");
		strcat(szSql,ppTableName[i]);
	}
	strcat(szSql,",objkey) VALUE (");
	for (int i = 0; i < g_sznValCnt[pData->bModelType]; i++)
	{
		if(i != 0)
			strcat(szSql,",");
		sprintf(szSql+strlen(szSql),"%d",pData->GetVal(i));
	}
	sprintf(szSql+strlen(szSql),",%I64d)",pData->llKey);
	m_pDB->ExecuteSQL(FALSE,szSql);
}

void AmzDataManager::_removeAmzDB( AmzDataBase* pData )
{
	m_pDB->ExecuteSQL(FALSE,"delete from %s where uin=%d and id=%d",g_szTableName[pData->bModelType],m_dwUin,pData->nModelID);
}

char** AmzDataManager::_getDBValsName( BYTE bMType )
{
	switch(bMType)
	{
	case 0:
		return LINK_DBNAME(0);
	case 1:
		return LINK_DBNAME(1);
	case 2:
		return LINK_DBNAME(2);
	}

	return NULL;
}

AmzDataBase* AmzDataManager::GetAmzDataByPos( BYTE bMT, int nCount )
{
	if(bMT >= AMZ_DATA_MAX)
		return NULL;
	if (nCount >= m_pListAmzD[bMT]->Count)
	{
		return NULL;
	}
	AmzDataBase* pData	= (AmzDataBase*)m_pListAmzD[bMT]->Items[nCount];
	return pData;
}

AmzDataBase* AmzDataManager::CreateAmzData( BYTE bEMax,BYTE bMT,char* pData )
{
	AmzDataBase* pBase = new AmzDataBase(bEMax,bMT, 0);
	if (pData)
	{
		memcpy(pBase->pbData,pData,bEMax*sizeof(int));
	}
	return pBase;
}

void AmzDataManager::SaveLeave()
{
	for (int i = m_pListCacha->Count-1; i >= 0; i--)
	{
		AmzDataBase* pAmzData	= (AmzDataBase*)m_pListCacha->Items[i];
		switch(pAmzData->bStateFlag)
		{
		case AMZ_STATE_DELETE:
		case AMZ_STATE_RELEASE:
			{
				_removeAmzDB(pAmzData);
			}
			break;
		}
	}
}
