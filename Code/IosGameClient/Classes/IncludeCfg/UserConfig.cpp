
#include "stdafx.h"
#ifdef _ISSERVER
#include "stdio.h"
#define SAFE_FREE(p) { if(p) { free(p); p=NULL; } }
#endif

#include "UserConfig.h"
#include "xnExtstr.h"
#ifdef _ISSERVER
#include "LuaPlus.h"
#endif

#include <string>
#include <list>
#include "ItemConfig.h"
CreateSingleton(ActiveConfig);
CreateSingleton(WarRankItemConfig);
CreateSingleton(UserConfig);

extern std::list<std::string> SpliceString(std::string szStr,std::string szSplice);
// {
// 	std::list<std::string> list;
// 	int nPos = szStr.find(szSplice);
// 	while (nPos != std::string::npos)
// 	{
// 		list.push_back(szStr.substr(0,nPos));
// 		szStr = szStr.substr(nPos+1,szStr.length());
// 		nPos = szStr.find(szSplice);
// 	}
// 	list.push_back(szStr);
// 	return list;
// }

UserConfig::UserConfig()
{
	memset(nParam,0,sizeof(int)*nUserMaxLv*nUserMaxJob*nUserMaxAtt);
	memset(nExp,0,sizeof(unsigned int)*nUserMaxLv);
}

UserConfig::~UserConfig()
{

}

void UserConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	char sztxt[32];
	if(pStr->SetSection("exp"))
	{
		for (int i=0;i<nUserMaxLv;i++)
		{
			sprintf(sztxt,"%d",i+1);
			char* szTemp = strdup(pStr->ReadString(sztxt, "0"));
			nExp[i] = strtoul (szTemp,&szTemp,0);

		}
	}
	for (int i=0;i<nUserMaxJob;i++)
	{
		if (pStr->SetSection(i+1)!=NULL)
		{
			for (int j=0;j<nUserMaxLv;j++)
			{
				sprintf(sztxt,"%d",j+1);
				std::list<std::string> list = SpliceString(pStr->ReadString (sztxt,"0"),"|");
				int n = 0;
				for (std::list<std::string>::iterator itor = list.begin();itor!=list.end()&&n<EQUIP_ATTR_MAX;itor++,n++)
				{
					nParam[j][i][n] = atoi (itor->c_str());
				}

			}
		}
	}
	pStr->Free();
}

void ActiveConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[128];

	while (pStr->SetSection(index++) != NULL)
	{
		ActiveItem*	pIB	= new ActiveItem();
		pIB->nType		= atoi (pStr->sectionName);
		pIB->nItem			= atoi (pStr->ReadString ("item","0"));
		pIB->nFlag			= atoi(pStr->ReadString ("flag","0"));
		m_mapData[pIB->nType] = pIB;
	}

	pStr->Free ();
}

ActiveItem* ActiveConfig::GetItem( int nID )
{
	map<int,ActiveItem*>::iterator itor = m_mapData.find(nID);
	if (itor != m_mapData.end())
	{
		return itor->second;
	}
	return NULL;
}

ActiveConfig::ActiveConfig()
{

}

ActiveConfig::~ActiveConfig()
{
	for (map<int,ActiveItem*>::iterator itor = m_mapData.begin();itor!= m_mapData.end();itor++)
	{
		delete itor->second;
	}
}

WarRankItemConfig::WarRankItemConfig()
{

}

WarRankItemConfig::~WarRankItemConfig()
{
	for (list<WarRankItem*>::iterator itor = m_wrList.begin();itor!=m_wrList.end();itor++)
	{
		delete (*itor);
	}
}

void WarRankItemConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[128];

	while (pStr->SetSection(index++) != NULL)
	{
		WarRankItem*	pIB	= new WarRankItem();
		pIB->nPos		= atoi (pStr->ReadString ("title","0"));
		pIB->nCnt		= atoi (pStr->ReadString ("count1","0"));
		pIB->nItem		= atoi (pStr->ReadString ("item1","0"));
		pIB->nMoney		= atoi (pStr->ReadString ("money","0"));
		m_wrList.push_back(pIB);
	}

	pStr->Free ();
}

void WarRankItemConfig::GetItem( int nPre,int nNow,list<WarRankItem*>& m_List )
{
	for (list<WarRankItem*>::iterator itor = m_wrList.begin();itor!=m_wrList.end();itor++)
	{
		if ((*itor)->nPos<nPre&&(*itor)->nPos>=nNow)
		{
			m_List.push_back(*itor);
		}
	}
}
