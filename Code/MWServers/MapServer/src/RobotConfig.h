#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif
#include "ios_mapobj_packet.h"

class RobotObj
{
public:
	RobotObj();
	~RobotObj();

	WORD	wID;
	BYTE	bLv;
	BYTE	bJob;
	BYTE	bSex;
	WORD	wSkin[3];
	xnList*	pLSkill;
	MParam	xParam;
};

class RobotConfig	: public CSingleton<RobotConfig>
{
public:
	RobotConfig();
	~RobotConfig();

	void	LoadFile(char*	szFile);
	RobotObj*	GetRobotByID(WORD	wID);

	xnList*		m_pListRobot;
};


class MRActionPt
{
public:
	MRActionPt();
	~MRActionPt();
	int		nID;
	int		nX;
	int		nY;
	WORD	bForce;
	xnList*	pLActPt;
	xnList*	pLRobot;
};

class MRActionCenter
{
public:
	MRActionCenter(int	nBaseMIndex);
	~MRActionCenter();

	MRActionPt*	GetActPt(int nID);

	xnList*		m_pListActPts;
};