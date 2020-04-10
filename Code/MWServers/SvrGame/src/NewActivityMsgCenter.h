#pragma once
#include "xnlist.h"
#include <Windows.h>


enum MSG_ACTIVITY
{
	ACTIVITY_NULL,
	ACTIVITY_ENTER,			//开始事件
	ACTIVITY_SUCCESSFUL,	//成功事件
	ACTIVITY_FAIL,			//失败事件
	ACTIVITY_MAX,
};

enum MSG_PLACE
{
	PLACE_NULL,
	PLACE_ZHENGZHAN,		//征战
	PLACE_ZHAOMU,			//武将招幕
	PLACE_YANWU,			//演武挑战
	PLACE_YIJI,				//远古遗迹
	PLACE_LONGYANSHAN,		//龙炎山
	PLACE_TASHENLU,			//踏神路
	PLACE_MAX,		
};

struct ActivityMsg{
	BYTE byActivity;
	BYTE byPlace;
	int nVal;
};

class UserData;

class NAMsgCenter
{
public:
	NAMsgCenter(UserData* pData);
	~NAMsgCenter();
	void AddMsg(BYTE byac,BYTE bypla,int nVal=0);
	void Update();

private:
	bool	_dealWithMsg(ActivityMsg* pMsg);
	bool	_checkDailyActivity(ActivityMsg* pMsg);
	xnList* m_pWaitList;
	UserData* m_pParent;
};