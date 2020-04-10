#include "stdafx.h"
#include "ActivedegreeCfg.h"
#include "xnExtstr.h"

CreateSingleton(ActiveDegree);

ActiveDegree::ActiveDegree()
{
	m_pDaylyList = xnList::Create();
	memset(szAward,0,sizeof(ADAward)*MAX_DEGREE_LEVEL);
	for(int i = 0; i < MAX_DEGREE_LEVEL; i ++)
		szAward[i].pList = xnList::Create();
}

ActiveDegree::~ActiveDegree()
{
	delete m_pDaylyList;
	for(int i = 0; i < MAX_DEGREE_LEVEL; i++)
	{
		if(szAward[i].pList)
		{
			while(szAward[i].pList->Count)
				delete (ADAwardPart*)szAward[i].pList->Delete(0);
			szAward[i].pList->Free();
		}
	}
}

void ActiveDegree::GetNums(char* pMsg,xnList* pList)
{
	if(pMsg == NULL || pList == NULL)
		return;
	char* pTemp = NULL;
	char* p = pMsg;
	pTemp = strstr(p,"|");
	while(pTemp)
	{
		*pTemp = '\0';
		int nNum = atoi(p);
		pList->Add((void*)nNum);
		p = pTemp+1;
		pTemp = strstr(p,"|");
	}
	if(p)
	{
		int nNum = atoi(p);
		pList->Add((void*)nNum);
	}
}

void ActiveDegree::LoadFile(char * pFile)
{
	xnExtstr* pStr = xnExtstr::Import(pFile);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	if(pStr->SetSection(nIndex++))
	{
		int nCnt = atoi(pStr->ReadString("cnt","0"));
		for(int i =0; i < nCnt && i < 6; i ++)
		{
			char szName[32];
			sprintf(szName,"degree%d",i+1);
			szAward[i].nDegree = atoi(pStr->ReadString(szName,"1"));
			xnList* pList1 = xnList::Create();
			sprintf(szName,"id%d",i+1);
			GetNums(pStr->ReadString(szName),pList1);
			xnList* pList2 = xnList::Create();
			sprintf(szName,"num%d",i+1);
			GetNums(pStr->ReadString(szName),pList2);
			while(pList1->Count && pList2->Count)
			{
				ADAwardPart* pPart = new ADAwardPart();
				pPart->nID = (int)pList1->Delete(0);
				pPart->nCnt = (int)pList2->Delete(0);
				szAward[i].pList->Add((void*)pPart);
			}
			pList1->Free();
			pList2->Free();
		}
	}

	while(pStr->SetSection(nIndex++))
	{
		ADDaylyPart* pPart = new ADDaylyPart();
		pPart->nPoint = atoi(pStr->ReadString("point","0"));
		pPart->pList = xnList::Create();
		int nCnt = atoi(pStr->ReadString("cnt","0"));
		for(int i = 0; i < nCnt ; i ++)
		{
			char szName[32];
			ADPartChoose* pTemp = new ADPartChoose();
			sprintf(szName,"name%d",i+1);
			pTemp->pName = strdup(pStr->ReadString(szName,""));
			sprintf(szName,"icon%d",i+1);
			pTemp->pIocn = strdup(pStr->ReadString(szName,""));
			sprintf(szName,"form%d",i+1);
			pTemp->pForm = strdup(pStr->ReadString(szName,""));
			sprintf(szName,"comp%d",i+1);
			pTemp->pComp = strdup(pStr->ReadString(szName,""));
			sprintf(szName,"desc%d",i+1);
			pTemp->pDesc = strdup(pStr->ReadString(szName,""));
			sprintf(szName,"count%d",i+1);
			pTemp->nCount = atoi(pStr->ReadString(szName,"0"));
			sprintf(szName,"action%d",i+1);
			pTemp->byAction = atoi(pStr->ReadString(szName,"0"));
			sprintf(szName,"object%d",i+1);
			pTemp->byObject = atoi(pStr->ReadString(szName,"0"));
			sprintf(szName,"aoid%d",i+1);
			pTemp->nID = atoi(pStr->ReadString(szName,"0"));
			sprintf(szName,"aoval%d",i+1);
			pTemp->nVal = atoi(pStr->ReadString(szName,"0"));
			pPart->pList->Add(pTemp);
		}
		m_pDaylyList->Add(pPart);
	}
	pStr->Free();
}

int ActiveDegree::CheckLevel(int nPoint)
{
	if(nPoint == 0)
		return 0;
	for(int i = MAX_DEGREE_LEVEL-1; i >= 0; i --)
	{
		if(szAward[i].nDegree <= nPoint)
			return i+1;
	}
	return 0;
}