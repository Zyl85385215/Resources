#include "stdafx.h"
#ifdef _ISSERVER
#include "stdio.h"
#endif
#include "TextConfig.h"

#include "xnExtstr.h"

CreateSingleton(RandNameConfig);
CreateSingleton(LoadingTxtConfig);


RandNameConfig::RandNameConfig()
{
	m_pNameList[0]	= xnList::Create ();
	m_pNameList[1]	= xnList::Create ();
	m_pNameList[2]	= xnList::Create ();
	m_pNameList[3]	= xnList::Create ();
	m_pListFb		= xnList::Create();
}

RandNameConfig::~RandNameConfig()
{
	for(int i = 0; i < 4; i ++)
	{
		if(m_pNameList[i])
		{
			while(m_pNameList[i]->Count)
				free(m_pNameList[i]->Delete(0));
			m_pNameList[i]->Free();
		}
	}
	while(m_pListFb->Count)
		free(m_pListFb->Delete(0));
	m_pListFb->Free();
}

void RandNameConfig::LoadFile( char* szFile )
{
	for(int i = 0; i < 4; i ++)
	{
		if(m_pNameList[i])
		{
			while(m_pNameList[i]->Count)
				free(m_pNameList[i]->Delete(0));
		}
	}

	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;

	int index = 0;
	while (pStr->SetSection(index) != NULL)
	{
		int		nCnt	= atoi (pStr->ReadString ("cnt","0"));

		char	szTmp[32];
		for (int i = 0; i < nCnt; i++)
		{
			sprintf(szTmp,"%d",i+1);
			char* szStr	= strdup(pStr->ReadString(szTmp,""));

			m_pNameList[index]->Add(szStr);
		}
		index++;
	}
	pStr->Free();
}

char * RandNameConfig::GetRndName(int nSex)
{
	static char	s_szRtlName[16];

	sprintf(s_szRtlName,"%s",(char*)m_pNameList[nSex?1:0]->Items[rand ()%m_pNameList[nSex?1:0]->Count]);
	sprintf(s_szRtlName,"%s%s",s_szRtlName,(char*)m_pNameList[2]->Items[rand ()%m_pNameList[2]->Count]);
	sprintf(s_szRtlName,"%s%s",s_szRtlName,(char*)m_pNameList[3]->Items[rand ()%m_pNameList[3]->Count]);
	return s_szRtlName;
}

char * RandNameConfig::GetRndName( int nSex,BYTE bR1,BYTE bR2,BYTE bR3 )
{
	static char	s_szRtlName[16];

	sprintf(s_szRtlName,"%s",(char*)m_pNameList[nSex?1:0]->Items[bR1%m_pNameList[nSex?1:0]->Count]);
	sprintf(s_szRtlName,"%s%s",s_szRtlName,(char*)m_pNameList[2]->Items[bR2%m_pNameList[2]->Count]);
	sprintf(s_szRtlName,"%s%s",s_szRtlName,(char*)m_pNameList[3]->Items[bR3%m_pNameList[3]->Count]);
	return s_szRtlName;
}

void RandNameConfig::LoadFBFile( char* szFile )
{
	while(m_pListFb->Count)
		free(m_pListFb->Delete(0));

	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	if(pStr->SetSection(0) != NULL)
	{
		int		nCnt	= atoi (pStr->ReadString ("Cnt","0"));

		char	szTmp[32];
		for (int i = 0; i < nCnt; i++)
		{
			sprintf(szTmp,"%d",i+1);
			char* szStr	= strdup(pStr->ReadString(szTmp,""));

			m_pListFb->Add(szStr);
		}
	}
	pStr->Free();
}

bool RandNameConfig::CheckForbid( char* szStr )
{
	bool bRtl	= false;
	std::string strMsg(szStr);
	for (int i = 0; i < m_pListFb->Count; i++)
	{
		char* szCheck	= (char*)m_pListFb->Items[i];
		int nLen	= strlen (szCheck);
		if(nLen == 0)
			continue;
		int nPos = strMsg.find(szCheck,0);
		if(nPos == std::string::npos)
			continue;
		int nCount = 0;
		for(int k = 0; k <nPos; k ++)
		{
			if(strMsg[k] < 0 || strMsg[k] > 0x80)
				nCount ++;
		}
		if(nCount%2 == 1)
			continue;
		for(int k = nPos; k < nPos+nLen; k ++)
		{
			strMsg[k] = '*';
			szStr[k] = '*';
		}
		bRtl = true;
		i--;
	}
	return bRtl;
}

LoadingTxtConfig::LoadingTxtConfig()
{
	m_pList	= xnList::Create();
}

LoadingTxtConfig::~LoadingTxtConfig()
{
	while(m_pList->Count)
		free(m_pList->Delete(0));
	m_pList->Free();
}

void LoadingTxtConfig::LoadFile( char* szFile )
{
	while(m_pList->Count)
		free(m_pList->Delete(0));

	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	if(pStr->SetSection(0) != NULL)
	{
		int		nCnt	= atoi (pStr->ReadString ("cnt","0"));
		char	szTmp[256];
		for (int i = 0; i < nCnt; i++)
		{
			sprintf(szTmp,"%d",i+1);
			char* szStr	= strdup(pStr->ReadString(szTmp,""));

			m_pList->Add(szStr);
		}
	}
	pStr->Free();
}

char* LoadingTxtConfig::GetLoadingTxt()
{
	if (m_pList->Count < 1)
		return "";

	int nVal = 1+ rand()%m_pList->Count;
	if(nVal > 0&& nVal < m_pList->Count+1)
		return (char*)m_pList->Items[nVal-1];

	return "";
}