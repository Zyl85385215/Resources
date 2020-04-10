#include "stdafx.h"
#include "GameAwardCenter.h"
#include "UserData.h"
#include "UserFlag.h"
#include "GameAwardCfg.h"
#include "ios_award_packet.h"
#include "ServerCfg.h"

OnlineAwardCenter::OnlineAwardCenter(UserData* pUser)
{
	m_dwOnlineSec = 0;
	m_dwStart = GetTickCount()/1000;
	m_byGiftFlag = 0;
	m_pParent = pUser;
}

OnlineAwardCenter::~OnlineAwardCenter()
{

}

void OnlineAwardCenter::RefreshFlag()
{
	m_byGiftFlag = m_pParent->m_pUserFlag->GetFlag(6);
}

void OnlineAwardCenter::ResetFlag()
{
	m_pParent->m_pUserFlag->SetFlag(6,m_byGiftFlag);
}

void OnlineAwardCenter::UpdateSec()
{
	//m_dwOnlineSec ++;
	if(GetTickCount()/1000 > m_dwStart)
		m_dwOnlineSec = GetTickCount()/1000-m_dwStart;
	else
		m_dwOnlineSec = 0;
}

void OnlineAwardCenter::RequestForGift(int nPos)
{
	RefreshFlag();
	if(nPos == m_byGiftFlag)
	{
		OnlineAward* pAward = GameAwardConfig::GetInstance().GetOnlineAward(m_byGiftFlag);
		if(pAward == NULL)
			return;
		if((m_dwOnlineSec/60) >= pAward->nTime)
		{
			m_pParent->AddItemEx(pAward->xAward.nID,pAward->xAward.nNum,RMB_SAVE_ONLINEAWARD,UW_GOLD_ONLINEAWARD,ITEM_LOG_ONLINEAWARD);	
			//m_dwOnlineSec = 0;
			//m_dwStart = GetTickCount()/1000;
			_initOnlineSec();
			m_byGiftFlag++;
		}
		ResetFlag();
		SendInfo();
		m_pParent->PlayEffect(nEffectSuccess);
	}
}

void OnlineAwardCenter::SendInfo(bool ifRefresh)
{
	if(ifRefresh)
		RefreshFlag();
	BuildPacketEx(pPacket,RPGAWARD,S_SEND_ONLINEINFO,buf,1024);
	SET_DATA(pData,RPGAWARD,S_SEND_ONLINEINFO,pPacket);
	pData->byFlag = m_byGiftFlag;
	pData->dwSec = m_dwOnlineSec;
	TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
}

void OnlineAwardCenter::_initOnlineSec()
{
	m_dwOnlineSec = 0;
	m_dwStart = GetTickCount()/1000;
	m_pParent->m_pDB->ExecuteSQL(false,"update "RPG_USER_TABLE" set onlineawardsec = %d where uin = %d",m_dwOnlineSec,m_pParent->m_dwUin);
	return;
}

void OnlineAwardCenter::_SaveOnlineSec()
{
	m_pParent->m_pDB->ExecuteSQL(false,"update "RPG_USER_TABLE" set onlineawardsec = %d where uin = %d",m_dwOnlineSec,m_pParent->m_dwUin);
	return;
}

LvupAwardCenter::LvupAwardCenter(UserData* pUser)
{
	m_pParent = pUser;
	memset(m_szGetFlag,0,10);
}

LvupAwardCenter::~LvupAwardCenter()
{

}

void LvupAwardCenter::RefreshFlag()
{
	int nFlag = m_pParent->m_pUserFlag->GetFlag(9);
	int nCheck = 0x01;
	for(int i = 0; i < 10; i ++)
	{
		if(i > 0)
			nCheck = (nCheck<<1);
		if((nFlag&nCheck) == nCheck)
			m_szGetFlag[i] = 1;
		else
			m_szGetFlag[i] = 0;
	}
}

void LvupAwardCenter::RequestGift(int nPos)
{
	RefreshFlag();
	if(m_szGetFlag[nPos] || nPos >= GameAwardConfig::GetInstance().m_pLvupList->Count)
		return;
	LvupAward* pAward = (LvupAward*)GameAwardConfig::GetInstance().m_pLvupList->Items[nPos];
	if(m_pParent->m_xUWealth[UW_LV] < pAward->nLv)
		return;
	for(int i = 0; i < pAward->pList->Count; i ++)
	{
		_simpleAward* pA = (_simpleAward*)pAward->pList->Items[i];
		m_pParent->AddItemEx(pA->nID,pA->nNum,RMB_SAVE_LVUPAWARD,UW_GOLD_LVUPAWARD,ITEM_LOG_LVUPAWARD);	
	}
	int nCheck = 0x01;
	if(nPos > 0)
		nCheck = (nCheck<<nPos);
	int nFlag = m_pParent->m_pUserFlag->GetFlag(9);
	nFlag = (nFlag|nCheck);
	m_pParent->m_pUserFlag->SetFlag(9,nFlag);
	m_pParent->PlayEffect(nEffectSuccess);
}

LoginAwardCenter::LoginAwardCenter(UserData* pUser)
{
	m_pParent = pUser;
	memset(m_szGetFlag,0,30);
}

LoginAwardCenter::~LoginAwardCenter()
{

}

void LoginAwardCenter::RefreshFlag()
{
	int nFlag = m_pParent->m_pUserFlag->GetFlag(50);
	int nCheck = 0x01;
	for(int i = 0; i < 30; i ++)
	{
		if(i > 0)
			nCheck = (nCheck<<1);
		if((nFlag&nCheck) == nCheck)
			m_szGetFlag[i] = 1;
		else
			m_szGetFlag[i] = 0;
	}
}

void LoginAwardCenter::RequestGift(int nPos)
{
	RefreshFlag();

	if (m_pParent->m_bLoginAwarded)
	{
		m_pParent->SendWarnMsg("该奖励已领取，今日无法再次领取！");
		return;
	}

	xnList* pList = NULL;
	int nTmpDays  = 0;
	if (m_pParent->m_nLoginDays > 1000)
	{
		nTmpDays = m_pParent->m_nLoginDays - 1000;
		pList = GameAwardConfig::GetInstance().m_pLoginNewList;
	}
	else
	{
		nTmpDays = m_pParent->m_nLoginDays;
		pList = GameAwardConfig::GetInstance().m_pLoginList;
	}

	if(m_szGetFlag[nPos] || nPos >= pList->Count)
		return;
	LoginAward* pAward = (LoginAward*)pList->Items[nPos];
	if(nTmpDays < pAward->nDays)
		return;
	for(int i = 0; i < pAward->pList->Count; i ++)
	{
		_simpleAward* pA = (_simpleAward*)pAward->pList->Items[i];
		m_pParent->AddItemEx(pA->nID,pA->nNum,RMB_SAVE_LOGINGIFT,UW_GOLD_LOGINGIFT,ITEM_LOG_LOGINGIFT);	
	}

	m_pParent->SetLoginAwarded(true);

	int nCheck = 0x01;
	if(nPos > 0)
		nCheck = (nCheck<<nPos);
	int nFlag = m_pParent->m_pUserFlag->GetFlag(50);
	nFlag = (nFlag|nCheck);
	m_pParent->m_pUserFlag->SetFlag(50,nFlag);
	m_pParent->PlayEffect(nEffectSuccess);
}

RechargeAwardCenter::RechargeAwardCenter(UserData* pUser)
{
	m_pParent = pUser;
	m_ifRecharge = false;
	for (int i = 0; i < 3; i++)
	{
		m_ifGetEx[i] = false;
		m_CanGet[i]  = false;
	}
	m_ifGet = false;
}

RechargeAwardCenter::~RechargeAwardCenter()
{

}

void RechargeAwardCenter::RefreshFlag()
{
	int nFlag = m_pParent->m_pUserFlag->GetFlag(9);
	int nCheck = 0x01<<31;

	if(m_pParent->m_dwTotalRmb > 0)
		m_ifRecharge = true;
	else
		m_ifRecharge = false;

	//if(ServerCfg::GetInstance().nVersionDef)
	//	m_ifRecharge	= true;

	if(m_ifRecharge)
	{
		if((nFlag&nCheck) != nCheck)
		{
			nFlag = nFlag | nCheck;
			m_pParent->m_pUserFlag->SetFlag(9,nFlag);
		}
	}
	nCheck = 0x01<<30;
	if((nFlag&nCheck) == nCheck)
		m_ifGet = true;
	else
		m_ifGet = false;
}

void RechargeAwardCenter::RequestGift()
{
	RefreshFlag();
	if(!m_ifRecharge || m_ifGet)
		return;
	m_ifGet = true;
	xnList* pList = GameAwardConfig::GetInstance().m_pRechargeList;
	for(int i = 0; i < pList->Count; i ++)
	{
		RechargeAward* pAwardList = (RechargeAward*) pList->Items[i];
		for (int j = 0; j < pAwardList->pList->Count; j++)
		{
			_simpleAward* pAward = (_simpleAward*)pAwardList->pList->Items[j];
			m_pParent->AddItemEx(pAward->nID,pAward->nNum,RMB_SAVE_RECHARGEAWARD,UW_GOLD_RECHARGE_AWARD,ITEM_LOG_RECHANGE_AWARD);	
		}
	}
	int nCheck = 0x01<<30;
	int nFlag = m_pParent->m_pUserFlag->GetFlag(9);
	nFlag = (nFlag|nCheck);
	m_pParent->m_pUserFlag->SetFlag(9,nFlag);
	m_pParent->PlayEffect(nEffectSuccess);
}

void RechargeAwardCenter::ReSetDailyFlag()
{
	int nFlag = m_pParent->m_pUserFlag->GetFlag(51);
	if (nFlag)
	{
		int nCheck = 1;
		for(int i = 0; i < 3; i ++)
		{
			if(i > 0)
				nCheck = (nCheck<<1);
			if((nFlag&nCheck) != nCheck)
			{
				nFlag = (nFlag|nCheck);
				m_pParent->m_pUserFlag->SetFlag(51,nFlag);
				break;
			}
		}
	}
}

void RechargeAwardCenter::RefreshFlagEx()
{
	int nFlag = m_pParent->m_pUserFlag->GetFlag(51);
	int nCheck = 0x01<<31;

	if(m_pParent->m_dwTotalRmb > 0)
		m_ifRecharge = true;
	else
		m_ifRecharge = false;

	//if(ServerCfg::GetInstance().nVersionDef)
	//	m_ifRecharge	= true;

	if(m_ifRecharge)
	{
		if((nFlag&nCheck) != nCheck)
		{
			nFlag = nFlag | nCheck;
			nFlag = nFlag | 1;
			m_pParent->m_pUserFlag->SetFlag(51,nFlag);
		}
	}
	nCheck = 0x01<<30;
	for(int i = 0; i < 3; i ++)
	{
		if(i > 0)
			nCheck = (nCheck>>1);
		if((nFlag&nCheck) == nCheck)
			m_ifGetEx[i] = true;
		else
			m_ifGetEx[i] = false;
	}
	nCheck = 1;
	for (int j = 0; j < 3; j++)
	{
		if (j > 0)
			nCheck = (nCheck << 1);
		if((nFlag&nCheck) == nCheck)
			m_CanGet[j] = true;
		else
			m_CanGet[j] = false;
	}
}

void RechargeAwardCenter::RequestGiftEx(int nPos)
{
	if (nPos < 0 || nPos > 2)
		return;

	RefreshFlagEx();
	if(!m_ifRecharge|| !m_CanGet[nPos] || m_ifGetEx[nPos])
		return;
	m_ifGetEx[nPos] = true;
	xnList* pList = ((RechargeAward*)GameAwardConfig::GetInstance().m_pRechargeList->Items[nPos])->pList;
	for(int i = 0; i < pList->Count; i ++)
	{
		_simpleAward* pAward = (_simpleAward*)pList->Items[i];
		m_pParent->AddItemEx(pAward->nID,pAward->nNum,RMB_SAVE_RECHARGEAWARD,UW_GOLD_RECHARGE_AWARD,ITEM_LOG_RECHANGE_AWARD);	
	}
	int nCheck = 0x01<<30;
	if(nPos > 0)
		nCheck = (nCheck>>nPos);
	int nFlag = m_pParent->m_pUserFlag->GetFlag(51);
	nFlag = (nFlag|nCheck);
	m_pParent->m_pUserFlag->SetFlag(51,nFlag);
	m_pParent->PlayEffect(nEffectSuccess);
}

MounthAwardCenter::MounthAwardCenter(UserData* pUser)
{
	m_nStart = 0;
	m_pParent = pUser;
	m_nCount = 0;
	memset(m_szGetFlag,0,5);
	m_DayCheck = false;
}

MounthAwardCenter::~MounthAwardCenter()
{

}

void MounthAwardCenter::RefreshFlag()
{
	tm xNow = CMTime::GetCurTime();
	int nWeekDay = xNow.tm_wday;
	int nMounthDat = xNow.tm_mday;
	int nTemp = nMounthDat + (6-nWeekDay);
	m_nStart = 7 - (nTemp%7);
	m_nCount = 0;
	m_DayCheck = false;
	int nFlag1 = m_pParent->m_pUserFlag->GetFlag(7);
	int nFlag2 = m_pParent->m_pUserFlag->GetFlag(8);
	int nCheck = 0x01;
	for(int i = 0; i < 31; i ++)
	{
		if(i > 0)
			nCheck = nCheck << 1;

		if((nFlag1&nCheck) == nCheck)
		{
			m_nCount ++;
			if(i + 1 == nMounthDat)
				m_DayCheck = true;
		}
	}
	nCheck = 0x01;
	for(int i = 0; i < 5; i ++)
	{
		if(i > 0)
			nCheck = nCheck << 1;
		if((nFlag2&nCheck) == nCheck)
			m_szGetFlag[i] = 1;
		else
			m_szGetFlag[i] = 0;
	}
}

void MounthAwardCenter::ResetFlag()
{
	m_nCount = 0;
	m_nStart = 0;
	memset(m_szGetFlag,0,5);
	m_pParent->m_pUserFlag->SetFlag(7,0);
	m_pParent->m_pUserFlag->SetFlag(8,0);
}

void MounthAwardCenter::RequestSign(bool ifToday /* = true */)
{
	tm xNow = CMTime::GetCurTime();
	int nMday = xNow.tm_mday;
	RefreshFlag();
	if(ifToday)
	{
		if(m_DayCheck)
		{
			m_pParent->SendWarnMsg("你今天已经签到过了");
			return;
		}
		
		int nCheck = 0x01;
		if(nMday > 1)
			nCheck = nCheck << (nMday-1);
		int nFlag = m_pParent->m_pUserFlag->GetFlag(7);
		nFlag = nFlag | nCheck;
		m_pParent->m_pUserFlag->SetFlag(7,nFlag);
	}
	else
	{
		int nTotal = nMday;
		if(!m_DayCheck)
			nTotal --;
		if(m_nCount >= nTotal)
		{
			m_pParent->SendWarnMsg("你本月已签满，无需补签");
			return;
		}
		if(m_pParent->m_xUWealth[UW_RMB] < 5)
		{
			return;
		}
		m_pParent->m_xUWealth.SubWealth(UW_RMB,5,RMB_SAVE_SIGNUP_MOUNTH);
		int nCheck = 0x01;
		int nFlag = m_pParent->m_pUserFlag->GetFlag(7);
		for(int i = 0; i < 31; i ++)
		{
			if(i > 0)
				nCheck = nCheck<<1;
			if((nFlag&nCheck) != nCheck)
			{
				nFlag = nFlag | nCheck;
				break;
			}
		}
		m_pParent->m_pUserFlag->SetFlag(7,nFlag);
	}
}

void MounthAwardCenter::RequestForGift(int nPos)
{
	if(nPos >= 5)
		return;
	RefreshFlag();
	if(m_szGetFlag[nPos])
	{
		m_pParent->SendWarnMsg("奖励不能重复领取");
		return;
	}
	MounthAward* pAward = (MounthAward*)GameAwardConfig::GetInstance().m_pMounthList->Items[nPos];
	if(pAward == NULL)
	{
		m_pParent->SendWarnMsg("无奖励可以领取");
		return;
	}
	if(m_nCount < pAward->nCount)
	{
		m_pParent->SendWarnMsg("签到次数不足，请坚持每日签到");
		return;
	}
	m_pParent->AddItemEx(pAward->xAward.nID,pAward->xAward.nNum,RMB_SAVE_SIGNUP_MOUNTH,UW_GOLD_SIGUP_MOUTH,ITEM_LOG_SIGUP_MOUTH);	
	int nCheck = 0x01;
	if(nPos > 0)
		nCheck = nCheck << nPos;
	int nFlag = m_pParent->m_pUserFlag->GetFlag(8);
	nFlag = nFlag | nCheck;
	m_pParent->m_pUserFlag->SetFlag(8,nFlag);
	m_pParent->PlayEffect(nEffectSuccess);
}

void MounthAwardCenter::SendInfo()
{
	RefreshFlag();
	tm xNow = CMTime::GetCurTime();
	BuildPacketEx(pPacket,RPGAWARD,S_SEND_MOUNTHINFO,buf,1024);
	SET_DATA(pData,RPGAWARD,S_SEND_MOUNTHINFO,pPacket);
	pData->nStart = m_nStart;
	pData->nYear = xNow.tm_year + 1900;
	pData->nMounth = xNow.tm_mon + 1;
	pData->nDay = xNow.tm_mday;
	TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
}