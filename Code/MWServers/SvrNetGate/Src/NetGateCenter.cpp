#include "stdafx.h"
#include "NetGateCenter.h"
#include "ZNetServer.h"
#include "NetGateCfg.h"

#define  WM_CHECK_LINK	WM_USER + 1301

NetGateCenter* g_pMainGate	= NULL;

BOOLEAN CheckSvrLinkThreadProc(void * data, MSG * Msg)
{
	switch (Msg->message)
	{
	case	WM_CHECK_LINK:
		{
			g_pNetCenter->CheckServerLink();
		}
		break;
	}
	return TRUE;
}

NetGateCenter::NetGateCenter()
{
	NetGateCfg::GetInstance().LoadFile("ServerCfg.txt");
	MapSelCfg::GetInstance().LoadFile("MapSelCfg.txt");
	m_pThread	= NULL;
}

NetGateCenter::~NetGateCenter()
{
	if(m_pThread)
	{
		m_pThread->Stop();
		delete m_pThread;
	}
}


void NetGateCenter::InitNet()
{
	g_pNetCenter	= new ZNetCenter();
	g_pNetCenter->StartNetCenter();


	m_pThread = new xnThread(CheckSvrLinkThreadProc, this, "checklink");
	m_pThread->SetPriority(THREAD_PRIORITY_NORMAL);

	//OutMessage(msg_type_error,"PlayerDB%d ReStart.\n", pDBServer->m_dwID);
	m_pThread->Start();
}

#include "ios_system_packet.h"
void NetGateCenter::Update25()
{
	m_pThread->SendMessage(WM_CHECK_LINK,0,0,true);
	//g_pNetCenter->CheckServerLink();
}
