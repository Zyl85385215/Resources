#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif
#include <map>
#include <list>
using namespace std;
const int nUserMaxLv = 100;
const int nUserMaxJob = 4;
const int nUserMaxAtt = 9;

class UserConfig	: public CSingleton<UserConfig>
{
public:
	UserConfig();
	~UserConfig();

	void		LoadFile(char*	szFile);
	int			nParam[nUserMaxLv][nUserMaxJob][nUserMaxAtt];
	unsigned int		nExp[nUserMaxLv];
	unsigned int	GetExp(int nLv){
		if (nLv<=0||nLv>nUserMaxLv)
		{
			return 0;
		}
		return nExp[nLv-1];
	};
private:
};

const int	nFamilyQuestTimesFlag = 1;
const int	nFamilyQuestIDFlag = 2;
const int	nFamilyQuestMaxTimes = 10;


struct ActiveItem 
{
	int nType;
	int nItem;
	int nFlag;
};

class ActiveConfig	: public CSingleton<ActiveConfig>
{
public:
	ActiveConfig();
	~ActiveConfig();

	void		LoadFile(char*	szFile);
	ActiveItem*	GetItem(int nID);
	map<int,ActiveItem*>	m_mapData;
private:
};


struct WarRankItem 
{
	int nPos;
	int nItem;
	int nCnt;
	int nMoney;
};

class WarRankItemConfig	: public CSingleton<WarRankItemConfig>
{
public:
	WarRankItemConfig();
	~WarRankItemConfig();

	void		LoadFile(char*	szFile);
	void	GetItem(int nPre,int nNow,list<WarRankItem*>&	m_List);
	list<WarRankItem*>	m_wrList;
private:
};