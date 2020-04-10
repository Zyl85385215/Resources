#pragma once

#include "MapAIObj.h"

#define MAPPLATE_SIZE	128

//��ͼ���,��������Ŧ
//��Χ�ж�,�⻷Ч��,�ر�Ч��, ��Ұ�ж�
class ControlObj;
class ZPacket;
class MapManager;
class MapPlate
{
public:
	MapPlate();
	~MapPlate();

	//void	Update();		//�ظ���ε��ú������CPU���⿪֧
	void	UpdateSec();

	bool	EnterPlate(MapAIObj* pObj);		//������Ұ���, ״̬�����
	bool	LeavePlate(MapAIObj* pObj,bool bDisapear = false);		//
	
	void	AddCtrlView(ControlObj* pObj);
	void	RemoveCtrlView(ControlObj* pObj);

	//��ע������Ŀ���ģ��,�ų�pC
	void	SendPacketOutCtrl(ZPacket* pPacket,ControlObj* pC = NULL);
	void	SendPacketExCtrl(ZPacket* pPacket,ControlObj* pC = NULL);
	//��ע������Ŀ���ģ��,�ų�pPlate
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

	xnList*			m_pListEff;		//�ر�Ч��, ��������״̬,�⻷,һ����һ��

	xnList*			m_pListViews;		//��ע�ð��Ŀ��ƶ���   //ControlObj
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