#pragma once

#include "groupCmd.h"

enum 
{
	OPEN_GROW_MENU,
	REQUEST_START_GROW,
	SEND_GROW_RESULT,
};

struct ITEM_GROW_REQUEST_START_GROW_Data 
{
	int nGrowEquipIndex;
	int nGrowStoneIndex;
};

struct ITEM_GROW_SEND_GROW_RESULT_Data 
{
	bool bResult;
};