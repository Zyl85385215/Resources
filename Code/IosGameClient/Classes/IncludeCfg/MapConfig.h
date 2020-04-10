#pragma once

#include "singleton.h"
#include "xnlist.h"

#ifndef _ISSERVER
#include "GlobalDefine.h"
#endif

class _mapTile
{
public:
	int		nIndex;
	int		nPosX;
	int		nPosY;
	char*	szSpr;
};

class MapTileData
{
public:
	MapTileData();
	~MapTileData();

	void	InitTile();
	int		GetInViewTile(int nPosX, int nPosY,int nEX,int nEY,xnList* pList);

	WORD	wIndex;
	WORD	wRealIndex;
	BYTE	byRealMiniMap;
	int		m_nTWidth;
	int		m_nTHeight;
	int		m_nSWidth;		//������
	int		m_nSHeight;		//������
	int		m_nXCnt;
	int		m_nYCnt;

	char		m_szName[32];
	char*		m_szBackMp3;

	_mapTile**	m_ppTiles;
};


class MapViewConfig		: public CSingleton<MapViewConfig>
{
public:
	MapViewConfig();
	~MapViewConfig();

	void			LoadFile(char*	szFile);
	MapTileData*	GetMapTileData(int nIndex);

	xnList*		m_pList;
private:
};

class MapLimitData
{
public:
	MapLimitData();
	~MapLimitData();

	WORD   wMapBaseID;
	bool   bHorseLimit;          //�����������
	bool   bSlaveLimit;          //�����ٻ�����

	xnList* pItemLimitList;      //����ʹ����Ʒ
};


class MapLimitConfig		: public CSingleton<MapLimitConfig>
{
public:
	MapLimitConfig();
	~MapLimitConfig();

	void			LoadFile(char*	szFile);

	bool            CheckMapHorseLimit(WORD wMapRIndex);
	bool            CheckMapSlaveLimit(WORD wMapRIndex);
	bool            CheckMapItemULimit(WORD wMapRIndex,WORD wItemID);

	xnList*		m_pList;
private:
};