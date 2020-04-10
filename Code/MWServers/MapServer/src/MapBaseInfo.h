#pragma once

#include "singleton.h"
#include "xnlist.h"
#include <Windows.h>

#define BLOCKING_SIZE 16

class MapBarrier
{
public:
	MapBarrier();
	~MapBarrier();

	void	InitMapBarrier(char* szFile);
	bool	GetBlock(int x, int y);
	bool	CheckLineBlock(int x1, int y1 ,int x2, int y2,int * pX=0,int * pY=0);
	int		m_nWidth, m_nHeight; 
	char*	m_pData;

};
class MRActionCenter;
class MapCfgObj
{
public:
	MapCfgObj();
	~MapCfgObj();
	MapBarrier*	m_pMapBarr;
	
	WORD		m_wMapIndex;
	WORD		m_wMapBase;
	WORD		m_wMapSizeW;
	WORD		m_wMapSizeH;
	WORD		m_wReviveX;
	WORD		m_wReviveY;
	BYTE		m_byFType;
	BYTE		RandGetWeather();
	xnList*		pListWeather;
};

class MapConfig	: public CSingleton<MapConfig>
{
public:
	MapConfig();
	~MapConfig();

	void		LoadFile(char*	szFile);
	MapCfgObj*	GetMapCfgObj(WORD wMIndex);

	xnList*		m_pList;
};