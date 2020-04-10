#pragma once

#ifdef _ISSERVER
#include <Windows.h>
#else
#include "GlobalDefine.h"
#endif
#include "ItemConfig.h"

#pragma pack (push)
#pragma pack (1)

#define MAX_RANK_VIEW	20
#define MAX_RANK_SIZE	50

enum RANKTYPE                     //���а�����
{
	RANK_LEVEL,//�ȼ�
	RANK_MONEY,//�Ƹ�
	RANK_ATTACK,//ս��
	RANK_HOME,//����
	RANK_EQUIP,//װ��
	RANK_HORNOUR,//����
	RANK_DAMAGE,//��ħ
	RANK_MAX,
};

//����Ԫ��
class Rank_Data
{
public:
	DWORD   m_dwID1;
	DWORD	m_dwID2;
	DWORD	m_dwCheck1;
	DWORD	m_dwCheck2;
	char	m_szDesc[4][20];	//��Ӧ�ͻ��˽��� desc1~4
	DWORD	m_dwVal[4];		//��Ӧ�ͻ��˽��� val1~4
	int		m_nPos;
	RANKTYPE m_byType;
};

struct RankUserInfo
{
	DWORD dwUin;
	char szName[20];
	int	nLv;
	BYTE byJob;
	int nScore;
	int nSkin[3];
	char pData[EQUIP_MAX][128];
	BYTE szSize[EQUIP_MAX];
};

class SvRankUserInfo	: public RankUserInfo
{
public:
	bool ifInfoChg;
	bool ifEquipChg[EQUIP_MAX];
};

struct RankUWatchInfo
{
	DWORD dwUin;
	DWORD dwid;
	BYTE byType;		//0װ��
	BYTE byState;
	char pData[128];
	BYTE szSize;
};

enum
{
	S_SEND_RANKLIST,
	S_SEND_RANKCHANGE,
	S_SEND_MYRANK,
	C_REQUEST_RANKINFO,
	S_SEND_RANKINFO_RLT,
	C_REQUEST_GET_USERINFO,
	S_SEND_USERINFO,
	C_REQUEST_GET_WATCHDATA,
	S_SEND_WATCHDATA,
	S_LAST_DAMAGE,
};

struct RPGRANK_S_SEND_WATCHDATA_Data
{
	RankUWatchInfo xInfo;
};

struct RPGRANK_C_REQUEST_GET_WATCHDATA_Data
{
	int nUin;
	int nID;
	BYTE byType;
};

struct RPGRANK_S_SEND_USERINFO_Data
{
	RankUserInfo xInfo;
};

struct RPGRANK_C_REQUEST_GET_USERINFO_Data
{
	int nUin;
};

struct RPGRANK_C_REQUEST_RANKINFO_Data
{
	DWORD dwRankFlag[RANK_MAX];
};

struct RPGRANK_S_SEND_RANKINFO_RLT_Data
{
	DWORD dwRankFlag[RANK_MAX];
};

struct RPGRANK_S_SEND_RANKLIST_Data
{
	int nCnt;
	Rank_Data szData[1];
};
struct RPGRANK_S_LAST_DAMAGE_Data
{
	Rank_Data szData;
};
struct RPGRANK_S_SEND_RANKCHANGE_Data
{
	int nCnt;
	Rank_Data szData[1];
};

struct RPGRANK_S_SEND_MYRANK_Data
{
	Rank_Data xData;
};

#pragma pack (pop)