#include "stdafx.h"

#include "ServerCenter.h"
#include "ZNetServer.h"
#include "MapCenter.h"
#include "ControlCenter.h"
#include "debug_def.h"
#include "EnemyConfig.h"
#include "MapServerCfg.h"
#include "SkillConfig.h"
#include "StateConfig.h"
#include "RobotConfig.h"

ZLogFile*			g_pLogFile		= NULL;
MapServerCenter*	g_pMSvrCenter	= NULL;
#define  WM_SERVER_INIT	WM_USER + 1101
#define  WM_HEART_BEAT	WM_USER + 1102
#define  WM_DEAL_PACKET	WM_USER + 1103

void OutMessage(int channel, char* lpszFormat, ...)
{
	char msg[4096] = {0};
	vsprintf(msg, lpszFormat, (char*) (&lpszFormat + 1));

	if(g_pLogFile != NULL)
		g_pLogFile->OutPut(msg);
}

BOOLEAN	FnMainThread(void * data, MSG * Msg)
{
	MapServerCenter*	pMCenter	= (MapServerCenter*)data;
	switch (Msg->message)
	{
	case	WM_SERVER_INIT:
		{
			pMCenter->MTInitCenter();
		}
		break;
	case	WM_HEART_BEAT:
		{
			g_pNetCenter->CheckServerLink();
			pMCenter->HeartBeat();
		}
		break;
	case	WM_DEAL_PACKET:
		{
			pMCenter->DealPacket();
		}
		break;
	}
	return TRUE;
}

MapServerCenter::MapServerCenter()
{
	m_pMainThread = new xnThread(FnMainThread, this, "mainthread");
	m_pMainThread->SetPriority(THREAD_PRIORITY_HIGHEST);
	m_pMainThread->Start();
	m_pMainThread->SendMessage(WM_SERVER_INIT,0,0,TRUE);
	InstallMyExceptHandle();
	srand(GetTickCount());

	g_pLogFile		= new ZLogFile(0);
	g_pLogFile->Open("MapLog");
}

MapServerCenter::~MapServerCenter()
{
	m_pMainThread->Stop();
	delete m_pMainThread;

	delete g_pNetCenter;

	delete g_pCtrlList;
	delete g_pMapList;
}

void MapServerCenter::MTFireHeartBeat()
{
	m_pMainThread->SendMessage(WM_HEART_BEAT,0,0);
}

void MapServerCenter::MTFireDealPacket()
{
	m_pMainThread->SendMessage(WM_DEAL_PACKET,0,0);
}

void MapServerCenter::MTInitCenter()
{
	srand(GetTickCount());
	//初始化配置文件
	//初始化3个核心模块
	EnemyBaseCfg::GetInstance().LoadFile("cfg\\enemybase.txt");
	MapConfig::GetInstance().LoadFile("Map\\mapconfig.ini");
	MapServerCfg::GetInstance().LoadFile("ServerCfg.txt");
	SkillConfig::GetInstance().LoadFile("cfg\\SkillConfig.txt");
	StateConfig::GetInstance().LoadFile("cfg\\StateConfig.txt");
	RobotConfig::GetInstance().LoadFile("cfg\\RobotCfg.txt");
	g_pNetCenter	= new ZNetCenter();
	g_pNetCenter->StartNetCenter();

	g_pCtrlList	= new ControlList();
	g_pMapList	= new MapList();

	
}

void MapServerCenter::DealPacket()
{
#ifndef _DEBUG
	__try
#endif
	{
		//先收包,再UPDATE处理
		_dealClientPackets();
	}
#ifndef _DEBUG
	__except(DUMP_CALLSTACK())
	{
		DUMP_STRING("MapServerCenter DealPacket Error!\n");
	}
#endif
}

void MapServerCenter::HeartBeat()
{
#ifndef _DEBUG
	__try
#endif
	{
		//先收包,再UPDATE处理
		Update();
	}
#ifndef _DEBUG
	__except(DUMP_CALLSTACK())
	{
		DUMP_STRING("MapServerCenter HeartBeat Error!\n");
	}
#endif
}

void MapServerCenter::_dealClientPackets()
{
	xnList*	pLPackets	= g_pNetCenter->SwitchDealGameSvrList();
	while(pLPackets->Count)
	{
		ZPacket*	pPacket	= (ZPacket*)pLPackets->Delete(0);
		g_pMapList->RecvPacket(pPacket,pPacket->dwKey);
		free(pPacket);
	}

	pLPackets	= g_pNetCenter->SwitchDealNetGateList();
	while(pLPackets->Count)
	{
		ZPacket*	pPacket	= (ZPacket*)pLPackets->Delete(0);
		g_pCtrlList->RecvPacket(pPacket,pPacket->dwKey);
		free(pPacket);
	}
}

void MapServerCenter::Update()
{
	g_pMapList->Update();
	g_pCtrlList->Update();

	static DWORD	s_dwCenterUpdateCnt	= 0;
	s_dwCenterUpdateCnt++;
	if(s_dwCenterUpdateCnt % 10 == 0)
	{
		g_pMapList->UpdateSec();
		g_pCtrlList->UpdateSec();
	}
}
