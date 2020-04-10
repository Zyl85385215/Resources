#pragma once

#include "CtrlObj.h"

class MiniMapMenu
{
public:
	MiniMapMenu();
	~MiniMapMenu();

	void		Update();

	void		OpenBigMap();

	void		MoveMap(int nDifX, int nDifY, bool dwTick = 0);

	DForm*		m_pForm;
	DComponent*	m_pDCompRadar;
	DComponent*	m_pDCompPos;
	DComponent*	m_pDCompMName;
	DComponent*	m_pDCompMine;

	DForm*		m_pFormBig;
	DComponent*	m_pCompBigMap;
	DComponent*	m_pDCompBMMine;

	DForm*		m_pFormWorld;

	int			m_nSrcX;
	int			m_nSrcY;
	int			m_nStartX;
	int			m_nStartY;

	int			m_nMineDir;
	xnList*		m_pListRadars;
	int			m_nCurSelGuide;
	bool		m_bGuideMark;
	DWORD		m_dwTickBMapLock;
	bool		m_bOpenInit;

	void		ChgMap(int	nBase);		//当前查看的地图
	int			m_nMaxLineCnt;

	int			m_nCurBaseMIndex;
	int			m_nMaxWidth;
	int			m_nMaxHeight;

	DComponent*	m_pCTickComp[3];
private:
	void		_updateRidar();
	void		_updatePathLine();
};