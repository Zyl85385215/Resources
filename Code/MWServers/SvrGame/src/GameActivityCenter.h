#pragma once


#include "xnlist.h"
#include <Windows.h>
#include "GameActivityCfg.h"
#include <map>


class UserData;
class BossInfo;
class BossState;

class GameActivityCenter
{
public:
	GameActivityCenter();
	~GameActivityCenter();
	void UpdateSec();
	void CheckDaylyList();
	void RequestForInfo(UserData* pUser,int nFlag);
	void RequestEnter(UserData* pUser,int nID);
	void EnterActivity(UserData* pUser,ActivityObj* pObj);
	void FreeActivityMap(int nID,int nMIndex);

	xnList* m_pTodayList;
	xnList* m_pOtherList;
	int m_nShowFlag;
	std::map<int,xnList*> mapActivityMCopy;

	//ת��
	void RequestTurntableStart(UserData* pUser);
	void RequestTurntableStartForTen(UserData* pUser);
	void RequestTurntableGift(UserData* pUser,int nPos);

	//��Ծ��
	void ResetActiveDegree(UserData* pUser);
	void ResetActiveDegree(UserData* pUser, BYTE byPos);
	void SaveActiveDegree(UserData* pUser);
	void LoadActiveDegree(UserData* pUser,BYTE* byData,int nSize);
	void SendActiveDegree(UserData* pUser,int nPos);
	void AddDegreeVal(UserData* pUser,BYTE byPos,int nVal);
	int	GetDegreeRlt(UserData* pUser);
	bool RequestDegreeAward(UserData* pUser);

	//����boss
	void RequestBossState(UserData* pUser);

	//��������
	void JoinSendActive(UserData* pUser);

	//���������
	void SendWarnTipActive(ActivityObj* pObj);
	void FilterSendListByAct(xnList* pList,ActivityObj* pObj);
	xnList* m_pRubbishList;

	//GM���ûƫ��ʱ��
	int     m_nOffsetTime;

	BYTE		m_bActivityTime[24];
	void		SetActivyTime(ActivityObj* pActivityObj,int nHour);

	//�Ƿ��ڻʱ����
	bool  CheckBeingActive(int nActID);
	bool  CheckActiveEnd(int nActID);

	bool  m_bKillActBoss;

	tm	  m_tmTmp;
};