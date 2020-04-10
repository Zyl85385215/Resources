#pragma once
#include <map>
#include "ios_muli_packet.h"
#include "xnlist.h"

class ItemObj;
class DBControl;
class FortCenter;
class UserData;
class FortData{
public:
	FortData(FortCenter* pCenter);
	~FortData();

public:
	//ׯ԰����
	bool		CheckManorPos(int nPos);		//ׯ԰λ����
	bool		UnlockManorPos();				//����ׯ԰λ
	bool		SeedManorPos(int nPos,ItemObj* pItem);	//����
	bool		FastManorPos(int nPos,DWORD dwUin,int* nExp = NULL);			//����
	bool		FastManorPosEx(int nPos,DWORD dwUin,int* nExp = NULL);			//�¿ͻ�������

	int         GetFastCount(int nPos);
	bool		CheckFast(DWORD dwUin,int nPos,bool bOld = false);					//����ܷ�����˼���
	bool		SetFastUin(DWORD dwUin,int nPos);					//�����˼��ٺ�ʹ��

	int         GetStealCount(int nPos);
	bool		CheckSteal(DWORD dwUin,int nPos,bool bOld = false);					//����ܷ�͵��
	bool		SetStealUin(DWORD dwUin,int nPos,WORD nID);					//

	bool		GetManorReward(int nPos,UserData* pUser);			//�ջ���
	void		AddManorExp(int nExp);

	bool		CheckHarvest(bool bSend);

public:
	//����������
	bool		AddWarScore(int nScore);
	int			GetWarRankTimes(){return m_pWarRankData->nTimes;};

public:

	bool		LoadManorData(BYTE* pData,int nSize);
	bool		LoadWarRankData(BYTE* pData,int nSize);

	bool		LoadManorRecord(BYTE* pData,int nSize);

	void		SendManorData(DWORD dwUin, bool bOldC);
	void		SendFortData(DWORD dwUin, bool bOldC);
	void		SendWarRankData(DWORD dwUin);

	void		SendManorRecord(DWORD dwUin);
	void		InsertManorRecord(DWORD dwUin,WORD wVal);

	void		SendChgFortData(DWORD dwUin,BYTE byType,BYTE byPos,DWORD dwVal);

	DWORD		m_dwUin;

	void		SetUser(UserData* pUser);

	int			GetWarRankPos();
	void		SetWarRankPos(int nPos);

public:
	ManorDataEx*	m_pManorData;
	WarRankData*	m_pWarRankData;

	ManorData*  GetOldManorData();

	xnList*      m_pManorRecordList;

	void		ResetDailyData();

	bool        m_bSendHarvest;

	bool		m_bShopWarRank;
private:
	FortCenter*	m_pCenter;
	UserData*	m_pUser;
	void		_updateDBManorData();
	void		_updateDBWarRankata();
	void		_updateDBManorRecord();
};

class FortCenter{
public:
	FortData*	GetFortData(DWORD dwUin,bool bCreate = true);
	
	//bool		SaveFortData(FortData* pData);
	bool		DeleteFortData(DWORD dwUin,bool bSave);
	FortData*	LoadFortData(DWORD dwUin,bool bCreate = true);
	void		SendFortData(FortData* pData,DWORD dwUin,bool bOldC);
	FortData*	CreateNewFortData(DWORD dwUin);



	void		InitDB(DBControl* pDB);
	DBControl*	m_pDB;

	std::map<DWORD,FortData*>	m_pMapData;


private:
	bool		_insertDBFortData(FortData* pData);
};