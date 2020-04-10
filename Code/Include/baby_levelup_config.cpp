#include "baby_levelup_config.h"
#include "xnExtstr.h"

CreateSingleton(CBabyLevelupConfigList);

CBabyLevelupConfigList::CBabyLevelupConfigList()
{
	m_pList = xnList::Create();
}

CBabyLevelupConfigList::~CBabyLevelupConfigList()
{
	int nCnt = m_pList->Count;
	for(int i=0; i<nCnt; i++)
	{
		BabyLevelupConfig * pConfig = (BabyLevelupConfig*)m_pList->Items[i];
		SAFE_DELETE(pConfig);
	}
	m_pList->Free();
}

bool CBabyLevelupConfigList::Load(const char * filename)
{
	xnExtstr * pIni = xnExtstr::Import((char*)filename);
	if(pIni == NULL) return false;

	int nSectionCount = pIni->sectionCount;
	for(int i=0; i<nSectionCount; i++)
	{
		pIni->SetSection(i);

		BabyLevelupConfig * pConfig = new BabyLevelupConfig;
		pConfig->m_wSlaveIndex		= atoi(pIni->sectionName);
		pConfig->m_bLevel			= atoi(pIni->ReadString("level", "0"));
		pConfig->m_wNewSlaveIndex	= atoi(pIni->ReadString("newslaveindex", "0"));

		m_pList->Add(pConfig);
	}
	pIni->Free();
	return false;
}

bool CBabyLevelupConfigList::IsSpecialSlave(SlaveDataEx & xData)
{
	WORD wSlaveIndex = xData.m_wSlaveIndex;

	int nCnt = m_pList->Count;
	for(int i=0; i<nCnt; i++)
	{
		BabyLevelupConfig * pConfig = (BabyLevelupConfig*)m_pList->Items[i];
		if(wSlaveIndex == pConfig->m_wSlaveIndex)
		{
			return true;
		}
	}
	return false;
}

WORD CBabyLevelupConfigList::CheckCanLevelup(SlaveDataEx & xData)
{
	WORD wSlaveIndex = xData.m_wSlaveIndex;
	BYTE bSlaveLevel = xData.m_wLevel;

	int nCnt = m_pList->Count;
	for(int i=0; i<nCnt; i++)
	{
		BabyLevelupConfig * pConfig = (BabyLevelupConfig*)m_pList->Items[i];
		if(wSlaveIndex == pConfig->m_wSlaveIndex)
		{
			if(bSlaveLevel >= pConfig->m_bLevel) 
			{
				return pConfig->m_wNewSlaveIndex;
			}
			return 0;
		}
	}
	return 0;
}