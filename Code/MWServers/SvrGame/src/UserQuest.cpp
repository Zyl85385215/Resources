#include "stdafx.h"
#include "UserData.h"
#include "UserQuest.h"
#include "QuestConfig.h"
#include "UserFlag.h"
#include "MapStaticCfg.h"
#include "ios_user_packet.h"
#include "ZNetGamesvr.h"
#include "FsGameCenter.h"
#include "CallMapEnemy.h"
#include "MarkGameDB.h"
#include "MapSObject.h"

UserQuestCenter::UserQuestCenter( UserData* pData )
{
	m_pUser		= pData;
	m_pLQuest	= xnList::Create();
}

UserQuestCenter::~UserQuestCenter()
{
	while(m_pLQuest->Count)
	{
		UQuestObj* pQObj	= (UQuestObj*)m_pLQuest->Delete(0);
		if(pQObj->pszwVal)
			free(pQObj->pszwVal);
		delete pQObj;
	}
	m_pLQuest->Free();
}

void UserQuestCenter::DBInit()
{
	QueryRlt xRlt;
	if(!m_pUser->m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_QUEST_TABLE" where uin=%d",m_pUser->m_dwUin))
		return;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd == NULL)
		return;

	while(pCmd->FetchRow())
	{
		UQuestObj*		pQuest	= new UQuestObj();
		pQuest->wQId		= pCmd->GetInt("qid");
		int nLen = 0;
		BYTE* pData = pCmd->GetBLOB("progress",&nLen);
		pQuest->pQBase		= QuestCfg::GetInstance().GetQuestByID(pQuest->wQId);
		if(pQuest->pQBase == NULL)
		{
			delete pQuest;
			continue;
		}
		if(pQuest->pQBase->byValCnt)
		{
			pQuest->pszwVal	= (WORD*)malloc(pQuest->pQBase->byValCnt*2);
			memset(pQuest->pszwVal,0,pQuest->pQBase->byValCnt*2);
			if(nLen)
				memcpy(pQuest->pszwVal,pData,min(nLen,pQuest->pQBase->byValCnt*2));
		}
		else
			pQuest->pszwVal	= NULL;
		
		pQuest->dwSPID	= 0;
		m_pLQuest->Add(pQuest);
	}
}

bool UserQuestCenter::RequestAcceptQuest( WORD wQID,bool bForce )
{
	if(GetQuest(wQID))	//已接
		return true;
	if (!m_pUser->RunLuaFunctionForQuest(wQID,szLuaFunctionBeforeQuestAccept))
	{
		return false;
	}
	QuestObj* pQBase	= QuestCfg::GetInstance().GetQuestByID(wQID);
	if(pQBase == NULL)
	{
		m_pUser->SendWarnMsg("找不到任务");
		return false;
	}

	if(bForce == false)
	{
		if(pQBase->wLine)
		{
			if(GetQuestByLine(pQBase->wLine))
			{
				m_pUser->SendWarnMsg("请先完成同类型任务");
				return false;
			}
		}

		if(m_pUser->m_xUWealth[UW_LV] < pQBase->bLvNeed)
		{
			m_pUser->SendWarnMsg("等级%d可接",pQBase->bLvNeed);
			return false;
		}

		if(m_pUser->m_byCountry >= 3 || pQBase->bCtyNeed[m_pUser->m_byCountry] == 0 )
		{
			m_pUser->SendWarnMsg("国家不符");
			return false;
		}
		if(m_pUser->m_byJob >= 5 || pQBase->bJobNeed[m_pUser->m_byJob] == 0 )
		{
			m_pUser->SendWarnMsg("职业不符");
			return false;
		}

		if(pQBase->wNpcSrc)
		{
			PathObj* pPath	= NpcPathLeadCfg::GetInstance().GetNpcPath(pQBase->wNpcSrc);
			if(pPath == NULL)
			{
				m_pUser->SendWarnMsg("找不到NPC");
				return false;
			}

			if(pPath->nMapindex == m_pUser->m_nRealMIndex%100 && CalcDistance(pPath->nPosX,pPath->nPosY,m_pUser->GetPosX(),m_pUser->GetPosY()) > 700)
			{
				m_pUser->SendWarnMsg("找不到NPC");
				return false;
			}
		}

		if(pQBase->bDayTime)
		{
			int nCurTime	= m_pUser->m_pUserFlag->GetQuestTime(wQID);
			if(nCurTime >= pQBase->bDayTime)
			{
				m_pUser->SendWarnMsg("今日次数已用完");
				return false;
			}
			m_pUser->m_pUserFlag->SetQuestTime(wQID,nCurTime+1);
		}

		if(pQBase->wItemNeed)
		{//消耗物品接任务
			int nCnt = m_pUser->m_pItemCenter->GetItemCountByID(pQBase->wItemNeed);
			if(nCnt == 0)
			{
				m_pUser->SendWarnMsg("缺少所需物品");
				return false;
			}
			if(pQBase->bItemCost)
			{
				if(pQBase->bItemCost>nCnt)
				{
					m_pUser->SendWarnMsg("缺少所需物品");
					return false;
				}
				m_pUser->m_pItemCenter->RemoveItemByID(pQBase->wItemNeed,pQBase->bItemCost);
			}
		}

		if(pQBase->wLine && pQBase->wLine < 10000)
		{//该线任务已过
			if(m_pUser->m_pUserFlag->GetQuestStep(pQBase->wLine) != pQBase->bStep)
			{
				m_pUser->SendWarnMsg("前置任务未完成");
				return false;
			}
		}

		//查看任务前置
		if(pQBase->pLPreQ->Count)
		{
			bool bPreQOk	= false;
			for (int i = 0; i < pQBase->pLPreQ->Count; i++)
			{
				_MuliParamObj* pMP	= (_MuliParamObj*)pQBase->pLPreQ->Items[i];
				if(m_pUser->m_pUserFlag->GetQuestStep(pMP->nParam1) <= pMP->nParam2)
				{//前置任务未完成
					if(pQBase->bAllDone)
					{
						m_pUser->SendWarnMsg("前置任务未完成");
						return false;
					}
					else
					{
						bPreQOk	= true;
					}
				}
			}
			if(!bPreQOk)
			{//前置任务未完成
				m_pUser->SendWarnMsg("前置任务未完成");
				return false;
			}
		}
	}
	
	UQuestObj*	pQObj	= new UQuestObj();
	if(pQBase->byValCnt)
	{
		pQObj->pszwVal		= (WORD*)malloc(2*pQBase->byValCnt);
		memset(pQObj->pszwVal,0,2*pQBase->byValCnt);
	}
	else
		pQObj->pszwVal		= NULL;
	pQObj->pQBase		= pQBase;
	pQObj->wQId			= wQID;
	pQObj->dwSPID		= 0;
	m_pLQuest->Add(pQObj);

	_insertDB(pQObj);
	_sendClient(pQObj);
	m_pUser->m_pUserRecord->AddRecord(UA_GAIN,UO_QUEST,wQID,pQBase->bType);

	if(pQBase->wEnemyID)
	{
		BYTE bForce	= m_pUser->m_byCountry;
		if(pQBase->bCallType == 0)
		{
			bForce	= 11;
		}

		DWORD	dwEnemyID	= g_pGameCenter->m_pCallEnemyCenter->CallEnemyByCfg(pQBase->wEnemyID,m_pUser->m_nMIndex,
			m_pUser->m_pMObjCenter->m_pPlayer->nPosX-50+rand()%100,m_pUser->m_pMObjCenter->m_pPlayer->nPosY-50+rand()%100,bForce,100,m_pUser->m_szNick);

		if(pQBase->bCallType < 10)
		{
			BYTE bCType	= pQBase->bCallType;
			if(bCType == 0)
				bCType	= 8;
			MObjSMark*	pMark = m_pUser->m_pMObjCenter->SetCtrl(dwEnemyID,MAPOBJ_ENEMY,bCType,true);
			if(pMark)
				pQObj->dwSPID	= pMark->dwID;
		}
	}
	if(pQBase->wStateID)
		m_pUser->m_pMObjCenter->AddState(m_pUser->m_pMObjCenter->m_pPlayer,pQBase->wStateID,false);

	m_pUser->RunLuaFunctionForQuest(wQID,szLuaFunctionOnQuestAccept);
	return true;
}

bool UserQuestCenter::RequestCompleteQuest( WORD wQID )
{
	UQuestObj* pUQObj	= GetQuest(wQID);	//已接
	if(!pUQObj)
		return false;
	if (!m_pUser->RunLuaFunctionForQuest(wQID,szLuaFunctionBeforeQuestComplete))
	{
		return false;
	}
	QuestObj* pQBase	= pUQObj->pQBase;
	if(pQBase->wNpcEnd)
	{
		PathObj* pPath	= NpcPathLeadCfg::GetInstance().GetNpcPath(pQBase->wNpcEnd);
		if(pPath == NULL)
		{
			m_pUser->SendWarnMsg("找不到NPC");
			return false;
		}
		if(pPath->nMapindex == m_pUser->m_nRealMIndex%100 && CalcDistance(pPath->nPosX,pPath->nPosY,m_pUser->GetPosX(),m_pUser->GetPosY()) > 800)
		{
			m_pUser->SendWarnMsg("NPC距离过远");
			return false;
		}
	}

	if(pQBase->wEnemyID)
	{
		if(pQBase->bCallType == 3 || pQBase->bCallType == 4)
		{
			MObjSMark* pMark	= m_pUser->m_pMObjCenter->GetMark(pUQObj->dwSPID);
			if(pMark == NULL)
			{
				m_pUser->SendWarnMsg("护送目标丢失,任务已失败");
				RequestGiveUpQuest(wQID);
				return false;
			}

			if(abs(pMark->nPosX-m_pUser->m_pMObjCenter->m_pPlayer->nPosX) > 300 ||
				abs(pMark->nPosY-m_pUser->m_pMObjCenter->m_pPlayer->nPosY) > 300)
			{
				m_pUser->SendWarnMsg("护送目标距离过远");
				return false;
			}
		}

		m_pUser->m_pMObjCenter->RemoveMark(pUQObj->dwSPID);
	}
	if(pQBase->wStateID)
		m_pUser->m_pMObjCenter->AddState(m_pUser->m_pMObjCenter->m_pPlayer,pQBase->wStateID,true);

	int	nCheckPos	= 0;
	for (int i = 0; i < pQBase->pLQCondtion->Count; i++)
	{
		_QConditon*	pQC	= (_QConditon*)pQBase->pLQCondtion->Items[i];
		switch(pQC->bDType)
		{//1杀怪 2物品 8玩家行为 9引导
		case 1:
		case 8:
		case 9:
			if(pQBase->byValCnt < nCheckPos)
				return false;

			if(pUQObj->pszwVal[nCheckPos]	< pQC->wDNum)
				return false;

			nCheckPos++;
			break;
		case 2:
		case 3:
			if(m_pUser->m_pItemCenter->GetItemCountByID(pQC->wDID) < pQC->wDNum)
				return false;	//数量要求要先检查完再执行扣除
			break;
		}
	}
	for (int i = 0; i < pQBase->pLQCondtion->Count; i++)
	{
		_QConditon*	pQC	= (_QConditon*)pQBase->pLQCondtion->Items[i];
		if(pQC->bDType == 2||pQC->bDType == 3)
			m_pUser->m_pItemCenter->RemoveItemByID(pQC->wDID,pQC->wDNum);
	}
	//任务结算
	if(pQBase->wLine && pQBase->wLine < 10000)
		m_pUser->m_pUserFlag->SetQuestStep(pQBase->wLine,pQBase->bStep+1);

	_removeDB(wQID);
	_removeClient(wQID,true);
	m_pUser->m_pUserRecord->AddRecord(UA_COMPLETE,UO_QUEST,wQID,pQBase->bType);

	//任务奖励
	if(pQBase->nAwardExp)
		m_pUser->m_xUWealth.AddWealth(UW_EXP,pQBase->nAwardExp);

	if(pQBase->nAwardGold)
	{
		if(pQBase->bType == 4)
		{
			m_pUser->m_xUWealth.AddWealth(UW_HONOUR,pQBase->nAwardGold);
		}
		else
		{
			m_pUser->m_xUWealth.AddWealth(UW_GOLD,pQBase->nAwardGold,1,NULL,UW_GOLD_QUEST);
		}
	}

	if(pQBase->bType == 6)
	{
		MarryData* pData = m_pUser->m_pUserMarry->GetRelation();
		if(pData)
		{
			int nFlag = m_pUser->m_pUserFlag->GetFlag(27);
			if(nFlag < 100)
			{
				if(m_pUser->m_bySex == 1)
					g_pMarryCenter->AddMarryValue(m_pUser,pData->m_dwID,20,true);
				else
					g_pMarryCenter->AddMarryValue(m_pUser,pData->m_dwID,20,false);
				m_pUser->m_pUserFlag->AddFlag(27,10);
			}
		}
		int nFlag = m_pUser->m_pUserFlag->SetFlag(28,0);
		m_pUser->GetMarryQuest();
	}

	if(1 || pQBase->bSelOne)
	{//奖励物品,多选一暂缺
		for (int i = 0 ; i < pQBase->pLAwardItem->Count; i++)
		{
			_MuliParamObj* pMP	= (_MuliParamObj*)pQBase->pLAwardItem->Items[i];
			if(pMP->nParam1 >= 10000)
			{
				//if(pQBase->bSelOne)
				{
					ItemExBase*	pIBase	= ItemConfig::GetInstance().GetItemBase(pMP->nParam1);
					if(pIBase == NULL || !(pIBase->nJob&(1<<m_pUser->m_byJob)))
						continue;	//多选一情况下只得到本职业物品
				}
				m_pUser->m_pItemCenter->AddItemByID(pMP->nParam1,pMP->nParam2,true,ITEM_LOG_QUEST);
			}
			else if(pMP->nParam1 < 2000)
			{//坐骑
				m_pUser->m_pHorseCenter->AddNewHorse(pMP->nParam1);
			}
			else
			{//宠物
				m_pUser->m_pSlaveCenter->AddNewSlave(pMP->nParam1);
			}
		}
	}

	if(pUQObj->pszwVal)
		free(pUQObj->pszwVal);
	m_pLQuest->Remove(pUQObj);
	delete pUQObj;
	m_pUser->RunLuaFunctionForQuest(wQID,szLuaFunctionOnQuestComplete);
	g_pMarkGameDB->AddQICnt(wQID,1,1);
	return true;
}


bool UserQuestCenter::RequestGiveUpQuest( WORD wQID )
{
	UQuestObj* pUQObj	= GetQuest(wQID);	//已接
	if(!pUQObj)
		return false;

	if (wQID ==5001 || wQID ==5002 || wQID ==5003 || wQID > 5100 && wQID < 5111)
	{//护送商队 跟 运送资源 失败 清除上马限制
		m_pUser->m_pMObjCenter->ClearRideLock(m_pUser->m_pMObjCenter->m_pPlayer);
	}

	if (!m_pUser->RunLuaFunctionForQuest(wQID,szLuaFunctionBeforeQuestGiveUp))
	{
		return false;
	}
	if(pUQObj->pQBase->wEnemyID)
	{
		m_pUser->m_pMObjCenter->RemoveMark(pUQObj->dwSPID);
	}
	if(pUQObj->pQBase->wStateID)
		m_pUser->m_pMObjCenter->AddState(m_pUser->m_pMObjCenter->m_pPlayer,pUQObj->pQBase->wStateID,true);

	_removeDB(wQID);
	_removeClient(wQID,false);
	m_pUser->m_pUserRecord->AddRecord(UA_LOST,UO_QUEST,wQID,pUQObj->pQBase->bType);
	if(pUQObj->pszwVal)
		free(pUQObj->pszwVal);
	m_pLQuest->Remove(pUQObj);
	delete pUQObj;
	m_pUser->RunLuaFunctionForQuest(wQID,szLuaFunctionOnQuestGiveUp);
	return true;
}

//void UserQuestCenter::AddItem(int nID, int nCount)
//{
//	for(int i = 0; i < m_pLQuest->Count; i++)
//	{
//		UQuestObj* pQObj	= (UQuestObj*)m_pLQuest->Items[i];
//		for (int j = 0; j < pQObj->pQBase->pLQCondtion->Count; j++)
//		{
//			_QConditon* pQCond	= (_QConditon*)pQObj->pQBase->pLQCondtion->Items[j];
//			if(pQCond->bDType == 3 && pQCond->wDID == nID)
//			{
//				pQObj->pszwVal[pQCond->bValPos] += nCount;
//				_sendClient(pQObj);
//				_saveDB(pQObj);
//				break;
//			}
//		}
//	}
//}

void UserQuestCenter::KillEnemy( WORD wEnemyID )
{
	for(int i = 0; i < m_pLQuest->Count; i++)
	{
		UQuestObj* pQObj	= (UQuestObj*)m_pLQuest->Items[i];
		for (int j = 0; j < pQObj->pQBase->pLQCondtion->Count; j++)
		{
			_QConditon* pQCond	= (_QConditon*)pQObj->pQBase->pLQCondtion->Items[j];
			if(pQCond->bDType == 1 && pQCond->wDID == wEnemyID && pQObj->pszwVal[pQCond->bValPos]< pQCond->wDNum)
			{
				pQObj->pszwVal[pQCond->bValPos]++;
				_sendClient(pQObj);
				//_saveDB(pQObj);
				pQObj->bSaveDB	= true;
				break;
			}
		}

		for (int j = 0; j < pQObj->pQBase->pLMonDrop->Count; j++)
		{
			_QMonDrop* pQMD	= (_QMonDrop*)pQObj->pQBase->pLMonDrop->Items[j];
			if(pQMD->wMonID == wEnemyID)
			{
				bool bifAddItem = true;
				for (int j = 0; j < pQObj->pQBase->pLQCondtion->Count; j++)
				{
					_QConditon* pQCond	= (_QConditon*)pQObj->pQBase->pLQCondtion->Items[j];
					if(pQCond->bDType == 2 && pQCond->wDID == pQMD->wItemID)
					{//任务掉落物品不能超出任务需求
						if(m_pUser->m_pItemCenter->GetItemCountByID(pQMD->wItemID)>=pQCond->wDNum)
						{
							bifAddItem = false;
							break;
						}
					}
				}
				if(bifAddItem&&rand()%10000 < pQMD->wPerChance)
					m_pUser->m_pItemCenter->AddItemByID(pQMD->wItemID,1,0,ITEM_LOG_KILLENEMY);
			}
		}
	}
}


UQuestObj* UserQuestCenter::GetQuestByLine( WORD wLine )
{
	for(int i = 0; i < m_pLQuest->Count; i++)
	{
		UQuestObj* pQObj	= (UQuestObj*)m_pLQuest->Items[i];
		if(pQObj->pQBase->wLine == wLine)
			return pQObj;
	}
	return NULL;
}

UQuestObj* UserQuestCenter::GetQuest( WORD wQID )
{
	for(int i = 0; i < m_pLQuest->Count; i++)
	{
		UQuestObj* pQObj	= (UQuestObj*)m_pLQuest->Items[i];
		if(pQObj->wQId == wQID)
			return pQObj;
	}
	return NULL;
}

void UserQuestCenter::_insertDB( UQuestObj* pQObj )
{
	pQObj->bSaveDB	= false;
	m_pUser->m_pDB->ExecuteSQL(false,"INSERT INTO "RPG_QUEST_TABLE" (uin,qid,progress) \
															  VALUES(%d,%d,'')", m_pUser->m_dwUin,pQObj->wQId);
}

void UserQuestCenter::_saveDB( UQuestObj* pQObj )
{
	pQObj->bSaveDB	= false;
	m_pUser->m_pDB->SetBLOB(false,pQObj->pszwVal,pQObj->pQBase->byValCnt*2,"update "RPG_QUEST_TABLE" set progress=? where uin=%d and qid=%d",m_pUser->m_dwUin,pQObj->wQId);
}

void UserQuestCenter::_removeDB( WORD wQId )
{
	m_pUser->m_pDB->ExecuteSQL(false,"delete from "RPG_QUEST_TABLE" where uin=%d and qid=%d",m_pUser->m_dwUin,wQId);
}

void UserQuestCenter::_sendClient( UQuestObj* pQObj )
{
	BuildPacketEx(pPacket, USER, S_QUEST_INFO,buf,1024);
	SET_DATA(pSend, USER, S_QUEST_INFO, pPacket);
	pSend->wQid		= pQObj->wQId;
	if(pQObj->pQBase->byValCnt)
		memcpy(pSend->wVal,pQObj->pszwVal,pQObj->pQBase->byValCnt*2);
	pPacket->wSize	+= (pQObj->pQBase->byValCnt-1)*2;
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
}

void UserQuestCenter::_removeClient( WORD wQID ,bool bComplete)
{
	BuildPacketEx(pPacket, USER, S_REMOVE_QUEST,buf,1024);
	SET_DATA(pSend, USER, S_REMOVE_QUEST, pPacket);
	pSend->wQid		= wQID;
	pSend->bComplete	= bComplete;
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
}

void UserQuestCenter::SyncClient()
{
	bool ifCheck = false;
	tm	ctime = CMTime::GetCurTime();
	if (((ctime.tm_wday != 3)&&(ctime.tm_wday != 6)&&(ctime.tm_wday != 0))||(ctime.tm_hour!=16))
	{
		ifCheck = true;
	}
	for (int i = m_pLQuest->Count -1 ; i >= 0 ; i--)
	//for (int i = 0; i < m_pLQuest->Count; i++)
	{
		UQuestObj*	pUObj	= (UQuestObj*)m_pLQuest->Items[i];
		if (pUObj->pQBase->wQID == 5011 && ifCheck)
		{
			RequestGiveUpQuest(5011);
			m_pUser->SendWarnMsg("不在捕鱼大赛活动时间内，自动放弃活动任务");
		}
		else
			_sendClient(pUObj);
	}
}

void UserQuestCenter::ResetMapObj()
{//换地图后需要重新生成怪物
	for (int i = 0; i < m_pLQuest->Count; i++)
	{
		UQuestObj* pQuest	= (UQuestObj*)m_pLQuest->Items[i];
		if(pQuest->pQBase->wEnemyID && pQuest->dwSPID)
		{
			BYTE bForce	= m_pUser->m_byCountry;
			if(pQuest->pQBase->bCallType == 0)
			{
				bForce	= 11;
			}

			DWORD	dwEnemyID	= g_pGameCenter->m_pCallEnemyCenter->CallEnemyByCfg(pQuest->pQBase->wEnemyID,m_pUser->m_nMIndex,
				m_pUser->m_pMObjCenter->m_pPlayer->nPosX-50+rand()%100,m_pUser->m_pMObjCenter->m_pPlayer->nPosY-50+rand()%100,bForce);

			if(pQuest->pQBase->bCallType < 10)
			{
				BYTE bCType	= pQuest->pQBase->bCallType;
				if(bCType == 0)
					bCType	= 8;
				MObjSMark*	pMark = m_pUser->m_pMObjCenter->SetCtrl(dwEnemyID,MAPOBJ_ENEMY,bCType,true);
				if(pMark)
					pQuest->dwSPID	= pMark->dwID;
			}
		}
	}

	
}

void UserQuestCenter::CheckCtrlDead( DWORD dwSID )
{
	for (int i = 0; i < m_pLQuest->Count; i++)
	{
		UQuestObj* pQuest	= (UQuestObj*)m_pLQuest->Items[i];
		if(pQuest->pQBase->wEnemyID && pQuest->dwSPID)
		{
			if(pQuest->dwSPID	== dwSID)
				pQuest->dwSPID	= 0;
		}
	}
}

bool UserQuestCenter::CheckCompleteQuest( WORD wQID )
{
	UQuestObj* pUQObj	= GetQuest(wQID);	//已接
	if(!pUQObj)
		return false;
	QuestObj* pQBase	= pUQObj->pQBase;
	int	nCheckPos	= 0;
	for (int i = 0; i < pQBase->pLQCondtion->Count; i++)
	{
		_QConditon*	pQC	= (_QConditon*)pQBase->pLQCondtion->Items[i];
		switch(pQC->bDType)
		{//1杀怪 2物品 8玩家行为 9引导
		case 1:
		case 8:
		case 9:
			if(pQBase->byValCnt < nCheckPos)
				return false;

			if(pUQObj->pszwVal[nCheckPos]	< pQC->wDNum)
				return false;

			nCheckPos++;
			break;
		case 2:
		case 3:
			if(m_pUser->m_pItemCenter->GetItemCountByID(pQC->wDID) < pQC->wDNum)
				return false;	//数量要求要先检查完再执行扣除
			break;
		}
	}
	return true;
}

void UserQuestCenter::UpdateMinuteOrLeave()
{
	for(int i = 0; i < m_pLQuest->Count; i++)
	{
		UQuestObj* pQObj	= (UQuestObj*)m_pLQuest->Items[i];
		if(pQObj->bSaveDB)
			_saveDB(pQObj);
	}
}



