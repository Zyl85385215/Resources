#include "stdafx.h"
#include "FortCenter.h"
#include "TimeControl.h"
#include "UserData.h"
#include "ItemCenter.h"
#include "LuaClass.h"
#include "WarRank.h"
#include "FsGameCenter.h"
#include "ZLogFile.h"
#include "MailCenter.h"

extern ZLogFile* g_pLogFile;
extern FsGameCenter* g_pGameCenter;
extern CLuaStateManager * pLuaStateManager;
FortData* FortCenter::GetFortData( DWORD dwUin,bool bCreate )
{
	if (m_pMapData.find(dwUin) != m_pMapData.end())
	{
		return m_pMapData[dwUin];
	}
	return LoadFortData(dwUin,bCreate);
}

FortData* FortCenter::LoadFortData( DWORD dwUin ,bool bCreate)
{
// 	QueryRlt xRlt;
// 	if(!m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_FORT_TABLE" where uin=%d",dwUin))
// 		return NULL;
// 
// 	MySqlQuery* pCmd = xRlt.GetRlt();
// 	if (pCmd == NULL)
// 		return CreateNewFortData(dwUin);

// 	while(pCmd->FetchRow())
// 	{
// 		int temp = pCmd->GetInt("uin");
// 		FortData* pData = new FortData(this);
// 		pData->m_dwUin = dwUin;
// 
// 		int nLen = 0;
// 		BYTE* pLoadData = pCmd->GetBLOB("ManorData",&nLen);
// 		pData->LoadManorData(pLoadData,nLen);
// 
// 		pLoadData = pCmd->GetBLOB("WarRankData",&nLen);
// 		pData->LoadWarRankData(pLoadData,nLen);
// 
// 		m_pMapData[dwUin] = pData;
// 		return pData;
// 	}
 	if (!bCreate)
 	{
 		return NULL;
 	}
	return CreateNewFortData(dwUin);
}

void FortCenter::InitDB( DBControl* pDB )
{
	m_pDB = pDB;
	QueryRlt xRlt;
	if(!m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_FORT_TABLE))
		return;
	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd == NULL)
		return;

	while(pCmd->FetchRow())
	{
		int dwUin = pCmd->GetInt("uin");
		FortData* pData = new FortData(this);
		pData->m_dwUin = dwUin;

		int nLen = 0;
		BYTE* pLoadData = pCmd->GetBLOB("ManorData",&nLen);
		pData->LoadManorData(pLoadData,nLen);

		pLoadData = pCmd->GetBLOB("WarRankData",&nLen);
		pData->LoadWarRankData(pLoadData,nLen);

		pLoadData = pCmd->GetBLOB("ManorRecord",&nLen);
		pData->LoadManorRecord(pLoadData,nLen);

		m_pMapData[dwUin] = pData;
		//return pData;
	}
}

FortData* FortCenter::CreateNewFortData( DWORD dwUin )
{
	FortData* pData = new FortData(this);
	pData->m_dwUin = dwUin;
	_insertDBFortData(pData);
	m_pMapData[dwUin] = pData;
	return pData;
}

bool FortCenter::_insertDBFortData( FortData* pData )
{
	return m_pDB->ExecuteSQL(FALSE,"insert into "RPG_FORT_TABLE" (uin, ManorData,WarRankData,ManorRecord) values (%d,'','','')", 
		pData->m_dwUin);
}

void FortCenter::SendFortData(FortData* pFortData, DWORD dwUin,bool bOldC )
{
	if (bOldC)
	{
		BuildPacketEx(pPacket,FRIEND, S_SEND_FORTDATA, buf, 1024);
		SET_DATA(pData, FRIEND, S_SEND_FORTDATA, pPacket);
		pData->dwUin	= pFortData->m_dwUin;
		ManorData* pOldData = pFortData->GetOldManorData();
		pData->xManorData = *pOldData;
		pData->xWarRankData = *(pFortData->m_pWarRankData);
		TCP_SendPacketToUin(dwUin,pPacket);

		delete pOldData;
	}else
	{
		BuildPacketEx(pPacket,FRIEND, S_SEND_FORTDATA_NEW, buf, 1024);
		SET_DATA(pData, FRIEND, S_SEND_FORTDATA_NEW, pPacket);
		pData->dwUin	= pFortData->m_dwUin;
		pData->xManorData = *(pFortData->m_pManorData);
		pData->xWarRankData = *(pFortData->m_pWarRankData);
		TCP_SendPacketToUin(dwUin,pPacket);
	}
}

bool FortData::LoadManorData( BYTE* pData,int nSize )
{
	memset(m_pManorData,0,sizeof(ManorDataEx));
	if (nSize<=sizeof(ManorDataEx))
	{
		memcpy(m_pManorData,pData,nSize);
	}
	return true;
}

bool FortData::LoadManorRecord( BYTE* pData,int nSize )
{
	if(nSize == 0)
		return false;

	if(nSize%6!= 0)
		return false;

	while(m_pManorRecordList->Count)
		delete (MapManorRecord*)m_pManorRecordList->Delete(0);

	for (int i = 0; i < nSize; i+=6)
	{
		MapManorRecord* pRecord = new MapManorRecord;
		pRecord->dwUin = *(DWORD*)(pData+i);
		pRecord->wVal  = *(WORD*)(pData+i+4);

		m_pManorRecordList->Add((void*)pRecord);
	}

	return true;
}

FortData::FortData(FortCenter* pCenter):m_pCenter(pCenter),m_pUser(NULL)
{
	m_pManorData = new ManorDataEx;
	memset(m_pManorData,0,sizeof(ManorDataEx));

	m_pWarRankData = new WarRankData;
	memset(m_pWarRankData,0,sizeof(WarRankData));
	m_pWarRankData->nRankPos = -1;

	m_bSendHarvest = true;
	m_bShopWarRank = false;

	m_pManorRecordList = xnList::Create();
}

FortData::~FortData()
{
	if (m_pManorData)
	{
		delete m_pManorData;
	}
	if (m_pWarRankData)
	{
		delete m_pWarRankData;
	}

	while(m_pManorRecordList->Count)
		delete (MapManorRecord*)m_pManorRecordList->Delete(0);
	m_pManorRecordList->Free();
}

void FortData::SendFortData( DWORD dwUin, bool bOldC)
{
	m_pCenter->SendFortData(this,dwUin,bOldC);
}

void FortData::InsertManorRecord(DWORD dwUin,WORD wVal)
{
	MapManorRecord* pRecord = new MapManorRecord;
	pRecord->dwUin = dwUin;
	pRecord->wVal  = wVal;

	m_pManorRecordList->Add((void*)pRecord);

	while(m_pManorRecordList->Count > 200)
		delete (MapManorRecord*)m_pManorRecordList->Delete(0);

	_updateDBManorRecord();
}

void FortData::SendManorRecord(DWORD dwUin)
{
	BuildPacketEx(pPacket, FRIEND, S_SEND_MANOR_RECORD,buf,6144);
	SET_DATA(pSend, FRIEND, S_SEND_MANOR_RECORD, pPacket);
	int nRPos	= 0;
	for (int i = 0; i < m_pManorRecordList->Count; i++)
	{
		MapManorRecord* pRecord = (MapManorRecord*)m_pManorRecordList->Items[i];
		if (pRecord)
		{
			pSend->szRecord[nRPos].dwUin = pRecord->dwUin;
			pSend->szRecord[nRPos].bVal	 = pRecord->wVal;

			UserSimple* pUS = g_pUserList->GetUser(pRecord->dwUin);
			if (pUS)
			{
				strncpy(pSend->szRecord[nRPos].szName,pUS->szName,20);
			}else
			{
				if (pRecord->wVal < 10000)
					strcpy(pSend->szRecord[nRPos].szName,(char*)"雷锋");
				else
					strcpy(pSend->szRecord[nRPos].szName,(char*)"周扒皮");
			}

			nRPos++;
		}
	}

	pSend->wCnt = nRPos;
	pPacket->wSize	+= (nRPos-1)*sizeof(NetManorRecord);
	TCP_SendPacketToUin(dwUin,pPacket);
}

ManorData* FortData::GetOldManorData()
{
	ManorData* pOldData = new ManorData();
	pOldData->nLv = m_pManorData->nLv;
	pOldData->nExp = m_pManorData->nExp;
	pOldData->nPos = m_pManorData->nPos;

	for (int i = 0; i < nMaxManorCount; i++)
	{
		pOldData->dwTime[i] = m_pManorData->dwTime[i];
		pOldData->dwID[i] = m_pManorData->dwID[i];
		pOldData->dwFast[i] = m_pManorData->dwFast[i];
		pOldData->dwFastUin[i] = CheckFast(0,i,true)?0:1;
		pOldData->nState[i] = CheckSteal(0,i,true)?0:1;
	}

	return pOldData;
}

void FortData::SendManorData( DWORD dwUin, bool bOldC )
{
	if (bOldC)
	{
		BuildPacketEx(pPacket,FRIEND, S_SEND_MANORDATA, buf, 1024);
		SET_DATA(pData, FRIEND, S_SEND_MANORDATA, pPacket);
		pData->dwUin	= m_dwUin;
		ManorData* pOldData = GetOldManorData();
		pData->xManorData = *pOldData;
		TCP_SendPacketToUin(dwUin,pPacket);

		delete pOldData;
	}else
	{
		BuildPacketEx(pPacket,FRIEND, S_SEND_MANORDATA_NEW, buf, 1024);
		SET_DATA(pData, FRIEND, S_SEND_MANORDATA_NEW, pPacket);
		pData->dwUin	= m_dwUin;
		pData->xManorData = *m_pManorData;
		TCP_SendPacketToUin(dwUin,pPacket);
	}
}

void FortData::SendChgFortData( DWORD dwUin,BYTE byType,BYTE byPos,DWORD dwVal )
{
	BuildPacketEx(pPacket,FRIEND, S_SEND_FORTDATA_CHG, buf, 1024);
	SET_DATA(pData, FRIEND, S_SEND_FORTDATA_CHG, pPacket);
	pData->dwUin	= m_dwUin;
	pData->byType = byType;
	pData->byPos = byPos;
	pData->dwVal = dwVal;

	TCP_SendPacketToUin(dwUin,pPacket);
}

bool FortData::CheckManorPos( int nPos )
{
	if (!m_pUser)
	{
		return false;
	}
	if (nPos>=nMaxManorCount||nPos<0)
	{
		return false;
	}
	if (m_pManorData->dwID[nPos])
	{
		return false;
	}
	if (nPos == 0 && m_pUser->m_xUWealth.GetWealth(UW_LV)>=20)
	{
		return true;
	}else if (nPos == 1 && m_pUser->m_xUWealth.GetWealth(UW_LV)>=20)
	{
		return true;
	}else if (nPos == 2 && m_pUser->m_xUWealth.GetWealth(UW_LV)>=25)
	{
		return true;
	}else if (nPos == 3 && m_pUser->m_xUWealth.GetWealth(UW_LV)>=30)
	{
		return true;
	}else if (nPos == 4 && m_pUser->m_xUWealth.GetWealth(UW_VIPLV)>=2)
	{
		return true;
	}else if (nPos == 5 && m_pUser->m_xUWealth.GetWealth(UW_VIPLV)>=4)
	{
		return true;
	}else if (nPos == 6 && m_pUser->m_xUWealth.GetWealth(UW_VIPLV)>=6)
	{
		return true;
	}else if (nPos == 7 && (m_pManorData->nPos&1))
	{
		return true;
	}else if (nPos == 8 && (m_pManorData->nPos&2))
	{
		return true;
	}else if (nPos == 9 && (m_pManorData->nPos&4))
	{
		return true;
	}
	return false;
}

void FortData::SetUser( UserData* pUser )
{
	m_pUser = pUser;
}

bool FortData::UnlockManorPos()
{
	int i=0;
	int nCount = 0;
	for (;i<3;i++)
	{
		if (!(m_pManorData->nPos&(1<<i)))
		{
			break;
		}
		else
			nCount++;
	}
	if (i>=3)
	{
		return false;
	}
	static int nCostRmb[] = {20,100,500};
	if (!m_pUser->m_xUWealth.SubWealth(UW_RMB,nCostRmb[i],RMB_SAVE_FORT_UNLOCKMANOR))
	{
		return false;
	}
	m_pManorData->nPos = (m_pManorData->nPos|(1<<i));
	nCount ++;
	m_pUser->m_pUserRecord->RunLuaFarmFunc(3,nCount);
	SendChgFortData(m_dwUin,FORTDATA_CHG_MANOR_POS,0,m_pManorData->nPos);
	_updateDBManorData();
	return true;
}

void FortData::_updateDBManorData()
{
	m_pCenter->m_pDB->SetBLOB(false,m_pManorData,sizeof(ManorDataEx),"update "RPG_FORT_TABLE" set ManorData=? where uin=%d",m_dwUin);
}

bool FortData::SeedManorPos( int nPos,ItemObj* pItem )
{
	if (!m_pUser)
	{
		return false;
	}
	if (!pItem||pItem->m_pBase->byLv>m_pUser->m_xUWealth.GetWealth(UW_LV))
	{
		return false;
	}
	int nId = pItem->GetDataVal(ITEM_AD_TYPE);
	if (nId<25000&&nId>25499)
	{
		return false;
	}
	if (!CheckManorPos(nPos))
	{
		return false;
	}
	int nItemID = pItem->GetItemID();
	if (!m_pUser->m_pItemCenter->RemoveItemByIndex(pItem->GetIndex()))
	{
		return false;
	}
	m_pManorData->dwID[nPos] = nItemID;

	for (int i = 0; i < 5; i++)
	{
		m_pManorData->nState[nPos][i] = 0;
	}

	m_pManorData->dwTime[nPos] = CMTime::GetCurTickCount();
	SendChgFortData(m_dwUin,FORTDATA_CHG_MANOR_TIME,nPos,m_pManorData->dwTime[nPos]);
	SendChgFortData(m_dwUin,FORTDATA_CHG_MANOR_ID,nPos,m_pManorData->dwID[nPos]);
	_updateDBManorData();
	return true;

}

bool FortData::FastManorPos( int nPos,DWORD dwUin,int* nExp )
{
	if (nPos>=nMaxManorCount||nPos<0)
	{
		return false;
	}
	if (!m_pManorData->dwID[nPos])
	{
		return false;
	}
	int nItemLv = m_pManorData->dwID[nPos]%10;
	if (nItemLv>=6)
	{
		return false;
	}
	static int nCostTime[] = {120*60,240*60,360*60,480*60,600*60,720*60};
	if (CMTime::GetCurTickCount() - m_pManorData->dwTime[nPos]>=nCostTime[nItemLv])
	{
		return false;
	}
	m_pManorData->dwTime[nPos] -= nCostTime[nItemLv]/6;
	m_pManorData->dwFast[nPos] = dwUin;
	SetFastUin(dwUin,nPos);
	SendChgFortData(dwUin,FORTDATA_CHG_MANOR_TIME,nPos,m_pManorData->dwTime[nPos]);
	SendChgFortData(m_dwUin,FORTDATA_CHG_MANOR_TIME,nPos,m_pManorData->dwTime[nPos]);
	SendChgFortData(dwUin,FORTDATA_CHG_MANOR_EFFECT,nPos,239);

	if (nExp)
	{
		*nExp = (nItemLv+1)*5;
	}

	return true;
}

bool FortData::FastManorPosEx(int nPos,DWORD dwUin,int* nExp)
{
	if (nPos>=nMaxManorCount||nPos<0)
	{
		return false;
	}
	if (!m_pManorData->dwID[nPos])
	{
		return false;
	}
	int nItemLv = m_pManorData->dwID[nPos]%10;
	if (nItemLv>=6)
	{
		return false;
	}
	static int nCostTime[] = {120*60,240*60,360*60,480*60,600*60,720*60};
	if (CMTime::GetCurTickCount() - m_pManorData->dwTime[nPos]>=nCostTime[nItemLv])
	{
		return false;
	}
	m_pManorData->dwTime[nPos] -= nCostTime[nItemLv]/6/5;
	SetFastUin(dwUin,nPos);
	SendChgFortData(dwUin,FORTDATA_CHG_MANOR_TIME,nPos,m_pManorData->dwTime[nPos]);
	SendChgFortData(m_dwUin,FORTDATA_CHG_MANOR_TIME,nPos,m_pManorData->dwTime[nPos]);
	SendChgFortData(dwUin,FORTDATA_CHG_MANOR_EFFECT,nPos,239);

	if (nExp)
	{
		*nExp = nItemLv+1;
	}

	return true;
}

int FortData::GetFastCount(int nPos)
{
	int nFastCnt = 0;
	for (int i=0;i<5;i++)
	{
		if (m_pManorData->dwFastUin[nPos][i])
			nFastCnt++;
	}

	return nFastCnt;
}

bool FortData::CheckFast( DWORD dwUin,int nPos ,bool bOld)
{
	//if (!m_pUser)
	//{
	//	return false;
	//}
	if (bOld)
	{
		for (int i=0;i<5;i++)
		{
			if (m_pManorData->dwFastUin[nPos][i])
				return false;
		}
	}else
	{
		for (int i=0;i<5;i++)
		{
			if (m_pManorData->dwFastUin[nPos][i])
			{
				if (m_pManorData->dwFastUin[nPos][i] == dwUin)
					return false;
			}

		}
	}

	return true;
}

bool FortData::SetFastUin( DWORD dwUin,int nPos )
{
	//if (!m_pUser)
	//{
	//	return false;
	//}
	for (int i=0;i<5;i++)
	{
		if (!m_pManorData->dwFastUin[nPos][i])
		{
			m_pManorData->dwFastUin[nPos][i] = dwUin;
			InsertManorRecord(dwUin,1);
			return true;
		}
	}
	return false;
}

int FortData::GetStealCount(int nPos)
{
	int nStealCnt = 0;
	for (int i=0;i<5;i++)
	{
		if (m_pManorData->nState[nPos][i])
			nStealCnt++;
	}

	return nStealCnt;
}

bool FortData::CheckSteal(DWORD dwUin,int nPos,bool bOld)
{
	//if (!m_pUser)
	//{
	//	return false;
	//}

	if (bOld)
	{
		for (int i=0;i<5;i++)
		{
			if (m_pManorData->nState[nPos][i])
					return false;
		}
	}else
	{
		for (int i=0;i<5;i++)
		{
			if (m_pManorData->nState[nPos][i])
			{
				if (m_pManorData->nState[nPos][i] == dwUin)
					return false;
			}
		}
	}

	return true;
}

bool FortData::SetStealUin(DWORD dwUin,int nPos,WORD nID)
{
	//if (!m_pUser)
	//{
	//	return false;
	//}
	for (int i=0;i<5;i++)
	{
		if (!m_pManorData->nState[nPos][i])
		{
			m_pManorData->nState[nPos][i] = dwUin;
			InsertManorRecord(dwUin,nID);
			return true;
		}
	}
	return false;
}

bool FortData::GetManorReward( int nPos,UserData* pUser )
{
	if (!pUser)
	{
		return false;
	}
	if (nPos>=nMaxManorCount||nPos<0)
	{
		return false;
	}
	if (!m_pManorData->dwID[nPos])
	{
		return false;
	}
	int nItemLv = m_pManorData->dwID[nPos]%10;
	if (nItemLv>=6)
	{
		return false;
	}
	if (m_pUser != pUser && CheckSteal(pUser->m_dwUin,nPos) == false)
	{
		pUser->SendWarnMsg("你已经偷取过该植物了");
		return false;
	}else if (m_pUser != pUser && GetStealCount(nPos) > 4)
	{
		pUser->SendWarnMsg("该植物已被偷5次，手下留情吧~");
		return false;
	}
	static int nCostTime[] = {120*60,240*60,360*60,480*60,600*60,720*60};
	if (CMTime::GetCurTickCount() - m_pManorData->dwTime[nPos]>=nCostTime[nItemLv])
	{
		ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(m_pManorData->dwID[nPos]);
		if (!pBase)
		{
			return false;
		}
		if (!(pBase->nScrpitFlag & 1<<ITEM_SCRPITFLAG_SP))
		{
			return false;
		}

		m_pUser = g_pGameCenter->GetPlayer(m_dwUin);
		LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
		//pLuaState->DoFile(pBase->szLuaName);
		char sztxt[64];
		sprintf(sztxt,"%s%d",szLuaItemFlag_SP,m_pManorData->dwID[nPos]);
		LuaObject loFunction = pLuaState->GetGlobal(sztxt);
		if(!loFunction.IsFunction()){
			return false;
		}
		int psss = NULL; 
		LuaFunction<int> lf(loFunction);

		CLuaPlusSafeFunction_4<int, UserData*,UserData*,int> func;
		if(!func(pLuaState, lf, &psss, m_pUser,pUser,GetStealCount(nPos)))
		{
			g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);

		}
		if (pUser == m_pUser)
		{
			m_pManorData->dwFast[nPos] = 0;
			m_pManorData->dwID[nPos] = 0;
			m_pManorData->dwTime[nPos] = 0;
			for (int i=0;i<nMaxManorFast;i++)
			{
				for (int j = 0; j < 5; j++)
				{
					m_pManorData->dwFastUin[i][j] = 0;
				}
			}
			SendChgFortData(pUser->m_dwUin,FORTDATA_CHG_MANOR_FAST,nPos,0);
			SendChgFortData(pUser->m_dwUin,FORTDATA_CHG_MANOR_ID,nPos,0);
			SendChgFortData(pUser->m_dwUin,FORTDATA_CHG_MANOR_TIME,nPos,0);
			pUser->m_pUserRecord->RunLuaFarmFunc(1,pBase->nTypeID);
			CheckHarvest(false);
		}else{
			SetStealUin(pUser->m_dwUin,nPos,m_pManorData->dwID[nPos]);
			SendChgFortData(pUser->m_dwUin,FORTDATA_CHG_MANOR_EFFECT,nPos,240);
			pUser->m_pUserRecord->RunLuaFarmFunc(2,pBase->nTypeID);
		}
		_updateDBManorData();
		pUser->m_pUserRecord->AddRecord(UA_GAIN,UO_CROPS,pBase->nTypeID,1);
		return true;
	}
	return false;
}



void FortData::AddManorExp( int nExp )
{
	m_pManorData->nExp += nExp;
	static int nNeedExp[] = {100,300,900,2700,8100,24300,72900,218700,656100,1968300,0};
	static int nAwardNum[] = {10,20,40,70,110,160,220,290,360,0};
	static int	nAwardMoney[] = {5,10,20,35,55,80,110,145,180,0};
	while(m_pManorData->nExp >= nNeedExp[m_pManorData->nLv]&&nNeedExp[m_pManorData->nLv]){
		m_pManorData->nExp -= nNeedExp[m_pManorData->nLv];
		m_pManorData->nLv++;
		
		
		if (m_pManorData->nLv > 0)
		{
			char szText[256];
			sprintf(szText,"[%s]通过辛勤劳动，把要塞庄园等级升级到%d级,伙伴们快跟上他的脚步，海量经验、金币和荣誉等着你拿到手软哦。", m_pUser->m_szNick,m_pManorData->nLv);
			ChatCenter::GetInstance().SendSystemMsg(szText, 0, 0, 1, true);
			if (m_pManorData->nLv >= 2)
			{
				char	szTemp[128];
				sprintf(szTemp,"庄园等级达到[%d]级,获得系统赠送的奖励", m_pManorData->nLv);
				g_pGameCenter->m_pMailCenter->RequestSendItemMail(0,m_pUser->m_dwUin,"系统", szTemp,nAwardMoney[m_pManorData->nLv-2]*10000, 20002,nAwardNum[m_pManorData->nLv-2],true);
			}
			m_pUser->m_pUserRecord->AddFortLog(0,0,0,m_pManorData->nLv,m_pManorData->nExp,2);
		}
	}
	_updateDBManorData();
	SendChgFortData(m_dwUin,FORTDATA_CHG_MANOR_EXP,0,m_pManorData->nExp);
	SendChgFortData(m_dwUin,FORTDATA_CHG_MANOR_LV,0,m_pManorData->nLv);

}

bool FortData::CheckHarvest(bool bSend)
{
	if (!m_pUser)
	{
		return false;
	}
	if (bSend && m_bSendHarvest == false)
	{
		return false;
	}
	bool bCheckSend = false;
	static int nCostTime[] = {120*60,240*60,360*60,480*60,600*60,720*60};
	for (int i=0;i<nMaxManorCount;i++)
	{
		if (!m_pManorData->dwID[i])
			continue;
		int nItemLv = m_pManorData->dwID[i]%10;
		if (nItemLv>=6)
			continue;
		if (m_pManorData->dwTime[i]&&(CMTime::GetCurTickCount() - m_pManorData->dwTime[i]>=nCostTime[nItemLv]))
		{
			m_bSendHarvest = false;
			bCheckSend = true;
			break;
		}
	}
	if (bCheckSend == false)
		m_bSendHarvest = true;

	return bCheckSend;
}

void FortData::_updateDBWarRankata()
{
	m_pCenter->m_pDB->SetBLOB(false,m_pWarRankData,sizeof(WarRankData),"update "RPG_FORT_TABLE" set WarRankData=? where uin=%d",m_dwUin);
}

void FortData::_updateDBManorRecord()
{
	BYTE	byRecord[4096]	= {0};
	int		nSize	= 0;
	BYTE*	pWrite	= byRecord;

	for (int i = 0; i < m_pManorRecordList->Count; i++)
	{
		MapManorRecord* pRecord = (MapManorRecord*)m_pManorRecordList->Items[i];
		if (pRecord)
		{
			*((DWORD*)pWrite) = pRecord->dwUin;
			pWrite+=4;
			*(WORD*)pWrite	  = pRecord->wVal;
			pWrite+=2;
			nSize += 6;
		}
	}

	m_pCenter->m_pDB->SetBLOB(false,byRecord,nSize,"update "RPG_FORT_TABLE" set ManorRecord=? where uin=%d",m_dwUin);
}

void FortData::SendWarRankData( DWORD dwUin )
{
	if (!this)
	{
		return;
	}
	BuildPacketEx(pPacket,FRIEND, S_SEND_WARRANKDATA, buf, 1024);
	SET_DATA(pData, FRIEND, S_SEND_WARRANKDATA, pPacket);
	pData->dwUin	= m_dwUin;
	pData->xWarRankData = *(m_pWarRankData);

	TCP_SendPacketToUin(dwUin,pPacket);
}

bool FortData::LoadWarRankData( BYTE* pData,int nSize )
{
	memset(m_pWarRankData,0,sizeof(WarRankData));
	if (nSize<=sizeof(WarRankData))
	{
		memcpy(m_pWarRankData,pData,nSize);
	}
	m_pWarRankData->nRankPos = GetWarRankPos();
	return true;
}

bool FortData::AddWarScore( int nScore )
{
	m_pWarRankData->nRankScore += nScore;
	if (m_bShopWarRank)
	{
		m_pUser->m_pUserRecord->AddFortLog(nScore,m_pWarRankData->nRankScore,m_pWarRankData->nRankPos,0,0,1);
		m_bShopWarRank = false;
	}
	_updateDBWarRankata();
	SendChgFortData(m_dwUin,FORTDATA_CHG_WARRANK_SCORE,0,m_pWarRankData->nRankScore);
	return true;
}

void FortData::ResetDailyData()
{
	_updateDBManorData();
	m_pWarRankData->nTimes = 0;
	_updateDBWarRankata();
}

int FortData::GetWarRankPos()
{
	WarRankUser* pUser = g_pGameCenter->m_pWarRankCenter->GetWRUserByUin(m_dwUin);
	return pUser?pUser->wRankPos:-1;
}

void FortData::SetWarRankPos( int nPos )
{
	m_pWarRankData->nRankPos = nPos;
	_updateDBWarRankata();
	SendWarRankData(m_dwUin);
}
