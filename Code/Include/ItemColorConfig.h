#pragma once

#include "singleton.h"
#include <vector>
#include <windows.h>

#define SAFE_DELETE(p) { if ( p ) { delete ( p ); ( p ) = NULL; } }

#define ITEM_COLOR_NAME_LEN 20
#define ITEM_COLOR_SPR_PATH_LEN 64
#define ITEM_COLOR_ACT_PATH_LEN 64

struct ItemColorInfo
{
	int		m_nQuality;
#ifdef _CLIENT
	BYTE	m_byR, m_byG, m_byB;
	char	m_szName[ITEM_COLOR_NAME_LEN];
	char	m_szSpr[ITEM_COLOR_SPR_PATH_LEN];
	char	m_szAct[ITEM_COLOR_ACT_PATH_LEN];
#endif
};

class CItemColorConfig : public CSingleton<CItemColorConfig>
{
public:
	CItemColorConfig(void);
	~CItemColorConfig(void);

	bool load(const char * szFile);
	inline ItemColorInfo & getItemColorInfo(BYTE byItemColorIndex) { assert(byItemColorIndex < m_nCount); return *m_vecItemColorInfo[byItemColorIndex]; }
	inline int getItemColorCount(void) { return m_nCount; }

private:
	std::vector<ItemColorInfo*> m_vecItemColorInfo;
	int m_nCount;
};