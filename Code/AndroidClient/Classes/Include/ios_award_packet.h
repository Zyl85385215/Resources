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
	S_SEND_ONLINEINFO,
	C_REQUEST_ONLINEGIFT,
	C_REQUEST_RECHARGEGIFT,
	C_REQUEST_LVUPGIFT,
	S_SEND_MOUNTHINFO,
	C_REQUEST_MOUNTH_SIGN,
	C_REQUEST_MOUNTH_GIFT,
	C_REQUEST_LOGINGIFT,
	C_REQUEST_RECHARGEGIFT_EX,
};

struct RPGAWARD_C_REQUEST_MOUNTH_GIFT_Data
{
	int nPos;
};

struct RPGAWARD_C_REQUEST_MOUNTH_SIGN_Data
{
	bool ifToday;
};

struct RPGAWARD_S_SEND_MOUNTHINFO_Data
{
	int nStart;
	int nYear;
	int nMounth;
	int nDay;
};

struct RPGAWARD_C_REQUEST_RECHARGEGIFT_EX_Data
{
	int nPos;
};

struct RPGAWARD_C_REQUEST_LVUPGIFT_Data
{
	int nPos;
};

struct RPGAWARD_C_REQUEST_LOGINGIFT_Data
{
	int nPos;
};

struct RPGAWARD_S_SEND_ONLINEINFO_Data
{
	BYTE byFlag;
	DWORD dwSec;
};

struct RPGAWARD_C_REQUEST_ONLINEGIFT_Data
{
	BYTE byPos;
};

#pragma pack (pop)