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
	BYTE		bWorkTimeRm;		//ʣ�ഥ������
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

	//����������Է����仯ʱ����
	void		ResetBaseParam(MParam* pMP);
	void		FillParam(MParam* pMP);

	WORD		GetViewSkin(BYTE bPos);
	void		DealDamager(int &nVal);

	std::map<WORD,StateObj*>	m_mapStates;


	MapAIObj*	m_pParent;

	WORD		m_szwStateSkin[9][3];
	int			m_sznLockAction[8];			//1��ѣ, 2�޵�, 3�����ƶ�, 4 ���ܼ���, 5 ���ܹ���, 6 ������Ʒ  7 ��������	8����
	bool		m_ifUnhurt;		//GM�޵�
	bool		m_ifUnvisable;	//GM����
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


	//�ͻ��˵���Ϣ���
	std::map<WORD,BYTE>			m_mapSClient;
	void		_syncClient(WORD	wState, BYTE bLayer);
	void		_syncControl(StateObj* pState, bool bRemove);
};