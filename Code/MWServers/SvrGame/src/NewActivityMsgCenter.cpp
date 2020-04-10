#include "stdafx.h"
#include "NewActivityMsgCenter.h"
#include "UserData.h"
#include "FsGameCenter.h"

NAMsgCenter::NAMsgCenter(UserData* pData)
{
	m_pWaitList = xnList::Create();
	m_pParent = pData;
}

NAMsgCenter::~NAMsgCenter()
{
	if(m_pWaitList)
	{
		while(m_pWaitList->Count)
			delete (ActivityMsg*)m_pWaitList->Delete(0);
		m_pWaitList->Free();
	}
}

void NAMsgCenter::AddMsg(BYTE byac,BYTE bypla,int nVal)
{
	ActivityMsg* pMsg = new ActivityMsg();
	pMsg->byActivity = byac;
	pMsg->byPlace = bypla;
	pMsg->nVal = nVal;
	m_pWaitList->Add(pMsg);
}

void NAMsgCenter::Update()
{
	while(m_pWaitList->Count)
	{
		ActivityMsg* pMsg = (ActivityMsg*)m_pWaitList->Delete(0);
		_dealWithMsg(pMsg);
	}
}


bool NAMsgCenter::_dealWithMsg(ActivityMsg* pMsg)
{
	_checkDailyActivity(pMsg);
	return true;
}

bool NAMsgCenter::_checkDailyActivity(ActivityMsg* pMsg)
{
	
	return true;
}