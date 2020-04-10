#include "stdafx.h"
#include "RobotConfig.h"
#include "xnExtstr.h"

CreateSingleton(RobotConfig);

RobotObj::RobotObj()
{
	pLSkill		= xnList::Create();
}

RobotObj::~RobotObj()
{
	pLSkill->Free();
}

RobotConfig::RobotConfig()
{
	m_pListRobot	= xnList::Create();
}

RobotConfig::~RobotConfig()
{
	while(m_pListRobot->Count)
		delete (RobotObj*)m_pListRobot->Delete(0);
	m_pListRobot->Free();
}

void RobotConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[256];
	while (pStr->SetSection(index++) != NULL)
	{
		RobotObj*	pRobot	= new RobotObj();
		pRobot->wID			= atoi(pStr->sectionName);
		pRobot->wSkin[0]	= atoi(pStr->ReadString("skin1","0"));
		pRobot->wSkin[1]	= atoi(pStr->ReadString("skin2","0"));
		pRobot->wSkin[2]	= atoi(pStr->ReadString("skin3","0"));
		
		int		nSkillCnt	= atoi(pStr->ReadString("skillcnt","0"));
		for (int i = 0; i < nSkillCnt; i++)
		{
			sprintf(szTmp,"skill%d",i+1);
			pRobot->pLSkill->Add((void*)atoi(pStr->ReadString(szTmp,"0")));
		}
		pRobot->bLv			= atoi(pStr->ReadString("lv","1"));
		pRobot->bJob		= atoi(pStr->ReadString("job","1"));
		pRobot->bSex		= atoi(pStr->ReadString("sex","0"));
		pRobot->xParam.nHpMax	= atoi(pStr->ReadString("hpmax","500"));
		pRobot->xParam.nHp		= pRobot->xParam.nHpMax;
		pRobot->xParam.nMpMax	= 1000;
		pRobot->xParam.nMp		= 1000;
		pRobot->xParam.nAtkW	= atoi(pStr->ReadString("atkw","100"));
		pRobot->xParam.nAtkN	= atoi(pStr->ReadString("atkn","100"));
		pRobot->xParam.nDefW	= atoi(pStr->ReadString("defw","100"));
		pRobot->xParam.nDefN	= atoi(pStr->ReadString("defn","100"));
		pRobot->xParam.nAim		= atoi(pStr->ReadString("aim","100"));
		pRobot->xParam.nDodge	= atoi(pStr->ReadString("dodge","100"));
		pRobot->xParam.nCrit	= atoi(pStr->ReadString("crit","100"));
		pRobot->xParam.nMSpeed	= atoi(pStr->ReadString("speed","100"));
		m_pListRobot->Add(pRobot);
	}
	pStr->Free();
}

RobotObj* RobotConfig::GetRobotByID( WORD wID )
{
	for (int i = 0; i < m_pListRobot->Count; i++)
	{
		RobotObj*	pRobot	= (RobotObj*)m_pListRobot->Items[i];
		if(pRobot->wID	== wID)
			return pRobot;
	}
	return NULL;
}

MRActionPt::MRActionPt()
{
	pLActPt		= xnList::Create();
	pLRobot		= xnList::Create();
}

MRActionPt::~MRActionPt()
{
	while(pLActPt->Count)
		delete (_MuliParamObj*)pLActPt->Delete(0);
	pLActPt->Free();
	while(pLRobot->Count)
		delete (_MuliParamObj*)pLRobot->Delete(0);
	pLRobot->Free();
}

MRActionCenter::MRActionCenter(int	nBaseMIndex)
{
	m_pListActPts	= xnList::Create();
	char	szTmp[256];
	sprintf(szTmp,"Map//%d//RobotActionCfg.txt",nBaseMIndex);
	
	xnExtstr * pStr = xnExtstr::Import(szTmp);
	if( pStr == NULL )
		return;
	int index = 0;
	while (pStr->SetSection(index++) != NULL)
	{
		MRActionPt*	pActPt	= new MRActionPt();
		pActPt->nID			= atoi(pStr->sectionName);
		pActPt->nX			= atoi(pStr->ReadString("px","1000"));
		pActPt->nY			= atoi(pStr->ReadString("py","1000"));
		pActPt->bForce		= atoi(pStr->ReadString("rbforce","0"));


		int	nTypeCnt		= atoi(pStr->ReadString("rbtypecnt","0"));
		for (int i = 0; i < nTypeCnt; i++)
		{
			_MuliParamObj* pMP	= new _MuliParamObj();
			sprintf(szTmp,"rbcnt%d",i+1);
			pMP->nParam1	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"rbval%d",i+1);
			WORD	wRobot		= atoi(pStr->ReadString(szTmp,"0"));
			pMP->nParam2		= (int)RobotConfig::GetInstance().GetRobotByID(wRobot);
			pActPt->pLRobot->Add(pMP);
		}
		
		int	nActCnt			= atoi(pStr->ReadString("actcnt","0"));
		for (int i = 0; i < nActCnt; i++)
		{
			_MuliParamObj* pMP	= new _MuliParamObj();
			sprintf(szTmp,"act%d",i+1);
			pMP->nParam1	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"actval%d",i+1);
			pMP->nParam2	= atoi(pStr->ReadString(szTmp,"0"));
			pActPt->pLActPt->Add(pMP);
		}
		m_pListActPts->Add(pActPt);
	}
	pStr->Free();
}

MRActionCenter::~MRActionCenter()
{
	while(m_pListActPts->Count)
		delete (MRActionPt*)m_pListActPts->Delete(0);
	m_pListActPts->Free();
}

MRActionPt* MRActionCenter::GetActPt( int nID )
{
	for (int i = 0; i < m_pListActPts->Count; i++)
	{
		MRActionPt*	pPt	= (MRActionPt*)m_pListActPts->Items[i];
		if(pPt->nID	== nID)
			return pPt;
	}
	return NULL;
}
