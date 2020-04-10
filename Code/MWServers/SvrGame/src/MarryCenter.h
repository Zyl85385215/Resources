#pragma once

#include <Windows.h>
#include "xnlist.h"
#include <map>
#include "DBControl.h"
#include "NumberData.h"
#include "TimeControl.h"

class UserSimple;
class MarryData
{
public:
	MarryData();
	~MarryData();
	DWORD	m_dwID;			//·òÆÞÐÅÏ¢ID
	DWORD	m_dwManValue;
	DWORD	m_dwWomanValue;
	DWORD	m_dwManValue2;
	DWORD	m_dwWomanValue2;
	UserSimple*	m_pMan;	
	UserSimple*	m_pWoman;
	NumberData* m_pManSkill;		//·òÆÞ¼¼ÄÜ	0ÉúÃü	1Îï¹¥	2Îï·À	3Ä§¹¥	4Ä§·À
	NumberData* m_pWomanSkill;
	BYTE	m_byState;		//1¶©»é 2½á»é 0É¾³ý	3Ä£·¶·òÆÞ
	CMTime		m_timeMarry;
	BYTE	m_byAwardLv;
	int		m_nMEnemyCnt;
	int		m_nWEnemyCnt;
	int		m_nMarryCnt;
};

struct MarryRequestData
{
	UserSimple* pUser;
	int		nVal;
	BYTE	byType;		//0Õ÷»éÁÐ±í
	DWORD	dwEndTick;
	bool	ifDelete;
};

struct MarryCakeObj
{
	int nCnt;
	int nNum;
	int nMapIndex;
	int nPosx;
	int nPosy;
};

struct MarryStreetObj
{
	MarryData* pData;
	int nMapIndex;
	int nMasterID;
	xnList* pFollowList;
	DWORD dwEndTick;
	POINT ptEnd;
};

class UserData;
class MarryCenter
{
public:
	MarryCenter(DBControl*	pDB);
	~MarryCenter();
	void	DBInit();	
	void	Update();
	void	AddCakeEnemy(UserData* pUser,int nCnt,int nNum);
	void	GetRelationByuin(int nUin,OUT xnList* pList);
	MarryData* GetExistRelation(int nUin);
	MarryData* GetRelation(int nTuin,int nPuin);
	MarryRequestData*	GetRequest(int nUin);
	bool	ClearRequest(int nPuin);
	bool	DeleteObj(int nTuin,int nPuin);

	bool	RequestInstruct(UserData* pUser);		//Õ÷»é
	void	GetRequestList(UserData* pUser,int nFlag);

	void	BreakRelationship(UserData* pUser,int nID);
	bool	GetEngaged(DWORD dwUin1,DWORD dwUin2);		//¶©»é 
	bool	RequestMarry(UserData* pUser,int nID);		//½á»é
	bool	CheckMarryConditions(UserData* pUser,int nID);
	void	RequestTotalInfo(UserData* pUser);
	void	RequestUpSkill(UserData* pUser,int nID,BYTE byPos);
	void	AddMarryValue(UserData* pUser,int nID,int nVal);
	void	AddMarryValue(UserData* pUser,int nID,int nVal,bool ifMan);
	void	StreetMarry(UserData* pUser,int nID,BYTE byType);		//Ñ²½Ö
	void	CallStreetEnemy(MarryData* pData,UserData* pUser1,UserData* pUser2,int nCnt);
	void	SendRing(UserData* pUser,int nID,BYTE byType);
// 	int		CheckMarryTime(int nID);
// 	bool	RequestYearlyAward(int nID);
	void	RequestAward(UserData* pUser,int nID,BYTE byType);

	bool	DeleteObj(MarryData* pObj);

	void	DeleteUser(DWORD dwUin);		//É¾³ý½ÇÉ«Ê±²Ù×÷


	DWORD	m_dwMaxID;
	std::map<DWORD,MarryData*>	m_mapMarry;
	int		m_nRListFlag;
	xnList*	m_pRequestList;
	xnList*	m_pCakeList;
	DWORD	m_dwCakeTick;
	MarryStreetObj* m_pStreet;		//Ñ²ÓÎ
private:
	DBControl*	m_pDB;
	int		_getMaxID();
	bool	_insertObj(MarryData* pObj);
	bool	_updateObj(MarryData* pObj);
	bool	_updateSkill(MarryData* pObj,bool ifMan);
	bool	_deleteObj(MarryData* pObj);

	bool	_insertRequest(MarryRequestData* pObj);
	bool	_deleteRequest(MarryRequestData* pObj);
};

extern MarryCenter* g_pMarryCenter;

class UserMarry
{
public:
	UserMarry(UserData* pUser);
	~UserMarry();
	void	Update();
	void	AddData(MarryData* pData);
	void	DeleteData(int i);

	void	SendAll();
	void	SendInstruct();
	void	SendShow();
	void	SendDataAdd(MarryData* pData);
	void	SendDataDelete(MarryData* pData);
	void	SendDataChanged(MarryData* pData);
	void	ResetObj();
	void	ResetEff();

	bool	MarryExpAdd();

	MarryData* GetRelation();

	UserData*	m_pUser;

	bool		m_ifInstruct;
	bool		m_SendInstruct;

	bool		m_ifShow;
	bool		m_ifSendShow;

	xnList*		m_pRelationList;
};