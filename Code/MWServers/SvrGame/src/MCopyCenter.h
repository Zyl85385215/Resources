#pragma once

#include "xnlist.h"
#include <Windows.h>
#include <map>
#include <list>
#include "MapCopyConfig.h"
using namespace std;
class MapSrvObj;
class MapHelpObj
{
public:
	WORD	wHelpBase;
	DWORD	dwHelpID;
	DWORD	dwHelpTick;
};

class MapCopyObj
{
public:
	MapCopyObj(DWORD	dwMindex,DWORD dwUin,MCopyBase*	pMC,int nLimit = 4);
	~MapCopyObj();

	bool	CheckDelete();
	bool	CheckMember(DWORD dwUin);

	DWORD	m_dwMapIndex;
	DWORD	m_dwUin;

	MCopyBase*	m_pMCBase;
	MapSrvObj*	m_pNetSrvLink;
	DWORD	m_szdwPart[4];	//����
	xnList*		m_pPartList;	//���Ѹ������
	int		m_nPartLimit;

	DWORD	dwDelTick;

	xnList*	m_pListHelper;

	bool	m_DeleteLock;
};

class MapCopyCenter
{
public:
	MapCopyCenter();
	~MapCopyCenter();

	//���븱��  �Լ�,����Ҫ���ӳ���UIN
	MapCopyObj*	RequestEnterMCopy(DWORD dwLeadUin,DWORD	dwUin,MCopyBase*	pMC);
	MapCopyObj*	RequestEnterMCopy(DWORD dwMindex,DWORD	dwUin);
	bool	RequestLeaveMCopy(DWORD dwUin,DWORD	dwMIndex);

	bool		JoinMCopy(DWORD dwUin,MapCopyObj* pMCopy);
	MapCopyObj*	GetMCopy(DWORD dwUin);
	MapCopyObj*	GetMCopyEx(DWORD dwUin);
	MapCopyObj*	GetMCopyByMIndex(DWORD dwMIndex);
	void	UpdateSec();
	void	KickOutMember(DWORD dwMIndex);

	WORD	m_dwMaxIndex;	//���������ID��,1000��ʼ

	void    ReFreshDailyLmt();
	void    SetCopyDailyLmt(int nMCID);
	bool    CheckCanCreatCopy(int nMCID,DWORD dwUin);
private:
	

	std::map<DWORD,MapCopyObj*>	mapMCopy;
	xnList*		m_pListMCopyCache;

	list<int>	listActMapLmt;
};