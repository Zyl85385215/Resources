#pragma once

#include "xnThread.h"

class NetGateCenter
{
public:
	NetGateCenter();
	~NetGateCenter();

	void		InitNet();
	void		Update25();

private:

	xnThread*	m_pThread;
};
extern NetGateCenter* g_pMainGate;