#include "HonorTitle.h"
#include "xnExtstr.h"
#include "stdio.h"
CreateSingleton(HonorTitleConfig);

HonorTitleConfig::HonorTitleConfig()
{
	m_pListLv	= xnList::Create();
	m_pListSchool= xnList::Create();
}

HonorTitleConfig::~HonorTitleConfig()
{
	while(m_pListLv->Count)
	{
		delete (HonorTLv*)m_pListLv->Delete(0);
	}
	m_pListLv->Free();
	while(m_pListSchool->Count)
	{
		delete (HonorPart*)m_pListSchool->Delete(0);
	}
	m_pListSchool->Free();
}

void HonorTitleConfig::Load( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;
	int nIndex = 0;
	m_nMaxVal = 0;
	char szTmp[32];
	while (pStr->SetSection(nIndex++) != NULL)
	{
		if(strcmp(pStr->sectionName,"level")==0)
		{
			int		nCnt	= atoi(pStr->ReadString("lvlcnt", "0"));
			for (int i = 0; i < nCnt; i++)
			{
				HonorTLv*	pHTL	= new HonorTLv();
				sprintf(szTmp,"name%d",i+1);
				strncpy(pHTL->szName,pStr->ReadString(szTmp,"未命名"),32);
				sprintf(szTmp,"val%d",i+1);
				pHTL->nSVal		= atoi(pStr->ReadString(szTmp, "0"));
				if(pHTL->nSVal > m_nMaxVal)
					m_nMaxVal = pHTL->nSVal;
				m_pListLv->Add(pHTL);
			}
		}
		else
		{
			HonorPart*		pPart	= new HonorPart();
			strncpy(pPart->szPart,pStr->sectionName,32);
			strncpy(pPart->szTxt,pStr->ReadString("txt", "神秘"),255);

			int		nCnt	= atoi(pStr->ReadString("cnt", "0"));
			for (int i = 0; i < nCnt; i++)
			{
				HonorTSchool* pHTS	= new HonorTSchool();
				sprintf(szTmp,"name%d",i+1);
				strncpy(pHTS->szName,pStr->ReadString(szTmp,"未命名"),32);
				sprintf(szTmp,"txt%d",i+1);
				strncpy(pHTS->szTxt,pStr->ReadString(szTmp,"神秘"),255);
				
				for (int j = 0; j < 6; j++)
				{
					sprintf(szTmp,"title%d%d",i+1,j+1);
					pHTS->wTitle[j]	= atoi(pStr->ReadString(szTmp, "0"));
				}
				pPart->pList->Add(pHTS);
			}
			m_pListSchool->Add(pPart);
		}
	}
	pStr->Free();
}

WORD HonorTitleConfig::GetTitleBySchool( BYTE bPos, int nVal )
{
	int nLv	= GetLvByVal(nVal);
	if(nLv < 0 || nLv >= 6)		return 0;
	for (int i = 0; i < m_pListSchool->Count;i++)
	{
		HonorPart* pHP	= (HonorPart*)m_pListSchool->Items[i];
		if(bPos < pHP->pList->Count)
		{
			HonorTSchool* pHTS	= (HonorTSchool*)pHP->pList->Items[bPos];
			return pHTS->wTitle[nLv];
		}
		else
			bPos -= pHP->pList->Count;
	}
	return 0;
}

char* HonorTitleConfig::GetLvName( int nVal )
{
	int nLv	= GetLvByVal(nVal);
	if(nLv >= 0 && nLv < m_pListLv->Count)
	{
		HonorTLv* pHTL	= (HonorTLv*)m_pListLv->Items[nLv];
		return pHTL->szName;
	}
	return NULL;
}

char* HonorTitleConfig::GetLvlName(BYTE blvl)
{
	if(blvl >= 0 && blvl < m_pListLv->Count)
	{
		HonorTLv* pHTL	= (HonorTLv*)m_pListLv->Items[blvl];
		return pHTL->szName;
	}
	return NULL;
}

int HonorTitleConfig::GetLvByVal( int nVal )
{
	for(int i = 0; i < m_pListLv->Count; i++)
	{
		HonorTLv* pHTL	= (HonorTLv*)m_pListLv->Items[i];
		if(nVal < pHTL->nSVal)
			return i-1;
		if(nVal == pHTL->nSVal)
			return i;
	}
	return -1;
}

HonorTLv* HonorTitleConfig::InitLvlInfo( IN int nVal, OUT DWORD& nCurOut, OUT DWORD& nMaxOut )
{
	if (nVal==12000)
	{
		nMaxOut = 6000;
		nCurOut = 6000;
		return NULL;
	}
	for(int n = 1; n < m_pListLv->Count; n++)
	{
		HonorTLv* pHTLPrev	= (HonorTLv*)m_pListLv->Items[n-1];
		HonorTLv* pHTLNext	= (HonorTLv*)m_pListLv->Items[n];
		if(nVal >= pHTLPrev->nSVal && nVal < pHTLNext->nSVal)
		{
			nMaxOut	= pHTLNext->nSVal - pHTLPrev->nSVal;
			nCurOut	= nVal - pHTLPrev->nSVal;
			return pHTLPrev;
		}
	}
	return NULL;
}
#define  MAX_HONORSCHOOL 9
#define	 MAX_HONORTYPE 3
char* HonorTitleConfig::GetHonorName(BYTE bPos)
{
	int nRealPos	= 0;
	int nPart	= HonorTitleConfig::GetInstance().GetPartByPos(bPos, &nRealPos);
	if(nPart >= MAX_HONORTYPE)	return NULL;
	if(nRealPos >= MAX_HONORSCHOOL)	return NULL;
	HonorPart* pHonorPart	= HonorTitleConfig::GetInstance().GetHPart(nPart);
	if (pHonorPart)
	{
		HonorTSchool* pHTS	= (HonorTSchool*)pHonorPart->pList->Items[nRealPos];
		if (pHTS)
			return pHTS->szName;
	}
	return NULL;
}

HonorPart* HonorTitleConfig::GetHPart( BYTE bPart )
{
	if(bPart >= m_pListSchool->Count)
		return NULL;
	return (HonorPart*)m_pListSchool->Items[bPart];
}

int HonorTitleConfig::GetPartByPos( BYTE bPos ,OUT int* nOutPos)
{
	for (int i = 0; i < m_pListSchool->Count;i++)
	{
		HonorPart* pHP	= (HonorPart*)m_pListSchool->Items[i];
		if(bPos < pHP->pList->Count)
		{
			if(nOutPos)
				(*nOutPos)	= bPos;
			return i;
		}
		else
			bPos -= pHP->pList->Count;
	}
	return -1;
}