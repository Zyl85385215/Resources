#include "stdafx.h"
#include "WarRank.h"
#include "UserData.h"
#include "ios_muli_packet.h"
#include "SkillConfig.h"
#include "FsGameCenter.h"
#include "FortCenter.h"
#include "UserConfig.h"
#include <math.h>
#include "MCopyCenter.h"
#include "CallMapEnemy.h"
#include "MapSObject.h"
#include "ServerCfg.h"
#include "TextConfig.h"
#include "RankCenter.h"
#include "SlaveOpCenter.h"
#include "MailCenter.h"

extern int Random_Int(int min, int max);
static int s_nranklimit[] = {500,400,300,200,100};
static int s_nlvllimit[] = {5,10,15,20,25};

WarRankCenter::WarRankCenter()
{


}

WarRankCenter::~WarRankCenter()
{
	std::map<WORD,WarRankUser*>::iterator itor	= m_mapWarRank.begin();
	if(itor	!= m_mapWarRank.end())
	{
		delete itor->second;
		itor++;
	}
}

void WarRankCenter::DBInit( DBControl* pDB )
{
	QueryRlt xRlt;
 	if(!pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_RANK_WAR_TABLE))
 		return;
 
 	MySqlQuery* pCmd = xRlt.GetRlt();
 	if (pCmd == NULL)
 		return;
 
 	while(pCmd->FetchRow())
 	{
 		WORD wPos	= pCmd->GetWORD("pos");
 
 		m_mapWarRank[wPos]	= new WarRankUser();
 		m_mapWarRank[wPos]->wRankPos	= wPos;
 		m_mapWarRank[wPos]->byLv		= pCmd->GetWORD("lv");
 		strcpy(m_mapWarRank[wPos]->szName,pCmd->GetStr("name"));
		m_mapWarRank[wPos]->dwUin	= pCmd->GetDWORD("uin");
		m_mapWarRank[wPos]->byJob	= pCmd->GetWORD("job");
		m_mapWarRank[wPos]->bySex	= pCmd->GetWORD("sex");
		m_mapWarRank[wPos]->wScore	= pCmd->GetWORD("score");
 		m_mapWarRank[wPos]->nWinCnt	= pCmd->GetDWORD("wincnt");
		m_mapWarRank[wPos]->pFight	= NULL;
 		int nLen = 0;
 		BYTE* pData = pCmd->GetBLOB("param",&nLen);
 		if(nLen&&nLen > sizeof(WarRankParam))
 		{//长度错误,检查原因
 			return;
 		}
 		memcpy(&m_mapWarRank[wPos]->xParam,pData,nLen);
 	}

	CfgInit("test");
}

void WarRankCenter::CfgInit(char*	szFile)
{
	int nSizePlayer = m_mapWarRank.size()+MAX_WARRANK_CNT;
 	for (int i = 0; i < nSizePlayer; i++)
 	{
 		if(m_mapWarRank.find(i)	!= m_mapWarRank.end())
 			continue;	//已经有人了就不用加载电脑了
 
 		WarRankUser* pWRNew	= new WarRankUser();
 		pWRNew->byLv	= 40-i*10/nSizePlayer;
 		pWRNew->dwUin	= 0;
 		pWRNew->wRankPos	= i;
		int nJob = Random_Int(0,7);
 			
 		pWRNew->xParam.nSpeed = 180;
		int nExVal = 200*Random_Int(50,120);
		for (int i=0;i<EQUIP_ATTR_MAX;i++)
		{

			pWRNew->xParam.nParams[i] += (UserConfig::GetInstance().nParam[pWRNew->byLv-1][nJob/2][i]*nExVal)/10000;
		}
		pWRNew->xParam.nParams[EQUIP_ATTR_HP] = pWRNew->xParam.nParams[EQUIP_ATTR_HP]*15/10;
		pWRNew->xParam.nParams[EQUIP_ATTR_ATK_W] = pWRNew->xParam.nParams[EQUIP_ATTR_ATK_W]/2;
		pWRNew->xParam.nParams[EQUIP_ATTR_ATK_N] = pWRNew->xParam.nParams[EQUIP_ATTR_ATK_N]/2;
		pWRNew->xParam.nSkin[0] = 1001+nJob;
		pWRNew->xParam.nSkin[1] = 3001+nJob;
		pWRNew->bySex = nJob%2?0:1;
		strcpy(pWRNew->szName,RandNameConfig::GetInstance().GetRndName(pWRNew->bySex));
		pWRNew->byJob = nJob/2+1;
		if (pWRNew->byJob == 1)
		{
			pWRNew->xParam.wSkillID[0] = 6500;
			pWRNew->xParam.wSkillID[1] = 6501;
			pWRNew->xParam.wSkillID[2] = 6502;
			pWRNew->xParam.wSkillID[3] = 6503;

		}else if (pWRNew->byJob == 2)
		{
			pWRNew->xParam.wSkillID[0] = 6504;
			pWRNew->xParam.wSkillID[1] = 6505;
			pWRNew->xParam.wSkillID[2] = 6506;
			pWRNew->xParam.wSkillID[3] = 6507;
		}else if (pWRNew->byJob == 3)
		{
			pWRNew->xParam.wSkillID[0] = 6508;
			pWRNew->xParam.wSkillID[1] = 6509;
			pWRNew->xParam.wSkillID[2] = 6510;
			pWRNew->xParam.wSkillID[3] = 6511;
		}else if (pWRNew->byJob == 4)
		{
			pWRNew->xParam.wSkillID[0] = 6512;
			pWRNew->xParam.wSkillID[1] = 6513;
			pWRNew->xParam.wSkillID[2] = 6514;
			pWRNew->xParam.wSkillID[3] = 6515;
		}
		pWRNew->xParam.bySkillLv[0] = 1;
		pWRNew->xParam.bySkillLv[1] = 1;
		pWRNew->xParam.bySkillLv[2] = 1;
		pWRNew->xParam.bySkillLv[3] = 1;
		pWRNew->pFight = NULL;
		pWRNew->wScore = UserData::GetScore(pWRNew->xParam.nParams,pWRNew->byJob);
		pWRNew->nWinCnt = 0;
 		m_mapWarRank[i]	= pWRNew;
 	}
}

void WarRankCenter::RequestFight( UserData* pUser, WORD wPos )
{
	if (pUser->m_pMObjCenter->m_pListMark->Count)
	{
		pUser->SendPopMsg("当前不能进入竞技场");
		return;
	}
	FortData* pData = pUser->GetFortData();
	if (!pData)
	{
		return;
	}
 	int nCurTime	= pUser->GetFlag(nWarRankCntFlag);
 	if(nCurTime >= 10)
 		return;

	WarRankUser*	pWRMine		= GetWRUserByUin(pUser->m_dwUin);
	if (pWRMine->wRankPos == wPos)
	{
		pUser->SendPopMsg("无法挑战自己");
		return;
	}

	if (pUser->GetFlag(85) > 29 && pUser->m_bWarnJJCTip)
	{
		pUser->m_bWarnJJCTip = false;
		pUser->SendPopMsg("每日挑战30次后，将无法获得经验。");
		return;
	}
	
	if(pWRMine)
	{
		if(wPos+20 < pWRMine->wRankPos)
			return;	
	}
	else
	{
		if(wPos+20<(MAX_WARRANK_CNT+m_mapWarRank.size()))
			return;
	}
 
 	WarRankUser*	pWRObj	= GetWRUserByPos(wPos);
 	if(!pWRObj)
 		return;
	if (pWRObj->pFight)
	{
		pUser->SendWarnMsg("对方正在被挑战");
		return;
	}
	if (pUser->m_pMapCopy)
	{
		return;
	}
	MCopyBase* pMC	= MCopyConfig::GetInstance().GetMCopy(401);
	if (!pMC)
	{
		return;
	}

	pUser->m_pMapCopy	= g_pGameCenter->m_pMapCopyCenter->RequestEnterMCopy(pUser->m_dwUin,pUser->m_dwUin,pMC);
	if (!pUser->m_pMapCopy)
	{
		return;
	}
	LockFight(pWRObj,pUser);
	SendWarRankState(pUser->m_dwUin,WARRANK_STATE_START,0);
	pUser->m_wOldMIndex = pUser->m_nMIndex;
	pUser->m_wOldPosX = pUser->GetPosX();
	pUser->m_wOldPosY = pUser->GetPosY();
	pUser->JumpTo(pUser->m_pMapCopy->m_dwMapIndex,pUser->m_pMapCopy->m_pMCBase->nStartX,pUser->m_pMapCopy->m_pMCBase->nStartY,pUser->m_pMapCopy->m_pMCBase->wBaseMap);
	pUser->AddState(nJJCStartState,false);
	int wEnemyType = 8001+pWRObj->byJob;
// 	if (pWRObj->byJob == 2||pWRObj->byJob == 4)
// 	{
// 		wEnemyType = 8002;
// 	}else if (pWRObj->byJob == 3)
// 	{
// 		wEnemyType = 8003;
// 	}
	WORD wEnamyID = g_pGameCenter->m_pCallEnemyCenter->CallEnemyByCfg(wEnemyType,pUser->m_pMapCopy->m_dwMapIndex,pUser->m_pMapCopy->m_pMCBase->nStartX-1360,pUser->m_pMapCopy->m_pMCBase->nStartY,11);
	_changeEnamyParam(wEnamyID,pWRObj,pUser);
	_changeEnamySkill(wEnamyID,pWRObj,pUser);
	g_pGameCenter->m_pCallEnemyCenter->SetEnemyAct(wEnamyID,pUser->m_pMapCopy->m_dwMapIndex,9,pUser->m_dwUin,MAPOBJ_PLAYER);

	if (pUser->GetFlag(85) < 35)
		pUser->AddFlag(85,1);

	pUser->AddFlag(nWarRankCntFlag,1);
	pUser->m_pSlaveCenter->CallBackSlave();

}

WarRankUser* WarRankCenter::GetWRUserByPos( WORD wPos )
{
	std::map<WORD,WarRankUser*>::iterator	itor	= m_mapWarRank.find(wPos);
	if(itor != m_mapWarRank.end())
		return itor->second;

	return NULL;
}

WarRankUser* WarRankCenter::GetWRUserByUin( DWORD dwUin )
{
	std::map<WORD,WarRankUser*>::iterator	itor	= m_mapWarRank.begin();

	while(itor	!= m_mapWarRank.end())
	{
		if(itor->second->dwUin	== dwUin)
			return itor->second;
		
		itor++;
	}

	return NULL;
}

void WarRankCenter::RequestWRObjs( UserData* pUser )
{
 	WarRankUser*	pWRUser		= GetWRUserByUin(pUser->m_dwUin);
 	int nStartRank	= m_mapWarRank.size() - 1;
 	if(pWRUser)
	{
		FortData* pData = pUser->GetFortData();
		if (pData&&pData->m_pWarRankData&&pData->m_pWarRankData->nRankPos == -1)
			pData->SetWarRankPos(pWRUser->wRankPos);

 		nStartRank	= (int)pWRUser->wRankPos-1;
	}
	else{
		AddNewRankUser(pUser);
		pUser->GetFortData()->SendWarRankData(pUser->m_dwUin);
	}
 	if(nStartRank	< 5)	//第一名无挑战对象
	{
		nStartRank = 5;
 		//return;
	}


 	BuildPacketEx(pSendPacket, FRIEND, S_WARRANK_INFO, buf, 2048);
 	SET_DATA(pData, FRIEND, S_WARRANK_INFO, pSendPacket);
 	pData->bCnt	= 0;
	if (nStartRank<15)
	{
		for (int i = nStartRank; i >=0 && i > nStartRank-6; i--)
		{
			WarRankUser*	pWRU	= GetWRUserByPos(i);
			if(pWRU)
			{
				pData->szWRObj[pData->bCnt].byLv	= pWRU->byLv;
				pData->szWRObj[pData->bCnt].byJob	= pWRU->byJob;
				pData->szWRObj[pData->bCnt].bySex	= pWRU->bySex;
				pData->szWRObj[pData->bCnt].wPos	= pWRU->wRankPos;
				pData->szWRObj[pData->bCnt].dwScore	= pWRU->wScore;
				strcpy(pData->szWRObj[pData->bCnt].szName,pWRU->szName);
				if(ServerCfg::GetInstance().nVersionDef)
				{
					if(pWRU->xParam.nSkin[0] >= 1021 && pWRU->xParam.nSkin[0] <= 1028)
						pWRU->xParam.nSkin[0] -= 20;
				}
				if(pWRU->xParam.nSkin[0] >= 1011 && pWRU->xParam.nSkin[0] <= 1018)
					pWRU->xParam.nSkin[0] -= 10;

				pData->szWRObj[pData->bCnt].wSkin[0]	= pWRU->xParam.nSkin[0];

				if(ServerCfg::GetInstance().nVersionDef)
				{
					if(pWRU->xParam.nSkin[1] >= 3011 && pWRU->xParam.nSkin[1] <= 3028)
						pWRU->xParam.nSkin[1] = 3000+pWRU->xParam.nSkin[1]%10;
				}
				pData->szWRObj[pData->bCnt].wSkin[1]	= pWRU->xParam.nSkin[1];



				pData->bCnt++;
			}
		}
	}else
	{
		int nRandMax = nStartRank;
		int nRandMin = (nStartRank-15>0)?(nStartRank-15):0;
		int	nReTryTime = 20;
		while(pData->bCnt < 6 && nReTryTime)
		{
			nReTryTime--;
			int nRandPos = Random_Int(nRandMin,nRandMax);
			WarRankUser*	pWRU	= GetWRUserByPos(nRandPos);
			if(pWRU)
			{
				bool bAgain = false;
				for (int j = 0; j< pData->bCnt; j++)
				{
					if (pData->szWRObj[j].wPos == pWRU->wRankPos)
						bAgain = true;
				}
				if (bAgain)
					continue;

				pData->szWRObj[pData->bCnt].byLv	= pWRU->byLv;
				pData->szWRObj[pData->bCnt].byJob	= pWRU->byJob;
				pData->szWRObj[pData->bCnt].bySex	= pWRU->bySex;
				pData->szWRObj[pData->bCnt].wPos	= pWRU->wRankPos;
				pData->szWRObj[pData->bCnt].dwScore	= pWRU->wScore;
				strcpy(pData->szWRObj[pData->bCnt].szName,pWRU->szName);
				if(ServerCfg::GetInstance().nVersionDef)
				{
					if(pWRU->xParam.nSkin[0] >= 1021 && pWRU->xParam.nSkin[0] <= 1028)
						pWRU->xParam.nSkin[0] -= 20;
				}
				if(pWRU->xParam.nSkin[0] >= 1011 && pWRU->xParam.nSkin[0] <= 1018)
					pWRU->xParam.nSkin[0] -= 10;
				pData->szWRObj[pData->bCnt].wSkin[0]	= pWRU->xParam.nSkin[0];

				if(ServerCfg::GetInstance().nVersionDef)
				{
					if(pWRU->xParam.nSkin[1] >= 3011 && pWRU->xParam.nSkin[1] <= 3028)
						pWRU->xParam.nSkin[1] = 3000+pWRU->xParam.nSkin[1]%10;
				}
				pData->szWRObj[pData->bCnt].wSkin[1]	= pWRU->xParam.nSkin[1];

				pData->bCnt++;
			}
		}

	}
 	
 	pSendPacket->wSize	+= sizeof(WRankSimpleObj)*pData->bCnt;
 
 	TCP_SendPacketToUin(pUser->m_dwUin,pSendPacket);

	int nTenCnt = 10;
	if(nTenCnt > m_mapWarRank.size())
		nTenCnt = m_mapWarRank.size();
	BuildPacketEx(pSendPacket2, FRIEND, S_WARRANK_TEN_INFO, buf2, 2048);
	SET_DATA(pData2, FRIEND, S_WARRANK_TEN_INFO, pSendPacket2);
	pData2->bCnt	= 0;
	for (int i = 0; i < nTenCnt ; i++)
	{
		WarRankUser*	pWRU	= GetWRUserByPos(i);
		if(pWRU)
		{
			pData2->szWRObj[pData2->bCnt].byLv	= pWRU->byLv;
			pData2->szWRObj[pData2->bCnt].byJob	= pWRU->byJob;
			pData2->szWRObj[pData2->bCnt].bySex	= pWRU->bySex;
			pData2->szWRObj[pData2->bCnt].wPos	= pWRU->wRankPos;
			pData2->szWRObj[pData2->bCnt].dwScore	= pWRU->wScore;
			strcpy(pData2->szWRObj[pData2->bCnt].szName,pWRU->szName);
			pData2->szWRObj[pData2->bCnt].wSkin[0]	= pWRU->xParam.nSkin[0];
			pData2->szWRObj[pData2->bCnt].wSkin[1]	= pWRU->xParam.nSkin[1];
			pData2->szWRObj[pData2->bCnt].dwUin	= pWRU->dwUin;
			pData2->bCnt++;
		}
	}
	pSendPacket2->wSize	+= sizeof(WRankSimpleObj)*pData2->bCnt;

	TCP_SendPacketToUin(pUser->m_dwUin,pSendPacket2);
}

void WarRankCenter::Update10Mintue()
{
// 	int nTotalSize = MAX_WARRANK_CNT+m_mapWarRank.size();
// 	for (int i = 0; i < nTotalSize; i++)
// 	{
// 		WarRankUser* pWRUser	= GetWRUserByPos(i);
// 		if(pWRUser && pWRUser->dwUin)
// 		{//多线程访问
// 			int nPos = i>500?500:i;
// 			int nScore	= (600-nPos) * (650-nPos)/5000;
// 			pWRUser->_addUserScore(nScore);
// // 			UserData* pUser	= g_pGameCenter->GetPlayer(pWRUser->dwUin);
// //  			if(pUser)
// //  				pUser->m_xUWealth.AddWealth(UW_WRANK, nScore);
// //  			else
// //  			{//不在线
// //  				pWRUser->_addUserScore(nScore);
// //  			}
// 		}
// 	}
}

static int s_nAwardRankLimit[] = {1,10,50,100,200,300,400,500};
static int s_nAwardNumLimit[] = {1000,800,600,400,300,200,100,100};

void WarRankCenter::UpdateRank(UserData* pUser)
{
// 	WarRankUser* pRank = GetWRUserByUin(pUser->m_dwUin);
// 	if(pRank)
// 	{
// 		for(int i = 0; i < 8 ; i ++)
// 		{
// 			if(pUser->m_xUWealth.GetWealth(UW_AWARDLVL) < (8-i) && pRank->wRankPos < s_nAwardRankLimit[i])
// 			{
// 				pUser->m_xUWealth.SetWealth(UW_AWARDLVL,8-i);
// 				break;
// 			}
// 		}
// 	}
}

void WarRankCenter::RequestForAward(UserData* pUser,BYTE byPos)
{
// 	if(pUser == NULL || byPos > pUser->m_xUWealth.GetWealth(UW_AWARDLVL))
// 		return;
// 		
// 	//int nTemp = pow(10.0,byPos-1);
// 	int nTemp = 1;
// 	for(int i = 0; i < byPos; i ++)
// 		nTemp *= 10;
// 	int nRlt = pUser->m_xUWealth.GetWealth(UW_AWARDRLT)/nTemp;
// 	nRlt = nRlt % 10;
// 	if(nRlt == 0)
// 	{
// 		pUser->AddItemByID(2003,s_nAwardNumLimit[8-byPos]);
// 		pUser->m_xUWealth.AddWealth(UW_AWARDRLT,nTemp);
// 	}
}

void WarRankCenter::RequestBuyItem(UserData* pUser,BYTE byType)
{
// 	if(pUser == NULL || byType > 3)
// 		return;
// 	int nCost[] = {1000,10000,100000};
// 	int nNum[] = {5,50,500};
// 	int nScore = pUser->m_xUWealth.GetWealth(UW_WRANK);
// 	if(nScore < nCost[byType-1])
// 		return;
// 	pUser->AddItemByID(2003,nNum[byType-1],true);
// 	pUser->m_xUWealth.SubWealth(UW_WRANK,nCost[byType-1]);
}

void WarRankCenter::_changeEnamyParam( WORD wID,WarRankUser* pData ,UserData* pUser)
{
	BuildPacketEx(pPacket, MAPOBJ,GS_SYNCPARAM,buf,2048);
	SET_DATA(pSend, MAPOBJ,GS_SYNCPARAM, pPacket);
	pSend->byType		= MAPOBJ_ENEMY;
	pSend->dwID			= wID;
	pSend->byLv			= pData->byLv;
	pSend->xParam.nHp = pData->xParam.nParams[EQUIP_ATTR_HP];
	pSend->xParam.nHpMax = pData->xParam.nParams[EQUIP_ATTR_HP];
	pSend->xParam.nMp = pData->xParam.nParams[EQUIP_ATTR_MP];
	pSend->xParam.nMpMax = pData->xParam.nParams[EQUIP_ATTR_MP];
	pSend->xParam.nAtkW = pData->xParam.nParams[EQUIP_ATTR_ATK_W];
	pSend->xParam.nAtkN = pData->xParam.nParams[EQUIP_ATTR_ATK_N];
	pSend->xParam.nDefW = pData->xParam.nParams[EQUIP_ATTR_DEF_W];
	pSend->xParam.nDefN = pData->xParam.nParams[EQUIP_ATTR_DEF_N];
	pSend->xParam.nAim = pData->xParam.nParams[EQUIP_ATTR_HIT];
	pSend->xParam.nDodge = pData->xParam.nParams[EQUIP_ATTR_MISS];
	pSend->xParam.nCrit = pData->xParam.nParams[EQUIP_ATTR_CRIT];
	pSend->xParam.nMSpeed = pData->xParam.nSpeed;
	pSend->xParam.nPvpDef	= (95+pData->byLv*5)/2;
	pSend->wSkin[0] = pData->xParam.nSkin[0];
	pSend->wSkin[1] = (pData->xParam.nSkin[0]>=1500&&pData->xParam.nSkin[0]<2000)?0:pData->xParam.nSkin[1];
	pSend->wSkin[2] = 0;
	g_pNetCenter->SendMSvrPacket(pPacket,pUser->m_nMIndex,pUser->m_pMObjCenter->GetMapSrvObj()->pMapSvr);

	BuildPacketEx(pPacket1, MAPOBJ,GS_ADDSTATE,buf1,1024);
	SET_DATA(pSend1, MAPOBJ,GS_ADDSTATE, pPacket1);
	pSend1->byType		= MAPOBJ_ENEMY;
	pSend1->dwID			= wID;
	pSend1->wState		= nJJCStartState;
	pSend1->bRemove		= false;
	g_pNetCenter->SendMSvrPacket(pPacket1,pUser->m_nMIndex,pUser->m_pMObjCenter->GetMapSrvObj()->pMapSvr);
}

void WarRankCenter::_changeEnamySkill( WORD wID,WarRankUser* pData,UserData* pUser )
{
	BuildPacketEx(pPacket, MAPOBJ,GS_SYNCSKILL,buf,2048);
	SET_DATA(pSend, MAPOBJ,GS_SYNCSKILL, pPacket);
	pSend->byType		= MAPOBJ_ENEMY;
	pSend->dwID			= wID;
	
	DWORD	dwTick		= CMTime::GetCurTickCount();
	int nCnt = 0;
	for (int i = 0; i < 12; i++)
	{
		if (pData->xParam.wSkillID[i])
		{
			pSend->szSkill[nCnt].bPos = 0;
			pSend->szSkill[nCnt].byLv = pData->xParam.bySkillLv[i];
			pSend->szSkill[nCnt].dwCDTick = 0;
			pSend->szSkill[nCnt].wSkill = pData->xParam.wSkillID[i];
			nCnt++;
		}
	}
	pSend->bySCnt		= nCnt;
	pPacket->wSize		+= (pSend->bySCnt-1)*sizeof(_NetSkillInfo);
	g_pNetCenter->SendMSvrPacket(pPacket,pUser->m_nMIndex,pUser->m_pMObjCenter->GetMapSrvObj()->pMapSvr);
}

void WarRankCenter::GMChgWarRankPos(UserData* pUser,int nPos)
{
	if (nPos < 0)
		return;

	WarRankUser*	pWRObj	= GetWRUserByPos(nPos);
	if(!pWRObj)
		return;

	WarRankUser* pWRMine = GetWRUserByUin(pUser->m_dwUin);
	if (!pWRMine && !pUser->GetFortData())
			return;

	if (pWRObj->wRankPos == pWRMine->wRankPos)
		return;

	pWRObj->wRankPos	= pWRMine->wRankPos;
	m_mapWarRank[pWRMine->wRankPos]	= pWRObj;
	pWRObj->ClearWinCnt(pWRMine->szName);
	pWRObj->_SaveDBInfo(false);

	if(pWRObj->dwUin)
	{
		char szMsg[128];
		sprintf(szMsg,"你被%s击败了，排名降至%d",pWRMine->szName,pWRMine->wRankPos+1);
		g_pGameCenter->m_pMailCenter->RequestSendMail(0,pWRObj->dwUin,"竞技场排名",szMsg);

		UserData* pUserTag = g_pGameCenter->GetPlayer(pWRObj->dwUin);
		if (pUserTag)
		{
			pUserTag->GetFortData()->SetWarRankPos(pWRMine->wRankPos);
		}
	}

	m_mapWarRank[nPos]	= pWRMine;
	pWRMine->wRankPos	= nPos;
	pWRMine->_SaveDBInfo(false);

	if (pUser->m_nMaxRankPos-1>nPos)
	{
		pUser->SetMaxRankPos(nPos+1);
	}

	pUser->GetFortData()->SetWarRankPos(nPos);
	SendWarRankState(pUser->m_dwUin,WARRANK_STATE_WIN,nPos);
}

void WarRankCenter::OnBattleResult( UserData* pUser,bool bWin )
{
	if (pUser)
	{
		int nUSLv = pUser->m_xUWealth.GetWealth(UW_LV);
		if (pUser->GetFlag(85) < 31)
			pUser->m_xUWealth.AddWealth(UW_EXP,nUSLv*nUSLv*(bWin?30:15));
	}

	if (bWin)
	{

		WarRankUser* pWRObj = pUser->m_pWarRankFight;
		if (!pWRObj)
		{
			return;
		}

		bool bFightAI = !pWRObj->dwUin;
		//胜利,替换排名
		WarRankUser* pWRMine = GetWRUserByUin(pUser->m_dwUin);
		if (!pWRMine)
		{
			UnlockFight(pUser->m_pWarRankFight);
			pUser->SendPopMsg("该玩家无法挑战竞技场");
			return;
		}
		int nPos = 0;
		nPos = pWRObj->wRankPos;
		if (nPos<10)
		{
			g_pGameCenter->m_pRankCenter->AddUser(pUser);
		}
		if (pWRMine->wRankPos > nPos)
		{
			SendWarRankState(pUser->m_dwUin,WARRANK_STATE_WIN,pWRObj->wRankPos);

			if (pWRObj->dwUin)
			{
				char szMsg[128];
				sprintf(szMsg,"你被%s击败了，排名降至%d",pWRMine->szName,pWRMine->wRankPos+1);
				g_pGameCenter->m_pMailCenter->RequestSendMail(0,pWRObj->dwUin,"竞技场排名",szMsg);

				UserData* pUserTag = g_pGameCenter->GetPlayer(pWRObj->dwUin);
				if (pUserTag)
				{
					pUserTag->GetFortData()->SetWarRankPos(pWRMine->wRankPos);
				}
			}

			if(pWRMine)
			{
				pWRObj->wRankPos	= pWRMine->wRankPos;
				m_mapWarRank[pWRMine->wRankPos]	= pWRObj;
				pWRObj->ClearWinCnt(pWRMine->szName);
				pWRObj->_SaveDBInfo(false);
			}
			m_mapWarRank[nPos]	= pWRMine;
			pWRMine->wRankPos	= nPos;

			if (pUser->m_nMaxRankPos-1>nPos)
			{
				pUser->SetMaxRankPos(nPos+1);
			}
		}else
		{
			SendWarRankState(pUser->m_dwUin,WARRANK_STATE_WIN,pWRMine->wRankPos);
			if(pWRMine)
			{
				pWRObj->ClearWinCnt(pWRMine->szName);
				pWRObj->_SaveDBInfo(false);
			}
		}

		 pWRMine->byLv	= pUser->m_xUWealth.GetWealth(UW_LV);
		 pWRMine->byJob = pUser->m_byJob;
		 pWRMine->bySex = pUser->m_bySex;
		 pWRMine->pFight = NULL;
		 pWRMine->wScore = pUser->GetScore();
		 for (int i=0;i<EQUIP_ATTR_MAX;i++)
		 {
			 pWRMine->xParam.nParams[i] = pUser->m_Param.nParam[i];
		 }
		 pWRMine->xParam.nSpeed = pUser->m_Param.nSpeed;
		 int nSkinTemp = pUser->m_byJob*2+(pUser->m_bySex?0:1);

		 pWRMine->xParam.nSkin[0] = pUser->m_Param.nSkin?pUser->m_Param.nSkin:(999+nSkinTemp);
		 pWRMine->xParam.nSkin[1] = pUser->m_Param.nWeapon?pUser->m_Param.nWeapon:(2999+nSkinTemp);
		 memset(pWRMine->xParam.wSkillID,0,sizeof(WORD)*12);
		 memset(pWRMine->xParam.bySkillLv,0,sizeof(BYTE)*12);
		 if (pUser->m_pMObjCenter->m_pPlayer)
		 {
			 int nCnt = 0;
			 for (int i=0;i<pUser->m_pMObjCenter->m_pPlayer->pListSkill->Count&&i<12;i++)
			 {
				 _NetSkillInfo* pInfo = (_NetSkillInfo*)pUser->m_pMObjCenter->m_pPlayer->pListSkill->Items[i];
				 if (pInfo->wSkill/1000 == 7)
				 {
					 continue;
				 }
				 pWRMine->xParam.wSkillID[nCnt] = pInfo->wSkill;
				 pWRMine->xParam.bySkillLv[nCnt] = pInfo->byLv;
				 nCnt++;
			 }
		 }
		 pWRMine->AddWinCnt();
		 pUser->m_pUserRecord->RunLuaPVPFunc(pWRMine->nWinCnt);
		 pWRMine->_SaveDBInfo(bFightAI);
		 UnlockFight(pUser->m_pWarRankFight);
		 pUser->m_pWarRankFight = NULL;
		 if(nPos)
			 pUser->GetFortData()->SetWarRankPos(nPos);
		
	}else{
		SendWarRankState(pUser->m_dwUin,WARRANK_STATE_FAIL,0);
		WarRankUser* pWRObj = pUser->m_pWarRankFight;
		if (!pWRObj)
		{
			return;
		}
		WarRankUser* pWRMine = GetWRUserByUin(pUser->m_dwUin);

		if (pWRMine)
		{
			pWRMine->ClearWinCnt(pWRObj->szName);
			pWRMine->_SaveDBInfo(false);
		}
// 		pUser->m_pMObjCenter->HealthHpMp(pUser->m_pMObjCenter->m_pPlayer,pUser->m_pMObjCenter->m_pPlayer->xParam.nHpMax,pUser->m_pMObjCenter->m_pPlayer->xParam.nMpMax,true);
// 		pUser->JumpTo(pUser->m_wOldMIndex,pUser->m_wOldPosX,pUser->m_wOldPosY);
		UnlockFight(pUser->m_pWarRankFight);
	}
	
}

void WarRankCenter::LockFight( WarRankUser* pData,UserData* pUser )
{
	pData->pFight = pUser;
	pUser->m_pWarRankFight = pData;
}

void WarRankCenter::UnlockFight( WarRankUser* pData )
{
	if (!pData)
	{
		return;
	}
	if(pData->pFight) pData->pFight->m_pWarRankFight = NULL;
	pData->pFight = NULL;
}

void WarRankCenter::SendWarRankState( DWORD dwUin,BYTE byState,int nVal )
{
	BuildPacketEx(pSendPacket, FRIEND, C_SEND_WARRANK_STATE, buf, 2048);
	SET_DATA(pData, FRIEND, C_SEND_WARRANK_STATE, pSendPacket);
	pData->byState	= byState;
	pData->nVal = nVal;
	TCP_SendPacketToUin(dwUin,pSendPacket);
}

void WarRankCenter::AddWarRankTimes(UserData* pUser)
{
	if (pUser->GetFlag(31) > pUser->m_pVIPCenter->GetVipLv()-1)
		return;
	if (pUser->GetFlag(19) < 1)
		return;
	if(pUser->m_xUWealth.SubWealth(UW_RMB,20,RMB_SAVE_WAR_RANK)== false)
		return;

	pUser->SetFlag(19,pUser->GetFlag(19)-1);
	pUser->SetFlag(31,pUser->GetFlag(31)+1);

	
}

bool WarRankCenter::AddWarRankTimesByUse(UserData* pUser)
{
	if (pUser->GetFlag(19) < 1)
	{
		pUser->SendWarnMsg("可挑战次数已满，先去挑战吧");
		return false;
	}

	pUser->SetFlag(19,pUser->GetFlag(19)-1);
	pUser->SendMsg("挑战次数+1");
	return true;
}

void WarRankCenter::AddNewRankUser( UserData* pUser )
{
	int nPos = m_mapWarRank.size();
	WarRankUser* pWRMine	= new WarRankUser();
	
	m_mapWarRank[nPos]	= pWRMine;
	pWRMine->dwUin = pUser->m_dwUin;
	pWRMine->wRankPos	= nPos;

	pWRMine->byLv	= pUser->m_xUWealth.GetWealth(UW_LV);
	pWRMine->byJob = pUser->m_byJob;
	pWRMine->bySex = pUser->m_bySex;
	pWRMine->pFight = NULL;
	int nSkinTemp = pUser->m_byJob*2+(pUser->m_bySex?0:1);
	strcpy(pWRMine->szName,pUser->m_szNick);
	pWRMine->wScore = pUser->GetScore();
	pWRMine->nWinCnt = 0;
	for (int i=0;i<EQUIP_ATTR_MAX;i++)
	{
		pWRMine->xParam.nParams[i] = pUser->m_Param.nParam[i];
	}
	pWRMine->xParam.nSpeed = pUser->m_Param.nSpeed;
	pWRMine->xParam.nSkin[0] = pUser->m_Param.nSkin?pUser->m_Param.nSkin:(2999+nSkinTemp);
	pWRMine->xParam.nSkin[1] = pUser->m_Param.nWeapon?pUser->m_Param.nWeapon:(999+nSkinTemp);
	if (pUser->m_pMObjCenter->m_pPlayer)
	{
		for (int i=0;i<pUser->m_pMObjCenter->m_pPlayer->pListSkill->Count&&i<12;i++)
		{
			_NetSkillInfo* pInfo = (_NetSkillInfo*)pUser->m_pMObjCenter->m_pPlayer->pListSkill->Items[i];
			pWRMine->xParam.wSkillID[i] = pInfo->wSkill;
			pWRMine->xParam.bySkillLv[i] = pInfo->byLv;
		}
	}
	pWRMine->_SaveDBInfo(true);
	pUser->m_pWarRankFight = NULL;
	pUser->GetFortData()->SetWarRankPos(nPos);
}

void WarRankUser:: _SaveDBInfo(bool bNew)
{
	if(dwUin == 0)
	{
		g_pDBase->ExecuteSQL(FALSE, "delete from "RPG_RANK_WAR_TABLE" where pos=%d",wRankPos);
	}
	else
	{
		if(bNew)
		{
			g_pDBase->ExecuteSQL(FALSE,"INSERT INTO "RPG_RANK_WAR_TABLE" (pos,uin,name,lv,param,job,sex,score,wincnt) \
																	  VALUES(%d,%d,'%s',%d,'',%d,%d,%d,%d)",wRankPos,dwUin,szName,byLv,byJob,bySex,wScore,nWinCnt);
		}
		else
			g_pDBase->ExecuteSQL(FALSE, "update "RPG_RANK_WAR_TABLE" set uin=%d,name='%s',lv=%d,job=%d,sex=%d,score=%d,wincnt=%d where pos=%d",dwUin,szName,byLv,byJob,bySex,wScore,nWinCnt,wRankPos);
		g_pDBase->SetBLOB(FALSE,&xParam,sizeof(WarRankParam), "UPDATE "RPG_RANK_WAR_TABLE" set param=? where pos=%d",wRankPos);

	}
}

void WarRankUser::_addUserScore( int nScore )
{
  	if(dwUin){
		FortData* pData = g_pGameCenter->m_pFortCenter->GetFortData(dwUin);
		if (pData)
		{
			pData->AddWarScore(nScore);
		}
	}
}

void WarRankUser::AddWinCnt()
{
	nWinCnt++;
	char sztxt[128];
	if (nWinCnt == 3)
	{
		sprintf(sztxt,"%s在竞技场中连胜3场，愈战愈勇！",szName);
		ChatCenter::GetInstance().SendSystemMsg(sztxt,0,0,1,true);
	}else if (nWinCnt == 6)
	{
		sprintf(sztxt,"%s在竞技场中连胜6场，势不可挡！",szName);
		ChatCenter::GetInstance().SendSystemMsg(sztxt,0,0,1,true);
	}else if (nWinCnt == 10)
	{
		sprintf(sztxt,"%s在竞技场中连胜10场，没有人能阻止他了！",szName);
		ChatCenter::GetInstance().SendSystemMsg(sztxt,0,0,1,true);
	}else if (nWinCnt>=15&&nWinCnt%5 == 0)
	{
		sprintf(sztxt,"%s在竞技场中连胜%d场，没有人能阻止他了！",szName,nWinCnt);
		ChatCenter::GetInstance().SendSystemMsg(sztxt,0,0,1,true);
	}
	
}

void WarRankUser::ClearWinCnt(char* szWinName)
{
	if (nWinCnt>=10)
	{
		char sztxt[128];
		sprintf(sztxt,"%s在竞技场中被%s击败，%d连胜总算被终结了！",szName,szWinName,nWinCnt);
		ChatCenter::GetInstance().SendSystemMsg(sztxt,0,0,1,true);
	}
	nWinCnt = 0;
	
}

WarRankParam::WarRankParam()
{
	memset(this,0,sizeof(WarRankParam));
}
