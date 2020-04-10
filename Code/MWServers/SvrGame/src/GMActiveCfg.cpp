#include "stdafx.h"
#include "stdio.h"
#include "GMActiveCfg.h"
#include "xnExtstr.h"
#include "FsGameCenter.h"
#include "CallMapEnemy.h"

CreateSingleton(GMActiveConfig);

GMActiveBase::GMActiveBase()
{
	nID = 0;
	nWaveCnt = 0;
	nNowWave = 0;
	nWaiteTick = 0;
	dwNextTick = 0;
	pBeginNotice = NULL;
	pOverNotice = NULL;

	pEnemyList   = xnList::Create();
	pBornPosList = xnList::Create();
}

GMActiveBase::~GMActiveBase()
{
	while(pEnemyList->Count)
		delete (_MuliParamObj*)pEnemyList->Delete(0);
	pEnemyList->Free();

	while(pBornPosList->Count)
		delete (EnemyPos*)pBornPosList->Delete(0);
	pBornPosList->Free();
}

int GMActiveBase::RandBornEnemy()
{
	if (pEnemyList->Count == 0)
		return 0;

	int nRandRlt = rand()%100;
	for (int i = 0; i < pEnemyList->Count; i++)
	{
		_MuliParamObj* pMP = (_MuliParamObj*)pEnemyList->Items[i];
		if (pMP&&pMP->nParam2 > nRandRlt)
			return pMP->nParam1;
		else
			nRandRlt -= pMP->nParam2;
	}
	return 0;
}

GMActiveConfig::GMActiveConfig()
{
	m_pList = xnList::Create();
}

GMActiveConfig::~GMActiveConfig()
{
	while(m_pList->Count)
		delete (GMActiveBase*)m_pList->Delete(0);
	m_pList->Free();
}

void GMActiveConfig::LoadFile(char* szFile)
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[256];
	while (pStr->SetSection(index++) != NULL)
	{
		GMActiveBase*	pBase	= new GMActiveBase();
		pBase->nID	    = atoi (pStr->sectionName);
		pBase->nMBaseIndex = atoi(pStr->ReadString("mapbase","0"));
		pBase->nScale = atoi(pStr->ReadString("scale","0"));
		pBase->nTotalCnt = atoi(pStr->ReadString("totalcnt","0"));

		int nPosCnt     = atoi(pStr->ReadString("poscnt","0"));
		char szTmp[32];
		for (int i = 0; i < nPosCnt; i++)
		{
			EnemyPos* pEnemyPos = new EnemyPos(); 
			sprintf(szTmp,"posx%d",i+1);
			pEnemyPos->nX = atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"posy%d",i+1);
			pEnemyPos->nY = atoi(pStr->ReadString(szTmp,"0"));

			pBase->pBornPosList->Add((void*)pEnemyPos);
		}

		int nEnemyCnt = atoi(pStr->ReadString("enemycnt","0"));
		for (int j = 0; j < nEnemyCnt; j++)
		{
			_MuliParamObj* pEnemy = new _MuliParamObj();
			sprintf(szTmp,"enemy%d",j+1);
			pEnemy->nParam1 = atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"enemyper%d",j+1);
			pEnemy->nParam2 = atoi(pStr->ReadString(szTmp,"0"));

			pBase->pEnemyList->Add((void*)pEnemy);
		}

		pBase->nWaveCnt	= atoi (pStr->ReadString ("wave","0"));
		pBase->nWaiteTick = atoi (pStr->ReadString ("waitetime","0"));

		pBase->pBeginNotice = strdup(pStr->ReadString("beginnotice",""));
		pBase->pOverNotice = strdup(pStr->ReadString("overnotice",""));

		m_pList->Add((void*)pBase);
	}
	pStr->Free ();
}

GMActiveBase* GMActiveConfig::GetActiveBase(int nID)
{
	if (m_pList->Count == 0)
		return NULL;

	for (int i = 0; i < m_pList->Count; i++)
	{
		GMActiveBase* pBase = (GMActiveBase*)m_pList->Items[i];
		if (pBase->nID == nID)
			return pBase;
	}

	return NULL;
}

GMActiveCenter::GMActiveCenter()
{
	m_pList = xnList::Create();
}

GMActiveCenter::~GMActiveCenter()
{
	while(m_pList->Count)
		m_pList->Delete(0);
	m_pList->Free();
}

void GMActiveCenter::AddActive(int nID,bool bOver/* = false*/)
{
	if (bOver == false)
	{
		GMActiveBase* pBase = GMActiveConfig::GetInstance().GetActiveBase(nID);
		if (pBase)
		{
			m_pList->Add((void*)pBase);
		}
	}else
	{
		for (int i = 0; i < m_pList->Count; i++)
		{
			GMActiveBase* pBase = (GMActiveBase*)m_pList->Items[i];
			if (pBase&&pBase->nID == nID)
			{
				if (strlen(pBase->pOverNotice))
				{
					ChatCenter::GetInstance().SendSystemMsg(pBase->pOverNotice,0,0,1);
				}

				pBase->nNowWave = 0;
				pBase->dwNextTick = 0;
				m_pList->Delete(i);
			}
		}
	}
	return;
}

void GMActiveCenter::Update()
{
	if (m_pList->Count == 0)
		return;

	DWORD dwNow = GetTickCount()/1000;

	for (int i = 0; i < m_pList->Count; i++)
	{
		GMActiveBase* pBase = (GMActiveBase*)m_pList->Items[i];
		if (pBase)
		{
			if (pBase->dwNextTick < dwNow)
			{
				if (pBase->dwNextTick == 0&&strlen(pBase->pBeginNotice))
				{
					ChatCenter::GetInstance().SendSystemMsg(pBase->pBeginNotice,0,0,1);
				}

				pBase->dwNextTick = dwNow + pBase->nWaiteTick;
				for (int j = 0; j < pBase->pBornPosList->Count; j++)
				{
					EnemyPos* pPos = (EnemyPos*)pBase->pBornPosList->Items[j];
					int nEnemyCnt = pBase->nTotalCnt;
					while(nEnemyCnt)
					{
						int nEnemyID = pBase->RandBornEnemy();
						if (nEnemyID)
						{
							g_pGameCenter->m_pCallEnemyCenter->CallEnemyByCfg(nEnemyID,pBase->nMBaseIndex,pPos->nX,pPos->nY,11,100,NULL,pBase->nScale);
							nEnemyCnt--;
						}
					}
				}
				pBase->nNowWave++;
			}
			if (pBase->nNowWave > pBase->nWaveCnt-1)
				AddActive(pBase->nID,true);
		}
	}
}