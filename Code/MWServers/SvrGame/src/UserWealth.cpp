#include "stdafx.h"
#include "UserWealth.h"
#include "DBControl.h"
#include "ios_user_packet.h"
#include "ZNetGamesvr.h"
#include "UserConfig.h"
#include "UserData.h"
#include "VipConfig.h"
#include "FsGameCenter.h"
#include "UserList.h"
#include "MarkGameDB.h"
#include "RankCenter.h"

static	int	s_sznChgMark[]	= {1,5000,1,1000,1,1,1,1,1,10,1,1,1,1,1};

UserWealth::UserWealth(UserData* pUser):m_pUser(pUser)
{
	memset(m_nWealths,0,UW_MAX*4);
	memset(m_sznChgVal,0,UW_MAX*4);
	memset(m_bChged,0,UW_MAX);
	m_nGoldChangeVal = 0;
	m_nExpChangeVal = 0;
}

UserWealth::~UserWealth()
{

}

void	UserWealth::SetTotalSend()
{
	for(int i = 0; i < UW_MAX; i ++)
	{
		m_bChged[i] = true;
	}
}

void UserWealth::SendWealthSingle(BYTE bEnum , int nChangeVal,DWORD dwUin)
{
	BuildPacketEx(pPacket, USER, S_WEALTH_SINGLE,buf,256);
	SET_DATA(pSend, USER, S_WEALTH_SINGLE, pPacket);
	pSend->byType		= bEnum;
	pSend->dwVal			= nChangeVal;
	TCP_SendPacketToUin(dwUin,pPacket);
}

bool UserWealth::SubWealth( BYTE bEnum, int nVal ,BYTE byOrigin )
{
	if(nVal < 0)
		return false;
	if(m_nWealths[bEnum] < nVal)
		return false;
	m_nWealths[bEnum]	-= nVal;

	m_sznChgVal[bEnum]	-= nVal;
	m_bChged[bEnum]		= true;
	if(bEnum == UW_EXP)
	{
		m_pUser->SendMsg("失去%d经验",nVal);
		m_nExpChangeVal += -nVal;
	}
	else if(bEnum == UW_GOLD)
	{
		m_pUser->SendMsg("失去%d金币",nVal);
		m_nGoldChangeVal += -nVal;

		if(nVal > 100)
			g_pMarkGameDB->AddMarkCost(2,nVal/100);
	}
	else if(bEnum == UW_HONOUR)
	{
		m_pUser->SendMsg("失去%d点荣誉",nVal);
		g_pGameCenter->m_pRankCenter->CheckRank(RANK_HORNOUR,(void*)m_pUser->m_dwUin,true);
	}
	else if(bEnum == UW_RMB)
	{
		m_pUser->SendMsg("失去%d钻石",nVal);
		m_pUser->m_pUserRecord->AddRmbLog(m_nWealths[UW_RMB],-nVal,byOrigin);

		g_pMarkGameDB->AddMarkCost(0,nVal);
		if(byOrigin != RMB_SAVE_SHOPBUY)
			g_pMarkGameDB->AddMarkCost(1000+byOrigin,1);
	}
	else if(bEnum == UW_BINDRMB)
	{
		m_pUser->SendMsg("失去%d绑钻",nVal);
		m_pUser->m_pUserRecord->AddBindRmbLog(m_nWealths[UW_BINDRMB],-nVal,byOrigin);
		g_pMarkGameDB->AddMarkCost(1,nVal);
		if(byOrigin != RMB_SAVE_SHOPBUY)
			g_pMarkGameDB->AddMarkCost(1000+byOrigin,1);
	}
	return true;
}

bool UserWealth::SubExpWealth( BYTE bEnum, DWORD nVal ,BYTE byOrigin )
{
	if(nVal < 0)
		return false;
	if (bEnum != UW_EXP)
		return false;

	if(GetExpWealthVal() < nVal)
		return false;

	m_sznChgVal[UW_EXP]	-= nVal;
	m_bChged[UW_EXP]		= true;
	m_nExpChangeVal += -nVal;
	SetExpWealthVal(GetExpWealthVal()-nVal);
	m_pUser->SendMsg("失去%u经验",nVal);
	return true;
}

bool UserWealth::SetWealth(BYTE bEnum, int nVal)
{
	if (bEnum == UW_LV)
		SubExpWealth(UW_EXP,m_nWealths[UW_EXP]);
	if (bEnum == UW_VIPLV)
		SubWealth(UW_VIPEXP,m_nWealths[UW_VIPEXP]);

	int nTmpVal = nVal - m_nWealths[bEnum];
	if (nTmpVal > 0)
		AddWealth(bEnum,nTmpVal);
	else
		SubWealth(bEnum,-nTmpVal);

	return true;
}

void UserWealth::SetExpWealthVal(DWORD dwExpVal)
{
	m_nWealths[UW_EXP] = dwExpVal%UW_EXP_VALUE;
	m_nWealths[UW_EXP_CNT] = dwExpVal/UW_EXP_VALUE;
	m_bChged[UW_EXP]		= true;
	m_bChged[UW_EXP_CNT]		= true;
	m_sznChgVal[UW_EXP_CNT]	= 1;

}
DWORD UserWealth::GetExpWealthVal()
{
	return m_nWealths[UW_EXP] + m_nWealths[UW_EXP_CNT]*UW_EXP_VALUE;
}


bool UserWealth::AddWealth( BYTE bEnum, int nVal ,BYTE byOrigin,char* szDesc,BYTE byType)
{
	if(nVal == 0)
		return true;
	if (bEnum == UW_POW)
	{
		if (m_nWealths[bEnum]+nVal>50)
		{
			nVal = 50-m_nWealths[bEnum];
		}
	}
	m_nWealths[bEnum]	+= nVal;

	m_sznChgVal[bEnum]	+= nVal;
	m_bChged[bEnum]		= true;
	if (bEnum == UW_EXP)
	{
		if (m_nWealths[UW_LV]<nUserMaxLv&&m_nWealths[UW_LV]>0)
		{
			SetExpWealthVal(m_nWealths[UW_EXP] + m_nWealths[UW_EXP_CNT]*UW_EXP_VALUE);
			DWORD dwCurExpVal = GetExpWealthVal();
			DWORD nNeedExp = UserConfig::GetInstance().nExp[m_nWealths[UW_LV]-1];
			while (dwCurExpVal>=nNeedExp)
			{
				dwCurExpVal -= nNeedExp;
				AddWealth(UW_LV,1);
				AddWealth(UW_SKILLPT,3);
				if (m_nWealths[UW_LV]>=nUserMaxLv)
				{
					break;
				}
				if (m_pUser->m_pMySimple)
				{
					m_pUser->m_pMySimple->bLv = m_nWealths[UW_LV];
				}
				nNeedExp = UserConfig::GetInstance().nExp[m_nWealths[UW_LV]-1];
				m_nExpChangeVal += 10000;
			}
			SetExpWealthVal(dwCurExpVal);
		}
		m_pUser->SendMsg("获得%d经验",nVal);
		m_nExpChangeVal += nVal;
	}
	else if(bEnum == UW_GOLD)
	{
		m_pUser->SendMsg("获得%d金币",nVal);
		m_nGoldChangeVal += nVal;
		m_pUser->m_pUserRecord->RunLuaWealthFunc(1,nVal,m_nWealths[bEnum]);

		if (nVal >= 50000)
			m_pUser->m_pUserRecord->AddGoldFromLog(nVal,m_nWealths[UW_GOLD],byType);
	}
	else if(bEnum == UW_HONOUR)
	{
		m_pUser->SendMsg("获得%d点荣誉",nVal);
		g_pGameCenter->m_pRankCenter->CheckRank(RANK_HORNOUR,(void*)m_pUser->m_dwUin,true);
		m_pUser->m_pUserRecord->RunLuaWealthFunc(2,nVal,m_nWealths[bEnum]);
	}
	else if(bEnum == UW_RMB)
	{
		m_pUser->SendMsg("获得%d钻石",nVal);
		m_pUser->m_pUserRecord->AddRmbLog(m_nWealths[UW_RMB],nVal,byOrigin,szDesc);
	}
	else if(bEnum == UW_BINDRMB)
	{
		m_pUser->SendMsg("获得%d绑钻",nVal);
		m_pUser->m_pUserRecord->AddBindRmbLog(m_nWealths[UW_BINDRMB],nVal,byOrigin);
	}
	else if(bEnum == UW_STARLV)
	{
		m_pUser->m_pUserRecord->RunLuaStarFunc(m_nWealths[bEnum]);
		m_pUser->RecalcParam();
	}
	else if (bEnum == UW_LIFE_STARLV)
	{
		m_pUser->RecalcParam();
	}
	else if (bEnum == UW_LV)
	{
		m_pUser->m_pMObjCenter->m_pPlayer->byLv = m_nWealths[UW_LV];
		m_pUser->OnLvChange();
		m_pUser->m_pMObjCenter->m_pPlayer->bParamChged = true;
		m_pUser->m_pUserRecord->RunLuaWealthFunc(4,nVal,m_nWealths[bEnum]);
	}
	else if(bEnum == UW_VIPEXP)
	{
		if(m_nWealths[UW_VIPLV] < VipConfig::GetInstance().GetMaxLvl())
		{
			VipInfoObj* pObj = VipConfig::GetInstance().GetObjByLv(m_nWealths[UW_VIPLV]+1);
			while(pObj && m_nWealths[UW_VIPEXP] >= pObj->m_nNeedgold)
			{
				m_nWealths[UW_VIPEXP] -= pObj->m_nNeedgold;
				AddWealth(UW_VIPLV,1);
				if(m_nWealths[UW_VIPLV] >= VipConfig::GetInstance().GetMaxLvl())
					break;
				pObj = VipConfig::GetInstance().GetObjByLv(m_nWealths[UW_VIPLV]+1);
			}
		}
	}
	else if(bEnum == UW_VIPLV)
	{
		m_pUser->m_pUserRecord->RunLuaWealthFunc(3,nVal,m_nWealths[bEnum]);
	}
	return true;
}

bool UserWealth::DBInitWealth( BYTE bEnum, int nVal )
{
	//m_sznChgVal[bEnum]	= nVal-m_nWealths[bEnum];
	m_nWealths[bEnum]	= nVal;

	m_bChged[bEnum]		= true;
	return true;
}

void UserWealth::CheckSaveDB( DBControl* pDB,DWORD dwUin,DWORD dwAcc )
{
	int nTempVal = m_nWealths[UW_LV]*200;
	if(m_nGoldChangeVal >= nTempVal)
	{
		m_nGoldChangeVal = 0;
		m_pUser->m_pUserRecord->AddGoldLog(m_nWealths[UW_GOLD]);
	}
	if(m_nExpChangeVal >= nTempVal)
	{
		m_nExpChangeVal = 0;
		m_pUser->m_pUserRecord->AddExpLog(m_nWealths[UW_EXP],m_nWealths[UW_LV]);
	}

	for (int i = 0; i < UW_MAX; i++)
	{
		if(m_bChged[i])
		{
			SendWealthSingle(i,m_nWealths[i],dwUin);

			m_bChged[i]	= false;
		}
	}



	for (int i = 0; i < UW_MAX; i++)
	{
		if(i == UW_EXP || i == UW_GOLD)
		{//经验值,金钱保存间隔拉长
			if(abs(m_sznChgVal[i]) < nTempVal)
				continue;
		}

		if(abs(m_sznChgVal[i]) >= s_sznChgMark[i])
		{
			switch (i)
			{
			case UW_RMB:
				pDB->ExecuteSQL(FALSE,"update "RPG_ACCOUNT_TABLE" set rmbmoney=%d where accountUin=%d",m_nWealths[i],dwAcc);
				break;
			case UW_BINDRMB:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set rmbbind=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_LV:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set lvl=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_EXP:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set exp=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_EXP_CNT:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set expcnt=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_GOLD:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set gold=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_POW:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set power=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_VIPLV:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set viplvl=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_VIPEXP:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set vipexp=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_SKILLPT:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set skillpt=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_STARLV:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set starlv=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_LIFE_STARLV:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set lifestarlv=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_GSLAVELMT:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set getslavelimit=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_HONOUR:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set honour=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			}
			m_sznChgVal[i]	= 0;
		}
	}
}

void UserWealth::SaveToDB( DBControl* pDB,DWORD dwUin,DWORD dwAcc )
{
	for (int i = 0; i < UW_MAX; i++)
	{
		if(abs(m_sznChgVal[i]))
		{
			switch (i)
			{
			case UW_RMB:
				pDB->ExecuteSQL(FALSE,"update "RPG_ACCOUNT_TABLE" set rmbmoney=%d where accountUin=%d",m_nWealths[i],dwAcc);
				break;
			case UW_BINDRMB:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set rmbbind=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_LV:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set lvl=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_EXP:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set exp=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_EXP_CNT:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set expcnt=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_GOLD:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set gold=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_POW:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set power=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_SKILLPT:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set skillpt=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_STARLV:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set starlv=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_LIFE_STARLV:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set lifestarlv=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_GSLAVELMT:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set getslavelimit=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_VIPLV:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set viplvl=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_VIPEXP:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set vipexp=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			case UW_HONOUR:
				pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set honour=%d where Uin=%d",m_nWealths[i],dwUin);
				break;
			}
			m_sznChgVal[i]	= 0;
		}
	}
}
