#include "EffectObj.h"
#include "GlobalDefine.h"
#include "xnExtstr.h"

CreateSingleton(EffectCenter);

EffectCenter::EffectCenter()
{
	m_pList		=xnList::Create ();


	//InitFile ("cfg/effectcfg.ini");
}

EffectCenter::~EffectCenter()
{
	while(m_pList->Count)
	{
		delete (EffectObj*)m_pList->Delete (0);
	}
	m_pList->Free ();
}

EffectObj* EffectCenter::GetEffObj( int nID )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		EffectObj* pEff	= (EffectObj*)m_pList->Items[i];
		if(pEff->m_nEffID	== nID)
			return pEff;
	}
	return NULL;
}

void EffectCenter::InitFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);

	int nIndex=0;
	while(pStr->SetSection (nIndex++))
	{
		EffectObj*	pEff	= new EffectObj();
		m_pList->Add (pEff);


		pEff->m_nEffID	= atoi (pStr->sectionName);
		pEff->m_nTimeFrm	= atoi(pStr->ReadString("costtime", "1000"));
		pEff->m_nRealSprID	= atoi(pStr->ReadString("plist", "1"));
		pEff->m_nLoopTime	= atoi(pStr->ReadString("looptm", "0"));
		pEff->m_nX		= atoi(pStr->ReadString("posX", "0"));
		pEff->m_nY		= atoi(pStr->ReadString("posY", "0"));
		pEff->m_nHeight		= atoi(pStr->ReadString("height", "0"));
		pEff->byR		= atoi(pStr->ReadString("R", "255"));
		pEff->byG		= atoi(pStr->ReadString("G", "255"));
		pEff->byB		= atoi(pStr->ReadString("B", "255"));

		pEff->m_nScale	= atoi(pStr->ReadString("scale", "100"));
		pEff->m_byScaleBySize	= atoi(pStr->ReadString("byobjsize", "0"));
		pEff->m_bPosMark	= atoi(pStr->ReadString("posmark", "0"));
		pEff->m_nRunType	= atoi(pStr->ReadString("runtype", "0"));
		pEff->m_nZorder	= atoi(pStr->ReadString("zorder", "-1"));
		pEff->m_szSound	= strdup(pStr->ReadString ("sound",""));
	}
	pStr->Free();
}

EffectObj::~EffectObj()
{
	if(m_szSound) 
		free(m_szSound);
}