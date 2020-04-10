#pragma once

#include "MapAIObj.h"

#define MAPPLATE_SIZE	128

//地图板块,交互的枢纽
//范围判断,光环效果,地表效果, 视野判断
class ControlObj;
class ZPacket;
class MapManager;
class MapPlate
{
public:
	MapPlate();
	~MapPlate();

	//void	Update();		//重复多次调用函数造成CPU额外开支
	void	UpdateSec();

	bool	EnterPlate(MapAIObj* pObj);		//处理视野变更, 状态变更等
	bool	LeavePlate(MapAIObj* pObj,bool bDisapear = false);		//
	
	void	AddCtrlView(ControlObj* pObj);
	void	RemoveCtrlView(ControlObj* pObj);

	//关注该区块的控制模块,排除pC
	void	SendPacketOutCtrl(ZPacket* pPacket,ControlObj* pC = NULL);
	void	SendPacketExCtrl(ZPacket* pPacket,ControlObj* pC = NULL);
	//关注该区块的控制模块,排除pPlate
	void	SendOutPlatePacket(ZPacket* pPacket,MapPlate* pPlate);

	MapAIObj*	GetAtkTarget(MapAIObj* pRqer);

	void	FillAllObj(OUT xnList* pList);
	void	KillEnemy(MapAIObj* pRqer);
	void	KillEnemyByID(WORD wEnemyID);

	void	ResetMapForce();
	void	ChgPowLv(int nPow, int nLv);

	WORD	m_wMX,m_wMY;
	xnList*	m_pListEnemy;
	xnList*	m_pListSlave;
	xnList*	m_pListPlayer;
	xnList*	m_pListRobot;
	//xnList*	m_pListNpc;

	xnList*			m_pListEff;		//地表效果, 包括地形状态,光环,一秒检测一次

	xnList*			m_pListViews;		//关注该板块的控制对象   //ControlObj
	MapManager*		m_pMap;
	int				m_nRestSec;
private:
	xnList*		_getListByType(BYTE bType);
	void		_sendSyncPacketList(MapAIObj* pObj, xnList* pList);
	void		_sendOutPacketList(MapAIObj* pObj, xnList* pList);

	void		_sendSyncListPacket(xnList* pList, ControlObj* pCtrl);
	void		_sendOutListPacket(xnList* pList, ControlObj* pCtrl);

	ZPacket*	_getSyncPacket(MapAIObj* pObj, char* buf);

	void		_checkListCmd(xnList* pList);
};