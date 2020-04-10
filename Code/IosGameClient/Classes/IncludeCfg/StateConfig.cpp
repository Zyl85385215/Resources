
#include "stdafx.h"
#ifdef _ISSERVER
#include "stdio.h"
#endif

#include "StateConfig.h"
#include "xnExtstr.h"

CreateSingleton(StateConfig);

StateBase::StateBase()
{
	szName		= NULL;
	szDesc		= NULL;
	pListStateEff		= xnList::Create ();
}

StateBase::~StateBase()
{
	if(szName)
		free(szName);
	if(szDesc)
		free(szDesc);
	while(pListStateEff->Count)
	{
		delete (StateEffVal*)pListStateEff->Delete (0);
	}
	pListStateEff->Free ();
}

StateConfig::StateConfig()
{
	m_pList		= xnList::Create ();
}

StateConfig::~StateConfig()
{
	while(m_pList->Count)
	{
		delete (StateBase*)m_pList->Delete (0);
	}
	m_pList->Free ();
}

void StateConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[32];
	while (pStr->SetSection(index++) != NULL)
	{
		StateBase*	pSBase	= new StateBase();
		pSBase->wStateID	= atoi (pStr->sectionName);
		pSBase->wWorkPos	= atoi (pStr->ReadString ("spos","1"));
		pSBase->bWorkVal	= atoi (pStr->ReadString ("sval","1"));
		pSBase->wIcon		= atoi (pStr->ReadString ("icon","1"));
		pSBase->szName		= strdup(pStr->ReadString("name","state"));
		pSBase->szDesc		= strdup(pStr->ReadString("txt","desc"));

		pSBase->wEff		= atoi (pStr->ReadString ("eff","0"));
		pSBase->wEffex		= atoi (pStr->ReadString ("effex","0"));
		pSBase->bWorkTime	= atoi (pStr->ReadString ("workTime","1"));
		pSBase->dwPerTick	= atoi (pStr->ReadString ("time","3000"));

		int nEffCnt	= atoi (pStr->ReadString ("spmcnt","0"));
		for (int i = 0; i < nEffCnt; i++)
		{
			StateEffVal* pSEV	= new StateEffVal();
			sprintf(szTmp,"spmtype%d",i+1);
			pSEV->bType		= atoi (pStr->ReadString (szTmp,"0"));
			sprintf(szTmp,"spmval%d",i+1);
			pSEV->nSpVal	= atoi (pStr->ReadString (szTmp,"0"));
			sprintf(szTmp,"spmpper%d",i+1);
			pSEV->nPer		= atoi (pStr->ReadString (szTmp,"0"));
			pSBase->pListStateEff->Add (pSEV);
		}

		char*	szSpEff		= pStr->ReadString ("spefftype","0000000");
		pSBase->szbSpEff[0]	= szSpEff[0]-'0';
		pSBase->szbSpEff[1]	= szSpEff[1]-'0';
		pSBase->szbSpEff[2]	= szSpEff[2]-'0';
		pSBase->szbSpEff[3]	= szSpEff[3]-'0';
		pSBase->szbSpEff[4]	= szSpEff[4]-'0';
		pSBase->szbSpEff[5]	= szSpEff[5]-'0';
		if(strlen(szSpEff) < 7)
			pSBase->szbSpEff[6]	= 0;
		else
			pSBase->szbSpEff[6]	= szSpEff[6]-'0';
		if(strlen(szSpEff) < 8)
			pSBase->szbSpEff[7]	= 0;
		else
			pSBase->szbSpEff[7]	= szSpEff[7]-'0';
		
		pSBase->bLayerStart	= atoi (pStr->ReadString ("lstart","1"));
		pSBase->bLayerMax	= atoi (pStr->ReadString ("lmax","1"));
		
		pSBase->wChgSkin	= atoi (pStr->ReadString ("chgskin","0"));
		pSBase->wChgWeapon	= atoi (pStr->ReadString ("chgweapon","0"));
		pSBase->wChgHorse	= atoi (pStr->ReadString ("chghorse","0"));
		pSBase->bSkinPos	= atoi (pStr->ReadString ("skinpos","0"));
		pSBase->bDisType	= atoi (pStr->ReadString ("distype","0"));
		pSBase->bOffLineDis	= atoi (pStr->ReadString ("offlinedis","0"));
		pSBase->bSyncClient	= atoi (pStr->ReadString ("syncclient","1"));
		pSBase->wRemoveMark	= atoi (pStr->ReadString ("removemark","0"));
		m_pList->Add (pSBase);
	}
	pStr->Free ();
}

StateBase* StateConfig::GetStateBase( WORD wStateID )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		StateBase*	pSB		= (StateBase*)m_pList->Items[i];
		if(pSB->wStateID	== wStateID)
			return pSB;
	}
	return NULL;
}