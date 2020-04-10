#include "SlaveConfig.h"
#include "xnExtstr.h"
#include "stdio.h"

CreateSingleton(CaptureConfig);
CreateSingleton(SlaveConfig);
CreateSingleton(SlaveFuseConfig);
CreateSingleton(SlaveChgBaseSpecial);
CreateSingleton(SlaveFuseSkillCfg);
CreateSingleton(SlavePhaseExCfg);


SlaveConfig::SlaveConfig( )
{
	pListPhase		= xnList::Create();
	pListQuaSplit	= xnList::Create();
	pListLvUpExp	= xnList::Create();
}

SlaveConfig::~SlaveConfig()
{
	while(pListQuaSplit->Count)
		delete (SlaveQuaSplit*)pListQuaSplit->Delete(0);

	pListQuaSplit->Free();

	while(pListPhase->Count)
		delete (SlavePhaseInfo*)pListPhase->Delete(0);

	pListPhase->Free();
	pListLvUpExp->Free();
}

void SlaveConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;
	int i = 0;
	char szTmp[32];
	if (pStr->SetSection("slavecfg") != NULL)
	{
		m_wAttriMin		= atoi(pStr->ReadString("attrimin", "80"));
		m_wAttriMax		= atoi(pStr->ReadString("attrimax", "120"));

		m_byQualityCnt	= atoi(pStr->ReadString("qualitycnt", "5"));
		m_wPowTotal		= atoi(pStr->ReadString("powtotal", "100"));
		m_wPowCostTime	= atoi(pStr->ReadString("powcosttime", "60"));
		m_wPowCostTmVal	= atoi(pStr->ReadString("powcostval", "0"));
		m_wPowCostJoin	= atoi(pStr->ReadString("powcostjoin", "0"));
		m_wPowCostDead	= atoi(pStr->ReadString("powcostdead", "0"));
		m_wTimeRecover	= atoi(pStr->ReadString("timerecover", "5"));
		m_wStateRecover = atoi(pStr->ReadString("staterecover", "0"));
		m_wResetItemID1	= atoi(pStr->ReadString("qualityitem1", "34882"));
		m_wResetItemID2	= atoi(pStr->ReadString("qualityitem2", "34881"));
		//m_wPowCostSkill	= atoi(pStr->ReadString("powcostskill", "1"));

		for (int i = 0; i < m_byQualityCnt; i++)
		{
			SlaveQuaSplit* pSQS = new SlaveQuaSplit();
			sprintf(szTmp,"quality%dname",i+1);
			strncpy(pSQS->szQuaName,pStr->ReadString(szTmp,"未知"),32);
			sprintf(szTmp,"quality%dval",i+1);
			pSQS->nPoint	= atoi(pStr->ReadString(szTmp, "2"));
			pListQuaSplit->Add(pSQS);
		}
		m_byEvoCnt		= atoi(pStr->ReadString("evolvecnt", "4"));
		for (int i = 0; i < m_byEvoCnt; i++)
		{
			SlavePhaseInfo* pSPI	= new SlavePhaseInfo();
			
			sprintf(szTmp,"evolve%dcost",i+1);
			pSPI->bPowLvCost	= atoi(pStr->ReadString(szTmp, "25"));
			sprintf(szTmp,"evolve%dget",i+1);
			pSPI->bParamAdd		= atoi(pStr->ReadString(szTmp, "5"));
			sprintf(szTmp,"evolve%dluck",i+1);
			pSPI->bSuccessPer	= atoi(pStr->ReadString(szTmp, "10"));
			pListPhase->Add(pSPI);
		}
	}
	if (pStr->SetSection("slaveLevel") != NULL)
	{
		int nCnt	= atoi(pStr->ReadString("lvlcnt", "10"));

		for (int i = 0; i < nCnt; i++)
		{
			sprintf(szTmp,"lvlExp%d",i+1);
			pListLvUpExp->Add((void*)atoi(pStr->ReadString(szTmp, "1000000")));
		}
	}

	if (pStr->SetSection("ridebase") != NULL)
	{
		for (int i = 0; i < m_byQualityCnt; i++)
		{
			sprintf(szTmp,"speed%d",i+1);
			((SlaveQuaSplit*)pListQuaSplit->Items[i])->nState	= atoi(pStr->ReadString(szTmp, "0"));
			sprintf(szTmp,"skill%d1per",i+1);
			((SlaveQuaSplit*)pListQuaSplit->Items[i])->byPerSkill1	= atoi(pStr->ReadString(szTmp, "0"));
			sprintf(szTmp,"skill%d2per",i+1);
			((SlaveQuaSplit*)pListQuaSplit->Items[i])->byPerSkill2	= atoi(pStr->ReadString(szTmp, "0"));
		}
	}

	pStr->Free();
}

int SlaveConfig::GetLvUpExp( BYTE bCurLv )
{
	if(pListLvUpExp->Count == 0)
		return 10000000;
	if(bCurLv >= pListLvUpExp->Count)
		bCurLv	= pListLvUpExp->Count-1;

	return (int)pListLvUpExp->Items[bCurLv];
}

int SlaveConfig::GetPowUpExp( BYTE bCurPow )
{
	if(bCurPow>=100)
		return 1000000000;
	return (bCurPow+1)*12;
}

int SlaveConfig::GetQualityPoint( BYTE bQuality )
{
	if(bQuality < 0 || bQuality >= pListQuaSplit->Count)
		return NULL;
	return ((SlaveQuaSplit*)pListQuaSplit->Items[bQuality])->nPoint;
}

char* SlaveConfig::GetQualityName( BYTE bVal )
{
	if(bVal < 0 || bVal >= pListQuaSplit->Count)
		return NULL;
	return ((SlaveQuaSplit*)pListQuaSplit->Items[bVal])->szQuaName;

}

SlavePhaseInfo* SlaveConfig::GetPhaseInfo( BYTE bLv )
{
	if(bLv > pListPhase->Count)
		bLv = pListPhase->Count;

	return (SlavePhaseInfo*)pListPhase->Items[bLv-1];
}

int SlaveConfig::GetRideState( BYTE bQua )
{
	if(bQua < 0 || bQua >= pListQuaSplit->Count)
		return 0;
	return ((SlaveQuaSplit*)pListQuaSplit->Items[bQua])->nState;
}

SlaveQuaSplit* SlaveConfig::GetQuaInfo( BYTE bQua )
{
	if(bQua < 0 || bQua >= pListQuaSplit->Count)
		return NULL;
	return ((SlaveQuaSplit*)pListQuaSplit->Items[bQua]);
}

BYTE SlaveConfig::RandomQuality()
{
	int nRnd	= rand()%10000;
	if(nRnd < 2000)
		return 0;
	else if(nRnd < 7000)
		return 1;
	else if(nRnd < 9000)
		return 2;
	else if(nRnd < 9999)
		return 3;
	else
		return 4;
}
CaptureConfig::CaptureConfig()
{
	m_pList	= xnList::Create();
}

CaptureConfig::~CaptureConfig()
{
	while(m_pList->Count)
	{
		delete (CaptureInfo*)m_pList->Delete(0);
	}
	m_pList->Free();
}

void CaptureConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;
	int i = 0;
	char szTmp[32];
	int nSctIndex	= 0;
	while (pStr->SetSection(nSctIndex++) != NULL)
	{
		CaptureInfo* pCI	= new CaptureInfo();
		pCI->wEnemyID	= atoi(pStr->ReadString("enemyid", "0"));
		pCI->wSlaveID	= atoi(pStr->ReadString("slaveid", "0"));
		pCI->wSlaveExID	= atoi(pStr->ReadString("slaveidex", "0"));
		pCI->wSlaveExPer= atoi(pStr->ReadString("slaveexper", "0"));
		pCI->bIsRide	= atoi(pStr->ReadString("isride", "0"));

		m_pList->Add(pCI);
	}
	pStr->Free();
}

int CaptureConfig::GetRtlID( IN int nEID, OUT bool* pbIsRide )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		CaptureInfo* pCI	= (CaptureInfo*)m_pList->Items[i];
		if(pCI->wEnemyID	== nEID)
		{
			if(pbIsRide)
				*pbIsRide	= pCI->bIsRide;

			if(pCI->wSlaveExPer && (rand()%100)<pCI->wSlaveExPer)
				return pCI->wSlaveExID;
			else
				return pCI->wSlaveID;
		}
	}
	return 0;
}

SlaveFuseConfig::SlaveFuseConfig()
{
	m_pList	= xnList::Create();
}

SlaveFuseConfig::~SlaveFuseConfig()
{
	while(m_pList->Count)
	{
		delete (SlaveFuseInfo*)m_pList->Delete(0);
	}
	m_pList->Free();
}

void SlaveFuseConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;
	int i = 0;
	char szTmp[32];
	int nSctIndex	= 0;
	while (pStr->SetSection(nSctIndex++) != NULL)
	{
		SlaveFuseInfo* pSFI	= new SlaveFuseInfo();
		pSFI->bSuccessPer	= atoi(pStr->ReadString("successper", "0"));
		pSFI->bEffect	= atoi(pStr->ReadString("addeffect", "0"));
		pSFI->bFailBack	= atoi(pStr->ReadString("failback", "0"));
		pSFI->nCost= atoi(pStr->ReadString("cost", "0"));
		pSFI->nNeedVal	= atoi(pStr->ReadString("needVal", "0"));
		pSFI->nMinVal	= atoi(pStr->ReadString("minVal", "-10"));
		pSFI->nMaxVal	= atoi(pStr->ReadString("maxVal", "30"));
		m_pList->Add(pSFI);
	}
	pStr->Free();
}

SlaveFuseInfo* SlaveFuseConfig::GetSFInfo( BYTE bLv )
{
	if(bLv >= m_pList->Count)
		return NULL;

	return (SlaveFuseInfo*)m_pList->Items[bLv];
}

void SlaveFuseConfig::ReloadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;
	int i = 0;
	char szTmp[32];
	int nSctIndex	= 0;
	while (pStr->SetSection(nSctIndex) != NULL)
	{
		if(nSctIndex >= m_pList->Count)
			break;
		SlaveFuseInfo* pSFI	= (SlaveFuseInfo*)m_pList->Items[nSctIndex];
		pSFI->bSuccessPer	= atoi(pStr->ReadString("successper", "0"));

		nSctIndex++;
	}
	pStr->Free();
}

SlaveChgBaseSpecial::SlaveChgBaseSpecial()
{
	m_pList		= xnList::Create();
}

SlaveChgBaseSpecial::~SlaveChgBaseSpecial()
{
	while(m_pList->Count)
		delete (SlaveChgBaseObj*)m_pList->Delete(0);
	m_pList->Free();
}

void SlaveChgBaseSpecial::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;

	int nSctIndex	= 0;
	while (pStr->SetSection(nSctIndex++) != NULL)
	{
		SlaveChgBaseObj* pSCB	= new SlaveChgBaseObj();
		m_pList->Add(pSCB);
		pSCB->wSlaveA	= atoi(pStr->ReadString("slaveA", "0"));
		pSCB->wSlaveB	= atoi(pStr->ReadString("slaveB", "0"));
		char szTmp[32];
		for (int i = 0; i < 6; i++)
		{
			sprintf(szTmp,"skillA%d",i+1);
			pSCB->wSkillA[i]	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"skillB%d",i+1);
			pSCB->wSkillB[i]	= atoi(pStr->ReadString(szTmp,"0"));
		}
	}
	pStr->Free();
}

SlaveChgBaseObj* SlaveChgBaseSpecial::GetChgBaseObj( WORD wSlave )
{
	for(int i = 0; i < m_pList->Count; i++)
	{
		SlaveChgBaseObj*	pSCB	= (SlaveChgBaseObj*)m_pList->Items[i];
		if(pSCB->wSlaveA == wSlave || pSCB->wSlaveB == wSlave)
			return pSCB;
	}
	return NULL;
}

SlaveFuseSkillCfg::SlaveFuseSkillCfg()
{
	m_pList		= xnList::Create();
	m_nTotalPer	= 0;
}

SlaveFuseSkillCfg::~SlaveFuseSkillCfg()
{
	while(m_pList->Count)
	{
		delete (SFuseSkillObj*)m_pList->Delete(0);
	}
	m_pList->Free();
}

void SlaveFuseSkillCfg::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;

	int nSctIndex	= 0;
	while (pStr->SetSection(nSctIndex++) != NULL)
	{
		SFuseSkillObj* pSFS	= new SFuseSkillObj();
		m_pList->Add(pSFS);
		pSFS->wSkill	= atoi(pStr->sectionName);
		pSFS->nPer		= atoi(pStr->ReadString("appPer", "0"));
		m_nTotalPer += pSFS->nPer;
		pSFS->byGet		= atoi(pStr->ReadString("getPer", "0"));
		pSFS->nIco		= atoi(pStr->ReadString("icon", "0"));
		pSFS->bType		= atoi(pStr->ReadString("type", "0"));
	}
	pStr->Free();
}

WORD SlaveFuseSkillCfg::RandomGetFSkill()
{
	int nRnd	= rand()%m_nTotalPer;
	for (int i = 0; i < m_pList->Count; i++)
	{
		SFuseSkillObj*	pSFS	= (SFuseSkillObj*)m_pList->Items[i];
		if(nRnd < pSFS->nPer)
			return pSFS->wSkill;

		nRnd	-= pSFS->nPer;
	}
	return 0;
}

SFuseSkillObj* SlaveFuseSkillCfg::GetFSkill( WORD wSID )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		SFuseSkillObj*	pSFS	= (SFuseSkillObj*)m_pList->Items[i];
		if(wSID == pSFS->wSkill)
			return pSFS;
	}
	return NULL;
}

SlavePhaseExCfg::SlavePhaseExCfg()
{
	m_pList		= xnList::Create();
}

SlavePhaseExCfg::~SlavePhaseExCfg()
{
	while(m_pList->Count)
	{
		delete	(SlavePExObj*)m_pList->Delete(0);
	}
	m_pList->Free();
}

void SlavePhaseExCfg::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;

	int nSctIndex	= 0;
	while (pStr->SetSection(nSctIndex++) != NULL)
	{
		SlavePExObj* pSPE	= new SlavePExObj();
		m_pList->Add(pSPE);
		pSPE->bLv		= atoi(pStr->sectionName);
		pSPE->bSuccess	= atoi(pStr->ReadString("success", "0"));
		pSPE->dwSoul	= atoi(pStr->ReadString("needsoul", "9999"));
		pSPE->dwCostVal	= atoi(pStr->ReadString("needval", "9999"));
		pSPE->nEffAtkW	= atoi(pStr->ReadString("effatkw", "0"));
		pSPE->nEffAtkN	= atoi(pStr->ReadString("effatkn", "0"));
		pSPE->nEffDefW	= atoi(pStr->ReadString("effdefw", "0"));
		pSPE->nEffDefN	= atoi(pStr->ReadString("effdefn", "0"));
		pSPE->nEffHp	= atoi(pStr->ReadString("effhp", "0"));
		pSPE->nEffFuse	= atoi(pStr->ReadString("efffuse", "0"));
		pSPE->wEffID	= atoi(pStr->ReadString("clienteff", "0"));
	}
	pStr->Free();
}

SlavePExObj* SlavePhaseExCfg::GetPhaseExObj( BYTE bLv )
{
	if(bLv >= m_pList->Count)
		return NULL;

	return (SlavePExObj*)m_pList->Items[bLv];
}