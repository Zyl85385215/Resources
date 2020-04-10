#include "stdafx.h"
#include "UserDataEx.h"
#include "UserData.h"
#include "ios_user_packet.h"

UserLuckData::UserLuckData(UserData* pData):m_pData(pData)
{
	m_nAwardCnt	= 0;
}

int UserLuckData::GetFreePower()
{
	if (m_dwVipLv<1)
	{
		return 0;
	}else if (m_dwVipLv<=1)
	{
		return 1;
	}else if (m_dwVipLv<=2)
	{
		return 2;
	}else{
		return 3;
	}
}

int UserLuckData::GetMoneyEquipLvUp()
{
	if (m_dwVipLv<2)
	{
		return 2;
	}else if (m_dwVipLv<=2)
	{
		return 3;
	}else if (m_dwVipLv<=3)
	{
		return 4;
	}else{
		return 5;
	}
}

bool UserLuckData::CanCreateLeague()
{
	return m_dwVipLv>2?true:false;
}

int UserLuckData::GetPetCritic()
{
	if (m_dwVipLv<4)
	{
		return 3;
	}else if (m_dwVipLv<=4)
	{
		return 4;
	}else if (m_dwVipLv<=5)
	{
		return 5;
	}else{
		return 6;
	}
}

int UserLuckData::GetMagicExpPer()
{
	if (m_dwVipLv<5)
	{
		return 0;
	}else{
		return (m_dwVipLv-4)*3;
	}
}

int UserLuckData::GetFreePVPTimes()
{
	if (m_dwVipLv<6)
	{
		return 0;
	}else{
		return m_dwVipLv-5;
	}
}

int UserLuckData::GetFreeJJCTimes()
{
	if (m_dwVipLv<7)
	{
		return 0;
	}else{
		return m_dwVipLv-6;
	}
}

int UserLuckData::GetFreeLeaderSkill()
{
	if (m_dwVipLv<8)
	{
		return 2;
	}else{
		return m_dwVipLv-5;
	}
}

int UserLuckData::GetLeagueContributeTimes()
{
	if (m_dwVipLv<9)
	{
		return 1;
	}else{
		return m_dwVipLv-7;
	}
}

int UserLuckData::GetMazeTimes()
{
	if (m_dwVipLv<10)
	{
		return 3;
	}else{
		return m_dwVipLv-6;
	}
}

int UserLuckData::GetConvoyTimes()
{
	if (m_dwVipLv<11)
	{
		return 5;
	}else{
		return m_dwVipLv-5;
	}
}

int UserLuckData::GetMonsterTimes()
{
	if (m_dwVipLv<12)
	{
		return 3;
	}else{
		return m_dwVipLv-8;
	}
}

int UserLuckData::GetEndLessTimes()
{
	if (m_dwVipLv<13)
	{
		return 1;
	}else{
		return m_dwVipLv-11;
	}
}

static int	s_szLuckPos3[]	= {6,12,18,24,9,21,15};
static int	s_szLuckPosA[]	= {5,11,17,22,23};
static int	s_szLuckPosB[]	= {1,10,13};
static int	s_szLuckPosC[]	= {7,19};
static int	s_szLuckPosD[]	= {2,14};

BYTE UserLuckData::CalcLuckBetRtl( BYTE byCoin, WORD szVal[] )
{
	int	nTotalVal	= 0;
	for (int i = 0; i < 8; i++)
	{
		nTotalVal	+= szVal[i];
	}
	int	nRand	= 1000;
	if(byCoin  == 0)
	{//元宝
		if(nTotalVal > m_pData->m_xUWealth[UW_RMB])
			return 0;

		nRand	= rand()%1800;
	}
	else
	{//金币
		if(nTotalVal > m_pData->m_xUWealth[UW_GOLD])
			return 0;

		nRand	= rand()%1000;
	}

	BYTE	byRtl	= 0;
	
	int	nAwardCnt	= 0;
	if(nRand < 120)
	{
		byRtl	= s_szLuckPosA[rand()%5];
		nAwardCnt	= szVal[7]*5;
	}
	else if(nRand >= 120 && nRand < 180)
	{
		byRtl	= s_szLuckPosB[rand()%3];
		nAwardCnt	= szVal[6]*10;
	}
	else if(nRand >= 180 && nRand < 220)
	{
		byRtl	= s_szLuckPosC[rand()%2];
		nAwardCnt	= szVal[5]*15;
	}
	else if(nRand >= 220 && nRand < 250)
	{
		byRtl	= s_szLuckPosD[rand()%2];
		nAwardCnt	= szVal[4]*20;
	}
	else if(nRand >= 250 && nRand < 280)
	{
		byRtl	= 8;
		nAwardCnt	= szVal[3]*20;
	}
	else if(nRand >= 280 && nRand < 300)
	{
		byRtl	= 20;
		nAwardCnt	= szVal[2]*30;
	}
	else if(nRand >= 300 && nRand < 315)
	{
		byRtl	= 16;
		nAwardCnt	= szVal[1]*40;
	}
	else if(nRand >= 315 && nRand < 327)
	{
		byRtl	= 3;
		nAwardCnt	= szVal[0]*50;
	}
	else if(nRand >= 327 && nRand < 332)
	{
		byRtl	= 4;
		nAwardCnt	= szVal[0]*120;
	}
	else
	{
		BYTE	byRnd	= rand()%7;
		byRtl	= s_szLuckPos3[byRnd];

		nAwardCnt	= szVal[7-byRnd]*3;
	}

	if(byCoin  == 0)
	{//元宝
		m_pData->m_xUWealth.SubWealth(UW_RMB,nTotalVal,RMB_SAVE_GAMBLE);

	}
	else
	{//金币
		m_pData->m_xUWealth.SubWealth(UW_GOLD,nTotalVal);

	}

	if(nAwardCnt)
	{
		m_byCoinType	= byCoin;
		m_nAwardCnt		= nAwardCnt;
	}

	return byRtl;
}

void UserLuckData::GetLuckBetAward()
{
	if(m_nAwardCnt == 0)
		return;
	if(m_byCoinType  == 0)
	{//元宝
		m_pData->m_xUWealth.AddWealth(UW_RMB,m_nAwardCnt,RMB_SAVE_GAMBLE);
	}
	else
	{//金币
		m_pData->m_xUWealth.AddWealth(UW_GOLD,m_nAwardCnt,1,NULL,UW_GOLD_LUCK_AWAED);
	}
	m_nAwardCnt	= 0;
}

BYTE UserLuckData::CalcLuckBetRtlEx(BYTE bySide)
{
	if(rand()%10 < 4)
	{
		m_nAwardCnt	*=2;
		if(bySide == 0)
			return rand()%6+1;
		else
			return rand()%6+8;

	}
	else
	{
		m_nAwardCnt	= 0;
		if(bySide == 0)
			return rand()%7+7;
		else
			return rand()%7+1;
	}
	return 0;
}
