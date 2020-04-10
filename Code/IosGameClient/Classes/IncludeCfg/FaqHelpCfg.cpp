#include "stdafx.h"
#include "FaqHelpCfg.h"
#include "xnExtstr.h"

CreateSingleton(FaqHelpCfg);

FaqHelpCfg::FaqHelpCfg()
{
	m_pList = xnList::Create();
}

FaqHelpCfg::~FaqHelpCfg()
{
	while(m_pList->Count)
		delete (FaqHelpObj*)m_pList->Delete(0);
	m_pList->Free();
}

void FaqHelpCfg::LoadFile(char* pFileName)
{
	xnExtstr* pStr = xnExtstr::Import(pFileName);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	while(pStr->SetSection(nIndex ++))
	{
		FaqHelpObj* pObj	= new FaqHelpObj();

		pObj->nFaqID			= atoi(pStr->sectionName);
		pObj->szName	= strdup(pStr->ReadString("name",""));
		pObj->nFaqCount	= atoi(pStr->ReadString("cnt","0"));

		char	szTmp[256];
		for (int i = 0; i < pObj->nFaqCount; i++)
		{
			FaqHelpMsg*	pFaqMsg	= new FaqHelpMsg();
			sprintf(szTmp,"faqname%d", i+1);
			pFaqMsg->szFaqName	= strdup(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"faqdesc%d", i+1);
			pFaqMsg->szFaqDesc	= strdup(pStr->ReadString(szTmp,"0"));
			pObj->pFaqHelpMsgList->Add((void*)pFaqMsg);
		}
		m_pList->Add(pObj);
	}
	pStr->Free();
}

FaqHelpObj::FaqHelpObj()
{
	pFaqHelpMsgList = xnList::Create();
}

FaqHelpObj::~FaqHelpObj()
{
	while(pFaqHelpMsgList->Count)
	{
		FaqHelpMsg* pFaqMsg	= (FaqHelpMsg*)pFaqHelpMsgList->Delete(0);
		if(pFaqMsg->szFaqName)
			free(pFaqMsg->szFaqName);
		if (pFaqMsg->szFaqDesc)
			free(pFaqMsg->szFaqDesc);
		delete pFaqMsg;
	}
	pFaqHelpMsgList->Free(); 
}
