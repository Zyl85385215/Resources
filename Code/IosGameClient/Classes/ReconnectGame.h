#pragma once

#include "PacketCenter.h"
#include "singleton.h"


class ReconnectModel	: public PacketListener, public CSingleton<ReconnectModel>
{
public:
	ReconnectModel();
	~ReconnectModel();
	//��ɫֻ��1��,��������

	void		SetServerInfo(char* szIP, int nPort);

	virtual	bool PacketProcess(ZPacket* pPacket);
	void		Update();
	
	void		SetModelWorked(bool bVal);
	bool		GetModelWorked()	{return m_bWorking;}
	
private:
	char*		m_szServerIP;
	int			m_nPort;
	bool		m_bWorking;
	DWORD		m_dwReTryTick;
};