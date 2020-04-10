#ifndef _STAT_PACKET_
#define _STAT_PACKET_

#pragma pack (push)
#pragma pack (1)

enum{
	STAT_LOGIN,
	STAT_LOGOUT,
	STAT_LOGIN_SUCCEED,
	STAT_LOGIN_FAILED,
	ADD_USER,
	ADD_USER_RIGHT,
	DEL_USER,
	CHANGE_USER,
	CHANGE_USER_PASSWORD,
	CHARNE_PASSWORD_RESULT,
	DEL_USER_RESULT,
	CHANGE_USER_RESULT,
	ADD_USER_RESULT,

	SERVER_DATA,
	SERVER_ONLINENUM,
	SERVER_MAXNUM,

	SERVER_ID,
	USER_INFO,

	GET_SERVER_DATA,
	GET_SERVER_DATA_RESULT,
	SERVER_LOG,
	SERVER_LOG_END
};

struct ServerLogData
{
	int count;
	char time[20];
};

struct STAT_STAT_LOGIN_Data {
	char name[20];
	char password[20];
};

struct STAT_STAT_LOGOUT_Data{
	char name[20];
};

struct STAT_STAT_LOGIN_SUCCEED_Data{
	BYTE type;
};

struct STAT_STAT_LOGIN_FAILED_Data{
	BYTE errorCode;
};

struct STAT_ADD_USER_Data{
	STAT_STAT_LOGIN_Data user;
	BYTE type;
	int count;
	int right[1];
};

struct STAT_ADD_USER_RIGHT_Data{
	int id[1];
};

struct STAT_DEL_USER_Data{
	char name[20];
};

struct STAT_ADD_USER_RESULT_Data{
	BYTE errorCode;
};

struct STAT_CHANGE_USER_Data{
    char name[20];
	char password[20];
	BYTE type;
	int count;
	int right[1];
};

struct STAT_CHANGE_USER_PASSWORD_Data{
	char name[20];
	char oldpsw[20];
	char password[20];
};

struct STAT_CHARNE_PASSWORD_RESULT_Data{
	BYTE rlt;
};

struct STAT_DEL_USER_RESULT_Data{
	BYTE rlt;
	char name[20];
};

struct STAT_CHANGE_USER_RESULT_Data{
	BYTE rlt;
};

struct STAT_SERVER_DATA_Data{
	int id;
	int micount;
	char time[20];
	int mocount;
	int month;
};

struct STAT_SERVER_ONLINENUM_Data{
	int id;
	int count;
};

struct STAT_SERVER_MAXNUM_Data{
	int id;
	int month;
	int count;
	char time[20];
};

struct STAT_GET_SERVER_DATA_Data{
	char stime[20];
	char etime[20];
	int id;
};

struct STAT_SERVER_LOG_Data
{
	int id;
	int count;
	ServerLogData data[10];
};

struct STAT_GET_SERVER_DATA_RESULT_Data{
	int id;
	int count;
};

struct STAT_SERVER_LOG_END_Data{
	int id;
};

struct STAT_SERVER_ID_Data{
	int id;
	char ip[20];	
};

struct STAT_USER_INFO_Data{
	char name[20];
	char password[20];
	int count;
	int  id[1];
};

#pragma pack (pop)
#endif