#pragma once

#include "singleton.h"
#include <map.h>


class AdvExpConfig	: public CSingleton<AdvExpConfig>
{
public:
	void	LoadFile(char* szFile);
	WORD	GetNeedBall(BYTE bLv);
private:
	std::map<BYTE,WORD>		mapLvBall;
};