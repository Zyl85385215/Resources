#include "stdafx.h"
#include "AchievementCenter.h"
#include "AchievementCfg.h"
#include "UserData.h"
#include "FsGameCenter.h"
#include "GameAwardCfg.h"

AchievementCenter::AchievementCenter(DBControl*	pDB)
{
	m_pDB = pDB;
	m_pAchievementList	= xnList::Create();
}

AchievementCenter::~AchievementCenter()
{
	if(m_pAchievementList)
	{
		while(m_pAchievementList->Count)
			delete (AchievementUserObj*)m_pAchievementList->Delete(0);
		m_pAchievementList->Free();
	}
}

AchievementUserObj*	AchievementCenter::GetUserObj(int nUin,int nID)
{
	for(int i = 0; i < m_pAchievementList->Count; i ++)
	{
		AchievementUserObj* pObj = (AchievementUserObj*)m_pAchievementList->Items[i];
		if(pObj->nUin == nUin && pObj->nId == nID)
			return pObj;
	}
	return NULL;
}

void	AchievementCenter::GetUserObjList(int nUin,OUT xnList* pList)
{
	if(pList == NULL)
		pList = xnList::Create();
	for(int i = 0; i < m_pAchievementList->Count; i ++)
	{
		AchievementUserObj* pObj = (AchievementUserObj*)m_pAchievementList->Items[i];
		if(pObj->nUin == nUin)
		{
			pList->Add(pObj);
			if(pObj->nFloor == 0)
			{
				AchievemetnObj* pCheck = AchievementCfg::GetInstance().GetObj(pObj->nId);
				pObj->nFloor = pCheck->nFloor;
				ATileObj* pFloor = AchievementCfg::GetInstance().GetFloor(pObj->nFloor);
				pObj->nGroup = pFloor->nGroup;
			}
		}
	}
}

bool	AchievementCenter::DBInit()
{
	QueryRlt xRlt;
	if(!m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_ACHIEVEMENT_TABLE))
		return false;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd == NULL)
		return false;

	while(pCmd->FetchRow())
	{
		AchievementUserObj* pObj = new AchievementUserObj();
		pObj->nUin	= pCmd->GetInt("uin");
		pObj->nId	= pCmd->GetInt("id");
		pObj->nCnt	= pCmd->GetInt("cnt");
		pObj->ifDone= pCmd->GetInt("ifdone");
		pObj->ifShow= pCmd->GetInt("ifshow");
		pObj->nGroup= 0;
		pObj->nFloor= 0;
		pObj->byState = 0;
		m_pAchievementList->Add(pObj);
	}
	return true;
}

bool	AchievementCenter::AddAchievementVal(int nUin,int nID,int nVal)
{
	UserData* pUser = g_pGameCenter->GetPlayer(nUin);
	if(pUser && pUser->m_pUserAchievement)
	{
		return pUser->m_pUserAchievement->AddAchievementVal(nID,nVal);
	}
	AchievementUserObj* pObj =GetUserObj(nUin,nID);
	if(pObj)
	{
		if(pObj->ifDone)
			return true;
		AchievemetnObj* pTemp = AchievementCfg::GetInstance().GetObj(nID);
		if(pTemp == NULL)
			return false;
		if(pObj->nCnt < pTemp->nCnt)
			pObj->nCnt += nVal;
		if(pObj->nCnt >= pTemp->nCnt)
			pObj->ifDone = true;
		_updateObj(pObj);
	}
	else
	{
		AchievemetnObj* pTemp = AchievementCfg::GetInstance().GetObj(nID);
		if(pTemp == NULL)
			return false;
		pObj = new AchievementUserObj();
		memset(pObj,0,sizeof(AchievementUserObj));
		pObj->nUin = nUin;
		pObj->nId = nID;
		pObj->nCnt += nVal;
		if(pObj->nCnt >= pTemp->nCnt)
			pObj->ifDone = true;
		pObj->nFloor = 0;
		pObj->nGroup = 0;
		_insertObj(pObj);
		pObj->byState = 0;
		m_pAchievementList->Add(pObj);
	}
	return true;
}

bool	AchievementCenter::SetAchievementVal(int nUin,int nID,int nVal)
{
	UserData* pUser = g_pGameCenter->GetPlayer(nUin);
	if(pUser && pUser->m_pUserAchievement)
	{
		return pUser->m_pUserAchievement->SetAchievementVal(nID,nVal);
	}
	AchievementUserObj* pObj =GetUserObj(nUin,nID);
	int nDelta = 0;
	if(pObj)
	{
		if(pObj->ifDone)
			return true;
		nDelta = nVal - pObj->nCnt;
	}
	else
	{
		nDelta = nVal;
	}
	AddAchievementVal(nUin,nID,nDelta);
	return true;
}

bool	AchievementCenter::DoneAchievement(int nUin,int nID)
{
	UserData* pUser = g_pGameCenter->GetPlayer(nUin);
	if(pUser && pUser->m_pUserAchievement)
	{
		return pUser->m_pUserAchievement->DoneAchievement(nID);
	}
	AchievementUserObj* pObj =GetUserObj(nUin,nID);
	if(pObj)
	{
		if(pObj->ifDone)
			return true;
		pObj->ifDone = true;
		_updateObj(pObj);
	}
	else
	{
		pObj = new AchievementUserObj();
		memset(pObj,0,sizeof(AchievementUserObj));
		pObj->nUin = nUin;
		pObj->nId = nID;
		pObj->ifDone = true;
		pObj->nFloor = 0;
		pObj->nGroup = 0;
		_insertObj(pObj);
		pObj->byState = 0;
		m_pAchievementList->Add(pObj);
	}
	return true;
}

void	AchievementCenter::DeleteUser(int nUin)
{
	for(int i = m_pAchievementList->Count-1; i >= 0; i --)
	{
		AchievementUserObj* pObj = (AchievementUserObj*)m_pAchievementList->Items[i];
		if(pObj->nUin == nUin)
		{
			m_pAchievementList->Delete(i);
			delete pObj;
		}
	}
	m_pDB->ExecuteSQL(FALSE,"delete from "RPG_ACHIEVEMENT_TABLE" where uin=%d",nUin);
}

bool	AchievementCenter::_insertObj(AchievementUserObj* pObj)
{
	m_pDB->ExecuteSQL(FALSE,"insert into "RPG_ACHIEVEMENT_TABLE" (uin,id,cnt,ifdone,ifshow) values (%d,%d,%d,%d,%d)",pObj->nUin,pObj->nId,pObj->nCnt,pObj->ifDone,pObj->ifShow);
	return true;
}

bool	AchievementCenter::_updateObj(AchievementUserObj* pObj)
{
	m_pDB->ExecuteSQL(FALSE,"update "RPG_ACHIEVEMENT_TABLE" set cnt=%d,ifdone=%d,ifshow=%d where uin=%d and id=%d",pObj->nCnt,pObj->ifDone,pObj->ifShow,pObj->nUin,pObj->nId);
	return true;
}

bool	AchievementCenter::_deleteObj(AchievementUserObj* pObj)
{
	m_pDB->ExecuteSQL(FALSE,"delete from "RPG_ACHIEVEMENT_TABLE" where uin=%d and id=%d",pObj->nUin,pObj->nId);
	return true;
}


UserAchievement::UserAchievement(UserData* pUser)
{
	m_pUser = pUser;
	m_nPoint = 0;
	m_pList = xnList::Create();
	g_pGameCenter->m_pAchievementCenter->GetUserObjList(m_pUser->m_dwUin,m_pList);
	for(int i = 0; i < m_pList->Count; i ++)
	{
		AchievementUserObj* pObj = (AchievementUserObj*)m_pList->Items[i];
		if(pObj->ifDone)
		{
			AchievemetnObj* pCheck = AchievementCfg::GetInstance().GetObj(pObj->nId);
			if(pCheck)
				m_nPoint += pCheck->nPoint;
		}
	}
}

UserAchievement::~UserAchievement()
{
	if(m_pList)
	{
		while(m_pList->Count)
			m_pList->Delete(0);
		m_pList->Free();
	}
}

AchievementUserObj* UserAchievement::GetObj(int nID)
{
	for(int i = 0; i < m_pList->Count; i ++)
	{
		AchievementUserObj* pObj = (AchievementUserObj*)m_pList->Items[i];
		if(pObj->nId == nID)
			return pObj;
	}
	return NULL;
}

bool	UserAchievement::AddAchievementVal(int nID,int nVal)
{
	AchievementUserObj* pObj = GetObj(nID);
	if(pObj)
	{
		if(pObj->ifDone)
			return true;
		AchievemetnObj* pTemp = AchievementCfg::GetInstance().GetObj(nID);
		if(pTemp == NULL)
			return false;
		if(pObj->nCnt < pTemp->nCnt)
			pObj->nCnt += nVal;
		if(pObj->nCnt >= pTemp->nCnt)
		{
			pObj->ifDone = true;
			m_nPoint += pTemp->nPoint;
		}
		if(pObj->ifDone && !pObj->ifShow)
		{
			SendShow(pObj->nId);
			pObj->ifShow = true;
		}
		//_updateObj(pObj);
		if(pObj->byState == 0)
			pObj->byState = 1;
		SendObj(pObj);
	}
	else
	{
		AchievemetnObj* pTemp = AchievementCfg::GetInstance().GetObj(nID);
		if(pTemp == NULL)
			return false;
		pObj = new AchievementUserObj();
		memset(pObj,0,sizeof(AchievementUserObj));
		pObj->nUin = m_pUser->m_dwUin;
		pObj->nId = nID;
		pObj->nCnt += nVal;
		if(pObj->nCnt >= pTemp->nCnt)
		{
			pObj->ifDone = true;
			m_nPoint += pTemp->nPoint;
		}
		if(pObj->ifDone && !pObj->ifShow)
		{
			SendShow(pObj->nId);
			pObj->ifShow = true;
		}
		AchievemetnObj* pCheck = AchievementCfg::GetInstance().GetObj(pObj->nId);
		pObj->nFloor = pCheck->nFloor;
		ATileObj* pFloor = AchievementCfg::GetInstance().GetFloor(pObj->nFloor);
		pObj->nGroup = pFloor->nGroup;
		//_insertObj(pObj);
		pObj->byState = 2;
		SendObj(pObj);
		m_pList->Add(pObj);
		g_pGameCenter->m_pAchievementCenter->m_pAchievementList->Add(pObj);
	}
	return true;
}

bool	UserAchievement::SetAchievementVal(int nID,int nVal)
{
	AchievementUserObj* pObj = GetObj(nID);
	int nDelta = 0;
	if(pObj)
	{
		if(pObj->ifDone)
			return true;
		nDelta = nVal - pObj->nCnt;
	}
	else
	{
		nDelta = nVal;
	}
	AddAchievementVal(nID,nDelta);
	return true;
}

bool	UserAchievement::DoneAchievement(int nID)
{
	AchievementUserObj* pObj = GetObj(nID);
	if(pObj)
	{
		if(pObj->ifDone)
			return true;
		pObj->ifDone = true;
		AchievemetnObj* pTemp = AchievementCfg::GetInstance().GetObj(nID);
		if(pTemp)
			m_nPoint += pTemp->nPoint;
		if(pObj->ifDone && !pObj->ifShow)
		{
			SendShow(pObj->nId);
			pObj->ifShow = true;
		}
		//_updateObj(pObj);
		if(pObj->byState == 0)
			pObj->byState = 1;
		SendObj(pObj);
	}
	else
	{
		AchievemetnObj* pTemp = AchievementCfg::GetInstance().GetObj(nID);
		if(pTemp == NULL)
			return false;
		pObj = new AchievementUserObj();
		memset(pObj,0,sizeof(AchievementUserObj));
		pObj->nUin = m_pUser->m_dwUin;
		pObj->nId = nID;
		pObj->ifDone = true;
		m_nPoint += pTemp->nPoint;
		if(pObj->ifDone && !pObj->ifShow)
		{
			SendShow(pObj->nId);
			pObj->ifShow = true;
		}
		AchievemetnObj* pCheck = AchievementCfg::GetInstance().GetObj(pObj->nId);
		pObj->nFloor = pCheck->nFloor;
		ATileObj* pFloor = AchievementCfg::GetInstance().GetFloor(pObj->nFloor);
		pObj->nGroup = pFloor->nGroup;
		//_insertObj(pObj);
		pObj->byState = 2;
		SendObj(pObj);
		m_pList->Add(pObj);
		g_pGameCenter->m_pAchievementCenter->m_pAchievementList->Add(pObj);
	}
	return true;
}

void	UserAchievement::SendAll()
{
	BuildPacketEx(pPacket,RPGACTIVITY,S_SEND_ACHIEVEMENT_ALL,buf,1024);
	SET_DATA(pData,RPGACTIVITY,S_SEND_ACHIEVEMENT_ALL,pPacket);
	int n = 0;
	BYTE p[1024];
	int nSize;
	for(int i = 0; i < m_pList->Count; i ++)
	{
		AchievementUserObj* pObj = (AchievementUserObj*)m_pList->Items[i];
		pData->xInfo[n].nID	= pObj->nId;
		pData->xInfo[n].nCnt = pObj->nCnt;
		pData->xInfo[n].ifDone = pObj->ifDone;
		pData->xInfo[n].nFloor = pObj->nFloor;
		pData->xInfo[n].nGroup = pObj->nGroup;
		n++;
		if(n >= 20)
		{
			pData->nCnt = n;
			pPacket->wSize += n*sizeof(AchievementUserObj);
			TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
			pPacket->wSize	= sizeof(RPGACTIVITY_S_SEND_ACHIEVEMENT_ALL_Data);
			n = 0;
		}
	}
	if(n > 0)
	{
		pData->nCnt = n;
		pPacket->wSize += n*sizeof(AchievementUserObj);
		TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
	}
}

void	UserAchievement::SendObj(AchievementUserObj* pObj)
{
	BuildPacketEx(pPacket,RPGACTIVITY,S_SEND_ACHIEVEMENT_CHG,buf,1024);
	SET_DATA(pData,RPGACTIVITY,S_SEND_ACHIEVEMENT_CHG,pPacket);
	pData->xInfo.nID = pObj->nId;
	pData->xInfo.nCnt = pObj->nCnt;
	pData->xInfo.ifDone = pObj->ifDone;
	pData->xInfo.nGroup = pObj->nGroup;
	pData->xInfo.nFloor = pObj->nFloor;
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
}

void	UserAchievement::SendShow(int nID)
{
	BuildPacketEx(pPacket,RPGACTIVITY,S_SEND_ACHIEVEMENT_SHOW,buf,1024);
	SET_DATA(pData,RPGACTIVITY,S_SEND_ACHIEVEMENT_SHOW,pPacket);
	pData->nID = nID;
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
}

void	UserAchievement::CheckShow()
{
	for(int i = 0; i < m_pList->Count; i ++)
	{
		AchievementUserObj* pObj = (AchievementUserObj*)m_pList->Items[i];
		if(pObj->ifDone && !pObj->ifShow)
		{
			SendShow(pObj->nId);
			pObj->ifShow = true;
			_updateObj(pObj);
		}
	}
}

void	UserAchievement::RequestForGift(int i)
{
	if(i < AchievementCfg::GetInstance().m_pAwardList->Count)
	{
		int nFlag = m_pUser->GetFlag(25);
		int nCheck = 0x01;
		nCheck = nCheck << i;
		bool ifCan = false;
		AchieveAward* pAward = (AchieveAward*)AchievementCfg::GetInstance().m_pAwardList->Items[i];
		switch(pAward->byType)
		{
		case 0:
			{
				if(m_nPoint >= pAward->nValue)
				{
					ifCan = true;
				}
			}
			break;
		case 1:
			{
				int nCount = 0;
				for(int j = 0; j < m_pList->Count; j ++)
				{
					AchievementUserObj* pCheck = (AchievementUserObj*)m_pList->Items[j];
					if(pCheck->nFloor == pAward->nValue)
						nCount ++;
				}
				ATileObj* pObj = AchievementCfg::GetInstance().GetFloor(pAward->nValue);
				if(pObj && pObj->nCount <= nCount)
					ifCan = true;
			}
			break;
		case 2:
			{
				int nCount = 0;
				for(int j = 0; j < m_pList->Count; j ++)
				{
					AchievementUserObj* pCheck = (AchievementUserObj*)m_pList->Items[j];
					if(pCheck->nGroup == pAward->nValue)
						nCount ++;
				}
				ATileObj* pObj = AchievementCfg::GetInstance().GetGroup(pAward->nValue);
				if(pObj && pObj->nCount <= nCount)
					ifCan = true;
			}
			break;
		}
		if(ifCan)
		{
			if((nFlag&nCheck) != nCheck)
			{
				for(int k = 0; k < pAward->pList->Count; k ++)
				{
					_simpleAward* pA = (_simpleAward*)pAward->pList->Items[k];
					m_pUser->AddItemEx(pA->nID,pA->nNum,RMB_SAVE_ACHIEVEMENT,UW_GOLD_ACHIEVENT,ITEM_LOG_ACHIEVENT);	
				}
				nFlag = nFlag|nCheck;
				m_pUser->SetFlag(25,nFlag);
				m_pUser->PlayEffect(nEffectSuccess);
			}
		}
	}
}

bool	UserAchievement::_insertObj(AchievementUserObj* pObj)
{
	m_pUser->m_pDB->ExecuteSQL(FALSE,"insert into "RPG_ACHIEVEMENT_TABLE" (uin,id,cnt,ifdone,ifshow) values (%d,%d,%d,%d,%d)",pObj->nUin,pObj->nId,pObj->nCnt,pObj->ifDone,pObj->ifShow);
	return true;
}

bool	UserAchievement::_updateObj(AchievementUserObj* pObj)
{
	m_pUser->m_pDB->ExecuteSQL(FALSE,"update "RPG_ACHIEVEMENT_TABLE" set cnt=%d,ifdone=%d,ifshow=%d where uin=%d and id=%d",pObj->nCnt,pObj->ifDone,pObj->ifShow,pObj->nUin,pObj->nId);
	return true;
}

bool	UserAchievement::_deleteObj(AchievementUserObj* pObj)
{
	m_pUser->m_pDB->ExecuteSQL(FALSE,"delete from "RPG_ACHIEVEMENT_TABLE" where uin=%d and id=%d",pObj->nUin,pObj->nId);
	return true;
}

void	UserAchievement::SaveAll()
{
	for(int i = 0; i < m_pList->Count; i ++)
	{
		AchievementUserObj* pObj = (AchievementUserObj*)m_pList->Items[i];
		if(pObj->byState == 1)
		{
			pObj->byState = 0;
			_updateObj(pObj);
		}
		else if(pObj->byState == 2)
		{
			pObj->byState = 0;
			_insertObj(pObj);
		}
	}
}