#include "stdafx.h"
#include "SlaveConfig.h"
#include "xnExtstr.h"
#ifdef _ISSERVER
#include "UserData.h"
#include "ChatCenter.h"
#endif
CreateSingleton(SlaveBaseCfg);
CreateSingleton(SlaveStrengthenCfg);
CreateSingleton(SlaveVariatCfg);
#ifdef _ISSERVER
extern int Random_Int(int min, int max);
#endif
static void GetNumFormStr(std::map<int,int>& mapAA,char* pTxt)
{
	if(pTxt == NULL)
		return;
	char* p1 = pTxt;
	char* p2 = strstr(pTxt,"|");
	int nIndex = 0;
	while(p2 != NULL)
	{	
		*p2 = '\0';
		int nVal = atoi(p1);
		if(nVal > 0)
			mapAA.insert(std::make_pair(nIndex,nVal));
		p1 = p2 + 1;
		p2 = strstr(p1,"|");
		nIndex ++;
	}
	if(p1 != NULL)
	{
		int nVal = atoi(p1);
		if(nVal > 0)
			mapAA.insert(std::make_pair(nIndex,nVal));
	}
}

SlaveBaseCfg::SlaveBaseCfg()
{
	m_pList = xnList::Create();
	m_pLvList = xnList::Create();
#ifdef _ISSERVER
	m_pQWhiteList = xnList::Create();
	m_pQGreenList = xnList::Create();
	m_pQBlueList = xnList::Create();
	m_pQPurpleList = xnList::Create();
#endif
}

SlaveBaseCfg::~SlaveBaseCfg()
{
	if(m_pList)
	{
		while(m_pList->Count)
			delete (SlaveBasic*)m_pList->Delete(0);
		m_pList->Free();
	}
#ifdef _ISSERVER
	if(m_pQWhiteList)
	{
		while(m_pQWhiteList->Count)
			m_pQWhiteList->Delete(0);
		m_pQWhiteList->Free();
	}
	if(m_pQGreenList)
	{
		while(m_pQGreenList->Count)
			m_pQGreenList->Delete(0);
		m_pQGreenList->Free();
	}
	if(m_pQBlueList)
	{
		while(m_pQBlueList->Count)
			m_pQBlueList->Delete(0);
		m_pQBlueList->Free();
	}
	if(m_pQPurpleList)
	{
		while(m_pQPurpleList->Count)
			m_pQPurpleList->Delete(0);
		m_pQPurpleList->Free();
	}
#endif
	if(m_pLvList)
	{
		while(m_pLvList->Count)
			m_pLvList->Delete(0);
		m_pLvList->Free();
	}
}

void SlaveBaseCfg::LoadFile(char * pFileName)
{
	xnExtstr* pStr = xnExtstr::Import(pFileName);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	while(pStr->SetSection(nIndex ++))
	{
		SlaveBasic* pBase = new SlaveBasic();
		pBase->nID = atoi(pStr->sectionName);
		char* szTemp = strdup(pStr->ReadString("name","宠物"));
		char* p = strstr(szTemp,"//");
		if(p)
			*p = '\0';
		pBase->pName = strdup(szTemp);
		pBase->szBaseParam[SLAVE_PARAM_HPMAX] = atoi(pStr->ReadString("hp","100"));
		pBase->szBaseParam[SLAVE_PARAM_MPMAX] = atoi(pStr->ReadString("mp","100"));
		pBase->szBaseParam[SLAVE_PARAM_AD] = atoi(pStr->ReadString("ad","100"));
		pBase->szBaseParam[SLAVE_PARAM_DD] = atoi(pStr->ReadString("dd","100"));
		pBase->szBaseParam[SLAVE_PARAM_AP] = atoi(pStr->ReadString("ap","100"));
		pBase->szBaseParam[SLAVE_PARAM_DP] = atoi(pStr->ReadString("dp","100"));
		pBase->szBaseUp[SLAVE_PARAM_HPMAX] = atoi(pStr->ReadString("hpup","1"));
		pBase->szBaseUp[SLAVE_PARAM_MPMAX] = atoi(pStr->ReadString("mpup","1"));
		pBase->szBaseUp[SLAVE_PARAM_AD] = atoi(pStr->ReadString("adup","1"));
		pBase->szBaseUp[SLAVE_PARAM_DD] = atoi(pStr->ReadString("ddup","1"));
		pBase->szBaseUp[SLAVE_PARAM_AP] = atoi(pStr->ReadString("apup","1"));
		pBase->szBaseUp[SLAVE_PARAM_DP] = atoi(pStr->ReadString("dpup","1"));
		pBase->nSkill = atoi(pStr->ReadString("skill","0"));
		pBase->byQuality = atoi(pStr->ReadString("quality","0")); 
		pBase->nSkin = atoi(pStr->ReadString("skin","1000"));
		pBase->byLvLimit = atoi(pStr->ReadString("lvlimit","50"));
		pBase->byExchangLv = atoi(pStr->ReadString("exchangelv","0"));
		pBase->nExchangeID = atoi(pStr->ReadString("exchangeid","1000"));
		pBase->nExchangeGold = atoi(pStr->ReadString("exchangegold","1000"));
		pBase->nAtkDis = atoi(pStr->ReadString("atkdistance","5"));
		GetNumFormStr(pBase->mapExchangeCost,pStr->ReadString("exchangecost"));
		GetNumFormStr(pBase->mapExchangeItem,pStr->ReadString("exchangeitem"));
		m_pList->Add((void*)pBase);
#ifdef _ISSERVER
		if (pBase->nID < 2100&&pBase->byQuality == 0)
		{
			m_pQWhiteList->Add((void*)pBase);
		}
		if (pBase->nID < 2100&&pBase->byQuality == 1)
		{
			m_pQGreenList->Add((void*)pBase);
		}
		if (pBase->nID < 2100&&pBase->byQuality == 2)
		{
			m_pQBlueList->Add((void*)pBase);
		}
		if (pBase->nID < 2100&&pBase->byQuality == 3)
		{
			m_pQPurpleList->Add((void*)pBase);
		}
#endif
	}
	pStr->Free();
}

SlaveBasic* SlaveBaseCfg::GetBaseInfo(int nID)
{
	for(int i = 0; i < m_pList->Count; i ++)
	{
		SlaveBasic* pInfo = (SlaveBasic*)m_pList->Items[i];
		if(pInfo->nID == nID)
			return pInfo;
	}
	return NULL;
}

void SlaveBaseCfg::LoadLvFile(char* pFileName)
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

int SlaveBaseCfg::GetLvExp(int nLv)
{
	if(nLv-1 >=m_pLvList->Count || nLv <= 0)
		return 0;
	return (int)m_pLvList->Items[nLv-1];
}
#ifdef _ISSERVER
int SlaveBaseCfg::RandSlaveID(int nType,UserData* pUser)
{
	if (nType<1||nType>3)
	{
		return 0;
	}
	int nRand = Random_Int(0, 10000);
	if (nType == 1)
	{
		if (m_pQBlueList->Count&&nRand < 500)
		{
			int nRandCount = Random_Int(0, m_pQBlueList->Count-1);
			SlaveBasic* RandSlave = (SlaveBasic*)m_pQBlueList->Items[nRandCount];
			return RandSlave->nID;
		}else if (m_pQGreenList->Count&&nRand < 5750)
		{
			int nRandCount = Random_Int(0, m_pQGreenList->Count-1);
			SlaveBasic* RandSlave = (SlaveBasic*)m_pQGreenList->Items[nRandCount];
			return RandSlave->nID;
		}else if (m_pQWhiteList->Count)
		{
			int nRandCount = Random_Int(0, m_pQWhiteList->Count-1);
			SlaveBasic* RandSlave = (SlaveBasic*)m_pQWhiteList->Items[nRandCount];
			return RandSlave->nID;
		}
	}
	if (nType == 2)
	{
		if (m_pQPurpleList->Count&&nRand < 200)
		{
			int nRandCount = Random_Int(0, m_pQPurpleList->Count-1);
			SlaveBasic* RandSlave = (SlaveBasic*)m_pQPurpleList->Items[nRandCount];

			char szMsg[128];
			sprintf(szMsg,"人品大爆发，[ %s ]经过不懈努力，终于召唤了4星的宠物[ %s ]",pUser->m_szNick,RandSlave->pName);
			ChatCenter::GetInstance().SendSystemMsg(szMsg,0,0,1);

			return RandSlave->nID;
		}else if (m_pQBlueList->Count&&nRand < 2200)
		{
			int nRandCount = Random_Int(0, m_pQBlueList->Count-1);
			SlaveBasic* RandSlave = (SlaveBasic*)m_pQBlueList->Items[nRandCount];
			return RandSlave->nID;
		}else if (m_pQGreenList->Count)
		{
			int nRandCount = Random_Int(0, m_pQGreenList->Count-1);
			SlaveBasic* RandSlave = (SlaveBasic*)m_pQGreenList->Items[nRandCount];
			return RandSlave->nID;
		}
	}
	if (nType == 3)
	{
		if (m_pQPurpleList->Count&&nRand < 500)
		{
			int nRandCount = Random_Int(0, m_pQPurpleList->Count-1);
			SlaveBasic* RandSlave = (SlaveBasic*)m_pQPurpleList->Items[nRandCount];
			
			char szMsg[128];
			sprintf(szMsg,"人品大爆发，[ %s ]经过不懈努力，终于召唤了4星的宠物[ %s ]",pUser->m_szNick,RandSlave->pName);
			ChatCenter::GetInstance().SendSystemMsg(szMsg,0,0,1);

			return RandSlave->nID;
		}else if (m_pQBlueList->Count)
		{
			int nRandCount = Random_Int(0, m_pQBlueList->Count-1);
			SlaveBasic* RandSlave = (SlaveBasic*)m_pQBlueList->Items[nRandCount];
			return RandSlave->nID;
		}
	}
	return 0;
}
#endif
SlaveStrengthenCfg::SlaveStrengthenCfg()
{
	m_pList = xnList::Create();
}

SlaveStrengthenCfg::~SlaveStrengthenCfg()
{
	if(m_pList)
	{
		while(m_pList->Count)
			delete (SlaveStreBasic*)m_pList->Delete(0);
		m_pList->Free();
	}
}

void SlaveStrengthenCfg::LoadFile(char* pFileName)
{
	xnExtstr* pStr = xnExtstr::Import(pFileName);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	while(pStr->SetSection(nIndex++))
	{
		SlaveStreBasic* pBase = new SlaveStreBasic();
		pBase->nVal = atoi(pStr->ReadString("param","1"));
		pBase->nSucPer = atoi(pStr->ReadString("paramper","10"));
		pBase->nLvNeed = atoi(pStr->ReadString("paramlvneed","0"));
		pBase->nMoneyCost = atoi(pStr->ReadString("paramgoldneed","10000"));
		pBase->nItemCost = atoi(pStr->ReadString("paramcostneed","1"));
		m_pList->Add((void*)pBase);
	}
	pStr->Free();
}

SlaveStreBasic* SlaveStrengthenCfg::GetBase(BYTE byPos,int nLv)
{
	if(nLv == 0)
		return NULL;
	if(nLv > m_pList->Count)
		return NULL;
	return (SlaveStreBasic*)m_pList->Items[nLv-1];
}

SlaveVariatCfg::SlaveVariatCfg()
{
	m_pList = xnList::Create();
}

SlaveVariatCfg::~SlaveVariatCfg()
{
	if(m_pList)
	{
		while(m_pList->Count)
			delete (SlaveVariatBasic*)m_pList->Delete(0);
		m_pList->Free();
	}
}

void SlaveVariatCfg::LoadFile(char* pFileName)
{
	xnExtstr* pStr = xnExtstr::Import(pFileName);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	while(pStr->SetSection(nIndex++))
	{
		SlaveVariatBasic* pNew = new SlaveVariatBasic();
		pNew->nMoney	= atoi(pStr->ReadString("goldcost","1"));
		pNew->nVal		= atoi(pStr->ReadString("rlt1","100"));
		int nSuc = atoi(pStr->ReadString("sucper","100"));
		for(int i = 0; i < 5; i ++)
		{
			char szName[32];
			sprintf(szName,"support%d",i+1);
			int nTemp = atoi(pStr->ReadString(szName,"750"));
			pNew->nSucPer[i] = nTemp*nSuc/100;
		}
		m_pList->Add((void*)pNew);
	}
	pStr->Free();
}

SlaveVariatBasic* SlaveVariatCfg::GetBase(BYTE byValv)
{
	if(byValv > m_pList->Count || byValv == 0)
		return NULL;
	return (SlaveVariatBasic*)m_pList->Items[byValv-1];
}