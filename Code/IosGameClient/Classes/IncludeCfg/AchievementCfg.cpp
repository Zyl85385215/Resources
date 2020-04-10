#include "stdafx.h"
#include "AchievementCfg.h"
#include "xnExtstr.h"
#include "GameAwardCfg.h"

CreateSingleton(AchievementCfg);

AchievementCfg::AchievementCfg()
{
	m_pGroupList	= xnList::Create();
	m_pFloorList	= xnList::Create();
	m_pObjList		= xnList::Create();
	m_pAwardList	= xnList::Create();
}

AchievementCfg::~AchievementCfg()
{
	if(m_pGroupList)
	{
		while(m_pGroupList->Count)
			delete (ATileObj*)m_pGroupList->Delete(0);
		m_pGroupList->Free();
	}
	if(m_pFloorList)
	{
		while(m_pFloorList->Count)
			delete (ATileObj*)m_pFloorList->Delete(0);
		m_pFloorList->Free();
	}
	if(m_pObjList)
	{
		while(m_pObjList->Count)
			delete (AchievemetnObj*)m_pObjList->Delete(0);
		m_pObjList->Free();
	}
	if(m_pAwardList)
	{
		while(m_pAwardList->Count)
		{
			AchieveAward*pObj = (AchieveAward*)m_pAwardList->Delete(0);
			if(pObj->pList)
			{
				while(pObj->pList->Count)
					delete (_simpleAward*)pObj->pList->Delete(0);
				pObj->pList->Free();
			}
			delete pObj;
		}
		m_pAwardList->Free();
	}
}

ATileObj*	AchievementCfg::GetFloor(int nID)
{
	for(int i = 0; i < m_pFloorList->Count; i ++)
	{
		ATileObj* pObj = (ATileObj*)m_pFloorList->Items[i];
		if(pObj->nID == nID)
			return pObj;
	}
	return NULL;
}

ATileObj*	AchievementCfg::GetGroup(int nID)
{
	for(int i = 0; i < m_pGroupList->Count; i ++)
	{
		ATileObj* pObj = (ATileObj*)m_pGroupList->Items[i];
		if(pObj->nID == nID)
			return pObj;
	}
	return NULL;
}

void	AchievementCfg::AddFloorCnt(int nID,int nVal)
{
	for(int i = 0; i < m_pFloorList->Count; i ++)
	{
		ATileObj* pObj = (ATileObj*)m_pFloorList->Items[i];
		if(pObj->nID == nID)
		{
			pObj->nCount += nVal;
			break;
		}
	}
}

void	AchievementCfg::AddGroupCnt(int nID,int nVal)
{
	for(int i = 0; i < m_pGroupList->Count; i ++)
	{
		ATileObj* pObj = (ATileObj*)m_pGroupList->Items[i];
		if(pObj->nID == nID)
		{
			pObj->nCount += nVal;
			break;
		}
	}
}

void	AchievementCfg::LoadFile(char* pFile)
{
	xnExtstr* pStr = xnExtstr::Import(pFile);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	if(pStr->SetSection(nIndex ++))
	{
		int nCnt = atoi(pStr->ReadString("cnt","0"));
		char szTemp[32];
		for(int i = 0; i < nCnt; i ++)
		{
			ATileObj* pObj = new ATileObj();
			sprintf(szTemp,"name%d",i+1);
			pObj->pName = strdup(pStr->ReadString(szTemp,"未知"));
			FixDescString(pObj->pName);
			sprintf(szTemp,"id%d",i+1);
			pObj->nID = atoi(pStr->ReadString(szTemp,"1"));
			sprintf(szTemp,"page%d",i+1);
			pObj->nGroup = atoi(pStr->ReadString(szTemp,"1"));
			pObj->nCount = 0;
			m_pGroupList->Add(pObj);
		}
	}
	if(pStr->SetSection(nIndex ++))
	{
		int nCnt = atoi(pStr->ReadString("cnt","0"));
		char szTemp[32];
		for(int i = 0; i < nCnt; i ++)
		{
			ATileObj* pObj = new ATileObj();
			sprintf(szTemp,"name%d",i+1);
			pObj->pName = strdup(pStr->ReadString(szTemp,"未知"));
			FixDescString(pObj->pName);
			sprintf(szTemp,"id%d",i+1);
			pObj->nID = atoi(pStr->ReadString(szTemp,"1"));
			sprintf(szTemp,"group%d",i+1);
			pObj->nGroup = atoi(pStr->ReadString(szTemp,"1"));
			pObj->nCount = 0;
			m_pFloorList->Add(pObj);
		}
	}
	while(pStr->SetSection(nIndex ++))
	{
		AchievemetnObj* pObj = new AchievemetnObj();
		pObj->nID = atoi(pStr->sectionName);
		pObj->nFloor = atoi(pStr->ReadString("floor","1"));
		pObj->nPoint = atoi(pStr->ReadString("point","0"));
		pObj->pName = strdup(pStr->ReadString("name","未知"));
		pObj->pIcon = strdup(pStr->ReadString("icon",""));
		pObj->pDesc = strdup(pStr->ReadString("desc","未知"));
		pObj->nCnt = atoi(pStr->ReadString("cnt","1"));
		AddFloorCnt(pObj->nFloor,1);
		m_pObjList->Add(pObj);
	}
	for(int i = 0; i < m_pFloorList->Count; i ++)
	{
		ATileObj* pObj = (ATileObj*)m_pFloorList->Items[i];
		AddGroupCnt(pObj->nGroup,pObj->nCount);
	}
	pStr->Free();
}

void	AchievementCfg::LoadAwardFile(char* pFile)
{
	xnExtstr* pStr = xnExtstr::Import(pFile);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	while(pStr->SetSection(nIndex++))
	{
		AchieveAward* pAward = new AchieveAward();
		pAward->byType = atoi(pStr->ReadString("type","0"));
		pAward->nValue = atoi(pStr->ReadString("value","1000"));
		pAward->pName = strdup(pStr->ReadString("name","新人"));
		pAward->pDesc = strdup(pStr->ReadString("desc","描述"));
		pAward->pList = xnList::Create();
		char szName[32];
		sprintf(szName,"item");
		char* pTemp1 = pStr->ReadString(szName);
		sprintf(szName,"num");
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
			pAward->pList->Add((void*)pSimA);
		}
		m_pAwardList->Add((void*)pAward);
	}

	pStr->Free();
}

void	AchievementCfg::GetObjList(int nFloor,OUT xnList* pList)
{
	if(pList == NULL)
		pList = xnList::Create();
	for(int i = 0; i < m_pObjList->Count; i ++)
	{
		AchievemetnObj* pObj = (AchievemetnObj*)m_pObjList->Items[i];
		if(pObj->nFloor == nFloor)
			pList->Add(pObj);
	}
}

AchievemetnObj*	AchievementCfg::GetObj(int nID)
{
	for(int i = 0; i < m_pObjList->Count; i ++)
	{
		AchievemetnObj* pObj = (AchievemetnObj*)m_pObjList->Items[i];
		if(pObj->nID == nID)
			return pObj;
	}
	return NULL;
}