#pragma once

#include <windows.h>
#include "singleton.h"
#include "xnlist.h"

#define SAFE_DELETE(p) { if(p) { delete (p); (p) = 0; } }

struct LevelUpTalentData
{
	int nLevel;
	int nTalent;
};

class CLevelUpTalentConfig : public CSingleton<CLevelUpTalentConfig>
{
public:
	CLevelUpTalentConfig(void);
	~CLevelUpTalentConfig(void);

	bool		Load(const char * filename);
	int			GetTalent(int nLevel);
	int			GetNextLevel(int nLevel);
	int			GetAllTalentOnLevel(int nLevel);
private:
	xnList *	m_pList;
};