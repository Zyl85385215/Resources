#include "ExpandBagConfig.h"
#include "xnExtstr.h"
#include "stdio.h"

CreateSingleton(ExpandBagConfig);
CreateSingleton(SpecialBagConfig);

ExpandBagConfig::ExpandBagConfig()
{
	m_pkBagConfigList = xnList::Create();
}

ExpandBagConfig::~ExpandBagConfig()
{
	while(m_pkBagConfigList->Count)
		delete (ExpandBagConfigElem*)m_pkBagConfigList->Delete(0);
	m_pkBagConfigList->Free();
}

bool ExpandBagConfig::loadExpandBagConfig(char* szFile)
{
	xnExtstr* pStr = xnExtstr::Import(szFile);
	if(pStr == NULL) return false; 
	int index = 0;

	while (pStr->SetSection(index) != NULL)
	{
		ExpandBagConfigElem* pData = new ExpandBagConfigElem;
		pData->byBagPage = atoi(pStr->ReadString("ExpandBagPage", "0"));
		pData->iNeedScore = atoi(pStr->ReadString("NeedScore", "0"));
		pData->iNeedMoney = atoi(pStr->ReadString("NeedMoney", "0"));
		pData->iUnbindCount = atoi(pStr->ReadString("UnBindBagPageCount", "0"));
	
		m_pkBagConfigList->Add(pData);
		index++;
	}

	pStr->Free();
	return true;
}

const ExpandBagConfigElem* ExpandBagConfig::getExpandBagConfigByPage(BYTE byBagPage)
{
	for (int i = 0; i < m_pkBagConfigList->Count; i++)
	{
		ExpandBagConfigElem* pElem	= (ExpandBagConfigElem*)m_pkBagConfigList->Items[i];

		if(pElem->byBagPage == byBagPage)
			return pElem;
	}
	return NULL;
}


SpecialBagConfig::SpecialBagConfig()
{
	m_pList		= xnList::Create();
}

SpecialBagConfig::~SpecialBagConfig()
{
	while(m_pList->Count)
	{
		delete (SPItemCfgObj*)m_pList->Delete(0);
	}
	m_pList->Free();
}

void SpecialBagConfig::LoadFile( char* szFile )
{
	xnExtstr* pStr = xnExtstr::Import(szFile);
	if(pStr == NULL) return; 

	m_pList->Clear();
	if(pStr->SetSection(0) != NULL)
	{
		char	szTmp[256];
		int nCnt	= atoi(pStr->ReadString("cnt", "0"));
		for(int i = 0; i < nCnt; i++)
		{
			SPItemCfgObj*	pObj	= new SPItemCfgObj;
			m_pList->Add(pObj);
			sprintf(szTmp,"item%d",i+1);
			pObj->dwItemID	= atoi(pStr->ReadString(szTmp, "0"));
			sprintf(szTmp,"desc%d",i+1);
			strncpy(pObj->szDesc,pStr->ReadString(szTmp,"δ֪"),255);
		}
	}

	pStr->Free();
}

bool SpecialBagConfig::CheckIsSPItem( DWORD dwItemID )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		SPItemCfgObj*	pObj	= (SPItemCfgObj*)m_pList->Items[i];
		if(pObj->dwItemID==dwItemID)
		{
			return true;
		}
	}
	return false;
}

char* SpecialBagConfig::GetSpItemDesc( DWORD dwItemID )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		SPItemCfgObj*	pObj	= (SPItemCfgObj*)m_pList->Items[i];
		if(pObj->dwItemID==dwItemID)
		{
			return pObj->szDesc;
		}
	}
	return NULL;
}

SPItemCfgObj* SpecialBagConfig::GetSPIObj( int nIndex )
{
	if(nIndex < 0 || nIndex >= m_pList->Count)
		return NULL;

	return (SPItemCfgObj*)m_pList->Items[nIndex];
}