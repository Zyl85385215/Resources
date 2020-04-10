#pragma once
#include "ios_user_packet.h"

#include "CtrlObj.h"
class TeamMenu{
public:
	TeamMenu();
	~TeamMenu();
	void	Open(int nEff);
	void	Close();
	void	Update();
	bool	m_bUpdate;
	DForm*		m_pForm;
	DWORD   m_dwLeaderUin;

	DWORD		GetLeadID();
	_TeamNet	m_xUser[TEAM_MAXUSER];

	int		GetMyTeamState();
	int		GetTeamCount();
	int		GetTeamedCount();
	int		m_nTeamMemberPos;
	DWORD	GetSelectMemberID();
	int		GetSelectMemberPos();
	bool	IsTeamMember(DWORD	dwID);

	_TeamNet* GetStudentInfo();

	DComponent*	m_pCompName[TEAM_MAXUSER-1];
	DComponent*	m_pCompHp[TEAM_MAXUSER-1];
	DComponent*	m_pCompJob[TEAM_MAXUSER-1];
	DComponent*	m_pCompLv[TEAM_MAXUSER-1];
	DComponent*	m_pCompD1[TEAM_MAXUSER-1];
	DComponent*	m_pCompD2[TEAM_MAXUSER-1];
	DComponent*	m_pCompD3[TEAM_MAXUSER-1];
	DComponent*	m_pCompD4[TEAM_MAXUSER-1];
	DComponent*	m_pCompSelect[TEAM_MAXUSER-1];
	DComponent*	m_pCompLeader[TEAM_MAXUSER-1];
};

class TeamDropMenu
{
public:
	TeamDropMenu();
	~TeamDropMenu();
	void	Close();
	void	Updata();

	DForm*		m_pForm;
	DWORD       m_dwWaitTDNeed;
	DWORD       m_dwItemID;
	xnList*     m_pDropList;
};

class DropShowMenu
{
public:
	DropShowMenu();
	~DropShowMenu();
	void	Updata();

	DForm*		m_pForm;
	DWORD       m_dwWaitTick;
	xnList*     m_pList;
};