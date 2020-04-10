#pragma once

#pragma pack (push)
#pragma pack (1)

enum
{
	//�ͻ��˵�����
	TSYS_GETTEAMS,		//GetFullTST(int uin, OUT TSysTeam** ppTST, int nCnt, int nStartIndex = 0 )
	TSYS_GETPLAYERS,		//GetFullTSP(int uin, OUT TSysPlayer** ppTSP, int nCnt, int nStartIndex = 0 )
	TSYS_GETTEAMEX,
	TSYS_GETPLAYEREX,
	TSYS_REQUEST,		//GetTSTeam(int nIndex)
	TSYS_INVITE,		//GetTSPlayer(int nIndex)

	TSYS_ADDTST,		//AddTSTeam(int uin, BYTE byMax, BYTE byCur, WORD wInfo, BYTE* szLimit, int* szMember, char* szTxt)
	TSYS_ADDTSP,		//AddTSPlayer(int uin, WORD wInfo)

	TSYS_CHGTST,
	TSYS_CHGTSP,

	TSYS_CLEARTST,
	TSYS_CLEARTSP,

	//��Ϸ������������
	//TSYS_CHGTSTMEM,
	//TSYS_ADDTEAM_LEAD,	//ȥ��������Ϣ
	TSYS_ADDTEAM,		//ȥ��������Ϣ,ȥ����ļ��Ϣ
	TSYS_CHGLEAD,		//�ӳ��ı�,�޸�UIN
	TSYS_LEAVETEAM,		//���һ���뿪����
	TSYS_LEAVEGAME,		//ȥ��������Ϣ,ȥ����ļ��Ϣ

	//���ĵ���Ϸ������
	TSYS_RTL_REQUEST,	//InviteSucceed(int uin, int nObjUin);��Ϸ�������¼�������޶�����Ա������ӵĹ���
	TSYS_RTL_INVITE,	//RequestSucceed(int uin, int nObjUin);

	//���ĵ��ͻ���
	TSYS_SEND_TST,
	TSYS_SEND_TSP,

	TSYS_SEND_TSTEX,
	TSYS_SEND_TSPEX,
};

class PacktTSysT
{
public:
	int			nIndex;		//ʶ���
	BYTE		byLimit;	//��������
	BYTE		byCurNum;	//��ǰ����
	WORD		wInfo;		//��ӵ�ͼ���
	int			nUin;		//�ӳ�ID
	BYTE		byJobLvLimit[8];		//0
	BYTE		bLineIndex;
	char		szName[16];
	
};

class PacktTSysP
{
public:
	int			nIndex;
	WORD		wInfo;		//��ӵ�ͼ���
	int			nUin;		//���ID
	BYTE		bLineIndex;
	char		szName[16];
};

struct	TEAM_SYSTEM_TSYS_ADDTST_Data
{
	BYTE	byMax;
	BYTE	byCur;
	WORD	wInfo;
	BYTE	byLimit[8];
	int		szMember[6];
	char	szTxt[1];
};

struct	TEAM_SYSTEM_TSYS_ADDTSP_Data
{
	WORD	wInfo;
	char	szTxt[1];
};

struct	TEAM_SYSTEM_TSYS_CHGTST_Data
{
	BYTE	byMax;
	WORD	wInfo;
	BYTE	byLimit[8];
	char	szTxt[1];
};
struct	TEAM_SYSTEM_TSYS_CLEARTST_Data
{
	int		nIndex;
};
struct	TEAM_SYSTEM_TSYS_CLEARTSP_Data
{
	int		nIndex;
};

struct	TEAM_SYSTEM_TSYS_CHGTSP_Data
{
	int		nIndex;
	WORD	wInfo;
	char	szTxt[1];
};
struct	TEAM_SYSTEM_TSYS_CHGTSTMEM_Data
{
	int		nMemUin;
	BYTE	byJoin;
};
struct	TEAM_SYSTEM_TSYS_ADDTEAM_Data
{
	int		nLeadID;
};
struct	TEAM_SYSTEM_TSYS_LEAVETEAM_Data
{
	int		nLeadID;
};

struct	TEAM_SYSTEM_TSYS_CHGLEAD_Data
{
	int		nNewUin;
};

struct	TEAM_SYSTEM_TSYS_GETTEAMS_Data
{
	int		nCnt;
	int		nStartIndex;
};

struct	TEAM_SYSTEM_TSYS_GETPLAYERS_Data
{
	int		nCnt;
	int		nStartIndex;
};

struct	TEAM_SYSTEM_TSYS_GETTEAMEX_Data
{
	int		nIndex;
};

struct	TEAM_SYSTEM_TSYS_GETPLAYEREX_Data
{
	int		nIndex;
};

struct	TEAM_SYSTEM_TSYS_REQUEST_Data
{
	int		nIndex;
};

struct	TEAM_SYSTEM_TSYS_INVITE_Data
{
	int		nIndex;
};

struct	TEAM_SYSTEM_TSYS_SEND_TST_Data
{
	WORD		wCnt;
	PacktTSysT	tst[1];
};

struct	TEAM_SYSTEM_TSYS_SEND_TSP_Data
{
	WORD		wCnt;
	PacktTSysP	tsp[1];
};

struct	TEAM_SYSTEM_TSYS_SEND_TSTEX_Data
{
	int			nIndex;
	char		szInfo[1];
};

struct	TEAM_SYSTEM_TSYS_SEND_TSPEX_Data
{
	int			nIndex;
	char		szInfo[1];
};

struct	TEAM_SYSTEM_TSYS_RTL_REQUEST_Data
{
	int		nUinFrom;	//������(����)
	int		nUinTo;		//��ѯ����(����,����ѯ��)
};

struct	TEAM_SYSTEM_TSYS_RTL_INVITE_Data
{
	int		nUinFrom;	//������(��Ա/��)
	int		nUinTo;		//��������(����,������ʾ)
};



#pragma pack (pop)