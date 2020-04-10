#pragma once

#include "xnlist.h"
#include <Windows.h>
#include "MapManager.h"
#include <map>

class MapList
{
public:
	MapList();
	~MapList();

	void	RecvPacket(ZPacket* pPacket, int nMapIndex);

	void	Update();
	void	UpdateSec();

	std::map<DWORD,MapManager*>	m_mapMaps;
private:
	void			_initMaps();		//≥ı ºµÿÕº
	MapManager*		_createMapCopy(int nMapIndex, int nRealMIndex);
	void			_releaseMap(int nMapIndex);

	inline MapManager*		GetMap(WORD wMIndex);
};

extern	MapList*	g_pMapList;