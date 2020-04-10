
#include "stdafx.h"
#ifdef _ISSERVER
#include "stdio.h"
#endif

#include "EnemyDropConfig.h"
#include "xnExtstr.h"

CreateSingleton(DropGroupConfig2);
CreateSingleton(EnemyKillConfig);

_DropGroup::_DropGroup()
{
	pListItem	= xnList::Create();
}

_DropGroup::~_DropGroup()
{
	while(pListItem->Count)
		delete (_MuliParamObj*)pListItem->Delete(0);
	pListItem->Free();

}

int _DropGroup::RandGetItem()
{
	int	nRand	= rand()%10000;
	for (int i = 0; i < pListItem->Count; i++)
	{
		_MuliParamObj*	pMP	= (_MuliParamObj*)pListItem->Items[i];

		if(nRand < pMP->nParam2)
		{
			return pMP->nParam1;
		}
		else
			nRand -= pMP->nParam2;
	}
	return 0;
}

DropGroupConfig2::DropGroupConfig2()
{
	m_pList		= xnList::Create();
}

DropGroupConfig2::~DropGroupConfig2()
{
	while(m_pList->Count)
		delete (_DropGroup*)m_pList->Delete(0);
	m_pList->Free();
}

void DropGroupConfig2::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[32];
	while (pStr->SetSection(index++) != NULL)
	{
		_DropGroup*	pDG	= new _DropGroup();
		pDG->wDGID		= atoi (pStr->sectionName);
		int	nCnt		= atoi (pStr->ReadString("cnt","0"));
		for (int i = 0; i < nCnt; i++)
		{
			_MuliParamObj*	pMP	= new _MuliParamObj();
			sprintf(szTmp,"item%d",i+1);
			pMP->nParam1	= atoi (pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"per%d",i+1);
			pMP->nParam2	= atoi (pStr->ReadString(szTmp,"0"));
			pDG->pListItem->Add(pMP);
		}
		m_pList->Add(pDG);
	}
	pStr->Free();
}

_DropGroup* DropGroupConfig2::GetDropGroup( DWORD wID )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		_DropGroup*	pDG		= (_DropGroup*)m_pList->Items[i];
		if(pDG->wDGID	== wID)
			return pDG;
	}
	return NULL;
}

_EnemyAward::_EnemyAward()
{
	pListItem		= xnList::Create();
}

_EnemyAward::~_EnemyAward()
{
	while(pListItem->Count)
		delete (_MuliParamObj*)pListItem->Delete(0);
	pListItem->Free();
}

void _EnemyAward::RandGetItem(xnList* pOutList)
{
	for (int i = 0; i < pListItem->Count; i++)
	{
		_MuliParamObj*	pMP	= (_MuliParamObj*)pListItem->Items[i];
		int	nRandA	= rand()%10000;
		if(nRandA < pMP->nParam2)
		{
			if(pMP->nParam1 < 10000)
			{
				_DropGroup*	pDG	= DropGroupConfig2::GetInstance().GetDropGroup(pMP->nParam1);

				pOutList->Add((void*)pDG->RandGetItem());
			}
			else
				pOutList->Add((void*)pMP->nParam1);
		}
	}
	return;
}

int _EnemyAward::RandGetGold()
{
	int		nRand	= rand()%10000;
	if(nRand < wGoldPer)
	{
		return nGold*(90+(rand()%21))/100;
	}
	return 0;
}

int _EnemyAward::CalcExp( int nPlayerLv )
{
	int nDifLv	= abs(nPlayerLv - byLv);

	if(nDifLv < 5)
		return dwExp;
	int nPer	= 100-nDifLv*(nDifLv-4);
	if(nPer <= 0)
		return 1;
	int nRtl	= dwExp*nPer/100;
	if(nRtl <= 0)
		nRtl	= 1;
	return nRtl;
}

EnemyKillConfig::EnemyKillConfig()
{
	m_pList		= xnList::Create();
}

EnemyKillConfig::~EnemyKillConfig()
{
	while(m_pList->Count)
		delete (_EnemyAward*)m_pList->Delete(0);
	m_pList->Free();
}

void EnemyKillConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[32];
	while (pStr->SetSection(index++) != NULL)
	{
		_EnemyAward*	pEA	= new _EnemyAward();
		pEA->wID		= atoi (pStr->sectionName);
		pEA->byLv		= atoi (pStr->ReadString("lv","10"));
		pEA->dwExp		= atoi (pStr->ReadString("exp","10"));
		pEA->nGold		= atoi (pStr->ReadString("gold","100"));
		pEA->wGoldPer	= atoi (pStr->ReadString("goldper","1000"));
		int	nCnt		= atoi (pStr->ReadString("itemcnt","0"));
		for (int i = 0; i < nCnt; i++)
		{
			_MuliParamObj*	pMP	= new _MuliParamObj();
			sprintf(szTmp,"item%d",i+1);
			pMP->nParam1	= atoi (pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"itemper%d",i+1);
			pMP->nParam2	= atoi (pStr->ReadString(szTmp,"0"));
			pEA->pListItem->Add(pMP);
		}

		m_pList->Add(pEA);
	}
	pStr->Free();

}

_EnemyAward* EnemyKillConfig::GetEAward( WORD wID )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		_EnemyAward*	pEA		= (_EnemyAward*)m_pList->Items[i];
		if(pEA->wID	== wID)
			return pEA;
	}
	return NULL;
}
