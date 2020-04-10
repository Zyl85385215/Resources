#pragma once

#include "PacketCenter.h"
#include "MapPlayer.h"


class MapObjManager
{
public:
	MapObjManager();
	virtual ~MapObjManager();

	virtual	MapObj*		AddMapObj(int nID,BYTE bType);
	virtual	MapObj*		GetMapObj(int nID);
	virtual	bool		DelMapObj(int nID);
	virtual	void		ClearMapObj();
	virtual	void		Update()	{}

	xnList*		m_pList;
};

class MapAIObjManager	: public MapObjManager
{
public:
	MapAIObjManager();
	virtual ~MapAIObjManager();

	virtual	void		Update();

	virtual	MapObj*		AddMapObj(int nID,BYTE bType);
};

class MapPlayerManager	: public MapAIObjManager
{
public:
	MapPlayerManager();
	virtual ~MapPlayerManager();

	virtual	MapObj*		AddMapObj(int nID,BYTE bType);
};