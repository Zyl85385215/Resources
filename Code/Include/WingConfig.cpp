#include "WingConfig.h"
#include "xnExtstr.h"
#include <iostream>

CreateSingleton(ItemWingConfig);
CreateSingleton(HorseELvConfig);
CreateSingleton(BombSkillConfig);

ItemWingConfig::ItemWingConfig()
{
	m_pList		= xnList::Create();
}

ItemWingConfig::~ItemWingConfig()
{
	while(m_pList->Count)
	{
		delete (WingStepObj*)m_pList->Delete(0);
	}
	m_pList->Free();
}

bool ItemWingConfig::LoadFile(char* szFile)
{
	xnExtstr* pStr = xnExtstr::Import(szFile);
	if(pStr == NULL) return false; 
	int index = 0;
	char szTmp[32];
	while (pStr->SetSection(index++) != NULL)
	{
		WingStepObj* pWSO = new WingStepObj;
		pWSO->bSkin = atoi(pStr->ReadString("skin", "50"));
		pWSO->bNeedLv = atoi(pStr->ReadString("needlv", "50"));
		pWSO->nPowMin = atoi(pStr->ReadString("powmin", "-2"));
		pWSO->nPowMax = atoi(pStr->ReadString("powmax", "10"));

		pWSO->nDuraMax = atoi(pStr->ReadString("dura", "50"));
		pWSO->nRepairTimeLimit = atoi(pStr->ReadString("duralimit", "50"));
		pWSO->nLvlNeed = atoi(pStr->ReadString("lvlupneed", "0"));

		pWSO->nQuaMax[0]	= atoi(pStr->ReadString("quality1", "500"));
		pWSO->nQuaMax[1]	= atoi(pStr->ReadString("quality2", "500"));
		pWSO->nQuaMax[2]	= atoi(pStr->ReadString("quality3", "500"));
		pWSO->nQuaMax[3]	= atoi(pStr->ReadString("quality4", "500"));
		pWSO->nQuaMax[4]	= atoi(pStr->ReadString("quality5", "500"));
	
		m_pList->Add(pWSO);
	}

	pStr->Free();
	return true;
}

WingStepObj* ItemWingConfig::GetStepInfo( BYTE bStep )
{
	if(bStep < m_pList->Count)
	{
		return (WingStepObj*)m_pList->Items[bStep];
	}
	return NULL;
}

HorseELvConfig::HorseELvConfig()
{
	m_pList		= xnList::Create();
	m_pList2		= xnList::Create();
	m_pList3		= xnList::Create();
	m_pList4		= xnList::Create();
}

HorseELvConfig::~HorseELvConfig()
{
	while(m_pList->Count)
	{
		delete (HorseELvCfgObj*)m_pList->Delete(0);
	}
	m_pList->Free();

	while(m_pList2->Count)
	{
		delete (HorseELv2CfgObj*)m_pList2->Delete(0);
	}
	m_pList2->Free();
	while(m_pList3->Count)
	{
		delete m_pList3->Delete(0);
	}
	m_pList3->Free();
	while(m_pList4->Count)
	{
		delete m_pList4->Delete(0);
	}
	m_pList4->Free();
}

bool HorseELvConfig::LoadFile( char* szFile )
{
	xnExtstr* pStr = xnExtstr::Import(szFile);
	if(pStr == NULL) return false; 
	int index = 0;
	char szTmp[32];
	while (pStr->SetSection(index++) != NULL)
	{
		HorseELvCfgObj* pHEI = new HorseELvCfgObj;

		pHEI->bEffPer		= atoi(pStr->ReadString("Eff", "0"));
		pHEI->wNeedPoint	= atoi(pStr->ReadString("point", "50"));
		pHEI->nNeedGold		= atoi(pStr->ReadString("gold", "-2"));

		m_pList->Add(pHEI);
	}
	pStr->Free();
	return true;
}

HorseELvCfgObj* HorseELvConfig::GetHorseELv( BYTE bLv )
{
	if(bLv-1 >= m_pList->Count||bLv<1)
		return NULL;
	return (HorseELvCfgObj*)m_pList->Items[bLv-1];
}

HorseELv2CfgObj* HorseELvConfig::GetHorseELv2( BYTE bLv )
{
	if(bLv >= m_pList2->Count)
		return NULL;
	return (HorseELv2CfgObj*)m_pList2->Items[bLv];
}

bool HorseELvConfig::LoadFile2( char* szFile )
{
	xnExtstr* pStr = xnExtstr::Import(szFile);
	if(pStr == NULL) return false; 
	int index = 0;
	char szTmp[32];
	while (pStr->SetSection(index++) != NULL)
	{
		HorseELv2CfgObj* pHEI = new HorseELv2CfgObj;

		pHEI->wAddPer		= atoi(pStr->ReadString("addper", "0"));
		pHEI->wNeedExp	= atoi(pStr->ReadString("needexp", "0"));
		pHEI->wWXAdd		= atoi(pStr->ReadString("addwx", "0"));

		m_pList2->Add(pHEI);
	}
	pStr->Free();
	return false;
}

bool HorseELvConfig::LoadFile3( char* szFile )
{
	xnExtstr* pStr = xnExtstr::Import(szFile);
	if(pStr == NULL) return false; 
	int index = 0;
	char szTmp[32];
	while (pStr->SetSection(index++) != NULL)
	{
		HorseSkinCfgObj* pHEI = new HorseSkinCfgObj;

		pHEI->dwHorseID		= atoi(pStr->sectionName);
		for (int i=0;i<5;i++)
		{
			sprintf(szTmp,"skin%d",i+1);
			pHEI->Skin[i]	= atoi(pStr->ReadString(szTmp, "0"));
			sprintf(szTmp,"state%d",i+1);
			pHEI->state[i]	= atoi(pStr->ReadString(szTmp, "0"));
		}
		pHEI->dwSpeed = atoi(pStr->ReadString("speed", "0"));
		

		m_pList3->Add(pHEI);
	}
	pStr->Free();
	return false;
}

DWORD HorseELvConfig::GetHorseSkin( BYTE bLv,DWORD HorseID )
{
	int templv = bLv/9;
	if (templv<0||templv>5)
	{
		return 0;
	}
	for (int i=0;i<m_pList3->Count;i++)
	{
		HorseSkinCfgObj* pinfo = (HorseSkinCfgObj*)m_pList3->Items[i];
		if (!pinfo)
		{
			return 0;
		}
		if (pinfo->dwHorseID == HorseID)
		{
			return pinfo->Skin[templv];
		}
	}
	return 0;
}

DWORD HorseELvConfig::GetHorseState( BYTE bLv,DWORD HorseID )
{
	int templv = bLv/9;
	if (templv<0||templv>5)
	{
		return 0;
	}
	for (int i=0;i<m_pList3->Count;i++)
	{
		HorseSkinCfgObj* pinfo = (HorseSkinCfgObj*)m_pList3->Items[i];
		if (!pinfo)
		{
			return 0;
		}
		if (pinfo->dwHorseID == HorseID)
		{
			return pinfo->state[templv];
		}
	}
	return 0;
}

bool HorseELvConfig::LoadFile4( char* szFile )
{
	xnExtstr* pStr = xnExtstr::Import(szFile);
	if(pStr == NULL) return false; 
	int index = 0;
	char szTmp[32];
	while (pStr->SetSection(index++) != NULL)
	{
		HorseWXLvUpCfgObj* pHEI = new HorseWXLvUpCfgObj;

		pHEI->costmoney		= atoi(pStr->ReadString("cost", "0"));
		pHEI->eff = atoi(pStr->ReadString("addeffect", "0"));
		pHEI->fallback = atoi(pStr->ReadString("failback", "0"));
		pHEI->successper = atoi(pStr->ReadString("successper", "0"));

		m_pList4->Add(pHEI);
	}
	pStr->Free();
	return false;
}

HorseWXLvUpCfgObj* HorseELvConfig::GetHorseWXlvUpCfg( BYTE BLv )
{

	if(BLv >= m_pList4->Count||BLv<0)
		return NULL;
	return (HorseWXLvUpCfgObj*)m_pList4->Items[BLv];
	//return NULL;
}

DWORD HorseELvConfig::GetHorseSpeed( DWORD HorseID )
{
	for (int i=0;i<m_pList3->Count;i++)
	{
		HorseSkinCfgObj* pinfo = (HorseSkinCfgObj*)m_pList3->Items[i];
		if (!pinfo)
		{
			return 0;
		}
		if (pinfo->dwHorseID == HorseID)
		{
			return pinfo->dwSpeed;
		}
	}
	return 0;
}
DWORD BombSkillConfig::GetSkinID( BYTE bLv,DWORD index,BYTE type )
{
	if (index>m_pList->Count)
	{
		return 0;
	}
	if (type!=1&&type!=2)
	{
		return 0;
	}
	int skilllv = 0;
/*
	if (type == 1)
	{*/

	skilllv = bLv/10;
	skilllv = skilllv>10?9:skilllv;
/*
	}else if (type == 2)
	{
		skilllv = bLv/10;

	}*/
	
	for (int i=0;i<m_pList->Count;i++)
	{
		HorseBombSKillCfgObj * pinfo = (HorseBombSKillCfgObj *)m_pList->Items[i];
		if (pinfo&&pinfo->index == index)
		{
			if (type == 1)
			{
				return pinfo->skill1[skilllv];
			}else if (type == 2)
			{
				return pinfo->skill2[skilllv];
			}
		}
	}
	return 0;
}

BombSkillConfig::BombSkillConfig()
{
	m_pList = xnList::Create();
}

BombSkillConfig::~BombSkillConfig()
{
	while (m_pList->Count)
	{
		delete m_pList->Delete(0);
	}
}

bool BombSkillConfig::LoadFile( char* szFile )
{
	xnExtstr* pStr = xnExtstr::Import(szFile);
	if(pStr == NULL) return false; 
	int index = 0;
	char szTmp[32];
	while (pStr->SetSection(index++) != NULL)
	{
		HorseBombSKillCfgObj* pHEI = new HorseBombSKillCfgObj;
		pHEI->index = atoi(pStr->sectionName);
		int skillid1 = atoi(pStr->ReadString("skill1", "0"));
		int skillid2 = atoi(pStr->ReadString("skill2", "0"));
		for (int i=0;i<10;i++)
		{
			pHEI->skill1[i] = skillid1+i;
			pHEI->skill2[i] = skillid2+i;
		}
		pHEI->dwButtle1 = atoi(pStr->ReadString("bullet1", "0"));
		pHEI->dwButtle2 = atoi(pStr->ReadString("bullet2", "0"));
		pHEI->dwButtleIco = atoi(pStr->ReadString("bulletico", "0"));
		m_pList->Add(pHEI);
	}
	pStr->Free();
	return true;
}

DWORD BombSkillConfig::GetSkinButtet( DWORD index,BYTE type )
{
	if (index>m_pList->Count)
	{
		return 0;
	}
	if (type!=1&&type!=2)
	{
		return 0;
	}


	for (int i=0;i<m_pList->Count;i++)
	{
		HorseBombSKillCfgObj * pinfo = (HorseBombSKillCfgObj *)m_pList->Items[i];
		if (pinfo&&pinfo->index == index)
		{
			if (type == 1)
			{
				return pinfo->dwButtle1;
			}else if (type == 2)
			{
				return pinfo->dwButtle2;
			}
		}
	}
	return 0;
}

DWORD BombSkillConfig::GetBulletIco( DWORD index )
{

	if (index>m_pList->Count)
	{
		return 0;
	}


	for (int i=0;i<m_pList->Count;i++)
	{
		HorseBombSKillCfgObj * pinfo = (HorseBombSKillCfgObj *)m_pList->Items[i];
		if (pinfo&&pinfo->index == index)
		{
			return pinfo->dwButtleIco;
		}
	}
	return 0;
}