#include "stdafx.h"
#include "FamilyObject.h"
#include "UserList.h"
#include "FsGameCenter.h"
#include "FamilyCenter.h"
#include "ChatCenter.h"
#include "ios_home_packet.h"
HomeObj::HomeObj()
{
	m_pList			= xnList::Create();
	m_pListRequest	= xnList::Create();
	m_pListActiveUS	= xnList::Create();
	m_pListBattle = xnList::Create();
	m_pBattleResult = NULL;

	m_pBuildTag = new HomeBuildTag;
	memset(m_pBuildTag,0,sizeof(HomeBuildTag));
	m_ifLvUp	= false;
	m_ifMemberCh= false;
}

HomeObj::~HomeObj()
{
	m_pListRequest->Free();
	m_pList->Free();
	m_pListActiveUS->Free();
	m_pListBattle->Free();
	if (m_pBuildTag)
	{
		delete m_pBuildTag;
	}
}

bool HomeObj::RequestJoin( UserSimple* pUs )
{
	if(m_pListRequest->Count > 10)
		return false;

	if(pUs->wHomeID != 0)
		return false;
	if(CheckJoin(pUs->dwUin)){
		return false;
	}
// 	for (int i=0;i<m_pListRequest->Count;i++)
// 	{
// 		UserSimple* pInfo = (UserSimple*)m_pListRequest->Items[i];
// 		if (pInfo == pUs)
// 		{
// 			return false;
// 		}
// 	}
	m_pListRequest->Add(pUs);

	return true;
}

bool HomeObj::RemoveMember( UserSimple* pUS)
{
	int nRtl	= m_pList->Remove(pUS);

	if(nRtl != -1)
	{
		m_ifMemberCh = true;
		return true;
	}
	return false;
	//同步

}

void HomeObj::AddMemberFromDB( UserSimple* pUS )
{
	m_pList->Add(pUS);
}

static int nMemberLimit[] = {0,30,35,40,45,50,55,60,65,70,75};

bool HomeObj::AcceptRequest( UserSimple* pUS )
{
	if(m_pList->Count > nMemberLimit[m_byLv]-1)
		return false;
	RemoveRequest(pUS);
	for (int i=0;i<m_pList->Count;i++)
	{
		UserSimple* pTemp = (UserSimple*)m_pList->Items[i];
		if (pTemp == pUS)
		{
			return false;
		}
	}
	UserData* pUser = g_pGameCenter->GetPlayer(pUS->dwUin);
	if(pUser)
	{
		AddActiveMember(pUS->dwUin);
		pUser->m_pUserRecord->RunLuaFamilyFunc(1,m_byLv);
	}
	m_pList->Add(pUS);
	m_ifMemberCh = true;
	//发送同步包,存数据库
	return true;
}

void HomeObj::AddHomeExp( int nVal )
{
//	static int nNeedExp[] = {250,500,1000,2000,4000,8000,16000,32000,64000,0};
	m_dwExp += nVal;
// 	while(nNeedExp[m_byLv]&&m_dwExp>=nNeedExp[m_byLv]){
// 		m_byLv++;
// 		m_dwExp -= nNeedExp[m_byLv];
// 		m_ifLvUp = true;
// 	}
}

bool HomeObj::RemoveRequest( UserSimple* pUs )
{
	for (int i=0;i<m_pListRequest->Count;i++)
	{
		UserSimple* pInfo = (UserSimple*)m_pListRequest->Items[i];
		if (pInfo == pUs)
		{
			m_pListRequest->Remove(pUs);
			return true;
		}
	}
	return false;
}

char* HomeObj::GetMasterName()
{
	for (int i=0;i<m_pList->Count;i++)
	{
		UserSimple* pInfo = (UserSimple*)m_pList->Items[i];
		if (pInfo->byHomeDuty == HOME_DUTY_BOSS)
		{
			return pInfo->szName;
		}
	}
	return "";
}

int HomeObj::GetDutyCount( BYTE byDuty )
{
	int nCount = 0;
	for (int i=0;i<m_pList->Count;i++)
	{
		UserSimple* pInfo = (UserSimple*)m_pList->Items[i];
		if (pInfo->byHomeDuty == byDuty)
		{
			nCount++;
		}
	}
	return nCount;
}

void HomeObj::RemoveActiveMember( DWORD dwUin )
{
	m_pListActiveUS->Remove((void*)dwUin);
}

void HomeObj::AddActiveMember( DWORD dwUin )
{
	m_pListActiveUS->Add((void*)dwUin);
	UserData* pUser = g_pGameCenter->GetPlayer(dwUin);
	if(pUser)
	{
		pUser->m_pUserRecord->RunLuaFamilyFunc(1,m_byLv);
		pUser->m_pUserRecord->RunLuaFamilyFunc(2,m_byLv);
	}
}

int HomeObj::GetOccupyCount()
{
	int nCount = 0;
	for (int i=0;i<7;i++)
	{
		if(g_pGameCenter->m_pHomeCenter->m_Battle.Pos[i].pHome == this) nCount++;

	}
	return nCount;
}

void HomeObj::LoadBuildTag( BYTE* pData,int nSize )
{
	memcpy(m_pBuildTag,pData,nSize>sizeof(HomeBuildTag)?sizeof(HomeBuildTag):nSize);
}

bool HomeObj::AddBuildLv( BYTE byType)
{
	if (byType>=HOME_BUILD_MAX)
	{
		return false;
	}
	static int nCost[] = {50,100,200,400,800,1600,3200,6400,12800,25600};
	int nNowLv = m_pBuildTag->nBuildLv[byType];
	if (nNowLv>=nMaxBuildLv||nNowLv>=m_byLv)
	{
		return false;
	}
	if (m_dwExp < nCost[nNowLv])
	{
		return false;
	}
	m_dwExp -= nCost[nNowLv];
	nNowLv++;
	m_pBuildTag->nBuildLv[byType] = nNowLv;
	return true;
}

bool HomeObj::CheckJoin( DWORD dwUin )
{
	for (int i=0;i<m_pListRequest->Count;i++)
	{
		UserSimple* pInfo = (UserSimple*)m_pListRequest->Items[i];
		if (pInfo->dwUin == dwUin)
		{
			return true;
		}
	}
	return false;
}

bool HomeObj::AddHomeLv()
{
	static int nNeedExp[] = {0,250,500,1000,2000,4000,8000,16000,32000,64000,0};
	if (m_byLv>=10)
	{
		return false;
	}
	if (m_dwExp>=nNeedExp[m_byLv])
	{
		for(int i = 0; i < m_pListActiveUS->Count; i ++)
		{
			DWORD pUSUin = (DWORD)m_pListActiveUS->Items[i];
			if (pUSUin)
			{
				UserData* pUser = g_pGameCenter->GetPlayer(pUSUin);
				if(pUser)
					pUser->m_pUserRecord->RunLuaFamilyFunc(2,m_byLv);
			}
		}
		m_dwExp -= nNeedExp[m_byLv];
		m_byLv++;

		char szMsg[128];
		sprintf(szMsg,"众志成城力量大！%s军团全体成员团结一致、齐心协力，终于将军团升级到了%d级！",m_szName,m_byLv);
		ChatCenter::GetInstance().SendSystemMsg(szMsg,0,0,1);

		m_ifLvUp = true;
		return true;
	}
	return false;
	// 	while(nNeedExp[m_byLv]&&m_dwExp>=nNeedExp[m_byLv]){
	// 		m_byLv++;
	// 		m_dwExp -= nNeedExp[m_byLv];
	// 		m_ifLvUp = true;
	// 	}
}

bool HomeObj::RequestCancelJoin( UserSimple* pUs )
{

// 	if(pUs->wHomeID != 0)
// 		return false;

	for (int i=0;i<m_pListRequest->Count;i++)
	{
		UserSimple* pInfo = (UserSimple*)m_pListRequest->Items[i];
		if (pInfo == pUs)
		{
			m_pListRequest->Delete(i);
			return true;
		}
	}

	return false;
}

bool HomeObj::CheckChangeLeader()
{
	UserSimple* pSelectData = NULL;
	UserSimple* pLeader = NULL;
	for (int i=0;i<m_pList->Count;i++)
	{
		UserSimple* pData = (UserSimple*)m_pList->Items[i];
		if (pData->byHomeDuty != HOME_DUTY_BOSS&&(!pSelectData ||pSelectData->byHomeDuty<pData->byHomeDuty))
		{
			pSelectData = pData;
		}else if(pData->byHomeDuty == HOME_DUTY_BOSS){
			pLeader = pData;
		}
	}
	if (!pSelectData||!pLeader)
	{
		return false;
	}
	pLeader->byHomeDuty = HOME_DUTY_NORMAL;
	pSelectData->byHomeDuty	= HOME_DUTY_BOSS;
	UserData* pUser = g_pGameCenter->GetPlayer(pSelectData->dwUin);
	if(pUser) pUser->m_dwHomeDuty = HOME_DUTY_NORMAL;
	//保存同步
	g_pGameCenter->m_pHomeCenter->_dbUpdateMember(pLeader);
	g_pGameCenter->m_pHomeCenter->_dbUpdateMember(pSelectData);
	g_pGameCenter->m_pHomeCenter->_netMemberDuty(this,pLeader);
	g_pGameCenter->m_pHomeCenter->_netMemberDuty(this,pSelectData);
	return true;
}
