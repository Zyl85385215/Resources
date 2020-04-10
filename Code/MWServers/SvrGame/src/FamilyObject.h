#pragma once

#include "xnlist.h"
#include <Windows.h>

enum
{

	HOME_DUTY_NORMAL,
	HOME_DUTY_ADV,
	HOME_DUTY_MNG,
	HOME_DUTY_BOSS,
};

class UserSimple;
class HomeBuildTag;
class HomeObj
{
public:
	HomeObj();
	~HomeObj();

	//数据库加载的时候初始化成员,不用发客户端
	void		AddMemberFromDB(UserSimple* pUS);
	//加入帮会只有从成员列表
	bool		AcceptRequest(UserSimple*	pUS);

	bool		RemoveMember(UserSimple*	pUS);

	bool		RequestJoin(UserSimple* pUs);
	bool		RequestCancelJoin(UserSimple* pUs);
	bool		RemoveRequest(UserSimple* pUs);
	void		AddHomeExp(int nVal);
	bool		AddHomeLv();
	bool		AddBuildLv(BYTE byType);
	int			GetDutyCount(BYTE byDuty);
	char*		GetMasterName();		//获得会长名字

	void		AddActiveMember(DWORD dwUin);
	void		RemoveActiveMember(DWORD dwUin);

	int			GetOccupyCount();

	void		LoadBuildTag(BYTE* pData,int nSize);

	bool		CheckJoin(DWORD dwUin);
	bool		CheckChangeLeader();

	WORD		m_wHomeID;
	char		m_szName[20];
	char		m_szDesc[100];

	BYTE		m_byLv;
	DWORD		m_dwExp;
	DWORD		m_dwCreateUin;
	char		m_szCreateName[20];
	BYTE		m_byCountry;
	
	//在线成员
	xnList*		m_pListActiveUS;
	
	//成员列表
	xnList*		m_pList;
	//申请列表
	xnList*		m_pListRequest;

	xnList*		m_pListBattle;
	
	xnList*		m_pBattleResult;

	HomeBuildTag*	m_pBuildTag;
	bool		m_ifLvUp;
	bool		m_ifMemberCh;
};