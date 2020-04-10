#pragma once

#include <windows.h>
#include "singleton.h"
#include "MapBattleInfo.h"

#define SAFE_DELETE(p) { if(p) { delete (p); (p) = 0; } }

class CBattleInfoConfig : public CSingleton<CBattleInfoConfig>
{
public:
	CBattleInfoConfig(void);
	~CBattleInfoConfig(void);

	bool		Load(const char * filename);
	char *		GetSprName(int b_i_g,int b_i_o);

private:
	char *		szSprName[B_I_G_MAX][B_I_O_MAX];
};