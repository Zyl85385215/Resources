#include "CaptainConfig.h"
#include "xnExtstr.h"

#ifdef _ISSERVER
#include "stdio.h"
#define SAFE_FREE(p) { if(p) { free(p); p=NULL; } }

#endif
CreateSingleton(CaptainConfig);

CaptainConfig::CaptainConfig()
{
	pListCaptain	= xnList::Create ();
	pListLv			= xnList::Create ();
}

CaptainConfig::~CaptainConfig()
{
	while(pListLv->Count)
		delete (CaptainLvCfgObj*)pListLv->Delete (0);
	pListLv->Free ();
	while(pListCaptain->Count)
		delete (CaptainBase*)pListCaptain->Delete (0);
	pListCaptain->Free ();
}

void CaptainConfig::LoadCaptainFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[32];
	while (pStr->SetSection(index++) != NULL)
	{
		CaptainBase*	pSPC	= new CaptainBase();
		pSPC->nTypeID		= atoi (pStr->sectionName);
		pSPC->szName		= strdup(pStr->ReadString ("name","未知船长"));
		pSPC->szDesc		= strdup(pStr->ReadString ("desc","神秘的"));
		pSPC->nSpr			= atoi (pStr->ReadString ("spr","1"));
		pSPC->nStar			= atoi (pStr->ReadString ("star","1"));

		int nCnt		= atoi (pStr->ReadString ("effcnt","0"));
		for (int i = 0; i < nCnt; i++)
		{
			_MuliParamObj* pMP	= new _MuliParamObj();
			sprintf(szTmp,"efftype%d",i+1);
			pMP->nParam1		= atoi (pStr->ReadString (szTmp,"0"));
			sprintf(szTmp,"effval%d",i+1);
			pMP->nParam2		= atoi (pStr->ReadString (szTmp,"1"));
			pSPC->pListEffect->Add (pMP);
		}
		pListCaptain->Add (pSPC);
	}
	pStr->Free ();
}

void CaptainConfig::LoadLvFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[32];
	while (pStr->SetSection(index++) != NULL)
	{
		CaptainLvCfgObj*	pSLC	= new CaptainLvCfgObj();
		
		pSLC->byLv			= index+1;
		pSLC->nExpCost		= atoi (pStr->ReadString ("exp","100000"));
		pSLC->nGoldCost		= atoi(pStr->ReadString ("gold","100000"));

		pSLC->nParams[WARS_PARAM_MAXHP]		= atoi (pStr->ReadString ("hp","0"));
		pSLC->nParams[WARS_PARAM_ATK]		= atoi (pStr->ReadString ("atk","0"));
		pSLC->nParams[WARS_PARAM_DEF]		= atoi (pStr->ReadString ("def","0"));
		//pSLC->nParams[WARS_PARAM_COOL]		= atoi (pStr->ReadString ("speed","300"));
		pSLC->nParams[WARS_PARAM_MISS]		= atoi (pStr->ReadString ("miss","0"));
		pSLC->nParams[WARS_PARAM_CRITIC]	= atoi (pStr->ReadString ("critic","0"));
		pListLv->Add (pSLC);
	}
	pStr->Free ();

}

CaptainBase* CaptainConfig::GetCaptainBase( int nSType )
{
	for (int i = 0; i < pListCaptain->Count; i++)
	{
		CaptainBase* pSPC	= (CaptainBase*)pListCaptain->Items[i];
		if(pSPC->nTypeID == nSType)
			return pSPC;
	}
	return NULL;
}

CaptainLvCfgObj* CaptainConfig::GetCaptainLv( int nLv )
{
	if(nLv > pListLv->Count || nLv == 0)
		return NULL;
	return (CaptainLvCfgObj*)pListLv->Items[nLv-1];
}

CaptainBase::CaptainBase()
{
	pListEffect		= xnList::Create ();
	szName			= NULL;
	szDesc			= NULL;
}

CaptainBase::~CaptainBase()
{
	SAFE_FREE(szName);
	SAFE_FREE(szDesc);
	while(pListEffect->Count)
		delete (_MuliParamObj*)pListEffect->Delete (0);
	pListEffect->Free ();
}