#include "equipstarupconfig.h"
#include "xnExtstr.h"

CreateSingleton(CEquipStarUpConfig);



CEquipStarUpConfig::CEquipStarUpConfig(void)
{
}

CEquipStarUpConfig::~CEquipStarUpConfig(void)
{
}


bool CEquipStarUpConfig::Load( const char * szFile )
{
	xnExtstr * pExtstr = xnExtstr::Import((char*)szFile);
	if(!pExtstr) return false;


	pExtstr->SetSection("Base");
	nMustSucCount =  atoi(pExtstr->ReadString("MustSucCount","2"));
	nMustFailCount = atoi(pExtstr->ReadString("MustFailCount","6"));
	int item = 0;
	char szSection[10];
	sprintf(szSection,"%d",item);
	while(pExtstr->SetSection(szSection))
	{
		STAR_UP_CONFIG config;
		
		//4个升星基数等级
		for (int i = 1;i <= 4;i++)
		{
			char sz[100] = {0};
			sprintf(sz,"BasePercent%d",i);
			char * szBasePercent = pExtstr->ReadString(sz);
			config.BasePercenttable[i] = atoi(szBasePercent);
		}
		///////////////////////////////
		char * szDecLevelFailed = pExtstr->ReadString("DecLevelFailed");
		config.nDecLevelFailed = atoi(szDecLevelFailed);
		//////////////////////////////		
		char * szNeedGold = pExtstr->ReadString("NeedGold");
		config.nNeedGold = atoi(szNeedGold);
		///////////////////////////////
		char * szAtrribUpPercent = pExtstr->ReadString("AtrribUpPercent");
		config.nAtrribUpPercen = atoi(szAtrribUpPercent);

		config.nExHpVal = atoi(pExtstr->ReadString("ExHpVal","0"));
		config.dwSpeedItem	= atoi(pExtstr->ReadString("spclitem","0"));
		config.dwSpeedItemEx	= atoi(pExtstr->ReadString("spclitemex","0"));
		starUpCfg_[item] = config;
		item++;
		sprintf(szSection,"%d",item);
	}
	pExtstr->Free();
	return true;
}

STAR_UP_CONFIG* CEquipStarUpConfig::GetStarUpCfg( int nLevel )
{
	map<int,STAR_UP_CONFIG>::iterator itor = starUpCfg_.find(nLevel);
	if(itor != starUpCfg_.end())
	{
		return &itor->second;
	}
	return NULL;
}

int CEquipStarUpConfig::GetStarUpPercent( STAR_UP_CONFIG* pStarUp,int nNum )
{
	map<int,int>::iterator	itor	= pStarUp->BasePercenttable.find(nNum);
	if(itor != pStarUp->BasePercenttable.end())
	{
		return itor->second;
	}
	return 0;
}

int CEquipStarUpConfig::GetStarUpSpclItem( int dwItemID )
{
	map<int,STAR_UP_CONFIG>::iterator itor = starUpCfg_.begin();
	while(itor != starUpCfg_.end())
	{
		if(itor->second.dwSpeedItem	== dwItemID)
			return itor->first;
		if(itor->second.dwSpeedItemEx	== dwItemID)
			return itor->first;
		itor++;
	}
	return 0;
}