#include "WarMchCfg.h"
#include <stdio.h>


CreateSingleton(CfgWarMachine);

WMCfgLvObj::WMCfgLvObj()
{
	pListItem	= xnList::Create();
}

WMCfgLvObj::~WMCfgLvObj()
{
	while(pListItem->Count)
	{
		delete (WMCfgItem*)pListItem->Delete(0);
	}
	pListItem->Free();
}

CfgWarMachine::CfgWarMachine()
{
	m_pLSkillAtk	= xnList::Create();
	m_pLSkillDef	= xnList::Create();

	memset(m_ppWMLvl,0,sizeof(m_ppWMLvl));
}

CfgWarMachine::~CfgWarMachine()
{
	for (int i = 0; i < 37; i++)
	{
		if(m_ppWMLvl[i])
			delete m_ppWMLvl[i];
	}
	m_pLSkillAtk->Free();
	m_pLSkillDef->Free();
}

void CfgWarMachine::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if (NULL == pStr)
		return;

	char	szTmp[32];
	for (int i = 0; i < 37; i++)
	{
		sprintf(szTmp,"level%d",i+1);
		if (pStr->SetSection(szTmp) != NULL)
		{
			m_ppWMLvl[i]	= new WMCfgLvObj();
			m_ppWMLvl[i]->bSkin	= atoi(pStr->ReadString("SKIN","0"));
			m_ppWMLvl[i]->dwItemID	= atoi(pStr->ReadString("ITEM","0"));
			m_ppWMLvl[i]->dwHP	= atoi(pStr->ReadString("HP", "5000000"));
			m_ppWMLvl[i]->dwDef	= atoi(pStr->ReadString("DEF", "100000"));
			m_ppWMLvl[i]->dwAtk	= atoi(pStr->ReadString("ATK", "100000"));
			m_ppWMLvl[i]->wState	= atoi(pStr->ReadString("STATE", "0"));
			m_ppWMLvl[i]->dwDure	= atoi(pStr->ReadString("DURE", "1000"));
			m_ppWMLvl[i]->dwNeedExp	= atoi(pStr->ReadString("EXP", "100000"));
			m_ppWMLvl[i]->dwNeedGold	= atoi(pStr->ReadString("MONEY", "0"));
			m_ppWMLvl[i]->dwSpeed		= atoi(pStr->ReadString("SPEED", "0"));
			int	nICnt	= atoi(pStr->ReadString("cnt", "0"));
			for (int j = 0; j < nICnt; j++)
			{
				WMCfgItem*	pWMCItem	= new WMCfgItem();
				m_ppWMLvl[i]->pListItem->Add(pWMCItem);
				sprintf(szTmp,"item%d",j+1);
				pWMCItem->dwItemID	= atoi(pStr->ReadString(szTmp, "0"));
				sprintf(szTmp,"num%d",j+1);
				pWMCItem->wNum	= atoi(pStr->ReadString(szTmp, "0"));
			}
		}
	}
	if (pStr->SetSection("SkillAtk") != NULL)
	{
		int nCnt	= atoi(pStr->ReadString("cnt", "0"));
		for (int i = 0; i < nCnt; i++)
		{
			sprintf(szTmp,"skill%d",i+1);
			m_pLSkillAtk->Add((void*)(atoi(pStr->ReadString(szTmp, "0"))));
		}
	}
	if (pStr->SetSection("SkillDef") != NULL)
	{
		int nCnt	= atoi(pStr->ReadString("cnt", "0"));
		for (int i = 0; i < nCnt; i++)
		{
			sprintf(szTmp,"skill%d",i+1);
			m_pLSkillDef->Add((void*)(atoi(pStr->ReadString(szTmp, "0"))));
		}
	}
	pStr->Free();
}

WMCfgLvObj* CfgWarMachine::GetWMCObj( BYTE bLv )
{
	if(bLv < 37)
		return m_ppWMLvl[bLv];
	return NULL;
}

WORD CfgWarMachine::GetAtkSkill( BYTE bPos )
{
	if(bPos >= m_pLSkillAtk->Count)
		return 0;

	return (DWORD)m_pLSkillAtk->Items[bPos];
}

WORD CfgWarMachine::GetDefSkill( BYTE bPos )
{
	if(bPos >= m_pLSkillDef->Count)
		return 0;

	return (DWORD)m_pLSkillDef->Items[bPos];
}

bool CfgWarMachine::CheckCanLearn( WORD wSkill )
{
	for (int i = 0; i < m_pLSkillAtk->Count; i++)
	{
		WORD wSA	= (DWORD)m_pLSkillAtk->Items[i];
		if(wSA == wSkill)
			return true;
	}
	for (int i = 0; i < m_pLSkillDef->Count; i++)
	{
		WORD wSD	= (DWORD)m_pLSkillDef->Items[i];
		if(wSD == wSkill)
			return true;
	}
	return false;
}

WMClientInfo::WMClientInfo()
{
	pList	= xnList::Create();
	pLUsed	= xnList::Create();
	dwExp	= 0;
	bLv		= 0;
	nSPPoint	= 0;
}

WMClientInfo::~WMClientInfo()
{
	pList->Free();
	pLUsed->Free();
}