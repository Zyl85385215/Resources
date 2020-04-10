#include "CfgObject.h"
#include <Windows.h>
#include "xnExtstr.h"

//CreateSingleton(CCfgObject);

CCfgObject::CCfgObject( char* szFile )
{
	char szString[128] = {0};
	char*	szKey = NULL;

	xnExtstr * pExtstr = xnExtstr::Import(szFile);
	if(!pExtstr) return;

	for (int i = 0; i < pExtstr->sectionCount; i++)
	{
		if(!pExtstr->SetSection(i)) break;
		for (int j = 0; j < pExtstr->keyCount; j++)
		{
			szKey = pExtstr->GetKey(j,NULL);
			if(!szKey)	break;
			sprintf(szString,"%s_%s",pExtstr->sectionName,szKey);
			m_mapCfg.insert(std::make_pair(strdup(szString),strdup(pExtstr->ReadString(j))));
		}
	}
	pExtstr->Free();
}

CCfgObject::~CCfgObject()
{
	map_CfgObjs::iterator	itor = m_mapCfg.begin();
	while(itor != m_mapCfg.end())
	{
		free(itor->first);
		free(itor->second);
		m_mapCfg.erase(itor++);
	}
}

int CCfgObject::GetInt( char* szSection, char* szKey ,int nDef/*=0*/)
{
	char szTmp[128];
	sprintf(szTmp,"%s_%s",szSection,szKey);
	map_CfgObjs::iterator	itor = m_mapCfg.find(szTmp);
	if(itor != m_mapCfg.end())
	{
		return atoi(itor->second);
	}
	return nDef;
}

char* CCfgObject::GetString( char* szSection, char* szKey )
{
	char szTmp[128];
	sprintf(szTmp,"%s_%s",szSection,szKey);
	map_CfgObjs::iterator	itor = m_mapCfg.find(szTmp);
	if(itor != m_mapCfg.end())
	{
		return itor->second;
	}
	return NULL;
}