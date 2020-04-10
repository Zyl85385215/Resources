#include "stdafx.h"
#include "UserList.h"
#include "DBControl.h"
#include "FsGameCenter.h"
#include "FamilyCenter.h"
UserList*	g_pUserList;

UserSimple::UserSimple()
{
	szName	= NULL;
	pHomeList = xnList::Create();
	bOnline = false;
	byHomeDuty = 0;
	wHomeID = 0;
	nContribution = 0;
	nMentoringVal = 0;
	nMentoringTick = 0;
	dwTotalRmb = 100;
	nMuliPay   = 0;
	nAccountUin = 0;
	ifDelete = false;
}

UserSimple::~UserSimple()
{
	if(szName)
		free(szName);
	pHomeList->Free();
}

void UserSimple::RemoveHome( HomeObj* pObj )
{
	for (int i=0;i<pHomeList->Count;i++)
	{
		if (pObj == (HomeObj*)pHomeList->Items[i])
		{
			pHomeList->Delete(i);
			pObj->RequestCancelJoin(this);
			return;
		}
	}
}

void UserSimple::RemoveAllHome()
{
	for (int i=pHomeList->Count-1;i>=0;i--)
	{
		HomeObj* pObj = (HomeObj*)pHomeList->Items[i];
		pHomeList->Delete(i);
		pObj->RequestCancelJoin(this);
	}
}

static int SortByUin(const void * Item1, const void * Item2)
{
	UserSimple * pData1 = *(UserSimple **)Item1;
	UserSimple * pData2 = *(UserSimple **)Item2;
	return (pData1->dwUin - pData2->dwUin);
}
static int SortByName(const void * Item1, const void * Item2)
{
	UserSimple * pData1 = *(UserSimple **)Item1;
	UserSimple * pData2 = *(UserSimple **)Item2;
	return strcmp(pData1->szName , pData2->szName);
}

static int FindUserByUin(const void * Item1, const void * Item2)
{
	int id = *(int*)Item1;
	UserSimple * pData = *(UserSimple **)Item2;
	return (id - pData->dwUin);
}

static int FindUserByAcc(const void * Item1, const void * Item2)
{
	int id = *(int*)Item1;
	UserSimple * pData = *(UserSimple **)Item2;
	return (id - pData->nAccountUin);
}

static int FindUserByName(const void * Item1, const void * Item2)
{
	char* id = *(char**)Item1;
	UserSimple * pData = *(UserSimple **)Item2;
	return strcmp(id,pData->szName);
}


UserList::UserList()
{
	m_pListUsers	= xnList::Create();
	m_pListUsersSortByName = xnList::Create();
	m_dwMaxUin		= 10000;
	//m_pListUsers->Sort(SortByUin);
	//UserSimple* pUS	= (UserSimple*)m_pListUsers->Search(&dwUin, FindUserByUin);
}

UserList::~UserList()
{
	while(m_pListUsers->Count)
		delete (UserSimple*)m_pListUsers->Delete(0);

	m_pListUsers->Free();
	m_pListUsersSortByName->Free();
}

int UserList::GetMaxUserID( DBIO* pDB )
{
	QueryRlt xRlt;
	if(!pDB->Query(&xRlt,
		"select Max(Uin) as maxUin from "RPG_USER_TABLE))
	{
		return 10000;
	}
	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd != NULL)
	{
		if(pCmd->FetchRow())
		{
			m_dwMaxUin = pCmd->GetInt("maxUin") + 1;
		}
	}
	if(m_dwMaxUin < 10000)
		m_dwMaxUin	= 10000;
	return m_dwMaxUin;
}

void UserList::DBInit( DBIO* pDB )
{
	GetMaxUserID(pDB);
	QueryRlt xRlt;
	if(!pDB->Query(&xRlt,"SELECT * FROM "RPG_USER_TABLE))
		return;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd == NULL)
		return;

	tm timeNow = CMTime::GetCurTime();
	while(pCmd->FetchRow())
	{
		BYTE	byLv	= pCmd->GetInt("lvl");
		CMTime	timeOut	= CMTime(pCmd->GetStr("LogoutTime"));
		if(byLv < 10 && ((timeNow.tm_year-timeOut[time_year])*365+timeNow.tm_yday-timeOut[time_yeardays]) > 30)
			continue;	//需要先处理婚姻和师徒等处的引用

		UserSimple*	pUS	= new UserSimple();
		pUS->dwUin		= pCmd->GetDWORD("uin");
		pUS->byHomeDuty	= pCmd->GetInt("homeduty");
		pUS->wHomeID	= pCmd->GetWORD("homeid");
		pUS->bLv		= pCmd->GetInt("lvl");
		pUS->bJob		= pCmd->GetInt("job");
		pUS->bSex		= pCmd->GetInt("sex");
		pUS->bVipLv		= pCmd->GetInt("viplvl");
		pUS->bCountry		= pCmd->GetInt("country");
		pUS->nContribution = pCmd->GetInt("homecontribution");
		pUS->szName		= strdup(pCmd->GetStr("Nickname"));
		pUS->nMentoringVal = pCmd->GetInt("mentoringval");
		pUS->nMentoringTick = pCmd->GetInt("mentoringtick");
		//int nLen = 0;
		//int* pData = (int*)pCmd->GetBLOB("flag",&nLen);
		//if(pData&&nLen){
		//	int nVal = nLen/4>BYTE_DATA_LEAGUE_CONTRIBUTE?pData[BYTE_DATA_LEAGUE_CONTRIBUTE]:0;
		//	pUS->nContribution = nVal<<8>>8;
		//}
		m_pListUsers->Add(pUS);
		m_pListUsersSortByName->Add(pUS);
		if (pUS->wHomeID)
		{
			HomeObj* pObj = g_pGameCenter->m_pHomeCenter->_getHomeObj(pUS->wHomeID);
			if (pObj)
			{
				pObj->AddMemberFromDB(pUS);
			}
		}
		if (pCmd->GetInt("everforbid"))
			g_pGameCenter->m_mapEverForbid[pUS->dwUin] = true;
	}

	m_pListUsers->Sort(SortByUin);
	m_pListUsersSortByName->Sort(SortByName);
}

UserSimple* UserList::GetUser( DWORD dwUin )
{
	void*	pFind	= m_pListUsers->Search(&dwUin, FindUserByUin);
	if(pFind == NULL)
		return NULL;
	UserSimple* pUS	= *(UserSimple**)pFind;
	return pUS;
}

UserSimple* UserList::GetUserByAcc( DWORD dwAcc )
{
	void*	pFind	= m_pListUsers->Search(&dwAcc, FindUserByAcc);
	if(pFind == NULL)
		return NULL;
	UserSimple* pUS	= *(UserSimple**)pFind;
	return pUS;
}

UserSimple* UserList::GetUser( char* szName )
{
	void*	pFind	= m_pListUsersSortByName->Search(&szName, FindUserByName);
	if(pFind == NULL)
		return NULL;
	UserSimple* pUS	= *(UserSimple**)pFind;
	return pUS;
}

void	UserList::DeleteUser(DWORD dwUin)
{
	for(int i = 0; i < m_pListUsers->Count; i ++)
	{
		UserSimple* pUs = (UserSimple*)m_pListUsers->Items[i];
		if(pUs->dwUin == dwUin)
		{
			m_pListUsers->Delete(i);
			pUs->ifDelete = true;
			break;
		}
	}
	for(int i = 0; i < m_pListUsersSortByName->Count; i ++)
	{
		UserSimple* pUs = (UserSimple*)m_pListUsersSortByName->Items[i];
		if(pUs->dwUin == dwUin)
		{
			m_pListUsersSortByName->Delete(i);
			break;
		}
	}
}

void	UserList::SetTotalRmb(DWORD dwAcc,int nVal)
{
	for(int i = 0; i < m_pListUsers->Count; i ++)
	{
		UserSimple* pUs = (UserSimple*)m_pListUsers->Items[i];
		if(pUs->nAccountUin == dwAcc)
			pUs->dwTotalRmb = nVal;
	}
}

void UserList::AddUser( UserSimple* pUser )
{
	m_pListUsers->Add(pUser);
	m_pListUsers->Sort(SortByUin);
	m_pListUsersSortByName->Add(pUser);
	m_pListUsersSortByName->Sort(SortByName);
}
