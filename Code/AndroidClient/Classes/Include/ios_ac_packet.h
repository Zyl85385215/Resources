#pragma once

#ifndef _ISSERVER
#include "GlobalDefine.h"
#endif

#pragma pack (push)
#pragma pack (1)

enum _ACCOUNT 
{
	INIT_CLIENT,
	LOGIN,
	LOGIN_RLT,
	LOGOUT,

	SIMPLEINFOS,

	C_REQUEST_JOIN,
	S_JOINGAME,

	C_CREATE_CHARACTER,

	CHANGEPASSWORD,
	SPEED_REGIST,					//����ע��
	SPEED_REGIST_CHECK,				//����ע��ѯ��

	TPCOMM_LOGIN,
	TPCOMM_PAY,

	S_SET_MAPSERVER,

	OUT_BY_RELOGIN,

	TP_LOGIN,
	TP_LOGINRTL,

	GA_SEND,	//��Ϸ�����������˺ŷ�������
	GA_RECV,	//�˺ŷ�����������Ϸ

	CLIENT_HEATBEAT,
	OUT_BY_BUG,

	GA_GMDATA,
	ACTIVE_CNT,

	TP_LOGIN_SPTOKEN,

	TP_IAPPAY,

	INIT_NOTICE,		//���������Ϣ��
	OUT_BY_KICK,

	SVRLINE_STATE,
	COUNTRY_CHOOSE,
	C_DELETE_CHARACTER,
	S_DELETE_CHARACTER,
	SVRLINE_AREA_NAME,

	LOGINMAIN,
	TP_LOGINMAIN,
	IAP_CHECK_FAIL,
};

enum 
{
	LOGIN_SUCCESSFUL,
	LOGIN_ERROR_ACOUNT_ERROR,		//�����ڵ��˺�
	LOGIN_ERROR_PSWD_ERROR,			//�������
	LOGIN_ERROR_ACOUNT_ISBEREG,		//�ѱ�ע��
	LOGIN_ERROR_SERVER_USER_FULL,
	LOGIN_ERROR_INVITE_ERROR,		//���������
	LOGIN_ERROR_UNENABLE,		//���
	LOGIN_ERROR_OUT_SERVER,

	LOGIN_SUCCESSFUL_BYAD,
};

struct ACCOUNT_LOGIN_Data 
{
	char	szLogin[20];		//�˺��19λ
	char	szPassword[16];		//�������15λ
	BYTE	bSpeedReg;			//�˺Ų�����ֱ��ע��, 2ֻ��ע��
	BYTE	bJoin;					//��֤�˺�ʱ��ֱ�ӽ�
	char	szMacAddress[32];
	BYTE	bTPID;
	char	szAPNToken[80];
	char	szIp[20];
};

struct ACCOUNT_LOGINMAIN_Data 
{
	char	szLogin[20];		//�˺��19λ
	char	szPassword[16];		//�������15λ
	BYTE	bSpeedReg;			//�˺Ų�����ֱ��ע��, 2ֻ��ע��
	BYTE	bJoin;					//��֤�˺�ʱ��ֱ�ӽ�
	char	szMacAddress[32];
	int		nIOSVersion;		//6.0000*10000	//���Ͱ汾��
	char	szIDFA[50];
	char	szIp[20];
};


struct ACCOUNT_C_DELETE_CHARACTER_Data 
{
	DWORD	dwAcc;
	DWORD	dwUin;
};

struct ACCOUNT_S_DELETE_CHARACTER_Data 
{

	DWORD	dwUin;
};

struct ACCOUNT_TP_LOGIN_Data 
{
	BYTE	byTPID;
	char	szUin[50];		//�˺��19λ
	char	szToken[50];		//�������15λ
	char	szMacAddress[32];
};

struct ACCOUNT_TP_LOGIN_SPTOKEN_Data 
{
	BYTE	byTPID;
	char	szUin[20];		//�˺��19λ
	char	szToken[255];		//�������15λ
	char	szMacAddress[32];
	int		nIOSVersion;		//6.0000*10000	//���Ͱ汾��
	char	szIDFA[50];
	char	szIp[20];
};

struct ACCOUNT_TP_LOGINMAIN_Data 
{
	BYTE	byTPID;
	char	szUin[20];		//�˺��19λ
	char	szToken[80];		//�������15λ
	char	szMacAddress[32];
	int		nIOSVersion;		//6.0000*10000	//���Ͱ汾��
	char	szIDFA[50];
	char	szIp[20];
};
struct ACCOUNT_IAP_CHECK_FAIL_Data 
{
	char	szAccount[20];
};
struct ACCOUNT_TP_LOGINRTL_Data 
{
	BYTE	byErrorCode;
	int		nAccUin;
	char	szAcc[32];
	char	szPswd[16];
	char	szBackKey[80];
};

struct ACCOUNT_C_CREATE_CHARACTER_Data
{
	DWORD	dwAcc;
	char	szName[16];
	BYTE	bSex;
	BYTE	bCountry;
	BYTE	bJob;
	char	szInvite[10];
};

struct ACCOUNT_S_JOINGAME_Data 
{
	DWORD	dwUin;
	WORD	wMapSvrIndex;
	WORD	wHomeID;
	BYTE	byHomeDuty;
	BYTE	byVipLvl;
	char	szNick[20];
};

struct _rfSvrObj
{
	char	szIP[20];
	WORD	wPort;
	WORD	wIndex;
};
struct ACCOUNT_INIT_CLIENT_Data 
{
	DWORD		dwKey;
	BYTE		bServerCnt;
	WORD		wVersion;
	BYTE		bRfSvrCnt;
	_rfSvrObj	rfSvrs[1];
};

struct ACCOUNT_S_SET_MAPSERVER_Data 
{
	WORD		wMapIndex;
};

struct ACCOUNT_OUT_BY_RELOGIN_Data 
{
	
};

struct ACCOUNT_OUT_BY_BUG_Data 
{
	
};
struct ACCOUNT_LOGOUT_Data 
{
	DWORD		dwUin;
};

struct	USimpleInfo
{
	DWORD	dwUin;
	BYTE	byLv;
	BYTE	byJob;
	BYTE	byCountry;
	BYTE	bySex;
	char	szNick[16];
};

struct ACCOUNT_C_REQUEST_JOIN_Data 
{
	DWORD	dwAcc;
	DWORD	dwUin;
};

struct ACCOUNT_SIMPLEINFOS_Data 
{
	BYTE		bCnt;
	USimpleInfo	szUSInfos[1];
};

struct ACCOUNT_LOGIN_RLT_Data 
{
	BYTE	byErrorCode;
	BYTE	byJoin;
	int		nAccUin;
	BYTE	bSvrLock;
	BYTE	bSvrIsAD;
};

struct ACCOUNT_SPEED_REGIST_Data 
{
	char	szAcc[50];
	char	szPassword[20];
};

struct ACCOUNT_TPCOMM_LOGIN_Data 
{
	int		nMid;
	int		nType;			//1����,2UC,3pps
	char	szToken[50];
};

struct ACCOUNT_TP_IAPPAY_Data 
{
	char	szAcc[50];		//1����,2UC,3pps
	char	szToken[1];
};
struct ACCOUNT_COUNTRY_CHOOSE_Data 
{
	int nUin;
	BYTE byCountry;
};
struct ACCOUNT_INIT_NOTICE_Data 
{
	WORD	wGameSvr;
	char	szMsg[1];
};

struct ACCOUNT_SVRLINE_STATE_Data 
{
	WORD	wCnt;
	int		sznState[1];		//��λsvrindex,��λʱ״̬, 0   1����  2�¿�   
};

struct ACCOUNT_TPCOMM_PAY_Data
{
	int		nAccUin;
	int		nRealVal;
	int		nRmbVal;
	BYTE	byType;
	char	szOrder[1];
};

struct ACCOUNT_GA_SEND_Data
{//ע�ἤ����, ��ɼ�����
	//int	nSvrID;		//�˺ŷ�������
	char	szCode[10];
	BYTE	bEvent;		//0ע��, 1���
};
struct ACCOUNT_GA_GMDATA_Data
{//GM�������
	int nType;
	int nPVal;
	int nExVal;
};
struct ACCOUNT_ACTIVE_CNT_Data
{//GM�������
	int nPVal;
};
struct ACCOUNT_GA_RECV_Data
{//
	int		nUin;	
	BYTE	bAdd;		//�ɹ����һ��
};

struct ACCOUNT_SPEED_REGIST_CHECK_Data 
{
	char	szAcc[50];
	BYTE	bOk;
	char	szSp[50];
};

struct ACCOUNT_CHANGEPASSWORD_Data
{
	char	szLoginName[50];
	char	szOldPassword[20];
	char	szNewPassword[20];
};

struct ACCOUNT_SVRLINE_AREA_NAME_Data
{
	BYTE    byCountry;
	//char	szAreaName[1];
};

#pragma pack (pop)