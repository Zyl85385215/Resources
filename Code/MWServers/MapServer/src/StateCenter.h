#pragma once

#include "StateConfig.h"
#include <map>
#include "ios_mapobj_packet.h"

class StateObj
{
public:
	StateObj()	{dwFromID = 0;}
	StateBase*	pSBase;
	BYTE		bLv;
	BYTE		bWorkTimeRm;		//剩余触发次数
	DWORD		dwTickStart;

	DWORD		dwFromID;
	BYTE		byFromType;
};

class MapAIObj;
class StateCenter
{
public:
	StateCenter(MapAIObj* pObj);
	~StateCenter();

	void		Update();

	StateObj*	AddState(WORD wState,BYTE bType = 0,DWORD dwUin = 0);
	StateObj*	SetState(WORD wState,BYTE bLv, DWORD dwTickRm);
	StateObj*	GetState(WORD	wState);
	
	bool		RemoveState(WORD wState);

	//人物基本属性发生变化时调用
	void		ResetBaseParam(MParam* pMP);
	void		FillParam(MParam* pMP);

	WORD		GetViewSkin(BYTE bPos);
	void		DealDamager(int &nVal);

	std::map<WORD,StateObj*>	m_mapStates;


	MapAIObj*	m_pParent;

	WORD		m_szwStateSkin[9][3];
	int			m_sznLockAction[8];			//1晕眩, 2无敌, 3不能移动, 4 不能技能, 5 不能攻击, 6 不能物品  7 不能骑马	8隐身
	bool		m_ifUnhurt;		//GM无敌
	bool		m_ifUnvisable;	//GM隐身
	bool		m_bSkinChged;
	MParam		m_xStateParam;

	MParam		m_xBaseParam;
	bool		m_bParamChged;
	int			m_nShildVal;
	int			m_nRemoveStateMark;
	WORD		m_wShildState;

	int			FillClientStates(xnList*	pList);
	void		CleanRideLockState();
private:
	StateObj*	_getStateByPos(WORD	wPos);
	bool		_removeStateByPos(WORD wPos);
	StateObj*	_addState(StateBase* pBase,BYTE bType,DWORD dwUin);
	StateObj*	_setState(StateBase* pBase,BYTE bLv, DWORD dwRmTick);
	bool		_addStateLayer(StateObj* pState,BYTE bType,DWORD dwUin);
	void		_stateWork(StateBase* pBase, BYTE bLv,bool bBack,bool bRecal=false,bool	bNew	= true,BYTE bType = 0,DWORD dwUin = 0);


	//客户端的信息情况
	std::map<WORD,BYTE>			m_mapSClient;
	void		_syncClient(WORD	wState, BYTE bLayer);
	void		_syncControl(StateObj* pState, bool bRemove);
};