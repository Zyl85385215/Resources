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
	ONLINE_NUM,				//��������
	SERVER_STATE,			//������״̬
	SERVER_STOP_BACK_TIME,	//ֹͣ����������ʱ
	SERVER_HEARTBEAT_NUM,
};

enum
{
	SERVER_UNCONN,			//��������|ֹͣ|��ͣ|δ����:ȫ��Ч
	SERVER_START,			//�˳�����ʱ:ֹͣ  |��ʼ:��ͣ,ֹͣ
	SERVER_PAUSE,			//��ͣ:��ʼ,ֹͣ
	SERVER_STOP,			//ֹͣ:��ʼ
	SERVER_STARTING,
	SERVER_STOPING,
};

enum
{
	CMD_SVRSTART,			//����������
	CMD_SVRPAUSE,			//��ͣ������
	CMD_SVRSTOPPAUSE,		//�˳���ͣ������
	CMD_SVRPRESTOP,			//��ʼֹͣ������
	CMD_SVRSTOP,			//�رշ�����
	CMD_SVRCANCEL,
	CMD_SVRCLOSE,
	CMD_GETSVRSTATE,		//��ȡ������״̬
	CMD_GETSVRNUM,
	CMD_OPENJNETSTAT,		//��������ͳ��
	CMD_STOPJNETSTAT,		//�ر�����ͳ��
	CMD_PRINTNETSTAT,
	CMD_THREADCPU,
	CMD_SVRDBLOG_START,		//����������dbִ�м��
	CMD_SVRDBLOG_STOP,		//�رշ�����dbִ�м��
	CMD_FRESHMALL,			//ˢ���̳�����
};
#endif