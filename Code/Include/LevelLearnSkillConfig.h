#pragma once

#include <windows.h>
#include "singleton.h"
#include "xnlist.h"

#define SAFE_DELETE(p) { if(p) { delete (p); (p) = 0; } }

struct LevelLearnSkillData 
{
	int nLevel;
	DWORD dwSkillId[3];
};

class CLevelLearnSkillConfig : public CSingleton<CLevelLearnSkillConfig>
{
public:
	xnList * m_pList[4];
	xnList * m_pFullList[4];
	CLevelLearnSkillConfig(void);
	~CLevelLearnSkillConfig(void);

	bool		Load(const char * filename);
	LevelLearnSkillData * GetDataOnLevel(int nLevel,BYTE bJob);
};