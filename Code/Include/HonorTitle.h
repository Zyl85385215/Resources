#pragma once

#include "singleton.h"
#include "xnlist.h"
#include <Windows.h>

class HonorTLv
{
public:
	char	szName[32];
	int		nSVal;
};

class HonorTSchool
{
public:
	char	szName[32];
	WORD	wTitle[6];
	char	szTxt[256];
};

class HonorPart
{
public:
	HonorPart()
	{
		pList	= xnList::Create();
	}
	~HonorPart()
	{
		while(pList->Count)
			delete (HonorTSchool*)pList->Delete(0);
		pList->Free();
	}
	char	szPart[32];
	char	szTxt[255];
	xnList*	pList;
};

class HonorTitleConfig:public CSingleton<HonorTitleConfig>
{
public:
	HonorTitleConfig();
	~HonorTitleConfig();
	void		Load(char* szFile);

	char*		GetLvName(int nVal);
	char*		GetLvlName(BYTE blvl);
	WORD		GetTitleBySchool(BYTE bPos,int nVal);
	HonorTLv*	InitLvlInfo(IN int nVal, OUT DWORD& nCurOut, OUT DWORD& nMaxOut);
	HonorPart*	GetHPart(BYTE bPart);
	int			GetPartByPos(BYTE bPos,OUT int* nOutPos=NULL);
	int			GetLvByVal(int nVal);
	char*       GetHonorName(BYTE bPos);
	int         GetHonorLvlNum() {return m_pListLv->Count;}
	int			GetMaxVal() {return m_nMaxVal;}
private:

	int			m_nMaxVal;
	xnList*		m_pListLv;
	xnList*		m_pListSchool;
};