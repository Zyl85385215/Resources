#pragma once


#include <Windows.h>
#include "ZNetPacket.h"
#include "xnlist.h"

#define CTRLVIEW_WIDTH		12
#define CTRLVIEW_HEIGHT		8

class MapAIObj;
class MapManager;
class ControlObj
{
public:
	ControlObj(DWORD	dwK);
	~ControlObj();

	//0取消控制, 1指令型控制, 10, 操作控制 11, 视野控制(10,11只能选其一)
	void	SetCtrl(BYTE bControl, MapAIObj* pObj);

	void	RecvPacket(ZPacket* pPacket);
	void	SendPacket(ZPacket* pPacket);

	void	Update();

	void	ChgSP(int nSp);
	void	StrikeSkill(WORD wSkill);

	DWORD	m_dwKey;
	DWORD	m_dwUin;
	int		m_nMIndex;		//为0表示未初始化
	WORD	m_wPX,m_wPY;	//原先所处块
	WORD	m_bForce;
	int		m_nSpVal;
	
	MapManager*	m_pRunMap;		//当前所在地图
	MapAIObj*	m_pMainAIObj;	//主要控制单位,视角锁定


	DWORD		m_dwDamageTotal;
	DWORD		m_dwHealthTotal;
	DWORD		m_dwHitTotal;

	bool	AddCtrlChild(MapAIObj* pObj);
	bool	RemoveCtrlChild(MapAIObj* pObj);
	xnList*		m_pListChild;	//副本助战, 镖车, 任务NPC等

	//这边可过滤是否不显示其他玩家,NPC,怪物,宠物之类的
	bool	GetCtrlAICommand(MapAIObj* pObj);
private:
	void	_setMainCtrl(MapAIObj* pObj, BYTE bCtrl);
};