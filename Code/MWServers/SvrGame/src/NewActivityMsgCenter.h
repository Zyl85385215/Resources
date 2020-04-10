#pragma once
#include "xnlist.h"
#include <Windows.h>


enum MSG_ACTIVITY
{
	ACTIVITY_NULL,
	ACTIVITY_ENTER,			//��ʼ�¼�
	ACTIVITY_SUCCESSFUL,	//�ɹ��¼�
	ACTIVITY_FAIL,			//ʧ���¼�
	ACTIVITY_MAX,
};

enum MSG_PLACE
{
	PLACE_NULL,
	PLACE_ZHENGZHAN,		//��ս
	PLACE_ZHAOMU,			//�佫��Ļ
	PLACE_YANWU,			//������ս
	PLACE_YIJI,				//Զ���ż�
	PLACE_LONGYANSHAN,		//����ɽ
	PLACE_TASHENLU,			//̤��·
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