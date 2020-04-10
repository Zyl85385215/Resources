
#include "stdafx.h"
#ifdef _ISSERVER
#include "stdio.h"
#endif

#include "SkillConfig.h"
#include "xnExtstr.h"

CreateSingleton(SkillConfig);

SkillBase::SkillBase()
{
	szName	= NULL;
	//szDesc	= NULL;
	
	pListLvInfo		= xnList::Create();
	pListStateAdd	= xnList::Create();
	pListTrigEff	= xnList::Create();
}

SkillBase::~SkillBase()
{
	if(szName)
		free(szName);
	//if(szDesc)
	//	free(szDesc);

	while (pListLvInfo->Count)
	{
		delete (SEffLvInfo*)pListLvInfo->Delete (0);
	}
	pListLvInfo->Free ();
	while (pListTrigEff->Count)
	{
		delete (SEffTrig*)pListTrigEff->Delete (0);
	}
	pListTrigEff->Free ();
	while (pListStateAdd->Count)
	{
		delete (_MuliParamObj*)pListStateAdd->Delete (0);
	}
	pListStateAdd->Free ();
}

SEffLvInfo* SkillBase::GetSELv( BYTE bLv,bool bSafe )
{
	if(bLv == 0)
		return NULL;
	if(bLv > pListLvInfo->Count)
	{
		if(bSafe)
			bLv	= pListLvInfo->Count;
		else
			return NULL;
	}
	return (SEffLvInfo*)pListLvInfo->Items[bLv-1];
}

SkillConfig::SkillConfig()
{
	m_pList		= xnList::Create();
	m_pLJobSkill[0]	= xnList::Create();
	m_pLJobSkill[1]	= xnList::Create();
	m_pLJobSkill[2]	= xnList::Create();
	m_pLJobSkill[3]	= xnList::Create();
	m_pLJobSkill[4]	= xnList::Create();
}

SkillConfig::~SkillConfig()
{
	while(m_pList->Count)
		delete (SkillBase*)m_pList->Delete(0);
	m_pList->Free();

	m_pLJobSkill[0]->Free();
	m_pLJobSkill[1]->Free();
	m_pLJobSkill[2]->Free();
	m_pLJobSkill[3]->Free();
	m_pLJobSkill[4]->Free();
}

void SkillConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[256];
	while (pStr->SetSection(index++) != NULL)
	{
		SkillBase*	pSBase	= new SkillBase();
		pSBase->wSkillID	= atoi (pStr->sectionName);
		pSBase->bType		= atoi (pStr->ReadString ("type","0"));
		pSBase->wCDPos		= atoi (pStr->ReadString ("cdpos","0"));

		pSBase->szName		= strdup(pStr->ReadString("name","skill"));
		//pSBase->szDesc		= strdup(pStr->ReadString("desc","desc"));
		pSBase->wIcon		= atoi(pStr->ReadString("ico","0"));
	
		pSBase->byJob		= atoi(pStr->ReadString("job","0"));
		pSBase->nGoldLvNeed	= atoi(pStr->ReadString("lvlgold","0"));
		pSBase->wItemLvNeed	= atoi(pStr->ReadString("lvlitem","0"));
		pSBase->byCostSPoint	= atoi(pStr->ReadString("lvpoint","1"));
		pSBase->wPreSkillID	= atoi(pStr->ReadString("preskillid","0"));
		pSBase->bPreSkillLv	= atoi(pStr->ReadString("preskilllv","0"));

		pSBase->bCostNoPer	= atoi(pStr->ReadString("costisper","0"));
		pSBase->nMpCost		= atoi(pStr->ReadString("mpcost","0"));
		pSBase->nHpCost		= atoi(pStr->ReadString("hpcost","0"));
		pSBase->nSpCost		= atoi(pStr->ReadString("spcost","0"));	
		pSBase->wItemCost	= atoi(pStr->ReadString("itemcost","0"));

		pSBase->wStartEff	= atoi(pStr->ReadString("seff","0"));
		pSBase->byStartEffDir	= atoi(pStr->ReadString("seffdir","0"));
		pSBase->byOnlyEnemy	= atoi(pStr->ReadString("onlyenemy","0"));
		pSBase->wFlyEff		= atoi(pStr->ReadString("feff","0"));
		pSBase->wEndEff		= atoi(pStr->ReadString("eeff","0"));
		pSBase->dwEndDelay	= atoi(pStr->ReadString("edelay","0"));

		pSBase->wAtkRange	= atoi(pStr->ReadString("atkrange","300"));
		pSBase->bAoeType	= atoi(pStr->ReadString("aoetype","1"));
		pSBase->wAoeRange	= atoi(pStr->ReadString("aoerange","0"));
		pSBase->bMaxTarget	= atoi(pStr->ReadString("aoemaxobj","0"));
		pSBase->bActType	= atoi(pStr->ReadString("acttype","0"));
		pSBase->bDmgType	= atoi(pStr->ReadString("stype","0"));

		int	nTripCnt	= atoi(pStr->ReadString("concnt","0"));
		for (int i = 0; i < nTripCnt; i ++)
		{
			SEffTrig*	pSETrip	= new SEffTrig();
			sprintf(szTmp,"contype%d",i+1);
			pSETrip->bTrigType	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"conval%d",i+1);
			pSETrip->wTrigVal	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"conid%d",i+1);
			pSETrip->wTrigID	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"conefftype%d",i+1);
			pSETrip->bTrigEffType= atoi(pStr->ReadString(szTmp,"0"));
			pSBase->pListTrigEff->Add(pSETrip);
		}

		int	nStateCnt	= atoi(pStr->ReadString("statecnt","0"));
		for (int i = 0; i < nStateCnt; i ++)
		{
			_MuliParamObj*	pMP	= new _MuliParamObj();
			sprintf(szTmp,"statedest%d",i+1);
			pMP->nParam1	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"stateid%d",i+1);
			pMP->nParam2	= atoi(pStr->ReadString(szTmp,"0"));
			pSBase->pListStateAdd->Add(pMP);
		}

		int	nLvCnt	= atoi(pStr->ReadString("lvcnt","0"));
		for (int i = 0; i < nLvCnt; i ++)
		{
			SEffLvInfo*	pLvInfo	= new SEffLvInfo();
			sprintf(szTmp,"lvx%d",i+1);
			pLvInfo->nP_X	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"lvy1%d",i+1);
			pLvInfo->nP_Y1	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"lvy2%d",i+1);
			pLvInfo->nP_Y2	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"spmtype1%d",i+1);
			pLvInfo->nP_Y1Type	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"spmtype2%d",i+1);
			pLvInfo->nP_Y2Type	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"lvcd%d",i+1);
			pLvInfo->dwCDTick	= atoi(pStr->ReadString(szTmp,"0"));

			sprintf(szTmp,"lvlimit%d",i+1);
			pLvInfo->byUpLvNeed	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"lvitem%d",i+1);
			pLvInfo->wUpItemNeed	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"lvmp%d",i+1);
			pLvInfo->nMpCost		= atoi(pStr->ReadString(szTmp,"0"));


			sprintf(szTmp,"lvstl%d",i+1);
			pLvInfo->bAddStateID	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"connum%d",i+1);
			pLvInfo->bAddSkillID	= atoi(pStr->ReadString(szTmp,"0"));

			sprintf(szTmp,"lvrange%d",i+1);
			pLvInfo->wRangeAdd		= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"lvmax%d",i+1);
			pLvInfo->bTargetAdd		= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"lvbite%d",i+1);
			pLvInfo->wIgnoDef		= atoi(pStr->ReadString(szTmp,"0"));

			sprintf(szTmp,"lvdesc%d",i+1);
			pLvInfo->szDesc		= strdup(pStr->ReadString(szTmp,"²»Ïê"));

			pSBase->pListLvInfo->Add(pLvInfo);
		}
		m_pList->Add (pSBase);
	}
	pStr->Free ();
}

void SkillConfig::LoadFileJob( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[256];
	while (pStr->SetSection(index) != NULL)
	{
		if(index > 4)
			break;

		for (int j = 0; j < 12; j++)
		{
			sprintf(szTmp,"skill%d",j+1);
			int	nSkill	= atoi(pStr->ReadString(szTmp,"0"));
			SkillBase* pSBase	= GetSkillBase(nSkill);
			if(pSBase)
				m_pLJobSkill[index]->Add(pSBase);
		}
		index++;
	}
	pStr->Free ();
}

SkillBase* SkillConfig::GetSkillBase( int nID )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		SkillBase* pSBase	= (SkillBase*)m_pList->Items[i];
		if(pSBase->wSkillID	== nID)
			return pSBase;
	}

	return NULL;
}

SkillBase* SkillConfig::GetJobSkill( BYTE bJob, BYTE bPos )
{
	if(bJob > 4)
		return NULL;
	if(bPos >= m_pLJobSkill[bJob]->Count)
		return NULL;

	return (SkillBase*)m_pLJobSkill[bJob]->Items[bPos];
}

SEffLvInfo::~SEffLvInfo()
{
	if(szDesc)
		free(szDesc);
}
