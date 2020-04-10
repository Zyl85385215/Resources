#include "stdafx.h"
#include "TestAwardCfg.h"
#include "xnExtstr.h"
#include <stdio.h>
#include <sstream>
#include <vector>

CreateSingleton(TestAwardCfg);

TestAwardCfg::TestAwardCfg()
{
	m_pRankAward = xnList::Create();
	m_pTestAward = xnList::Create();
	m_byStartH = 0;
	m_byStartM = 0;
	m_byEndH = 0;
	m_byEndM = 0;
}

TestAwardCfg::~TestAwardCfg()
{
	if(m_pRankAward)
	{
		while(m_pRankAward->Count)
			delete (AwardObj*)m_pRankAward->Delete(0);
		m_pRankAward->Free();
	}
	if(m_pTestAward)
	{
		while(m_pTestAward->Count)
			delete (AwardObj*)m_pTestAward->Delete(0);
		m_pTestAward->Free();
	}
}

void TestAwardCfg::LoadFile(char* pFile)
{
	xnExtstr* pStr = xnExtstr::Import(pFile);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	if(pStr->SetSection(nIndex++))
	{
		char* szTemp = pStr->ReadString("starttime","0:0");
		sscanf(szTemp,"%d:%d",&m_byStartH,&m_byStartM);
		int nLH,nLM;

		szTemp = pStr->ReadString("lasttime","0:0");
		sscanf(szTemp,"%d:%d",&nLH,&nLM);
		m_byEndH = m_byStartH + nLH;
		m_byEndM = m_byStartM + nLM;

		m_nACount = atoi(pStr->ReadString("count","10"));
		int nCnt = atoi(pStr->ReadString("cnt","0"));
		char szName[32];
		for(int i = 0; i < nCnt; i ++)
		{
			AwardObj* pObj = new AwardObj();
			pObj->pItemList = xnList::Create();
			sprintf(szName,"money%d",i+1);
			pObj->nMoney = atoi(pStr->ReadString(szName,"1"));
			sprintf(szName,"exp%d",i+1);
			pObj->nExp = atoi(pStr->ReadString(szName,"1"));
			m_pTestAward->Add((void*)pObj);
		}
		
	}
	while(pStr->SetSection(nIndex++))
	{
		AwardObj* pObj = new AwardObj();
		pObj->pItemList = xnList::Create();
		pObj->nMoney = atoi(pStr->ReadString("money","1"));
		pObj->nExp = atoi(pStr->ReadString("exp","0"));
		char* szItem = strdup(pStr->ReadString("item",""));
		std::string strItem(szItem);
		std::vector<std::string> vec;
		StringSplit(szItem,"|",vec);
		for(int i = 0; i < vec.size(); i ++)
		{
			int nID = atoi(vec[i].c_str());
			if(nID > 0)
				pObj->pItemList->Add((void*)nID);
		}
		//free(szItem);
		m_pRankAward->Add((void*)pObj);
	}
	pStr->Free();
}

AwardObj* TestAwardCfg::GetAwardByStar(int nStar)
{
	if(nStar > m_pTestAward->Count || nStar <= 0)
		return NULL;
	return (AwardObj*)m_pTestAward->Items[nStar-1];
}

AwardObj* TestAwardCfg::GetAwardByRank(int nPlace)
{
	if(nPlace >= m_pRankAward->Count || nPlace < 0)
		return NULL;
	return (AwardObj*)m_pRankAward->Items[nPlace];
}