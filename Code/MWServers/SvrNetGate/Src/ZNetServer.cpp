#include "stdafx.h"
#include "ZNetServer.h"
#include "NetGateCfg.h"
#include "ZNetPacket.h"
#include "ios_system_packet.h"
#include "ios_ac_packet.h"
#include "ios_user_packet.h"
#include "debug_def.h"

ZNetCenter*	g_pNetCenter	= NULL;

ZNetClientCenter::ZNetClientCenter()
{
	m_dwMaxKey		= 1;
	InitializeCriticalSection(&m_xLock);
}

ZNetClientCenter::~ZNetClientCenter()
{
	DeleteCriticalSection(&m_xLock);
}

int ZNetClientCenter::AddClient( JNetClient* pNClient )
{
	EnterCriticalSection(&m_xLock);
	ZNetCData*	pData	= new ZNetCData();
	pData->dwKey		= m_dwMaxKey + (NetGateCfg::GetInstance().bySubKey<<28);
	pData->dwUin		= 0;
	pData->dwAccount	= 0;
	pData->dwRecv		= 0;
	pData->dwSend		= 0;
	pData->bMapSrvID	= 0;
	pData->dwTick		= GetTickCount();
	pData->dwHeartBeatTime	= 0;
	pData->dwTickHBReset	= GetTickCount();
	pNClient->SetData(pData);
	pNClient->m_xRandom.Random_Seed(pData->dwKey);
	mapClients.insert(std::make_pair(pData->dwKey,pNClient));

	m_dwMaxKey++;
	if(m_dwMaxKey > 200000000)
		m_dwMaxKey	= 1;
	LeaveCriticalSection(&m_xLock);
	return pData->dwKey;
}

void ZNetClientCenter::RejoinClient()
{
	EnterCriticalSection(&m_xLock);
	HMapClientPtr	itor	= mapClients.begin();
	while(itor != mapClients.end())
	{
		JNetClient*	pNC		= itor->second;
		ZNetCData*	pNData	= (ZNetCData*)(pNC->GetData());

		BuildPacketEx(pSPacket,ACCOUNT, C_REQUEST_JOIN,buf,256);
		pSPacket->dwKey		= pNData->dwKey;
		SET_DATA(pSend, ACCOUNT, C_REQUEST_JOIN, pSPacket);
		pSend->dwAcc		= pNData->dwAccount;
		pSend->dwUin		= pNData->dwUin;

		g_pNetCenter->m_pSvrGame->SendPacket(pSPacket);

		itor++;
	}
	LeaveCriticalSection(&m_xLock);
}

bool ZNetClientCenter::RemoveClient( JNetClient* pNClient,bool bOutGame )
{
	if(pNClient->IsActive())
		pNClient->ShutDown();

	EnterCriticalSection(&m_xLock);
	HMapClientPtr	itor	= mapClients.begin();
	while(itor != mapClients.end())
	{
		if(itor->second == pNClient)
		{
			ZNetCData*	pNData	= (ZNetCData*)(pNClient->GetData());

			if(pNData && bOutGame && pNData->dwUin)
			{//多次登录不需要通知游戏服务器
				BuildPacketEx(pSPacket, ACCOUNT,LOGOUT, pBuf,256);
				SET_DATA(pLogout, ACCOUNT,LOGOUT, pSPacket);
				pLogout->dwUin		= pNData->dwUin;
				pSPacket->dwKey		= pNData->dwKey;
				g_pNetCenter->m_pSvrGame->SendPacket(pSPacket);
			}
			//delete pNData;					//容易出多线程访问问题
			//pNClient->SetData(NULL);		//容易出多线程访问问题
			mapClients.erase(itor++);
			LeaveCriticalSection(&m_xLock);
			return true;
		}
		else
			itor++;
	}
	LeaveCriticalSection(&m_xLock);
	return false;
}

JNetClient* ZNetClientCenter::GetByKey( DWORD dwKey )
{
	EnterCriticalSection(&m_xLock);
	JNetClient* pRtl	= NULL;
	HMapClientPtr	itor	= mapClients.find(dwKey);
	if(itor != mapClients.end())
		pRtl	= itor->second;
	LeaveCriticalSection(&m_xLock);
	return pRtl;
}

JNetClient* ZNetClientCenter::GetByAccUin( DWORD dwAcc )
{
	EnterCriticalSection(&m_xLock);
	JNetClient* pRtl	= NULL;
	HMapClientPtr	itor	= mapClients.begin();
	while(itor != mapClients.end())
	{
		ZNetCData* pND	= (ZNetCData*)itor->second->GetData();
		if(pND->dwAccount	== dwAcc)
		{
			LeaveCriticalSection(&m_xLock);
			return itor->second;
		}
		itor++;
	}
	LeaveCriticalSection(&m_xLock);
	return NULL;
}

void ZNetClientCenter::RefreshListView( CListCtrl* pCtrlList )
{
	pCtrlList->DeleteAllItems();
	EnterCriticalSection(&m_xLock);
	HMapClientPtr	itor	= mapClients.begin();
	char	szTmp[256];
	DWORD	dwCurTick	= GetTickCount();
	while(itor != mapClients.end())
	{
		ZNetCData* pND	= (ZNetCData*)itor->second->GetData();


		sprintf(szTmp,"%d", pND->dwKey);
		int nPos	= pCtrlList->InsertItem(pCtrlList->GetItemCount(),szTmp);
		sprintf(szTmp,"%d", pND->dwAccount);
		pCtrlList->SetItemText(nPos,1,szTmp);
		sprintf(szTmp,"%d", pND->dwUin);
		pCtrlList->SetItemText(nPos,2,szTmp);
		if(itor->second && itor->second->IsActive())
			pCtrlList->SetItemText(nPos,3,inet_ntoa(itor->second->GetAddress()->sin_addr));
		sprintf(szTmp,"%d", pND->dwRecv);
		pCtrlList->SetItemText(nPos,4,szTmp);
		sprintf(szTmp,"%d", pND->dwSend);
		pCtrlList->SetItemText(nPos,5,szTmp);

		sprintf(szTmp,"%dM", (dwCurTick-pND->dwTick)/60000);
		pCtrlList->SetItemText(nPos,6,szTmp);
		itor++;
	}
	LeaveCriticalSection(&m_xLock);
}



ZNetSvrLink::ZNetSvrLink()
{
	m_sSocketTcp	= NULL;
}

ZNetSvrLink::~ZNetSvrLink()
{
	Disconnect();
}

void ZNetSvrLink::Disconnect()
{
	if(m_sSocketTcp)
	{
		m_sSocketTcp->Diconnect();
		m_sSocketTcp	= NULL;
	}
}

bool ZNetSvrLink::isActive()
{
	return (m_sSocketTcp && m_sSocketTcp->IsActive());
}

int ZNetSvrLink::ProcessData( char* pRecvBuffer,int nDataLen )
{
	int dataSize = nDataLen;
	char * k = pRecvBuffer;
	while (dataSize >= sizeof(ZPacket)-1)
	{
		ZPacket * pPacket = (ZPacket *)k;
		if (pPacket->wMark != ZPACKET_MARK)
		{
			//Error Data
			if (strnicmp((char*)k, "FUCK", 4) == 0)
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
				g_pNetCenter->_dealSvrPacket(pPacket);
				
				k += packetSize;
				dataSize -= packetSize;
			}
			else
				break;
		}
	}
	return (nDataLen - dataSize);
}

bool ZNetSvrLink::SendPacket( ZPacket * pPacket )
{
	if(isActive())
		return m_sSocketTcp->SendData((BYTE*)pPacket,pPacket->wSize + sizeof(ZPacket)-1);
	return false;
}


int ZNetSLinkCenter::ExtractPacket( CSocketTCP* pClient,char* pRecvBuffer,int nDataLen )
{
	if(pClient	== g_pNetCenter->m_pSvrAccount->m_sSocketTcp)
		return g_pNetCenter->m_pSvrAccount->ProcessData(pRecvBuffer,nDataLen);
	if(pClient == g_pNetCenter->m_pSvrGame->m_sSocketTcp)
		return g_pNetCenter->m_pSvrGame->ProcessData(pRecvBuffer,nDataLen);
	for (int i = 0; i < g_pNetCenter->m_nMapSvrCnt; i++)
	{
		if(pClient == g_pNetCenter->m_ppSvrMap[i]->m_sSocketTcp)
			return g_pNetCenter->m_ppSvrMap[i]->ProcessData(pRecvBuffer,nDataLen);
	}
	return nDataLen;
}


ZNetCenter::ZNetCenter()
{
	m_pCCLinkSvr	= new ZNetSLinkCenter();
	m_pNetClients	= new ZNetClientCenter();

	m_pSvrAccount	= new ZNetSvrLink();
	m_pSvrGame		= new ZNetSvrLink();

	m_nMapSvrCnt	= MapSelCfg::GetInstance().GetMapSvrCnt();
	m_ppSvrMap		= new ZNetSvrLink*[m_nMapSvrCnt];
	for (int i = 0; i < m_nMapSvrCnt; i++)
	{
		m_ppSvrMap[i]	= new ZNetSvrLink();
	}
	m_nLinkTotal	= 0;
	m_nLinkCnt		= 0;

	m_dwRecvClient	= 0;
	m_dwSendClient	= 0;
	m_nServerCnt	= 4;
	m_szNotice		= NULL;
	m_szADNotice    = NULL;
	m_pHotSvrList   = xnList::Create();
	m_nDefCountry   = 0;
}

ZNetCenter::~ZNetCenter()
{
	delete m_pCCLinkSvr;
	delete m_pNetClients;

	delete	m_pSvrAccount;
	delete	m_pSvrGame;

	for (int i = 0; i < m_nMapSvrCnt; i++)
	{
		delete m_ppSvrMap[i];
	}
	delete[] m_ppSvrMap;

	while(m_pHotSvrList->Count)
		m_pHotSvrList->Delete(0);
	m_pHotSvrList->Free();
}

void ZNetCenter::StartNetCenter()
{

	InitServer(NetGateCfg::GetInstance().nPort,NetGateCfg::GetInstance().szIP,NetGateCfg::GetInstance().nMaxConn,false);
	m_pCCLinkSvr->InitCenter(100);
	m_pCCLinkSvr->Start();
	if(Start() == false)
		AfxMessageBox("服务器启动失败");

	//CheckServerLink();
}

void ZNetCenter::StopNetCenter()
{
	Stop();
	m_pCCLinkSvr->Stop();
}

void ZNetCenter::OnCreateConnect( JNetClient* pClient )
{
	int dwKey	= m_pNetClients->AddClient(pClient);

	SendSeverState(pClient);

	if(m_nServerCnt == 0)
	{//临时处理!!!!!!!
		BuildPacketEx(pPacket,USER, S_SEND_POP_MSG, buf, 2048);
		SET_DATA(pData, USER, S_SEND_POP_MSG, pPacket);
		pPacket->wSize += 60;
		strcpy(pData->szMsg,"服务器正在维护中，维护结束时间请关注官方QQ群：252585887");//230886602
		pClient->SendData((BYTE*)pPacket,pPacket->wSize + sizeof(ZPacket)-1);
	}
	else
	{
		BuildPacketEx(pPacket,ACCOUNT,INIT_CLIENT,buf, 256);
		SET_DATA(pSend, ACCOUNT, INIT_CLIENT, pPacket);
		pSend->dwKey	= dwKey;
		pSend->bServerCnt	= m_nServerCnt;
		pSend->wVersion		= 5;
		pSend->bRfSvrCnt	= 0;

		//需要更改服务器列表
		//pSend->bRfSvrCnt	= 1;
		//strcpy(pSend->rfSvrs[0].szIP,"192.168.1.100");
		//pSend->rfSvrs[0].wIndex	= 101;
		//pSend->rfSvrs[0].wPort	= 8130;

		pPacket->wSize	+= pSend->bRfSvrCnt*sizeof(_rfSvrObj) - sizeof(_rfSvrObj);
		pClient->SendData((BYTE*)pPacket,pPacket->wSize + sizeof(ZPacket)-1);
	}
	SendAreaName(pClient);
	NoticeClient(pClient,0);

	m_nLinkCnt++;
	m_nLinkTotal++;
}

void ZNetCenter::OnDisconnect( JNetClient* pClient )
{
	m_pNetClients->RemoveClient(pClient);

	m_nLinkCnt--;
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
			if (strnicmp((char*)k, "FUCK", 4) == 0)
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
				if(pPacket->dwKey == pClient->m_xRandom.Random_Int(0,10000)
					|| pPacket->bCmdGroup == GMTOOL)
					_dealClickPacket(pClient,pPacket);

				k += packetSize;
				dataSize -= packetSize;
			}
			else
				break;
		}
	}
	return (nDataLen - dataSize);
}

void ZNetCenter::RecvSvrPacket( ZPacket* pSvrPacket )
{//收到其他服务器的包直接转发对应的客户端
	if(pSvrPacket->dwKey == 0)
	{
		if(pSvrPacket->bCmdGroup == ACCOUNT)
		{
			if(pSvrPacket->bCmd == TPCOMM_PAY)
			{//充值包
				m_pSvrGame->SendPacket(pSvrPacket);
				return;
			}
			if(pSvrPacket->bCmd == GA_GMDATA)
			{
				m_pSvrAccount->SendPacket(pSvrPacket);
				return;
			}
			if(pSvrPacket->bCmd == GA_SEND)
			{
				m_pSvrAccount->SendPacket(pSvrPacket);
				return;
			}
			if(pSvrPacket->bCmd == GA_RECV)
			{
				m_pSvrGame->SendPacket(pSvrPacket);
				return;
			}
		}
	}


	JNetClient* pClient	= m_pNetClients->GetByKey(pSvrPacket->dwKey);
	if(pClient)
	{
		if(pSvrPacket->bCmdGroup == ACCOUNT)
		{
			switch(pSvrPacket->bCmd)
			{
			case LOGIN_RLT:
				{

					SET_DATA(pAccData, ACCOUNT, LOGIN_RLT, pSvrPacket);
					if(pAccData->byErrorCode == LOGIN_SUCCESSFUL || pAccData->byErrorCode == LOGIN_SUCCESSFUL_BYAD)
					{
						pAccData->bSvrLock	= 0;
						pAccData->bSvrIsAD	= 0;
						//检查多重登陆
						JNetClient* pOldNC	= m_pNetClients->GetByAccUin(pAccData->nAccUin);
						if(pOldNC && pOldNC != pClient)
						{//连接中断, ?通知?
							
							BuildPacketEx(pSendPacket,ACCOUNT,OUT_BY_RELOGIN,bufs,256);
							_sendPacketToClient(pOldNC,pSendPacket);
							m_pNetClients->RemoveClient(pOldNC,true);
						}
						//赋值连接对象的账号
						ZNetCData* pND	= (ZNetCData*)pClient->GetData();
						pND->dwAccount	= pAccData->nAccUin;

						if(pAccData->byErrorCode == LOGIN_SUCCESSFUL_BYAD)
						{
							pAccData->bSvrIsAD	= 1;
							NoticeClient(pClient,GetADHotSvr(),1);
							pAccData->byErrorCode = LOGIN_SUCCESSFUL;
						}
						else
							NoticeClient(pClient,GetHotSvr());
					}
					else if(pAccData->byErrorCode == LOGIN_ERROR_SERVER_USER_FULL)
					{//临时处理!!!!!!!
						BuildPacketEx(pSendPacket,USER, S_SEND_POP_MSG, bufs, 256);
						SET_DATA(pSData, USER, S_SEND_POP_MSG, pSendPacket);
						pSendPacket->wSize += 50;
						strcpy(pSData->szMsg,"服务器爆满, 请选择其他服务器或稍后重试");
						_sendPacketToClient(pClient,pSendPacket);
						return;
					}
				}
				break;
			case TP_LOGINRTL:
				{
					SET_DATA(pAccData, ACCOUNT, TP_LOGINRTL, pSvrPacket);
					if(pAccData->byErrorCode == LOGIN_SUCCESSFUL || pAccData->byErrorCode == LOGIN_SUCCESSFUL_BYAD)
					{
						//检查多重登陆
						JNetClient* pOldNC	= m_pNetClients->GetByAccUin(pAccData->nAccUin);
						if(pOldNC && pOldNC != pClient)
						{//连接中断, ?通知?

							BuildPacketEx(pSendPacket,ACCOUNT,OUT_BY_RELOGIN,bufs,256);
							_sendPacketToClient(pOldNC,pSendPacket);
							m_pNetClients->RemoveClient(pOldNC,true);
						}
						//赋值连接对象的账号
						ZNetCData* pND	= (ZNetCData*)pClient->GetData();
						pND->dwAccount	= pAccData->nAccUin;

						if(pAccData->byErrorCode == LOGIN_SUCCESSFUL_BYAD)
						{
							NoticeClient(pClient,GetADHotSvr(),1);
							pAccData->byErrorCode = LOGIN_SUCCESSFUL;
						}
						else
							NoticeClient(pClient,GetHotSvr());
					}
					else if(pAccData->byErrorCode == LOGIN_ERROR_SERVER_USER_FULL)
					{//临时处理!!!!!!!
						BuildPacketEx(pSendPacket,USER, S_SEND_POP_MSG, bufs, 256);
						SET_DATA(pSData, USER, S_SEND_POP_MSG, pSendPacket);
						pSendPacket->wSize += 50;
						strcpy(pSData->szMsg,"服务器爆满, 请选择其他服务器或稍后重试");
						_sendPacketToClient(pClient,pSendPacket);
						return;
					}
				}
				break;
			case COUNTRY_CHOOSE:
				{
					SET_DATA(pAccData, ACCOUNT, COUNTRY_CHOOSE, pSvrPacket);
					_sendPacketToClient(pClient,pSvrPacket);
					return;
				}
				break;
			case OUT_BY_RELOGIN:
				{
					_sendPacketToClient(pClient,pSvrPacket);
					m_pNetClients->RemoveClient(pClient,true);
					return;
				}
				break;
			case OUT_BY_KICK:
				{
					_sendPacketToClient(pClient,pSvrPacket);
					m_pNetClients->RemoveClient(pClient,true);
					return;
				}
				break;
			case S_JOINGAME:
				{
					SET_DATA(pAccData, ACCOUNT, S_JOINGAME, pSvrPacket);

					//赋值连接对象的UIN
					ZNetCData* pND	= (ZNetCData*)pClient->GetData();
					pND->dwUin	= pAccData->dwUin;
					pND->dwTickHBReset	= GetTickCount();
					pND->dwHeartBeatTime	= 0;
					BYTE	bMSID	= MapSelCfg::GetInstance().GetByMapIndex(pAccData->wMapSvrIndex);
					if(bMSID >=  m_nMapSvrCnt)
						pND->bMapSrvID	= 0;
					else
						pND->bMapSrvID	= bMSID;

					BuildPacketEx(pBackPacket,USER,N_NETGATEOK,buf1, 256);
					SET_DATA(pBack, USER, N_NETGATEOK, pBackPacket);
					pBackPacket->dwKey		= pND->dwUin;
					m_pSvrGame->SendPacket(pBackPacket);
				}
				break;
			case S_SET_MAPSERVER:
				{
					SET_DATA(pAccData, ACCOUNT, S_SET_MAPSERVER, pSvrPacket);
					ZNetCData* pND	= (ZNetCData*)pClient->GetData();
					BYTE	bMSID	= MapSelCfg::GetInstance().GetByMapIndex(pAccData->wMapIndex);
					if(bMSID >=  m_nMapSvrCnt)
						pND->bMapSrvID	= 0;
					else
						pND->bMapSrvID	= bMSID;
				}
				return;
			}
			
		}

		_sendPacketToClient(pClient,pSvrPacket);
		//pClient->SendData((BYTE*)pSvrPacket,pSvrPacket->wSize+sizeof(ZPacket)-1);
	}
}

void ZNetCenter::RecvClientPacket( JNetClient* pClient,ZPacket* pPacket )
{
	int nSize	= sizeof(ZPacket)-1+pPacket->wSize;
	ZNetCData*	pCData	= (ZNetCData*)pClient->GetData();
	if(pCData == NULL)
		return;
	pCData->dwRecv	+= nSize;
	m_dwRecvClient	+= nSize;
	//if(m_pSvrGame->isActive() == false)
	//{//断线重连的时候再断掉
	//	pClient->ShutDown();
	//	return;
	//}
	switch (pPacket->bCmdGroup)
	{
	case ACCOUNT:
		pPacket->dwKey		= pCData->dwKey;
		switch(pPacket->bCmd)
		{
		case CLIENT_HEATBEAT:
			{
				DWORD	dwTickPass	= GetTickCount() - pCData->dwTickHBReset;
				if(dwTickPass > 180000)
				{
					if(pCData->dwHeartBeatTime > 12)
					{//加速了?
						BuildPacketEx(pSendPacket,ACCOUNT,OUT_BY_BUG,bufs,256);
						_sendPacketToClient(pClient,pSendPacket);
						m_pNetClients->RemoveClient(pClient,true);
					}
					//3分钟无异常就清除
					pCData->dwTickHBReset = GetTickCount();
					pCData->dwHeartBeatTime = 0;
				}
				else
					pCData->dwHeartBeatTime++;
			}
			break;
		case C_REQUEST_JOIN:
			{//登录的账号验证,同时保留角色号
				SET_DATA(pRecv,ACCOUNT,C_REQUEST_JOIN,pPacket);
				pRecv->dwAcc	= pCData->dwAccount;
				if(pRecv->dwAcc == 0)
				{
					BuildPacketEx(pSendPacket,ACCOUNT,INIT_CLIENT,buf, 256);
					SET_DATA(pSend, ACCOUNT, INIT_CLIENT, pSendPacket);
					pSend->dwKey	= pCData->dwKey;
					pSend->bServerCnt	= 4;
					pSend->wVersion		= 5;
					pSend->bRfSvrCnt	= 0;
					_sendPacketToClient(pClient,pSendPacket);
					return;
				}
				m_pSvrGame->SendPacket(pPacket);
			}
			break;
		case C_CREATE_CHARACTER:
			{
				SET_DATA(pRecv,ACCOUNT,C_CREATE_CHARACTER,pPacket);
				pRecv->dwAcc	= pCData->dwAccount;
				if(pRecv->dwAcc == 0)
					return;
				m_pSvrGame->SendPacket(pPacket);
			}
			break;
		case C_DELETE_CHARACTER:
			{
				SET_DATA(pRecv,ACCOUNT,C_DELETE_CHARACTER,pPacket);
				pRecv->dwAcc	= pCData->dwAccount;
				if(pRecv->dwAcc == 0)
					return;
				m_pSvrGame->SendPacket(pPacket);
			}
			break;
		case LOGIN:
			{
				SET_DATA(pRecv,ACCOUNT,LOGIN,pPacket);
				if(pRecv->bJoin == true && m_nLinkCnt >= 5000)
				{//服务器人数已满
					BuildPacketEx(pSendPacket, ACCOUNT, LOGIN_RLT,buf,256);
					SET_DATA(pSend, ACCOUNT, LOGIN_RLT, pSendPacket);
					pSendPacket->dwKey	= pPacket->dwKey;
					pSend->nAccUin		= 0;
					pSend->byErrorCode	= LOGIN_ERROR_SERVER_USER_FULL;
					pSend->byJoin		= true;
					_sendPacketToClient(pClient,pSendPacket);
					return;
				}
				strcpy(pRecv->szIp,inet_ntoa(pClient->GetAddress()->sin_addr));
				m_pSvrAccount->SendPacket(pPacket);
			}
			break;
		case ACTIVE_CNT:
			{
				m_pSvrAccount->SendPacket(pPacket);
			}
			break;
		case LOGINMAIN:
			{
				SET_DATA(pRecv,ACCOUNT,LOGINMAIN,pPacket);
				strcpy(pRecv->szIp,inet_ntoa(pClient->GetAddress()->sin_addr));
				m_pSvrAccount->SendPacket(pPacket);
			}
			break;
		case TP_LOGINMAIN:
			{
				SET_DATA(pRecv,ACCOUNT,TP_LOGINMAIN,pPacket);
				strcpy(pRecv->szIp,inet_ntoa(pClient->GetAddress()->sin_addr));

				m_pSvrAccount->SendPacket(pPacket);
			}
			break;
		case TP_LOGIN_SPTOKEN:
			{
				SET_DATA(pRecv,ACCOUNT,TP_LOGIN_SPTOKEN,pPacket);
				strcpy(pRecv->szIp,inet_ntoa(pClient->GetAddress()->sin_addr));
				m_pSvrAccount->SendPacket(pPacket);
			}
			break;
		default:
			m_pSvrAccount->SendPacket(pPacket);
			break;
		}
		break;
	case MAPCMD:
		{//发送给地图服务器
			pPacket->dwKey		= pCData->dwKey;
			m_ppSvrMap[pCData->bMapSrvID]->SendPacket(pPacket);
		}
		break;
	case MAPOBJ:
		{
			//玩家不能发该类型包给地图服务器,不安全
		}
		break;
	case GMTOOL:
		{
			SOCKADDR_IN*  aa = pClient->GetAddress();
			if(!NetGateCfg::GetInstance().CheckGMTool(aa->sin_addr.S_un.S_addr)){
				break;
			}
			pPacket->dwKey		= pCData->dwKey;
			m_pSvrGame->SendPacket(pPacket);
		}
		break;
	default:	//游戏服务器直接用UIN发
		pPacket->dwKey		= ((ZNetCData*)pClient->GetData())->dwUin;
		m_pSvrGame->SendPacket(pPacket);
		break;
	}
}

void ZNetCenter::_sendPacketToClient( JNetClient* pClient, ZPacket* pPacket )
{
	int nSize	= pPacket->wSize+sizeof(ZPacket)-1;
	m_dwSendClient	+= nSize;
	((ZNetCData*)(pClient->GetData()))->dwSend	+= nSize;
	pClient->SendData((BYTE*)pPacket,nSize);
}

void ZNetCenter::CheckServerLink()
{
	if(m_pSvrAccount->isActive() == false)
	{
		CSocketTCP* pSockAcc	= m_pCCLinkSvr->InitConnect(NetGateCfg::GetInstance().szAccIP,NetGateCfg::GetInstance().nAccPort);

		if(pSockAcc)
		{
			m_pSvrAccount->SetSocket(pSockAcc);

			//认证连接信息
			BuildPacketEx(pPacket,SYSTEM,NETGATE_LOGIN_ACC,buf, 256);
			SET_DATA(pSend,SYSTEM,NETGATE_LOGIN_ACC,pPacket );
			pSend->bySubKey	= NetGateCfg::GetInstance().bySubKey;
			m_pSvrAccount->SendPacket(pPacket);
		}
	}
	if(strlen(NetGateCfg::GetInstance().szGameIP) == 0)
		return;
	if(m_pSvrGame->isActive() == false)
	{
		CSocketTCP* pSockGame	= m_pCCLinkSvr->InitConnect(NetGateCfg::GetInstance().szGameIP,NetGateCfg::GetInstance().nGamePort);

		if(pSockGame)
		{
			m_pSvrGame->SetSocket(pSockGame);
			//认证连接信息
			BuildPacketEx(pPacket,SYSTEM,NETGATE_LOGIN_ACC,buf, 256);
			SET_DATA(pSend,SYSTEM,NETGATE_LOGIN_ACC,pPacket );
			pSend->bySubKey	= NetGateCfg::GetInstance().bySubKey;
			m_pSvrGame->SendPacket(pPacket);

			//m_pNetClients->RejoinClient();
		}
	}
	if(m_ppSvrMap[0]->isActive() == false)
	{
		CSocketTCP* pSockMapDf	= m_pCCLinkSvr->InitConnect(MapSelCfg::GetInstance().m_pMapSrvDefaul->szIP,MapSelCfg::GetInstance().m_pMapSrvDefaul->nPort);

		if(m_ppSvrMap[0])
		{
			m_ppSvrMap[0]->SetSocket(pSockMapDf);
			//认证连接信息
			BuildPacketEx(pPacket,SYSTEM,NETGATE_LOGIN_ACC,buf, 256);
			SET_DATA(pSend,SYSTEM,NETGATE_LOGIN_ACC,pPacket );
			pSend->bySubKey	= NetGateCfg::GetInstance().bySubKey;
			m_ppSvrMap[0]->SendPacket(pPacket);
		}
	}

	for (int i = 1; i < m_nMapSvrCnt; i++)
	{
		if(m_ppSvrMap[i]->isActive() == false)
		{
			MapSrvObj* pMSvr	= (MapSrvObj*)MapSelCfg::GetInstance().m_pList->Items[i-1];
			CSocketTCP* pSockMap	= m_pCCLinkSvr->InitConnect(pMSvr->szIP,pMSvr->nPort);

			if(m_ppSvrMap[i])
			{
				m_ppSvrMap[i]->SetSocket(pSockMap);
				//认证连接信息
				BuildPacketEx(pPacket,SYSTEM,NETGATE_LOGIN_ACC,buf, 256);
				SET_DATA(pSend,SYSTEM,NETGATE_LOGIN_ACC,pPacket );
				pSend->bySubKey	= NetGateCfg::GetInstance().bySubKey;
				m_ppSvrMap[i]->SendPacket(pPacket);
			}
		}
	}

}

void ZNetCenter::_dealClickPacket( JNetClient* pClient,ZPacket* pPacket )
{
#ifndef _DEBUG
	__try
#endif
	{
		RecvClientPacket(pClient,pPacket);
	}
#ifndef _DEBUG
	__except(DUMP_CALLSTACK())
	{
		DUMP_STRING("_dealClickPacket DealPacket Error!\n");
	}
#endif
}

void ZNetCenter::_dealSvrPacket( ZPacket* pPacket )
{
//#ifndef _DEBUG
	__try
//#endif
	{
		RecvSvrPacket(pPacket);
	}
//#ifndef _DEBUG
	__except(DUMP_CALLSTACK())
	{
		DUMP_STRING("_dealSvrPacket DealPacket Error!\n");
	}
//#endif
}

void ZNetCenter::NoticeClient(JNetClient* pClient,WORD wDefSvr,BYTE bySvrType)
{
	BuildPacketEx(pPacket,ACCOUNT,INIT_NOTICE,buf, 2048);
	SET_DATA(pSend, ACCOUNT, INIT_NOTICE, pPacket);
	pSend->wGameSvr		= wDefSvr;
	if (bySvrType == 1)
	{
		if(m_szADNotice)
		{
			strcpy(pSend->szMsg,m_szADNotice);
			pPacket->wSize	+= strlen(m_szADNotice);
		}
		else
			pSend->szMsg[1]	= 0;
	}else
	{
		if(m_szNotice)
		{
			strcpy(pSend->szMsg,m_szNotice);
			pPacket->wSize	+= strlen(m_szNotice);
		}
		else
			pSend->szMsg[1]	= 0;
	}

	_sendPacketToClient(pClient,pPacket);
}

void ZNetCenter::GetSvrListByType(xnList* pOutList,bool bAD)
{
	while(pOutList->Count)
		pOutList->Delete(0);

	for (int i = 0; i < m_pHotSvrList->Count; i++)
	{
		int nSvrID = (int)m_pHotSvrList->Items[i];
		if (bAD)
		{
			if (nSvrID > 1100)
				pOutList->Add((void*)nSvrID);
		}else
		{
			if (nSvrID < 1100)
				pOutList->Add((void*)nSvrID);
		}
	}
}

int ZNetCenter::GetHotSvr()
{
	xnList* pTmpList = xnList::Create();
	GetSvrListByType(pTmpList,false);

	if (pTmpList->Count == 0)
	{
		pTmpList->Free();
		return 0;
	}

	int nTmpSvr = 0;
	int nRlt = rand()%pTmpList->Count;
	if (nRlt > -1 && nRlt < pTmpList->Count)
	{
		nTmpSvr = (int)pTmpList->Items[nRlt];
	}else
		nTmpSvr = (int)pTmpList->Items[0];

	while(pTmpList->Count)
		pTmpList->Delete(0);
	pTmpList->Free();

	return nTmpSvr;
}

int ZNetCenter::GetADHotSvr()
{
	xnList* pTmpList = xnList::Create();
	GetSvrListByType(pTmpList,true);

	if (pTmpList->Count == 0)
	{
		pTmpList->Free();
		return 0;
	}

	int nTmpSvr = 0;
	int nRlt = rand()%pTmpList->Count;
	if (nRlt > -1 && nRlt < pTmpList->Count)
	{
		nTmpSvr = (int)pTmpList->Items[nRlt];
	}else
		nTmpSvr = (int)pTmpList->Items[0];

	while(pTmpList->Count)
		pTmpList->Delete(0);
	pTmpList->Free();

	return nTmpSvr;
}

void ZNetCenter::RefHotSvrList(int nSvID,CListCtrl* pCtrlList,bool bClear)
{
	if (bClear)
	{
		while(m_pHotSvrList->Count)
			m_pHotSvrList->Delete(0);
	}else
	{
		bool bAdd = true;
		for (int i = 0; i < m_pHotSvrList->Count; i++)
		{
			if ((int)m_pHotSvrList->Items[i] == nSvID)
			{
				bAdd = false;
			}
		}

		if (bAdd)
			m_pHotSvrList->Add((void*)nSvID);
	}

	pCtrlList->DeleteAllItems();
	char	szTmp[256];
	for (int j = 0; j < m_pHotSvrList->Count; j++)
	{
		sprintf(szTmp,"%d", (int)m_pHotSvrList->Items[j]);
		pCtrlList->InsertItem(pCtrlList->GetItemCount(),szTmp);
	}
}

void ZNetCenter::SendSeverState(JNetClient* pClient)
{
	BuildPacketEx(pPacket,ACCOUNT,SVRLINE_STATE,buf, 1024);
	SET_DATA(pSend, ACCOUNT, SVRLINE_STATE, pPacket);
	int nCnt = 0;
	for (map<int,BYTE>::iterator itor = m_mapSeverState.begin(); itor!=m_mapSeverState.end();itor++)
	{
		int nTmpVal = 0;
		nTmpVal = nTmpVal>>16|itor->first;
		pSend->sznState[nCnt] = nTmpVal<<16|itor->second;

		nCnt++;
	}
	pSend->wCnt = nCnt;
	pPacket->wSize	+= (nCnt-1)*sizeof(int);
	_sendPacketToClient(pClient,pPacket);
}

void ZNetCenter::RefSeverState(int nSvID,int nState,CListCtrl* pCtrlList,bool bClear)
{
	if (bClear)
	{
		m_mapSeverState.clear();
	}else
	{
		map<int,BYTE>::iterator itor = m_mapSeverState.find(nSvID);
		if (itor != m_mapSeverState.end())
			itor->second = nState;
		else
			m_mapSeverState[nSvID] = nState;
	}

	pCtrlList->DeleteAllItems();
	map<int,BYTE>::iterator itor = m_mapSeverState.begin();
	char	szTmp[256];
	while(itor != m_mapSeverState.end())
	{
		sprintf(szTmp,"%d", itor->first);
		int nPos = pCtrlList->InsertItem(pCtrlList->GetItemCount(),szTmp);
		sprintf(szTmp,"%d", itor->second);
		pCtrlList->SetItemText(nPos,1,szTmp);
		itor++;
	}

}

void ZNetCenter::SendAreaName(JNetClient* pClient)
{
	//char* szAreaName = {"测试服1|测试服2|测试服3"};

	BuildPacketEx(pPacket,ACCOUNT,SVRLINE_AREA_NAME,buf, 1024);
	SET_DATA(pSend, ACCOUNT, SVRLINE_AREA_NAME, pPacket);
	pSend->byCountry = m_nDefCountry;
	//strcpy(pSend->szAreaName,szAreaName);
	//pPacket->wSize	+= strlen(szAreaName);

	_sendPacketToClient(pClient,pPacket);
}

