#include "MapCopyCondition.h"
#include "xnExtstr.h"

CreateSingleton(CMapCopyConditionConfig);

//////////////////////////////////////////////////////////////////////////

CMapCopyConditionConfig::CMapCopyConditionConfig( void )
{

}

CMapCopyConditionConfig::~CMapCopyConditionConfig( void )
{
	std::map<int, MapCopyConditionInfo*>::iterator iterator = m_mapConditionInfo.begin();
	while(iterator != m_mapConditionInfo.end())
	{
		MapCopyConditionInfo * pInfo = iterator->second;
		SAFE_DELETE(pInfo);
		iterator++;
	}
	m_mapConditionInfo.clear();
}

bool CMapCopyConditionConfig::load( const char * szFile )
{
	xnExtstr * pExtstr = xnExtstr::Import((char*)szFile);
	if(!pExtstr) return false;

	int nMapCopyCount = 0;

	if(pExtstr->SetSection("common"))
	{
		nMapCopyCount = atoi(pExtstr->ReadString("count", "0"));
	}

	for(int i=0; i<nMapCopyCount; i++)
	{
		char szKey[1024] = {0};
		sprintf(szKey, "%d", i);
		if(!pExtstr->SetSection(szKey)) continue;

		MapCopyConditionInfo * pInfo = new MapCopyConditionInfo;
		int nMapCopyId = atoi(pExtstr->ReadString("mapcopyid", "0"));
		pInfo->m_nMinLevel = atoi(pExtstr->ReadString("minlevel", "0"));
		pInfo->m_nMaxLevel = atoi(pExtstr->ReadString("maxlevel", "0"));
		pInfo->m_blInTeam = static_cast<bool>(atoi(pExtstr->ReadString("inteam", "0")));
		m_mapConditionInfo[nMapCopyId] = pInfo;
	}
	pExtstr->Free();
}

MapCopyConditionInfo * CMapCopyConditionConfig::getConditionInfo( int nMapCopyId )
{
	std::map<int, MapCopyConditionInfo*>::iterator iterator = m_mapConditionInfo.find(nMapCopyId);
	if(iterator == m_mapConditionInfo.end()) return 0;
	return iterator->second;
}