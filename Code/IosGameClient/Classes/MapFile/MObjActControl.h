#pragma once

#include "xnlist.h"
#include "singleton.h"

/*--------------------------------------------------------
控制地图对象的播放帧变更

会动的怪的数据收集中心
上层数据通过该类投影到地图层
--------------------------------------------------------*/
class   MapAIObj;
class	MapSprObj;
class	MapObj;

class	MObjActCtrl
{
public:
	MObjActCtrl();
	~MObjActCtrl();

	//调用后生成MapSprObj, 并运行维护
	bool	LinkCtrl(MapAIObj* pMObj);
	//对象删除的时候进入该模块删除对象
	bool	DisLinkCtrl(MapObj* pMObj);
	void	ResetObj();
	void	GetInViewObj(int nPosX, int nPosY, int nEX, int nEY,xnList* pLOut);
	void	Update();

	MapSprObj*	CheckSelObj(int nX, int nY);
	MapSprObj*	SearchObj(int nX, int nY,BYTE bEnemy,DWORD	nMinDistance, DWORD dwQusEnemyID = 0);
	MapSprObj*	GetObjByID(BYTE	byType, int	nID);

	xnList*	m_pListAllMObj;
	xnList*	m_pListCache;
private:
	void	_checkPlaySound(BYTE bType, BYTE bSex,BYTE bJob, BYTE bAct, BYTE bFrm,char*	szSkinSound = NULL);
};