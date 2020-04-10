#include "stdafx.h"
#include "HorseConfig.h"
#include "xnExtstr.h"

CreateSingleton(HorseBaseCfg);

HorseBaseCfg::HorseBaseCfg()
{
	m_pList = xnList::Create();
	m_pLvList = xnList::Create();
}

HorseBaseCfg::~HorseBaseCfg()
{
	if(m_pList)
	{
		while(m_pList->Count)
			delete (HorseBasic*)m_pList->Delete(0);
		m_pList->Free();
	}
	if(m_pLvList)
	{
		while(m_pLvList->Count)
			m_pLvList->Delete(0);
		m_pLvList->Free();
	}
}

void HorseBaseCfg::LoadFile(char * pFileName)
{
	xnExtstr* pStr = xnExtstr::Import(pFileName);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	while(pStr->SetSection(nIndex++))
	{
		HorseBasic* pBasic = new HorseBasic();
		pBasic->nID = atoi(pStr->sectionName);
		char* szTemp = strdup(pStr->ReadString("name","×øÆï"));
		char* p = strstr(szTemp,"//");
		if(p)
			*p = '\0';
		pBasic->pName = strdup(szTemp);
		pBasic->szBaseParam[HORSE_PARAM_HPMAX] = atoi(pStr->ReadString("hp","100"));
		pBasic->szBaseParam[HORSE_PARAM_MPMAX] = atoi(pStr->ReadString("mp","100"));
		pBasic->szBaseParam[HORSE_PARAM_AD] = atoi(pStr->ReadString("ad","100"));
		pBasic->szBaseParam[HORSE_PARAM_DD] = atoi(pStr->ReadString("dd","100"));
		pBasic->szBaseParam[HORSE_PARAM_AP] = atoi(pStr->ReadString("ap","100"));
		pBasic->szBaseParam[HORSE_PARAM_DP] = atoi(pStr->ReadString("dp","100"));
		pBasic->szBaseUp[HORSE_PARAM_HPMAX] = atoi(pStr->ReadString("hpup","1"));
		pBasic->szBaseUp[HORSE_PARAM_MPMAX] = atoi(pStr->ReadString("mpup","1"));
		pBasic->szBaseUp[HORSE_PARAM_AD] = atoi(pStr->ReadString("adup","1"));
		pBasic->szBaseUp[HORSE_PARAM_DD] = atoi(pStr->ReadString("ddup","1"));
		pBasic->szBaseUp[HORSE_PARAM_AP] = atoi(pStr->ReadString("apup","1"));
		pBasic->szBaseUp[HORSE_PARAM_DP] = atoi(pStr->ReadString("dpup","1"));
		pBasic->byQuality = atoi(pStr->ReadString("quality","0")); 
		std::string strTemp(pStr->ReadString("skin","1000"));
		std::vector<std::string> vecTemp;
		StringSplit(strTemp,"|",vecTemp);
		if(vecTemp.size() == 0)
		{
			pBasic->nSkin = 1000;
			pBasic->szUpSkin[0] = 1000;
			pBasic->szUpSkin[1] = 1000;
			pBasic->szUpSkin[2] = 1000;
		}
		else
		{
			pBasic->nSkin = atoi(vecTemp[0].c_str());
			for(int i = 0; i < 3; i ++)
			{
				if(i+1 < vecTemp.size())
					pBasic->szUpSkin[i] = atoi(vecTemp[i+1].c_str());
				else
					pBasic->szUpSkin[i] = pBasic->nSkin;
			}
		}
		strTemp.clear();
		vecTemp.clear();
		pBasic->byLvLimit = atoi(pStr->ReadString("lvlimit","50"));
		pBasic->nSpeed	= atoi(pStr->ReadString("speed","100"));
		pBasic->nPotential = atoi(pStr->ReadString("potential","10"));
		m_pList->Add((void*)pBasic);
	}
	pStr->Free();
}

void HorseBaseCfg::LoadLvFile(char* pFileName)
{
	xnExtstr* pStr = xnExtstr::Import(pFileName);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	while(pStr->SetSection(nIndex++))
	{
		int nExp = atoi(pStr->ReadString("exp","100"));
		m_pLvList->Add((void*)nExp);
	}
	pStr->Free();
}

HorseBasic* HorseBaseCfg::GetBasic(int nID)
{
	for(int i = 0; i < m_pList->Count; i ++)
	{
		HorseBasic* pInfo = (HorseBasic*)m_pList->Items[i];
		if(pInfo->nID == nID)
			return pInfo;
	}
	return NULL;
}

int HorseBaseCfg::GetLvExp(int nLv)
{
	if(nLv >m_pLvList->Count || nLv <= 0)
		return 0;
	return (int)m_pLvList->Items[nLv-1];
}

int HorseBaseCfg::GetFeedExp(int nLv,int nExp)
{
	int nTotal = nExp;
	for(int i = 0; i < nLv-1; i ++)
	{
		if(i < m_pLvList->Count)
			nTotal += (int)m_pLvList->Items[i];
	}
	return nTotal;
}