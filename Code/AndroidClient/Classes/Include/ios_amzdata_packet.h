#pragma once

#ifdef _ISSERVER
#include <Windows.h>
#else
#include "GlobalDefine.h"
#endif

#pragma pack (push)
#pragma pack (1)

enum 
{
	S_AMZD_INFO,
	S_AMZD_SIMPLE,
	S_AMZD_DELETE,
};

struct AMZDATA_S_AMZD_INFO_Data 
{
	BYTE	bMType;
	int		nMID;

	int		nSize;
	BYTE	pbData[1];
	
};

struct AmzSimpleData
{
	BYTE	bPos;
	int		nVal;
};

struct AMZDATA_S_AMZD_SIMPLE_Data 
{
	BYTE	bMType;
	int		nMID;

	BYTE	bCnt;
	AmzSimpleData	pSData[1];
};

struct AMZDATA_S_AMZD_DELETE_Data 
{
	BYTE	bMType;
	int		nMID;
};


#pragma pack (pop)