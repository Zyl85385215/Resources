#ifndef _BATTLE_PACKET_H_
#define _BATTLE_PACKET_H_

#include "player_packet.h"
#include "signupsystem_define.h"

#pragma pack (push)
#pragma pack (1)

//��Ϣ����
enum _BATTLE
{
	BATTLE_MSG_NULL,					//��ս��������Ϣ
	BATTLE_MSG_BATTLE,					//ս����Ϣ��ս��Ƶ����
	BATTLE_MSG_IMPORTANT,				//��Ҫ֪ͨ����Ļ�����ı���
	BATTLE_MSG_SPECIAL,					//��Ч��Ϣ
	BATTLE_TIME_REMAIN,					//ս���ر�ʣ��ʱ��
	BATTLE_IMFORMATION,					//��ǰս����Ϣ
	BATTLE_ENTER,						//��������ս��
	BATTLE_JOINMENU,					//��ս��������ʾ����
	BATTLE_CLOSEMENU,					//�رմ��ڣ�ͳһ���
	BATTLE_PLAYER_LIST_ADD,				//ս���е�����б���ӳ�Ա
	BATTLE_PLAYER_LIST_DEL,				//ս���е�����б�ɾ����Ա
	BATTLE_RESOURCE_SHOW,				//ս����Դ���ݸ���

	//new battle (DXWar)
	BATTLE_BUILD_BATTLE,				//����ս��                 �����ķ����� -> ������������
	BATTLE_BUILD_RESULT,				//ս���������             ������������ -> ���ķ�������
	BATTLE_LIST_VIEW,					//ս���б�鿴             ����Ϸ������ -> ���ķ������������ķ����� -> ������������     
	BATTLE_PULL_PLAYER,					//����
	BATTLE_PUSH_PLAYER,					//����
	BATTLE_DISTROY_BATTLE,				//�ر�ս��                 ������������ -> ���ķ�������
	BATTLE_PLAYERS_POSITION,			//ս���������

	BATTLE_REQUEST_ENTER,				//�������
	BATTLE_SET_FORCE_ID,

	BATTLE_DELETE_PLAYER,				//����뿪��ս������
	BATTLE_SERVER_TAG,					//ս�����������
	BATTLE_BUILD_STATUS,				//ս���������

	BATTLE_PUSH_PLAYER_RESULT,			//ս���������
	BATTLE_ON_PLAYER_EXIT_WAR,			//��ɫȡ������
	BATTLE_GET_PLAYER_ALL_STATUS,		//��ȡ��ɫ���
	BATTLE_SEND_PLAYER_ALL_STATUS,		//���ͽ�ɫ���
	BATTLE_SEND_PLAYER_STATUS,			//���ͽ�ɫĳս�������
};

//��Ч��Ϣ����
enum
{
	BATTLE_TYPE_START,					//����ս��
	BATTLE_TYPE_QUIT,					//��;�˳�
	BATTLE_TYPE_WIN,					//ʤ��
	BATTLE_TYPE_FAILED,					//ʧ��
	BATTLE_TYPE_STANDOFF,				//�;�
};

//ս��״̬
enum
{
	BATTLE_ONBATTLE,					//��ս��
	BATTLE_TIMEUP,						//ʱ��ֹͣ
	BATTLE_END,							//����
	BATTLE_BREAK,						//ս������
};

//ս������
enum 
{	
	BATTLE_TYPE_NULL,
	BATTLE_TYPE_MINI,
	BATTLE_TYPE_CONF,
	BATTLE_TYPE_COUNTRY,
	BATTLE_TYPE_WORLD,
};

enum
{
	TIME_BATTLE_UPDATE,
};

struct globalWarId
{
	int		warBaseIndex;
	int		serverIndex;
	int		instanceIndex;

	bool operator==(globalWarId & warId)
	{
		return (serverIndex == warId.serverIndex && instanceIndex == warId.instanceIndex);
	}
};

struct TIME_TIME_BATTLE_UPDATE_Data
{

};

struct BATTLE_BATTLE_TIME_REMAIN_Data
{
	int		minite;
};

struct BATTLE_BATTLE_MSG_BATTLE_Data
{
	char	name[40];
	char	msg[1];
};

struct BATTLE_BATTLE_MSG_IMPORTANT_Data
{
	char	msg[1];
};

struct BATTLE_BATTLE_MSG_SPECIAL_Data
{
	BYTE	bMsg;
};

struct BATTLE_BATTLE_IMFORMATION_Data
{
	WORD	wBattleID;                  //ս��ID
	BYTE	bState;                     //ս��״̬
	int		tRemain;                    //��ǰʣ��ʱ��
};

struct BATTLE_BATTLE_ENTER_Data
{
	DWORD	uin;
	globalWarId	warId;					//ս������
	BYTE	bType;                      //�ĸ�����
};

struct BATTLE_BATTLE_REQUEST_ENTER_Data
{
	globalWarId warId;
	BYTE	bType;
};

struct WarView
{
	globalWarId	warId;					//���ķ�����������
	char    szName[MAX_NAMELEN];        //���ID
};

struct BATTLE_BATTLE_JOINMENU_Data
{
	int		nNum;                       //ս������
	WarView warView[1];					//ս��ID
};

struct BattlePInfo						//�б�����ʾ�������Ϣ�ṹ
{
	DWORD	uin;
	char	name[64];
	BYTE	lvl;
	BYTE	bForce;
};

struct BATTLE_BATTLE_PLAYER_LIST_ADD_Data
{
	int		nNum;						//�ð�һ���м�¼���������
	BattlePInfo playerinfo[1];			//������Ա�б�
};

struct BATTLE_BATTLE_PLAYER_LIST_DEL_Data
{
	DWORD	uin;
};

struct BATTLE_BATTLE_RESOURCE_SHOW_Data
{
	int		nResource[1];				//��Դ��Ŀ
};

/*----------------------------DX INSTANCE WAR-------------------------------*/

struct BATTLE_BATTLE_BUILD_BATTLE_Data
{
	DWORD	uin;						//������ID
	int		warBaseId;					//������ս������
};

struct BATTLE_BATTLE_BUILD_RESULT_Data
{
	globalWarId warId;					//ս��ΨһID
	DWORD	uin;						//���ID   
};

struct BATTLE_BATTLE_LIST_VIEW_Data
{
	DWORD  uin;							//�鿴�����
};

struct BATTLE_BATTLE_PULL_PLAYER_Data
{
	int    nServerIndex;				//��������������
	char   szName[256];					//�ĸ�ս��
	bool   blForce;						//�Ƿ�ǿ��
};

struct BATTLE_BATTLE_PUSH_PLAYER_Data
{
	DWORD	uin;
	int		warBaseId;					//������ս������
	WORD	wInstanceID;				//�ĸ�ս�� 
	BYTE	bType;						//�ĸ�����
};

struct BATTLE_BATTLE_DISTROY_BATTLE_Data
{
	globalWarId warId;				//ս��ΨһID
};

struct BattlePlayerPos
{
	DWORD uin;
	int	  x;
	int   y;
};

struct BATTLE_BATTLE_PLAYERS_POSITION_Data
{
	int		nNum;						//�ð����ж��ٸ��û�
	BattlePlayerPos playerpos[1];
};

struct ForceID
{
	BYTE	bForce;
	int		nForceID;
};

struct BATTLE_BATTLE_SET_FORCE_ID_Data
{
	WORD	wInstanceID;
	int	    nCnt;
	ForceID xForceID[1];
};

struct BATTLE_BATTLE_DELETE_PLAYER_Data
{
	DWORD	dwUin;
};

struct BATTLE_BATTLE_BUILD_STATUS_Data
{
	int count;
	globalWarId warId[1];
};

struct BATTLE_BATTLE_PUSH_PLAYER_RESULT_Data
{
	int result;
};

struct BATTLE_BATTLE_ON_PLAYER_EXIT_WAR_Data
{
	int baseWarId;
	DWORD uin;
};

struct BATTLE_BATTLE_GET_PLAYER_ALL_STATUS_Data
{
	DWORD uin;
};

struct BATTLE_BATTLE_SEND_PLAYER_ALL_STATUS_Data
{
	int count;
	dxSignUpPlayerGameStatus status[1];
};

struct BATTLE_BATTLE_SEND_PLAYER_STATUS_Data
{
	dxSignUpPlayerGameStatus status;
};

#pragma pack (pop)

#endif