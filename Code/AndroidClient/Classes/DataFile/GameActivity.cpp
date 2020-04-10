#include "GameActivity.h"
#include "MainMenu.h"
#include "ActivedegreeCfg.h"
#include "GameActivityCfg.h"
#include "ZNetPacket.h"
#include "ios_activity_packet.h"
#include "MapStaticCfg.h"
#include "MapMouse.h"
#include "MapCopyConfig.h"

WorldTestCenter::WorldTestCenter()
{
	m_nCount = 0;
	m_nStar = 0;
	m_pObj = NULL;
	m_byState = TEST_STATE_OFF;
	m_nCorrect = 0;
	m_nPlace = 0;
	m_nEndSec = 0;
	m_ifRefresh = false;
	memset(m_byCh,0,4);

	m_nRankCnt = 0;
	m_nTime = 0;
	memset(m_szName,0,200);
	memset(m_szScore,0,40);
}

WorldTestCenter::~WorldTestCenter()
{

}

bool WorldTestCenter::PacketProcess(ZPacket* pPacket)
{
	if(pPacket->bCmdGroup != RPGACTIVITY)
		return false;
	switch(pPacket->bCmd)
	{
	case S_SEND_TESTINFO:
		{
			SET_DATA(pData,RPGACTIVITY,S_SEND_TESTINFO,pPacket);
			m_nCount = pData->nCount;
			m_nStar = pData->nStar;
			m_byState = pData->byState;
			m_nCorrect = pData->nCorrect;
			m_nPlace = pData->nPlace;
			memcpy(m_byCh,pData->byCh,4);
			if(pData->nTypeID == 0)
				m_pObj = NULL;
			else
				m_pObj = TestLibraryCfg::GetInstance().GetTest(pData->nTypeID);
			if(pData->nTime == 0)
				m_nEndSec = 0;
			else
				m_nEndSec = time(0) + pData->nTime;
			m_ifRefresh = true;
		}
		return true;
	case S_SEND_START_RLT:
		{
			SET_DATA(pData,RPGACTIVITY,S_SEND_START_RLT,pPacket);
			if(pData->isRlt == 1)
			{
				pMainMenu->m_pWTestMenu->Open();
				pMainMenu->m_pActivityCtrl->Close();
				pMainMenu->m_pActivityJoinMenu->m_pForm->Close();
			}
			else if(pData->isRlt == 0)
			{
				//pMainMenu->PopTipMsg("今日答题机会已经用掉了，请明天再来");
				//pMainMenu->m_pWTestRankMenu->Open();
				//pMainMenu->m_pWTestMenu->Close();
			}
			else if(pData->isRlt == -1)
			{
				//pMainMenu->PopTipMsg("活动尚未开启，请稍候再试");
			}
			else if(pData->isRlt == -2)
			{
				pMainMenu->PopTipMsg("活动已被暂停，请稍候再试");
			}
			else if(pData->isRlt == -3)
			{
				pMainMenu->PopTipMsg("活动意外中止，请稍候再试");
			}
		}
		return true;
	case S_SEND_RANKINFO:
		{
			SET_DATA(pData,RPGACTIVITY,S_SEND_RANKINFO,pPacket);
			m_nRankCnt = pData->nCnt;
			if(pData->nMin == 0)
				m_nTime = 0;
			else
				m_nTime = xnGetSecCount()+ pData->nMin*60;
			memcpy(m_szName,pData->szName,200);
			memcpy(m_szScore,pData->szScore,40);
			pMainMenu->m_pWTestRankMenu->Open();
			pMainMenu->m_pWTestMenu->Close();
			pMainMenu->m_pActivityCtrl->Close();
			pMainMenu->m_pActivityJoinMenu->m_pForm->Close();
		}
		return true;
	}
	return false;
}

void WorldTestCenter::AnswerQuestion(BYTE byPos)
{
	if(byPos > 3)
		return;
	if(m_byState != TEST_STATE_ON)
		return;
	if(m_nCount >= 10)
		return;
	BuildPacketEx(pPakcet,RPGACTIVITY,C_ANSWER_QUESTION,buf,1024);
	SET_DATA(pData,RPGACTIVITY,C_ANSWER_QUESTION,pPakcet);
	pData->byPos = byPos;
	SendPacket(pPakcet);
}

void WorldTestCenter::RequestStart()
{
	SendCmd(RPGACTIVITY,C_REQUEST_START);
}

void WorldTestCenter::RequestGiveup()
{
	SendCmd(RPGACTIVITY,C_REQUEST_GIVEUP);
}

void WorldTestCenter::RequestForGift()
{
	SendCmd(RPGACTIVITY,C_REQUEST_FOR_GIFT);
}


ACDegreeCenter::ACDegreeCenter()
{
	m_pList = xnList::Create();
	m_byDegreeLV = 0;
	m_ifNeedReflesh = false;
}

ACDegreeCenter::~ACDegreeCenter()
{
	if(m_pList)
	{
		while(m_pList->Count)
			delete (UserDegreePart*)m_pList->Delete(0);
		m_pList->Free();
	}
}

void ACDegreeCenter::SetVal(RPGACTIVITY_S_SEND_ACDEGREE_Data* pData)
{
	if(pData == NULL)
		return;
	if(pData->nCnt == 1)
	{
		if(m_pList->Count == 0)
		{
			UserDegreePart* pTemp = new UserDegreePart();
			memcpy(pTemp,&pData->szDegree[0],sizeof(UserDegreePart));
			m_pList->Add(pTemp);
		}
		else
		{
			if(pData->byPos < m_pList->Count)
			{
				UserDegreePart* pTemp = (UserDegreePart*)m_pList->Items[pData->byPos];
				memcpy(pTemp,&pData->szDegree[0],sizeof(UserDegreePart));
			}
		}
	}
	else
	{
		while(m_pList->Count)
			delete (UserDegreePart*)m_pList->Delete(0);
		for(int i = 0; i < pData->nCnt; i ++)
		{
			UserDegreePart* pTemp = new UserDegreePart();
			memcpy(pTemp,&pData->szDegree[i],sizeof(UserDegreePart));
			m_pList->Add(pTemp);
		}
	}
	m_byDegreeLV = pData->byDLv;
	m_ifNeedReflesh = true;
}

void ACDegreeCenter::RequestForAward()
{
	int nGetPoint = 0;
	for(int k = 0; k < pChar->m_pADegreeCenter->m_pList->Count; k ++)
	{
		UserDegreePart* pTempPart = (UserDegreePart*)pChar->m_pADegreeCenter->m_pList->Items[k];
		ADDaylyPart* pPartA = (ADDaylyPart*)ActiveDegree::GetInstance().m_pDaylyList->Items[k];
		if(pTempPart->ifDone)
			nGetPoint += pPartA->nPoint;

	}
	int nLv = ActiveDegree::GetInstance().CheckLevel(nGetPoint);
	if(nLv <= m_byDegreeLV)
	{
		pMainMenu->PopTipMsg("没有奖励可以领取");
		return;
	}
	SendCmd(RPGACTIVITY,C_REQUEST_DEGREEAWARD);
}

bool ACDegreeCenter::PacketProcess(ZPacket* pPacket)
{
	if(pPacket->bCmdGroup != RPGACTIVITY)
		return false;
	switch(pPacket->bCmd)
	{
	case S_SEND_ACDEGREE:
		{
			SET_DATA(pData,RPGACTIVITY,S_SEND_ACDEGREE,pPacket);
			SetVal(pData);
		}
		return true;
	case S_SEND_ACDLV:
		{
			SET_DATA(pData,RPGACTIVITY,S_SEND_ACDLV,pPacket);
			SetVal(pData->byDlv);
		}
		return true;
	case S_SEND_BOSSINFO:
		{
			SET_DATA(pData,RPGACTIVITY,S_SEND_BOSSINFO,pPacket);
			for (int i = 0; i < pData->nCnt; i++)
			{
				pMainMenu->m_pACBossMenu->m_pStateList->Add((void*)pData->wBossID[i]);
			}
		}
		return true;
	}
	return false;
}

GameActivityCenter::GameActivityCenter()
{
	m_pList = xnList::Create();
	m_nShowFlag = 0;
	m_ifChg = false;

	m_byOpenFrom = 0;
	m_bShowActive = true;
	m_pWarnActiveList = xnList::Create();
	m_pJoinList = xnList::Create();
}

GameActivityCenter::~GameActivityCenter()
{
	if(m_pList)
	{
		while(m_pList->Count)
			m_pList->Delete(0);
		m_pList->Free();
	}

	if(m_pJoinList)
	{
		while(m_pJoinList->Count)
			m_pJoinList->Delete(0);
		m_pJoinList->Free();
	}
	if (m_pWarnActiveList)
	{
		while(m_pWarnActiveList->Count)
			m_pWarnActiveList->Delete(0);
		m_pWarnActiveList->Free();
	}
}

void cb_CheckJumpToNpc(void* pParam, int nVal)
{
	PathObj* pNpc = (PathObj*)pParam;
	if (nVal)
	{
		BuildPacketEx(pPacket,RPGACTIVITY,C_SEND_JUMPTONPC,buf,1024);
		SET_DATA(pData,RPGACTIVITY,C_SEND_JUMPTONPC,pPacket);
		pData->nID = pNpc->nID;
		pChar->SendPacket(pPacket);
	}else
		g_pMapCommand->CmdPathObj(pNpc);
}

void fun_cbActivityMCopy(void * pParam,int nVal)
{
	if(nVal == 1)
	{	
		BuildPacketEx(pPacket, RPGACTIVITY, C_OPEN_MAPCOPY_RLT, buf, 256);
		SET_DATA(pSend, RPGACTIVITY, C_OPEN_MAPCOPY_RLT, pPacket);
		pSend->wMCID	= (int)pParam;
		pChar->SendPacket(pPacket);
	}
}

bool GameActivityCenter::PacketProcess(ZPacket* pPacket)
{
	if(pPacket->bCmdGroup != RPGACTIVITY)
		return false;
	switch(pPacket->bCmd)
	{
	case S_SEND_ACINFO:
		{
			SET_DATA(pData,RPGACTIVITY,S_SEND_ACINFO,pPacket);
			if(pData->ifDifferent)
			{
				while(m_pList->Count)
					m_pList->Delete(0);
				for(int i = 0; i < pData->nCnt; i ++)
				{
					WORD wID = pData->szID[i];
					ActivityObj* pObj = GameActivityCfg::GetInstance().GetObj(wID);
					if(pObj != NULL && (pObj->byCountry == 0 || pObj->byCountry == pChar->m_byCountry))
						m_pList->Add(pObj);
				}
				m_ifChg = true;
				m_nShowFlag = pData->nShowFlag;
			}
		}
		return true;
	case S_JION_SEND_ACINFO:
		{
			SET_DATA(pData,RPGACTIVITY,S_JION_SEND_ACINFO,pPacket);
			while(m_pJoinList->Count)
				m_pJoinList->Delete(0);

			m_byOpenFrom = 0;
			if (pData->nCnt)
			{
				for(int i = 0; i < pData->nCnt; i ++)
				{
					WORD wID = pData->szID[i];
					ActivityObj* pObj = GameActivityCfg::GetInstance().GetObj(wID);
					if(pObj != NULL && (pObj->byCountry == 0 || pObj->byCountry == pChar->m_byCountry))
						m_pJoinList->Add(pObj);
				}

				m_byOpenFrom = 2;
			}

			pChar->m_pMounthAwardCenter->RefreshFlag();
			if (pChar->m_pMounthAwardCenter->CheckTodaySign())
			{
				m_byOpenFrom = 1;
			}else if (m_byOpenFrom = 0)
				m_byOpenFrom = 3;
		}
		return true;
	case S_SEND_CURTIME_ACINFO:
		{
			SET_DATA(pData,RPGACTIVITY,S_SEND_CURTIME_ACINFO,pPacket);
			ActivityObj* pObj = GameActivityCfg::GetInstance().GetObj(pData->wID);
			if (pObj)
			{
				m_pWarnActiveList->Add((void*)pData->wID);
			}
		}
		return true;
	case S_SEND_OPEN_TURNTABLE:
		{
			pMainMenu->m_pTurntableMenu->Open(2);
			pMainMenu->m_pActivityCtrl->Close();
			pMainMenu->m_pActivityJoinMenu->m_pForm->Close();
		}
		return true;
	case S_SEND_OPEN_HOMECOPY:
		{
			pMainMenu->m_pTabFamilyCtrl->Open(1,0);
			pMainMenu->m_pActivityCtrl->Close();
			pMainMenu->m_pActivityJoinMenu->m_pForm->Close();
		}
		return true;
	case S_SEND_OPEN_HOMEQUEST:
		{
			pMainMenu->m_pTabFamilyCtrl->Open(1,4);
			pMainMenu->m_pActivityCtrl->Close();
			pMainMenu->m_pActivityJoinMenu->m_pForm->Close();
		}
		return true;
	case S_SEND_FINDNPC:
		{
			SET_DATA(pData,RPGACTIVITY,S_SEND_FINDNPC,pPacket);
			PathObj*	pNpc	= NpcPathLeadCfg::GetInstance().GetNpcPath(pData->nID);
			if(pNpc)
			{
				if (g_pMapCenter->m_nMapIndex%100 == pNpc->nMapindex && CalcDistance(g_pMapCenter->m_pMObjLockView->nPosX,g_pMapCenter->m_pMObjLockView->nPosY,pNpc->nPosX,pNpc->nPosY) < 1000)
				{
					g_pMapCommand->CmdPathObj(pNpc);
				}else
				{
					if (pChar->m_pItemCenter->GetItemCountByID(30009))
					{
						pMainMenu->PopAskMsgBox("是否消耗炉石，直接传送至NPC",cb_CheckJumpToNpc,(void*)pNpc);
					}else
					{
						pMainMenu->PopTipMsg("缺少炉石，将自动寻路前往");
						g_pMapCommand->CmdPathObj(pNpc);
					}
				}
			}
			pMainMenu->m_pActivityCtrl->Close();
			pMainMenu->m_pActivityJoinMenu->m_pForm->Close();
		}
		return true;
	case S_CHECK_OPEN_MAPCOPY:
		{
			SET_DATA(pData,RPGACTIVITY,S_CHECK_OPEN_MAPCOPY,pPacket);

			MCopyBase* pMBase	= MCopyConfig::GetInstance().GetMCopy(pData->wMCID);
			char	szTmp[256];
			if (pData->dwCostGold)
				sprintf(szTmp,"是否消耗%dW金币，进入活动副本 %s ?",pData->dwCostGold/10000,pMBase->szName);
			else
				sprintf(szTmp,"是否进入活动副本 %s ?",pMBase->szName);
			pMainMenu->PopAskMsgBox(szTmp,fun_cbActivityMCopy,(void*)pData->wMCID);
		}
		return true;
	}
	return false;
}

void GameActivityCenter::RequestEnter(int nPos)
{
	if(nPos < 0 || nPos >= m_pList->Count)
		return;
	ActivityObj* pObj = (ActivityObj*)m_pList->Items[nPos];
	BuildPacketEx(pPacket,RPGACTIVITY,C_REQUEST_ENTERAC,buf,1024);
	SET_DATA(pData,RPGACTIVITY,C_REQUEST_ENTERAC,pPacket);
	pData->nID = pObj->nID;
	SendPacket(pPacket);
}

void GameActivityCenter::RequestEnterEx(int nPos)
{
	if(nPos < 0 || nPos >= m_pJoinList->Count)
		return;
	ActivityObj* pObj = (ActivityObj*)m_pJoinList->Items[nPos];
	BuildPacketEx(pPacket,RPGACTIVITY,C_REQUEST_ENTERAC,buf,1024);
	SET_DATA(pData,RPGACTIVITY,C_REQUEST_ENTERAC,pPacket);
	pData->nID = pObj->nID;
	SendPacket(pPacket);
}

void GameActivityCenter::RequestForInfo()
{
	BuildPacketEx(pPacket,RPGACTIVITY,C_REQUEST_ACINFO,buf,1024);
	SET_DATA(pData,RPGACTIVITY,C_REQUEST_ACINFO,pPacket);
	pData->nFlag = m_nShowFlag;
	SendPacket(pPacket);
}

void GameActivityCenter::RequestFind(int nPos)
{
	BossInfoObj* pBossInfo = (BossInfoObj*)ACBossCfg::GetInstance().m_pList->Items[nPos];
	PathObj* pObj = MonsterPathLeadCfg::GetInstance().GetPathObj(pBossInfo->nID);
	if(pObj)
	{
		g_pMapCommand->CmdPathObj(pObj);
	}
}

void GameActivityCenter::RequestBossInfo()
{
	SendCmd(RPGACTIVITY,C_REQUEST_BOSSINFO);
}

void GameActivityCenter::OpenActWarnMenu()
{
	if (pMainInterface->GetDForm("main4")->bOpened == false)
		return;

	if (m_byOpenFrom == 0)
		return;

	if (pChar->m_pRechargeAwardCenter->m_ifWarn == false)
		return;

	if (pChar->m_pRechargeAwardCenter->m_ifShine)
	{
		pMainMenu->m_pRechargeAwardMenu->Open(2);
		m_byOpenFrom = 0;
		return;
	}

	if (pChar->m_pLoginAwardCenter->m_ifWarn == false)
		return;

	if (pChar->m_pLoginAwardCenter->m_ifShine)
	{
		pMainMenu->m_pTabAwardCtrl->Open(2,5);
		m_byOpenFrom = 0;
		return;
	}

	if (m_byOpenFrom == 1)
	{
		pMainMenu->m_pTabAwardCtrl->Open(2,1);
	}else if (m_byOpenFrom == 2)
	{
		pMainMenu->m_pActivityJoinMenu->OpenFrom();
	}

	m_byOpenFrom = 0;

	return;
}

TurntableCenter::TurntableCenter()
{
	m_byPos = 0;
	for (int i = 0; i < 10; i++)
	{
		m_byTenPos[i] = 0;
	}
	m_dwEndTick = 0;
	m_nCount = 0;
	m_nFreeTimes = 0;
	m_nGiftFlag = 0;
}

TurntableCenter::~TurntableCenter()
{

}

void TurntableCenter::RefreshFlag()
{
	int nFlag = pChar->m_pClientFlag->GetFlag(5);		//转盘flag,前4位记免费次数，中间12位记累计次数奖励，后16位记累计抽奖次数
	m_nCount = nFlag&0xFFFF;	//次数
	int nFlag2 = nFlag>>16;
	m_nFreeTimes = nFlag2>>12;	//免费次数
	m_nGiftFlag = nFlag2&0xFFF;	//累计奖励领取
}

bool TurntableCenter::PacketProcess(ZPacket* pPacket)
{
	if(pPacket->bCmdGroup != RPGACTIVITY)
		return false;
	switch(pPacket->bCmd)
	{
	case S_SEND_TURNTABLE_START:
		{
			if(m_dwEndTick > 0)
				return true;
			SET_DATA(pData,RPGACTIVITY,S_SEND_TURNTABLE_START,pPacket);
			m_byPos = pData->nPos;
			m_dwEndTick = xnGetTickCount()+6200;
			pMainMenu->m_pTurntableMenu->ActionStart(m_byPos);
		}
		return true;
	case S_SEND_TURNTABLE_FORTEN:
		{
			if(m_dwEndTick > 0)
				return true;
			SET_DATA(pData,RPGACTIVITY,S_SEND_TURNTABLE_FORTEN,pPacket);
			m_byPos = pData->byPos[0];
			for (int i = 0; i < pData->nCnt; i++)
			{
				m_byTenPos[i] = pData->byPos[i];
			}
			m_dwEndTick = xnGetTickCount()+6200;
			pMainMenu->m_pTurntableMenu->m_bShowTen = true;
			pMainMenu->m_pTurntableMenu->ActionStart(m_byPos);
		}
		return true;
	}
	return false;
};

void TurntableCenter::RequestStart()
{
	SendCmd(RPGACTIVITY,C_REQUEST_TURNTABLE_START);
}

void TurntableCenter::RequestGetTen()
{
	SendCmd(RPGACTIVITY,C_REQUEST_TURNTABLE_FORTEN);
}

void TurntableCenter::RequestForGift(int nPos)
{
	BuildPacketEx(pPacket,RPGACTIVITY,C_REQUEST_TURNTABLE_GIFT,buf,1024);
	SET_DATA(pData,RPGACTIVITY,C_REQUEST_TURNTABLE_GIFT,pPacket);
	pData->nPos = nPos;
	SendPacket(pPacket);
}