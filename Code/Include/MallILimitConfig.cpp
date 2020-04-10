#include "MallILimitConfig.h"
#include "xnExtstr.h"

CreateSingleton(MallILimitConfig);
CreateSingleton(CfgRideExChg);

MallILimitConfig::MallILimitConfig(void)
{
}

MallILimitConfig::~MallILimitConfig(void)
{
	std::map<int,xnList*>::iterator	itor	= mapMILimitTime.begin();
	while(itor != mapMILimitTime.end())
	{
		while(itor->second->Count)
		{
			delete (MI_TIME_OBJ*)itor->second->Delete(0);
		}
		itor->second->Free();
		mapMILimitTime.erase(itor++);
	}
}


bool MallILimitConfig::Load( const char * szFile )
{
	xnExtstr * pExtstr = xnExtstr::Import((char*)szFile);
	if(!pExtstr) return false;

	int nIndex	= 0;
	while(pExtstr->SetSection(nIndex++))
	{
		int	nItemID	= atoi(pExtstr->sectionName);
		std::map<int,xnList*>::iterator	itor	= mapMILimitTime.find(nItemID);
		xnList* pList	= NULL;
		if(itor == mapMILimitTime.end())
		{
			pList	= xnList::Create();
			mapMILimitTime.insert(std::make_pair(nItemID,pList));
		}
		else
			pList	= itor->second;
		
		char	szTmp[32];
		int nCnt	= atoi(pExtstr->ReadString("timecnt","0"));
		for (int i = 0; i < nCnt; i++)
		{
			MI_TIME_OBJ* pMI	= new MI_TIME_OBJ;
			sprintf(szTmp,"stm%d",i+1);
			pMI->tmStart	= CMTime(pExtstr->ReadString(szTmp,"2012-01-01 00:00:00"));
			sprintf(szTmp,"etm%d",i+1);
			pMI->tmEnd		= CMTime(pExtstr->ReadString(szTmp,"2020-01-01 00:00:00"));
			sprintf(szTmp,"price%d",i+1);
			pMI->bPricePer	= atoi(pExtstr->ReadString(szTmp,"100"));
			sprintf(szTmp,"close%d",i+1);
			pMI->bClosed	= atoi(pExtstr->ReadString(szTmp,"1"));
			pList->Add(pMI);
		}
	}

	pExtstr->Free();
	return true;
}

MI_TIME_OBJ* MallILimitConfig::GetMIObj( int nItemID )
{
	DWORD	dwTimeNow	= CMTime::GetCurTickCount();
	std::map<int,xnList*>::iterator	itor	= mapMILimitTime.find(nItemID);
	if(itor	!= mapMILimitTime.end())
	{
		for (int i = itor->second->Count-1; i >=0; i--)
		{
			MI_TIME_OBJ*	pMI	= (MI_TIME_OBJ*)itor->second->Items[i];
			if(dwTimeNow >= pMI->tmStart.GetCurTickCount() && dwTimeNow <= pMI->tmEnd.GetCurTickCount())
			{
				return pMI;
			}
		}
	}
	return NULL;
}

void MallILimitConfig::AddLockMallI( int nItemID )
{
	DWORD	dwTimeNow	= CMTime::GetCurTickCount();
	xnList*	pList	= NULL;
	std::map<int,xnList*>::iterator	itor	= mapMILimitTime.find(nItemID);
	if(itor	== mapMILimitTime.end())
	{
		pList	= xnList::Create();
		mapMILimitTime.insert(std::make_pair(nItemID,pList));
	}
	else
		pList	= itor->second;

	MI_TIME_OBJ*	pMI	=	new MI_TIME_OBJ;
	pMI->bPricePer	= 100;
	pMI->bClosed	= 1;
	pMI->tmStart	= dwTimeNow;
	pMI->tmEnd		= dwTimeNow+2000000;
	pList->Add(pMI);
}

CfgRideExChg::CfgRideExChg()
{
	m_pList		=	xnList::Create();
}

CfgRideExChg::~CfgRideExChg()
{
	while(m_pList->Count)
	{
		delete (RideExChgObj*)m_pList->Delete(0);
	}
	m_pList->Free();
}

void CfgRideExChg::Load( char* szFile )
{
	xnExtstr * pExtstr = xnExtstr::Import(szFile);
	if(!pExtstr) return;

	int nIndex	= 0;
	char	szTmp[32];
	while(pExtstr->SetSection(nIndex++))
	{
		RideExChgObj*	pRECO	= new RideExChgObj();
		pRECO->dwItemID	= atoi(pExtstr->sectionName);
		pRECO->bOutBind	= atoi(pExtstr->ReadString("bind","0"));
		pRECO->dwCostItem	= atoi(pExtstr->ReadString("itemex","0"));
		for (int i = 0; i < 10; i++)
		{
			sprintf(szTmp,"item%d",i+1);
			pRECO->dwUseIID[i]	=	atoi(pExtstr->ReadString(szTmp,"0"));
			sprintf(szTmp,"num%d",i+1);
			pRECO->wNeedCnt[i]	=	atoi(pExtstr->ReadString(szTmp,"0"));
		}

		pRECO->bUseBind	= atoi(pExtstr->ReadString("usebind","0"));
		m_pList->Add(pRECO);
	}

	pExtstr->Free();
}

RideExChgObj* CfgRideExChg::GetRECObj( DWORD dwItem )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		RideExChgObj*	pRECO	= (RideExChgObj*)m_pList->Items[i];
		if(pRECO->dwItemID == dwItem)
			return pRECO;
	}
	return NULL;
}

RideExChgObj* CfgRideExChg::GetRECByIndex( WORD wPos )
{
	if(wPos >= m_pList->Count)
		return NULL;
	return (RideExChgObj*)m_pList->Items[wPos];
}

RideExChgObj* CfgRideExChg::GetRECByCost( DWORD dwItem )
{
	if(dwItem	== 0)
		return NULL;
	for (int i = 0; i < m_pList->Count; i++)
	{
		RideExChgObj*	pRECO	= (RideExChgObj*)m_pList->Items[i];
		if(pRECO->dwCostItem == dwItem)
			return pRECO;
	}
	return NULL;
}