#pragma once

#include "xnlist.h"
#include "ZNetPacket.h"
#include <Windows.h>
#include <hash_map>
#include "ControlObject.h"

typedef std::hash_map<DWORD,ControlObj*>						HMapCtrlObj;
typedef std::hash_map<DWORD,ControlObj*>::iterator			HMapCtrlObjPtr;
/*------------------------------------------------------------------------
玩家的控制器管理中心, 主要与客户端交互, 
游戏服务器决定具体控制那个mapobj对象

------------------------------------------------------------------------*/
class ControlList
{
public:
	ControlList();
	~ControlList();
	
	ControlObj*	GetCtrl(DWORD	dwKey, bool bCreated = false);
	bool	RemoveCtrl(DWORD dwKey);

	void	RecvPacket(ZPacket* pPacket, DWORD	dwKey);

	void	SendMapPacket(ZPacket* pPacket, int nMIndex);
	void	CheckCtrlRealseMap(void* pMap);

	void	Update();
	void	UpdateSec();


	HMapCtrlObj		mapControlers;
};

extern	ControlList*	g_pCtrlList;