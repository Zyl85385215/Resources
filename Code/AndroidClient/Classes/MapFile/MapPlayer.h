#pragma once

#include "MapAIObj.h"

class MapPlayer	: public MapAIObj
{
public:
	MapPlayer();
	virtual ~MapPlayer();

	BYTE	byJob;
	BYTE	bySex;
};