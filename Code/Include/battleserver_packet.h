#pragma once

#pragma pack (push)
#pragma pack (1)

enum
{
	BATTLE_CLIENT_ENTER,		//	服务器注册
	CREATE_BATTLE_INFO,			//	战斗创建信息
	REQUEST_GET_BATTLE_INFO,	//	申请获得战斗信息
	SEND_BATTLE_INFO,			//	发送战斗信息
	SIGNUP_BATTLE_INFO,			//	战斗报名信息

	REQUEST_SIGNUP,		//報名
	REQUEST_CANCEL,		//取消報名
	REQUEST_LIST,		//請求列表
	BATTLE_EXIT,		//離開戰場,由戰場服務器發
	BATTLE_DELETE,		//銷毀戰場
};

struct BATTLE_SERVER_REQUEST_SIGNUP_Data
{
	DWORD dwBattleID;
	DWORD dwUin;
	DWORD dwForceID;
	DWORD dwExID;
	DWORD dwInstanceID;
};

struct BATTLE_SERVER_REQUEST_CANCEL_Data
{
	DWORD dwBattleID;
	DWORD dwUin;
	DWORD dwInstanceID;
};

struct BATTLE_SERVER_REQUEST_LIST_Data
{
	DWORD dwBattleID;
	DWORD dwUin;
	DWORD dwExID;
};

struct BATTLE_SERVER_BATTLE_EXIT_Data
{
	DWORD dwBattleID;
	DWORD dwUin;
	DWORD dwInstanceID;
};

struct BATTLE_SERVER_BATTLE_DELETE_Data
{
	DWORD dwBattleID;
	DWORD dwInstanceID;
};

struct BATTLE_SERVER_BATTLE_CLIENT_ENTER_Data
{
	DWORD dwServerId;
};

struct BATTLE_SERVER_CREATE_BATTLE_INFO_Data
{
	DWORD dwID;						//	战斗模板编号
	DWORD dwExID;						//	战斗标记
};

struct BATTLE_SERVER_REQUEST_GET_BATTLE_INFO_Data
{
	char szTag[1];						//	战斗标记
};

struct BATTLE_SERVER_SEND_BATTLE_INFO_Data
{
	DWORD dwCount;						//	实例个数
	DWORD dwIndex[1];					//	实例编号数组
};

#pragma pack (pop)