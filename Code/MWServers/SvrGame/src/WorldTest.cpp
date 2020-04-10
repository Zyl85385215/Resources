#include "stdafx.h"
#include "WorldTest.h"
#include "UserData.h"
#include "ios_activity_packet.h"
#include "FsGameCenter.h"
#include "UserFlag.h"


int CompareTestRlt(const void * Item1, const void * Item2)
{
	WTestInfo* p1 = *(WTestInfo**)Item1;
	WTestInfo* p2 = *(WTestInfo**)Item2;
	return p2->nStar - p1->nStar;
}

WorldTestCenter::WorldTestCenter(DBControl*	pDB)
{
	m_pDB = pDB;
	m_pList = xnList::Create(); 
	m_pWaitList = xnList::Create();
	m_byState = TEST_STATE_WAIT;
	m_ifSend = false;
	m_ifCheckPlace = false;
}

WorldTestCenter::~WorldTestCenter()
{
	if(m_pWaitList)
	{
		while(m_pWaitList->Count)
			delete (WWaitObj*)m_pWaitList->Delete(0);
		m_pWaitList->Free();
	}
	if(m_pList)
	{
		while(m_pList->Count)
			delete (WTestInfo*)m_pList->Delete(0);
		m_pList->Free();
	}
}

void WorldTestCenter::SetState(BYTE byState)
{
	if(byState >= TEST_STATE_NULL)
		return;
	if(m_byState != byState)
	{
		m_byState = byState;
		m_ifSend = true;
	}
}

WTestInfo* WorldTestCenter::AddNewObj(UserData* pUser)
{
	if(pUser == NULL)
		return NULL;
	WTestInfo* pObj = new WTestInfo();
	memset(pObj,0,sizeof(WTestInfo));
	pObj->nUin = pUser->m_dwUin;
	strcpy(pObj->szName,pUser->m_szNick);
	pObj->nStar = 0;
	pObj->pObj = NULL;
	RandTest(pObj);
	pObj->nPlace = m_pList->Count+1;
	_InsertInfo(pObj);
	m_pList->Add(pObj);
	return pObj;
}

WTestInfo* WorldTestCenter::GetObjByUin(DWORD dwUin)
{
	for(int i = 0; i < m_pList->Count; i ++)
	{
		WTestInfo* pObj = (WTestInfo*)m_pList->Items[i];
		if(pObj->nUin == dwUin)
			return pObj;
	}
	return NULL;
}

void WorldTestCenter::RandTest(WTestInfo* pObj)
{
	pObj->pObj = TestLibraryCfg::GetInstance().RandTest();
	if(pObj->pObj)
	{
		for(int i = 0; i < 4; i ++)
		{
			pObj->szCh[i] = i+1;
		}
		for(int i = 0; i < 4; i ++)
		{
			int j = rand()%4;
			if(i != j)
			{
				int nTemp = pObj->szCh[i];
				pObj->szCh[i] = pObj->szCh[j];
				pObj->szCh[j] = nTemp;
			}
		}
	}
	pObj->nEndSec = time(0)+16;
}

void WorldTestCenter::Update()
{
	if(m_pWaitList->Count > 0)
	{
		DWORD dwNow = GetTickCount();
		for(int i = m_pWaitList->Count-1; i >= 0; i --)
		{
			WWaitObj* pObj = (WWaitObj*)m_pWaitList->Items[i];
			if(dwNow >= pObj->dwTick)
			{
				SendQuestion(pObj->pInfo);
				delete (WWaitObj*)m_pWaitList->Delete(i);
			}
		}
	}
}

void WorldTestCenter::Update25()
{
	if(m_byState == TEST_STATE_PAUSE)
		return;
	ActivityObj* pObj = GameActivityCfg::GetInstance().GetObj(1001);
	if(pObj == NULL)
		return;
	tm	tmNow	= CMTime::GetCurTime();
	DWORD dwTemp = (tmNow.tm_hour<<16) + tmNow.tm_min;
	
	if(dwTemp >= pObj->dwEndTime || dwTemp < pObj->dwStartTime)
	{
		if(m_byState == TEST_STATE_ON)
		{
			SetState(TEST_STATE_WAIT);
			m_ifCheckPlace = true;
		}
	}
	else
	{
		if(m_byState == TEST_STATE_OFF)
			SetState(TEST_STATE_REFLESH);
	}
	switch(m_byState)
	{
	case TEST_STATE_REFLESH:
		{
// 			for(int i = 0; i < m_pList->Count; i ++)
// 			{
// 				WTestInfo* pInfo = (WTestInfo*)m_pList->Items[i];
// 				pInfo->nCount = 0;
// 				pInfo->nStar = 0;
// 				pInfo->nCorrect = 0;
// 				RandTest(pInfo);
// 				pInfo->nEndSec = 0;
// 				pInfo->byGiftFlag = 0;
// 				SetState(TEST_STATE_WAIT);
// 				_UpdateInfo(pInfo);
// 			}
			while(m_pWaitList->Count)
				delete (WWaitObj*)m_pWaitList->Delete(0);
			while(m_pList->Count)
				delete (WTestInfo*)m_pList->Delete(0);
			SetState(TEST_STATE_WAIT);
			_ClearSql();
		}
		break;
	case TEST_STATE_WAIT:
		{			
			if(dwTemp >= pObj->dwEndTime ||dwTemp < pObj->dwStartTime)
				SetState(TEST_STATE_OFF);
			else
				SetState(TEST_STATE_ON);
		}
		break;
	}
	if(m_ifSend)
	{
		m_ifSend = false;
		for(int i = 0; i < m_pList->Count; i ++)
		{
			WTestInfo* pObj = (WTestInfo*)m_pList->Items[i];
			SendQuestion(pObj);
		}
		return;
	}
	if(m_ifCheckPlace)
	{
		m_ifCheckPlace = false;
		for(int i = 0; i < m_pList->Count; i ++)
		{
			WTestInfo* pObj = (WTestInfo*)m_pList->Items[i];
			if(pObj->nPlace != i+1)
			{
				pObj->nPlace = i+1;
			//	SendRankInfo(pObj->nUin);
			}		
		}
		return;
	}
	CheckQuestionTime();
}

void WorldTestCenter::RequestStart(UserData* pUser)
{
	if(pUser == NULL)
		return;
	WTestInfo * pObj = GetObjByUin(pUser->m_dwUin);
	if(pObj == NULL)
		pObj = AddNewObj(pUser);
	BuildPacketEx(pPacket,RPGACTIVITY,S_SEND_START_RLT,buf,1024);
	SET_DATA(pData,RPGACTIVITY,S_SEND_START_RLT,pPacket);
	if(m_byState == TEST_STATE_OFF)
	{
		pData->isRlt = -1;
		SendRankInfo(pUser->m_dwUin);
	}
	else if(m_byState == TEST_STATE_PAUSE)
	{
		pData->isRlt = -2;
		//SendRankInfo(pUser->m_dwUin);
	}
	else if(m_byState == TEST_STATE_REFLESH || m_byState == TEST_STATE_WAIT)
	{
		pData->isRlt = -3;
		//SendRankInfo(pUser->m_dwUin);
	}
	else if(pObj->nCount >= TestAwardCfg::GetInstance().m_nACount)
	{
		pData->isRlt = 0;
		SendRankInfo(pUser->m_dwUin);
	}
	else
	{
		if(pObj->pObj == NULL)
			RandTest(pObj);
		if(pObj->nEndSec == 0)
			pObj->nEndSec = time(0) + 15;
		pData->isRlt = 1;
	}
	TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
	SendQuestion(pObj);
}

void WorldTestCenter::RequestForGift(UserData* pUser)
{
	if(m_byState != TEST_STATE_OFF)
	{
		pUser->SendWarnMsg("活动结束即可领取奖励");
		return;
	}
	WTestInfo* pObj = GetObjByUin(pUser->m_dwUin);
	if(pObj == NULL)
	{
		pUser->SendWarnMsg("您未参加本次答题，不可领取奖励");
		return;
	}
	if(pObj->byGiftFlag == 1)
	{
		pUser->SendWarnMsg("您已领取本次奖励，不能重复领取");
		return;
	}
	AwardObj* pAward = TestAwardCfg::GetInstance().GetAwardByRank(pObj->nPlace-1);
	if(pAward == NULL || pObj->nStar == 0)
	{
		pUser->SendWarnMsg("您未进入排行榜");
		return;
	}
	pObj->byGiftFlag = 1;
	_UpdateInfo(pObj);
	std::string strMsg("");
	char szTemp[128];
	int nRltExp = pAward->nExp*pUser->m_xUWealth[UW_LV]*pUser->m_xUWealth[UW_LV];
	int nRltMoney = pAward->nMoney*pUser->m_xUWealth[UW_LV]*pUser->m_xUWealth[UW_LV];
	pUser->m_xUWealth.AddWealth(UW_EXP,nRltExp);
	pUser->m_xUWealth.AddWealth(UW_GOLD,nRltMoney,1,NULL,UW_GOLD_TEST);
	sprintf(szTemp,"恭喜您获得%d经验，%d金币",nRltExp,nRltMoney);
	strMsg.append(szTemp);
	for(int i = 0; i < pAward->pItemList->Count; i ++)
	{
		int nID = (int)pAward->pItemList->Items[i];
		ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(nID);
		if(pBase)
		{
			pUser->m_pItemCenter->AddItemByID(nID,1,0,ITEM_LOG_TEST);
			sprintf(szTemp,",1件%s",pBase->szName);
			strMsg.append(szTemp);
		}
	}
	pUser->SendWarnMsg(strMsg.c_str());
}

void WorldTestCenter::GiveUpTest(UserData* pUser)
{
	if(pUser == NULL)
		return;
	WTestInfo * pObj = GetObjByUin(pUser->m_dwUin);
	if(pObj == NULL)
		return;
	if(pObj->nCount >= TestAwardCfg::GetInstance().m_nACount)
		return;
	pObj->nCount = TestAwardCfg::GetInstance().m_nACount;
	pObj->nEndSec = 0;
	_UpdateInfo(pObj);
	SendQuestion(pObj);
	m_pList->Sort(CompareTestRlt);
	m_ifCheckPlace = true;
}

void WorldTestCenter::CheckQuestionTime()
{
	if(m_byState == TEST_STATE_ON)
	{
		for(int i = 0; i < m_pList->Count; i ++)
		{
			WTestInfo* pObj = (WTestInfo*)m_pList->Items[i];
			if(pObj->nCount >= TestAwardCfg::GetInstance().m_nACount)
				continue;
			if(pObj->nEndSec > 0 && pObj->nEndSec <= time(0))
			{
				UserData* pUser = g_pGameCenter->GetPlayer(pObj->nUin);
				if(pUser == NULL)
				{
					pObj->nEndSec = 0;
				}
				else
				{
					pObj->nCount ++;
					pUser->m_pUserRecord->RunLuaActivityFunc(1001,pObj->pObj->nStar,0);
					RandTest(pObj);
					//SendQuestion(pUser);
					AddWaitSend(pObj);
					_UpdateInfo(pObj);
					if(pObj->nCount >= TestAwardCfg::GetInstance().m_nACount)
					{
						SupportAwards(pObj,pUser);
						SendRankInfo(pUser->m_dwUin);
					}
					pUser->PlayEffect(nEffectWrong);
				}
			}
		}
	}
}

bool WorldTestCenter::AnswerQuestion(UserData* pUser,BYTE byCh)
{
	if(m_byState != TEST_STATE_ON)
		return false;
	if(byCh > 3)
		return false;
	WTestInfo* pObj = GetObjByUin(pUser->m_dwUin);
	if(pObj == NULL || pObj->pObj == NULL)
		return false;
	if(pObj->nCount >= TestAwardCfg::GetInstance().m_nACount)
		return false;
	if(pObj->szCh[byCh] == 1)
	{
		pObj->nStar += pObj->pObj->nStar;
		pObj->nCount ++;
		pObj->nCorrect ++;
		pUser->m_pUserRecord->RunLuaActivityFunc(1001,pObj->pObj->nStar,1);
		RandTest(pObj);
		//SendQuestion(pUser);
		AddWaitSend(pObj);
		_UpdateInfo(pObj);
		m_pList->Sort(CompareTestRlt);
		m_ifCheckPlace = true;
		if(pObj->nCount >= TestAwardCfg::GetInstance().m_nACount)
		{
			SupportAwards(pObj,pUser);
			SendRankInfo(pUser->m_dwUin);
		}
		pUser->PlayEffect(nEffectCorrect);
		return true;
	}
	else
	{
		pObj->nCount ++;
		pUser->m_pUserRecord->RunLuaActivityFunc(1001,pObj->pObj->nStar,0);
		RandTest(pObj);
		//SendQuestion(pUser);
		AddWaitSend(pObj);
		_UpdateInfo(pObj);
		if(pObj->nCount >= TestAwardCfg::GetInstance().m_nACount)
		{
			SupportAwards(pObj,pUser);
			SendRankInfo(pUser->m_dwUin);
		}
		pUser->PlayEffect(nEffectWrong);
		return false;
	}
}

void WorldTestCenter::SupportAwards(WTestInfo* pObj,UserData* pUser)
{
	if(pObj == NULL || pUser == NULL)
		return;
	pUser->m_pUserRecord->AddRecord(UA_COMPLETE,UO_ACTIVITY,1001,0);
	AwardObj* pAward = TestAwardCfg::GetInstance().GetAwardByStar(pObj->nStar);
	if(pAward == NULL)
		return;
	int nRltExp = pAward->nExp*pUser->m_xUWealth[UW_LV]*pUser->m_xUWealth[UW_LV];		//待添加公式
	int nRltMoney = pAward->nMoney*pUser->m_xUWealth[UW_LV]*pUser->m_xUWealth[UW_LV];
	pUser->m_xUWealth.AddWealth(UW_EXP,nRltExp);
	pUser->m_xUWealth.AddWealth(UW_GOLD,nRltMoney,1,NULL,UW_GOLD_TEST);
	pUser->SendWarnMsg("本次答题共获得%d颗星，奖励%d经验，%d金币",pObj->nStar,nRltExp,nRltMoney);
}

void WorldTestCenter::SendRankInfo(int nUin)
{
	BuildPacketEx(pPacket,RPGACTIVITY,S_SEND_RANKINFO,buf,1024);
	SET_DATA(pData,RPGACTIVITY,S_SEND_RANKINFO,pPacket);
	if(m_byState == TEST_STATE_ON)
	{
		tm ctime = CMTime::GetCurTime();
		ActivityObj* pObj = GameActivityCfg::GetInstance().GetObj(1001);
		int nH = pObj->dwEndTime>>16;
		int nM = pObj->dwEndTime&0xFFFF;
		pData->nMin = (nH - ctime.tm_hour)*60+(nM-ctime.tm_min);
	}
	else
		pData->nMin = 0;
	int n = m_pList->Count > 10 ? 10 : m_pList->Count;
	pData->nCnt = 0;
	for(int i = 0; i < n; i ++)
	{
		WTestInfo* pInfo = (WTestInfo*)m_pList->Items[i];
		strcpy(pData->szName[i],pInfo->szName);
		if(pInfo->nStar == 0)
			break;
		pData->szScore[i] = pInfo->nStar;
		pData->nCnt ++;
	}
	TCP_SendPacketToUin(nUin,pPacket);
}

void WorldTestCenter::SendQuestion(UserData* pUser)
{
	WTestInfo* pObj = GetObjByUin(pUser->m_dwUin);
	if(pObj == NULL)
		pObj = AddNewObj(pUser);
	BuildPacketEx(pPacket,RPGACTIVITY,S_SEND_TESTINFO,buf,1024);
	SET_DATA(pData,RPGACTIVITY,S_SEND_TESTINFO,pPacket);
	pData->nStar = pObj->nStar;
	pData->nCount = pObj->nCount;
	pData->nTypeID = 0;
	pData->byState = m_byState;
	pData->nCorrect = pObj->nCorrect;
	pData->nPlace = pObj->nPlace;
	if(pObj->nEndSec == 0)
		pData->nTime = 0;
	else
		pData->nTime = pObj->nEndSec - time(0);
	if(pObj->pObj)
		pData->nTypeID = pObj->pObj->nID;
	for(int i = 0; i < 4; i ++)
		pData->byCh[i] = pObj->szCh[i];
	TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
}

void WorldTestCenter::AddWaitSend(WTestInfo* pObj)
{
	WWaitObj* pNew = new WWaitObj();
	pNew->pInfo = pObj;
	pNew->dwTick = GetTickCount() + 1200;
	m_pWaitList->Add(pNew);
}

void WorldTestCenter::SendQuestion(WTestInfo* pObj)
{
	if(pObj == NULL)
		return;
	UserData* pUser = g_pGameCenter->GetPlayer(pObj->nUin);
	if(pUser == NULL)
		return;
	BuildPacketEx(pPacket,RPGACTIVITY,S_SEND_TESTINFO,buf,1024);
	SET_DATA(pData,RPGACTIVITY,S_SEND_TESTINFO,pPacket);
	pData->nStar = pObj->nStar;
	pData->nCount = pObj->nCount;
	pData->nTypeID = 0;
	pData->byState = m_byState;
	pData->nCorrect = pObj->nCorrect;
	pData->nPlace = pObj->nPlace;
	if(pObj->nEndSec == 0)
		pData->nTime = 0;
	else
		pData->nTime = pObj->nEndSec - time(0);
	if(pObj->pObj)
		pData->nTypeID = pObj->pObj->nID;
	for(int i = 0; i < 4; i ++)
		pData->byCh[i] = pObj->szCh[i];
	TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
}

bool WorldTestCenter::DBInit()
{
	QueryRlt xRlt;
	if(!m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_WORLDTEST_TABLE))
		return false;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd == NULL)
		return false;

	while(pCmd->FetchRow())
	{
		WTestInfo* pInfo = new WTestInfo();
		pInfo->nUin = pCmd->GetInt("uinA");
		pInfo->nStar = pCmd->GetWORD("star");
		pInfo->nCount = pCmd->GetInt("count");
		int nTypeID = pCmd->GetInt("typeid");
		pInfo->pObj = TestLibraryCfg::GetInstance().GetTest(nTypeID);
		pInfo->szCh[0] = pCmd->GetWORD("choose1");
		pInfo->szCh[1] = pCmd->GetWORD("choose2");
		pInfo->szCh[2] = pCmd->GetWORD("choose3");
		pInfo->szCh[3] = pCmd->GetWORD("choose4");
		pInfo->nCorrect = pCmd->GetInt("correct");
		pInfo->byGiftFlag = pCmd->GetWORD("awardflag");
		sprintf(pInfo->szName,pCmd->GetStr("name"));

		m_pList->Add(pInfo);
	}
	m_pList->Sort(CompareTestRlt);
	m_ifCheckPlace = true;
	return true;
}

bool WorldTestCenter::_InsertInfo(WTestInfo* pInfo)
{
	int nType = 0;
	if(pInfo->pObj)
		nType = pInfo->pObj->nID;
	m_pDB->ExecuteSQL(FALSE,"insert into "RPG_WORLDTEST_TABLE" (uinA, star, count, typeid,choose1,choose2,choose3,choose4,correct,awardflag,name) values (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,'%s')", 
		pInfo->nUin,pInfo->nStar,pInfo->nCount,nType,pInfo->szCh[0],pInfo->szCh[1],pInfo->szCh[2],pInfo->szCh[3],pInfo->nCorrect,pInfo->byGiftFlag,pInfo->szName);
	return true;
}

bool WorldTestCenter::_UpdateInfo(WTestInfo* pInfo)
{
	int nType = 0;
	if(pInfo->pObj)
		nType = pInfo->pObj->nID;
	m_pDB->ExecuteSQL(FALSE,"update "RPG_WORLDTEST_TABLE" set star=%d,count=%d,typeid=%d,choose1=%d,choose2=%d,choose3=%d,choose4=%d,correct=%d,awardflag=%d,name='%s' where uinA=%d",
		pInfo->nStar,pInfo->nCount,nType,pInfo->szCh[0],pInfo->szCh[1],pInfo->szCh[2],pInfo->szCh[3],pInfo->nCorrect,pInfo->byGiftFlag,pInfo->szName,pInfo->nUin);
	return true;
}

bool WorldTestCenter::_RemoveInfo(WTestInfo* pInfo)
{
	m_pDB->ExecuteSQL(FALSE,"delete from "RPG_WORLDTEST_TABLE" where uinA=%d",pInfo->nUin);
	return true;
}

bool WorldTestCenter::_ClearSql()
{
	m_pDB->ExecuteSQL(FALSE,"delete from "RPG_WORLDTEST_TABLE"");
	return true;
}


TurntableCenter::TurntableCenter(UserData* pUser)
{
	m_pUser = pUser;
	//m_nPos = 0;
	for (int i = 0; i < 10; i++)
	{
		m_nPos[i] = 0;
	}
	m_nGiftFlag = 0;
	m_nFreeTimes = 0;
	m_nCount = 0;
	m_dwEndTime = 0;
}

TurntableCenter::~TurntableCenter()
{

}

void TurntableCenter::RefreshFlag()
{
	int nFlag = m_pUser->m_pUserFlag->GetFlag(5);		//转盘flag,前4位记免费次数，中间12位记累计次数奖励，后16位记累计抽奖次数
	m_nCount = nFlag&0xFFFF;	//次数
	int nFlag2 = nFlag>>16;
	m_nFreeTimes = nFlag2>>12;	//免费次数
	m_nGiftFlag = nFlag2&0xFFF;	//累计奖励领取
}

void TurntableCenter::ReSetFlag()
{
	int nFlag = (m_nFreeTimes<<28)+(m_nGiftFlag<<16)+m_nCount;
	m_pUser->m_pUserFlag->SetFlag(5,nFlag);
}

void TurntableCenter::Update()
{
	DWORD dwNow = GetTickCount();
	if(m_dwEndTime > 0 && m_dwEndTime <= dwNow)
	{
		m_dwEndTime = 0;
		for (int i = 0; i < 10; i++)
		{
			if(m_nPos[i] > 0 && m_nPos[i] <= 8)
			{
				char szWords[128];
				bool bMsg = false;
				TurntableAward award = TurntableCfg::GetInstance().m_szAward[m_nPos[i]-1];
				if(award.nID == 666)
				{
					m_pUser->m_xUWealth.AddWealth(UW_GOLD,award.nNum,1,NULL,UW_GOLD_ZUAN);
					//sprintf(szWords,"获得%d金币",award.nNum);
				}
				else if(award.nID == 555)
				{
					m_pUser->m_xUWealth.AddWealth(UW_EXP,award.nNum);
					//sprintf(szWords,"获得%d经验",award.nNum);
				}
				else if(award.nID == 888)
				{
					m_pUser->m_xUWealth.AddWealth(UW_RMB,award.nNum,RMB_SAVE_TURNTABLE);
					//sprintf(szWords,"获得%d钻石",award.nNum);
				}
				else if(award.nID == 333)
				{
					m_pUser->m_xUWealth.AddWealth(UW_BINDRMB,award.nNum,RMB_SAVE_TURNTABLE);
					//sprintf(szWords,"获得%d绑钻",award.nNum);
				}
				else if(award.nID == 999)
				{
					m_pUser->m_pSlaveCenter->AddNewSlave(award.nNum);
					SlaveBasic* pBase = SlaveBaseCfg::GetInstance().GetBaseInfo(award.nNum);
					if (pBase)
					{
						bMsg = true;
						sprintf(szWords,"人品大爆发，%s在幸运大转盘中获得了宠物%s",m_pUser->m_szNick,pBase->pName);
					}
				}
				else
				{
					m_pUser->m_pItemCenter->AddItemByID(award.nID,award.nNum,true,ITEM_LOG_TURN);
					ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(award.nID);
					if(pBase&&pBase->nStar>2)
					{
						bMsg = true;
						sprintf(szWords,"%s在幸运大转盘中获得了%s",m_pUser->m_szNick,pBase->szName);
					}
				}
				m_pUser->m_pUserRecord->RunLuaActivityFunc(1002,award.nID,award.nNum);
				//m_pUser->SendPopMsg(szWords);
				if (bMsg)
					ChatCenter::GetInstance().SendSystemMsg(szWords,0,0,1);
			}
			m_nPos[i] = 0;
		}
	}
}

void TurntableCenter::Start()
{
	DWORD dwNow = GetTickCount();
	if(m_dwEndTime > dwNow)
		return;
	RefreshFlag();
	for (int i = 0; i < 10; i++)
	{
		m_nPos[i] = 0;
	}
	if(m_nFreeTimes < 3)
	{
		m_nFreeTimes ++;
	}
	else
	{
		if(m_pUser->m_xUWealth[UW_RMB] < 20)
		{
			return;
		}
		m_pUser->m_xUWealth.SubWealth(UW_RMB,20,RMB_SAVE_TURNTABLE);
	}
	m_nPos[0] = TurntableCfg::GetInstance().RandPos();
	m_dwEndTime = dwNow+6000;
	m_nCount ++;
	BuildPacketEx(pPacket,RPGACTIVITY,S_SEND_TURNTABLE_START,buf,1024);
	SET_DATA(pData,RPGACTIVITY,S_SEND_TURNTABLE_START,pPacket);
	pData->nPos = m_nPos[0];
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
	ReSetFlag();
	m_pUser->m_pUserRecord->AddRecord(UA_COMPLETE,UO_ACTIVITY,1002,0);
}

void TurntableCenter::StartForTen()
{
	DWORD dwNow = GetTickCount();
	if(m_dwEndTime > dwNow)
		return;
	RefreshFlag();
	for (int j = 0; j < 10; j++)
	{
		m_nPos[j] = 0;
	}

	int nNeedRmb = 0;
	int nCount = 0;
	BuildPacketEx(pPacket,RPGACTIVITY,S_SEND_TURNTABLE_FORTEN,buf,1024);
	SET_DATA(pData,RPGACTIVITY,S_SEND_TURNTABLE_FORTEN,pPacket);
	pData->nCnt = 10;
	while(nCount < 10)
	{
		int nTmpPos = TurntableCfg::GetInstance().RandPos();
		if (nTmpPos)
		{
			if(m_nFreeTimes < 3)
			{
				m_nFreeTimes ++;
			}
			else
			{
				nNeedRmb += 20;
			}
			m_nPos[nCount] = nTmpPos;
			pData->byPos[nCount] = nTmpPos;
			m_nCount ++;
			nCount++;
		}
	}

	//for (int i = 0; i < pData->nCnt; i++)
	//{
	//	if(m_nFreeTimes < 3)
	//	{
	//		m_nFreeTimes ++;
	//	}
	//	else
	//	{
	//		nNeedRmb += 20;
	//	}
	//	m_nPos[i] = TurntableCfg::GetInstance().RandPos();
	//	pData->byPos[i] = m_nPos[i];
	//	m_nCount ++;
	//}

	if (!m_pUser->m_xUWealth.SubWealth(UW_RMB,nNeedRmb*80/100,RMB_SAVE_TURNTABLE))
	{
		m_pUser->SendWarnMsg("钻石不足！");
		return;
	}

	pPacket->wSize	+= (pData->nCnt-1)*sizeof(BYTE);
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);

	m_dwEndTime = dwNow+6000;
	ReSetFlag();
	m_pUser->m_pUserRecord->AddRecord(UA_COMPLETE,UO_ACTIVITY,1002,0);
}

bool TurntableCenter::AddFreeTimes()
{
	RefreshFlag();
	if (m_nFreeTimes < 1)
	{
		m_pUser->SendWarnMsg("可免费次数已满，先去抽取吧");
		return false;
	}

	m_nFreeTimes--;
	ReSetFlag();
	m_pUser->SendMsg("免费次数+1");
	return true;
}

void TurntableCenter::RequestForGift(int nPos)
{
	if(nPos >= TurntableCfg::GetInstance().m_pList->Count)
		return;
	RefreshFlag();
	TurntableAward* pAward = (TurntableAward*)TurntableCfg::GetInstance().m_pList->Items[nPos];
	if(m_nCount < pAward->nVal)
	{
		m_pUser->SendPopMsg("领取条件不足");
		return;
	}
	int szCheck[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
	if((m_nGiftFlag&szCheck[nPos]) == szCheck[nPos])
	{
		m_pUser->SendPopMsg("该奖励已经领取");
		return;
	}
	char szWords[128];
	if(pAward->nID == 666)
	{
		m_pUser->m_xUWealth.AddWealth(UW_GOLD,pAward->nNum,1,NULL,UW_GOLD_AWARD);
		sprintf(szWords,"获得%d金币",pAward->nNum);
	}
	else if(pAward->nID == 555)
	{
		m_pUser->m_xUWealth.AddWealth(UW_EXP,pAward->nNum);
		sprintf(szWords,"获得%d经验",pAward->nNum);
	}
	else if(pAward->nID == 888)
	{
		m_pUser->m_xUWealth.AddWealth(UW_RMB,pAward->nNum,RMB_SAVE_TURNTABLE);
		sprintf(szWords,"获得%d钻石",pAward->nNum);
	}
	else if(pAward->nID == 333)
	{
		m_pUser->m_xUWealth.AddWealth(UW_BINDRMB,pAward->nNum,RMB_SAVE_TURNTABLE);
		sprintf(szWords,"获得%d绑钻",pAward->nNum);
	}
	else
	{
		m_pUser->m_pItemCenter->AddItemByID(pAward->nID,pAward->nNum,true,ITEM_LOG_TURN);
		ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(pAward->nID);
		if(pBase)
		{
			sprintf(szWords,"获得%s×%d",pBase->szName,pAward->nNum);
		}
	}
	m_pUser->SendPopMsg(szWords);
	m_nGiftFlag = m_nGiftFlag|szCheck[nPos];
	ReSetFlag();
}