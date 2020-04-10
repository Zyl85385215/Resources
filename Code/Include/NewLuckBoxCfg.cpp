#include "NewLuckBoxCfg.h"
#include "xnExtstr.h"

CreateSingleton(NewLuckBoxCfg);
CreateSingleton(NewJinLianCfg);

extern int	Random_Int(int min, int max);

NewLuckBoxCfg::NewLuckBoxCfg()
{
	
}

NewLuckBoxCfg::~NewLuckBoxCfg()
{
	
}

void NewLuckBoxCfg::LoadFile( char* szFile )
{
	xnExtstr* pStr = xnExtstr::Import(szFile);
	if(pStr == NULL) return; 
	char	szTmp[32];
	char	szVal[32];
	if(pStr->SetSection(0))
	{
		int nCnt	= atoi(pStr->ReadString("cnt",0));
		for (int i = 0; i < nCnt; i++)
		{
			sprintf(szTmp,"item%d",i+1);
			sprintf(szVal,"per%d",i+1);
			int nPer	= atoi(pStr->ReadString(szVal, "0"));
			m_mapLuckItem[atoi(pStr->ReadString(szTmp, "0"))]	= nPer;
			m_nLuckTotal	+= nPer;
		}
	}
	pStr->Free();
}

DWORD NewLuckBoxCfg::GetItemByLuck( int nTime ,DWORD dwUin /*= 0*/ )
{
	//特定人员的奖励部分
	UinLuckObj*	pUObj	= NewJinLianCfg::GetInstance().GetUinInfo(dwUin);
	if(pUObj)
	{
		std::map<int,DWORD>::iterator itTime	= pUObj->mapTimeItem.find(nTime);
		if(itTime != pUObj->mapTimeItem.end())
			return itTime->second;
	}

	//正常抽奖流程
#ifndef _CLIENT
	int nLVal	= Random_Int(1,m_nLuckTotal);
	std::map<DWORD,int>::iterator	itItem	= m_mapLuckItem.begin();
	while(itItem	!= m_mapLuckItem.end())
	{
		if(nLVal <= itItem->second)
		{
			return itItem->first;
		}
		nLVal -= itItem->second;
		itItem++;
	}
#endif
	return 0;
}

NewJinLianCfg::NewJinLianCfg()
{
	
}

NewJinLianCfg::~NewJinLianCfg()
{
	std::map<DWORD,UinLuckObj*>::iterator	itor	= m_mapUinLuck.begin();
	while(itor != m_mapUinLuck.end())
	{
		delete itor->second;
		itor++;
	}
}

void NewJinLianCfg::LoadFile( char* szFile )
{
	xnExtstr* pStr = xnExtstr::Import(szFile);
	if(pStr == NULL) return; 
	int index = 0;
	char szTmp[32];
	char	szTime[32];
	while (pStr->SetSection(index++) != NULL)
	{
		DWORD	dwUin	= atoi(pStr->sectionName);
		UinLuckObj*	pULObj	= new UinLuckObj;
		m_mapUinLuck[dwUin]	= pULObj;
		int nCnt	= atoi(pStr->ReadString("cnt","0"));
		for (int i = 0; i < nCnt; i++)
		{
			sprintf(szTmp,"item%d",i+1);
			sprintf(szTime,"time%d",i+1);
			pULObj->mapTimeItem[atoi(pStr->ReadString(szTime, "0"))]	= atoi(pStr->ReadString(szTmp, "99999"));
		}
	}

	pStr->Free();
}

UinLuckObj* NewJinLianCfg::GetUinInfo( DWORD dwUin )
{
	std::map<DWORD,UinLuckObj*>::iterator	itor	= m_mapUinLuck.find(dwUin);
	if(itor	!= m_mapUinLuck.end())
	{
		return itor->second;
	}
	return NULL;
}