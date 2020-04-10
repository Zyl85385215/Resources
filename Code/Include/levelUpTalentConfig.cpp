#include "levelUpTalentConfig.h"
#include "xnExtstr.h"

CreateSingleton(CLevelUpTalentConfig);

CLevelUpTalentConfig::CLevelUpTalentConfig( void )
{
	m_pList = xnList::Create();
}

CLevelUpTalentConfig::~CLevelUpTalentConfig( void )
{
	int nCnt = m_pList->Count;
	for(int i=0; i<nCnt; i++)
	{
		LevelUpTalentData * pConfig = (LevelUpTalentData*)m_pList->Items[i];
		SAFE_DELETE(pConfig);
	}
	m_pList->Free();
}

bool CLevelUpTalentConfig::Load( const char * filename )
{
	xnExtstr * pIni = xnExtstr::Import((char*)filename);
	if(pIni == NULL) return false;

	int nSectionCount = pIni->sectionCount;
	for(int i=0; i<nSectionCount; i++)
	{
		pIni->SetSection(i);
		int nCount = pIni->keyCount;
		for (int j = 0;j<nCount;j++)
		{
			LevelUpTalentData * pConfig = new LevelUpTalentData;
			pConfig->nLevel = atoi(pIni->GetKey(j,"0"));
			pConfig->nTalent = atoi(pIni->ReadString(j,"0"));
			m_pList->Add(pConfig);
		}
	}
	pIni->Free();
	return false;
}

int CLevelUpTalentConfig::GetTalent(int nLevel)
{
	for (int i=0;i<m_pList->Count;i++)
	{
		LevelUpTalentData * pData = (LevelUpTalentData*)m_pList->Items[i];
		if (pData&&pData->nLevel == nLevel)
			return pData->nTalent;
	}
	return 0;
}

int CLevelUpTalentConfig::GetNextLevel(int nLevel)
{
	for (int i=0;i<m_pList->Count;i++)
	{
		LevelUpTalentData * pData = (LevelUpTalentData*)m_pList->Items[i];
		if (pData&&pData->nLevel>nLevel&&pData->nTalent>0)
		{
			return pData->nLevel;
		}
	}
	return 0;
}

int CLevelUpTalentConfig::GetAllTalentOnLevel(int nLevel)
{
	int nTalent = 0;
	for (int i=0;i<m_pList->Count;i++)
	{
		LevelUpTalentData * pData = (LevelUpTalentData*)m_pList->Items[i];
		if (pData&&pData->nLevel<=nLevel)
		{
			nTalent +=pData->nTalent;
		}
		else
			break;
	}
	return nTalent;
}