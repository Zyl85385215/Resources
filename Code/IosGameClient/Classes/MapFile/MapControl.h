#pragma once

#include "MapOthers.h"
#include "GlobalDefine.h"
#include "SkinConfig.h"
#include "MapConfig.h"
#include "MapObjManagers.h"



/*---------------------------------------------------------------------
���ڿ��ƽ�����ʾ����
���ݸ�������ֵĽ���ͨ��
--------------------------------------------------------------------*/

enum
{
	OBJ_ACTION_STAND,
	OBJ_ACTION_RUN,
	OBJ_ACTION_ATK1,
	OBJ_ACTION_ATK2,
	OBJ_ACTION_HURT,
	OBJ_ACTION_HSTAND,
	OBJ_ACTION_HRUN,

	OBJ_ACTION_DEAD,
	OBJ_ACTION_MAX,
};

//��ɫ�ƶ����ⲿ���ݲ����
class MapTileData;
class MObjActCtrl;
class ToopTip9Spr;
class MapSprObj
{
public:
	MapSprObj();
	~MapSprObj();

	void	InitSkin(int nSkin);
	void	SetSkinPA(int nSkinA);
	void	SetSkinPB(int nSkinB);

	int		GetTitleH();
	void*	pMObj;

	ObjSkinInfo*	pSkin;		//Ƥ��

	ObjSkinInfo*	pSkinPA;		//����
	ObjSkinInfo*	pSkinPB;		//����

	BYTE		byType;
	BYTE		byAction;	//��ǰ����			ʵʱ����
	BYTE		byDirect;	//����,				ʵʱ����
	int			nPosX,nPosY;	//��ǰλ��		ʵʱ����
	BYTE		byFrame;	//��ǰ�ڼ�֡		ʵʱ����

	int			nHp, nMaxHp;
	char*		szName;			//ֱ��ָ��MapObj�����ֵ�ַ
	char*		szTitle;
	char*		szMarryTitle;
	BYTE*		pCountry;
	BYTE*		pVipLevel;
	xnList*		pListEffAdd;	//ֱ��ָ��MapObj���б��ַ
	xnList*		pListEffDel;
	xnList*		pListFightNum;

	//BYTE		byDelState;		//1�ⲿɾ��, 2�ڲ���ͬ��,���ͷ�ָ��	
	ToopTip9Spr*	pTalkInfo;

	DWORD		dwFrmTick;
	BYTE		byUpdateFlag;
	BYTE		byIsEnemy;

	//DWORD		m_dwParticleID;
	//void*		pSprView;
};

class MapEffObj
{
public:
	int			nEffID;
	int			nPosX,nPosY;
};

//�ռ��������ͼ���������Ҫ��Ԫ��, �ٵ�ͼ��UPDATE�����
class	MapControl;
extern	MapControl*		g_pMapCenter;
class MapControl	: public PacketListener
{
public:
	MapControl();
	~MapControl();
	static	MapControl*	GetDefault(){return g_pMapCenter;}


	virtual bool PacketProcess(ZPacket* pPacket);
	//����һ����ͼ,��Ҫ��ʼ���ܶණ��
	bool	EnterMap(int nIndex,int nBase);
	void	SetWeather(BYTE bVal);
	int		m_nWeatherID;
	int		m_nEffID;
	int		m_nWeatherSound;
	void	Update();

	int		m_nMapIndex;		//��ͼID
	int		m_nBaseMapIndex;	//����ʱ���õ�ģ���ͼID
	int		m_nX, m_nY;		//�ӽǵ����½�����λ��

	DWORD	m_dwBusyWordTick;	//��ʼ�Ż���ʱ��
	DWORD	m_dwFrmBusyTime;	//����֡��

	int		GetMapWidth()	{return m_pMapTile?m_pMapTile->m_nTWidth:0;}
	int		GetMapHeight()	{return m_pMapTile?m_pMapTile->m_nTHeight:0;}
	bool	LinkMapObj(MapAIObj* pObj);
	bool	DeLinkMapObj(MapObj* pObj);

	void	AddMapEff(int nEffID, int nPosX, int nPosY, DWORD	dwTick);
	MapAIObj*	GetMapObj(BYTE byType, DWORD dwID);
	bool		RemoveMapObj(BYTE byType, DWORD dwID);

	void	AskBornEff(MapAIObj* pObj);

	//��ͼ������ѡ��Ŀ��
	void	SearchNearObj(BYTE bEnemy,DWORD	dwMaxLen = 100000, DWORD dwQusEnemyID = 0);
	bool	CheckSelObj(int nX, int nY);

	bool	CheckDispHitNum(MapAIObj* pA, MapAIObj* pB);

	void	PlayFuryEff(int nX, int nY);
	DWORD	PlayerParticle(int nX, int nY,DWORD	dwRunTick, BYTE bType);
	void	SetParticle(int nID,int nX, int nY );

	bool	CheckLineBlock(int nX, int nY, int nEX, int nEY, bool bAtkTarget=false);

	void	AddCharNAtkEff(MapAIObj* pObjA,MapAIObj* pObjB);
	
	MapTileData*	m_pMapTile;			//�ر����
	MapBarrier*		m_pMapBarrier;		//�ϰ�ģ��
	MapBarrier*		m_pMapAlpha;
	MapFindPath*	m_pMapFindPath;		//Ѱ·ģ��

	MObjActCtrl*	m_pMapActCtrl;
	
	xnList*		m_pListTile;
	xnList*		m_pListMObj;
	xnList*		m_pListEff;		//��ͼ���ڲ��ŵĹ�Ч, ��ҵļ��ܹ�Ч(��Ҫ��Χ���ܺͷ��м���)

	xnList*		m_pLRunEffView;	//�Ѳ��ŵĵ�ͼ��Ч
	xnList*		m_pLRunEffOut;

	xnList*		m_pListMapTP;
	xnList*		m_pListMapEffStatic;
	xnList*		m_pLMapTmpEffs;		//��ͼ�ȴ����ŵĹ�Ч

	MapAIObjManager*	m_pMEnemyList;
	MapAIObjManager*	m_pMSlaveList;
	MapAIObjManager*	m_pMNpcList;
	MapPlayerManager*	m_pMPlayerList;
	
	MapAIObj*			m_pMObjLockView;	//����Ŀ��
	DWORD				m_dwLockID;
	BYTE				m_byLockType;
	BYTE				m_bySpEff;

	MapSprObj*			m_pMObjSel;			//ѡ��Ŀ��

	DWORD				m_dwTickEnter;
private:
	void				_checkEffInView(int nPosX, int nPosY,int nEX,int nEY,xnList* pList);
};
