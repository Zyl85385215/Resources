#include "GameAwardData.h"
#include "ios_award_packet.h"
#include "GameAwardCfg.h"
#include "MainMenu.h"

OnlineAwardCenter::OnlineAwardCenter()
{
	m_byFlag = 0;
	m_dwSec = 0;
	m_dwLeftsec = 0;
	m_ifShine = false;
}

OnlineAwardCenter::~OnlineAwardCenter()
{

}

void OnlineAwardCenter::UpdateSec()
{
// 	OnlineAward* pAward = GameAwardConfig::GetInstance().GetOnlineAward(m_byFlag);
// 	if(pAward == NULL)
// 		m_dwLeftsec = 0;
// 	else if(m_dwSec >= pAward->nTime*60)
// 		m_dwLeftsec = 0;
// 	else
// 		m_dwLeftsec = pAward->nTime*60-m_dwSec;
	int nNowSec = xnGetSecCount();
	if(m_dwSec == 0)
		m_dwLeftsec = 0;
	else if(m_dwSec > nNowSec)
		m_dwLeftsec = m_dwSec - nNowSec;
	else
		m_dwLeftsec = 0;
	if(m_byFlag < 5 && m_dwLeftsec == 0)
		m_ifShine = true;
	else
		m_ifShine = false;
}

void OnlineAwardCenter::RefreshFlag(BYTE byFlag,DWORD dwSec)
{
	m_byFlag = byFlag;
	OnlineAward* pAward = GameAwardConfig::GetInstance().GetOnlineAward(m_byFlag);
	if(pAward == NULL)
	{
		m_dwSec = 0;
		m_dwLeftsec = 0;
	}
	else
	{
		m_dwSec = xnGetSecCount() + pAward->nTime*60 - dwSec;
		m_dwLeftsec = pAward->nTime*60-m_dwLeftsec;
	}
}

bool OnlineAwardCenter::PacketProcess(ZPacket* pPacket)
{
	if(pPacket->bCmdGroup != RPGAWARD)
		return false;
	switch(pPacket->bCmd)
	{
	case S_SEND_ONLINEINFO:
		{
			SET_DATA(pRecv,RPGAWARD,S_SEND_ONLINEINFO,pPacket);
			RefreshFlag(pRecv->byFlag,pRecv->dwSec);
		}
		return true;
	}
	return false;
}

void OnlineAwardCenter::RequestForGift(BYTE byPos)
{
	if(m_dwLeftsec > 0 || byPos > m_byFlag)
	{
		pMainMenu->PopTipMsg("时间没到，暂时不能领取");
	}
	
	BuildPacketEx(pPacket,RPGAWARD,C_REQUEST_ONLINEGIFT,buf,1024);
	SET_DATA(pData,RPGAWARD,C_REQUEST_ONLINEGIFT,pPacket);
	pData->byPos = byPos;
	SendPacket(pPacket);
}

MounthAwardCenter::MounthAwardCenter()
{
	m_nStart = 0;
	m_nYear = 0;
	m_nMounth = 0;
	m_nDay = 0;
	m_nCount = 0;
	m_nLeftCount = 0;
	memset(m_szGetFlag,0,5);
	memset(m_szSignFlag,0,31);
	m_ifShine = false;
}

MounthAwardCenter::~MounthAwardCenter()
{

}

bool MounthAwardCenter::PacketProcess(ZPacket* pPacket)
{
	if(pPacket->bCmdGroup != RPGAWARD)
		return false;
	switch(pPacket->bCmd)
	{
	case S_SEND_MOUNTHINFO:
		{
			SET_DATA(pData,RPGAWARD,S_SEND_MOUNTHINFO,pPacket);
			m_nStart = pData->nStart;
			m_nYear = pData->nYear;
			m_nMounth = pData->nMounth;
			m_nDay = pData->nDay;
		}
		return true;
	}
	return false;
}

void MounthAwardCenter::UpdateSec()
{
	RefreshFlag();
}

void MounthAwardCenter::RefreshFlag()
{
	m_nLeftCount = 0;
	m_nCount = 0;
	int nFlag1 = pChar->m_pClientFlag->GetFlag(7);
	int nFlag2 = pChar->m_pClientFlag->GetFlag(8);
	int nCheck = 0x01;
	for(int i = 0; i < 31; i ++)
	{
		if(i > 0)
			nCheck = nCheck << 1;

		if((nFlag1&nCheck) == nCheck)
		{
			m_szSignFlag[i] = true;
			m_nCount ++;
		}
		else
		{
			m_szSignFlag[i] = false;
			if(i + 1 < m_nDay)
				m_nLeftCount ++;
		}
	}
	m_ifShine = false;
	nCheck = 0x01;
	for(int i = 0; i < 5; i ++)
	{
		if(i > 0)
			nCheck = nCheck << 1;
		if((nFlag2&nCheck) == nCheck)
			m_szGetFlag[i] = true;
		else
		{
			m_szGetFlag[i] = false;
			MounthAward*pAward = (MounthAward*)GameAwardConfig::GetInstance().m_pMounthList->Items[i];
			if(m_nCount >= pAward->nCount)
				m_ifShine = true;
		}
	}
}

bool	MounthAwardCenter::CheckTodaySign()
{
	for(int i = 0; i < 42; i ++)
	{

		if(i < m_nStart || i > (m_nStart+30))
			continue;
		else
		{
			if(m_szSignFlag[i-m_nStart])
			{
				continue;
			}
			else
			{
				if((i-m_nStart + 1) == m_nDay)
					return true;
			}
		}
	}
	return false;
}

void	MounthAwardCenter::RequestSign(bool ifToday)
{
	BuildPacketEx(pPacket,RPGAWARD,C_REQUEST_MOUNTH_SIGN,buf,1024);
	SET_DATA(pData,RPGAWARD,C_REQUEST_MOUNTH_SIGN,pPacket);
	pData->ifToday = ifToday;
	SendPacket(pPacket);
}
void	MounthAwardCenter::RequestGift(int nPos)
{
	BuildPacketEx(pPacket,RPGAWARD,C_REQUEST_MOUNTH_GIFT,buf,1024);
	SET_DATA(pData,RPGAWARD,C_REQUEST_MOUNTH_GIFT,pPacket);
	pData->nPos = nPos;
	SendPacket(pPacket);
}

LvupAwardCenter::LvupAwardCenter()
{
	memset(m_szGetFlag,0,10);
	m_ifShine = false;
}

LvupAwardCenter::~LvupAwardCenter()
{

}

void LvupAwardCenter::UpdateSec()
{
	RefreshFlag();
}

void LvupAwardCenter::RefreshFlag()
{
	int nFlag = pChar->m_pClientFlag->GetFlag(9);
	int nCheck = 0x01;
	m_ifShine = false;
	for(int i = 0; i < 10; i ++)
	{
		if(i > 0)
			nCheck = (nCheck<<1);
		if((nFlag&nCheck) == nCheck)
			m_szGetFlag[i] = true;
		else
		{
			m_szGetFlag[i] = false;
			LvupAward* pAward = (LvupAward*)GameAwardConfig::GetInstance().m_pLvupList->Items[i];
			if(pChar->m_byLvl >= pAward->nLv)
				m_ifShine = true;
		}
	}
}

void LvupAwardCenter::RequestGift(int nPos)
{
	if(nPos > 10 || m_szGetFlag[nPos])
	{
		pMainMenu->PopTipMsg("奖励已领取，不能重复领取");
		return;
	}
	BuildPacketEx(pPacket,RPGAWARD,C_REQUEST_LVUPGIFT,buf,1024);
	SET_DATA(pData,RPGAWARD,C_REQUEST_LVUPGIFT,pPacket);
	pData->nPos = nPos;
	pChar->SendPacket(pPacket);
}

LoginAwardCenter::LoginAwardCenter()
{
	memset(m_szGetFlag,0,30);
	m_ifShine = false;
	m_ifWarn  = false;

	m_nOpenStap = 0;
}

LoginAwardCenter::~LoginAwardCenter()
{

}

void LoginAwardCenter::UpdateSec()
{
	RefreshFlag();
}

void LoginAwardCenter::RefreshFlag()
{
	m_ifWarn = true;
	int nFlag = pChar->m_pClientFlag->GetFlag(50);
	int nCheck = 0x01;
	m_ifShine = false;

	for (int j = 0; j < 30; j++)
	{
		m_szGetFlag[j] = false;
	}

	xnList* pList = NULL;
	int nTmpLD = 0;
	if (pChar->m_nLoginDays > 1000)
	{
		nTmpLD = pChar->m_nLoginDays - 1000;
		pList = GameAwardConfig::GetInstance().m_pLoginNewList;
	}
	else
	{
		nTmpLD = pChar->m_nLoginDays;
		pList = GameAwardConfig::GetInstance().m_pLoginList;
	}

	for(int i = 0; i < pList->Count; i ++)
	{
		if(i > 0)
			nCheck = (nCheck<<1);
		if((nFlag&nCheck) == nCheck)
			m_szGetFlag[i] = true;
		else
		{
			m_szGetFlag[i] = false;
			LoginAward* pAward = (LoginAward*)pList->Items[i];
			if(nTmpLD >= pAward->nDays)
			{
				m_ifShine = true;
				m_nOpenStap = pAward->nDays;
			}
		}
	}
}

void LoginAwardCenter::RequestGift(int nPos)
{
	if(nPos >= 30 || m_szGetFlag[nPos])
	{
		pMainMenu->PopTipMsg("奖励已领取，不能重复领取");
		return;
	}
	BuildPacketEx(pPacket,RPGAWARD,C_REQUEST_LOGINGIFT,buf,1024);
	SET_DATA(pData,RPGAWARD,C_REQUEST_LOGINGIFT,pPacket);
	pData->nPos = nPos;
	pChar->SendPacket(pPacket);
}

RechargeAwardCenter::RechargeAwardCenter()
{
	m_ifRecharge = false;
	memset(m_ifGet,0,3);
	memset(m_CanGet,0,3);
	m_ifShine = false;
	m_ifWarn = false;
}

RechargeAwardCenter::~RechargeAwardCenter()
{

}

void RechargeAwardCenter::UpdateSec()
{
	RefreshFlag();
}

void RechargeAwardCenter::RefreshFlag()
{
	for (int k = 0; k < 3; k++)
	{
		m_ifGet[k] = false;
	}

	m_ifWarn = true;
	int nFlag = pChar->m_pClientFlag->GetFlag(51);
	int nCheck = 0x01<<31;
	if((nFlag&nCheck) == nCheck)
		m_ifRecharge = true;
	else
		m_ifRecharge = false;

	nCheck = 0x01<<30;
	m_ifShine = false;
	if (m_ifRecharge)
	{
		for(int i = 0; i < 3; i ++)
		{
			if(i > 0)
				nCheck = (nCheck>>1);
			if((nFlag&nCheck) == nCheck)
				m_ifGet[i] = true;
			else
			{
				m_ifGet[i] = false;
			}
		}
		nCheck = 1;
		for (int j = 0; j < 3; j++)
		{
			if (j > 0)
				nCheck = (nCheck << 1);
			if((nFlag&nCheck) == nCheck)
			{
				m_CanGet[j] = 1;
				if (m_ifGet[j] == false)
					m_ifShine = true;
			}
			else
				m_CanGet[j] = 0;
		}
	}
}

void RechargeAwardCenter::RequestGift(int nPos)
{
	if(!m_ifRecharge)
	{
		pMainMenu->PopTipMsg("未充值，不能领取");
		return;
	}
	if(nPos > 2 || m_ifGet[nPos])
	{
		pMainMenu->PopTipMsg("您已领取过，不能重复领取");
		return;
	}

	BuildPacketEx(pPacket,RPGAWARD,C_REQUEST_RECHARGEGIFT_EX,buf,1024);
	SET_DATA(pData,RPGAWARD,C_REQUEST_RECHARGEGIFT_EX,pPacket);
	pData->nPos = nPos;
	pChar->SendPacket(pPacket);
}