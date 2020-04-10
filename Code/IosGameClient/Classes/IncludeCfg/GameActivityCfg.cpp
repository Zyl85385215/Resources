#include "stdafx.h"
#include "GameActivityCfg.h"
#include "xnExtstr.h"
#include <vector>
#include <string>

CreateSingleton(GameActivityCfg);
CreateSingleton(TurntableCfg);
CreateSingleton(ACBossCfg);

GameActivityCfg::GameActivityCfg()
{
	m_pList = xnList::Create();
}

GameActivityCfg::~GameActivityCfg()
{
	if(m_pList)
	{
		while(m_pList->Count > 0)
			delete (ActivityObj*)m_pList->Delete(0);
		m_pList->Free();
	}
}

void GameActivityCfg::LoadFile(char* pFile)
{
	xnExtstr* pStr = xnExtstr::Import(pFile);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	while(pStr->SetSection(nIndex++))
	{
		ActivityObj* pNew = new ActivityObj();
		pNew->nID	= atoi(pStr->sectionName);
		pNew->pName	= strdup(pStr->ReadString("name","未知活动"));
		FixDescString(pNew->pName);
		pNew->byCountry = atoi(pStr->ReadString("country","0"));
		pNew->pIcon = strdup(pStr->ReadString("icon",""));
		FixDescString(pNew->pIcon);
		pNew->nLvMin= atoi(pStr->ReadString("lvmin","0"));
		pNew->nFlag= atoi(pStr->ReadString("flag","0"));
		pNew->nMaxnum = atoi(pStr->ReadString("nummax","0"));
		pNew->nLvMax= atoi(pStr->ReadString("lvmax","0"));
		pNew->nAwardType = atoi(pStr->ReadString("awardtype","0"));
		char * pTemp = pStr->ReadString("week","1|2|3|4|5|6|7");
		std::vector<std::string> veca;
		std::string stra(pTemp);
		StringSplit(stra,"|",veca);
		pNew->pWeekList = xnList::Create();
		for(int i = 0; i < veca.size(); i ++)
			pNew->pWeekList->Add((void*)(atoi(veca[i].c_str())));
		pTemp = pStr->ReadString("starttime","0:0");
		int nH1,nM1;
		sscanf(pTemp,"%d:%d",&nH1,&nM1);
		pNew->dwStartTime = (nH1<<16)+nM1;
		pTemp = pStr->ReadString("lasttime","0:0");
		int nH2,nM2;
		sscanf(pTemp,"%d:%d",&nH2,&nM2);
		nH2 += nH1;                                    
		nM2 += nM1;
		if(nM2 >= 60)
		{
			nM2 -= 60;
			nH2 ++;
		}
		pNew->dwEndTime = (nH2<<16)+nM2;
		pTemp = pStr->ReadString("registtime","0:0");
		sscanf(pTemp,"%d:%d",&nH2,&nM2);
		nH2 += nH1;
		nM2 += nM1;
		if(nM2 >= 60)
		{
			nM2 -= 60;
			nH2 ++;
		}
		pNew->dwRegistTime = (nH2<<16)+nM2;
		pNew->byActionEvent = atoi(pStr->ReadString("actionevent","0"));
		pNew->pObject = strdup(pStr->ReadString("actionobject",""));
		FixDescString(pNew->pObject);
		pNew->pDesc = strdup(pStr->ReadString("desc",""));
		FixDescString(pNew->pDesc);
		m_pList->Add((void*)pNew);
	}
	pStr->Free();
}

ActivityObj* GameActivityCfg::GetObj(int nID)
{
	for(int i = 0; i < m_pList->Count; i ++)
	{
		ActivityObj* pObj = (ActivityObj*)m_pList->Items[i];
		if(pObj->nID == nID)
			return pObj;
	}
	return NULL;
}

TurntableCfg::TurntableCfg()
{
	m_pList = xnList::Create();
	memset(m_szAward,0,sizeof(TurntableAward)*8);
}

TurntableCfg::~TurntableCfg()
{
	if(m_pList)
	{
		while(m_pList->Count)
			delete (TurntableAward*)m_pList->Delete(0);
		m_pList->Free();
	}
}

void TurntableCfg::LoadFile(char* pFile)
{
	xnExtstr* pStr = xnExtstr::Import(pFile);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	if(pStr->SetSection(nIndex++))
	{
		char* pTemp1 = pStr->ReadString("item");
		char* pTemp2 = pStr->ReadString("num");
		char* pTemp3 = pStr->ReadString("per");
		std::string strS1(pTemp1);
		std::string strS2(pTemp2);
		std::string strS3(pTemp3);
		std::vector<std::string> vec1,vec2,vec3;
		StringSplit(strS1,"|",vec1);
		StringSplit(strS2,"|",vec2);
		StringSplit(strS3,"|",vec3);
		int nTempPet = 0;
		for(int i = 0; i < vec1.size() &&i < 8; i ++)
		{
			m_szAward[i].nID = atoi(vec1[i].c_str());
			m_szAward[i].nNum = atoi(vec2[i].c_str());
			if(i == vec1.size()-1 || i == 7)
				nTempPet = 10000;
			else
				nTempPet +=atoi(vec3[i].c_str());
			m_szAward[i].nVal = nTempPet;
		}
	}

	while(pStr->SetSection(nIndex++))
	{
		TurntableAward* pNew = new TurntableAward();
		pNew->nID = atoi(pStr->ReadString("item","0"));
		pNew->nNum = atoi(pStr->ReadString("num","0"));
		pNew->nVal = atoi(pStr->ReadString("count","0"));
		m_pList->Add((void*)pNew);
	}
	pStr->Free();
}

int TurntableCfg::RandPos()
{
	int nRlt = rand()%10000;
	for(int i = 0; i < 8; i ++)
	{
		if(nRlt < m_szAward[i].nVal)
		{
			return i + 1;
		}
	}
	return 0;
}

ACBossCfg::ACBossCfg()
{
	m_pList = xnList::Create();
}

ACBossCfg::~ACBossCfg()
{
	if(m_pList)
	{
		while(m_pList->Count > 0)
			delete (BossInfoObj*)m_pList->Delete(0);
		m_pList->Free();
	}
}

void ACBossCfg::LoadFile(char* pFile)
{
	xnExtstr* pStr = xnExtstr::Import(pFile);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	while(pStr->SetSection(nIndex++))
	{
		BossInfoObj* pNew = new BossInfoObj();
		pNew->nID = atoi(pStr->ReadString("id","0"));
		pNew->nLv = atoi(pStr->ReadString("lv","0"));
		pNew->pName = strdup(pStr->ReadString("name","未知名"));
		pNew->pIcon =atoi(pStr->ReadString("icon","0"));
		pNew->pDesc = strdup(pStr->ReadString("txt","无"));
		pNew->nMap =atoi(pStr->ReadString("map","0"));
		m_pList->Add((void*)pNew);
	}
	pStr->Free();
}

BossInfoObj* ACBossCfg::GetObj(int nID)
{
	for(int i = 0; i < m_pList->Count; i ++)
	{
		BossInfoObj* pObj = (BossInfoObj*)m_pList->Items[i];
		if(pObj->nID == nID)
			return pObj;
	}
	return NULL;
}