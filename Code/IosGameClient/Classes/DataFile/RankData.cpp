#include "RankData.h"
#include "UserData.h"
#include "MainMenu.h"

RankList::RankList()
{
	m_pList = xnList::Create();
	pMine = NULL;
	m_dwValueFlag = 0;
}

RankList::~RankList()
{
	if(m_pList)
	{
		while(m_pList->Count > 0)
			delete (Rank_Data*)m_pList->Delete(0);
		m_pList->Free();
	}
	if(pMine)
		delete pMine;
}

Rank_Data* RankList::GetObj(DWORD id1,DWORD id2)
{
	for(int i = 0; i < m_pList->Count; i ++)
	{
		Rank_Data* pData = (Rank_Data*)m_pList->Items[i];
		if(pData->m_dwID1 == id1 && pData->m_dwID2 == id2)
			return pData;
	}
	return NULL;
}

Rank_Data* RankList::GetObjbyPos(int nPos)
{
	for(int i = 0; i < m_pList->Count; i ++)
	{
		Rank_Data* pData = (Rank_Data*)m_pList->Items[i];
		if(pData->m_nPos == nPos)
			return pData;
	}
	return NULL;
}


bool RankList::AddNewObj(Rank_Data* pData)
{
	if(m_pList->Count >= MAX_RANK_VIEW)
		return false;
	Rank_Data* pNew = new Rank_Data();
	memcpy(pNew,pData,sizeof(Rank_Data));
	m_pList->Add(pNew);
	return true;
}

bool RankList::UpdateObj(Rank_Data* pData)
{
	Rank_Data* pTemp = GetObjbyPos(pData->m_nPos);
	if(pTemp)
	{
		memcpy(pTemp,pData,sizeof(Rank_Data));
	}
	else
	{
		pTemp = new Rank_Data();
		memcpy(pTemp,pData,sizeof(Rank_Data));
		m_pList->Add(pTemp);
	}
	return true;
}

bool RankList::UpdateMine(Rank_Data* pData)
{
	if(pData == NULL)
		return false;
	if(pMine == NULL)
		pMine = new Rank_Data();
	memcpy(pMine,pData,sizeof(Rank_Data));
	return true;
}

RankCenter::RankCenter()
{
	for(int i = 0; i < RANK_MAX; i ++)
	{
		m_szRankList[i] = new RankList();
		m_szCH[i] = false;
	}
	m_pUserInfoList = xnList::Create();
	m_pWatchEquipList = xnList::Create();
	m_pDamageLastData = NULL;
}

RankCenter::~RankCenter()
{
	for(int i = 0; i < RANK_MAX; i ++)
		delete m_szRankList[i];
	if(m_pUserInfoList)
	{
		while(m_pUserInfoList->Count > 0)
			delete (ClientUserInfo*)m_pUserInfoList->Delete(0);
		m_pUserInfoList->Free();
	}
	if(m_pWatchEquipList)
	{
		while(m_pWatchEquipList->Count > 0)
			delete (ClientWatchEquip*)m_pWatchEquipList->Delete(0);
		m_pWatchEquipList->Free();
	}
}

bool RankCenter::PacketProcess(ZPacket* pPacket)
{
	if(pPacket->bCmdGroup != RPGRANK)
		return false;
	switch(pPacket->bCmd)
	{
	case S_SEND_RANKLIST:
		{
			SET_DATA(pRecv,RPGRANK,S_SEND_RANKLIST,pPacket);
			for(int i = 0; i < pRecv->nCnt; i ++)
				UpdateObj(&pRecv->szData[i]);
		}
		return true;
	case S_SEND_RANKCHANGE:
		{
// 			SET_DATA(pRecv,RPGRANK,S_SEND_RANKCHANGE,pPacket);
// 			for(int i = 0; i < pRecv->nCnt; i ++)
// 				UpdateObj(&pRecv->szData[i]);
		}
		return true;
	case S_SEND_MYRANK:
		{
			SET_DATA(pRecv,RPGRANK,S_SEND_MYRANK,pPacket);
			UpdateMine(&pRecv->xData);
		}
		return true;
	case S_SEND_RANKINFO_RLT:
		{
			SET_DATA(pRecv,RPGRANK,S_SEND_RANKINFO_RLT,pPacket);
			for(int i = 0; i < RANK_MAX; i ++)
				m_szRankList[i]->m_dwValueFlag = pRecv->dwRankFlag[i];
		}
		return true;
	case S_SEND_USERINFO:
		{
			SET_DATA(pRecv,RPGRANK,S_SEND_USERINFO,pPacket);
			ClientUserInfo* pNew = AddUserInfo(&pRecv->xInfo);
			pMainMenu->m_pSelObjInfoMenu->Open(pNew);
		}
		return true;
	case S_LAST_DAMAGE:
		{
			SET_DATA(pRecv,RPGRANK,S_LAST_DAMAGE,pPacket);
			if (m_pDamageLastData)
			{
				delete m_pDamageLastData;
			}
			m_pDamageLastData = new Rank_Data;
			memcpy(m_pDamageLastData,&pRecv->szData,sizeof(Rank_Data));
			pMainMenu->m_pRankMenu[RANK_DAMAGE]->UpdateDamage();
		}
		return true;
	case S_SEND_WATCHDATA:
		{
			SET_DATA(pRecv,RPGRANK,S_SEND_WATCHDATA,pPacket);
			if(pRecv->xInfo.byType == 0)
			{
				ClientWatchEquip* pNew = AddUserEquip(&pRecv->xInfo);
				if(pNew)
					pMainMenu->m_pEquipInfoMenu->Open(pNew->pItem,FORM_NULL);
			}
		}
		return true;
	}
	return false;
}

void RankCenter::AddNewObj(Rank_Data* pData)
{
	if(pData->m_byType >= RANK_MAX)
		return;
	if(m_szRankList[pData->m_byType]->AddNewObj(pData))
		m_szCH[pData->m_byType] = true;
}

void RankCenter::UpdateObj(Rank_Data* pData)
{
	if(pData->m_byType >= RANK_MAX)
		return;
	if(m_szRankList[pData->m_byType]->UpdateObj(pData))
		m_szCH[pData->m_byType] = true;
}

void RankCenter::UpdateMine(Rank_Data* pData)
{
	if(pData->m_byType >= RANK_MAX)
		return;
	if(m_szRankList[pData->m_byType]->UpdateMine(pData))
		m_szCH[pData->m_byType] = true;
}

void RankCenter::RequestRankInfo()
{
	BuildPacketEx(pPacket,RPGRANK,C_REQUEST_RANKINFO,buf,1024);
	SET_DATA(pData,RPGRANK,C_REQUEST_RANKINFO,pPacket);
	for(int i = 0; i < RANK_MAX; i ++)
	{
		pData->dwRankFlag[i] = m_szRankList[i]->m_dwValueFlag;
	}
	SendPacket(pPacket);
}

ClientUserInfo* RankCenter::AddUserInfo(RankUserInfo* pInfo)
{
	for(int i = 0; i < m_pUserInfoList->Count; i ++)
	{
		ClientUserInfo* pCheck = (ClientUserInfo*)m_pUserInfoList->Items[i];
		if(pCheck->dwUin == pInfo->dwUin)
			return pCheck;
	}
	ClientUserInfo* pNew = new ClientUserInfo();
	pNew->dwUin = pInfo->dwUin;
	pNew->nLv = pInfo->nLv;
	pNew->byJob = pInfo->byJob;
	pNew->nScore = pInfo->nScore;
	pNew->nSkin[0] = pInfo->nSkin[0];
	pNew->nSkin[1] = pInfo->nSkin[1];
	pNew->nSkin[2] = pInfo->nSkin[2];
	memcpy(pNew->szName,pInfo->szName,20);
	memset(pNew->szItem,0,EQUIP_MAX*4);
	for(int i = 0; i < EQUIP_MAX; i ++)
	{
		if(pInfo->szSize[i] > 0)
		{
			pNew->szItem[i] = pChar->m_pItemCenter->CreateItemByData(pInfo->pData[i],pInfo->szSize[i]);
		}
	}
	m_pUserInfoList->Add((void*)pNew);
	if(m_pUserInfoList->Count > 20)
		delete (ClientUserInfo*)m_pUserInfoList->Delete(0);
	return pNew;
}

ClientUserInfo* RankCenter::GetUserInfo(int nUin)
{
	for(int i = 0; i < m_pUserInfoList->Count; i ++)
	{
		ClientUserInfo* pCheck = (ClientUserInfo*)m_pUserInfoList->Items[i];
		if(pCheck->dwUin == nUin)
			return pCheck;
	}
	BuildPacketEx(pPacket,RPGRANK,C_REQUEST_GET_USERINFO,buf,1024);
	SET_DATA(pData,RPGRANK,C_REQUEST_GET_USERINFO,pPacket);
	pData->nUin = nUin;
	SendPacket(pPacket);
	return NULL;
}

ClientWatchEquip* RankCenter::AddUserEquip(RankUWatchInfo* pInfo)
{
	if(pInfo->byType == 0)
	{
		for(int i = 0; i < m_pWatchEquipList->Count; i ++)
		{
			ClientWatchEquip* pCheck = (ClientWatchEquip*)m_pWatchEquipList->Items[i];
			if(pCheck->dwUin == pInfo->dwUin && pCheck->dwID == pInfo->dwid)
				return pCheck;
		}
		ClientWatchEquip* pNew = new ClientWatchEquip();
		pNew->dwUin = pInfo->dwUin;
		pNew->dwID = pInfo->dwid;
		pNew->pItem = pChar->m_pItemCenter->CreateItemByData(pInfo->pData,pInfo->szSize);
		
		m_pWatchEquipList->Add((void*)pNew);
		if(m_pWatchEquipList->Count > 20)
			delete (ClientWatchEquip*)m_pWatchEquipList->Delete(0);
		return pNew;
	}
	return NULL;
}

ClientWatchEquip* RankCenter::GetUserEquip(int nUin,int nID)
{
	for(int i = 0; i < m_pWatchEquipList->Count; i ++)
	{
		ClientWatchEquip* pCheck = (ClientWatchEquip*)m_pWatchEquipList->Items[i];
		if(pCheck->dwUin == nUin && pCheck->dwID == nID)
			return pCheck;
	}
	BuildPacketEx(pPacket,RPGRANK,C_REQUEST_GET_WATCHDATA,buf,1024);
	SET_DATA(pData,RPGRANK,C_REQUEST_GET_WATCHDATA,pPacket);
	pData->nUin = nUin;
	pData->nID = nID;
	pData->byType = 0;
	SendPacket(pPacket);
	return NULL;
}