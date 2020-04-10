#include "ClientFlagConfig.h"
#include "xnExtstr.h"
#include "stdio.h"

CreateSingleton(ClientFlagConfig);
CreateSingleton(LuckBoxConfig);
CreateSingleton(ClientCompFlagCfg);

ClientFlagConfig::ClientFlagConfig()
{
	m_pList	= xnList::Create();
}

ClientFlagConfig::~ClientFlagConfig()
{
	while(m_pList->Count)
	{
		free((char*)m_pList->Delete(0));
	}
	m_pList->Free();
}

void ClientFlagConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;
	int iIndex = 0;
	char szTmp[32];

	if(pStr->SetSection(0))
	{
		int nCnt	= atoi(pStr->ReadString("cnt","0"));

		for (int i = 0; i < nCnt; i++)
		{
			sprintf(szTmp,"flag%d",i+1);
			m_pList->Add(strdup(pStr->ReadString(szTmp,"")));
		}
	}
	pStr->Free();
}

int ClientFlagConfig::GetFlagPos( char* szFlag )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		char* szTmp	= (char*)m_pList->Items[i];
		if(strcmp(szFlag,szTmp)==0)
		{
			return i;
		}
	}
	return -1;
}

char* ClientFlagConfig::GetPosFlag( int nPos )
{
	if(nPos < 0 || nPos >= m_pList->Count)
		return NULL;

	return (char*)m_pList->Items[nPos];
}

ItemLuckBox::ItemLuckBox()
{
	pListLuckI		= xnList::Create();
	pListNormalI	= xnList::Create();
	pListPointI		= xnList::Create();
}

ItemLuckBox::~ItemLuckBox()
{
	while(pListLuckI->Count)
	{
		delete (LItemObjInfo*)pListLuckI->Delete(0);
	}
	pListLuckI->Free();
	while(pListNormalI->Count)
	{
		delete (LItemObjInfo*)pListNormalI->Delete(0);
	}
	pListNormalI->Free();
	while(pListPointI->Count)
	{
		delete (LItemObjInfo*)pListPointI->Delete(0);
	}
	pListPointI->Free();
}

LuckBoxConfig::LuckBoxConfig()
{
	m_pList		= xnList::Create();
}

LuckBoxConfig::~LuckBoxConfig()
{
	while(m_pList->Count)
	{
		delete (ItemLuckBox*)m_pList->Delete(0);
	}
	m_pList->Free();
}

void LuckBoxConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;
	int nIndex = 0;
	char szTmp[32];

	while(pStr->SetSection(nIndex++))
	{
		ItemLuckBox* pILB	= new ItemLuckBox();
		m_pList->Add(pILB);
		pILB->dwItemID	= atoi(pStr->sectionName);
		pILB->byNeedPos	= atoi(pStr->ReadString("needPos","10"));

		int nPtCnt	= atoi(pStr->ReadString("ptCnt","0"));
		for (int i = 0; i < nPtCnt; i++)
		{
			LItemObjInfo*	pLIBI	= new LItemObjInfo();
			pILB->pListPointI->Add(pLIBI);
			sprintf(szTmp,"ptItem%d",i+1);
			pLIBI->dwItemID	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"ptVal%d",i+1);
			pLIBI->nVal	= atoi(pStr->ReadString(szTmp,"99999"));
		}

		int nNCnt	= atoi(pStr->ReadString("nCnt","0"));
		for (int i = 0; i < nNCnt; i++)
		{
			LItemObjInfo*	pLIBI	= new LItemObjInfo();
			pILB->pListNormalI->Add(pLIBI);
			sprintf(szTmp,"nItem%d",i+1);
			pLIBI->dwItemID	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"nNum%d",i+1);
			pLIBI->nVal	= atoi(pStr->ReadString(szTmp,"1"));
		}

		int nLCnt	= atoi(pStr->ReadString("lCnt","0"));
		for (int i = 0; i < nLCnt; i++)
		{
			LItemObjInfo*	pLIBI	= new LItemObjInfo();
			pILB->pListLuckI->Add(pLIBI);
			sprintf(szTmp,"lItem%d",i+1);
			pLIBI->dwItemID	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"lPer%d",i+1);
			pLIBI->nVal	= atoi(pStr->ReadString(szTmp,"0"));
		}
	}
	pStr->Free();
}

ItemLuckBox* LuckBoxConfig::GetLuckItemInfo( DWORD dwID )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		ItemLuckBox* pILB	= (ItemLuckBox*)m_pList->Items[i];
		if(pILB->dwItemID	== dwID)
			return pILB;
	}
	return NULL;
}

ClientCompFlagCfg::ClientCompFlagCfg()
{
	m_pListFlag		= xnList::Create();
}

ClientCompFlagCfg::~ClientCompFlagCfg()
{
	while(m_pListFlag->Count)
	{
		delete (CCFlagObj*)m_pListFlag->Delete(0);
	}
	m_pListFlag->Free();
}

void ClientCompFlagCfg::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;
	int iIndex = 0;
	char szTmp[32];

	while(pStr->SetSection(iIndex++))
	{
		int nCnt	= atoi(pStr->ReadString("cnt","0"));

		CCFlagObj*	pCCFlag	= new CCFlagObj;
		m_pListFlag->Add(pCCFlag);
		strcpy(pCCFlag->szFlag,pStr->sectionName);

		for (int i = 0; i < nCnt; i++)
		{
			CCFlagFormCmp*	pFFC	= new CCFlagFormCmp;
			pCCFlag->pListFC->Add(pFFC);
			sprintf(szTmp,"form%d",i+1);
			strcpy(pFFC->szForm,pStr->ReadString(szTmp,""));
			sprintf(szTmp,"comp%d",i+1);
			strcpy(pFFC->szComp,pStr->ReadString(szTmp,""));
		}
	}
	pStr->Free();
}

int ClientCompFlagCfg::GetFlagPos( char* szFlag )
{
	for (int i = 0; i < m_pListFlag->Count; i++)
	{
		CCFlagObj* pFlag	= (CCFlagObj*)m_pListFlag->Items[i];
		if(strcmp(szFlag,pFlag->szFlag)==0)
		{
			return i;
		}
	}
	return -1;
}
CCFlagObj::CCFlagObj()
{
	pListFC		= xnList::Create();
}

CCFlagObj::~CCFlagObj()
{
	while(pListFC->Count)
	{
		delete (CCFlagFormCmp*)pListFC->Delete(0);
	}
	pListFC->Free();
}	