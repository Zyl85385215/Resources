#include "stdafx.h"
#include "UserFlag.h"
#include "xnExtstr.h"
#include "UserData.h"
#include "ZNetPacket.h"
#include "MallConfig.h"
#include "ItemConfig.h"
CreateSingleton(UFlagConfig);

UFlagConfig::UFlagConfig()
{
	m_pList		= xnList::Create();
}

UFlagConfig::~UFlagConfig()
{
	while(m_pList->Count)
		delete (UFlagObj*)m_pList->Delete(0);
	m_pList->Free();
}

void UFlagConfig::LoadFile( char* szFile )
{
	xnExtstr* pStr = xnExtstr::Import(szFile);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	m_nCurOffPos	= 0;
	while(pStr->SetSection(nIndex ++))
	{
		UFlagObj* pFlag = new UFlagObj();
		pFlag->nId = atoi(pStr->sectionName);
		pFlag->bType	= atoi(pStr->ReadString("type","0"));
		pFlag->bResetType	= atoi(pStr->ReadString("reset","0"));
		pFlag->bSyncClient	= atoi(pStr->ReadString("client","0"));
		pFlag->bSaved	= atoi(pStr->ReadString("save","1"));

		pFlag->nOffPos		= m_nCurOffPos;
		switch(pFlag->bType)
		{
		case 0:
			m_nCurOffPos	+= 1;
			break;
		case 1:
			m_nCurOffPos	+= 2;
			break;
		case 2:
			m_nCurOffPos	+= 4;
			break;
		}

		m_pList->Add(pFlag);
	}
	pStr->Free();
}

UFlagObj* UFlagConfig::GetFlagByID( int nID )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		UFlagObj* pFlag	= (UFlagObj*)m_pList->Items[i];
		if(pFlag->nId == nID)
			return pFlag;
	}
	return NULL;
}

UserFlag::UserFlag(UserData* pU)
{
	pUser	= pU;

	pbyData	= (BYTE*)malloc(UFlagConfig::GetInstance().m_nCurOffPos);
	memset(pbyData,0,UFlagConfig::GetInstance().m_nCurOffPos);
	memset(pBySLimit,0,SHOPLIMIT_MAX);
}

UserFlag::~UserFlag()
{
	free(pbyData);
}

int UserFlag::SetFlag( WORD wID,int nVal )
{
	UFlagObj* pFlag	= UFlagConfig::GetInstance().GetFlagByID(wID);
	if(pFlag == NULL)
		return 0;

	switch(pFlag->bType)
	{
	case 0:
		{
			pbyData[pFlag->nOffPos]	= nVal;
			if(pFlag->bSaved)
				_saveDB();
			if(pFlag->bSyncClient)
				_sendClient(wID,nVal);
			return nVal;
		}
		break;
	case 1:
		{
			WORD*	pVal	= (WORD*)(pbyData+pFlag->nOffPos);
			*pVal	= nVal;
			
			if(pFlag->bSaved)
				_saveDB();
			if(pFlag->bSyncClient)
				_sendClient(wID,nVal);
			return nVal;
		}
		break;
	case 2:
		{
			DWORD*	pVal	= (DWORD*)(pbyData+pFlag->nOffPos);
			*pVal	= nVal;

			if(pFlag->bSaved)
				_saveDB();
			if(pFlag->bSyncClient)
				_sendClient(wID,nVal);
			return nVal;
		}
		break;
	}
	return 0;
}

int UserFlag::GetFlag(WORD wID)
{
	UFlagObj* pFlag	= UFlagConfig::GetInstance().GetFlagByID(wID);
	if(pFlag == NULL)
		return 0;
	switch(pFlag->bType)
	{
	case 0:
		{
			return pbyData[pFlag->nOffPos];
		}
		break;
	case 1:
		{
			WORD*	pVal	= (WORD*)(pbyData+pFlag->nOffPos);
			return *pVal;
		}
		break;
	case 2:
		{
			DWORD*	pVal	= (DWORD*)(pbyData+pFlag->nOffPos);
		
			return *pVal;
		}
		break;
	}
	return 0;
}

int UserFlag::AddFlag( WORD wID,int nVal )
{
	UFlagObj* pFlag	= UFlagConfig::GetInstance().GetFlagByID(wID);
	if(pFlag == NULL)
		return 0;

	switch(pFlag->bType)
	{
	case 0:
		{
			pbyData[pFlag->nOffPos]	+= nVal;
			if(pFlag->bSaved)
				_saveDB();
			if(pFlag->bSyncClient)
				_sendClient(wID,pbyData[pFlag->nOffPos]);
			return pbyData[pFlag->nOffPos];
		}
		break;
	case 1:
		{
			WORD*	pVal	= (WORD*)(pbyData+pFlag->nOffPos);
			*pVal	+= nVal;

			if(pFlag->bSaved)
				_saveDB();
			if(pFlag->bSyncClient)
				_sendClient(wID,*pVal);
			return *pVal;
		}
		break;
	case 2:
		{
			DWORD*	pVal	= (DWORD*)(pbyData+pFlag->nOffPos);
			*pVal	+= nVal;

			if(pFlag->bSaved)
				_saveDB();
			if(pFlag->bSyncClient)
				_sendClient(wID,*pVal);
			return *pVal;
		}
		break;
	}
	return 0;
}

int UserFlag::SetSLimit( BYTE bSL,int nVal )
{
	if(bSL >= SHOPLIMIT_MAX)
		return 0;
	pBySLimit[bSL]	= nVal;
	_saveSLDB();
	_sendSLClient(bSL,nVal);
	return pBySLimit[bSL];
}

int UserFlag::GetSLimit( BYTE bSL )
{
	if(bSL >= SHOPLIMIT_MAX)
		return 0;
	return pBySLimit[bSL];
}

void UserFlag::DBInitFlag( BYTE* pbD, int nSize )
{
	if(nSize > UFlagConfig::GetInstance().m_nCurOffPos)
		nSize	= UFlagConfig::GetInstance().m_nCurOffPos;

	memcpy(pbyData,pbD,nSize);
}

void UserFlag::DBInitSLimit( BYTE* pbD, int nSize )
{
	if(nSize > SHOPLIMIT_MAX)
		nSize	= SHOPLIMIT_MAX;

	memcpy(pBySLimit,pbD,nSize);
}

void UserFlag::DBInitMark( BYTE* pbD, int nSize )
{
	if(nSize == 0)
		return;

	for (int i = 0; i < nSize; i+=3)
	{
		mapMark[*(WORD*)(pbD+i)]	= pbD[i+2];
	}
}

void UserFlag::DBInitQuest( BYTE* pbD, int nSize )
{
	if(nSize == 0)
		return;
	if((nSize-4)%3 != 0)
		return;
	DWORD	dwOffPos	= *(DWORD*)pbD;

	for (int i = 4; i < dwOffPos; i+=3)
	{
		mapLineStep[*(WORD*)(pbD+i)]	= pbD[i+2];
	}

	for (int i = dwOffPos; i < nSize; i+=3)
	{
		mapDayTime[*(WORD*)(pbD+i)]		= pbD[i+2];
	}
}

void UserFlag::_sendClient( WORD wID, int nVal )
{
	BuildPacketEx(pPacket, USER, S_FLAG_SINGLE,buf,256);
	SET_DATA(pSend, USER, S_FLAG_SINGLE, pPacket);
	pSend->szFlag.wID	= wID;
	pSend->szFlag.nVal	= nVal;
	TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
}


void UserFlag::_sendSLClient( BYTE bSL, BYTE byVal )
{
	BuildPacketEx(pPacket, USER, S_FLAG_SLIMIT_SINGLE,buf,256);
	SET_DATA(pSend, USER, S_FLAG_SLIMIT_SINGLE, pPacket);
	pSend->szFlag.wID	= bSL;
	pSend->szFlag.nVal	= byVal;
	TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
}

void UserFlag::_sendQuestDB(BYTE bIsTime, WORD wLine, BYTE bStep )
{
	BuildPacketEx(pPacket, USER, S_FLAG_QUEST_SINGLE,buf,256);
	SET_DATA(pSend, USER, S_FLAG_QUEST_SINGLE, pPacket);
	pSend->bIsTime		= bIsTime;
	pSend->szFlag.wID	= wLine;
	pSend->szFlag.bVal	= bStep;
	TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
}

void UserFlag::_sendMarkClient(WORD wID, BYTE byVal)
{
	BuildPacketEx(pPacket, USER, S_FLAG_COPYLMT_SINGLE,buf,256);
	SET_DATA(pSend, USER, S_FLAG_COPYLMT_SINGLE, pPacket);
	pSend->nCopyID = wID;
	pSend->bVal = byVal;
	TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
}

void UserFlag::_saveDB()
{
	pUser->m_pDB->SetBLOB(false,pbyData,UFlagConfig::GetInstance().m_nCurOffPos,"update "RPG_USER_TABLE" set flag=? where uin=%d",pUser->m_dwUin);
}

void UserFlag::_saveSLDB()
{
	pUser->m_pDB->SetBLOB(false,pBySLimit,SHOPLIMIT_MAX,"update "RPG_USER_TABLE" set slimitflag=? where uin=%d",pUser->m_dwUin);
}

void UserFlag::_saveMarkDB()
{
	BYTE	bMarkVal[4096]	= {0};
	int		nSize	= 0;
	BYTE*	pWrite	= bMarkVal;
	std::map<WORD,BYTE>::iterator itor	= mapMark.begin();
	while(itor	!= mapMark.end())
	{
		*((WORD*)pWrite)		= itor->first;
		pWrite+=2;
		pWrite[0]				= itor->second;
		pWrite++;
		nSize	+= 3;
		itor++;
	}
	
	pUser->m_pDB->SetBLOB(false,bMarkVal,nSize,"update "RPG_USER_TABLE" set cfgflag=? where uin=%d",pUser->m_dwUin);
}

void UserFlag::_saveQuestDB( )
{
	BYTE	bQuestVal[4096]	= {0};
	int		nSize	= 0;
	BYTE*	pWrite	= bQuestVal;
	*((DWORD*)pWrite)	= 4+mapLineStep.size()*3;
	pWrite	+= 4;
	nSize	+= 4;
	std::map<WORD,BYTE>::iterator itor	= mapLineStep.begin();
	while(itor	!= mapLineStep.end())
	{
		*((WORD*)pWrite)		= itor->first;
		pWrite+=2;
		pWrite[0]				= itor->second;
		pWrite++;
		nSize	+= 3;
		itor++;
	}
	itor	= mapDayTime.begin();
	while(itor	!= mapDayTime.end())
	{
		*((WORD*)pWrite)		= itor->first;
		pWrite+=2;
		pWrite[0]				= itor->second;
		pWrite++;
		nSize	+= 3;
		itor++;
	}
	pUser->m_pDB->SetBLOB(false,bQuestVal,nSize,"update "RPG_USER_TABLE" set questflag=? where uin=%d",pUser->m_dwUin);
}

void UserFlag::SyncClient()
{
	BuildPacketEx(pPacket, USER, S_FLAG_ALL,buf,4096);
	SET_DATA(pSend, USER, S_FLAG_ALL, pPacket);
	int nCnt	= 0;
	for(int i = 0; i < UFlagConfig::GetInstance().m_pList->Count; i++)
	{
		UFlagObj*	pFlag	= (UFlagObj*)UFlagConfig::GetInstance().m_pList->Items[i];
		if(pFlag->bSyncClient)
		{
			pSend->szFlag[nCnt].wID	= pFlag->nId;
			switch(pFlag->bType)
			{
			case 0:
				pSend->szFlag[nCnt].nVal	= pbyData[pFlag->nOffPos];
				break;
			case 1:
				pSend->szFlag[nCnt].nVal	= *((WORD*)(pbyData+pFlag->nOffPos));
				break;
			case 2:
				pSend->szFlag[nCnt].nVal	= *((DWORD*)(pbyData+pFlag->nOffPos));
				break;
			}
			nCnt++;
		}
	}
	if(nCnt == 0)
		return;
	pSend->wCnt	= nCnt;
	pPacket->wSize	+= sizeof(NetUFlag)*(nCnt-1);

	TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
}


void UserFlag::SyncSLimitClient()
{
	BuildPacketEx(pPacket, USER, S_FLAG_SLIMIT_ALL,buf,4096);
	SET_DATA(pSend, USER, S_FLAG_SLIMIT_ALL, pPacket);
	
	memcpy(pSend->szbyData,pBySLimit,SHOPLIMIT_MAX);
	pSend->wCnt	= SHOPLIMIT_MAX;
	pPacket->wSize	+= SHOPLIMIT_MAX;
	TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
}

void UserFlag::SyncCopyLmtClient()
{
	BuildPacketEx(pPacket, USER, S_FLAG_COPYLMT_ALL,buf,4096);
	SET_DATA(pSend, USER, S_FLAG_COPYLMT_ALL, pPacket);
	pSend->wCnt = mapMark.size();
	int nRPos	= 0;
	std::map<WORD,BYTE>::iterator itor	= mapMark.begin();
	while(itor	!= mapMark.end())
	{
		pSend->szFlag[nRPos].wID	= itor->first;
		pSend->szFlag[nRPos].bVal	= itor->second;
		itor++;
		nRPos++;
	}
	pPacket->wSize	+= (pSend->wCnt-1)*sizeof(NetQuestFlag);
	TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
}

void UserFlag::SyncQuestClient()
{
	BuildPacketEx(pPacket, USER, S_FLAG_QUEST_ALL,buf,4096);
	SET_DATA(pSend, USER, S_FLAG_QUEST_ALL, pPacket);
	pSend->wCntLine		= mapLineStep.size();
	pSend->wCntTime		= mapDayTime.size();
	int nRPos	= 0;
	std::map<WORD,BYTE>::iterator itor	= mapLineStep.begin();
	while(itor	!= mapLineStep.end())
	{
		pSend->szFlag[nRPos].wID	= itor->first;
		pSend->szFlag[nRPos].bVal	= itor->second;
		itor++;
		nRPos++;
	}
	itor	= mapDayTime.begin();
	while(itor	!= mapDayTime.end())
	{
		pSend->szFlag[nRPos].wID	= itor->first;
		pSend->szFlag[nRPos].bVal	= itor->second;
		itor++;
		nRPos++;
	}

	pPacket->wSize	+= (pSend->wCntLine+pSend->wCntTime-1)*sizeof(NetQuestFlag);
	TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
}

void UserFlag::CheckResetDaily(bool bWeek, bool bSend)
{
	//重置配置的FLAG
	for(int i = 0; i < UFlagConfig::GetInstance().m_pList->Count; i++)
	{
		UFlagObj*	pFlag	= (UFlagObj*)UFlagConfig::GetInstance().m_pList->Items[i];
		if(pFlag->bResetType == 1 || (pFlag->bResetType == 2 && bWeek))
		{
			switch(pFlag->bType)
			{
			case 0:
				{
					pbyData[pFlag->nOffPos]	= 0;
				}
				break;
			case 1:
				{
					WORD*	pVal	= (WORD*)(pbyData+pFlag->nOffPos);
					*pVal	= 0;
				}
				break;
			case 2:
				{
					DWORD*	pVal	= (DWORD*)(pbyData+pFlag->nOffPos);
					*pVal	= 0;
				}
				break;
			}
		}
	}
	_saveDB();
	if(bSend)
		SyncClient();

	//重置商城购买的规则
	BYTE pBySLimitTemp[SHOPLIMIT_MAX] = {0};
	memcpy(pBySLimitTemp,pBySLimit,SHOPLIMIT_MAX*sizeof(BYTE));
	memset(pBySLimit,0,SHOPLIMIT_MAX*sizeof(BYTE));
	for(int i = 0; i < MallConfig::GetInstance().m_pItemList->Count; i++)
	{
		MallItemCfg*	pFlag	= (MallItemCfg*)MallConfig::GetInstance().m_pItemList->Items[i];
		if (pFlag->m_nFlag == -1||pFlag->m_nFlag>=SHOPLIMIT_MAX)
		{
			continue;
		}
		if(pFlag->m_byType == 1 || (pFlag->m_byType == 2 && bWeek) || (pFlag->m_byType == 5 && bWeek))
		{
			pBySLimit[pFlag->m_nFlag]	= 0;
		}else{
			pBySLimit[pFlag->m_nFlag]	= pBySLimitTemp[pFlag->m_nFlag];
		}
	}
	_saveSLDB();
	if(bSend)
		SyncSLimitClient();

	mapDayTime.clear();
	_saveQuestDB();
	if(bSend)
		SyncQuestClient();

	mapMark.clear();
	_saveMarkDB();
	if(bSend)
		SyncCopyLmtClient();
}

int UserFlag::SetQuestStep( WORD wLine, BYTE bStep )
{
	if(wLine >= 10000)
		return 0;
	mapLineStep[wLine]	= bStep;
	_sendQuestDB(0,wLine,bStep);
	_saveQuestDB();
	return bStep;
}

int UserFlag::SetQuestTime( WORD wQId, BYTE bTime )
{
	mapDayTime[wQId]	= bTime;
	_sendQuestDB(1,wQId,bTime);
	_saveQuestDB();
	return bTime;
}

int UserFlag::GetQuestStep( WORD wLine )
{
	if(wLine >= 10000)
		return 0;
	std::map<WORD,BYTE>::iterator	itor	= mapLineStep.find(wLine);
	if(itor	!= mapLineStep.end())
		return itor->second;
	return 0;
}

int UserFlag::GetQuestTime( WORD wQId )
{
	std::map<WORD,BYTE>::iterator	itor	= mapDayTime.find(wQId);
	if(itor	!= mapDayTime.end())
		return itor->second;
	return 0;
}

int UserFlag::SetMarkFlag( WORD wID,int nVal )
{
	mapMark[wID]	= nVal;
	_saveMarkDB();
	_sendMarkClient(wID,nVal);
	return nVal;
}

int UserFlag::GetMarkFlag( WORD wID )
{
	std::map<WORD,BYTE>::iterator	itor	= mapMark.find(wID);
	if(itor	!= mapMark.end())
		return itor->second;
	return 0;
}

int UserFlag::AddMarkFlag( WORD wID )
{
	std::map<WORD,BYTE>::iterator	itor	= mapMark.find(wID);
	if(itor	== mapMark.end())
	{
		mapMark[wID]	= 1;
		_saveMarkDB();
		_sendMarkClient(wID,1);
		return 1;
	}
	itor->second	= itor->second+1;
	_saveMarkDB();
	_sendMarkClient(wID,itor->second);
	return itor->second;
}


UserCoolDown::UserCoolDown(UserData* pU)
{
	pUser	= pU;
}

UserCoolDown::~UserCoolDown()
{

}

void UserCoolDown::LockCD( WORD wPos, DWORD dwTickSec )
{
	m_mapCD[wPos]	= CMTime::GetCurTickCount()+dwTickSec;
	if(dwTickSec > 300)	//10分钟
		ReSaveDB();
	SyncClient(wPos);
}

DWORD UserCoolDown::CheckIsLock( WORD wPos )
{
	std::map<WORD,DWORD>::iterator	itor	= m_mapCD.find(wPos);
	if(itor	!= m_mapCD.end())
	{
		int	nVal	= itor->second-CMTime::GetCurTickCount();
		if(nVal > 0)
			return nVal;
	}
	return 0;
}

void UserCoolDown::DBInitCD( BYTE* pbD, int nSize )
{
	if(nSize%6!= 0)
		return;
	for (int i = 0; i < nSize; i+=6)
	{
		m_mapCD[*(WORD*)(pbD+i)]	= *(DWORD*)(pbD+i+2);
	}
}

void UserCoolDown::ReSaveDB()
{
	BYTE	byData[4096]	= {0};
	int		nUserSize	= 0;
	BYTE*	pWriteP	= byData;
	DWORD	dwCurSec	= CMTime::GetCurTickCount();
	std::map<WORD, DWORD>::iterator	itor	= m_mapCD.begin();
	while(itor != m_mapCD.end())
	{
		if(((int)(itor->second-dwCurSec)) > 30)
		{
			(*(WORD*)(pWriteP))	= itor->first;
			pWriteP+=2;
			(*(DWORD*)(pWriteP))	= itor->second;
			pWriteP+=4;
			nUserSize	+= 6;
		}
		itor++;
	}
	pUser->m_pDB->SetBLOB(false,byData,nUserSize,"update "RPG_USER_TABLE" set cdflag=? where uin=%d",pUser->m_dwUin);
}

void UserCoolDown::SyncClient()
{
	BuildPacketEx(pPacket, USER, S_COOLDOWN_ALL,buf,4096);
	SET_DATA(pSend, USER, S_COOLDOWN_ALL, pPacket);
	DWORD	dwCurSec	= CMTime::GetCurTickCount();
	std::map<WORD, DWORD>::iterator	itor	= m_mapCD.begin();
	int nCnt	= 0;
	while(itor != m_mapCD.end())
	{
		if (itor->second>dwCurSec)
		{
			pSend->szFlag[nCnt].wID = itor->first;
			pSend->szFlag[nCnt].nVal = itor->second - dwCurSec;
			nCnt++;
		}
		itor++;
		
	}
	if(nCnt == 0)
		return;
	pSend->wCnt	= nCnt;
	pPacket->wSize	+= sizeof(NetUFlag)*(nCnt-1);

	TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
}

void UserCoolDown::SyncClient( WORD wPos )
{
	std::map<WORD, DWORD>::iterator	itor = m_mapCD.find(wPos);
	if (itor == m_mapCD.end())
	{
		return;
	}
	BuildPacketEx(pPacket, USER, S_COOLDOWN_SINGLE,buf,4096);
	SET_DATA(pSend, USER, S_COOLDOWN_SINGLE, pPacket);
	pSend->szFlag.wID = itor->first;
	pSend->szFlag.nVal = itor->second - CMTime::GetCurTickCount();
	TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
}

