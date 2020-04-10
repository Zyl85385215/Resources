#pragma once

#include <windows.h>
#include "groupcmd.h"

class SearchGroupData
{
public:
	DWORD dwUin;
	char szName[20];
	int nLevel;
	int nMinLevel;
	int nMaxLevel;
	BYTE nNum;
	BYTE nType;
};

enum 
{
	REQUEST_OPEN_SEARCH,
	REQUEST_CLOSE_SEARCH,
	REQUEST_ADD_SEARCH,
	REQUEST_DEL_SEARCH,
	REQUEST_TEAM_SEARCH_GROUP,
	SEND_ALL_SEARCH_DATA,
	SEND_ADD_SEARCH,
	SEND_DEL_SEARCH,
	SEND_SEARCH_NUM,
};

struct SEARCH_GROUP_REQUEST_ADD_SEARCH_Data
{
	int nType;
	int nMinLevel;
	int nMaxLevel;
};

struct SEARCH_GROUP_REQUEST_TEAM_SEARCH_GROUP_Data
{
	DWORD dwUin;
};

struct SEARCH_GROUP_REQUEST_INVITE_Data
{
	DWORD dwUin;
};

struct SEARCH_GROUP_SEND_ALL_SEARCH_DATA_Data
{
	bool bOpen;
	int nCount;
	SearchGroupData pDatas[1];
};

struct SEARCH_GROUP_SEND_ADD_SEARCH_Data
{
	SearchGroupData pData;
};

struct SEARCH_GROUP_SEND_DEL_SEARCH_Data
{
	DWORD dwUin;
};

struct SEARCH_GROUP_SEND_SEARCH_NUM_Data
{
	DWORD dwUin;
	int nNum;
};