#include "AdvExpConfig.h"

CreateSingleton(AdvExpConfig);


void AdvExpConfig::LoadFile( char* szFile )
{
	xnExtstr * pExtstr = xnExtstr::Import(szFile);
	if(!pExtstr) return;

	int index=0;

	while(pExtstr->SetSection(index++))
	{
		BYTE bLv = atoi(pExtstr->sectionName);
		WORD wNum = atoi(pExtstr->ReadString("ball", "0"));
		mapLvBall[bLv]	= wNum;
	}
	pExtstr->Free();
}

WORD AdvExpConfig::GetNeedBall( BYTE bLv )
{
	std::map<BYTE,WORD>::iterator	itor	= mapLvBall.find(bLv);
	if(itor!= mapLvBall.end())
	{
		return itor->second;
	}
	return 0;
}