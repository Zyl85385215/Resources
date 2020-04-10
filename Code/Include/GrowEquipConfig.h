#pragma once

#include <windows.h>
#include "singleton.h"
#include "xnlist.h"

#define ATTRIB_MAX 45
#define SAFE_DELETE(p) { if(p) { delete (p); (p) = 0; } }

struct GrowEquipData
{
	GrowEquipData();
	int nItemID;
	int nLevel;
	WORD wAttri[ATTRIB_MAX];
};

class GrowEquipList : public CSingleton<GrowEquipList>
{
public:
	GrowEquipList();
	~GrowEquipList();
	GrowEquipData * GetNextGrowData(int nItemID,int nLevel);
	bool Load(const char * filename);
	GrowEquipData * GetPrevGrowData( int nItemID ,int nLevel);
	GrowEquipData * GetGrowData(int nItemID);
	GrowEquipData * GetFixedGrowData(int nItemID, int nLevel);
	xnList * m_pList;
};