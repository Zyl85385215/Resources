#pragma once

#include "singleton.h"
#include "xnlist.h"
#include "GlobalDefine.h"

class _plistResObj
{
public:
	_plistResObj();
	~_plistResObj();
	int		nMark;
	char*	szPList;

	int		nUseCnt;
	DWORD		dwTick;
};

class ResManager	: public CSingleton<ResManager>
{
public:
	ResManager();
	~ResManager();

	void	AddPList(char* szPL,int nMark,BYTE bType);
	void	SubPList(char* szPL,int nMark,BYTE bType);
	void	Update();
	void	ClearPList();
	xnList*		m_pListCache[5];
};