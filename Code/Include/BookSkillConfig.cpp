#include "BookSkillConfig.h"
#include "xnExtstr.h"
#include "stdio.h"
#include "misc.h"
#include "debug_def.h"

CreateSingleton(BSkillConfig);
CreateSingleton(BSkillCompState);
CreateSingleton(BSkillLvLimitCT);
CreateSingleton(WorldMsgConfig);
CreateSingleton(SPDriveStateCfg);
CreateSingleton(SwordSpriteConfig);

BSkillConfig::BSkillConfig( )
{
	m_pList		= xnList::Create();
	m_pListSt	= xnList::Create();
}


BSkillConfig::~BSkillConfig()
{
	while(m_pList->Count)
		delete (BSkillCfgObj*)m_pList->Delete(0);

	m_pList->Free();
	while(m_pListSt->Count)
		delete (BSkillCfgObj*)m_pListSt->Delete(0);

	m_pListSt->Free();
}

void BSkillConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;
	int iIndex = 0;
	char szTmp[32];

	while(pStr->SetSection(iIndex++))
	{
		BSkillCfgObj*	pInfo	= new BSkillCfgObj();
		m_pList->Add(pInfo);
		pInfo->wSkill	= atoi(pStr->sectionName);
		pInfo->bStep	= atoi(pStr->ReadString("step","0"));
		pInfo->bNeedJob	= atoi(pStr->ReadString("job","0"));	//2,4,8,16
		strcpy(pInfo->szTxt,pStr->ReadString("txt",""),512);
		pInfo->bUseTime = atoi(pStr->ReadString("USETIME","0"));
		pInfo->dwNeedSkill = atoi(pStr->ReadString("NEEDSKILL","0"));
		int skillnCnt	= atoi(pStr->ReadString("SKILLCOUNT","0"));
		for (int i = 0; i < skillnCnt; i++)
		{
			sprintf(szTmp,"NEXTSKILL%d",i+1);
			pInfo->pNextSkill->Add((void*)atoi(pStr->ReadString(szTmp,"0")));
		}
		int nCnt	= atoi(pStr->ReadString("CNT","0"));
		for (int i = 0; i < nCnt; i++)
		{
			sprintf(szTmp,"item%d",i+1);
			pInfo->pListItem->Add((void*)atoi(pStr->ReadString(szTmp,"0")));
		}
	}

	pStr->Free();
}

BSkillCfgObj* BSkillConfig::GetBookSkill( WORD wSID )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		BSkillCfgObj*	pBS	= (BSkillCfgObj*)m_pList->Items[i];
		if(pBS->wSkill	== wSID)
			return pBS;
	}
	return NULL;
}

BSkillCfgObj* BSkillConfig::GetBSkillByIndex( int nIndex , BYTE bJob)
{
	if(nIndex < 0 || nIndex >= m_pList->Count)
		return NULL;

	if(bJob == 0)
		return (BSkillCfgObj*)m_pList->Items[nIndex];
	else
	{
		for (int i = 0; i < m_pList->Count; i++)
		{
			BSkillCfgObj* pBS	= (BSkillCfgObj*)m_pList->Items[i];
			if(pBS->bNeedJob == 0 ||((pBS->bNeedJob >> bJob) & 1))
			{
				if(nIndex == 0)
					return pBS;
				else
					nIndex--;
			}
		}
	}
	return NULL;
}

void BSkillConfig::LoadSTFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;
	int iIndex = 0;
	char szTmp[32];

	while(pStr->SetSection(iIndex++))
	{
		BSkillCfgObj*	pInfo	= new BSkillCfgObj();
		m_pListSt->Add(pInfo);
		pInfo->wSkill	= atoi(pStr->sectionName);
		pInfo->bStep	= atoi(pStr->ReadString("step","0"));
		pInfo->bNeedJob	= atoi(pStr->ReadString("job","0"));	//2,4,8,16
		strcpy(pInfo->szTxt,pStr->ReadString("txt",""),512);

		int nCnt	= atoi(pStr->ReadString("CNT","0"));
		for (int i = 0; i < nCnt; i++)
		{
			sprintf(szTmp,"item%d",i+1);
			pInfo->pListItem->Add((void*)atoi(pStr->ReadString(szTmp,"0")));
		}
	}

	pStr->Free();
}

BSkillCfgObj* BSkillConfig::GetStFSkill( WORD wSID )
{
	for (int i = 0; i < m_pListSt->Count; i++)
	{
		BSkillCfgObj*	pBS	= (BSkillCfgObj*)m_pListSt->Items[i];
		if(pBS->wSkill	== wSID)
			return pBS;
	}
	return NULL;
}

BSkillCfgObj* BSkillConfig::GetStFSkillByIndex( int nIndex, BYTE bJob )
{
	if(nIndex < 0 || nIndex >= m_pListSt->Count)
		return NULL;

	if(bJob == 0)
		return (BSkillCfgObj*)m_pListSt->Items[nIndex];
	else
	{
		for (int i = 0; i < m_pListSt->Count; i++)
		{
			BSkillCfgObj* pBS	= (BSkillCfgObj*)m_pListSt->Items[i];
			if(pBS->bNeedJob == 0 ||((pBS->bNeedJob >> bJob) & 1))
			{
				if(nIndex == 0)
					return pBS;
				else
					nIndex--;
			}
		}
	}
	return NULL;
}

void BSkillCompState::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;
	char szTmp[32];

	for (int i = 0; i < 8; i++)
	{
		if(pStr->SetSection(i))
		{
			//cBSDriveCfgs[i].wState	= atoi(pStr->ReadString("state","0"));
			cBSDriveCfgs[i].wSkill	= atoi(pStr->ReadString("skill","0"));
			cBSDriveCfgs[i].bStart	= atoi(pStr->ReadString("startlv","1"));
			for (int j = 0; j < 10; j++)
			{
				sprintf(szTmp,"state%d",j+1);
				cBSDriveCfgs[i].wCoreState[j]	= atoi(pStr->ReadString(szTmp,"0"));
				sprintf(szTmp,"startlv%d",j+1);
				cBSDriveCfgs[i].bStartLv[j]	= atoi(pStr->ReadString(szTmp,"1"));
//#ifndef _ISSERVER
				sprintf(szTmp,"core%d",j+1);
				strcpy(cBSDriveCfgs[i].szCore[j],pStr->ReadString(szTmp,""));
				sprintf(szTmp,"pos%d",j+1);
				int x,y;
				sscanf(pStr->ReadString(szTmp,"0,0"),"%d,%d",&x,&y);//cBSDriveCfgs[i].ptPos[j].x,cBSDriveCfgs[i].ptPos[j].y);
				cBSDriveCfgs[i].ptPos[j].x	= x;
				cBSDriveCfgs[i].ptPos[j].y	= y;
				sprintf(szTmp,"spr%d",j+1);
				strcpy(cBSDriveCfgs[i].szCoreSpr[j],pStr->ReadString(szTmp,""));

				sprintf(szTmp,"statetxt%d",j+1);
				strcpy(cBSDriveCfgs[i].szStateTxt[j],pStr->ReadString(szTmp,"经脉效果未知"));
//#endif
			}

//#ifndef _ISSERVER
			strcpy(cBSDriveCfgs[i].szName,pStr->sectionName);
			strcpy(cBSDriveCfgs[i].szSpr,pStr->ReadString("spr",""));
//#endif
		}
	}

	pStr->Free();
}

BSkillCompState::BSkillCompState()
{
	memset(cBSDriveCfgs,0,sizeof(BSkillDriveCfg)*8);
}

BSkillLvLimitCT::BSkillLvLimitCT()
{
	m_pListLimit	= xnList::Create();
}

BSkillLvLimitCT::~BSkillLvLimitCT()
{
	while(m_pListLimit->Count)
	{
		delete (BSkillLimit*)m_pListLimit->Delete(0);
	}
	m_pListLimit->Free();
}

void BSkillLvLimitCT::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;
	char szTmp[32];

	int index=	0;
	while(pStr->SetSection(index++))
	{
		int		nTempID	= atoi(pStr->sectionName);
		BSkillLimit*	pBSL	= new BSkillLimit();
		m_pListLimit->Add(pBSL);
		pBSL->wDriveCore	= nTempID%1000;
		pBSL->bLevel		= nTempID/1000;
		int		nCnt	= atoi(pStr->ReadString("cnt","0"));
		for (int i = 0; i < nCnt; i++)
		{
			sprintf(szTmp,"dc%d",i+1);
			int	nDC	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"l%d",i+1);
			int	nLv	= atoi(pStr->ReadString(szTmp,"0"));
			if(nDC >= 1000)
				continue;

			pBSL->pListLimit->Add((void*)(nLv*1000+nDC));
		}
	}

	pStr->Free();
}

xnList* BSkillLvLimitCT::GetCoreLimit( BYTE bDrive, BYTE bCore, BYTE bLv )
{
	for (int i = 0; i < m_pListLimit->Count; i++)
	{
		BSkillLimit*	pBSL	= (BSkillLimit*)m_pListLimit->Items[i];
		if(pBSL->bLevel == bLv && pBSL->wDriveCore/100 == bDrive+1 && pBSL->wDriveCore%100 == bCore+1)
		{
			return pBSL->pListLimit;
		}
	}
	return NULL;
}

WorldMsgConfig::WorldMsgConfig()
{

}

WorldMsgConfig::~WorldMsgConfig()
{
	for (int i=0;i<count;i++)
	{
		delete szMsgtxt[count];
	}
	delete []szMsgtxt;
}

void WorldMsgConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;

	count = pStr->sectionCount;
	szMsgtxt = new char*[count];
	int index = 0;
	while(pStr->SetSection(index++))
	{
		szMsgtxt[index-1] = strdup(pStr->ReadString("msg",""));
	}

	pStr->Free();
}

char * WorldMsgConfig::GetString( int pos )
{
	if (pos<0||pos>=count)
	{
		return NULL;
	}
	return szMsgtxt[pos];
}

SPDriveStateCfg::SPDriveStateCfg()
{
	m_pListSPDrvState	= xnList::Create();
}

SPDriveStateCfg::~SPDriveStateCfg()
{
	while(m_pListSPDrvState->Count)
	{
		delete (SPDrvInfo*)m_pListSPDrvState->Delete(0);
	}
	m_pListSPDrvState->Free();
}

void SPDriveStateCfg::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;
	char szTmp[32];

	int index=	0;
	while(pStr->SetSection(index++))
	{
		if(strstr(pStr->sectionName,"lv") == 0)
		{
			SPDrvInfo*	pSPDI	= new SPDrvInfo;
			pSPDI->nID	= atoi(pStr->sectionName);
			pSPDI->wState	= atoi(pStr->ReadString("state","0"));
			pSPDI->byDrv1	= atoi(pStr->ReadString("drive1","0"));
			pSPDI->byDrv2	= atoi(pStr->ReadString("drive2","0"));
			pSPDI->byDrv3	= atoi(pStr->ReadString("drive3","0"));
			pSPDI->nEffType1= atoi(pStr->ReadString("arr1","0"));
			pSPDI->nEffType2= atoi(pStr->ReadString("arr2","0"));
			pSPDI->nEff11	= atoi(pStr->ReadString("eff11","0"));
			pSPDI->nEff12	= atoi(pStr->ReadString("eff12","0"));
			pSPDI->nEff13	= atoi(pStr->ReadString("eff13","0"));
			pSPDI->nEff21	= atoi(pStr->ReadString("eff21","0"));
			pSPDI->nEff22	= atoi(pStr->ReadString("eff22","0"));
			pSPDI->nEff23	= atoi(pStr->ReadString("eff23","0"));
			pSPDI->dwItem[0]	= atoi(pStr->ReadString("item1","0"));
			pSPDI->dwItem[1]	= atoi(pStr->ReadString("item2","0"));
			pSPDI->dwItem[2]	= atoi(pStr->ReadString("item3","0"));
			pSPDI->dwItem[3]	= atoi(pStr->ReadString("item4","0"));
			pSPDI->dwItem[4]	= atoi(pStr->ReadString("item5","0"));
			pSPDI->dwItem[5]	= atoi(pStr->ReadString("item6","0"));
			pSPDI->dwItem[6]	= atoi(pStr->ReadString("item7","0"));
			pSPDI->dwItem[7]	= atoi(pStr->ReadString("item8","0"));
			pSPDI->dwItem[8]	= atoi(pStr->ReadString("item9","0"));
			pSPDI->dwItem[9]	= atoi(pStr->ReadString("item10","0"));
			strcpy(pSPDI->szName,pStr->ReadString("name","NULL"));

			m_pListSPDrvState->Add(pSPDI);
		}
		else
		{
			int nLv	= atoi(pStr->sectionName+2)-1;
			if(nLv >= 0 && nLv < 10)
			{
				spDrvLv[nLv].byDrvLv1	= atoi(pStr->ReadString("drive1lv","0"));
				spDrvLv[nLv].byDrvLv2	= atoi(pStr->ReadString("drive2lv","0"));
				spDrvLv[nLv].byDrvLv3	= atoi(pStr->ReadString("drive3lv","0"));
				spDrvLv[nLv].nExp		= atoi(pStr->ReadString("exp","0"));
				spDrvLv[nLv].nGold		= atoi(pStr->ReadString("gold","0"));
			}
		}
	}
	pStr->Free();
}

SPDrvInfo* SPDriveStateCfg::GetSPDIByID( DWORD dwID )
{
	for(int i = 0; i < m_pListSPDrvState->Count; i++)
	{
		SPDrvInfo* pSPDI	= (SPDrvInfo*)m_pListSPDrvState->Items[i];
		if(pSPDI->nID == dwID)
			return pSPDI;
	}
	return NULL;
}

SwordSpriteConfig::SwordSpriteConfig()
{
	m_plist = xnList::Create();
}

SwordSpriteConfig::~SwordSpriteConfig()
{
	while (m_plist->Count)
	{
		delete m_plist->Delete(0);
	}
}

void SwordSpriteConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;

	pStr->SetSection("Main");

	int count = atoi(pStr->ReadString("maxlvl","0"));

	char sztxt[32];
	for (int i=0;i<SWORDSPRITE_TYPE_MAX;i++)
	{
		sprintf(sztxt,"state%d",i+1);
		dwStateID[i] = atoi(pStr->ReadString(sztxt,"0"));
	}

	int index = 1;
	while(pStr->SetSection(index++))
	{
		SwordSpriteCfg * pinfo = new SwordSpriteCfg();
		pinfo->wBasePer = atoi(pStr->ReadString("baseper","0"));
		pinfo->WSwordLimit = atoi(pStr->ReadString("swordlimit","0"));
		pinfo->wSwordPer = atoi(pStr->ReadString("swordper","0"));
		pinfo->wAttr1 = atoi(pStr->ReadString("attr1","0"));
		pinfo->wAttr2 = atoi(pStr->ReadString("attr2","0"));
		m_plist->Add(pinfo);
	}

	pStr->Free();
}

SwordSpriteCfg* SwordSpriteConfig::GetSwordSpriteCfg( int lv )
{
	if (lv<1||lv>m_plist->Count)
	{
		return NULL;
	}
	return (SwordSpriteCfg*)m_plist->Items[lv-1];
}
