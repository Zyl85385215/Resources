#pragma once

#include "singleton.h"
#include <map>

#define SAFE_DELETE(p) { if(p) { delete (p); (p) = 0; } }

#define MAP_COPY_NAME_LEN (64)

struct MapCopyConditionInfo
{
	int m_nMinLevel;
	int m_nMaxLevel;
	bool m_blInTeam;
};

class CMapCopyConditionConfig : public CSingleton<CMapCopyConditionConfig>
{
public:
	CMapCopyConditionConfig(void);
	~CMapCopyConditionConfig(void);

	bool load(const char * szFile);
	MapCopyConditionInfo * getConditionInfo(int nMapCopyId);

private:
	std::map<int, MapCopyConditionInfo*> m_mapConditionInfo;
};
