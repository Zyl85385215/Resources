#pragma once

#include "singleton.h"
#include "xnlist.h"
#include "GlobalDefine.h"
#include "EffectObj.h"
#include "SkinConfig.h"
#include "ios_mapobj_packet.h"

enum
{
	OBJ_STATE_STAND,
	OBJ_STATE_RUN,
	OBJ_STATE_ATK,
	OBJ_STATE_SKILL,
	//OBJ_STATE_HURT,		//受击动作只在客户端显示
	
	OBJ_STATE_MAX,
};

//地图物件对象(无生命,地图光效类),无目标范围光效, 飞行光效可采用这类对象
class	MapObj
{
public:
	MapObj();
	virtual ~MapObj();


	//增加一个光效,先进列表, 持续循环光效列表比对消除,非循环光效直接播放一次就OK了
	bool		AddEffect(EffectObj* pEff);
	void		SetAlpha(BYTE byVal);

	int			m_nID;			
	BYTE		m_byType;		//0, 无生命物体, 1 人物, 2宠物, 3怪物, 4,NPC
	DWORD		m_nMIndex;
	int			nPosX;
	int			nPosY;
};