#include "UserData.h"
#include "ClientAchievement.h"
#include "MainMenu.h"

CAchievementCenter::CAchievementCenter()
{
	m_pList = xnList::Create();
	m_pShowList = xnList::Create();
	m_nPoint = 0;
	m_nDoneCnt = 0;
}

CAchievementCenter::~CAchievementCenter()
{
	if(m_pList)
	{
		while(m_pList->Count)
			delete (ClientAchievement*)m_pList->Delete(0);
		m_pList->Free();
	}
	if(m_pShowList)
	{
		while(m_pShowList->Count)
			m_pShowList->Delete(0);
		m_pShowList->Free();
	}
}

void	CAchievementCenter::ClearList()
{
	while(m_pList->Count)
		delete (ClientAchievement*)m_pList->Delete(0);
	while(m_pShowList->Count)
		m_pShowList->Delete(0);
	m_ifChg = true;
	m_nPoint = 0;
	m_nDoneCnt = 0;
}

void	CAchievementCenter::UpdateObj(ClientAchievement* pObj)
{
	for(int i = 0; i < m_pList->Count; i ++)
	{
		ClientAchievement* pCheck = (ClientAchievement*)m_pList->Items[i];
		if(pCheck->nID == pObj->nID)
		{
			memcpy(pCheck,pObj,sizeof(ClientAchievement));
			return;
		}
	}
	ClientAchievement* pNew = new ClientAchievement();
	memcpy(pNew,pObj,sizeof(ClientAchievement));
	m_pList->Add(pNew);
}

ClientAchievement* CAchievementCenter::GetObj(int nID)
{

	for(int i = 0; i < m_pList->Count; i ++)
	{
		ClientAchievement* pCheck = (ClientAchievement*)m_pList->Items[i];
		if(pCheck->nID == nID)
		{
			return pCheck;
		}
	}
	return NULL;
}

void	CAchievementCenter::SetPoint()
{
	m_nPoint = 0;
	m_nDoneCnt = 0;
	for(int i = 0; i < m_pList->Count; i ++)
	{
		ClientAchievement* pTemp = (ClientAchievement*)m_pList->Items[i];
		if(pTemp->ifDone)
		{
			AchievemetnObj* pCheck = AchievementCfg::GetInstance().GetObj(pTemp->nID);
			if(pCheck)
			{
				m_nPoint += pCheck->nPoint;
				m_nDoneCnt ++;
			}
		}
	}
}


void	CAchievementCenter::RequestForGift(int i)
{
	if(i < AchievementCfg::GetInstance().m_pAwardList->Count)
	{
		BuildPacketEx(pPacket,RPGACTIVITY,C_REQUEST_ACHIEVEMENT_GIFT,buf,1024);
		SET_DATA(pData,RPGACTIVITY,C_REQUEST_ACHIEVEMENT_GIFT,pPacket);
		pData->nPos = i;
		SendPacket(pPacket);
	}
}

bool	CAchievementCenter::CheckAward(int i)
{
	if(i < AchievementCfg::GetInstance().m_pAwardList->Count)
	{
		AchieveAward* pAward = (AchieveAward*)AchievementCfg::GetInstance().m_pAwardList->Items[i];
		switch(pAward->byType)
		{
		case 0:
			{
				if(m_nPoint >= pAward->nValue)
					return true;
			}
			break;
		case 1:
			{
				int nCount = 0;
				for(int j = 0; j < m_pList->Count; j ++)
				{
					ClientAchievement* pCheck = (ClientAchievement*)m_pList->Items[j];
					if(pCheck->nFloor == pAward->nValue)
						nCount ++;
				}
				ATileObj* pObj = AchievementCfg::GetInstance().GetFloor(pAward->nValue);
				if(pObj && pObj->nCount <= nCount)
					return true;
			}
			break;
		case 2:
			{
				int nCount = 0;
				for(int j = 0; j < m_pList->Count; j ++)
				{
					ClientAchievement* pCheck = (ClientAchievement*)m_pList->Items[j];
					if(pCheck->nGroup == pAward->nValue)
						nCount ++;
				}
				ATileObj* pObj = AchievementCfg::GetInstance().GetGroup(pAward->nValue);
				if(pObj && pObj->nCount <= nCount)
					return true;
			}
			break;
		}
	}
	return false;
}

bool	CAchievementCenter::PacketProcess(ZPacket* pPacket)
{
	if(pPacket->bCmdGroup != RPGACTIVITY)
		return false;
	switch(pPacket->bCmd)
	{
	case S_SEND_ACHIEVEMENT_ALL:
		{
			SET_DATA(pRecv,RPGACTIVITY,S_SEND_ACHIEVEMENT_ALL,pPacket);
			for(int i = 0; i < pRecv->nCnt; i ++)
			{
				ClientAchievement* pNew = new ClientAchievement();
				memcpy(pNew,&pRecv->xInfo[i],sizeof(ClientAchievement));
				m_pList->Add(pNew);
			}
			SetPoint();
		}
		return true;
	case S_SEND_ACHIEVEMENT_CHG:
		{
			SET_DATA(pRecv,RPGACTIVITY,S_SEND_ACHIEVEMENT_CHG,pPacket);
			UpdateObj(&pRecv->xInfo);
			SetPoint();
		}
		return true;
	case S_SEND_ACHIEVEMENT_SHOW:
		{
			SET_DATA(pRecv,RPGACTIVITY,S_SEND_ACHIEVEMENT_SHOW,pPacket);
			m_pShowList->Add((void*)pRecv->nID);
		}
		return true;
	}
	return false;
}