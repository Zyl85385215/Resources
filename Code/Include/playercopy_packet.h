#pragma once
#include <Windows.h>
#include "player_packet.h"

#define MAX_PLAYER_COPY_RANK 1000

enum
{
	COPY_BATTLE_RLT_OK,
	COPY_BATTLE_RLT_INBATTLE,
	COPY_BATTLE_RLT_NOT_IN10,
	COPY_BATTLE_RLT_YOU_HAVE_NO_COPY,
	COPY_BATTLE_RLT_ERROR,
};

struct ObjParamOnCenter
{
	int		wHp;
	int		wMp;
	int		wMaxHp;
	int		wMaxMp;
	int		wAtkW;			//Îï¹¥
	int		wAtkN;			//·¨¹¥
	int		wDefW;			//Îï·À
	int		wDefN;			//·¨·À
	int		wApN;			//ÃüÖÐ
	int		wDpW;			//¶ã±Ü

	int		wPpN;			//ÖØ»÷
	int		wPpW;			//ÖÂÃü
	int		wAtkSpeedPer;
	int		wMoveSpeedPer;	
	int		bAtkSpeed;
	int		bMoveSpeed;	

	// ÎåÐÐÊôÐÔ
	int		nJin;
	int		nMu;	
	int		nShui;
	int		nHuo;
	int		nTu;

	int		nCriticalAtk;		//ÖÂÃü¹¥»÷
	int		nCriticalDef;		//ÖÂÃü·ÀÓù

	int		nParry;				//ÕÐ¼Ü

	int		nMinHp;
	int		nMinMp;

	int		nPhysicDamage;
	int		nPhysicDamagePer;

	int		nGetPhysicDamage;
	int		nGetPhysicDamagePer;

	int		nMagicDamage;
	int		nMagicDamagePer;

	int		nGetMagicDamage;
	int		nGetMagicDamagePer;

	int		nCureEffect;
	int		nCureEffectPer;

	int		nGetCureEffect;
	int		nGetCureEffectPer;

	int		nPVPPhysicDamagePer;
	int		nPVPMagicDamagePer;

	int		nAtkYY;
	int		nDefYY;
};

struct SimplePlayerCopyData 
{
	DWORD	m_dwUin;
	char	m_szName[MAX_NAMELEN];
	BYTE	m_bJob;
	BYTE	m_bLevel;
	bool	m_bIsInBattle;
	BYTE	m_bSex;
};

struct SimpleSlaveCopyData 
{
	DWORD	m_dwMasterUin;
	char	m_szName[MAX_NAMELEN];
	char	m_szMasterName[MAX_NAMELEN];
	BYTE	m_bLevel;
	bool	m_bIsInBattle;
	WORD	m_wMonId;
};

struct SlaveCopyData 
{
	DWORD	m_dwMasterUin;
	ObjParamOnCenter m_Param;
	WORD	wMonId;
	WORD	m_wSkillID[MAX_PLAYER_COPY_SKILL];
	char	m_szMasterName[MAX_NAMELEN];
	char	m_szName[MAX_NAMELEN];
	BYTE	m_bLevel;
	bool	m_bIsInBattle;
	WORD	m_wRankIndex;
	SlaveCopyData()
	{
		m_wRankIndex = 0;
		m_bIsInBattle = false;
		m_bLevel = 0;
		wMonId = 0;
	}
};

struct PlayerCopyData 
{
	DWORD m_dwUin;
	ObjParamOnCenter m_Param;
	BYTE	skin[SKIN_MAX];	
	WORD	m_wSkillID[MAX_PLAYER_COPY_SKILL];
	char	m_szName[MAX_NAMELEN];
	BYTE	m_bJob;
	BYTE	m_bLevel;
	BYTE	bRecoverType;
	BYTE	m_bSex;
	bool	m_bIsInBattle;
	int		m_wRankIndex;
	int		m_nLastIndex;
	int     m_nFisrtOpen;
	PlayerCopyData()
	{
		m_nLastIndex = 0;
		m_wRankIndex = 0;
		m_bIsInBattle = false;
		m_bJob = 0;
		m_bLevel = 0;
		m_bSex = 0;
		m_nFisrtOpen = 1;
	}
};

enum
{
	REQUEST_OPEN_COPY_MENU,
	REQUEST_CLOSE_COPY_MENU,
	REQUEST_START_COPY_BATTLE,
	SEND_ALL_COPY_DATA,
	SEND_COPY_END_BATTLE,
	SEND_ADD_COPY_RANK,
	SEND_COPY_BATTLE_STATE,
	SEND_OPPONENT_DATA,

	REQUEST_OPEN_SLAVECOPY_MENU,
	REQUEST_CLOSE_SLAVECOPY_MENU,
	REQUEST_START_SLAVECOPY_BATTLE,
	SEND_ALL_SLAVECOPY_DATA,
	SEND_SLAVECOPY_END_BATTLE,
	SEND_ADD_SLAVECOPY_RANK,
	SEND_SLAVECOPY_BATTLE_STATE,
	SEND_SLAVEOPPONENT_DATA,

	SEND_COPY_BATTLE_RESULT,
	REQUEST_GET_COPY_AWARD,
	REQUEST_GET_RANK_AWARD,

	REQUEST_GET_FIRSTOPEN,
	SEND_FIRSTOPEN,
};

struct PLAYER_COPY_SEND_OPPONENT_DATA_Data 
{
	PlayerCopyData pOppData;
};

struct PLAYER_COPY_SEND_SLAVEOPPONENT_DATA_Data
{
	SlaveCopyData pOppData;
};

struct PLAYER_COPY_SEND_ADD_COPY_RANK_Data 
{
	SimplePlayerCopyData pNewData;
};

struct PLAYER_COPY_SEND_ADD_SLAVECOPY_RANK_Data 
{
	SimpleSlaveCopyData pNewData;
};

struct PLAYER_COPY_SEND_COPY_BATTLE_STATE_Data 
{
	DWORD dwUin1;
	DWORD dwUin2;
	bool bInBattle1;
	bool bInBattle2;
};

struct PLAYER_COPY_SEND_SLAVECOPY_BATTLE_STATE_Data 
{
	DWORD dwUin1;
	DWORD dwUin2;
	bool bInBattle1;
	bool bInBattle2;
};

struct PLAYER_COPY_REQUEST_START_COPY_BATTLE_Data 
{
	DWORD dwPlayerUin;
	DWORD dwCopyUin;
};

struct PLAYER_COPY_REQUEST_START_SLAVECOPY_BATTLE_Data 
{
	DWORD dwPlayerUin;
	DWORD dwCopyUin;
};

struct PLAYER_COPY_SEND_ALL_COPY_DATA_Data 
{
	bool bOpen;
	int nCount;
	int nSelfLastIndex;
	SimplePlayerCopyData pCopyData[1];
};

struct PLAYER_COPY_SEND_ALL_SLAVECOPY_DATA_Data 
{
	bool bOpen;
	int nCount;
	SimpleSlaveCopyData pCopyData[1];
};

struct PLAYER_COPY_SEND_COPY_END_BATTLE_Data 
{
	DWORD dwWinUin;
	DWORD dwLoseUin;
	SimplePlayerCopyData pWinData;
};

struct PLAYER_COPY_SEND_SLAVECOPY_END_BATTLE_Data 
{
	DWORD dwWinUin;
	DWORD dwLoseUin;
	SimpleSlaveCopyData pWinData;
};

struct PLAYER_COPY_SEND_COPY_BATTLE_RESULT_Data 
{
	BYTE byAward;
};
struct PLAYER_COPY_SEND_FIRSTOPEN_Data 
{
	int firstopen;
};