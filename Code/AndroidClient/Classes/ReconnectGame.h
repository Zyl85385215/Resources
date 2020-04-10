#pragma once

#include "PacketCenter.h"
#include "singleton.h"


class ReconnectModel	: public PacketListener, public CSingleton<ReconnectModel>
{
public:
	ReconnectModel();
	~ReconnectModel();
	//角色只有1个,不需设置

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