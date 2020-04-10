#pragma once

#include "singleton.h"
#include "xnlist.h"
#include <Windows.h>


class P2pAskObj;
class P2pAskCenter	:	public	CSingleton<P2pAskCenter>
{
public:
	P2pAskCenter();
	~P2pAskCenter();

	void		UpdateCheck();
	bool		AddP2pAskObj(DWORD	dwSend, DWORD	dwRecv, BYTE bType, char* szName);
	P2pAskObj*	GetMyPAObj(DWORD	dwUin);
	P2pAskObj*	GetMyPAIndex(int	nIndex);
private:
	CRITICAL_SECTION	m_xPAskLock;
	int					m_nMaxIndex;
	xnList*				m_pList;


};