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

	//���ݿ���ص�ʱ���ʼ����Ա,���÷��ͻ���
	void		AddMemberFromDB(UserSimple* pUS);
	//������ֻ�дӳ�Ա�б�
	bool		AcceptRequest(UserSimple*	pUS);

	bool		RemoveMember(UserSimple*	pUS);

	bool		RequestJoin(UserSimple* pUs);
	bool		RequestCancelJoin(UserSimple* pUs);
	bool		RemoveRequest(UserSimple* pUs);
	void		AddHomeExp(int nVal);
	bool		AddHomeLv();
	bool		AddBuildLv(BYTE byType);
	int			GetDutyCount(BYTE byDuty);
	char*		GetMasterName();		//��û᳤����

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
	
	//���߳�Ա
	xnList*		m_pListActiveUS;
	
	//��Ա�б�
	xnList*		m_pList;
	//�����б�
	xnList*		m_pListRequest;

	xnList*		m_pListBattle;
	
	xnList*		m_pBattleResult;

	HomeBuildTag*	m_pBuildTag;
	bool		m_ifLvUp;
	bool		m_ifMemberCh;
};