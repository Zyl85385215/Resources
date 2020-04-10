#include "stdafx.h"
#include "VIPCenter.h"
#include "ChatCenter.h"
#include "UserData.h"
#include "UserFlag.h"

VIPCenter::VIPCenter(UserData* pUser)
{
	m_pObj = NULL;
	m_pParent = pUser;
}

VIPCenter::~VIPCenter()
{

}

void VIPCenter::AddVIPRMB(int nVal)
{
	if(nVal <= 0)
		return;
//	m_pParent->m_xUWealth.AddWealth(UW_RMB,nVal,RMB_SAVE_RECHARGE);
	int nOld = m_pParent->m_xUWealth[UW_VIPLV];
	m_pParent->m_xUWealth.AddWealth(UW_VIPEXP,nVal);
	if(nOld != m_pParent->m_xUWealth[UW_VIPLV])
	{
		Reflesh(m_pParent->m_xUWealth[UW_VIPLV]);
		SendChgAutoRight(m_pParent->m_xUWealth[UW_VIPLV]);
	}
}

void VIPCenter::SendChgAutoRight(int nVipLV)
{
	switch(nVipLV)
	{
	case 1:
		TCP_SendCmdToUin(m_pParent->m_dwUin,USER,C_SET_AUTOSKILL);
		break;
	case 3:
		TCP_SendCmdToUin(m_pParent->m_dwUin,USER,S_SET_AUTOFIX);
		break;
	case 5:
		TCP_SendCmdToUin(m_pParent->m_dwUin,USER,S_SET_AUTOBUY);
		break;
	}
}

void VIPCenter::Reflesh(BYTE byLv)
{
	m_pObj = VipConfig::GetInstance().GetObjByLv(byLv);
	if(m_pObj && m_pObj->m_nExpadd > 0)
		m_pParent->m_pUserExpCenter->AddObj(m_pObj->m_nExpadd);
	m_pParent->m_pMObjCenter->m_pPlayer->ChangeViplv(byLv);
}

void VIPCenter::SendWorldMessage(bool ifjoin)
{
	if(m_pObj == NULL || !m_pObj->m_proclamtion)
		return;
	char szMsg[128];
	if(ifjoin)
		sprintf(szMsg,"尊贵的%d级VIP%s上线了",m_pObj->m_nLv,m_pParent->m_szNick);
	else
		sprintf(szMsg,"尊贵的%d级VIP%s下线了",m_pObj->m_nLv,m_pParent->m_szNick);
	ChatCenter::GetInstance().SendSystemMsg(szMsg,0,0,1);
}

int		VIPCenter::GetExpAdd(int nExp)
{
	if(m_pObj && m_pObj->m_nExpadd > 0)
		nExp += nExp * m_pObj->m_nExpadd / 100;
	return nExp;
}

int		VIPCenter::GetHonourAdd(int nHonour)
{
	if(m_pObj && m_pObj->m_nPrestigeadd > 0)
		nHonour += nHonour * m_pObj->m_nPrestigeadd / 100;
	return nHonour;
}

int		VIPCenter::GetExFields()
{
	if(m_pObj)
		return m_pObj->m_nField;
	return 0;
}

bool	VIPCenter::CheckFixSwitch()
{
	if(m_pObj)
		return m_pObj->m_fixswitch;
	return false;
}

bool	VIPCenter::CheckBuySwitch()
{
	if(m_pObj)
		return m_pObj->m_buyswitch;
	return false;
}

int		VIPCenter::GetChatVal()
{
	if(m_pObj && m_pObj->m_chatswitch)
		return m_pObj->m_nLv;
	return 0;
}

int		VIPCenter::GetVipLv()
{
	if(m_pObj)
		return m_pObj->m_nLv;
	return 0;
}

void	VIPCenter::RequestForGift(int nLv)
{
	//int nBaseID = 46010;
	if(nLv > 0 && nLv <= m_pParent->m_xUWealth[UW_VIPLV] > 0)
	{
		int nFlag = m_pParent->m_pUserFlag->GetFlag(16);
		int nDelta = nLv-1;
		int nCheck = 0x01;
		if(nDelta > 0)
			nCheck = nCheck << nDelta;
		//int nID = nBaseID + nDelta;
		if((nFlag & nCheck) != nCheck)
		{
			//m_pParent->m_pItemCenter->AddItemByID(nID);
			VipAward* pAward = VipAwardConfig::GetInstance().GetObjByVipLv(nLv);
			if (!pAward)
				return;

			//m_pParent->AddWealth(UW_GOLD,pAward->nGold);
			m_pParent->m_xUWealth.AddWealth(UW_GOLD,pAward->nGold,1,NULL,UW_GOLD_VIP_GIFT);
			for (int i = 0; i < pAward->pList->Count; i++)
			{
				int nIDandNum = (int)pAward->pList->Items[i];
				m_pParent->m_pItemCenter->AddItemByID(nIDandNum/1000,nIDandNum%1000,1,ITEM_LOG_VIP);
			}

			nFlag = nFlag | nCheck;
			m_pParent->m_pUserFlag->SetFlag(16,nFlag);

			if (nLv == 9)
				m_pParent->AddState(8246,false);
			if (nLv == 10)
			{
				m_pParent->AddState(8246,true);
				m_pParent->AddState(8247,false);
			}
			if (nLv == 11)
			{
				m_pParent->AddState(8246,true);
				m_pParent->AddState(8247,true);
				m_pParent->AddState(8248,false);
			}
			if (nLv == 12)
			{
				m_pParent->AddState(8246,true);
				m_pParent->AddState(8247,true);
				m_pParent->AddState(8248,true);
				m_pParent->AddState(8249,false);
			}
		}
	}
}

void	VIPCenter::CheckAddVipState()
{
	int nVipLv = m_pParent->m_xUWealth[UW_VIPLV];
	if (nVipLv < 9)
		return;

	if (nVipLv == 9)
	{
		int nFlag = m_pParent->m_pUserFlag->GetFlag(16);
		int nCheck = 0x01;
		nCheck = nCheck << 8;
		if((nFlag & nCheck) == nCheck)
		{
			if (!m_pParent->CheckState(8246))
			{
				m_pParent->AddState(8246,false);
			}
		}
	}

	if (nVipLv == 10)
	{
		int nFlag = m_pParent->m_pUserFlag->GetFlag(16);
		int nCheck = 0x01;
		nCheck = nCheck << 9;
		if((nFlag & nCheck) == nCheck)
		{
			if (m_pParent->CheckState(8246))
			{
				m_pParent->AddState(8246,true);
			}

			if (!m_pParent->CheckState(8247))
			{
				m_pParent->AddState(8247,false);
			}
		}
	}
	if (nVipLv == 11)
	{
		int nFlag = m_pParent->m_pUserFlag->GetFlag(16);
		int nCheck = 0x01;
		nCheck = nCheck << 10;
		if((nFlag & nCheck) == nCheck)
		{
			if (m_pParent->CheckState(8246))
				m_pParent->AddState(8246,true);
			if (m_pParent->CheckState(8247))
				m_pParent->AddState(8247,true);

			if (!m_pParent->CheckState(8248))
				m_pParent->AddState(8248,false);
		}
	}
	if (nVipLv == 12)
	{
		int nFlag = m_pParent->m_pUserFlag->GetFlag(16);
		int nCheck = 0x01;
		nCheck = nCheck << 11;
		if((nFlag & nCheck) == nCheck)
		{
			if (m_pParent->CheckState(8246))
				m_pParent->AddState(8246,true);
			if (m_pParent->CheckState(8247))
				m_pParent->AddState(8247,true);
			if (m_pParent->CheckState(8248))
				m_pParent->AddState(8248,true);

			if (!m_pParent->CheckState(8249))
				m_pParent->AddState(8249,false);
		}
	}
}

void	VIPCenter::RequestGetPower()
{
	if(m_pObj && m_pObj->m_nPoweradd > 0)
	{
		if (m_pParent->m_xUWealth.GetWealth(UW_POW) > 49)
			return;

		int nFlag = m_pParent->m_pUserFlag->GetFlag(17);
		if(nFlag == 0)
		{
			m_pParent->m_xUWealth.AddWealth(UW_POW,m_pObj->m_nPoweradd);
			m_pParent->m_pUserFlag->SetFlag(17,m_pObj->m_nPoweradd);
		}
	}
}