#include "MapPVEConfig.h"
#include "xnExtstr.h"

CreateSingleton(CMapPVEConfig);

bool CMapPVEConfig::Load( const char * filename )
{
	xnExtstr * pIni = xnExtstr::Import((char*)filename);
	if(pIni == NULL) return false;

	int nSectionCount = pIni->sectionCount;
	for(int i=0; i<nSectionCount; i++)
	{
		pIni->SetSection(i);

		MapPVEConfigData * pData = new MapPVEConfigData;
		pData->nMapIndex		= atoi(pIni->sectionName);
		pData->bIsPVE = atoi(pIni->ReadString("PVE","0"));

		m_pList->Add(pData);
	}
	pIni->Free();
	return false;
}

CMapPVEConfig::CMapPVEConfig( void )
{
	m_pList = xnList::Create();
}

CMapPVEConfig::~CMapPVEConfig( void )
{
	int nCnt = m_pList->Count;
	for(int i=0; i<nCnt; i++)
	{
		MapPVEConfigData * pConfig = (MapPVEConfigData*)m_pList->Items[i];
		SAFE_DELETE(pConfig);
	}
	m_pList->Free();
}

BYTE CMapPVEConfig::GetIsPVE( int nMapIndex )
{
	for (int i=0;i<m_pList->Count;i++)
	{
		MapPVEConfigData * pData = (MapPVEConfigData *)m_pList->Items[i];
		if (pData->nMapIndex == nMapIndex)
		{
			return pData->bIsPVE;
		}
	}
	return 1;
}