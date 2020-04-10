#pragma once

#include "mapcopy_define.h"

#pragma pack (push)
#pragma pack (1)

enum EMAP_COPY
{
	MAP_COPY_REQUEST_GET_INSTANCE_STATUS,
	MAP_COPY_RESPONSE_GET_INSTANCE_STATUS,
	MAP_COPY_REQUEST_CREATE_INSTANCE,
	MAP_COPY_RESPONSE_CREATE_INSTANCE,
	MAP_COPY_REQUEST_INIT_INSTANCE,
	MAP_COPY_RESPONSE_INIT_INSTANCE,
	MAP_COPY_REQUEST_JOIN_INSTANCE,
	MAP_COPY_RESPONSE_JOIN_INSTANCE,
	MAP_COPY_OPEN_MENU,
	MAP_COPY_CLOSE_MENU,
	MAP_COPY_USER_MAX,
};

typedef struct MAP_COPY_MAP_COPY_REQUEST_GET_INSTANCE_STATUS_Data
{
	int nMapCopyId;
}MAP_COPY_MAP_COPY_REQUEST_CREATE_INSTANCE_Data,
MAP_COPY_MAP_COPY_REQUEST_INIT_INSTANCE_Data,
MAP_COPY_MAP_COPY_REQUEST_JOIN_INSTANCE_Data;

struct MAP_COPY_MAP_COPY_RESPONSE_GET_INSTANCE_STATUS_Data
{
	int nMapCopyId;
	int nPlanSize;
	int pnPlanData[1];
};

typedef struct MAP_COPY_MAP_COPY_RESPONSE_CREATE_INSTANCE_Data
{
	int nMapCopyId;
	int nResult;
}MAP_COPY_MAP_COPY_RESPONSE_INIT_INSTANCE_Data,
MAP_COPY_MAP_COPY_RESPONSE_JOIN_INSTANCE_Data;

struct MAP_COPY_MAP_COPY_OPEN_MENU_Data
{
	int nMapCopyId;
};

#pragma pack (pop)