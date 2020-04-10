#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif



class RandNameConfig	: public CSingleton<RandNameConfig>
{
public:
	RandNameConfig();
	~RandNameConfig();

	void	LoadFile(char* szFile);
	void	LoadFBFile(char* szFile);
	char *	GetRndName(int nSex);
	char *	GetRndName(int nSex,BYTE bR1,BYTE bR2,BYTE bR3);
	bool	CheckForbid(char*  szStr);
private:
	xnList* m_pNameList[4];
	xnList*	m_pListFb;
};

class LoadingTxtConfig	: public CSingleton<LoadingTxtConfig>
{
public:
	LoadingTxtConfig();
	~LoadingTxtConfig();

	void	LoadFile(char* szFile);
	char *	GetLoadingTxt();
private:
	xnList*	m_pList;
};