#pragma once

#pragma pack (push)
#pragma pack (1)

enum
{
	BATTLE_CLIENT_ENTER,		//	������ע��
	CREATE_BATTLE_INFO,			//	ս��������Ϣ
	REQUEST_GET_BATTLE_INFO,	//	������ս����Ϣ
	SEND_BATTLE_INFO,			//	����ս����Ϣ
	SIGNUP_BATTLE_INFO,			//	ս��������Ϣ

	REQUEST_SIGNUP,		//����
	REQUEST_CANCEL,		//ȡ������
	REQUEST_LIST,		//Ո���б�
	BATTLE_EXIT,		//�x�_����,�ɑ����������l
	BATTLE_DELETE,		//�N������
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
	DWORD dwID;						//	ս��ģ����
	DWORD dwExID;						//	ս�����
};

struct BATTLE_SERVER_REQUEST_GET_BATTLE_INFO_Data
{
	char szTag[1];						//	ս�����
};

struct BATTLE_SERVER_SEND_BATTLE_INFO_Data
{
	DWORD dwCount;						//	ʵ������
	DWORD dwIndex[1];					//	ʵ���������
};

#pragma pack (pop)