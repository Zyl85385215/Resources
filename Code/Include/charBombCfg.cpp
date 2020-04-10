#include "charBombCfg.h"
#include "xnExtstr.h"

CreateSingleton(PlayerBombConfig);
CreateSingleton(PlayerAutoMsgConfig);

PlayerBombConfig::PlayerBombConfig()
{
	pListParams		= xnList::Create();
	pListQuaInfo	= xnList::Create();
}

PlayerBombConfig::~PlayerBombConfig()
{
	while(pListParams->Count)
	{
		delete (PBCfgParamLv*)pListParams->Delete(0);
	}
	pListParams->Free();
	while(pListQuaInfo->Count)
	{
		delete (PBQuaInfo*)pListQuaInfo->Delete(0);
	}
	pListQuaInfo->Free();
}

void PlayerBombConfig::LoadLvFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;
	char szTmp[32];
	if(pStr->SetSection(0))
	{
		for (int i = 0; i < 100; i++)
		{
			sprintf(szTmp,"%d",i+1);
			nLvUpCost[i]	= atoi(pStr->ReadString(szTmp,"99999999"));
		}
	}
	pStr->Free();
}

void PlayerBombConfig::LoadParamFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;
	int	nSIndex	= 0;
	char szTmp[32];
	while(pStr->SetSection(nSIndex++))
	{
		PBCfgParamLv*	pCLParam	= new PBCfgParamLv();
		pListParams->Add(pCLParam);

		pCLParam->byFLv	= atoi(pStr->ReadString("lvlFrom","0"));
		pCLParam->byTLv	= atoi(pStr->ReadString("lvlTo","0"));
		pCLParam->spPreLv.nszVal[0]	= atoi(pStr->ReadString("hp","0"));
		pCLParam->spPreLv.nszVal[1]	= atoi(pStr->ReadString("mp","0"));
		pCLParam->spPreLv.nszVal[2]	= atoi(pStr->ReadString("atkw","0"));
		pCLParam->spPreLv.nszVal[3]	= atoi(pStr->ReadString("atkn","0"));
		pCLParam->spPreLv.nszVal[4]	= atoi(pStr->ReadString("defw","0"));
		pCLParam->spPreLv.nszVal[5]	= atoi(pStr->ReadString("defn","0"));
		pCLParam->spPreLv.nszVal[6]	= atoi(pStr->ReadString("apn","0"));
		pCLParam->spPreLv.nszVal[7]	= atoi(pStr->ReadString("dpw","0"));
		pCLParam->spPreLv.nszVal[8]	= atoi(pStr->ReadString("dunt","0"));
		pCLParam->spPreLv.nszVal[9]	= atoi(pStr->ReadString("mdl","0"));
	}
	pStr->Free();
}

void PlayerBombConfig::LoadQualityFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;
	int	nSIndex	= 0;
	char szTmp[32];
	while(pStr->SetSection(nSIndex++))
	{
		PBQuaInfo*	pQuaInfo	= new PBQuaInfo();
		pListQuaInfo->Add(pQuaInfo);

		pQuaInfo->byQua			= atoi(pStr->sectionName);
		pQuaInfo->nLvlUpCost	= atoi(pStr->ReadString("lvcost","9999"));
		pQuaInfo->nLvlEffect	= atoi(pStr->ReadString("lvleff","0"));

		int nCnt	= atoi(pStr->ReadString("cnt","0"));
		int	nVal,nPer;
		for (int i = 0; i < nCnt; i++)
		{
			sprintf(szTmp,"val%d",i+1);
			nVal	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"per%d",i+1);
			nPer	= atoi(pStr->ReadString(szTmp,"0"));
			pQuaInfo->mapQuaVal[nVal]	= nPer;
		}
	}
	pStr->Free();
}

bool PlayerBombConfig::DealSParamByLvl( OUT SimpleParam* pSPOut,BYTE bLv,BYTE bLvOld )
{
	if(bLvOld == 0)
	{
		for (int i = 0; i < 10; i++)
		{
			pSPOut->nszVal[i]	= 0;
		}
	}
	int	nLvPos	= bLvOld;
	for (int i = 0; i < pListParams->Count; i++)
	{
		PBCfgParamLv* pCPLvl	= (PBCfgParamLv*)pListParams->Items[i];

		if(bLv < pCPLvl->byFLv)
			break;

		if(nLvPos <= pCPLvl->byFLv || nLvPos > pCPLvl->byTLv)
			continue;

		int nFromLv	= pCPLvl->byFLv;
		if(nFromLv < nLvPos)
			nFromLv	= nLvPos;

		if(bLv > pCPLvl->byTLv)
			nLvPos	= pCPLvl->byTLv+1;
		else
			nLvPos	= bLv;

		*pSPOut	+=  pCPLvl->spPreLv*(nLvPos - nFromLv);
	}
	return true;
}

bool  PlayerBombConfig::DealSParamWithQua(OUT SimpleParam* pSPOut,SimpleParam* pSPLvl, SimpleParam* pSPQua, BYTE bQua )
{
	PBQuaInfo* pQI	= GetQuaInfo(bQua);
	if(pQI == NULL)
		return false;

	int nEff	= pQI->nLvlEffect;
	for (int i = 0; i < 10; i ++)
	{
		pSPOut->nszVal[i]	= pSPLvl->nszVal[i] * ((5000+(700* pSPQua->nszVal[i]/500)) / 10000.0) * nEff/10000;
	}

	return false;
}

PBQuaInfo* PlayerBombConfig::GetQuaInfo( BYTE bQua )
{
	for (int i = 0; i < pListQuaInfo->Count; i++)
	{
		PBQuaInfo* pQI	= (PBQuaInfo*)pListQuaInfo->Items[i];
		if(pQI->byQua	== bQua)
			return pQI;
	}
	return NULL;
}

DWORD PlayerBombConfig::GetLevelUpCost( BYTE bCurLv )
{
	if(bCurLv < 0 || bCurLv > 99)
		return 0xFFFFFFFF;

	return nLvUpCost[bCurLv];
}

void PlayerBombConfig::LoadInfo( char* szFile )
{
	
}
SimpleParam SimpleParam::operator*( int nVal )
{
	SimpleParam	spOut;
	for (int i = 0; i < 10; i++)
	{
		spOut.nszVal[i]	= nszVal[i]*nVal;
	}
	return spOut;
}

void SimpleParam::operator+=( SimpleParam& spVal )
{
	for (int i = 0; i < 10; i++)
	{
		nszVal[i]	+= spVal.nszVal[i];
	}
}

SimpleParam SimpleParam::operator+( SimpleParam& spVal )
{	
	SimpleParam	spOut;
	for (int i = 0; i < 10; i++)
	{
		spOut.nszVal[i]	= nszVal[i]+spVal.nszVal[i];
	}
	return spOut;
}

SimpleParam::SimpleParam()
{
	memset(nszVal,0,40);
}

PlayerAutoMsgConfig::PlayerAutoMsgConfig()
{
	m_pList = xnList::Create();
}

PlayerAutoMsgConfig::~PlayerAutoMsgConfig()
{
	while (m_pList->Count)
	{
		delete m_pList->Delete(0);
	}
}

void PlayerAutoMsgConfig::LoadInfo( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;

	pStr->SetSection("Main");
	StateID = atoi(pStr->ReadString("stateID","0"));

	int	nSIndex	= 1;
	char szTmp[32];
	while(pStr->SetSection(nSIndex++))
	{
		PlayerAutoMsgCfg * pinfo = new PlayerAutoMsgCfg;
		sprintf(pinfo->sztxt,"%s",pStr->ReadString("txt",""));
		pinfo->per = atoi(pStr->ReadString("per","0"));
		m_pList->Add(pinfo);
	}
	pStr->Free();
}

PlayerAutoMsgCfg* PlayerAutoMsgConfig::GetAutoMsgCfg( BYTE pos )
{
	if (/*pos>=AUTO_MSG_TYPE_MAX||*/pos>=m_pList->Count)
	{
		return NULL;
	}
	return (PlayerAutoMsgCfg*)m_pList->Items[pos];
}
PlayerAutoMsgCfg::PlayerAutoMsgCfg()
{
	memset(sztxt,0,128);
	per = 0;
}

PlayerAutoMsgCfg::~PlayerAutoMsgCfg()
{

}