#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif

class StateEffVal
{
public:
	BYTE	bType;	//作用属性当前血量, 最大血量,当前蓝量, 最大蓝量, 物攻, 魔攻, 物防, 魔防, 命中,闪避,暴击,移动速度
	int		nPer;	//万分比
	int		nSpVal;	//数值  万分比非0的时候表示 当前血量, 最大血量,当前蓝量, 最大蓝量, 物攻, 魔攻, 物防, 魔防, 命中,闪避,暴击
};

class StateBase
{
public:
	StateBase();
	~StateBase();
	
	WORD	wStateID;
	WORD	wWorkPos;
	BYTE	bWorkVal;

	//客户端版本
	char*	szName;
	char*	szDesc;
	WORD	wIcon;
	WORD	wEff;
	WORD	wEffex;
	
	BYTE	bWorkTime;	//作用次数
	DWORD	dwPerTick;	//每次持续时间
	
	xnList*	pListStateEff;
	BYTE	szbSpEff[8];	//1晕眩, 2无敌, 3不能移动, 4 不能技能, 5 不能攻击, 6 不能物品，7隐身

	BYTE	bLayerStart;
	BYTE	bLayerMax;
		
	WORD	wChgSkin;
	WORD	wChgHorse;
	WORD	wChgWeapon;
	BYTE	bSkinPos;
		
	BYTE	bDisType;		//0无, 1 移动, 2攻击, 3死亡, 4被攻击, 5使用物品
	BYTE	bOffLineDis;	//下线是否消失
	BYTE	bSyncClient;

	WORD	wRemoveMark;
};

class StateConfig	: public CSingleton<StateConfig>
{
public:
	StateConfig();
	~StateConfig();

	void	LoadFile(char* szFile);
	StateBase*	GetStateBase(WORD wStateID);
private:
	xnList*		m_pList;
};