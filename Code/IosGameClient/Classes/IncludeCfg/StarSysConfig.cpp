#include "stdafx.h"
#include "StarSysConfig.h"
#include "xnExtstr.h"

CreateSingleton(StarSysCfg);

StarLvObject::StarLvObject()
{
	for(int i = 0; i< MAXJOB; i ++)
		pListParam[i]		= xnList::Create();

	szIco1		= NULL;
	szIco2		= NULL;
}

StarLvObject::~StarLvObject()
{
	free(szIco1);
	free(szIco2);
	for(int i = 0; i< MAXJOB; i ++)
	{
		while(pListParam[i]->Count)
		{
			delete (_MuliParamObj*)pListParam[i]->Delete(0);
		}
		pListParam[i]->Free();
	}
}

StarSysCfg::StarSysCfg()
{
	m_pList = xnList::Create();
}

StarSysCfg::~StarSysCfg()
{
	while(m_pList->Count > 0)
		delete (StarLvObject*)m_pList->Delete(0);
	m_pList->Free();
}

void StarSysCfg::LoadFile(char * pFile)
{
	xnExtstr* pStr = xnExtstr::Import(pFile);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	char	szTmp[64];
	MParam	xTParam[MAXJOB];
	for(int i = 0; i< MAXJOB; i ++)
		memset(&xTParam[i],0,sizeof(MParam));
	while(pStr->SetSection(nIndex++))
	{
		StarLvObject* pObj = new StarLvObject();

		pObj->byLv		= atoi(pStr->sectionName);
		pObj->szIco1	= strdup(pStr->ReadString("ico1",""));
		pObj->szIco2	= strdup(pStr->ReadString("ico2",""));
		
		for (int j = 0; j < MAXJOB; j++)
		{
			sprintf(szTmp, "paramcnt%d",j+1);
			int	nPCnt	= atoi(pStr->ReadString(szTmp,"0"));
			for (int p	= 0; p < nPCnt; p++)
			{
				_MuliParamObj*	pMP	= new _MuliParamObj();
				sprintf(szTmp, "paramtype%d%d",j+1,p+1);
				pMP->nParam1	= atoi(pStr->ReadString(szTmp,"0"));
				sprintf(szTmp, "paramval%d%d",j+1,p+1);
				pMP->nParam2	= atoi(pStr->ReadString(szTmp,"0"));
				pObj->pListParam[j]->Add(pMP);
				*((&xTParam[j].nHp)+pMP->nParam1)	= pMP->nParam2;
			}
			memcpy(&pObj->xParamTotal[j],&xTParam[j],sizeof(MParam));
		}
		
		pObj->nCostGold		= atoi(pStr->ReadString("costgold","0"));
		char* szCostExp = strdup(pStr->ReadString("costexp","0"));
		pObj->nCostExp = strtoul(szCostExp,&szCostExp,0);
		
		m_pList->Add(pObj);
	}
	pStr->Free();
}

StarLvObject* StarSysCfg::GetStarInfo( BYTE bLv )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		StarLvObject* pStar	= (StarLvObject*)m_pList->Items[i];
		if(pStar->byLv	== bLv)
			return pStar;
	}
	return NULL;
}
