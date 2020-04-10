#pragma once
#include <Windows.h>

#define	ARENA_GROUP_MEM_MAX		10
#define PLAYER_NAME_LEN 16

enum  
{	
	ARENA_OPEN,
	ARENA_GROUP_LIST,
	ARENA_ADD_GROUP,
	ARENA_DEL_GROUP,
	ARENA_ADD_MEMBER,
	ARENA_DEL_MEMBER,
	ARENA_GROUP_BATTLE_STATE_CHG,
	ARENA_MEMBER_STATE_CHG,
	ARENA_GROUP_INFO,
	ARENA_GROUP_MEM_POS_CHG,
	ARENA_GROUP_MEM_POS_STATE_CHG,


	ARENA_REQUEST_ADD_GROUP,
	ARENA_REQUEST_JOIN_GROUP,
	ARENA_REQUEST_LEAVE_GROUP,
	ARENA_REQUEST_LEAVE_SYSTEM,
	ARENA_REQUEST_SET_READY,
	ARENA_REQUEST_START,
	ARENA_REQUEST_SET_POS_STATE,

	ARENA_RESULT,
	ARENA_MAX,
};

enum
{
	ARENA_MEMBER_STATE_FREE,
	ARENA_MEMBER_STATE_READY,
	ARENA_MEMBER_STATE_INMAP,
};

struct CLIENT_ARENA_MEMBER 
{
	DWORD m_dwUin;
	BYTE m_bState;
	bool bOpen;
	char szName[PLAYER_NAME_LEN];
	BYTE m_bHeadSkin;
};

struct CLIENT_ARENA_GROUP
{
	CLIENT_ARENA_GROUP()
	{
		for (int i=0;i<ARENA_GROUP_MEM_MAX;i++)
		{
			m_Members[i].m_bState = ARENA_MEMBER_STATE_FREE;
			m_Members[i].m_dwUin = 0;
			m_Members[i].bOpen = true;
			m_Members[i].m_bHeadSkin = 0;
			m_Members[i].szName[0] = '\0';
		}
	}
	CLIENT_ARENA_MEMBER m_Members[ARENA_GROUP_MEM_MAX];
	DWORD m_dwGroupID;
	bool  m_bInBattle;
	BYTE bMinLvl;
	BYTE bMaxLvl;
	BYTE bMode;
};

struct ARENA_ARENA_REQUEST_ADD_GROUP_Data 
{
	BYTE bMinLvl;
	BYTE bMaxLvl;
	BYTE bMode;
	char szName[PLAYER_NAME_LEN];
	BYTE m_bHeadSkin;
};

struct ARENA_ARENA_GROUP_LIST_Data
{
	int nCount;
	CLIENT_ARENA_GROUP Group[1];
};

struct ARENA_ARENA_ADD_GROUP_Data
{
	DWORD m_dwGroupID;
	BYTE bMinLvl;
	BYTE bMaxLvl;
	BYTE bMode;
	CLIENT_ARENA_MEMBER m_MOwner;
};

struct ARENA_ARENA_DEL_GROUP_Data
{
	DWORD dwGroupID;
};

struct ARENA_ARENA_ADD_MEMBER_Data
{
	DWORD dwGroupID;
	DWORD dwUin;
	BYTE byPos;
	char szName[PLAYER_NAME_LEN];
	BYTE m_bHeadSkin;
};

struct ARENA_ARENA_REQUEST_LEAVE_GROUP_Data 
{
	DWORD dwUin;
};

struct ARENA_ARENA_DEL_MEMBER_Data
{
	DWORD dwGroupID;
	DWORD dwUin;
};

struct ARENA_ARENA_GROUP_BATTLE_STATE_CHG_Data
{
	DWORD dwGroupID;
	bool bIsInBattle;
};

struct ARENA_ARENA_MEMBER_STATE_CHG_Data
{
	DWORD dwUin;
	BYTE bState;
};

struct ARENA_ARENA_GROUP_INFO_Data 
{
	DWORD dwGroupID;
	CLIENT_ARENA_MEMBER m_Member[ARENA_GROUP_MEM_MAX]; 
};

struct ARENA_ARENA_GROUP_MEM_POS_CHG_Data 
{
	DWORD dwUin;
	BYTE bPos;
};

struct ARENA_ARENA_REQUEST_JOIN_GROUP_Data 
{
	DWORD dwGroupID;
	BYTE bPos;
	char szName[PLAYER_NAME_LEN];
	BYTE m_bHeadSkin;
};

struct ARENA_ARENA_REQUEST_START_Data  
{
	DWORD dwGroupID;
	BYTE bMode;
};

struct ARENA_ARENA_REQUEST_SET_POS_STATE_Data 
{
	DWORD dwGroupID;
	BYTE bPos;
	bool bOpen;
};

struct ARENA_ARENA_GROUP_MEM_POS_STATE_CHG_Data 
{
	DWORD dwGroupID;
	BYTE bPos;
	bool bOpen;
};

struct ARENA_ARENA_REQUEST_SET_READY_Data 
{
	bool bReady;
};

struct ARENA_ARENA_RESULT_Data 
{
	DWORD dwUin[ARENA_GROUP_MEM_MAX];
	char szName[ARENA_GROUP_MEM_MAX][PLAYER_NAME_LEN];
	bool bWin[ARENA_GROUP_MEM_MAX];
	int	nScore[ARENA_GROUP_MEM_MAX];
	int nWinBattle[ARENA_GROUP_MEM_MAX];
	int nTotalBattle[ARENA_GROUP_MEM_MAX];
};