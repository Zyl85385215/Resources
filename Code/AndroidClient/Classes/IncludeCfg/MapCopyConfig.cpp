
#include "stdafx.h"
#ifdef _ISSERVER
#include "stdio.h"
#include "TimeControl.h"
#endif

#include "MapCopyConfig.h"
#include "xnExtstr.h"
#include "EnemyDropConfig.h"

CreateSingleton(MCopyConfig);

MCopyBase::MCopyBase()
{
	szName		= NULL;
	pLItemGet	= xnList::Create ();
	pLLimitTime	= xnList::Create ();
	pLRqMCopy	= xnList::Create ();
	pLHelper	= xnList::Create();
}

MCopyBase::~MCopyBase()
{
	if(szName)
		free(szName);
	
	while(pLItemGet->Count)
	{
		delete (_MCopyDrop*)pLItemGet->Delete (0);
	}
	pLItemGet->Free ();
	while(pLLimitTime->Count)
	{
		delete (_MCopyTime*)pLLimitTime->Delete (0);
	}
	pLLimitTime->Free ();
	while(pLHelper->Count)
	{
		delete (_MCopyHelp*)pLHelper->Delete(0);
	}
	pLHelper->Free();
	pLRqMCopy->Free();
}

bool MCopyBase::CheckTime()
{
#ifdef _ISSERVER
	tm ctime = CMTime::GetCurTime();
#else
	tm	ctime	= *(tm*)xnGetDayTime();
#endif
	if(szbyWeekDay[ctime.tm_wday] == 0)
		return false;
	
	if(pLLimitTime->Count)
	{
		bool	bCheck	= false;
		for (int i = 0; i < pLLimitTime->Count; i++)
		{
			_MCopyTime*	pT	= (_MCopyTime*)pLLimitTime->Items[i];
			if((ctime.tm_hour > pT->bySHour || (ctime.tm_hour == pT->bySHour && ctime.tm_min >= pT->bySMinute))
				&& (ctime.tm_hour < pT->byEHour || (ctime.tm_hour == pT->byEHour && ctime.tm_min <= pT->byEMinute)))
			{
				bCheck = true;
				break;
			}
		}
		return bCheck;
	}

	return true;
}

DWORD MCopyBase::RandGetItem()
{
	int	nRnd	= rand()%10000;
	for (int i = 0; i < pLItemGet->Count; i++)
	{
		_MCopyDrop* pMItem	= (_MCopyDrop*)pLItemGet->Items[i];

		if(pMItem->wPer > nRnd)
		{
			if(pMItem->wItemID < 10000)
			{
				_DropGroup*	pDG	= DropGroupConfig2::GetInstance().GetDropGroup(pMItem->wItemID);
				if(pDG != NULL)
					return pDG->RandGetItem();
			}
			else
				return pMItem->wItemID;
		}
		else
			nRnd -= pMItem->wPer;
	}

	return 0;
}

WORD MCopyBase::RandGetHelper( BYTE bJob )
{
	if(bJob > 4)
		return 0;
	WORD	wRtl	= 0;
	xnList* pList	= xnList::Create();
	for (int i = 0; i < pLHelper->Count; i++)
	{
		_MCopyHelp* pHelp	= (_MCopyHelp*)pLHelper->Items[i];

		if(pHelp->szByJobLimit[bJob])
			pList->Add((void*)pHelp->wBaseID);
	}
	if(pLHelper->Count == 0)
		return 0;

	wRtl	= (int)pList->Items[rand()%pList->Count];
	pList->Free();
	return wRtl;
}


MCopyConfig::MCopyConfig()
{
	m_pList		= xnList::Create ();
	m_nTypeCnt[0]	= 0;
	m_nTypeCnt[1]	= 0;
	m_nTypeCnt[2]	= 0;
}

MCopyConfig::~MCopyConfig()
{
	while(m_pList->Count)
	{
		delete (MCopyBase*)m_pList->Delete (0);
	}
	m_pList->Free ();
}

void MCopyConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[32];
	while (pStr->SetSection(index++) != NULL)
	{
		MCopyBase*	pMCopy	= new MCopyBase();
		pMCopy->wID			= atoi (pStr->sectionName);
		pMCopy->szName		= strdup(pStr->ReadString("name","MCopy"));
		pMCopy->wBaseMap	= atoi (pStr->ReadString("basemap","3"));
		pMCopy->wIcon	= atoi (pStr->ReadString("ico","1"));
		pMCopy->byType		= atoi (pStr->ReadString("type","0"));

		if(pMCopy->byType < 3)
			m_nTypeCnt[pMCopy->byType]++;

		pMCopy->nExPow		= atoi (pStr->ReadString("expow","100"));
		pMCopy->byChgLv		= atoi (pStr->ReadString("chglv","0"));
		pMCopy->byLvNeed		= atoi (pStr->ReadString("lvneed","1"));
		pMCopy->byLifeCost		= atoi (pStr->ReadString("life","0"));

		pMCopy->byLimitHelp		= atoi (pStr->ReadString("limithelp","0"));
		pMCopy->byLimitSlave	= atoi (pStr->ReadString("limitslave","0"));
		pMCopy->byLimitGroup	= atoi (pStr->ReadString("limitgroup","0"));
		pMCopy->byLimitItem		= atoi (pStr->ReadString("limititem","0"));
		pMCopy->byLimitTime		= atoi (pStr->ReadString("limittime","0"));

		pMCopy->byWinType		= atoi (pStr->ReadString("wintype","0"));
		pMCopy->wWinID			= atoi (pStr->ReadString("winid","0"));
		pMCopy->nGold			= atoi (pStr->ReadString("gold","0"));
		pMCopy->nExp			= atoi (pStr->ReadString("exp","0"));
		pMCopy->dwRankTime      = atoi (pStr->ReadString("ptime","0"));

		pMCopy->nStartX			= atoi (pStr->ReadString("px","2500"));
		pMCopy->nStartY			= atoi (pStr->ReadString("py","2500"));
		
		char*	szStr		= pStr->ReadString("wday","1111111");
		pMCopy->szbyWeekDay[1]	= szStr[0]-'0';
		pMCopy->szbyWeekDay[2]	= szStr[1]-'0';
		pMCopy->szbyWeekDay[3]	= szStr[2]-'0';
		pMCopy->szbyWeekDay[4]	= szStr[3]-'0';
		pMCopy->szbyWeekDay[5]	= szStr[4]-'0';
		pMCopy->szbyWeekDay[6]	= szStr[5]-'0';
		pMCopy->szbyWeekDay[0]	= szStr[6]-'0';

		int		nCnt	= atoi (pStr->ReadString("timecnt","0"));
		for (int i = 0; i < nCnt; i++)
		{
			_MCopyTime*	pMCT	= new _MCopyTime();
			pMCopy->pLLimitTime->Add(pMCT);
			int	nH	= 0, nM	= 0;
			sprintf(szTmp,"daytimes%d",i+1);
			sscanf(pStr->ReadString(szTmp,"00:00"),"%d:%d",&nH,&nM);
			pMCT->bySHour	= nH;
			pMCT->bySMinute	= nM;
			sprintf(szTmp,"daytimee%d",i+1);
			sscanf(pStr->ReadString(szTmp,"24:00"),"%d:%d",&nH,&nM);
			pMCT->byEHour	= nH;
			pMCT->byEMinute	= nM;
		}

		pMCopy->byHelpNum	= atoi (pStr->ReadString("helpnum","1"));

		nCnt	= atoi (pStr->ReadString("helpcnt","0"));
		for (int i = 0; i < nCnt; i++)
		{
			_MCopyHelp*	pHelp	= new _MCopyHelp();
			pMCopy->pLHelper->Add(pHelp);
			int	nH	= 0, nM	= 0;

			sprintf(szTmp,"helpid%d",i+1);
			pHelp->wBaseID			= atoi(pStr->ReadString(szTmp,"11111"));
			sprintf(szTmp,"helpjob%d",i+1);
			szStr		= pStr->ReadString(szTmp,"11111");
			pHelp->szByJobLimit[0]	= szStr[0]-'0';
			pHelp->szByJobLimit[1]	= szStr[1]-'0';
			pHelp->szByJobLimit[2]	= szStr[2]-'0';
			pHelp->szByJobLimit[3]	= szStr[3]-'0';
			pHelp->szByJobLimit[4]	= szStr[4]-'0';
		}

		nCnt	= atoi (pStr->ReadString("itemcnt","0"));
		for (int i = 0; i < nCnt; i++)
		{
			_MCopyDrop*	pMCD	= new _MCopyDrop();
			pMCopy->pLItemGet->Add(pMCD);
			
			sprintf(szTmp,"item%d",i+1);
			pMCD->wItemID	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"per%d",i+1);
			pMCD->wPer		= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"perex%d",i+1);
			pMCD->wPerEx	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"dis%d",i+1);
			pMCD->wDis	= atoi(pStr->ReadString(szTmp,"0"));
		}

		m_pList->Add (pMCopy);
	}
	pStr->Free ();
}

MCopyBase* MCopyConfig::GetMCopy( WORD wID )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		MCopyBase*	pMCB		= (MCopyBase*)m_pList->Items[i];
		if(pMCB->wID	== wID)
			return pMCB;
	}
	return NULL;
}

MCopyBase* MCopyConfig::GetMCopyByMBase( WORD wMBase )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		MCopyBase*	pMCB		= (MCopyBase*)m_pList->Items[i];
		if(pMCB->wBaseMap	== wMBase)
			return pMCB;
	}
	return NULL;
}

