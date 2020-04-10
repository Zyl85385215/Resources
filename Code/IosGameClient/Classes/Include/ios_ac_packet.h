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
	SPEED_REGIST,					//快速注册
	SPEED_REGIST_CHECK,				//快速注册询问

	TPCOMM_LOGIN,
	TPCOMM_PAY,

	S_SET_MAPSERVER,

	OUT_BY_RELOGIN,

	TP_LOGIN,
	TP_LOGINRTL,

	GA_SEND,	//游戏服务器发给账号服务器的
	GA_RECV,	//账号服务器发送游戏

	CLIENT_HEATBEAT,
	OUT_BY_BUG,

	GA_GMDATA,
	ACTIVE_CNT,

	TP_LOGIN_SPTOKEN,

	TP_IAPPAY,

	INIT_NOTICE,		//公告界面信息包
	OUT_BY_KICK,

	SVRLINE_STATE,
	COUNTRY_CHOOSE,
	C_DELETE_CHARACTER,
	S_DELETE_CHARACTER,
	SVRLINE_AREA_NAME,

	LOGINMAIN,
	TP_LOGINMAIN,
	IAP_CHECK_FAIL,
	CHANGEPASSWORD_RTL,
};

enum 
{
	LOGIN_SUCCESSFUL,
	LOGIN_ERROR_ACOUNT_ERROR,		//不存在的账号
	LOGIN_ERROR_PSWD_ERROR,			//密码错误
	LOGIN_ERROR_ACOUNT_ISBEREG,		//已被注册
	LOGIN_ERROR_SERVER_USER_FULL,
	LOGIN_ERROR_INVITE_ERROR,		//激活码错误
	LOGIN_ERROR_UNENABLE,		//封号
	LOGIN_ERROR_OUT_SERVER,

	LOGIN_SUCCESSFUL_BYAD,
};

struct ACCOUNT_LOGIN_Data 
{
	char	szLogin[20];		//账号最长19位
	char	szPassword[16];		//密码最多15位
	BYTE	bSpeedReg;			//账号不存在直接注册, 2只是注册
	BYTE	bJoin;					//验证账号时不直接进
	char	szMacAddress[32];
	BYTE	bTPID;
	char	szAPNToken[80];
	char	szIp[20];
};

struct ACCOUNT_LOGINMAIN_Data 
{
	char	szLogin[20];		//账号最长19位
	char	szPassword[16];		//密码最多15位
	BYTE	bSpeedReg;			//账号不存在直接注册, 2只是注册
	BYTE	bJoin;					//验证账号时不直接进
	char	szMacAddress[32];
	int		nIOSVersion;		//6.0000*10000	//机型版本号
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
	char	szUin[50];		//账号最长19位
	char	szToken[50];		//密码最多15位
	char	szMacAddress[32];
};

struct ACCOUNT_TP_LOGIN_SPTOKEN_Data 
{
	BYTE	byTPID;
	char	szUin[20];		//账号最长19位
	char	szToken[255];		//密码最多15位
	char	szMacAddress[32];
	int		nIOSVersion;		//6.0000*10000	//机型版本号
	char	szIDFA[50];
	char	szIp[20];
};

struct ACCOUNT_TP_LOGINMAIN_Data 
{
	BYTE	byTPID;
	char	szUin[20];		//账号最长19位
	char	szToken[80];		//密码最多15位
	char	szMacAddress[32];
	int		nIOSVersion;		//6.0000*10000	//机型版本号
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
	char	szRtlPswd[16];
};

struct ACCOUNT_SPEED_REGIST_Data 
{
	char	szAcc[50];
	char	szPassword[20];
};

struct ACCOUNT_TPCOMM_LOGIN_Data 
{
	int		nMid;
	int		nType;			//1当乐,2UC,3pps
	char	szToken[50];
};

struct ACCOUNT_TP_IAPPAY_Data 
{
	char	szAcc[50];		//1当乐,2UC,3pps
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
	int		sznState[1];		//高位svrindex,低位时状态, 0   1爆满  2新开   
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
{//注册激活码, 达成激活码
	//int	nSvrID;		//账号服务器有
	char	szCode[10];
	BYTE	bEvent;		//0注册, 1达成
};
struct ACCOUNT_GA_GMDATA_Data
{//GM工具相关
	int nType;
	int nPVal;
	int nExVal;
};
struct ACCOUNT_ACTIVE_CNT_Data
{//GM工具相关
	int nPVal;
};
struct ACCOUNT_GA_RECV_Data
{//
	int		nUin;	
	BYTE	bAdd;		//成功达成一个
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

struct ACCOUNT_CHANGEPASSWORD_RTL_Data
{
	BYTE	byRtl;			//0成功  , 1账号不存在, 2原密码错误 3格式不符
};

struct ACCOUNT_SVRLINE_AREA_NAME_Data
{
	BYTE    byCountry;
	//char	szAreaName[1];
};

#pragma pack (pop)