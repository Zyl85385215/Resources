#pragma once

#include <windows.h>
#include "singleton.h"
#include "xnlist.h"
#include "slave_packet.h"

#define SAFE_DELETE(p) { if(p) { delete (p); (p) = 0; } }

struct BabyLevelupConfig
{
	WORD	m_wSlaveIndex;
	BYTE	m_bLevel;
	WORD	m_wNewSlaveIndex;
};

class CBabyLevelupConfigList : public CSingleton<CBabyLevelupConfigList>
{
public:
	CBabyLevelupConfigList(void);
	~CBabyLevelupConfigList(void);

	bool		Load(const char * filename);
	bool		IsSpecialSlave	(SlaveDataEx & xData);
	WORD		CheckCanLevelup	(SlaveDataEx & xData);

private:
	xnList *	m_pList;
};