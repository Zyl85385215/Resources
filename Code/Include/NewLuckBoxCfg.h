#pragma once


#include "singleton.h"
#include "xnlist.h"
#include <Windows.h>
#include <map.h>

class NewLuckBoxCfg	: public CSingleton<NewLuckBoxCfg>
{
public:
	NewLuckBoxCfg();
	~NewLuckBoxCfg();

	void	LoadFile(char* szFile);
	DWORD	GetItemByLuck(int nTime ,DWORD dwUin	= 0);
	

	std::map<DWORD,int>		m_mapLuckItem;
	int						m_nLuckTotal;
};

class UinLuckObj
{
public:
	std::map<int,DWORD>		mapTimeItem;
};

class NewJinLianCfg : public CSingleton<NewJinLianCfg>
{
public:
	NewJinLianCfg();
	~NewJinLianCfg();

	void	LoadFile(char* szFile);
	UinLuckObj*	GetUinInfo(DWORD	dwUin);

	std::map<DWORD,UinLuckObj*>		m_mapUinLuck;
};