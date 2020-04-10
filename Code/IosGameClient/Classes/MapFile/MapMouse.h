#pragma once

#include "MapControl.h"
/*----------------------------------------------------------------------
设定玩家移动
选择目标


------------------------------------------------------------------------*/
//地图上的控制事件经过坐标转换后转接到这里
class MapEventCenter
{
public:
	MapEventCenter();
	~MapEventCenter();

	void	MapEventMove(int nX, int nY);
	bool	MapEventDown(int nX, int nY);
	void	MapEventUp(int nX, int nY);

	void	Update();

	int		m_nClickX;
	int		m_nClickY;
	//bool	m_bClickHold;	//手指一直点住
	DWORD	m_dwTickMove;	//上次移动指令
};

class MapAIObj;
class _BaseGuidePt;
class MiniMapMenu;
class MapCommand
{
	friend class MiniMapMenu;
public:
	MapCommand();
	~MapCommand();

	void	Reset(bool bMapTrans = false);

	bool	CmdPathObj(_BaseGuidePt* pGC);
	bool	CmdMapPoint(int nMIndex, int nPosX, int nPosY, _BaseGuidePt* pGC = NULL);
	bool	CmdPoint(int nPosX, int nPosY,bool bReset = true, bool bFromAuto = false);
	bool	CmdNormalAtk(MapAIObj*	pObj,bool bClearSkill = true);
	bool	CmdSkillAtk(MapAIObj*	pObj,WORD wSID);
	//当前地图的下个移动点,跳地图失败就不继续跑了
	bool	PopNextPoint(int nMap, POINT& pt);

	bool	IsInRest();	//空闲状态
	bool	IsMoving();
	//不移动的时候就判断有没攻击目标
	MapAIObj*	PopAtkCmd(int& nSkill);

	MapAIObj*	m_pObjNAtk;		//等级目标

	//int		m_nTrackCmd;	//-1表示开挂机, 其他 是NPC对话
	_BaseGuidePt*	pGuideCmd;
	WORD	m_wTrySkill;
	bool	m_bFromAuto;
	int		m_nLastPX;
	int		m_nLastPY;
	int		m_nLastMIndex;	
protected:
	int		m_nPathMIndex;	//当前寻路结果地图

	bool	m_bReCmd;		//指令重新下达
	xnList*	m_pLPtPath;		//寻路节点
	xnList*	m_pLMapTrans;	//多张地图间的跳转点
};

//手势识别中心,通过move事件调用触发, down,up事件清除
class HandSignCmd
{
public:
	HandSignCmd();
	~HandSignCmd();

	BYTE	AddPtCheck(int nX, int nY);		//0无, 1→  2←  3↑  4↓ 5／ 6\	7 5反,8	6反
	void	ClearPt();

private:
	BYTE	_AnalyPtList();
	bool	_checkPts(BYTE bType);
	BYTE	_checkPt(int nSX, int nSY, int nEX, int nEY);
	DWORD		dwTickCmd;
	xnList*		pListPt;
};

extern	HandSignCmd*	g_pHandSign;
extern	MapCommand*	g_pMapCommand;