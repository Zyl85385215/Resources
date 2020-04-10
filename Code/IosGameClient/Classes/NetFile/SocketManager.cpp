#include "SocketManager.h"
static pthread_t mainSocketThread;
static pthread_t sendSocketThread;

static void* dealPacket(void* data)
{
	SocketManager * pSocketManager = (SocketManager *)data;
	if (pSocketManager)
	{
		pSocketManager->PacketProcess();
	}
	return 0;
}

static void* dealSendPacket(void* data)
{
	SendPacketPool * pSendPacketPool = (SendPacketPool*)data;
	if (pSendPacketPool)
	{
		pSendPacketPool->UpdateSendPacket();
	}
	return 0;
}

SocketManager::SocketManager()
{
	nRemainSize = 0;
	nConnectState = INVALID_SOCKET;
	m_bActive		= false;
	m_bReConnect	= false;
	m_pSendPacketPool	= new SendPacketPool(this);
	pthread_create(&mainSocketThread, NULL, dealPacket, this);
}

SocketManager::~SocketManager()
{
	delete m_pSendPacketPool;
}

void SocketManager::SendPacket(ZPacket * pPacket)
{
	if(m_bActive)
	{
		ZPacket * tempSendPacket = (ZPacket*)malloc(pPacket->wSize+sizeof(ZPacket)-1);
		memcpy(tempSendPacket,pPacket,pPacket->wSize+sizeof(ZPacket)-1);
		tempSendPacket->dwKey	= m_xRandom.Random_Int(0,10000);
		m_pSendPacketPool->AddSendPacket(tempSendPacket);
	}
}



void SocketManager::_handleSendPacket(ZPacket * pPacket)
{
	const char * pBuff = (const char *)pPacket;
	mainSocket.Send(pBuff,pPacket->wSize+sizeof(ZPacket)-1,0);
}

void SocketManager::PacketProcess()
{
	while (1)
	{
		if(nConnectState <= 0)
		{
			if(m_bActive || m_bReConnect)
			{
				mainSocket.Init();
				mainSocket.Create(AF_INET,SOCK_STREAM,0);
				nConnectState = mainSocket.Connect(m_szIp,m_nPort);
				if(nConnectState>0)
				{
					mainSocket.Send("FUCK",4,0);
					m_bActive	= true;
				}
				else
					m_bActive	= false;
				m_bReConnect	= false;
				
			}
		}
		else
		{
			if(m_bActive == false || m_bReConnect)
			{
				nConnectState	= INVALID_SOCKET;
				mainSocket.Close ();
				m_bActive	= false;
			}
			else
			{
				int nLen = mainSocket.Recv((char*)pbRemainbuf+nRemainSize,2048,0);
				if(nLen == -1)
					DisConnect ();
				else if(nLen>0)
				{
					//memcpy(pbRemainbuf+nRemainSize,bTempBuff,nLen);
					nRemainSize+=nLen;
					int	nDealLen	= 0;
					BYTE* pbDealPos	= pbRemainbuf;
					while (nRemainSize>=sizeof(ZPacket)-1)
					{
						ZPacket* pRecvP	= (ZPacket*)pbDealPos;
						int nWholePacketSize = pRecvP->wSize+sizeof(ZPacket)-1;
						if (nRemainSize>=nWholePacketSize)
						{
							ZPacket * pPacket = (ZPacket *)malloc(nWholePacketSize);
							memcpy(pPacket,pRecvP,nWholePacketSize);
							m_PacketPool.AddPacket(pPacket);
							nDealLen	+= nWholePacketSize;
							nRemainSize -= nWholePacketSize;
							pbDealPos	+= nWholePacketSize;

						}
						else
							break;
					}
					memmove(pbRemainbuf,pbDealPos,nRemainSize);
				}
			}
		}
		
#ifdef WIN32
		Sleep(20);
#else
		sleep(0.02f);
#endif
	}
}

int SocketManager::Connect( char * szIp, int nPort )
{
	if(m_bReConnect)
		return 1;


	strcpy(m_szIp,szIp);
	m_nPort	= nPort;
	if(m_bActive)
	{
		DisConnect ();

		m_bReConnect	= true;
	}
	else
	{
		DisConnect ();
		m_bActive	= true;
	}


	return 1;
}

ZPacket* SocketManager::GetPacket()
{
	return m_PacketPool.GetNextPacket ();
}

void SocketManager::Filp()
{
	m_PacketPool.Flip ();
}

void SocketManager::DisConnect()
{
	mainSocket.Close ();
	//shutdown(mainSocket,2);
	m_bActive	= false;
}
PacketPool::PacketPool()
{
	pthread_mutex_init(&mainPacketLock, NULL);
	m_pFrontPacketList = xnList::Create();
	m_pBackPacketList = xnList::Create();
}

PacketPool::~PacketPool()
{
	pthread_mutex_destroy(&mainPacketLock);
}

void PacketPool::Flip()
{
	if(m_pBackPacketList->Count > 0) 
	{
		return;
	}
	pthread_mutex_lock(&mainPacketLock);
	xnList * pTempList = m_pFrontPacketList;
	m_pFrontPacketList = m_pBackPacketList;
	m_pBackPacketList = pTempList;
	pthread_mutex_unlock(&mainPacketLock);
}

void PacketPool::AddPacket(ZPacket * pPacket)
{
	pthread_mutex_lock(&mainPacketLock);
	m_pFrontPacketList->Add(pPacket);
	pthread_mutex_unlock(&mainPacketLock);
}

ZPacket * PacketPool::GetNextPacket()
{
	return (ZPacket*)m_pBackPacketList->Delete(0);
}

SendPacketPool::SendPacketPool(SocketManager* pParent)
{
	pthread_mutex_init(&sendPacketLock, NULL);
	pthread_cond_init(&sendPacketSign,NULL);
	m_pParent	= pParent;
	m_pSendPacketList = xnList::Create();
	pthread_create(&sendSocketThread, NULL, dealSendPacket, this);
}

SendPacketPool::~SendPacketPool()
{
	pthread_mutex_destroy(&sendPacketLock);
}

void SendPacketPool::AddSendPacket( ZPacket * pPacket )
{
	pthread_mutex_lock(&sendPacketLock);
	m_pSendPacketList->Add(pPacket);
	pthread_mutex_unlock(&sendPacketLock);

	pthread_cond_signal(&sendPacketSign);
}

void SendPacketPool::UpdateSendPacket()
{
	while(1)
	{
		pthread_mutex_lock(&sendPacketLock);
		while (m_pSendPacketList->Count)
		{
			ZPacket * pPacket = (ZPacket *)m_pSendPacketList->Delete(0);
			if (pPacket)
			{
				m_pParent->_handleSendPacket(pPacket);
				free(pPacket);
			}
		}
		pthread_cond_wait(&sendPacketSign,&sendPacketLock);
		pthread_mutex_unlock(&sendPacketLock);


		
//#ifdef WIN32
//		Sleep(10);
//#else
//		sleep(0.01f);
//#endif

	}
}
