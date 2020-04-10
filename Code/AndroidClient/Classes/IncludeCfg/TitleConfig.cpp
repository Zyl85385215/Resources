#include "stdafx.h"
#include "TitleConfig.h"
#include "xnExtstr.h"

CreateSingleton(TitleConfig);


TitleConfig::TitleConfig()
{
	m_pList = xnList::Create();
}

TitleConfig::~TitleConfig()
{
	if(m_pList)
	{
		while(m_pList->Count > 0)
			delete (TitleObj*)m_pList->Delete(0);
		m_pList->Free();
	}
}

void TitleConfig::LoadFile(char * pFile)
{
	xnExtstr* pStr = xnExtstr::Import(pFile);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	int ntotalval = 0;
	while(pStr->SetSection(nIndex++))
	{
		TitleObj* pObj = new TitleObj();
		ntotalval += atoi(pStr->ReadString("needval","0"));
		pObj->nNeedVal = ntotalval;
		pObj->nLostVal = atoi(pStr->ReadString("lostval","0"));
		pObj->nSupportVal = atoi(pStr->ReadString("supportval","0"));
		pObj->nHpAdd = atoi(pStr->ReadString("hp","0"));
		pObj->nMpAdd = atoi(pStr->ReadString("mp","0"));
		pObj->nAdAdd = atoi(pStr->ReadString("ad","0"));
		pObj->nApAdd = atoi(pStr->ReadString("ap","0"));
		pObj->nDdAdd = atoi(pStr->ReadString("dd","0"));
		pObj->nDpAdd = atoi(pStr->ReadString("dp","0"));
		pObj->pName1 = strdup(pStr->ReadString("name1","平民"));
		FixDescString(pObj->pName1);
		pObj->pName2 = strdup(pStr->ReadString("name2","平民"));
		FixDescString(pObj->pName2);
		pObj->nLvl = m_pList->Count;
		m_pList->Add((void*)pObj);
	}
	pStr->Free();
}

TitleObj* TitleConfig::GetNextObj(int nHonour)
{

	int nTemp = nHonour;
	for(int i = 0; i < m_pList->Count; i ++)
	{
		TitleObj* pObj = (TitleObj*)m_pList->Items[i];
		if(nTemp < pObj->nNeedVal)
			return pObj;
		nTemp -= pObj->nNeedVal;
	}
	return NULL;
}

TitleObj* TitleConfig::GetObj(int nHonour)
{
	int nLvl = -1;
	if(nHonour == 0)
		nLvl = 0;
	if(nLvl == -1)
	{
		int nTemp = nHonour;
		for(int i = 0; i < m_pList->Count; i ++)
		{
			TitleObj* pObj = (TitleObj*)m_pList->Items[i];
			if(nTemp < pObj->nNeedVal)
			{
				nLvl = i - 1;
				break;
			}

			if (i == m_pList->Count-1)
				return pObj;

			nTemp -= pObj->nNeedVal;
		}
	}
	if(nLvl < m_pList->Count && nLvl >= 0)
		return (TitleObj*)m_pList->Items[nLvl];
	return NULL;
}

void TitleConfig::GetPrograssVal(int nHonour,int& nCur,int& nMax)
{
	nCur = 999;
	nMax = 999;
	int nTemp = nHonour;
	for(int i = 0; i < m_pList->Count; i ++)
	{
		TitleObj* pObj = (TitleObj*)m_pList->Items[i];
		if(nTemp < pObj->nNeedVal)
		{
			nMax = pObj->nNeedVal;
			nCur = nTemp;
			return;
		}
		nTemp -= pObj->nNeedVal;
	}
}

int TitleConfig::GetMaxHonour()
{
	int nTmpVal = 0;
	for(int i = 0; i < m_pList->Count; i ++)
	{
		TitleObj* pObj = (TitleObj*)m_pList->Items[i];
		if (pObj)
		{
			nTmpVal += pObj->nNeedVal;
		}
	}

	return nTmpVal;
}