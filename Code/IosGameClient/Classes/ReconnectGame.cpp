#include "ReconnectGame.h"
#include "MainMenu.h"
#include "ios_ac_packet.h"

CreateSingleton(ReconnectModel);
extern	char*	g_szLoginAcc;
extern	char*	g_szLoginPswd;
ReconnectModel::ReconnectModel()
{
	m_szServerIP	= NULL;
	m_nPort			= 0;
	m_bWorking		= false;
	m_dwReTryTick	= 0;
}

ReconnectModel::~ReconnectModel()
{
	if(m_szServerIP)	free(m_szServerIP);
}

void ReconnectModel::SetServerInfo( char* szIP, int nPort )
{
	if(m_szServerIP)
		free(m_szServerIP);
	m_szServerIP	= strdup(szIP);
	m_nPort		= nPort;
}

bool ReconnectModel::PacketProcess( ZPacket* pPacket )
{
	if(!m_bWorking)
		return false;

	if(pPacket->bCmdGroup == ACCOUNT)
	{
		switch (pPacket->bCmd)
		{
		case INIT_CLIENT:
			{
				SET_DATA(pInit, ACCOUNT, INIT_CLIENT, pPacket);
				g_packetCenter.m_SocketManager.m_xRandom.Random_Seed(pInit->dwKey);
				BuildPacketEx(pNewPacket, ACCOUNT, LOGIN, buffer,1024);
				SET_DATA(loginData, ACCOUNT, LOGIN, pNewPacket);

				strcpy(loginData->szLogin, g_szLoginAcc);
				strcpy(loginData->szPassword, g_szLoginPswd);
				
				loginData->bSpeedReg	= 0;
				loginData->bJoin		= true;
				memset(loginData->szMacAddress,0,32);
				strncpy(loginData->szMacAddress,g_pTPObj->TP_GetMacAddress(),30);
				loginData->bTPID	= g_pTPObj->TP_GetPlat();
				memset(loginData->szAPNToken,0,80);
				SendPacket(pNewPacket);
			}
			return true;
		case	SIMPLEINFOS:
			{
				BuildPacketEx(pNewPacket, ACCOUNT, C_REQUEST_JOIN, buffer,256);
				SET_DATA(pSend,ACCOUNT, C_REQUEST_JOIN,pNewPacket);
				pSend->dwUin	= pChar->m_dwUin;
				SendPacket(pNewPacket);
				m_dwReTryTick	= 0;
			}
			return true;
		case LOGIN_RLT:
			{
				SET_DATA(pData, ACCOUNT, LOGIN_RLT, pPacket);
				if(pData->byErrorCode == LOGIN_SUCCESSFUL)
				{
					m_dwReTryTick	= 0;
				}

			}
			return true;
		}
	}
	return false;
}

void ReconnectModel::SetModelWorked(bool bVal)
{
	m_bWorking	= bVal;
	m_dwReTryTick	= 0;
	g_packetCenter.RemoveListener(this);
	g_packetCenter.AddListener(this);
}

void ReconnectModel::Update()
{
	if(!m_bWorking)
		return;

	if(g_packetCenter.m_bActive)
	{
		if(m_dwReTryTick && xnGetTickCount ()-m_dwReTryTick > 10000)
		{
			BuildPacketEx(pNewPacket, ACCOUNT, LOGIN, buffer,1024);
			SET_DATA(loginData, ACCOUNT, LOGIN, pNewPacket);

			strcpy(loginData->szLogin, g_szLoginAcc);
			strcpy(loginData->szPassword, g_szLoginPswd);

			loginData->bSpeedReg	= 0;
			loginData->bJoin		= true;
			memset(loginData->szMacAddress,0,32);
			strncpy(loginData->szMacAddress,g_pTPObj->TP_GetMacAddress(),30);
			loginData->bTPID	= g_pTPObj->TP_GetPlat();
			memset(loginData->szAPNToken,0,80);
			SendPacket(pNewPacket);
			m_dwReTryTick	= xnGetTickCount ();
		}
	}
	else if(m_dwReTryTick == 0 || xnGetTickCount ()-m_dwReTryTick > 10000)
	{
		g_packetCenter.ConnectServer (m_szServerIP,m_nPort);
		m_dwReTryTick	= xnGetTickCount ();
	}
}