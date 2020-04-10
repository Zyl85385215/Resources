#pragma once

#include <windows.h>
#include "singleton.h"
#include "xnlist.h"

#define SAFE_DELETE(p) { if(p) { delete (p); (p) = 0; } }

struct MapPVEConfigData 
{
	int nMapIndex;
	BYTE bIsPVE;
};

class CMapPVEConfig : public CSingleton<CMapPVEConfig>
{
public:
	xnList * m_pList;
	CMapPVEConfig(void);
	~CMapPVEConfig(void);

	bool		Load(const char * filename);
	BYTE		GetIsPVE(int nMapIndex);
};