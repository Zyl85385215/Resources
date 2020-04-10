#include "stdafx.h"
#include "MentoringCfg.h"
#include "xnExtstr.h"
#include "GameAwardCfg.h"

CreateSingleton(MentoringCfg);
CreateSingleton(MarrySkillCfg);

MentoringCfg::MentoringCfg()
{
	m_pList = xnList::Create();
}

MentoringCfg::~MentoringCfg()
{
	if(m_pList)
	{
		while(m_pList->Count)
			delete (MentoringBase*)m_pList->Delete(0);
		m_pList->Free();
	}
}

void	MentoringCfg::LoadFile(char* pFile)
{
	xnExtstr* pStr = xnExtstr::Import(pFile);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	if(pStr->SetSection(nIndex++))
	{
		m_nExpaddid = atoi(pStr->ReadString("expaddid","0"));
		m_nDaylyexpcoe = atoi(pStr->ReadString("daylyexpcoe","0"));
		m_nDaylygoldcoe = atoi(pStr->ReadString("daylygoldcoe","0"));
		m_nDaylypower = atoi(pStr->ReadString("daylypower","0"));
		m_nDaylyhonour = atoi(pStr->ReadString("daylyhonour","0"));
		m_nGraduateexp = atoi(pStr->ReadString("graduateexp","0"));
		m_nGraduategold = atoi(pStr->ReadString("graduategold","0"));
		m_nGraduatehonour = atoi(pStr->ReadString("graduatehonour","0"));
	}
	int nVal = 0;
	while(pStr->SetSection(nIndex++))
	{
		MentoringBase* pInfo = new MentoringBase();
		pInfo->pList = xnList::Create();
		pInfo->byLv = nIndex;
		char * pTemp = pStr->ReadString("name","");
		FixDescString(pTemp);
		strncpy(pInfo->szName,pTemp,20);
		nVal += atoi(pStr->ReadString("needval","0"));
		pInfo->nNeedval = nVal;
		pInfo->nPuplecnt = atoi(pStr->ReadString("puplecnt","0"));
		std::string strDesc1(pStr->ReadString("exitemid",""));
		std::string strDesc2(pStr->ReadString("exitemnum",""));
		std::string strDesc3(pStr->ReadString("exitemper",""));
		std::vector<std::string> vec1,vec2,vec3;
		StringSplit(strDesc1,"|",vec1);
		StringSplit(strDesc2,"|",vec2);
		StringSplit(strDesc3,"|",vec3);
		int nCount = 0;
		nCount = nCount<vec1.size()?nCount:vec1.size();
		nCount = nCount<vec2.size()?nCount:vec2.size();
		nCount = nCount<vec3.size()?nCount:vec3.size();
		for(int i = 0; i < nCount; i ++)
		{
			_mbInfo* pM = new _mbInfo();
			pM->nID = atoi(vec1[i].c_str());
			pM->nNum = atoi(vec2[i].c_str());
			pM->nPer = atoi(vec3[i].c_str());
		}
		m_pList->Add((void*)pInfo);
	}
	pStr->Free();
}

MentoringBase*	MentoringCfg::GetCurBase(int nNum)
{
	MentoringBase* pRlt = NULL;
	for(int i = 0; i < m_pList->Count; i ++)
	{
		MentoringBase* pBase = (MentoringBase*)m_pList->Items[i];
		if(pBase->nNeedval > nNum)
		{
			if(i > 0)
				pRlt = (MentoringBase*)m_pList->Items[i-1];
			break;
		}
	}
	return pRlt;
}

MentoringBase*	MentoringCfg::GetNextBase(int nNum)
{
	MentoringBase* pRlt = NULL;
	for(int i = 0; i < m_pList->Count; i ++)
	{
		MentoringBase* pBase = (MentoringBase*)m_pList->Items[i];
		if(pBase->nNeedval > nNum)
		{
			pRlt = pBase;
			break;
		}
	}
	return pRlt;
}

MarrySkillCfg::MarrySkillCfg()
{
	m_pList = xnList::Create();
	m_pAwardList = xnList::Create();
}

MarrySkillCfg::~MarrySkillCfg()
{
	if(m_pList)
	{
		while(m_pList->Count)
		{
			MarrySKObj* pData = (MarrySKObj*)m_pList->Delete(0);
			if(pData->pLvList)
			{
				while(pData->pLvList->Count)
					delete (_simpleAward*)pData->pLvList->Delete(0);
				pData->pLvList->Free();
			}
			free(pData->pName);
			delete pData;
		}
		m_pList->Free();
	}
	if(m_pAwardList)
	{
		while(m_pAwardList->Count)
			delete (MarryAward*)m_pAwardList->Delete(0);
		m_pAwardList->Free();
	}
}

void	MarrySkillCfg::LoadFile(char* pFile)
{
	xnExtstr* pStr = xnExtstr::Import(pFile);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	if(pStr->SetSection(nIndex++))
	{
		int nCnt = atoi(pStr->ReadString("cnt","0"));
		char szTemp[32];
		for(int i = 0; i < nCnt; i ++)
		{
			MarryAward* pAward = new MarryAward();
			sprintf(szTemp,"need%d",i+1);
			pAward->nNeed = atoi(pStr->ReadString(szTemp,"10000"));
			sprintf(szTemp,"exp%d",i+1);
			pAward->nExp = atoi(pStr->ReadString(szTemp,"0"));
			sprintf(szTemp,"gold%d",i+1);
			pAward->nGold = atoi(pStr->ReadString(szTemp,"0"));
			sprintf(szTemp,"point%d",i+1);
			pAward->nPoint = atoi(pStr->ReadString(szTemp,"0"));
			m_pAwardList->Add((void*)pAward);
		}
	}
	while(pStr->SetSection(nIndex ++))
	{
		MarrySKObj* pObj = new MarrySKObj();
		pObj->pLvList = xnList::Create();
		pObj->pName = strdup(pStr->ReadString("name","ÃÎÖÐÇéÈË"));
		pObj->pIcon = strdup(pStr->ReadString("icon",""));
		pObj->byType = atoi(pStr->ReadString("type","0"));
		char szName[32];
		sprintf(szName,"cost");
		char* pTemp1 = pStr->ReadString(szName);
		sprintf(szName,"value");
		char* pTemp2 = pStr->ReadString(szName);
		std::string strTemp1(pTemp1);
		std::string strTemp2(pTemp2);
		std::vector<std::string> vec1,vec2;
		StringSplit(strTemp1,"|",vec1);
		StringSplit(strTemp2,"|",vec2);
		for(int i = 0; i < vec1.size(); i ++)
		{
			_simpleAward* pSimA = new _simpleAward();
			pSimA->nID = atoi(vec1[i].c_str());
			pSimA->nNum= atoi(vec2[i].c_str());
			pObj->pLvList->Add((void*)pSimA);
		}
		m_pList->Add((void*)pObj);
	}
	pStr->Free();
}