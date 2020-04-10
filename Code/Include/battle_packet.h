#ifndef _BATTLE_PACKET_H_
#define _BATTLE_PACKET_H_

#include "player_packet.h"
#include "signupsystem_define.h"

#pragma pack (push)
#pragma pack (1)

//消息类型
enum _BATTLE
{
	BATTLE_MSG_NULL,					//非战场特有信息
	BATTLE_MSG_BATTLE,					//战场消息（战场频道）
	BATTLE_MSG_IMPORTANT,				//重要通知（屏幕居中文本）
	BATTLE_MSG_SPECIAL,					//特效信息
	BATTLE_TIME_REMAIN,					//战场关闭剩余时间
	BATTLE_IMFORMATION,					//当前战场信息
	BATTLE_ENTER,						//报名进入战场
	BATTLE_JOINMENU,					//打开战场报名显示界面
	BATTLE_CLOSEMENU,					//关闭窗口（统一命令）
	BATTLE_PLAYER_LIST_ADD,				//战场中的玩家列表添加成员
	BATTLE_PLAYER_LIST_DEL,				//战场中的玩家列表删除成员
	BATTLE_RESOURCE_SHOW,				//战场资源数据更新

	//new battle (DXWar)
	BATTLE_BUILD_BATTLE,				//创建战场                 （中心服务器 -> 副本服务器）
	BATTLE_BUILD_RESULT,				//战场创建结果             （副本服务器 -> 中心服务器）
	BATTLE_LIST_VIEW,					//战场列表查看             （游戏服务器 -> 中心服务器）（中心服务器 -> 副本服务器）     
	BATTLE_PULL_PLAYER,					//拉人
	BATTLE_PUSH_PLAYER,					//加人
	BATTLE_DISTROY_BATTLE,				//关闭战场                 （副本服务器 -> 中心服务器）
	BATTLE_PLAYERS_POSITION,			//战友坐标更新

	BATTLE_REQUEST_ENTER,				//申请加入
	BATTLE_SET_FORCE_ID,

	BATTLE_DELETE_PLAYER,				//玩家离开了战场名单
	BATTLE_SERVER_TAG,					//战场服务器标记
	BATTLE_BUILD_STATUS,				//战场创建情况

	BATTLE_PUSH_PLAYER_RESULT,			//战场报名结果
	BATTLE_ON_PLAYER_EXIT_WAR,			//角色取消报名
	BATTLE_GET_PLAYER_ALL_STATUS,		//获取角色情况
	BATTLE_SEND_PLAYER_ALL_STATUS,		//发送角色情况
	BATTLE_SEND_PLAYER_STATUS,			//发送角色某战场的情况
};

//特效信息类型
enum
{
	BATTLE_TYPE_START,					//进入战场
	BATTLE_TYPE_QUIT,					//中途退出
	BATTLE_TYPE_WIN,					//胜利
	BATTLE_TYPE_FAILED,					//失败
	BATTLE_TYPE_STANDOFF,				//和局
};

//战场状态
enum
{
	BATTLE_ONBATTLE,					//开战中
	BATTLE_TIMEUP,						//时间停止
	BATTLE_END,							//结束
	BATTLE_BREAK,						//战场销毁
};

//战场类型
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
	WORD	wBattleID;                  //战场ID
	BYTE	bState;                     //战场状态
	int		tRemain;                    //当前剩余时间
};

struct BATTLE_BATTLE_ENTER_Data
{
	DWORD	uin;
	globalWarId	warId;					//战场索引
	BYTE	bType;                      //哪个势力
};

struct BATTLE_BATTLE_REQUEST_ENTER_Data
{
	globalWarId warId;
	BYTE	bType;
};

struct WarView
{
	globalWarId	warId;					//中心服务器绑定索引
	char    szName[MAX_NAMELEN];        //玩家ID
};

struct BATTLE_BATTLE_JOINMENU_Data
{
	int		nNum;                       //战场数量
	WarView warView[1];					//战场ID
};

struct BattlePInfo						//列表中显示的玩家信息结构
{
	DWORD	uin;
	char	name[64];
	BYTE	lvl;
	BYTE	bForce;
};

struct BATTLE_BATTLE_PLAYER_LIST_ADD_Data
{
	int		nNum;						//该包一共有记录的玩家人数
	BattlePInfo playerinfo[1];			//所有人员列表
};

struct BATTLE_BATTLE_PLAYER_LIST_DEL_Data
{
	DWORD	uin;
};

struct BATTLE_BATTLE_RESOURCE_SHOW_Data
{
	int		nResource[1];				//资源数目
};

/*----------------------------DX INSTANCE WAR-------------------------------*/

struct BATTLE_BATTLE_BUILD_BATTLE_Data
{
	DWORD	uin;						//创建者ID
	int		warBaseId;					//创建的战场类型
};

struct BATTLE_BATTLE_BUILD_RESULT_Data
{
	globalWarId warId;					//战场唯一ID
	DWORD	uin;						//玩家ID   
};

struct BATTLE_BATTLE_LIST_VIEW_Data
{
	DWORD  uin;							//查看的玩家
};

struct BATTLE_BATTLE_PULL_PLAYER_Data
{
	int    nServerIndex;				//副本服务器索引
	char   szName[256];					//哪个战场
	bool   blForce;						//是否强制
};

struct BATTLE_BATTLE_PUSH_PLAYER_Data
{
	DWORD	uin;
	int		warBaseId;					//创建的战场类型
	WORD	wInstanceID;				//哪个战场 
	BYTE	bType;						//哪个势力
};

struct BATTLE_BATTLE_DISTROY_BATTLE_Data
{
	globalWarId warId;				//战场唯一ID
};

struct BattlePlayerPos
{
	DWORD uin;
	int	  x;
	int   y;
};

struct BATTLE_BATTLE_PLAYERS_POSITION_Data
{
	int		nNum;						//该包含有多少个用户
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