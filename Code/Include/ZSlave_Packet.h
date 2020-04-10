#pragma once

#include <windows.h>
#include "zslave_define.h"

#pragma pack (push)
#pragma pack (1)

enum ESLAVE_CMD
{
	SLAVE_CMD_REQUEST_TO_ACTIVE,		//C
	SLAVE_CMD_RESPONCE_TO_ACTIVE,		//S
	SLAVE_CMD_REQUEST_TO_INACTIVE,		//C
	SLAVE_CMD_RESPONCE_TO_INACTIVE,		//S
	SLAVE_CMD_REQUEST_TO_FREE,			//C
	SLAVE_CMD_RESPONCE_TO_FREE,			//S

	SLAVE_CMD_COMMAND_TO_MOVE,			//C
	SLAVE_CMD_COMMAND_TO_FLY,			//C
	SLAVE_CMD_COMMAND_TO_ATTACK,		//C
	SLAVE_CMD_COMMAND_TO_USESKILL,		//C

	S_ADDSLAVE,
	S_FIGHTPARAM,
	S_SLAVEATTRIB,
	S_LVEXP,
	S_POWLVEXP,
	S_CHGID,

	SLAVE_CMD_MAX,
};

enum ESLAVE_ACTIVE_CODE
{
	SLAVE_ACTIVE_CODE_SUCCESSFUL,
	SLAVE_ACTIVE_CODE_FAILED,
};

enum ESLAVE_INACTIVE_CODE
{
	SLAVE_INACTIVE_CODE_SUCCESSFUL,
	SLAVE_INACTIVE_CODE_FAILED,
};

enum ESLAVE_FREE_CODE
{
	SLAVE_FREE_CODE_SUCCESSFUL,
	SLAVE_FREE_CODE_FAILED,
};

struct ZSLAVE_S_LVEXP_Data
{
	LONGLONG		llSlaveKey;
	BYTE			byLv;
	DWORD			dwExp;
};

struct ZSLAVE_S_CHGID_Data
{
	LONGLONG		llSlaveKey;
	DWORD			dwUin;
};

struct ZSLAVE_S_POWLVEXP_Data
{
	LONGLONG		llSlaveKey;
	BYTE			byPowLv;
	DWORD			dwPowExp;
};

struct ZSLAVE_S_SLAVEATTRIB_Data
{
	LONGLONG		llSlaveKey;
	SlaveAttrib		sAttrib;
	WORD			wPoint;
};

struct ZSLAVE_S_ADDSLAVE_Data
{
	CZylSlaveData	slaveData;
};

struct ZSLAVE_S_FIGHTPARAM_Data
{
	LONGLONG		llSlaveKey;

	int				nMaxHP;
	int				nMaxMP;
	int				nAtkW;
	int				nAtkN;
	int				nCriticalAtk;
	int				nCriticalDef;
	int				nPpW;
	int				nDefW;
	int				nDefN;

	int				nApN;
	int				nDpW;
	int				nPpN;
	int				nHp;
	int				nMp;
};


struct ZSLAVE_SLAVE_CMD_REQUEST_TO_ACTIVE_Data
{
	LONGLONG llSlaveKey;
};

struct ZSLAVE_SLAVE_CMD_RESPONCE_TO_ACTIVE_Data
{
	ESLAVE_ACTIVE_CODE eCode;
	LONGLONG		llSlaveKey;
};

struct ZSLAVE_SLAVE_CMD_REQUEST_TO_INACTIVE_Data
{};

struct ZSLAVE_SLAVE_CMD_RESPONCE_TO_INACTIVE_Data
{
	ESLAVE_INACTIVE_CODE eCode;
	LONGLONG		llSlaveKey;
};

struct ZSLAVE_SLAVE_CMD_REQUEST_TO_FREE_Data
{
	LONGLONG llSlaveKey;
};

struct ZSLAVE_SLAVE_CMD_RESPONCE_TO_FREE_Data
{
	ESLAVE_FREE_CODE eCode;
	LONGLONG llSlaveKey;
};

struct ZSLAVE_SLAVE_CMD_COMMAND_TO_MOVE_Data
{
	POINT ptPos;
};

struct ZSLAVE_SLAVE_CMD_COMMAND_TO_FLY_Data
{};

struct ZSLAVE_SLAVE_CMD_COMMAND_TO_ATTACK_Data
{
	int nId;
	BYTE byType;
};

struct ZSLAVE_SLAVE_CMD_COMMAND_TO_USESKILL_Data
{
	int nPos;
	DWORD dwTarget;
};

#pragma pack (pop)