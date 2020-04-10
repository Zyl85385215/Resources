#pragma once

#include <Windows.h>
#include "xnlist.h"
#include "DBControl.h"

struct	AchievementUserObj
{
	int nUin;
	int nId;
	int nCnt;
	bool ifDone;
	bool ifShow;
	int	nFloor;
	int nGroup;
	BYTE byState;	// 1update,2insert
};

class	AchievementCenter
{
public:
	AchievementCenter(DBControl*	pDB);
	~AchievementCenter();
	AchievementUserObj*	GetUserObj(int nUin,int nID);
	void	GetUserObjList(int nUin,OUT xnList* pList);
	bool	AddAchievementVal(int nUin,int nID,int nVal);
	bool	SetAchievementVal(int nUin,int nID,int nVal);
	bool	DoneAchievement(int nUin,int nID);

	void	DeleteUser(int nUin);		//删除角色时的操作

	xnList*	m_pAchievementList;
	bool	DBInit();
	bool	_insertObj(AchievementUserObj* pObj);
	bool	_updateObj(AchievementUserObj* pObj);
	bool	_deleteObj(AchievementUserObj* pObj);
	DBControl*	m_pDB;
};

class UserData;
class	UserAchievement
{
public:
	UserAchievement(UserData* pUser);
	~UserAchievement();
	AchievementUserObj* GetObj(int nID);
	bool	AddAchievementVal(int nID,int nVal);
	bool	SetAchievementVal(int nID,int nVal);
	bool	DoneAchievement(int nID);
	void	SaveAll();

	void	SendAll();
	void	SendObj(AchievementUserObj* pObj);
	void	SendShow(int nID);
	void	CheckShow();
	void	RequestForGift(int i);

	xnList*	m_pList;
	bool	_insertObj(AchievementUserObj* pObj);
	bool	_updateObj(AchievementUserObj* pObj);
	bool	_deleteObj(AchievementUserObj* pObj);
	UserData*	m_pUser;
	int		m_nPoint;
};
