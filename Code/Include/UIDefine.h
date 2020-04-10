#ifndef UI_DEFINE_H
#define UI_DEFINE_H

#define WM_SVRCMD WM_USER+100
#define WM_SVRDATA WM_USER+101
#define WM_SVRUI

#define CHG_FILE_NAME "NewGameShell.exe"
#define CHG_FILE_OTHER_NAME "NewGameShellEx.exe"
#define GAMECORE_EXE "F:\\WORKSPASE\\Server\\ConsoleGameCore\\Debug\\ConsoleGameCore.exe"

struct UDP_Data
{
	DWORD	dwType;
	DWORD	dwData;
};

enum
{
	WAITLOIN_NUM,
	LOGIN_NUM,	
	ONLINE_NUM,				//在线人数
	SERVER_STATE,			//服务器状态
	SERVER_STOP_BACK_TIME,	//停止服务器倒计时
	SERVER_HEARTBEAT_NUM,
};

enum
{
	SERVER_UNCONN,			//正在启动|停止|暂停|未连接:全无效
	SERVER_START,			//退出倒计时:停止  |开始:暂停,停止
	SERVER_PAUSE,			//暂停:开始,停止
	SERVER_STOP,			//停止:开始
	SERVER_STARTING,
	SERVER_STOPING,
};

enum
{
	CMD_SVRSTART,			//开启服务器
	CMD_SVRPAUSE,			//暂停服务器
	CMD_SVRSTOPPAUSE,		//退出暂停服务器
	CMD_SVRPRESTOP,			//开始停止服务器
	CMD_SVRSTOP,			//关闭服务器
	CMD_SVRCANCEL,
	CMD_SVRCLOSE,
	CMD_GETSVRSTATE,		//获取服务器状态
	CMD_GETSVRNUM,
	CMD_OPENJNETSTAT,		//开启流量统计
	CMD_STOPJNETSTAT,		//关闭流量统计
	CMD_PRINTNETSTAT,
	CMD_THREADCPU,
	CMD_SVRDBLOG_START,		//开启服务器db执行监控
	CMD_SVRDBLOG_STOP,		//关闭服务器db执行监控
	CMD_FRESHMALL,			//刷新商城配置
};
#endif