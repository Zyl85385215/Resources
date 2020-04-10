#include "stdafx.h"
#include "ZNetServer.h"
#include "ServerCfg.h"
#include "AccountCenter.h"
#include "ios_system_packet.h"
#include "debug_def.h"

ZNetCenter*		g_pNetCenter	= NULL;
ZHttpCenter*	g_pHttpCenter	= NULL;
ZLogFile*		g_pLogFile		= NULL;

ZNetCenter::ZNetCenter()
{
	memset(m_szpNetGates,0,4*MAX_NETGATE);
	memset(m_szpGameSvr,0,4*MAX_GAMESVR);
	g_pLogFile	= new ZLogFile(0);
	g_pLogFile->Open("AccountLog");
}

ZNetCenter::~ZNetCenter()
{
	delete g_pLogFile;
}

void ZNetCenter::StartNetCenter()
{
	InitServer(ServerCfg::GetInstance().nAccPort,ServerCfg::GetInstance().szAccIP,20,true);
	if(Start() == false)
		AfxMessageBox("服务器启动失败");
}

void ZNetCenter::StopNetCenter()
{

}

void ZNetCenter::OnCreateConnect( JNetClient* pClient )
{
	
}

void ZNetCenter::OnDisconnect( JNetClient* pClient )
{
	
}

int ZNetCenter::ExtractPacket( JNetClient* pClient,char* pRecvBuffer,int nDataLen )
{
	int dataSize = nDataLen;
	char * k = pRecvBuffer;
	while (dataSize >= sizeof(ZPacket)-1)
	{
		ZPacket * pPacket = (ZPacket *)k;
		if (pPacket->wMark != ZPACKET_MARK)
		{
			//Error Data
			if (strnicmp((char*)k, "NOOP", 4) == 0)
			{
				k += 4;
				dataSize -= 4;
			}
			else
			{
				k++;
				dataSize--;
			}
		}
		else
		{
			int packetSize = sizeof(ZPacket) -1 + pPacket->wSize;
			if (dataSize >= packetSize)
			{
				RecvClientPacket(pClient,pPacket);

				k += packetSize;
				dataSize -= packetSize;
			}
			else
				break;
		}
	}
	return (nDataLen - dataSize);
}

void ZNetCenter::RecvClientPacket( JNetClient* pClient,ZPacket* pPacket )
{
	switch (pPacket->bCmdGroup)
	{
	case SYSTEM:
		{
			switch(pPacket->bCmd)
			{
			case NETGATE_LOGIN_ACC:
				{
					SET_DATA(pRecv,SYSTEM,NETGATE_LOGIN_ACC,pPacket);
					if(pRecv->bySubKey < MAX_NETGATE)
						m_szpNetGates[pRecv->bySubKey]	= pClient;
				}
				break;
			case GAMESVR_LOGIN_ACC:
				{
					SET_DATA(pRecv,SYSTEM,GAMESVR_LOGIN_ACC,pPacket);
					if(pRecv->bySubKey < MAX_NETGATE)
						m_szpGameSvr[pRecv->bySubKey]	= pClient;
				}
				break;
			}
			
		}
		break;
	default:
		g_pMainAccount->RecvPacket(pPacket);
		break;
	}
}

void ZNetCenter::_sendPacketToClient( JNetClient* pClient, ZPacket* pPacket )
{
	pClient->SendData((BYTE*)pPacket,pPacket->wSize+sizeof(ZPacket)-1);
}

void ZNetCenter::SendNGPacket( ZPacket* pPacket )
{
	if(pPacket->dwKey == 0)
	{
		for (int i = 0; i < MAX_NETGATE; i++)
		{
			if(m_szpNetGates[i] && m_szpNetGates[i]->IsActive())
			{
				m_szpNetGates[i]->SendData((BYTE*)pPacket,pPacket->wSize + sizeof(ZPacket)-1);
				return;
			}
		}
	}

	int bySubKey	= (pPacket->dwKey>>28);
	if(bySubKey >= MAX_NETGATE || m_szpNetGates[bySubKey] == NULL || m_szpNetGates[bySubKey]->IsActive() == false)
		return;

	m_szpNetGates[bySubKey]->SendData((BYTE*)pPacket,pPacket->wSize + sizeof(ZPacket)-1);
}



ZHttpCenter::ZHttpCenter()
{
	m_pHttpCenter	= new HttpCenter();
}

ZHttpCenter::~ZHttpCenter()
{
	delete m_pHttpCenter;
}

void ZHttpCenter::StartHttpCenter()
{
	InitServer(ServerCfg::GetInstance().nHttpPort,ServerCfg::GetInstance().szHttpIP,20,true);
	if(Start() == false)
		AfxMessageBox("服务器启动失败");
}

void ZHttpCenter::OnCreateConnect( JNetClient* pClient )
{

}

void ZHttpCenter::OnDisconnect( JNetClient* pClient )
{

}

int ZHttpCenter::ExtractPacket( JNetClient* pClient,char* pRecvBuffer,int nDataLen )
{
#ifndef _DEBUG
	__try
#endif
	{
		m_pHttpCenter->RecvHttpMsg((DWORD)pClient,pRecvBuffer,nDataLen);
	}
#ifndef _DEBUG
	__except(DUMP_CALLSTACK())
	{
		DUMP_STRING("AccountCenter RecvHttpMsg Error!\n");
	}
#endif
	//int nRtl	= 
	//if(nRtl)
	{
		//char	szRtl[2048]	= {0};
		//sprintf(szRtl,"HTTP/1.1 200 OK\r\n\
		//			  Content-Type: text/html\r\n\
		//			  Last-Modified: Sun, 07 Jul 2013 10:28:52 GMT\r\n\
		//			  Accept-Ranges: bytes\r\n\
		//			  ETag: \"955ca1c6fc7ace1:0\"\r\n\
		//			  Server: Microsoft-IIS/7.5\r\n\
		//			  X-Powered-By: ASP.NET\r\n\
		//			  Date: Tue, 11 Feb 2014 06:41:55 GMT\r\n\
		//			  Content-Length: 34\r\n\
		//			  \r\n\
		//			  result=0\r\n\
		//			  ");
		////可能iocp不行,尝试直接的TCP?
		//pClient->SendData((BYTE*)szRtl,strlen(szRtl));
		pClient->ShutDown();
	}

	return nDataLen;
}
