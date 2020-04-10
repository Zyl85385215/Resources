#pragma once

#include <Windows.h>
#include "xnList.h"
/*------------------------------------------------------------------------------------
ֻ�Ǽǹ��������, ������ˢ��ʱ���ά��
����ָ����������ٵĵط�
�߼��������ⲿʵ��, ָ��AI����
------------------------------------------------------------------------------------*/
class MapManager;
class MapAIObj;
//����ˢ�µ���Ϣ
class BornPointInfo
{
public:
	BornPointInfo(WORD wMapW,WORD wMapH);
	~BornPointInfo();

	void	GetRandPoint(int& nX, int& nY,MapManager* pParent,BYTE& byStep);
	void	CheckBGroupCmd(MapAIObj* pObj,MapManager* pMap);
	//void	GetBattleGPos(int& nX, int& nY,int nDestX, int nDestY, int nDir, int nPos);
	//����ˢ��ģ��
	WORD	wPosX;		//ˢ�µ�
	WORD	wPosY;
	WORD	wActiveNumMin;		//����˿�
	WORD	wActiveNumMax;		//����˿�
	DWORD	dwBornTickMin;		//ˢ��ʱ����
	DWORD	dwBornTickMax;
	WORD	wEBaseID;		//����Ĺ�������,���о����ɻ���ģ��ָ��
	WORD	wRange;			//ˢ������
	WORD	wExPow;			//�ٷֱ�ǿ��ϵ��
	BYTE	bMaxPlayer;
	BYTE	bForce;			//����

	//��Ϊ����ģ��
	WORD	wRangeRun;		//����ƶ�����
	WORD	wRangeAtk;		//���׷������
	
	BYTE	bPtRunCnt;		//Ϊ0��ʾ�̶�λ�������
	bool	bLoopRun;		//ѭ���ƶ�
	POINT*	szRunPts;

	//����ʱ����
	xnList*	pListEnemys;	//��ǰ������
	WORD	wActiveNum;		//����˿�
	DWORD	dwBornTick;		//ˢ��ʱ����
	xnList*	pListDeadTick;	//��������ʱ��

	WORD	wMaxWidth;
	WORD	wMaxHeight;

	BYTE	byBattleGroup;	//����, 0 ��  1 ����, 2 ����
};

//ˢ�¿���ģ��,�ռ�ˢ�µ���������
class BornCenter
{
public:
	BornCenter(WORD	wRMIndex,MapManager* pParent);
	~BornCenter();

	void	CheckGSSync();
	void	UpdateSec();

	//����false��ʾ��������Ѵ���
	bool	GSBornMapObj(MapAIObj* pObj);

	MapAIObj*	GetMapObj(BYTE bType, DWORD dwID);
	void	CheckMObjList();

	int		GetPlayerCnt()	{return m_pListGSPlayer->Count;}
private:
	void	_initNpc();		//��ʼ��NPC,���ص�ͼ��ʱ������
	void	_initBorn(WORD wRMIndex);
	void	_updateBorn();		//���������

	MapAIObj*	_findMObj(DWORD dwID, xnList* pList);
	void		_checkMObj(xnList* pList);

	xnList*		m_pListBornPts;		//BornPointInfo
	xnList*		m_pListRobot;			//NPC�б�
	xnList*		m_pListGSPlayer;	//��Ϸ���������ӵĶ���, �����,����,���
	xnList*		m_pListGSSlave;
	xnList*		m_pListGSEnemy;
	xnList*		m_pListCache;
	MapManager*	m_pParent;
	DWORD		m_dwUpdateCnt;
};