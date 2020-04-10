#pragma once
#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif


struct ExpMulObj
{
	int nID;
	char* szName;
	int nAddPer;			//加成倍率，百分比，可填负数
	bool ifDisOnline;		//是否离线消失
	bool ifDisDie;			//是否死亡消失
	bool ifTimeAdd;			//是否时间叠加
	int nTime;				//持续时间，-1表示无限
	BYTE byTargent;			//加成对象，0人，1宠物
	int nFamily;			//族群
};

class ExpMultipleCfg	: public CSingleton<ExpMultipleCfg>
{
public:
	ExpMultipleCfg();
	~ExpMultipleCfg();
	void	LoadFile(char* pFile);
	ExpMulObj* GetObj(int nID);

	xnList	*m_pList;
};