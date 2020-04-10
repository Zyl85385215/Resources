#pragma once

#include <windows.h>

#pragma pack (push)
#pragma pack (1)

enum BOOKSKILL_CMD
{
	S_DRIVE_INFOS,
	S_CORE_INFO,
	S_ACTIVE_BSKILL_INFO,
	C_REQUEST_CORE_UP,
	C_ACTIVE_BOOKSKILL,

	S_BSKILL_PARTS,
	S_BSKILL_PART,

	SEND_DCFAILVAL,

	C_ACTIVE_DRIVESKILL,

	S_SPDRV_INFO,
	C_ACTIVE_SPDRV,
	C_UP_SPDRV,

	SEND_BSKILL_TIME,

	S_DCQUEST_INFO,
	C_CMD_ACCEPTQ,
	C_CMD_REFRESH,
	C_CMD_COMPLETE,
};

struct BOOKSKILL_S_DRIVE_INFOS_Data {
	DWORD	dwLockRemain;
	BYTE	byPow[8][10];
	BYTE	byPrevUp;
};
struct BOOKSKILL_C_CMD_REFRESH_Data {
	BYTE bType;
};
struct BOOKSKILL_S_ACTIVE_BSKILL_INFO_Data {
	BYTE	byActPos[3];
	BYTE	bExCnt;
	WORD	wBSkills[10];
};
struct BOOKSKILL_S_CORE_INFO_Data {
	DWORD	dwLockRemain;
	//BYTE	bPos;	//个位,十位
	BYTE	bDrive;
	BYTE	bCore;
	BYTE	bPow;
	BYTE	bPrevUp;
};

struct BOOKSKILL_SEND_DCFAILVAL_Data 
{
	int	 nVal;
};

struct BOOKSKILL_SEND_BSKILL_TIME_Data 
{
	DWORD	dwRmSecond;
};

struct BOOKSKILL_C_ACTIVE_SPDRV_Data 
{
	BYTE	byID;
};

struct BOOKSKILL_C_UP_SPDRV_Data 
{
	BYTE	byID;
};


struct BOOKSKILL_S_SPDRV_INFO_Data 
{
	BYTE	byCurUse;
	DWORD	dwRmTick;
	int		nCnt;
	BYTE	byData[1];	//nCnt*2
};

struct BOOKSKILL_S_BSKILL_PART_Data {
	WORD	wSID;
	DWORD	dwItemID;
};

struct BOOKSKILL_S_DCQUEST_INFO_Data 
{
	BYTE	bRemainTm;
	BYTE	bCurLv;
};

struct BOOKSKILL_S_BSKILL_PARTS_Data {
	WORD	wSID;
	BYTE	bCnt;
	BYTE	bPart[1];
};

struct BOOKSKILL_C_REQUEST_CORE_UP_Data {
	BYTE	bDrive;
	BYTE	bCore;
};

struct BOOKSKILL_C_ACTIVE_BOOKSKILL_Data {
	WORD	wBSkill[10];
};

struct BOOKSKILL_C_ACTIVE_DRIVESKILL_Data {
	BYTE	bPos[3];
};

#pragma pack (pop)