#include "stdafx.h"
#include "EnemyConfig.h"
#include "xnExtstr.h"
#include "MapStaticCfg.h"

CreateSingleton(EnemyBaseCfg);


EnemyBaseCfg::EnemyBaseCfg()
{

}

EnemyBaseCfg::~EnemyBaseCfg()
{

}

void EnemyBaseCfg::LoadFile( char * pFileName )
{
	xnExtstr* pStr = xnExtstr::Import(pFileName);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	while(pStr->SetSection(nIndex ++))
	{
		EnemyBase* pBase	= new EnemyBase();
		pBase->nID			= atoi(pStr->sectionName);
		pBase->szName		= strdup(pStr->ReadString("name","Enemy"));
		pBase->bLvl			= atoi(pStr->ReadString("lv","1"));
		pBase->wSkin		= atoi(pStr->ReadString("skin","1"));
		pBase->wSkinEx		= atoi(pStr->ReadString("skinex","0"));
		
		pBase->bType		= atoi(pStr->ReadString("atktype","1"));	//普通怪,精英怪,BOSS怪,采集怪,技能怪A(可被攻击),技能怪B(不可)
		pBase->bGroup		= atoi(pStr->ReadString("group","0"));
		pBase->bAtkType		= atoi(pStr->ReadString("type","0"));		
				
		pBase->nMaxHp		= atoi(pStr->ReadString("hp","1000"));
		pBase->nAtkW		= atoi(pStr->ReadString("atkw","100"));
		pBase->nAtkN		= atoi(pStr->ReadString("atkn","100"));
		pBase->nDefW		= atoi(pStr->ReadString("defw","100"));
		pBase->nDefN		= atoi(pStr->ReadString("defn","100"));	
		pBase->nAim			= atoi(pStr->ReadString("aim","100"));
		pBase->nDodge		= atoi(pStr->ReadString("dodge","100"));
		pBase->nCrit		= atoi(pStr->ReadString("crit","100"));
		pBase->nSpeed		= atoi(pStr->ReadString("speed","120"));
		pBase->wAtkRange	= atoi(pStr->ReadString("atkrange","120"));
		pBase->wFindRange	= atoi(pStr->ReadString("findrange","150"));
		pBase->bNoticeGS	= atoi(pStr->ReadString("noticegs","0"));
#ifndef _ISSERVER
		pBase->pPath		= MonsterPathLeadCfg::GetInstance().GetPathObj(pBase->nID);
#else
		pBase->pPath		= NULL;
#endif
		int		nSCnt		= atoi(pStr->ReadString("skillcnt","0"));

		for (int s = 0; s < nSCnt; s++)
		{
			char	szTmp[32];
			sprintf(szTmp,"skill%d",s+1);
			pBase->pLSkill->Add((void*)atoi(pStr->ReadString(szTmp,"0")));
			
		}
		mapEnemyBase[pBase->nID]	= pBase;
	}
	pStr->Free();
}

EnemyBase* EnemyBaseCfg::GetEnemyBase( int nID )
{
	std::map<WORD,EnemyBase*>::iterator	itor	= mapEnemyBase.find(nID);
	if(itor	!= mapEnemyBase.end())
	{
		return itor->second;
	}
	return NULL;
}

EnemyBase::~EnemyBase()
{
	pLSkill->Free();
	free(szName);
}
