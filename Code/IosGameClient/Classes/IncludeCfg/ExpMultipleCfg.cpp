#include "stdafx.h"
#include "ExpMultipleCfg.h"
#include "xnExtstr.h"

CreateSingleton(ExpMultipleCfg);

ExpMultipleCfg::ExpMultipleCfg()
{
	m_pList = xnList::Create();
}

ExpMultipleCfg::~ExpMultipleCfg()
{
	if(m_pList)
	{
		while(m_pList->Count > 0)
			delete (ExpMulObj*)m_pList->Delete(0);
		m_pList->Free();
	}
}

void ExpMultipleCfg::LoadFile(char* pFile)
{
	xnExtstr* pStr = xnExtstr::Import(pFile);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	while(pStr->SetSection(nIndex ++))
	{
		ExpMulObj* pObj = new ExpMulObj();
		pObj->nID = atoi(pStr->sectionName);
		pObj->szName = strdup(pStr->ReadString("name"));
		if(pObj->szName)
			FixDescString(pObj->szName);
		pObj->nAddPer = atoi(pStr->ReadString("addper","0"));
		pObj->ifDisOnline = atoi(pStr->ReadString("disonline","1"));
		pObj->ifDisDie = atoi(pStr->ReadString("disdie","1"));
		pObj->ifTimeAdd = atoi(pStr->ReadString("timeadd","0"));
		pObj->nTime = atoi(pStr->ReadString("time","0"));
		pObj->byTargent = atoi(pStr->ReadString("targent","0"));
		pObj->nFamily = atoi(pStr->ReadString("family","0"));
		m_pList->Add((void*)pObj);
	}
	pStr->Free();
}

ExpMulObj* ExpMultipleCfg::GetObj(int nID)
{
	for(int i = 0; i < m_pList->Count; i ++)
	{
		ExpMulObj* pObj = (ExpMulObj*)m_pList->Items[i];
		if(pObj->nID == nID)
			return pObj;
	}
	return NULL;
}