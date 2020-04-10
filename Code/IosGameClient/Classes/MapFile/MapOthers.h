#pragma once

#include "cocos2d.h"
#include "xnlist.h"
#include "GlobalDefine.h"

class MapBarrier
{
public:
	MapBarrier();
	~MapBarrier();

	void	InitMapBarrier(char* szFile);
	bool	GetBlock(int x, int y);
	//最近的不是障碍的点
	bool	CheckLineBlock(int x1, int y1 ,int x2, int y2,int * pX=0,int * pY=0);
	int		m_nWidth, m_nHeight; 
	char*	m_pData;

	
};

//处理提交的寻路请求,保存寻路结果
class _TransPoint;
class MapFindPath
{
public:
	MapFindPath();
	~MapFindPath();

	void	InitBarrier(MapBarrier* pBarrier);
	bool	RequestFindPath(int nPosFX,int nPosFY, int nPosTX, int nPosTY,xnList* pLRtlPts);

	_TransPoint*	popTransPoint(int nMapIndex);

	BYTE	m_bLeadCmd;		//当前的自动寻路命令 , 找NPC,怪物?
	DWORD	m_dwLeadParam;
	DWORD	m_dwLeadParam2;
	xnList*		m_pPathRtl;
	xnList*		m_pMapTransPt;
	int			m_nPosMTPIndex;
	int			m_nPosMTPX;
	int			m_nPosMTPY;

	MapBarrier* m_pBarrier;
private:
	bool	_findPath(POINT ptFMin, POINT ptTMin,POINT ptRctLT,POINT ptRctRB);
	void	_initBlock(int * szMark,int nSX,int nSY, int nW,int nH);
	bool	_doubleAx(int * sznMark,int nX1, int nY1, int nX2, int nY2, int nW, int nH, int& nOutFx, int& nOutTx);
};