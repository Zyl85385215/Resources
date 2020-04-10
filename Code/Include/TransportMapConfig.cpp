#include "TransportMapConfig.h"
#include "xnExtstr.h"
#include "stdio.h"

CreateSingleton(TransportMapConfig);
CreateSingleton(HomeStatesConfig);

TransportMapConfig::TransportMapConfig()
{
	m_pList = xnList::Create();
}

TransportMapConfig::~TransportMapConfig()
{
	int nCnt = m_pList->Count;
	for(int i=0; i<nCnt; i++)
	{
		TransportMapData * pConfig = (TransportMapData*)m_pList->Items[i];
		SAFE_DELETE(pConfig);
	}
	m_pList->Free();
}

bool TransportMapConfig::Load(const char * filename)
{
	xnExtstr * pIni = xnExtstr::Import((char*)filename);
	if(pIni == NULL) return false;

	int nSectionCount = pIni->sectionCount;
	for(int i=0; i<nSectionCount; i++)
	{
		pIni->SetSection(i);

		TransportMapData * pConfig = new TransportMapData;
		pConfig->m_wMapIndex		= atoi(pIni->sectionName);
		pConfig->m_bFlagCount		= atoi(pIni->ReadString("flag", "1"));
		pConfig->m_nMoney			= atoi(pIni->ReadString("money","0"));

		m_pList->Add(pConfig);
	}
	pIni->Free();
	return false;
}

int TransportMapConfig::GetFlagCost( int nMapIndex )
{
	for (int i=0;i<m_pList->Count;i++)
	{
		TransportMapData * pData = (TransportMapData *)m_pList->Items[i];
		if (pData->m_wMapIndex == nMapIndex)
		{
			return pData->m_bFlagCount;
		}
	}
	return 1;
}

int TransportMapConfig::GetMoneyCost( int nMapIndex )
{
	for (int i=0;i<m_pList->Count;i++)
	{
		TransportMapData * pData = (TransportMapData *)m_pList->Items[i];
		if (pData->m_wMapIndex == nMapIndex)
		{
			return pData->m_nMoney;
		}
	}
	return 0;
}

HomeStatesConfig::HomeStatesConfig()
{
	m_pList		= xnList::Create();
}

HomeStatesConfig::~HomeStatesConfig()
{
	if (m_pList)
	{
		while(m_pList->Count)
		{
			delete (HSCfgObj*)m_pList->Delete(0);
		}
		m_pList->Free();
	}
}

void HomeStatesConfig::Load( const char * filename )
{
	xnExtstr * pIni = xnExtstr::Import((char*)filename);
	if(pIni == NULL) return;

	char	szTmp[32];
	int nIndex	= 0;
	while(pIni->SetSection(nIndex))
	{
		int	nCnt	= atoi(pIni->ReadString("cnt", "0"));

		for (int i = 0; i < nCnt; i++)
		{
			HSCfgObj * pHs = new HSCfgObj;
			pHs->bType	= nIndex;
			pHs->bLevel	= i+1;
			sprintf(szTmp,"skill%d",i+1);
			pHs->wSkill	= atoi(pIni->ReadString(szTmp, "0"));
			sprintf(szTmp,"state%d",i+1);
			pHs->wState	= atoi(pIni->ReadString(szTmp, "0"));
			sprintf(szTmp,"costhonor%d",i+1);
			pHs->nNeedHonor	= atoi(pIni->ReadString(szTmp,"0"));
			sprintf(szTmp,"costmoney%d",i+1);
			pHs->nNeedMoney	= atoi(pIni->ReadString(szTmp,"0"));
			sprintf(szTmp,"costexp%d",i+1);
			pHs->nNeedExp	= atoi(pIni->ReadString(szTmp,"0"));
			m_pList->Add(pHs);
		}
		
		nIndex++;
	}
	pIni->Free();
}

HSCfgObj* HomeStatesConfig::GetHSInfo( BYTE bType, BYTE bLv )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		HSCfgObj* pHS	= (HSCfgObj*)m_pList->Items[i];
		if(pHS->bType == bType && pHS->bLevel == bLv)
		{
			return pHS;
		}
	}
	return NULL;
}