#pragma once


#include "DBControl.h"
#include <map>

class MarkGameDB
{
public:
	MarkGameDB();
	~MarkGameDB();

	void	CheckTimeEvent();	//启动或者并检测

	void	AddMarkCost(int nID,int nVal);	//元宝消耗量, 商城物品购买量

	void	AddQICnt(int nID,BYTE bType,int nCnt);

	void	ChgOnline(bool bNew, bool bAdd);

	int		m_nDayMark;		//日期编号

	DWORD	m_dwTick5Minute;	//5分钟判断
	bool	m_bCheckNowOnline;

private:
	int		m_nOnlineNew;		//在线新用户
	int		m_nOnlineOld;

	std::map<int,BYTE>	mapMarkDB;
	std::map<int,BYTE>	mapQIMDB;
};

extern	MarkGameDB* g_pMarkGameDB;