#pragma once

#include "ios_mapobj_packet.h"
#include <Windows.h>
#include "xnlist.h"
//Ѫ����Ϣ,λ�ñ����Ϣ���ⲿ����
//����ͼ��,��Ծ�����������߷���


class ZPacket;
class MapSObject
{
public:
	MapSObject();
	~MapSObject();

	BYTE	byType;
	BYTE	byLv;
	BYTE	bySex;
	BYTE	byJob;
	BYTE	byCountry;
	BYTE	byViplv;
	DWORD	dwID;
	int		nPosX;		//��ʼ�������MAPserver�᲻�ϸ������λ����Ϣ
	int		nPosY;
	bool	bLockPos;		//��߳�����Ծ�¼�����������仯, �ȴ�mapserver����
	bool	bParamChged;		//ս���������ĺ���֪ͨ
	bool	bSkinChged;
	bool	bSkillChged;
	bool	bTitleChged;
	bool	bVipChged;
	bool	bMarryTitleChgd;
	bool	bMarryEffChgd;
	MParam	xParam;
	char	szName[16];
	char	szTitle[22];
	char	szMarryTitle[32];
	int		nMarryEff;
	WORD	wSkin[3];
	
	bool	IsDead()	{return (xParam.nHp<=0);}
	void	SetSkill(WORD wSkill, BYTE bLv, BYTE bPos,DWORD	dwCDTick);
	void	ResetSkill();
	void	ChangeTitle(char* pTitle);
	void	ChangeMarryTitle(char* pTitle);
	void	ChangeMarryEff(int nID);
	void	ChangeViplv(int nLv);
	xnList*	pListSkill;
	xnList*	pListState;
};

class MObjSMark
{
public:
	DWORD	dwID;
	int		nPosX;
	int		nPosY;
};

//��ҿ����µ�ս������
class MapSrvObj;
class UserData;
class MapServerObj;
class UserMObjCenter
{
public:
	UserMObjCenter(UserData* pUser);
	~UserMObjCenter();

	void	ChgWeather(int nw,int nSec);
	void	SetMyCtrlLock(bool ifLock,bool ifHide,BYTE byType,int nID);
	void	SetEnemyRunPath(int nID,int nCnt,POINT* szPoint);
	void	SetEnemyRunGroup(int nID,int nPos,int nGroup);

	void	Update25();
	void	Update();

	void	InitPlayer(DWORD dwUin,char* szName, DWORD wMapIndex,WORD wX, WORD wY, BYTE bJoy, BYTE bSex);

	void	ObjChgPos(MapSObject* pMObj);
	void	AddState(MapSObject* pMObj,WORD wState,bool bRemove);
	bool	CheckState(MapSObject* pMObj,WORD wState);
	void	ObjEnterMap(MapSObject* pMObj);
	void	ObjLeaveMap(MapSObject* pMObj);
	void	InitCtrl();
	void	LockMapAtk(DWORD	dwMIndex, BYTE bLock);
	void	SetCtrl(MapSObject* pMObj,BYTE bCtrl);
	MObjSMark*	SetCtrl(DWORD	dwID, BYTE bType,BYTE bCtrl, bool bMark	= false);
	void	OffCtrl();

	void	ClearMarks();
	bool	RemoveMark(DWORD	dwID);
	MObjSMark*	GetMark(DWORD	dwID);

	void	SetMapIndex(DWORD wMIndex,DWORD wRMIndex);
	

	bool	PacketProcess(ZPacket* pPacket);

	MapSObject*		m_pPlayer;
	MapSObject*		m_pSlave;
	xnList*			m_pListMark;

	DWORD			m_dwKey;
	DWORD			m_dwUin;
	DWORD			m_dwLockPos;
	UserData*		m_pUser;

	WORD			m_wFuryVal;
	void			HealthHpMp(MapSObject* pObj,int nHp, int nMp,bool bRevive=false);
	MapSrvObj*		GetMapSrvObj(){return m_pNetSrvLink;}

	void	        ClearRideLock(MapSObject* pMObj);

	void	        ReSetMapObjSkin(MapSObject* pMObj);
private:
	DWORD			m_dwMapIndex;
	MapSrvObj*		m_pNetSrvLink;

	void			_syncObjParams(MapSObject* pObj);
	void			_syncObjSkill(MapSObject* pObj);
	void			_syncObjTitle(MapSObject* pObj);
	void			_syncObjViplv(MapSObject* pObj);
	void			_syncObjState(MapSObject* pObj);
	void			_syncObjMarryTitle(MapSObject* pObj);
	void			_syncObjMarryEff(MapSObject* pObj);

	void			_sendMapPacket();
	
};