#ifndef _DBMANG_PACKET_H_
#define _DBMANG_PACKET_H_

#pragma pack (push)
#pragma pack (1)

enum 
{
	DBMANG_LOGIN,
	DBMANG_LOGOUT,
	DBMANG_LOGIN_SUCCEED,
	DBMANG_LOGIN_FAILED,
	ADD_DBNAME,
	CHANGE_DBNAME,
	DELETE_DBNAME,
	PROCESS_ERROR,
	ADD_MANGUSER,
	DEL_MANGUSER,
	DBMANG_CHGPASS,
};

struct DBMANG_DBMANG_LOGIN_Data {
	char name[20];
	char password[20];
};

struct DBMANG_DBMANG_CHGPASS_Data {
	char name[20];
	char oldpass[20];
	char password[20];
};

struct DBMANG_DBMANG_LOGOUT_Data{
	char name[20];
};

struct DBMANG_DBMANG_LOGIN_SUCCEED_Data{
	BYTE type;
};

struct DBMANG_DBMANG_LOGIN_FAILED_Data{
	BYTE errorCode;
};

struct DBMANG_ADD_DBNAME_Data{
	int id;
	char ip[20];
	char dbname[20];
	char name[20];
	char pass[20];
	char info[40];
};

struct DBMANG_CHANGE_DBNAME_Data{
	int id;
	char ip[20];
	char dbname[20];
	char name[20];
	char pass[20];
	char info[40];
};

struct DBMANG_DELETE_DBNAME_Data{
	int id;
};

struct DBMANG_PROCESS_ERROR_Data{
	BYTE code;
};

#endif