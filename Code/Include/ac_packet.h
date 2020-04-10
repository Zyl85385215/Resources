#ifndef _AC_PACKET_H_
#define _AC_PACKET_H_

#pragma pack (push)
#pragma pack (1)

#define	ACCOUNT_ADDPOINT_FAILED_INVALID_ACCOUNT				0x01
#define	ACCOUNT_ADDPOINT_FAILED_INVALID_CARD				0x02

#define	ACCOUNT_CREATE_FAILED_LOGINNAME_USED				0x01
#define	ACCOUNT_CREATE_FAILED_INVALID_ACCOUNT				0x02
#define	ACCOUNT_CREATE_FAILED_INVALID_CDKEY					0x03
#define	ACCOUNT_CREATE_FAILED_CDKEY_USED					0x04

#define	ACCOUNT_CHANGE_PASSWORD_FAILED_INVALID_DATA			0x01

#define	ACCOUNT_LOGIN_FAILED_INVALID_DATA					0x01
#define	ACCOUNT_LOGIN_FAILED_ACCOUNT_USED					0x02
#define	ACCOUNT_LOGIN_FAILED_EXPIRED						0x03

#define	ACCOUNT_POINT2MONTH_FAILED_INVALID_ACCOUNT			0x01
#define	ACCOUNT_POINT2MONTH_FAILED_NOT_ENOUGH_POINT			0x02

#define	ACCOUNT_QUERY_POINT_FAILED_INVALID_DATA				0x01

#define	ACCOUNT_QUERY_PERSONAL_DETAIL_FAILED_INVALID_DATA	0x01

#define	ACCOUNT_UPDATE_PERSONAL_DETAIL_FAILED_INVALID_DATA	0x01


// �����ܱ�����
// 1.�˺ŷ����� ������Ϸ������ ACCSVR_PWD_MATRIX_3NUM    ���Ϳͻ��� LOGIN_RLT_INPUT_PWD_MATRIX
// 2.��Ϸ������ �յ� ACCSVR_PWD_MATRIX_3NUM      ���� ����ֵ
// 3.��Ϸ�ͻ��� �յ� LOGIN_RLT_INPUT_PWD_MATRIX  ������Ϸ����� CLIENT_SEND_MATRIX_3NUM
// 4.��Ϸ������ �յ� CLIENT_SEND_MATRIX_3NUM     ������Ϸ�ͻ��� LOGIN_ERROR_SUCCESS || LOGIN_RLT_INPUT_PWD_MATRIX_ERROR

#define  PWD_MATRIX_SIZE 49
enum _ACCOUNT 
{
    SET_SOCKETKEY,                                          //��������ʱ�õģ���
    LOGIN,
    LOGIN_RLT,
    LOGOUT,
	GAME_PING,
	PLAYER_LOGIN,											// ��ɫ��½(���ɫϵͳ)
	ACCSVR_PWD_MATRIX_3NUM,									// �������
	CLIENT_SEND_MATRIX_3NUM,								// ��ȡ����
	SEND_SERVER_CONFIG,
	CHANGEPASSWORD,
	GAME_CHATMSG,
	NET_PING,
	CLIENT_CHECK_SPR,
	CV_SHUTUP_UIN,

	SPEED_REGIST,					//����ע��
	SPEED_REGIST_CHECK,				//����ע��ѯ��
	
	TPCOMM_LOGIN,

	TPCOMM_PAY,
};

enum 
{
	LOGIN_ERROR_ACOUNT_ERROR,
	LOGIN_ERROR_ACCOUNT_PWDERR,
	LOGIN_ERROR_DOUBLE_LOGIN,
	LOGIN_ERROR_ACCOUNT_NOMONEY,
	LOGIN_ERROR_OTHER_ERROR,
	LOGIN_ERROR_VERSION_ERR,				
	LOGIN_ERROR_ACOUNT_BLOCK,               
	LOGIN_ERROR_SERVER_BUSY,				
	LOGIN_ERROR_ACCOUNTSERVER_ERROR,
	LOGIN_ERROR_SUCCESS,
	LOGIN_ERROR_PARA_ERROR,
	LOGIN_ERROR_ACTIVE_ERROR,
	LOGIN_ERROR_ACTIVE_BLOCK,
	LOGIN_ERROR_ACOUNT_FULL,
	LOGIN_ERROR_ACOUNT_GMIPERROR,
	LOGIN_ERROR_GAMESERVER_STOPPING,
	LOGIN_ERROR_TOO_SHORT_TIME,
	LOGIN_ERROR_ACOUNT_IDLOCK,
	LOGIN_RLT_INPUT_PWD_MATRIX,
	LOGIN_RLT_INPUT_PWD_MATRIX_ERROR,
	LOGIN_ERROR_ACOUNT_POINTS_ERROR,
	LOGIN_ERROR_TOO_MANY_CLIENT_OPEN,
	LOGIN_ERROR_NEEDTO_CHANGE_PASSWAORD,
	LOGIN_ERROR_CHANGEPSW_SUCC,
	LOGIN_ERROR_CHANGEPSW_FAIL,
	LOGIN_ERROR_TOO_MANY_CLIENT_FROM_ONE_IP,
	LOGIN_RLT_CHECK_SPR,
	CHECK_SPR_RESET,
	LOGIN_RLT_CHECK_SPR_ERROR,
	LOGIN_RLT_LIMIT_LOGIN_ERROR,
};

struct ACCOUNT_LOGIN_Data 
{
	char	szLoginName[50];
	char	szPassword[20];
	char	szIP[16];
	int		nPing;
	int		nClientCntIn1PC;
	char	szMacAddress[20];
	BYTE	byCheckSpr;
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

struct ACCOUNT_TPCOMM_PAY_Data
{
	char	szAcc[50];
	char	szOrder[1];
};

struct ACCOUNT_SPEED_REGIST_CHECK_Data 
{
	char	szAcc[50];	//�ͻ��˼��ı�ʾ,  ���ص�ʱ����ʵ���˺�
	BYTE	bOk;
	char	szSp[50];	//��վ�ص����ڱ����˺�
};

struct ACCOUNT_LOGIN_RLT_Data 
{
	BYTE	yErrorCode;
	int     time;
};

struct ACCOUNT_CHECK_SPR_RESET_Data 
{
	WORD	wCheckSpr[4];
	BYTE	bOpen;
};


struct ACCOUNT_LOGOUT_Data 
{
	bool	bAbend;
	char	szClientIp[20];
};

struct ACCOUNT_SET_SOCKETKEY_Data 
{
    WORD    socketKey;
	DWORD	dwGameVersion;
	char	szUpdateMsg[1];
};


struct ACCOUNT_GAME_PING_Data 
{
	DWORD	timeTick;
	BYTE    bCntState;
	int		nServerIndex;
	int		nNumber;// �ڼ��η��İ�
	DWORD	dwTimeRecvInNet;
	DWORD	dwServerTime;
};

struct ACCOUNT_NET_PING_Data 
{
	DWORD	dwTimeSend;
};

struct ACCOUNT_GAME_CHATMSG_Data 
{
	DWORD	dwSend;
	DWORD	dwChannel;
	char	szMsg[1];
};

struct ACCOUNT_PLAYER_LOGIN_Data
{
	DWORD	dwUin;							//��ɫID
};

struct ACCOUNT_CLIENT_SEND_MATRIX_3NUM_Data
{
	WORD	wVal[3];
};

struct ACCOUNT_CV_SHUTUP_UIN_Data
{
	DWORD	dwUin;
	int		time;
};

struct ACCOUNT_CLIENT_CHECK_SPR_Data
{
	char	szCheck[10];
};

struct ACCOUNT_ACCSVR_PWD_MATRIX_3NUM_Data
{
	unsigned int uTag;
	WORD	wVal[3];
};

struct ACCOUNT_SEND_SERVER_CONFIG_Data
{
	bool	m_bArrCountryOpen[4];
};

struct ACCOUNT_CHANGEPASSWORD_Data
{
	char	szLoginName[50];
	char	szOldPassword[20];
	char	szNewPassword[20];
};

#pragma pack (pop)

#endif
