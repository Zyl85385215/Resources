#include "stdafx.h"
#include "RankCenter.h"
#include "UserData.h"
#include "FsGameCenter.h"
#include "FamilyCenter.h"
#include "xnlist.h"

static bool szifCompare2[] = {false,false,false,true,false,false,false};			//排序时，是否比较第二个参数（财富榜的第一个参数太大了）
int posCompare(const void * Item1, const void * Item2)
{
	Rank_Data* p1 = *(Rank_Data**)Item1;
	Rank_Data* p2 = *(Rank_Data**)Item2;

	return p1->m_nPos-p2->m_nPos;
}

int rankCompare1(const void * Item1, const void * Item2)
{
	Rank_Data* p1 = *(Rank_Data**)Item1;
	Rank_Data* p2 = *(Rank_Data**)Item2;

	return p2->m_dwCheck1-p1->m_dwCheck1;
}

int rankCompare(const void * Item1, const void * Item2)
{
	Rank_Data* p1 = *(Rank_Data**)Item1;
	Rank_Data* p2 = *(Rank_Data**)Item2;

	return (p2->m_dwCheck1-p1->m_dwCheck1)*100 + (p2->m_dwCheck2-p1->m_dwCheck2);
}

RankList::RankList(BYTE byType,DBControl* pDB,funGetInfo func,bool ifSend)
{
	m_byType = (RANKTYPE)byType;
	m_pList = xnList::Create();
	m_pSendList = xnList::Create();
	m_dwMin1 = 0;
	m_dwMin2 = 0;
	m_funGetInfo = func;
	m_pDB = pDB;
	m_ifSendMine = ifSend;
	m_dwValueFlag = 1000;
}

RankList::~RankList()
{
	if(m_pList)
	{
		while(m_pList->Count > 0)
		{
			delete (Rank_Data*)m_pList->Delete(0);
		}
		m_pList->Free();
	}
}

void RankList::AddData(Rank_Data* pData)
{
	if(pData->m_byType != m_byType)
		return;
	if(GetRankData(pData->m_dwID1,pData->m_dwID2))
		return;
	m_pList->Add((void*)pData);
}

void RankList::ReSort()
{
	if(szifCompare2[m_byType])
		m_pList->Sort(rankCompare);
	else
		m_pList->Sort(rankCompare1);
	for(int i = 0; i < m_pList->Count; i ++)
	{
		Rank_Data* pData = (Rank_Data*)m_pList->Items[i];
		if(pData->m_nPos != i)
		{
			pData->m_nPos = i;
			_updatePos(pData,m_pDB);
		}
		if(i >= MAX_RANK_SIZE-1)
		{
			m_dwMin1 = pData->m_dwCheck1;
			m_dwMin2 = pData->m_dwCheck2;
		}
		else
		{
			m_dwMin1 = 0;
			m_dwMin2 = 0;
		}
	}
}

void RankList::SendList(DWORD dwUin)
{
	if(m_pList->Count == 0)
		return;
	ZPacket* pPacket = NULL;
	BYTE buf[2048] = {0};
	RPGRANK_S_SEND_RANKLIST_Data* pSendData = (RPGRANK_S_SEND_RANKLIST_Data*)((ZPacket*)buf)->data;
	int n = 0;
	int count = m_pList->Count < MAX_RANK_VIEW ? m_pList->Count : MAX_RANK_VIEW;
	for(int i = 0; i < count; i ++)
	{
		Rank_Data* pData = (Rank_Data*)m_pList->Items[i];
		memcpy(&pSendData->szData[n],pData,sizeof(Rank_Data));
		n ++;
		if(n == 10 || i == count-1)
		{
			BuildPacketSize(pPacket,RPGRANK,S_SEND_RANKLIST,buf,sizeof(RPGRANK_S_SEND_RANKLIST_Data) + sizeof(Rank_Data)*n);
			pSendData->nCnt = n;
			n = 0;
			TCP_SendPacketToUin(dwUin,pPacket);
		}
	}
	SendMyRank(dwUin);
}

void RankList::SendMyRank()
{
// 	if(!m_ifSendMine)
// 		return;
// 	while(m_pSendList->Count > 0)
// 	{
// 		int nUin = (int)m_pSendList->Delete(0);
// 		Rank_Data* pData = GetRankData(nUin,0);
// 		BuildPacketEx(pPacket,RPGRANK,S_SEND_MYRANK,buf,1024);
// 		SET_DATA(pSendData,RPGRANK,S_SEND_MYRANK,pPacket);
// 		if(pData)
// 		{
// 			memcpy(&pSendData->xData,pData,sizeof(Rank_Data));	
// 		}
// 		else
// 		{
// 			memset(&pSendData->xData,0,sizeof(Rank_Data));
// 			pSendData->xData.m_nPos = 100;
// 		}
// 		TCP_SendPacketToUin(nUin,pPacket);
// 	}
}

void RankList::SendMyRank(DWORD dwUin)
{
	if(!m_ifSendMine)
		return;
	Rank_Data* pData = GetRankData(dwUin,0);
	BuildPacketEx(pPacket,RPGRANK,S_SEND_MYRANK,buf,1024);
	SET_DATA(pSendData,RPGRANK,S_SEND_MYRANK,pPacket);
	if(pData)
	{
		memcpy(&pSendData->xData,pData,sizeof(Rank_Data));	
	}
	else
	{
		memset(&pSendData->xData,0,sizeof(Rank_Data));
		pSendData->xData.m_nPos = 100;
		pSendData->xData.m_byType = m_byType;
	}
	TCP_SendPacketToUin(dwUin,pPacket);
}

Rank_Data* RankList::GetRankData(DWORD id1,DWORD id2)
{
	for(int i = 0; i < m_pList->Count; i ++)
	{
		Rank_Data* pData = (Rank_Data*)m_pList->Items[i];
		if(pData->m_dwID1 == id1 && pData->m_dwID2 == id2)
			return pData;
	}
	return NULL;
}

Rank_Data* RankList::GetRankDatabyPos(int nPos)
{
	if(nPos >= 0 && nPos < m_pList->Count)
		return (Rank_Data*)m_pList->Items[nPos];
	return NULL;
}

bool	RankList::DeleteRank(DWORD id1,DWORD id2)
{
	for(int i = 0; i < m_pList->Count; i ++)
	{
		Rank_Data* pData = (Rank_Data*)m_pList->Items[i];
		if(pData->m_dwID1 == id1 && pData->m_dwID2 == id2)
		{
			m_pList->Delete(i);
			_deleteRankData(pData,m_pDB);
			delete pData;
			m_dwValueFlag++;
			if(m_dwValueFlag < 1000)
				m_dwValueFlag = 1000;

			for(int j = i; j < m_pList->Count; j ++)
			{	
				Rank_Data* pCompa1 = (Rank_Data*)m_pList->Items[j];
				pCompa1->m_nPos --;
			}
			m_pDB->ExecuteSQL(FALSE,"update "RPG_RANKDATA_TABLE" set pos=pos-1 where type=%d and pos>=%d and pos<%d",
				pData->m_byType,i+1,m_pList->Count+1);
			return true;
		}
	}
	return false;
}

bool	RankList::DeleteRank(DWORD id1)
{
	bool ifDelete = false;
	for(int  i = m_pList->Count-1; i >= 0; i --)
	{
		Rank_Data* pData = (Rank_Data*)m_pList->Items[i];
		if(pData->m_dwID1 == id1)
		{
			m_pList->Delete(i);
			_deleteRankData(pData,m_pDB);
			delete pData;
			ifDelete = true;
			m_dwValueFlag++;
			if(m_dwValueFlag < 1000)
				m_dwValueFlag = 1000;
		}
	}
	if(ifDelete)
		ReSort();
	return false;
}

bool RankList::CheckRank(void* pParam)
{
	if(m_funGetInfo == NULL)
		return false;
	bool ifSend = false;
	Rank_Data* pTemp = new Rank_Data();
	memset(pTemp,0,sizeof(Rank_Data));
	m_funGetInfo(pParam,pTemp);
	if(pTemp->m_dwCheck1 == 0)
		return false;
	Rank_Data* pGet = GetRankData(pTemp->m_dwID1,pTemp->m_dwID2);
	if(pGet)
	{
		//if(pGet->m_dwCheck1 != pTemp->m_dwCheck1 || pGet->m_dwCheck2 != pTemp->m_dwCheck2)
		if(memcmp(pGet,pTemp,sizeof(Rank_Data)) != 0)
		{
			ifSend = true;
			int nOldPos = pGet->m_nPos;
			memcpy(pGet,pTemp,sizeof(Rank_Data));
			pGet->m_nPos = nOldPos;
			_updateRankData(pGet,m_pDB);
			delete pTemp;
			for(int i = nOldPos;i > 0; i --)
			{	
				Rank_Data* pCompa1 = (Rank_Data*)m_pList->Items[i];
				Rank_Data* pCompa2 = (Rank_Data*)m_pList->Items[i-1];
				if(pCompa1->m_dwCheck1 > pCompa2->m_dwCheck1 || (pCompa1->m_dwCheck1 == pCompa2->m_dwCheck1 && pCompa1->m_dwCheck2 > pCompa2->m_dwCheck2))
				{
					m_pList->Exchange(i,i-1);
					pCompa1->m_nPos = i-1;
					pCompa2->m_nPos = i;
				}
				else
					break;
			}
			for(int i = nOldPos;i < m_pList->Count -1; i ++)
			{	
				Rank_Data* pCompa1 = (Rank_Data*)m_pList->Items[i];
				Rank_Data* pCompa2 = (Rank_Data*)m_pList->Items[i+1];
				if(pCompa1->m_dwCheck1 < pCompa2->m_dwCheck1 || (pCompa1->m_dwCheck1 == pCompa2->m_dwCheck1 && pCompa1->m_dwCheck2 < pCompa2->m_dwCheck2))
				{
					m_pList->Exchange(i,i+1);
					pCompa1->m_nPos = i+1;
					pCompa2->m_nPos = i;
				}
				else
					break;
			}
			_updatePos(pGet,m_pDB,nOldPos);
		}
	}
	else
	{
		if(pTemp->m_dwCheck1 > m_dwMin1 || (pTemp->m_dwCheck1 == m_dwMin1 && pTemp->m_dwCheck2 > m_dwMin2))
		{
			ifSend = true;

			pTemp->m_nPos = m_pList->Count;
			m_pList->Add((void*)pTemp);
			_insertRankData(pTemp,m_pDB);
			int nOldPos = pTemp->m_nPos;
			for(int i = pTemp->m_nPos;i > 0 && i < m_pList->Count ; i --)
			{	
				Rank_Data* pCompa1 = (Rank_Data*)m_pList->Items[i];
				Rank_Data* pCompa2 = (Rank_Data*)m_pList->Items[i-1];
				if(pCompa1->m_dwCheck1 > pCompa2->m_dwCheck1 || (pCompa1->m_dwCheck1 == pCompa2->m_dwCheck1 && pCompa1->m_dwCheck2 > pCompa2->m_dwCheck2))
				{
					m_pList->Exchange(i,i-1);
					pCompa1->m_nPos = i-1;
					pCompa2->m_nPos = i;
				}
				else
					break;
			}
			_updatePos(pTemp,m_pDB,nOldPos);
			while(m_pList->Count > MAX_RANK_SIZE)
			{
				Rank_Data* pData = (Rank_Data*)m_pList->Items[m_pList->Count-1];
				_deleteRankData(pData,m_pDB);
				delete (Rank_Data*)m_pList->Delete(m_pList->Count-1);
			}	
			if(m_pList->Count >= MAX_RANK_SIZE)
			{
				Rank_Data* pData = (Rank_Data*)m_pList->Items[m_pList->Count-1];
				m_dwMin1 = pData->m_dwCheck1;
				m_dwMin2 = pData->m_dwCheck2;
			}
			else
			{
				m_dwMin1 = 0;
				m_dwMin2 = 0;
			}
		}
		else if(pTemp->m_dwCheck1 == m_dwMin1 && pTemp->m_dwCheck2 == m_dwMin2)
		{
			if(m_pList->Count < MAX_RANK_SIZE)
			{
				Rank_Data* pGet = GetRankData(pTemp->m_dwID1,pTemp->m_dwID2);
				if(pGet == NULL)
				{		
					ifSend = true;
					pTemp->m_nPos = m_pList->Count;
					m_pList->Add((void*)pTemp);
					_insertRankData(pTemp,m_pDB);
					if(m_pList->Count == MAX_RANK_SIZE)
					{
						m_dwMin1 = pTemp->m_dwCheck1;
						m_dwMin2 = pTemp->m_dwCheck2;
					}
					else
					{
						m_dwMin1 = 0;
						m_dwMin2 = 0;
					}

				}
				else
					delete pTemp;
			}
			else
				delete pTemp;
		}
	}
	
	if(ifSend)
	{
		m_dwValueFlag++;
		if(m_dwValueFlag < 1000)
			m_dwValueFlag = 1000;
	}
	return ifSend;
}

void RankList::AddChangeUin(DWORD dwUin)
{
// 	for(int i = 0; i < m_pSendList->Count; i ++)
// 	{
// 		if(dwUin == (DWORD)m_pSendList->Items[i])
// 			return;
// 	}
// 	m_pSendList->Add((void*)dwUin);
}

bool RankList::_insertRankData(Rank_Data* pDatak,DBControl* pDB)
{
	if(pDatak == NULL || pDB == NULL)
		return false;
	if(m_ifSendMine)
		AddChangeUin(pDatak->m_dwID1);
	pDB->ExecuteSQL(FALSE,"insert into "RPG_RANKDATA_TABLE" (id1,id2,type,pos,check1,check2,val1,val2,val3,val4,desc1,desc2,desc3,desc4) values (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s','%s','%s')",
		pDatak->m_dwID1,pDatak->m_dwID2,pDatak->m_byType,pDatak->m_nPos,pDatak->m_dwCheck1,pDatak->m_dwCheck2,pDatak->m_dwVal[0],pDatak->m_dwVal[1],pDatak->m_dwVal[2],pDatak->m_dwVal[3],
		pDatak->m_szDesc[0],pDatak->m_szDesc[1],pDatak->m_szDesc[2],pDatak->m_szDesc[3]);
		return true;
}

bool RankList::_updatePos(Rank_Data* pData,DBControl* pDB,int nOldPos)
{
	if(pData == NULL || pDB == NULL)
		return false;
// 	if(m_ifSendMine)
// 		AddChangeUin(pData->m_dwID1);
	if(nOldPos == pData->m_nPos)
		return false;
	if( pData->m_nPos < nOldPos)
	{
		pDB->ExecuteSQL(FALSE,"update "RPG_RANKDATA_TABLE" set pos=pos+1 where type=%d and pos>=%d and pos<%d",
			pData->m_byType,pData->m_nPos,nOldPos);
	}
	else
	{
		pDB->ExecuteSQL(FALSE,"update "RPG_RANKDATA_TABLE" set pos=pos-1 where type=%d and pos<=%d and pos>%d",
			pData->m_byType,pData->m_nPos,nOldPos);
	}
	pDB->ExecuteSQL(FALSE,"update "RPG_RANKDATA_TABLE" set pos=%d where type=%d and id1=%d and id2=%d",
		pData->m_nPos,pData->m_byType,pData->m_dwID1,pData->m_dwID2);
	return true;
}

bool RankList::_updatePos(Rank_Data* pData,DBControl* pDB)
{
	if(pData == NULL || pDB == NULL)
		return false;
	// 	if(m_ifSendMine)
	// 		AddChangeUin(pData->m_dwID1);
	pDB->ExecuteSQL(FALSE,"update "RPG_RANKDATA_TABLE" set pos=%d where type=%d and id1=%d and id2=%d",
		pData->m_nPos,pData->m_byType,pData->m_dwID1,pData->m_dwID2);
	return true;
}

bool RankList::_updateRankData(Rank_Data* pData,DBControl* pDB)
{
	if(pData == NULL || pDB == NULL)
		return false;
	if(m_ifSendMine)
		AddChangeUin(pData->m_dwID1);
	pDB->ExecuteSQL(FALSE,"update "RPG_RANKDATA_TABLE" set pos=%d,check1=%d,check2=%d,val1=%d,val2=%d,val3=%d,val4=%d,desc1='%s',desc2='%s',desc3='%s',desc4='%s' where type=%d and id1=%d and id2=%d",
		pData->m_nPos,pData->m_dwCheck1,pData->m_dwCheck2,pData->m_dwVal[0],pData->m_dwVal[1],pData->m_dwVal[2],pData->m_dwVal[3],pData->m_szDesc[0],pData->m_szDesc[1],pData->m_szDesc[2],pData->m_szDesc[3],
		pData->m_byType,pData->m_dwID1,pData->m_dwID2);
	return true;
}

bool RankList::_deleteRankData(Rank_Data* pData,DBControl* pDB)
{
	if(pData == NULL || pDB == NULL)
		return false;
	pDB->ExecuteSQL(FALSE,"delete from "RPG_RANKDATA_TABLE" where type=%d and id1=%d and id2=%d",pData->m_byType,pData->m_dwID1,pData->m_dwID2);
	return true;
}

void RankList::Clear()
{
// 	for(int i = m_pList->Count-1; i >0; i --)
// 	{
// 		Rank_Data* pData = (Rank_Data*)m_pList->Items[i];
// 		m_pList->Delete(i);
// 		_deleteRankData(pData,m_pDB);
// 		delete pData;
// 		m_dwValueFlag++;
// 		if(m_dwValueFlag < 1000)
// 			m_dwValueFlag = 1000;
// 
// 		m_pDB->ExecuteSQL(FALSE,"update "RPG_RANKDATA_TABLE" set pos=pos-1 where type=%d and pos>=%d and pos<%d",
// 			pData->m_byType,i+1,m_pList->Count+1);
// 	};
	bool ifDelete = false;
	if(m_pList->Count > 0)
		ifDelete = true;
	while(m_pList->Count)
	{
		delete (Rank_Data*)m_pList->Delete(0);
	}
	if(ifDelete)
	{
		m_dwValueFlag++;
		if(m_dwValueFlag < 1000)
			m_dwValueFlag = 1000;
		m_pDB->ExecuteSQL(FALSE,"delete from "RPG_RANKDATA_TABLE" where type=%d",m_byType);
	}
}

char* szCountryname[] = {"无","联盟","部落"};
char* szJobName[] = {"无","战士","魔法师","火枪手","牧师"};


void LvfuncGetInfo (void* param,Rank_Data* pData)
{
	int nUin = (int)param;
	if(nUin <= 0 || pData == NULL)
		return;
	UserSimple* pUser = g_pUserList->GetUser(nUin);
	if(pUser == NULL)
		return;
	memset(pData,0,sizeof(Rank_Data));
	pData->m_byType		= RANK_LEVEL;
	pData->m_dwCheck1	= pUser->bLv;
	pData->m_dwID1		= pUser->dwUin;
	pData->m_dwVal[0]	= pUser->bLv;
	strcpy(pData->m_szDesc[0],pUser->szName);
	strcpy(pData->m_szDesc[1],szCountryname[pUser->bCountry]);
	strcpy(pData->m_szDesc[2],szJobName[pUser->bJob]);
	if(pUser->wHomeID > 0)
	{
		strcpy(pData->m_szDesc[3],g_pGameCenter->m_pHomeCenter->GetHomeName(pUser->wHomeID));
	}
}

void MoneyfuncGetInfo (void* param,Rank_Data* pData)
{
	int nUin = (int)param;
	if(nUin <= 0 || pData == NULL)
		return;
	UserData* pUser = g_pGameCenter->GetPlayer(nUin);
	if(pUser == NULL)
		return;
	memset(pData,0,sizeof(Rank_Data));
	pData->m_byType		= RANK_MONEY;
	pData->m_dwCheck1	= pUser->m_xUWealth[UW_GOLD];
	pData->m_dwID1		= pUser->m_dwUin;
	pData->m_dwVal[0]	= pUser->m_xUWealth[UW_GOLD];
	strcpy(pData->m_szDesc[0],pUser->m_szNick);
	strcpy(pData->m_szDesc[1],szCountryname[pUser->m_byCountry]);
	strcpy(pData->m_szDesc[2],szJobName[pUser->m_byJob]);
	if(pUser->m_dwHomeID > 0)
	{
		strcpy(pData->m_szDesc[3],g_pGameCenter->m_pHomeCenter->GetHomeName(pUser->m_dwHomeID));
	}
}

void AttackfuncGetInfo (void* param,Rank_Data* pData)
{
	int nUin = (int)param;
	if(nUin <= 0 || pData == NULL)
		return;
	UserData* pUser = g_pGameCenter->GetPlayer(nUin);
	if(pUser == NULL)
		return;
	memset(pData,0,sizeof(Rank_Data));
	pData->m_byType		= RANK_ATTACK;
	pData->m_dwCheck1	= pUser->GetScore();
	pData->m_dwID1		= pUser->m_dwUin;
	pData->m_dwVal[0]	= pData->m_dwCheck1;
	strcpy(pData->m_szDesc[0],pUser->m_szNick);
	strcpy(pData->m_szDesc[1],szCountryname[pUser->m_byCountry]);
	strcpy(pData->m_szDesc[2],szJobName[pUser->m_byJob]);
	if(pUser->m_dwHomeID > 0)
	{
		strcpy(pData->m_szDesc[3],g_pGameCenter->m_pHomeCenter->GetHomeName(pUser->m_dwHomeID));
	}
}

void HomefuncGetInfo (void* param,Rank_Data* pData)
{
	if(param == NULL || pData == NULL)
		return;
	memset(pData,0,sizeof(Rank_Data));
	HomeObj* pHome = (HomeObj*)param;
	pData->m_byType		= RANK_HOME;
	pData->m_dwCheck1	= pHome->m_byLv;
	pData->m_dwCheck2	= pHome->m_pList->Count;
	pData->m_dwID1		= pHome->m_wHomeID;
	pData->m_dwVal[0]	= pHome->m_pList->Count;
	pData->m_dwVal[1]	= pHome->m_byLv;
	strcpy(pData->m_szDesc[0],pHome->m_szName);
	strcpy(pData->m_szDesc[1],szCountryname[pHome->m_byCountry]);
	strcpy(pData->m_szDesc[2],pHome->GetMasterName());
}

void EquipfuncGetInfo (void* param,Rank_Data* pData)
{
	_MuliParamObj* pTemp = (_MuliParamObj*)param;
	if(pTemp)
	{
		if(pTemp->nParam1&& pTemp->nParam2&& pData)
		{
			UserData* pUser = g_pGameCenter->GetPlayer(pTemp->nParam1);
			if(pUser)
			{
				ItemObj* pItem = pUser->m_pItemCenter->GetItemByIndex(pTemp->nParam2);
				if(pItem && pItem->m_pBase->byType == ITEMTYPE_EQUIP)
				{
					memset(pData,0,sizeof(Rank_Data));
					pData->m_byType		= RANK_EQUIP;
					pData->m_dwCheck1	= pItem->GetScore();
					pData->m_dwID1		= pTemp->nParam1;
					pData->m_dwID2		= pTemp->nParam2;
					pData->m_dwVal[0]	= pData->m_dwCheck1;
					strcpy(pData->m_szDesc[0],pUser->m_szNick);
					strcpy(pData->m_szDesc[1],szCountryname[pUser->m_byCountry]);
					strcpy(pData->m_szDesc[2],szJobName[pUser->m_byJob]);
					strcpy(pData->m_szDesc[3],pItem->m_pBase->szName);
				}
			}
		}
		delete pTemp;
	}
}

void HornourfuncGetInfo (void* param,Rank_Data* pData)
{
	int nUin = (int)param;
	if(nUin <= 0 || pData == NULL)
		return;
	UserData* pUser = g_pGameCenter->GetPlayer(nUin);
	if(pUser == NULL)
		return;
	memset(pData,0,sizeof(Rank_Data));
	pData->m_byType		= RANK_HORNOUR;
	pData->m_dwCheck1	= pUser->m_xUWealth[UW_HONOUR];
	pData->m_dwID1		= pUser->m_dwUin;
	pData->m_dwVal[0]	= pData->m_dwCheck1;
	strcpy(pData->m_szDesc[0],pUser->m_szNick);
	strcpy(pData->m_szDesc[1],szCountryname[pUser->m_byCountry]);
	strcpy(pData->m_szDesc[2],szJobName[pUser->m_byJob]);
	if(pUser->m_pTitleCenter->m_pObj)
	{
		if(pUser->m_byCountry == 1)
			strcpy(pData->m_szDesc[3],pUser->m_pTitleCenter->m_pObj->pName1);
		else
			strcpy(pData->m_szDesc[3],pUser->m_pTitleCenter->m_pObj->pName2);
	}
	else
	{
		strcpy(pData->m_szDesc[3],"平民");
	}
}
void DamagefuncGetInfo (void* param,Rank_Data* pData)
{
	_MuliParamObj* pTemp = (_MuliParamObj*)param;
	int nUin = pTemp->nParam1;
	if(nUin <= 0 || pData == NULL){
		delete pTemp;
		return;
	}
	UserSimple* pUser = g_pUserList->GetUser(nUin);
	if(pUser == NULL){
		delete pTemp;
		return;
	}
	memset(pData,0,sizeof(Rank_Data));
	pData->m_byType		= RANK_DAMAGE;
	pData->m_dwCheck1	= pTemp->nParam2;
	pData->m_dwID1		= pUser->dwUin;
	pData->m_dwVal[0]	= pTemp->nParam2;
	strcpy(pData->m_szDesc[0],pUser->szName);
	strcpy(pData->m_szDesc[1],szCountryname[pUser->bCountry]);
	strcpy(pData->m_szDesc[2],szJobName[pUser->bJob]);
	strcpy(pData->m_szDesc[3],g_pGameCenter->m_pHomeCenter->GetHomeName(pUser->wHomeID));
	
	delete pTemp;
}
static funGetInfo sz_funcGetInfo[] = {LvfuncGetInfo,MoneyfuncGetInfo,AttackfuncGetInfo,HomefuncGetInfo,EquipfuncGetInfo,HornourfuncGetInfo,DamagefuncGetInfo};	//刷新数据的函数
static bool szifSendMine[] = {true,true,true,false,false,true,true};				//是否人物榜

RankCenter::RankCenter(DBControl* pDB)
{
	m_pDB = pDB;
	for(int i = 0; i < RANK_MAX; i ++)
	{
		m_szRankList[i] = new RankList(i,m_pDB,sz_funcGetInfo[i],szifSendMine[i]);
		m_szifChange[i] = false;
	}
	m_pEventList = xnList::Create();
	m_pUWList = xnList::Create();
	m_nEventTick = 0;
	m_pLastDamage = NULL;
}

RankCenter::~RankCenter()
{
	if(m_pEventList)
	{
		while(m_pEventList->Count > 0)
			delete m_pEventList->Delete(0);
		m_pEventList->Free();
	}
	if(m_pUWList)
	{
		while(m_pUWList->Count > 0)
			delete (RankUWatchInfo*)m_pUWList->Delete(0);
		m_pUWList->Free();
	}
}

void RankCenter::SendRankList(UserData* pUser)
{
// 	if(pUser == NULL)
// 		return;
// 	for(int i = 0; i < RANK_MAX; i ++)
// 	{
// 		m_szRankList[i]->SendList(pUser->m_dwUin);
// 	}
}

void RankCenter::Update()
{
	while(m_pEventList->Count > 0)
	{
		RankEvent* pEvent = (RankEvent*)m_pEventList->Delete(0);
		if(m_szRankList[pEvent->byType]->CheckRank(pEvent->param))
			m_szifChange[pEvent->byType] = true;
		delete pEvent;
		pEvent = NULL;
		m_nEventTick ++;
		if(m_nEventTick > 30)
		{
			m_nEventTick = 0;
			return;
		}
	}

	for(int i = 0; i < m_pUWList->Count; i ++)
	{
		RankUWatchInfo* pCheck = (RankUWatchInfo*)m_pUWList->Items[i];
		if(pCheck->byState == 1)
		{
			pCheck->byState = 0;
			_insertUW(pCheck);
		}
		else if(pCheck->byState == 2)
		{
			pCheck->byState = 0;
			_updateUW(pCheck);
		}
	}
/*
		for(int i = 0; i < RANK_MAX; i ++)
		{
			if(m_szifChange[i])
			{
				m_szRankList[i]->SendChange();
				m_szifChange[i] = false;
			}
		}*/
	
}

void RankCenter::RequestInfo(UserData* pUser,DWORD* szVal)
{
	if(pUser == NULL)
		return;
	for(int i = 0; i < RANK_MAX; i ++)
	{
		if(szVal[i] != m_szRankList[i]->m_dwValueFlag)
			m_szRankList[i]->SendList(pUser->m_dwUin);
	}
	BuildPacketEx(pPacket,RPGRANK,S_SEND_RANKINFO_RLT,buf,1024);
	SET_DATA(pRecv,RPGRANK,S_SEND_RANKINFO_RLT,pPacket);
	for(int i = 0; i < RANK_MAX; i ++)
	{
		pRecv->dwRankFlag[i] = m_szRankList[i]->m_dwValueFlag;
	}
	TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
}

int		RankCenter::GetRankIDByPos(BYTE byType,int nPos)
{
	if(byType >= RANK_MAX)
		return 0;
	Rank_Data* pData = m_szRankList[byType]->GetRankDatabyPos(nPos);
	if(pData == NULL)
		return 0;
	return pData->m_dwID1;
}

bool RankCenter::CheckRank(BYTE byType,void* param,bool ifUser)
{
	if(byType >= RANK_MAX)
		return false;
	if(ifUser)
	{
		int nUin = (int)param;
		if (byType == RANK_DAMAGE)
		{
			_MuliParamObj* pTemp = (_MuliParamObj*)param;
			nUin = pTemp->nParam1;
		}
		UserData* pUser = g_pGameCenter->GetPlayer(nUin);
		if(pUser)
			AddUser(pUser);
	}
	else if(byType == RANK_EQUIP)
	{
		_MuliParamObj* pTemp = (_MuliParamObj*)param;
		if(pTemp)
			AddWatchData(pTemp->nParam1,pTemp->nParam2,0);
	}
	RankEvent* pEvent = new RankEvent();
	pEvent->byType = byType;
	pEvent->param = param;
	m_pEventList->Add(pEvent);
	return true;
}

void	RankCenter::AddWatchData(DWORD id1,DWORD id2,BYTE byType)
{
	UserData* pUser = g_pGameCenter->GetPlayer(id1);
	if(pUser)
	{
		bool ifFind = false;
		ItemObj* pItem = pUser->m_pItemCenter->GetItemByIndex(id2);
		if(pItem)
		{
			for(int i = 0; i < m_pUWList->Count; i ++)
			{
				RankUWatchInfo* pInfo = (RankUWatchInfo*)m_pUWList->Items[i];
				if(pInfo->dwUin == id1 && pInfo->dwid == id2 && pInfo->byType == byType)
				{
					pItem->GetByteData(pInfo->pData);
					pInfo->szSize = pItem->GetByteSize();
					if(pInfo->byState == 0)
						pInfo->byState = 2;
					ifFind = true;
					//_updateUW(pInfo);
					break;
				}
			}
			if(!ifFind)
			{
				RankUWatchInfo* pInfo = new RankUWatchInfo();
				pInfo->dwUin = id1;
				pInfo->dwid = id2;
				pInfo->byType = byType;
				pItem->GetByteData(pInfo->pData);
				pInfo->szSize = pItem->GetByteSize();
				pInfo->byState = 1;
				//_insertUW(pInfo);
				m_pUWList->Add(pInfo);
			}
		}
	}
}

void	RankCenter::DeleteRank(BYTE byType,DWORD id1,DWORD id2)
{
	if(byType >= RANK_MAX)
		return;
	m_szRankList[byType]->DeleteRank(id1,id2);
	if(byType == RANK_EQUIP)
	{
		for(int i = 0; i < m_pUWList->Count; i ++)
		{
			RankUWatchInfo* pInfo = (RankUWatchInfo*)m_pUWList->Items[i];
			if(pInfo->dwUin == id1 && pInfo->dwid == id2 && pInfo->byType == 0)
			{
				m_pUWList->Delete(i);
				_deleteUW(pInfo);
				delete pInfo;
				return;
			}
		}
	}
}

void	RankCenter::DeleteUser(DWORD dwUin)
{
	m_szRankList[RANK_LEVEL]->DeleteRank(dwUin,0);
	m_szRankList[RANK_MONEY]->DeleteRank(dwUin,0);
	m_szRankList[RANK_ATTACK]->DeleteRank(dwUin,0);
	m_szRankList[RANK_HORNOUR]->DeleteRank(dwUin,0);
	m_szRankList[RANK_EQUIP]->DeleteRank(dwUin);
	std::map<DWORD,SvRankUserInfo*>::iterator itor = m_mapUserInfo.find(dwUin);
	if(itor != m_mapUserInfo.end())
	{
		_deleteUserInfo(itor->second);
		m_mapUserInfo.erase(itor);
	}
	for(int i = m_pUWList->Count-1; i >= 0; i --)
	{
		RankUWatchInfo* pInfo = (RankUWatchInfo*)m_pUWList->Items[i];
		if(pInfo->dwUin == dwUin)
		{
			m_pUWList->Delete(i);
			_deleteUW(pInfo);
			delete pInfo;
		}
	}
}

void RankCenter::CheckAllUserRank(UserData* pUser)
{
	if(pUser == NULL)
		return;
	for(int i = 0; i < RANK_MAX; i ++)
	{
		if(szifSendMine[i]&&i != RANK_DAMAGE)
		{
			AddUser(pUser);
			if(m_szRankList[i]->CheckRank((void*)pUser->m_dwUin));
			//	m_szRankList[i]->SendChange();
		}
	}
}

SvRankUserInfo* RankCenter::UpdateUser(UserData* pUser)
{
	if(pUser == NULL)
		return NULL;
	std::map<DWORD,SvRankUserInfo*>::iterator itor = m_mapUserInfo.find(pUser->m_dwUin);
	if(itor != m_mapUserInfo.end())
	{
		SvRankUserInfo* pInfo = itor->second;
		if(pInfo->dwUin != pUser->m_dwUin)
			pInfo->ifInfoChg = true;
		pInfo->dwUin = pUser->m_dwUin;

		if(pInfo->nLv != pUser->m_xUWealth[UW_LV])
			pInfo->ifInfoChg = true;
		pInfo->nLv = pUser->m_xUWealth[UW_LV];

		if(pInfo->byJob != pUser->m_byJob)
			pInfo->ifInfoChg = true;
		pInfo->byJob = pUser->m_byJob;

		if(pInfo->nScore != pUser->m_nScore)
			pInfo->ifInfoChg = true;
		pInfo->nScore = pUser->m_nScore;

		if(strcmp(pInfo->szName,pUser->m_szNick)!=0)
			pInfo->ifInfoChg = true;
		strcpy(pInfo->szName,pUser->m_szNick);

		if(pInfo->nSkin[0] != pUser->m_Param.nSkin)
			pInfo->ifInfoChg = true;
		pInfo->nSkin[0] = pUser->m_Param.nSkin;

		if(pInfo->nSkin[1] != pUser->m_Param.nSkin)
			pInfo->ifInfoChg = true;
		pInfo->nSkin[1] = pUser->m_Param.nWeapon;

		if(pInfo->nSkin[2] != pUser->m_Param.nSkin)
			pInfo->ifInfoChg = true;
		pInfo->nSkin[2] = pUser->m_Param.nHorse;
		
		for(int i = 0; i < EQUIP_MAX; i ++)
		{
			char byTemp[256] = {0};
			int nSize = 0;
			if(pUser->m_pItemCenter->m_pEquip[i])
			{
				pUser->m_pItemCenter->m_pEquip[i]->GetByteData(byTemp);
				nSize = pUser->m_pItemCenter->m_pEquip[i]->GetByteSize();
			}

			if(pInfo->szSize[i] == 0 && nSize == 0)
				continue;
			if(pInfo->szSize[i] != nSize || memcmp(byTemp,pInfo->pData[i],nSize) != 0)
			{
				pInfo->szSize[i] = nSize;
				memcpy(pInfo->pData[i],byTemp,nSize);
				pInfo->ifEquipChg[i] = true;
			}
// 			if(pUser->m_pItemCenter->m_pEquip[i])
// 			{
// 				pUser->m_pItemCenter->m_pEquip[i]->GetByteData(pInfo->pData[i]);
// 				pInfo->szSize[i] = pUser->m_pItemCenter->m_pEquip[i]->GetByteSize();
// 			}
// 			else
// 			{
// 				pInfo->szSize[i]	= 0;
// 			}
		}
		_updateUserInfo(pInfo);
		return pInfo;
	}
	return NULL;
}

SvRankUserInfo* RankCenter::AddUser(UserData* pUser)
{
	if(pUser == NULL)
		return NULL;
	std::map<DWORD,SvRankUserInfo*>::iterator itor = m_mapUserInfo.find(pUser->m_dwUin);
	if(itor == m_mapUserInfo.end())
	{
		SvRankUserInfo* pInfo = new SvRankUserInfo();
		pInfo->dwUin = pUser->m_dwUin;
		pInfo->nLv = pUser->m_xUWealth[UW_LV];
		pInfo->nScore = pUser->m_nScore;
		strcpy(pInfo->szName,pUser->m_szNick);
		pInfo->nSkin[0] = pUser->m_Param.nSkin;
		pInfo->nSkin[1] = pUser->m_Param.nWeapon;
		pInfo->nSkin[2]	=  pUser->m_Param.nHorse;
		pInfo->ifInfoChg = false;
		for(int i = 0; i < EQUIP_MAX; i ++)
		{
			pInfo->ifEquipChg[i] = false;
			if(pUser->m_pItemCenter->m_pEquip[i])
			{
				pUser->m_pItemCenter->m_pEquip[i]->GetByteData(pInfo->pData[i]);
				pInfo->szSize[i] = pUser->m_pItemCenter->m_pEquip[i]->GetByteSize();
			}
			else
			{
				pInfo->szSize[i] = 0;
			}
		}
		m_mapUserInfo.insert(std::make_pair(pUser->m_dwUin,pInfo));
		_insertUserInfo(pInfo);
		return pInfo;
	}
	return NULL;
}

void RankCenter::AddInfo(SvRankUserInfo* pInfo)
{
	if(pInfo == NULL)
		return;
	std::map<DWORD,SvRankUserInfo*>::iterator itor = m_mapUserInfo.find(pInfo->dwUin);
	if(itor == m_mapUserInfo.end())
	{
		m_mapUserInfo.insert(std::make_pair(pInfo->dwUin,pInfo));
	}
}

void RankCenter::AddInfo(RankUWatchInfo* pInfo)
{
	if(pInfo == NULL)
		return;
	for(int i = 0; i < m_pUWList->Count; i ++)
	{
		RankUWatchInfo* pCheck = (RankUWatchInfo*)m_pUWList->Items[i];
		if(pInfo->byType == pCheck->byType && pInfo->dwUin == pCheck->dwUin && pInfo->dwid == pCheck->dwid)
			return;
	}
	m_pUWList->Add(pInfo);
}

SvRankUserInfo* RankCenter::GetUserInfo(int nUin)
{
	std::map<DWORD,SvRankUserInfo*>::iterator itor = m_mapUserInfo.find(nUin);
	if(itor == m_mapUserInfo.end())
	{
		UserData* pUser = g_pGameCenter->GetPlayer(nUin);
		if(pUser)
		{
			return	AddUser(pUser);
		}
		return NULL;
	}
	else
	{
		UserData* pUser = g_pGameCenter->GetPlayer(nUin);
		if(pUser)
		{
			return UpdateUser(pUser);
		}
		return itor->second;
	}
}

RankUWatchInfo* RankCenter::GetWatchData(int nUin,int nID,BYTE byType)
{
	UserData* pUser = g_pGameCenter->GetPlayer(nUin);
	if(pUser)
	{
		bool ifFind = false;
		ItemObj* pItem = pUser->m_pItemCenter->GetItemByIndex(nID);
		if(pItem)
		{
			for(int i = 0; i < m_pUWList->Count; i ++)
			{
				RankUWatchInfo* pInfo = (RankUWatchInfo*)m_pUWList->Items[i];
				if(pInfo->dwUin == nUin && pInfo->dwid == nID && pInfo->byType == byType)
				{
					pItem->GetByteData(pInfo->pData);
					pInfo->szSize = pItem->GetByteSize();
					if(pInfo->byState == 0)
						pInfo->byState = 2;
					ifFind = true;
					//_updateUW(pInfo);
					return pInfo;
				}
			}
			if(!ifFind)
			{
				RankUWatchInfo* pInfo = new RankUWatchInfo();
				pInfo->dwUin = nUin;
				pInfo->dwid = nID;
				pInfo->byType = byType;
				pItem->GetByteData(pInfo->pData);
				pInfo->szSize = pItem->GetByteSize();
				pInfo->byState = 1;
				//_insertUW(pInfo);
				m_pUWList->Add(pInfo);
				return pInfo;
			}
		}
	}
	for(int i = 0; i < m_pUWList->Count; i ++)
	{
		RankUWatchInfo* pInfo = (RankUWatchInfo*)m_pUWList->Items[i];
		if(pInfo->dwUin == nUin && pInfo->dwid == nID && pInfo->byType == byType)
		{
			return pInfo;
		}
	}
	return NULL;
}

void	RankCenter::GetOnlineUser(int nMinLv,xnList* pList,int nCount)
{
	std::map<DWORD,SvRankUserInfo*>::iterator itor = m_mapUserInfo.begin();
	while(itor != m_mapUserInfo.end())
	{
		if(nCount <= 0)
			return;
		SvRankUserInfo* pInfo = itor->second;
		UserData* pUser = g_pGameCenter->GetPlayer(pInfo->dwUin);
		if(pUser && pUser->m_xUWealth[UW_LV] > nMinLv)
		{
			pList->Add((void*)pUser);
			nCount --;
		}
		itor ++;
	}
}

void RankCenter::Sort()
{
	for(int i = 0; i < RANK_MAX; i ++)
		m_szRankList[i]->ReSort();
}

bool RankCenter::DBInit()
{
	QueryRlt xRlt;
	if(m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_RANKDATA_TABLE))
	{
		MySqlQuery* pCmd = xRlt.GetRlt();
		if (pCmd)
		{
			while(pCmd->FetchRow())
			{
				Rank_Data* pData = new Rank_Data();
				pData->m_dwID1 = pCmd->GetInt("id1");
				pData->m_dwID2 = pCmd->GetInt("id2");
				pData->m_byType = (RANKTYPE)pCmd->GetInt("type");
				pData->m_nPos = pCmd->GetInt("pos");
				pData->m_dwCheck1 = pCmd->GetInt("check1");
				pData->m_dwCheck2 = pCmd->GetInt("check2");
				pData->m_dwVal[0] = pCmd->GetInt("val1");
				pData->m_dwVal[1] = pCmd->GetInt("val2");
				pData->m_dwVal[2] = pCmd->GetInt("val3");
				pData->m_dwVal[3] = pCmd->GetInt("val4");
				strcpy(pData->m_szDesc[0],pCmd->GetStr("desc1"));
				strcpy(pData->m_szDesc[1],pCmd->GetStr("desc2"));
				strcpy(pData->m_szDesc[2],pCmd->GetStr("desc3"));
				strcpy(pData->m_szDesc[3],pCmd->GetStr("desc4"));
				if(pData->m_byType < RANK_MAX)
					m_szRankList[pData->m_byType]->AddData(pData);
			}
		}
	}

	if(m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_USERINFO_TABLE))
	{
		MySqlQuery* pCmd = xRlt.GetRlt();
		if (pCmd)
		{
			while(pCmd->FetchRow())
			{
				SvRankUserInfo* pInfo = new SvRankUserInfo();
				pInfo->dwUin = pCmd->GetInt("uin");
				pInfo->nLv = pCmd->GetInt("level");
				pInfo->byJob = pCmd->GetInt("job");
				pInfo->nScore = pCmd->GetInt("score");
				pInfo->nSkin[0] = pCmd->GetInt("skin1");
				pInfo->nSkin[1] = pCmd->GetInt("skin2");
				pInfo->nSkin[2] = pCmd->GetInt("skin3");
				strcpy(pInfo->szName,pCmd->GetStr("name"));
				pInfo->ifInfoChg = false;
				char sztem[16];
				for(int i = 0; i < EQUIP_MAX; i ++)
				{
					pInfo->ifEquipChg[i] = false;
					sprintf(sztem,"equip%d",i+1);
					int		nSize	= 0;
					BYTE* pRlt = pCmd->GetBLOB(sztem,&nSize);
					pInfo->szSize[i]	= nSize;
					if(pInfo->szSize[i])
						memcpy(pInfo->pData[i],pRlt,pInfo->szSize[i]);
				}
				AddInfo(pInfo);
			}
		}
	}

	if(m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_UWATCH_TABLE))
	{
		MySqlQuery* pCmd = xRlt.GetRlt();
		if (pCmd)
		{
			while(pCmd->FetchRow())
			{
				RankUWatchInfo* pInfo = new RankUWatchInfo();
				pInfo->dwUin = pCmd->GetInt("uin");
				pInfo->dwid = pCmd->GetInt("id");
				int		nSize	= 0;
				BYTE* pRlt = pCmd->GetBLOB("data",&nSize);
				pInfo->szSize = nSize;
				memcpy(pInfo->pData,pRlt,pInfo->szSize);
				pInfo->byState = 0;
				AddInfo(pInfo);
			}
		}
	}
	
	return true;
}

bool RankCenter::_insertUserInfo(SvRankUserInfo* pInfo)
{
	if(pInfo == NULL)
		return false;
	m_pDB->ExecuteSQL(FALSE,"insert into "RPG_USERINFO_TABLE" (uin,level,name,job,skin1,skin2,skin3,score,equip1,equip2,equip3,equip4,equip5,equip6,equip7,equip8,equip9,equip10,equip11,equip12) values (%d,%d,'%s',%d,%d,%d,%d,%d,'','','','','','','','','','','','')",
		pInfo->dwUin,pInfo->nLv,pInfo->szName,pInfo->byJob,pInfo->nSkin[0],pInfo->nSkin[1],pInfo->nSkin[2],pInfo->nScore);
	return true;
}

bool RankCenter::_updateUserInfo(SvRankUserInfo* pInfo)
{
	if(pInfo == NULL)
		return false;
	if(pInfo->ifInfoChg)
	{
		m_pDB->ExecuteSQL(FALSE,"update "RPG_USERINFO_TABLE" set level=%d,name='%s',job=%d,skin1=%d,skin2=%d,skin3=%d,score=%d where uin=%d",pInfo->nLv,pInfo->szName,pInfo->byJob,pInfo->nSkin[0],pInfo->nSkin[1],pInfo->nSkin[2],pInfo->nScore,pInfo->dwUin);
		pInfo->ifInfoChg = false;
	}
	char szname[16];
	for(int i = 0; i < EQUIP_MAX; i ++)
	{
		if(pInfo->ifEquipChg[i])
		{
			sprintf(szname,"equip%d",i+1);
			if(pInfo->szSize[i])
				m_pDB->SetBLOB(FALSE,pInfo->pData[i],pInfo->szSize[i],"update "RPG_USERINFO_TABLE" set %s=? where uin=%d",szname,pInfo->dwUin);
			else
				m_pDB->ExecuteSQL(FALSE,"update "RPG_USERINFO_TABLE" set %s='' where uin=%d",szname,pInfo->dwUin);
			pInfo->ifEquipChg[i] = false;
		}
	}
	return true;
}

bool RankCenter::_deleteUserInfo(SvRankUserInfo* pInfo)
{
	if(pInfo == NULL)
		return false;
	m_pDB->ExecuteSQL(FALSE,"delete from "RPG_USERINFO_TABLE" where uin = %d",pInfo->dwUin);
	return true;
}

bool RankCenter::_insertUW(RankUWatchInfo* pInfo)
{
	if(pInfo == NULL)
		return false;
	m_pDB->ExecuteSQL(FALSE,"insert into "RPG_UWATCH_TABLE" (uin,id,type,data) values (%d,%d,%d,'')",
		pInfo->dwUin,pInfo->dwid,pInfo->byType);
	if(pInfo->szSize)
		m_pDB->SetBLOB(FALSE,pInfo->pData,pInfo->szSize,"update "RPG_UWATCH_TABLE" set data=? where uin=%d and id=%d and type=%d",pInfo->dwUin,pInfo->dwid,pInfo->byType);
	return true;
}

bool RankCenter::_updateUW(RankUWatchInfo* pInfo)
{
	if(pInfo == NULL)
		return false;
	if(pInfo->szSize)
		m_pDB->SetBLOB(FALSE,pInfo->pData,pInfo->szSize,"update "RPG_UWATCH_TABLE" set data=? where uin=%d and id=%d and type=%d",pInfo->dwUin,pInfo->dwid,pInfo->byType);
	return true;
}

bool RankCenter::_deleteUW(RankUWatchInfo* pInfo)
{
	if(pInfo == NULL)
		return false;
	m_pDB->ExecuteSQL(FALSE,"delete from "RPG_UWATCH_TABLE" where uin = %d and id = %d and type = %d",pInfo->dwUin,pInfo->dwid,pInfo->byType);
	return true;
}

void RankCenter::ClearRank( BYTE byType )
{
	if(byType >= RANK_MAX)
		return;
	m_szRankList[byType]->Clear();
}

void RankCenter::RequestLastDamageInfo( UserData* pUser )
{
	if (!m_pLastDamage)
	{
		return;
	}
	BuildPacketEx(pPacket,RPGRANK,S_LAST_DAMAGE,buf,1024);
	SET_DATA(pRecv,RPGRANK,S_LAST_DAMAGE,pPacket);
	
	pRecv->szData = (*m_pLastDamage);
	TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
}
