#include "stdafx.h"
#include "TeamData.h"
#include "FsGameCenter.h"
#include "TimeControl.h"
#include "UserData.h"
#include "EnemyDropConfig.h"
#include "UserQuest.h"
#include "MentoringCfg.h"
#include "UserFlag.h"
#include "MarryCenter.h"

extern FsGameCenter* g_pGameCenter;

TeamKillDrop::TeamKillDrop()
{
	m_nNeedCnt = 0;
	m_nItemID = 0;
	memset(m_tTeamsEx,0,sizeof(TeamKillDropEx)*TEAM_MAXUSER);
}

TeamKillDrop::~TeamKillDrop()
{

}

TeamData::TeamData():m_bIsWaiting(false),m_bChgLeader(false)
{
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		m_xUser[i].nPos = i;
		m_dwTime[i] = 0;
	}
	m_dwWaitTDNeed = 0;
	m_pItemList	= xnList::Create();
	m_pTKDropList = xnList::Create();
}

TeamData::~TeamData()
{
	m_pItemList->Free();
}

bool TeamData::Join( UserData* pUser,int nState /*= TEAM_NORMAL*/ )
{
	if (pUser->m_pTeamData&&nState == TEAM_CREATE)
	{
		return false;
	}
	TeamUser* pUserData = GetTeamUser(pUser->m_dwUin);
	if (pUserData)
	{//再邀请队列里的
		pUserData->nState = nState;
		for (int i=0;i<TEAM_MAXUSER;i++)
		{
			if (m_xUser[i].dwUin/*&&i!=pUserData->nPos*/)
			{
				if(g_pMentoringCenter->GetRelation(pUser->m_dwUin,m_xUser[i].dwUin) || g_pMentoringCenter->GetRelation(m_xUser[i].dwUin,pUser->m_dwUin))
				{
					pUser->m_pUserExpCenter->AddObj(MentoringCfg::GetInstance().m_nExpaddid);
					if(m_xUser[i].pUser)
						m_xUser[i].pUser->m_pUserExpCenter->AddObj(MentoringCfg::GetInstance().m_nExpaddid);
				}
				if(g_pMarryCenter->GetRelation(pUser->m_dwUin,m_xUser[i].dwUin))
				{
					pUser->m_pUserExpCenter->AddObj(1102);
					if(m_xUser[i].pUser)
						m_xUser[i].pUser->m_pUserExpCenter->AddObj(1102);
				}
				SendTeamState(TEAM_COMMIT_ACCEPT,m_xUser[i].dwUin,pUser->m_szNick);
				SendTeamUser(pUserData->nPos,m_xUser[i].dwUin);
				SendTeamUser(i,pUser->m_dwUin);
			}
		}
		return true;
	}
	//
	if(nState == TEAM_NORMAL)
	{
		for (int i=0;i<TEAM_MAXUSER;i++)		//经验加成
		{
			if (m_xUser[i].dwUin)
			{
				if(g_pMentoringCenter->GetRelation(pUser->m_dwUin,m_xUser[i].dwUin) || g_pMentoringCenter->GetRelation(m_xUser[i].dwUin,pUser->m_dwUin))
				{
					pUser->m_pUserExpCenter->AddObj(MentoringCfg::GetInstance().m_nExpaddid);
					if(m_xUser[i].pUser)
						m_xUser[i].pUser->m_pUserExpCenter->AddObj(MentoringCfg::GetInstance().m_nExpaddid);
				}
				if(g_pMarryCenter->GetRelation(pUser->m_dwUin,m_xUser[i].dwUin))
				{
					pUser->m_pUserExpCenter->AddObj(1102);
					if(m_xUser[i].pUser)
						m_xUser[i].pUser->m_pUserExpCenter->AddObj(1102);
				}
			}
		}
	}
	//
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (!m_xUser[i].dwUin)
		{
			m_xUser[i].dwUin	= pUser->m_dwUin;
			m_xUser[i].pUser	= pUser;
			m_xUser[i].nState	= nState;
			m_xUser[i].bOnline	= true;
			m_xUser[i].nHp		= pUser->m_pMObjCenter->m_pPlayer->xParam.nHp;
			m_xUser[i].nMaxHp	= pUser->m_pMObjCenter->m_pPlayer->xParam.nHpMax;
			m_xUser[i].byJob	= pUser->m_byJob;
			m_xUser[i].bySex	= pUser->m_pMObjCenter->m_pPlayer->bySex;
			m_xUser[i].nLv		= pUser->m_pMObjCenter->m_pPlayer->byLv;
			memcpy(m_xUser[i].szName,pUser->m_szNick,20);
			pUser->m_pTeamData = this;

			if (nState == TEAM_CREATE)
			{
				m_dwTime[i] = CMTime::GetCurTickCount();
			}
			return true;
		}
	}
	return false;
}

void TeamData::Leave( UserData* pUser )
{
	TeamUser* pData = GetTeamUser(pUser->m_dwUin);
	if (pData)
	{
		for (int i=0;i<TEAM_MAXUSER;i++)		//经验加成
		{
			if (m_xUser[i].dwUin && m_xUser[i].dwUin != pUser->m_dwUin)
			{
				if(g_pMentoringCenter->GetRelation(pUser->m_dwUin,m_xUser[i].dwUin) || g_pMentoringCenter->GetRelation(m_xUser[i].dwUin,pUser->m_dwUin))
				{
					pUser->m_pUserExpCenter->RemoveObj(MentoringCfg::GetInstance().m_nExpaddid);
					if(m_xUser[i].pUser)
						m_xUser[i].pUser->m_pUserExpCenter->RemoveObj(MentoringCfg::GetInstance().m_nExpaddid);
				}
				if(g_pMarryCenter->GetRelation(pUser->m_dwUin,m_xUser[i].dwUin))
				{
					pUser->m_pUserExpCenter->RemoveObj(1102);
					if(m_xUser[i].pUser)
						m_xUser[i].pUser->m_pUserExpCenter->RemoveObj(1102);
				}
			}
		}
		if (pData->nState == TEAM_CREATE)
		{
			SendTeamState(TEAM_COMMIT_REFUSE,GetTeamLeaderUin(),pUser->m_szNick,pData->nPos);
		}
		else if (pData->nState == TEAM_NORMAL)
		{
			for (int i=0;i<TEAM_MAXUSER;i++)
			{
				if (m_xUser[i].dwUin&&m_xUser[i].bOnline)
				{
					SendTeamState(TEAM_COMMIT_LEAVE,m_xUser[i].dwUin,pData->nPos);
				}
			}
		}
		else if (pData->nState == TEAM_LEADER)
		{
			for (int i=0;i<TEAM_MAXUSER;i++)
			{
				if (m_xUser[i].dwUin&&m_xUser[i].dwUin!=pUser->m_dwUin&&m_xUser[i].bOnline)
				{
					ChangeLeader(i);
					break;
				}
			}
			for (int i=0;i<TEAM_MAXUSER;i++)
			{
				if (m_xUser[i].dwUin&&m_xUser[i].bOnline)
				{
					SendTeamState(TEAM_COMMIT_LEAVE,m_xUser[i].dwUin,pData->nPos);
				}
			}
		}

		pData->dwUin	= 0;
		pData->pUser	= NULL;
		pData->nState	= TEAM_CREATE;
		pData->bOnline	= false;
		pUser->OnLeaveTeam();
		pUser->m_pTeamData = NULL;
	}
}

void TeamData::Leave( BYTE nPos )
{
	if (nPos<0||nPos>=TEAM_MAXUSER)
	{
		return;
	}
	TeamUser* pData = &m_xUser[nPos];
	if (pData)
	{
		for (int i=0;i<TEAM_MAXUSER;i++)		//经验加成
		{
			if (m_xUser[i].dwUin && m_xUser[i].dwUin != pData->dwUin)
			{
				if(g_pMentoringCenter->GetRelation(pData->dwUin,m_xUser[i].dwUin) || g_pMentoringCenter->GetRelation(m_xUser[i].dwUin,pData->dwUin))
				{
					if(pData->pUser)
						pData->pUser->m_pUserExpCenter->RemoveObj(MentoringCfg::GetInstance().m_nExpaddid);
					if(m_xUser[i].pUser)
						m_xUser[i].pUser->m_pUserExpCenter->RemoveObj(MentoringCfg::GetInstance().m_nExpaddid);
				}
				if(g_pMarryCenter->GetRelation(pData->dwUin,m_xUser[i].dwUin))
				{
					if(pData->pUser)
						pData->pUser->m_pUserExpCenter->RemoveObj(1102);
					if(m_xUser[i].pUser)
						m_xUser[i].pUser->m_pUserExpCenter->RemoveObj(1102);
				}
			}
		}
		if (pData->nState == TEAM_NORMAL)
		{
			for (int i=0;i<TEAM_MAXUSER;i++)
			{
				if (m_xUser[i].dwUin&&m_xUser[i].bOnline)
				{
					SendTeamState(TEAM_COMMIT_LEAVE,m_xUser[i].dwUin,pData->nPos);
				}
			}
		}else if (pData->nState == TEAM_LEADER)
		{
			for (int i=0;i<TEAM_MAXUSER;i++)
			{
				if (m_xUser[i].dwUin&&m_xUser[i].dwUin!=pData->dwUin&&m_xUser[i].bOnline)
				{
					ChangeLeader(i);
					break;
				}
			}
			for (int i=0;i<TEAM_MAXUSER;i++)
			{
				if (m_xUser[i].dwUin&&m_xUser[i].bOnline)
				{
					SendTeamState(TEAM_COMMIT_LEAVE,m_xUser[i].dwUin,pData->nPos);
				}
			}
		}
		//UserData* pUser = g_pGameCenter->GetPlayer(pData->dwUin);
		pData->dwUin = 0;
		pData->nState = TEAM_CREATE;
		pData->bOnline = false;
		if(pData->pUser){
			pData->pUser->OnLeaveTeam();
			pData->pUser->m_pTeamData = NULL;
			pData->pUser	= NULL;
		}
	}
}

void TeamData::OnValChange( UserData* pUser )
{
	TeamUser* pData = GetTeamUser(pUser->m_dwUin);
	if (pData)
	{
		pData->nHp = pUser->m_pMObjCenter->m_pPlayer->xParam.nHp;
		pData->nMaxHp = pUser->m_pMObjCenter->m_pPlayer->xParam.nHpMax;
		pData->nLv = pUser->m_xUWealth.GetWealth(UW_LV);
		SendTeamUserToAll(pData->nPos);
	}
}

bool TeamData::Update()
{
	if (m_pTKDropList->Count)
	{
		SendTDItemAdd();
	}

	int nCount = 0;
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (m_xUser[i].dwUin)
		{
			nCount++;
		}
	}
	if(nCount<=1) return false;
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (m_xUser[i].dwUin&&m_xUser[i].nState == TEAM_CREATE&&CMTime::GetCurTickCount() - m_dwTime[i]>=30){
			Leave(i);
		}
	}
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (m_xUser[i].dwUin&&m_xUser[i].bOnline)
		{
			return true;
		}
	}
	return false;
}

void TeamData::CheckTKDropNeed(int nItemID,int nMIndex)
{
	ItemExBase* pItem = ItemConfig::GetInstance().GetItemBase(nItemID);
	if (pItem)
	{
		TeamKillDrop* pTempInfo = new TeamKillDrop();
		pTempInfo->m_nItemID = nItemID;

		BuildPacketEx(pPacket, USER, S_TEAM_DROPRAND, buf, 1024);
		SET_DATA(pSend, USER, S_TEAM_DROPRAND, pPacket);

		int nCount = 0;
		for (int i = 0;i < TEAM_MAXUSER; i++)
		{
			if (m_xUser[i].dwUin&&(m_xUser[i].nState != TEAM_CREATE)&&m_xUser[i].pUser&&(pItem->nJob&(1<<m_xUser[i].pUser->m_byJob))&&(m_xUser[i].pUser->m_nMIndex == nMIndex))
			{
				pTempInfo->m_tTeamsEx[nCount].dwUin = m_xUser[i].pUser->m_dwUin;
				pTempInfo->m_tTeamsEx[nCount].nRand = Random_Int(0,100);
				nCount++;

				pSend->nItemID = nItemID;
				TCP_SendPacketToUin(m_xUser[i].pUser->m_dwUin,pPacket);
			}
		}
		if (nCount == 0)
		{
			pTempInfo->m_nNeedCnt = 0;
			for (int i = 0;i < TEAM_MAXUSER; i++)
			{
				if (m_xUser[i].dwUin&&m_xUser[i].pUser&&(m_xUser[i].pUser->m_nMIndex == nMIndex))
				{
					pTempInfo->m_tTeamsEx[i].dwUin = m_xUser[i].pUser->m_dwUin;
					pTempInfo->m_tTeamsEx[i].nRand = Random_Int(0,100);
					pTempInfo->m_nNeedCnt++;

					pSend->nItemID = nItemID;
					TCP_SendPacketToUin(m_xUser[i].pUser->m_dwUin,pPacket);
				}
			}
		}else
			pTempInfo->m_nNeedCnt = nCount;
		m_pTKDropList->Add((void*)pTempInfo);
	}
}

void TeamData::SendTeamDropRand()
{
//	TeamKillDrop* pTempInfo = (TeamKillDrop*)m_pTKDropList->Items[0];
//	int nCount = 0;
//	for (int i = 0; i < TEAM_MAXUSER; i++)
//	{
//		if (pTempInfo->m_tTeamsEx[i].dwUin)
//		{
//			for (int j = 0; j < TEAM_MAXUSER; j++)
//			{
//				if (pTempInfo->m_tTeamsEx[i].dwUin == m_xUser[j].dwUin)
//				{
//					BuildPacketEx(pPacket, USER, S_TEAM_DROPRAND, buf, 1024);
//					SET_DATA(pSend, USER, S_TEAM_DROPRAND, pPacket);
//					pSend->nItemID = pTempInfo->m_nItemID;
//					TCP_SendPacketToUin(pTempInfo->m_tTeamsEx[i].dwUin,pPacket);
//				}
//			}
//		}
//	}
//	m_bIsWaiting = true;
}

void TeamData::RefreshTKData(BYTE	byRtlNeed, DWORD dwUin, WORD wItemID)
{
	for (int i = 0; i < m_pTKDropList->Count; i++)
	{
		TeamKillDrop* pTempInfo = (TeamKillDrop*)m_pTKDropList->Items[i];
		if (byRtlNeed&&pTempInfo->m_nItemID == wItemID)
		{
			for (int i = 0; i < TEAM_MAXUSER; i++)
			{
				if (pTempInfo->m_tTeamsEx[i].dwUin == dwUin&&!pTempInfo->m_tTeamsEx[i].byNeed)
				{
					pTempInfo->m_tTeamsEx[i].byNeed = byRtlNeed;
				}
			}
		}
	}
}

TeamKillDrop* TeamData::CheckSendTDItem()
{
	for (int i = 0; i < m_pTKDropList->Count; i++)
	{
		TeamKillDrop* pTempInfo = (TeamKillDrop*)m_pTKDropList->Items[i];
		if (pTempInfo)
		{
			for (int i=0;i<TEAM_MAXUSER;i++)
			{
				if (pTempInfo->m_tTeamsEx[i].dwUin)
				{
					TeamUser* pUserData = GetTeamUser(pTempInfo->m_tTeamsEx[i].dwUin);
					if (!pUserData||(pUserData&&!pUserData->bOnline))
						pTempInfo->m_tTeamsEx[i].byNeed = 2;
					else if (pUserData&&pUserData->bOnline&&!pTempInfo->m_tTeamsEx[i].byNeed)
						return NULL;
				}
			}
			return pTempInfo;
		}
	}
	return NULL;
}

DWORD TeamData::GetWinnerUin(TeamKillDrop* pTempInfo,xnList* pNeedList)
{
	DWORD nTeamerUin = 0;
	switch(pNeedList->Count)
	{
	case 0:
		{
			bool bCnt = false;
			for (int i=0;i<TEAM_MAXUSER;i++)
			{
				if (pTempInfo->m_tTeamsEx[i].dwUin)
				{
					TeamUser* pUserData = GetTeamUser(pTempInfo->m_tTeamsEx[i].dwUin);
					if (pUserData&&pUserData->bOnline)
					{
						nTeamerUin = pTempInfo->m_tTeamsEx[i].dwUin;
						bCnt = true;
					}
				}
			}
			if (bCnt == false)
				nTeamerUin = 123;
		}
		break;
	case 1:
		{
			TeamKillDropEx* pTempEx = (TeamKillDropEx*)pNeedList->Items[0];
			nTeamerUin = pTempEx->dwUin;
		}
		break;
	case 2:
		{
			TeamKillDropEx* pTempEx = (TeamKillDropEx*)pNeedList->Items[0];
			TeamKillDropEx* pTempEx1 = (TeamKillDropEx*)pNeedList->Items[1];
			nTeamerUin = pTempEx->nRand > pTempEx1->nRand?pTempEx->dwUin:pTempEx1->dwUin;
		}
		break;
	}
	if (pNeedList->Count > 2)
	{
		int temp = 0;
		for (int j = 0; j < pNeedList->Count; j++)
		{
			TeamKillDropEx* pTempEx = (TeamKillDropEx*)pNeedList->Items[j];
			temp = pTempEx->nRand > temp?pTempEx->nRand:temp;
		}
		for (int k = 0; k < pNeedList->Count; k++)
		{
			TeamKillDropEx* pTempEx = (TeamKillDropEx*)pNeedList->Items[k];
			if (temp == pTempEx->nRand)
			{
				nTeamerUin = pTempEx->dwUin;
			}
		}
	}
	return nTeamerUin;
}


void TeamData::SendTDItemAdd()
{
	TeamKillDrop* pTempInfo = CheckSendTDItem();
	if (pTempInfo == NULL)
		return;

	xnList*	pNeedMembers	= xnList::Create();
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (pTempInfo->m_tTeamsEx[i].byNeed == 1)
		{
			TeamUser* pUserData = GetTeamUser(pTempInfo->m_tTeamsEx[i].dwUin);
			if (pUserData&&pUserData->bOnline)
				pNeedMembers->Add((void*)&pTempInfo->m_tTeamsEx[i]);
		}
	}
	DWORD nTeamerUin = GetWinnerUin(pTempInfo,pNeedMembers);
	if (nTeamerUin == 123)
	{
		pNeedMembers->Free();
		delete (TeamKillDrop*)m_pTKDropList->Delete(0);
		return;
	}
	TeamUser* pUserData = GetTeamUser(nTeamerUin);
	if (pUserData&&pUserData->dwUin&&pUserData->bOnline)
	{
		ItemExBase* pItem = ItemConfig::GetInstance().GetItemBase(pTempInfo->m_nItemID);

		for (int m = 0; m < TEAM_MAXUSER; m++)
		{
			TeamUser* pTeamsUserData = GetTeamUser(pTempInfo->m_tTeamsEx[m].dwUin);
			if (pTeamsUserData&&pTeamsUserData->dwUin&&pTeamsUserData->bOnline)
			{
				if (pTempInfo->m_nNeedCnt != 1)
				{
					for (int n = 0; n < TEAM_MAXUSER; n++)
					{
						TeamUser* pTeamsUsersData = GetTeamUser(pTempInfo->m_tTeamsEx[n].dwUin);
						if (pTeamsUsersData&&pTeamsUsersData->dwUin&&pTeamsUsersData->bOnline)
						{
							if (pTempInfo->m_tTeamsEx[n].byNeed == 1)
							{
								pTeamsUserData->pUser->SendMsg("%s掷出了：%d点",pTeamsUsersData->pUser->m_szNick,pTempInfo->m_tTeamsEx[n].nRand);
							}else if (pTempInfo->m_tTeamsEx[m].byNeed == 2)
							{
								pTeamsUserData->pUser->SendMsg("%s放弃了掷点",pTeamsUsersData->pUser->m_szNick);
							}
						}
					}
				}
				pTeamsUserData->pUser->SendMsg("%s获得了：%s",pUserData->pUser->m_szNick,pItem->szName);
			}
		}
		int	nItemID	= pTempInfo->m_nItemID;
		delete (TeamKillDrop*)m_pTKDropList->Delete(0);
		pUserData->pUser->SendKillDrop(nItemID);
		pUserData->pUser->m_pItemCenter->AddItemByID(nItemID,1,0,ITEM_LOG_DROP);

		
		m_bIsWaiting = false;
	}
	pNeedMembers->Free();
}

bool TeamData::IsNew()
{
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (m_xUser[i].dwUin&&m_xUser[i].nState == TEAM_LEADER)
		{
			return false;
		}
	}
	return true;
}

char* TeamData::GetTeamLeaderName()
{
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (m_xUser[i].dwUin&&m_xUser[i].nState == TEAM_LEADER)
		{
			return m_xUser[i].szName;
		}
	}
	return "";
}

DWORD TeamData::GetTeamLeaderUin()
{
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (m_xUser[i].dwUin&&m_xUser[i].nState == TEAM_LEADER)
		{
			return m_xUser[i].dwUin;
		}
	}
	return m_xUser[0].dwUin;
}

TeamUser* TeamData::GetTeamLeader( )
{
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (m_xUser[i].dwUin&&m_xUser[i].nState == TEAM_LEADER)
		{
			return &m_xUser[i];
		}
	}
	return NULL;
}

TeamUser* TeamData::GetTeamUser( DWORD dwUin )
{
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (m_xUser[i].dwUin == dwUin)
		{
			return &m_xUser[i];
		}
	}
	return NULL;
}

TeamUser* TeamData::GetTeamUser( BYTE nPos )
{
	if (nPos<0||nPos>=TEAM_MAXUSER)
	{
		return NULL;
	}
	return &m_xUser[nPos];
}

void TeamData::SendTeamUser( int nPos,DWORD dwUin )
{
	if (nPos<0||nPos>=TEAM_MAXUSER)
	{
		return;
	}
	BuildPacketEx(pPacket, USER, S_TEAM_INFO, buf, 1024);
	SET_DATA(pSend, USER, S_TEAM_INFO, pPacket);
	pSend->xUser = m_xUser[nPos];
	TCP_SendPacketToUin(dwUin,pPacket);
}

void TeamData::SendTeamState(BYTE nType,DWORD dwUin,char* szMsg /*= NULL*/ ,BYTE byPos)
{
	BuildPacketEx(pPacket, USER, CS_TEAM_STATE, buf, 1024);
	SET_DATA(pSend, USER, CS_TEAM_STATE, pPacket);
	pSend->byType = nType;
	pSend->byPos = byPos;
	//pSend->nTeamID = nTeamID;
	if (szMsg)
	{
		int nSize = strlen(szMsg)+1;
		pPacket->wSize += nSize;
		memcpy(pSend->szMsg,szMsg,nSize);
	}
	TCP_SendPacketToUin(dwUin,pPacket);
}

void TeamData::SendTeamState( BYTE nType,DWORD dwUin,BYTE byVal )
{
	BuildPacketEx(pPacket, USER, CS_TEAM_STATE, buf, 1024);
	SET_DATA(pSend, USER, CS_TEAM_STATE, pPacket);
	pSend->byType = nType;
	pSend->byPos = byVal;
	//pSend->nTeamID = nTeamID;
	pSend->szMsg[0] = byVal;
	TCP_SendPacketToUin(dwUin,pPacket);
}

void TeamData::ChangeLeader( int nPos )
{
	if (nPos<0||nPos>=TEAM_MAXUSER)
	{
		return;
	}
	if (m_xUser[nPos].nState != TEAM_NORMAL)
	{
		return;
	}
	if (!m_xUser[nPos].bOnline)
	{
		return;
	}
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (m_xUser[i].dwUin &&m_xUser[i].nState == TEAM_LEADER)
		{
			m_xUser[i].nState = TEAM_NORMAL;
			SendTeamUserToAll(i);
		}
	}
	m_xUser[nPos].nState = TEAM_LEADER;
	m_bChgLeader = true;
	SendTeamUserToAll(nPos);
}

void TeamData::SendTeamUserToAll( int nPos )
{
	if (nPos<0||nPos>=TEAM_MAXUSER)
	{
		return;
	}
	if (m_bChgLeader)
	{
		m_bChgLeader = false;
		for (int j=0;j<TEAM_MAXUSER;j++)
		{
			if (m_xUser[j].dwUin&&m_xUser[j].bOnline&&m_xUser[j].nState != TEAM_CREATE)
			{
				m_xUser[j].pUser->SendWarnMsg("更换%s为队长",m_xUser[nPos].szName);
			}
		}
	}
	BuildPacketEx(pPacket, USER, S_TEAM_INFO, buf, 1024);
	SET_DATA(pSend, USER, S_TEAM_INFO, pPacket);
	pSend->xUser = m_xUser[nPos];
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (m_xUser[i].dwUin&&m_xUser[i].nState != TEAM_CREATE)
		{
			TCP_SendPacketToUin(m_xUser[i].dwUin,pPacket);
		}
	}
	
}

void TeamData::OnlineChg( int nPos,bool bOnline,UserData* pUser )
{
	if (nPos<0||nPos>=TEAM_MAXUSER)
	{
		return;
	}
	m_xUser[nPos].bOnline = bOnline;
	if(bOnline == false)
		m_xUser[nPos].pUser	= NULL;
	else
		m_xUser[nPos].pUser	= pUser;

	if (!bOnline&&m_xUser[nPos].nState == TEAM_LEADER)
	{
		for (int i=0;i<TEAM_MAXUSER;i++)
		{
			if (m_xUser[i].dwUin&&i!=nPos&&m_xUser[i].bOnline)
			{
				ChangeLeader(i);
				SendTeamUserToAll(i);
				break;
			}
		}
	}
	SendTeamUserToAll(nPos);
	if(bOnline)
		RecoverTeamInfo(nPos);
}

void TeamData::RecoverTeamInfo(int nPos)
{
	if (nPos<0||nPos>=TEAM_MAXUSER)
	{
		return;
	}
	BuildPacketEx(pPacket, USER, S_TEAM_INFO, buf, 1024);
	SET_DATA(pSend, USER, S_TEAM_INFO, pPacket);
	
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (m_xUser[i].dwUin&&m_xUser[i].dwUin!=m_xUser[nPos].dwUin&&m_xUser[i].nState != TEAM_CREATE)
		{
			pSend->xUser = m_xUser[i];
			TCP_SendPacketToUin(m_xUser[nPos].dwUin,pPacket);
		}
	}
}

int TeamData::GetPosByUin( DWORD dwUin )
{
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (m_xUser[i].dwUin&&m_xUser[i].dwUin == dwUin)
		{
			return i;
		}
	}
	return -1;
}

int TeamData::GetTeamUserCount(bool bInv)
{
	int nCount = 0;
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (m_xUser[i].dwUin)
		{
			if(!bInv && m_xUser[i].nState	!= TEAM_CREATE)
				nCount++;
		}
	}
	return nCount;
}

int TeamData::GetTeamUserMapCnt( int nMapIndex )
{
	int nCount = 0;
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (m_xUser[i].pUser && m_xUser[i].pUser->m_nMIndex == nMapIndex)
		{
			if(m_xUser[i].nState	!= TEAM_CREATE)
				nCount++;
		}
	}
	return nCount;
}

int TeamData::TeamKillEnemy( WORD wEnemyID,int nMIndex,DWORD dwMyUin)
{
	_EnemyAward* pEA	= EnemyKillConfig::GetInstance().GetEAward(wEnemyID);
	if(pEA)
		pEA->RandGetItem(m_pItemList);
	int	nTeamCnt	= GetTeamUserMapCnt(nMIndex);
	WORD	wExpPer	= 10000;
	if(nTeamCnt > 1)
		wExpPer	= 10000/nTeamCnt+500;
	for (int i = 0; i < TEAM_MAXUSER; i++)
	{
		if (m_xUser[i].pUser && m_xUser[i].nState != TEAM_CREATE && m_xUser[i].pUser->m_nMIndex == nMIndex)
		{
			if (m_xUser[i].dwUin != dwMyUin)
				m_xUser[i].pUser->CheckMCopyOk(wEnemyID);
		
			m_xUser[i].pUser->m_pQuestCenter->KillEnemy(wEnemyID);
			if(!pEA)
				continue;

			int nTotalExp	= pEA->CalcExp(m_xUser[i].pUser->m_xUWealth.GetWealth(UW_LV));
			//int	nExp	= m_xUser[i].pUser->m_pVIPCenter->GetExpAdd(/*nTotalExp + */nTotalExp*wExpPer/10000);
			int	nExp	=m_xUser[i].pUser->m_pUserExpCenter->GetExp(nTotalExp*wExpPer/10000,0);
			if(nExp <= 0)
				nExp	= 1;
			m_xUser[i].pUser->m_xUWealth.AddWealth(UW_EXP,nExp);
			if(m_xUser[i].pUser->m_pSlaveCenter->m_pActiveSlave)
			{
				int nPetExp = pEA->CalcExp(m_xUser[i].pUser->m_pSlaveCenter->m_pActiveSlave->m_byLevel);
				nExp	= nPetExp*wExpPer/10000;
				nExp	= m_xUser[i].pUser->m_pUserExpCenter->GetExp(nExp,1);
				if(nExp <= 0)
					nExp	= 1;
				m_xUser[i].pUser->m_pSlaveCenter->m_pActiveSlave->AddExp(nExp);
			}
			if(m_xUser[i].pUser->m_pUserExpCenter->GetObj(1102))
			{
				int nFlag = m_xUser[i].pUser->m_pUserFlag->GetFlag(26);
				if(nFlag < 1500)
				{
					MarryData* pMarry = m_xUser[i].pUser->m_pUserMarry->GetRelation();
					if(pMarry && m_xUser[i].pUser->m_pUserMarry->MarryExpAdd())
					{
						if(m_xUser[i].pUser->m_bySex == 1)
						{
							pMarry->m_nMEnemyCnt ++;
							if(pMarry->m_nMEnemyCnt >= 5)
							{
								pMarry->m_nMEnemyCnt = 0;
								g_pMarryCenter->AddMarryValue(m_xUser[i].pUser,pMarry->m_dwID,1,true);
								m_xUser[i].pUser->m_pUserFlag->AddFlag(26,1);
							}
						}
						else
						{
							pMarry->m_nWEnemyCnt ++;
							if(pMarry->m_nWEnemyCnt >= 5)
							{
								pMarry->m_nWEnemyCnt = 0;
								g_pMarryCenter->AddMarryValue(m_xUser[i].pUser,pMarry->m_dwID,1,false);
								m_xUser[i].pUser->m_pUserFlag->AddFlag(26,1);
							}
						}
					}
				}
			}
		}
	}
	if(pEA)
		return pEA->nGold;

	return 0;
}

TeamData* TeamCenter::CreateTeam( UserData* pUser )
{
	if (!pUser)
	{
		return NULL;
	}
	TeamData* pData = new TeamData();
	m_pTeamList->Add(pData);
	pData->nTeamID = _getTeamID();
	pData->Join(pUser,TEAM_LEADER);
	return pData;
}

int TeamCenter::_getTeamID()
{
// 	if (!m_pTeamList->Count)
// 	{
// 		return 1;
// 	}else{
// 		TeamData* pData = (TeamData*)m_pTeamList->Items[m_pTeamList->Count-1];
// 		return pData->nTeamID+1;
// 	}

	return m_nTeamMaxID++;
}

void TeamCenter::Update()
{
	for (int i=0;i<m_pTeamList->Count;i++)
	{
		TeamData* pData = (TeamData*)m_pTeamList->Items[i];
		if (!pData->Update())
		{
			for (int i=0;i<TEAM_MAXUSER;i++)
			{
				if(pData->m_xUser[i].dwUin) pData->Leave(i);
			}
			m_pTeamList->Delete(i);
			delete pData;
		}
	}
}

TeamCenter::TeamCenter()
{
	m_pTeamList = xnList::Create();
	m_nTeamMaxID = 1000;
}

TeamCenter::~TeamCenter()
{
	delete (TeamData*)m_pTeamList->Delete(0);
}

TeamData* TeamCenter::GetTeamData( DWORD dwUin )
{
	for (int i=0;i<m_pTeamList->Count;i++)
	{
		TeamData* pData = (TeamData*)m_pTeamList->Items[i];
		if (pData->GetTeamUser(dwUin))
		{
			return pData;
		}
	}
	return NULL;
}
