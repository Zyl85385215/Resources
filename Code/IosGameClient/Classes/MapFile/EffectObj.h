#pragma once

#include "xnlist.h"
#include "singleton.h"
#include "GlobalDefine.h"
//配置文件设定的光效信息

class EffectObj
{
public:
	EffectObj()	{m_szSound	= NULL;}
	~EffectObj();
	int		m_nEffID;			//配置ID, 可能针对同样图片做变色处理
	int		m_nRealSprID;		//实际effect中的ID
	int		m_nX,m_nY;			//中心点
	BYTE	byR,byG,byB;
	int		m_nHeight;
	float	m_nTimeFrm;
	int		m_nRunType;
	int		m_nLoopTime;
	int		m_nZorder;
	char*	m_szSound;	
	int		m_nScale;
	BYTE	m_byScaleBySize;		//叠加玩家影子放大倍率
	BYTE	m_bPosMark;
};

class EffectCenter	: public CSingleton<EffectCenter>
{
public:
	EffectCenter();
	~EffectCenter();

	void		InitFile(char* szFile);

	EffectObj*	GetEffObj(int nID);

	xnList*		m_pList;
};