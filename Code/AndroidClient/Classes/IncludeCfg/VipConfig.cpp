#include "stdafx.h"
#include "VipConfig.h"
#include "xnExtstr.h"

CreateSingleton(VipConfig);
CreateSingleton(VipAwardConfig);

VipConfig::VipConfig()
{
	m_pList = xnList::Create();
}

VipConfig::~VipConfig()
{
	if(m_pList)
	{
		while(m_pList->Count)
		{
			delete (VipInfoObj*)m_pList->Delete(0);
		}
		m_pList->Free();
	}
}

void VipConfig::LoadFile(char* pFile)
{
	xnExtstr* pStr = xnExtstr::Import(pFile);
	int nIndex = 0;
	while(pStr->SetSection(nIndex++))
	{
		VipInfoObj* pObj	= new VipInfoObj();
		pObj->m_nNeedgold	= atoi(pStr->ReadString("needgold","10"));
		pObj->m_nExpadd		= atoi(pStr->ReadString("expadd","0"));
		pObj->m_nTaxsub		= atoi(pStr->ReadString("taxsub","0"));
		pObj->m_nPrestigeadd= atoi(pStr->ReadString("prestigeadd","0"));
		pObj->m_nField		= atoi(pStr->ReadString("field","0"));
		pObj->m_fixswitch	= atoi(pStr->ReadString("fixswitch","0"));
		pObj->m_chatswitch	= atoi(pStr->ReadString("chatswitch","0"));
		pObj->m_buyswitch	= atoi(pStr->ReadString("buyswitch","0"));
		pObj->m_nPoweradd	= atoi(pStr->ReadString("poweradd","0"));
		pObj->m_nCardtimes	= atoi(pStr->ReadString("cardtimes","0"));
		pObj->m_proclamtion	= atoi(pStr->ReadString("proclamtion","0"));
		pObj->m_skillswitch	= atoi(pStr->ReadString("skillswitch","0"));
		char* pTemp = pStr->ReadString("desc","ÃèÊö");
		FixDescString(pTemp);
		std::string strM(pTemp);
		StringSplit(strM,"/n",pObj->vecdesc);
		pObj->m_nLv = m_pList->Count + 1;
		m_pList->Add(pObj);
	}
	pStr->Free();
}

VipInfoObj* VipConfig::GetObjByLv(int nLv)
{
	if(nLv <= 0 || nLv > m_pList->Count)
		return NULL;
	return (VipInfoObj*)m_pList->Items[nLv-1];
}

VipAwardConfig::VipAwardConfig()
{
	m_pList = xnList::Create();
}

VipAwardConfig::~VipAwardConfig()
{
	if(m_pList)
	{
		while(m_pList->Count)
		{
			VipAward* pObj = (VipAward*)m_pList->Delete(0);
			if(pObj->pList)
			{
				while(pObj->pList->Count)
					pObj->pList->Delete(0);
				pObj->pList->Free();
			}
			delete pObj;
		}
		m_pList->Free();
	}
}

void VipAwardConfig::LoadFile(char* pFile)
{
	xnExtstr* pStr = xnExtstr::Import(pFile);
	int nIndex = 0;
	char szTmp[32];
	while(pStr->SetSection(nIndex++))
	{
		VipAward* pObj	= new VipAward();
		pObj->pList = xnList::Create();

		pObj->nGold = atoi(pStr->ReadString("gold","0"));
		int nCnt = atoi(pStr->ReadString("itemcnt","0"));
		for (int i = 0; i < nCnt; i++)
		{
			sprintf(szTmp,"item%d",i+1);
			int ItemAndNum = atoi(pStr->ReadString(szTmp,"0"));
			pObj->pList->Add((void*)ItemAndNum);
		}

		m_pList->Add(pObj);
	}
	pStr->Free();
}

VipAward* VipAwardConfig::GetObjByVipLv(int nLv)
{
	if(nLv <= 0 || nLv > m_pList->Count)
		return NULL;
	return (VipAward*)m_pList->Items[nLv-1];
}