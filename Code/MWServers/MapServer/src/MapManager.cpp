#include "stdafx.h"

#include "MapManager.h"
#include "MapPlate.h"
#include "EnemyConfig.h"
#include "ZNetServer.h"
#include "ControlObject.h"
#include "SkillCenter.h"
#include "ios_mapcmd_packet.h"
#include "ControlCenter.h"
#include "StateCenter.h"
#include "BornCenter.h"
#include "RobotConfig.h"
#include "ServerCenter.h"
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
extern int CalcDistance(int nX1,int nY1,int nX2,int nY2);

MapManager::MapManager(WORD wMI, MapCfgObj* pMCfg)
{
	m_dwMapIndex		= wMI;
	m_pMCfgObj		= pMCfg;			//障碍及地图大小
	m_byForceType	= pMCfg->m_byFType;
	m_szMapPlates	= NULL;

	m_pBornCenter	= new BornCenter(m_pMCfgObj->m_wMapIndex,this);
	m_pRbActCenter	= new MRActionCenter(m_pMCfgObj->m_wMapIndex);
	if(m_pRbActCenter->m_pListActPts->Count == 0)
	{
		delete m_pRbActCenter;
		m_pRbActCenter	= NULL;
	}
	m_pAICenter		= new MapAICenter(this);
	
	m_nActiveCtrl	= 0;
	m_nActiveMUser	= 0;
	m_dwEnemyIndex	= 100000000;
	m_nActiveCnt	= 0;
	m_nUserCnt		= 0;
	m_wExPow		= 100;
	m_byChgLv		= 0;
	m_dwUpdateSec	= 0;
	m_byWeather		= 0;
	m_bLockAtk		= false;

	m_dwTempWeather = 0;
	m_dwWeatherSec = 0;
}

MapManager::~MapManager()
{
	g_pCtrlList->CheckCtrlRealseMap(this);

	if(m_szMapPlates)
		delete	[] m_szMapPlates;
	delete m_pBornCenter;
	delete m_pAICenter;
	if(m_pRbActCenter)
		delete m_pRbActCenter;
}

void MapManager::Init()
{
	m_wMaxPX	= (m_pMCfgObj->m_wMapSizeW+MAPPLATE_SIZE-1)/MAPPLATE_SIZE;
	m_wMaxPY	= (m_pMCfgObj->m_wMapSizeH+MAPPLATE_SIZE-1)/MAPPLATE_SIZE;

	m_szMapPlates	= new MapPlate[m_wMaxPX*m_wMaxPY];
	for (int i = 0; i < m_wMaxPY; i++)
	{
		for (int j = 0; j < m_wMaxPX; j++)
		{
			MapPlate* pMPlate	= &m_szMapPlates[i*m_wMaxPX+j];
			pMPlate->m_pMap	= this;
			pMPlate->m_wMX	= j;
			pMPlate->m_wMY	= i;
		}
	}

	if(m_pRbActCenter)
	{//机器人
		static	DWORD	s_dwRobotUin	= 100000000;
		for (int i = 0; i < m_pRbActCenter->m_pListActPts->Count; i++)
		{
			MRActionPt*	pActPt	= (MRActionPt*)m_pRbActCenter->m_pListActPts->Items[i];
			for (int	j	= 0; j < pActPt->pLRobot->Count; j++)
			{
				_MuliParamObj*	pMP	= (_MuliParamObj*)pActPt->pLRobot->Items[j];
				RobotObj* pRobot	= (RobotObj*)pMP->nParam2;
				for (int n = 0; n < pMP->nParam1; n++)
				{
					MapAIObj* pObj	= new MapAIObj();

					pObj->byType	= MAPOBJ_ROBOT;
					pObj->dwUin		= s_dwRobotUin++;
					pObj->nX		= pActPt->nX-30+rand()%60;
					pObj->nY		= pActPt->nY-30+rand()%60;

					if(pObj->nX <= 0)
						pObj->nX	= 1;
					if(pObj->nY <= 0)
						pObj->nY	= 1;

					if(pObj->nX >= m_pMCfgObj->m_wMapSizeW)
						pObj->nX	= m_pMCfgObj->m_wMapSizeW - 1;
					if(pObj->nY >= m_pMCfgObj->m_wMapSizeH)
						pObj->nY	= m_pMCfgObj->m_wMapSizeH - 1;

					pObj->bLv		= pRobot->bLv;
					pObj->byJob		= pRobot->bJob;
					pObj->bySex		= pRobot->bSex;
					memcpy(pObj->wSkin,pRobot->wSkin,6);

					strcpy(pObj->szName,"");
					pObj->szTitle[0]	= rand();
					pObj->szTitle[1]	= rand();
					pObj->szTitle[2]	= rand();
					pObj->szTitle[3]	= 0;
					memcpy(&pObj->xParam,&pRobot->xParam,sizeof(MParam));
					pObj->byCountry	= pActPt->bForce;
					pObj->wHomeID	= 0;
					pObj->byViplv	= 0;
					pObj->pRunActPt	= pActPt;
					pObj->dwTickRunAct	= GetTickCount()+rand()%3000;
					pObj->pSkillCenter->ResetJobSkill();
					if(rand()%100 < 20)
						pObj->byViplv	= rand()%3+1;
					else
						pObj->byViplv	= 0;
					for (int s = 0; s < pRobot->pLSkill->Count; s++)
					{
						pObj->pSkillCenter->SetSkill((WORD)pRobot->pLSkill->Items[s],1,0,0);
					}
					CalcObjForce(pObj);
					AIObjEnterMap(pObj);
					m_pBornCenter->GSBornMapObj(pObj);
				}
			}
		}	
	}
}

bool MapManager::Update()
{//地图释放才返回false
	MapPlate* pMp	= m_szMapPlates;
	for (int ny = 0; ny < m_wMaxPY; ny++)
	{
		for (int nx = 0; nx < m_wMaxPX; nx++)
		{
			//pMp->Update();
			//m_szMapPlates[i*m_wMaxPX+j].Update();
			for (int i = 0; i < pMp->m_pListPlayer->Count; i++)
			{
				MapAIObj* pObj	= (MapAIObj*)pMp->m_pListPlayer->Items[i];
				pObj->RunCmd();
				pObj->Update();
				WORD wMPX	= pObj->nX/MAPPLATE_SIZE;
				WORD wMPY	= pObj->nY/MAPPLATE_SIZE;
				if(wMPX != pObj->wMPX || wMPY != pObj->wMPY)
				{
					m_szMapPlates[pObj->wMPY*m_wMaxPX+pObj->wMPX].LeavePlate(pObj);
					m_szMapPlates[wMPY*m_wMaxPX+wMPX].EnterPlate(pObj);
				}
			}

			for (int i = 0; i < pMp->m_pListEnemy->Count; i++)
			{
				MapAIObj* pObj	= (MapAIObj*)pMp->m_pListEnemy->Items[i];
				pObj->RunCmd();
				pObj->Update();
				WORD wMPX	= pObj->nX/MAPPLATE_SIZE;
				WORD wMPY	= pObj->nY/MAPPLATE_SIZE;
				if(wMPX != pObj->wMPX || wMPY != pObj->wMPY)
				{
					m_szMapPlates[pObj->wMPY*m_wMaxPX+pObj->wMPX].LeavePlate(pObj);
					m_szMapPlates[wMPY*m_wMaxPX+wMPX].EnterPlate(pObj);
				}
			}

			for (int i = 0; i < pMp->m_pListSlave->Count; i++)
			{
				MapAIObj* pObj	= (MapAIObj*)pMp->m_pListSlave->Items[i];
				pObj->RunCmd();
				pObj->Update();
				WORD wMPX	= pObj->nX/MAPPLATE_SIZE;
				WORD wMPY	= pObj->nY/MAPPLATE_SIZE;
				if(wMPX != pObj->wMPX || wMPY != pObj->wMPY)
				{
					m_szMapPlates[pObj->wMPY*m_wMaxPX+pObj->wMPX].LeavePlate(pObj);
					m_szMapPlates[wMPY*m_wMaxPX+wMPX].EnterPlate(pObj);
				}
			}

			for (int i = 0; i < pMp->m_pListRobot->Count; i++)
			{
				MapAIObj* pObj	= (MapAIObj*)pMp->m_pListRobot->Items[i];
				pObj->RunCmd();
				pObj->Update();
				WORD wMPX	= pObj->nX/MAPPLATE_SIZE;
				WORD wMPY	= pObj->nY/MAPPLATE_SIZE;
				if(wMPX != pObj->wMPX || wMPY != pObj->wMPY)
				{
					m_szMapPlates[pObj->wMPY*m_wMaxPX+pObj->wMPX].LeavePlate(pObj);
					m_szMapPlates[wMPY*m_wMaxPX+wMPX].EnterPlate(pObj);
				}
			}

			pMp++;

		}
	}
	m_pBornCenter->CheckGSSync();
	return true;
}

void		MapManager::SetWeatherTime(int nw,int nsec)
{
	if(m_dwWeatherSec > 0)
	{
		if(m_byWeather == nw)
		{
			m_dwWeatherSec += nsec;
		}
		else
		{
			m_dwWeatherSec = nsec;
			m_byWeather = nw;
			BuildPacketEx(pPacket,MAPCMD,MS_WEATHCHG,buf,256);
			SET_DATA(pData,MAPCMD,MS_WEATHCHG,pPacket);
			pData->byWeath	= m_byWeather;
			g_pCtrlList->SendMapPacket(pPacket,m_dwMapIndex);
		}		
	}
	else
	{
		m_dwWeatherSec = nsec;
		m_dwTempWeather = m_byWeather;
		m_byWeather = nw;
		BuildPacketEx(pPacket,MAPCMD,MS_WEATHCHG,buf,256);
		SET_DATA(pData,MAPCMD,MS_WEATHCHG,pPacket);
		pData->byWeath	= m_byWeather;
		g_pCtrlList->SendMapPacket(pPacket,m_dwMapIndex);
	}
}

void MapManager::UpdateSec()
{
	if(m_dwWeatherSec == 0 && m_dwUpdateSec % 900 == 0)
	{//15分钟
		BYTE bRandW	= m_pMCfgObj->RandGetWeather();
		if(bRandW != m_byWeather)
		{
			m_byWeather	= bRandW;

			BuildPacketEx(pPacket,MAPCMD,MS_WEATHCHG,buf,256);
			SET_DATA(pData,MAPCMD,MS_WEATHCHG,pPacket);
			pData->byWeath	= m_byWeather;
			g_pCtrlList->SendMapPacket(pPacket,m_dwMapIndex);
		}
	}

	if(m_dwWeatherSec == 1)
	{
		m_dwWeatherSec = 0;
		m_byWeather	= m_dwTempWeather;
		m_dwTempWeather = 0;

		BuildPacketEx(pPacket,MAPCMD,MS_WEATHCHG,buf,256);
		SET_DATA(pData,MAPCMD,MS_WEATHCHG,pPacket);
		pData->byWeath	= m_byWeather;
		g_pCtrlList->SendMapPacket(pPacket,m_dwMapIndex);
	}
	else if(m_dwWeatherSec > 1)
		m_dwWeatherSec --;

	m_dwUpdateSec++;

	m_pBornCenter->UpdateSec();
	for (int i = 0; i < m_wMaxPY; i++)
	{
		for (int j = 0; j < m_wMaxPX; j++)
		{
			m_szMapPlates[i*m_wMaxPX+j].UpdateSec();
		}
	}
}

void MapManager::RecvPacket( ZPacket* pPacket )
{
	if(pPacket->bCmdGroup != MAPOBJ)
		return;

	switch(pPacket->bCmd)
	{
	case 	GS_ADDOBJBYCFG:
		{//游戏服务器增加对象指令,手动刷怪
			SET_DATA(pData, MAPOBJ, GS_ADDOBJBYCFG,pPacket);

			if(GetMapObj(pData->byType,pData->dwID))
				return;

			EnemyBase* pBase	= EnemyBaseCfg::GetInstance().GetEnemyBase(pData->wBase);
			if(pBase == NULL)
				return;

			MapAIObj* pObj	= new MapAIObj();
			pObj->byType	= MAPOBJ_ENEMY;
			pObj->dwUin		= pData->dwID;
			pObj->bLv		= pBase->bLvl;
			pObj->nX		= pData->wPosX;
			pObj->nY		= pData->wPosY;
			pObj->pBase		= pBase;
			pObj->wSkin[0]	= pBase->nID;

			int	nMaxLoop = 10;
			while(nMaxLoop-- > 0 && GetBlock(pObj->nX,pObj->nY))
			{
				pObj->nX	= pObj->nX -100+rand()%200;
				pObj->nY	= pObj->nY -100+rand()%200;
			}
			nMaxLoop = 10;
			while(nMaxLoop-- > 0 && GetBlock(pObj->nX,pObj->nY))
			{
				pObj->nX	= pObj->nX -250+rand()%500;
				pObj->nY	= pObj->nY -250+rand()%500;
			}

			if(pObj->nX <= 0)
				pObj->nX	= 1;
			if(pObj->nY <= 0)
				pObj->nY	= 1;

			if(pObj->nX >= m_pMCfgObj->m_wMapSizeW)
				pObj->nX	= m_pMCfgObj->m_wMapSizeW - 1;
			if(pObj->nY >= m_pMCfgObj->m_wMapSizeH)
				pObj->nY	= m_pMCfgObj->m_wMapSizeH - 1;

			if(pData->wPowEx == 0)
				pData->wPowEx	= 100;
			pObj->xParam.nHp	= pBase->nMaxHp*pData->wPowEx/100;
			if(pObj->xParam.nHp == 0)
				pObj->xParam.nHp	= 100;
			pObj->xParam.nHpMax	= pBase->nMaxHp*pData->wPowEx/100;
			pObj->xParam.nMp	= 100;
			pObj->xParam.nMpMax	= 100;
			pObj->xParam.nAtkW	= pBase->nAtkW*pData->wPowEx/100;
			pObj->xParam.nAtkN	= pBase->nAtkN*pData->wPowEx/100;
			pObj->xParam.nDefW	= pBase->nDefW*pData->wPowEx/100;
			pObj->xParam.nDefN	= pBase->nDefN*pData->wPowEx/100;
			pObj->xParam.nAim	= pBase->nAim*pData->wPowEx/100;
			pObj->xParam.nCrit	= pBase->nCrit*pData->wPowEx/100;
			pObj->xParam.nDodge	= pBase->nDodge*pData->wPowEx/100;
			pObj->xParam.nMSpeed= pBase->nSpeed;
			pObj->xParam.nPvpDef	= 0;
			
			pObj->bForce		= pData->bForce;
			if (strlen(pData->szTitle) != 0)
				strcpy(pObj->szTitle,pData->szTitle);

			for (int i = 0; i < pBase->pLSkill->Count; i++)
			{
				pObj->pSkillCenter->SetSkill((int)pBase->pLSkill->Items[i],1,0,0);
			}

			AIObjEnterMap(pObj);
			m_pBornCenter->GSBornMapObj(pObj);
		}
		break;
	case	GS_SETLOCKVIEW:
		{
			SET_DATA(pData, MAPOBJ, GS_SETLOCKVIEW,pPacket);
			MapAIObj* pMine	= GetMapObj(pData->byType,pData->dwID);
			if(pMine)
			{
				if(pData->bLock)
				{
					MapAIObj* pObj	= GetMapObj(pData->byObjType,pData->dwObjID);
					if(pObj)
					{
						pMine->JumpInMap(pObj->nX,pObj->nY);
						pMine->SetCmd(MCMD_LOCKVIEW_ENEMY,pObj->dwUin,pObj);
						pMine->_syncClientCmd(MCMD_LOCKVIEW_ENEMY,pObj->dwUin,true);
					}

				}
				else
				{
					if(pMine->bCmd == MCMD_LOCKVIEW_ENEMY)
					{
						pMine->_syncClientCmd(MCMD_NULL,0,true);
						pMine->SetCmd(MCMD_NULL,0);
					}
				}
			}
		}
		break;
	case	GS_HEALTHHPMP:
		{
			SET_DATA(pData, MAPOBJ, GS_HEALTHHPMP,pPacket);
			MapAIObj* pObj	= GetMapObj(pData->byType,pData->dwID);
			if(pObj)
			{
				if(pData->bRevive && pObj->xParam.nHp<=0)
					pObj->xParam.nHp	= 1;
				pObj->ChgHp(pData->nAddHp);
				pObj->ChgMp(pData->nAddMp);
				if(!pData->bRevive&&pData->nAddHp)
				{
					BuildPacketEx(pSendPacket,MAPCMD,MS_HEALTHHPMP,buf,256);
					SET_DATA(pSendData,MAPCMD,MS_HEALTHHPMP,pSendPacket);
					pSendData->byType = pObj->byType;
					pSendData->dwUin = pObj->dwUin;
					pSendData->nAddHp = pData->nAddHp;
					pObj->pRunPlate->SendPacketExCtrl(pSendPacket);
				}
			}
		}
		break;
	case	GS_CHGWEATHER_TIME:
		{
			SET_DATA(pData,MAPOBJ,GS_CHGWEATHER_TIME,pPacket);
			SetWeatherTime(pData->byWeather,pData->nSec);
		}
		break;
	case	GS_SETRUNPATH:
		{
			SET_DATA(pData,MAPOBJ,GS_SETRUNPATH,pPacket);
			MapAIObj* pObj	= GetMapObj(MAPOBJ_ENEMY,pData->dwID);
			if(pObj)
			{
				if(pObj->pPtPath)
					delete[] pObj->pPtPath;
				pObj->pPtPath	= new POINT[pData->wPtCnt];
				pObj->wPathPtCnt	= pData->wPtCnt;
				memcpy(pObj->pPtPath,pData->szPt,pData->wPtCnt*sizeof(POINT));
			}
		}
		break;
	case	GS_SETBATTLEGROUP:
		{
			SET_DATA(pData,MAPOBJ,GS_SETBATTLEGROUP,pPacket);
			MapAIObj* pObj	= GetMapObj(MAPOBJ_ENEMY,pData->dwID);
			if(pObj)
			{
				pObj->byBattelPos	= pData->byPos;
				pObj->dwBattelGroup	= pData->dwMaster;

				BuildPacketEx(pSendPacket, MAPCMD,MS_CHGBTGROUP,buf,256);
				SET_DATA(pSend, MAPCMD,MS_CHGBTGROUP,pSendPacket);
				pSend->byType	= MAPOBJ_ENEMY;
				pSend->dwID		= pData->dwID;
				pSend->bRandW	= 100+pData->byPos;
				pSend->dwMast	= pData->dwMaster;
				pObj->pRunPlate->SendPacketOutCtrl(pSendPacket);
				
				
			}
		}
		break;
	case	GS_INITSTATE:
		{
			SET_DATA(pData, MAPOBJ, GS_INITSTATE,pPacket);
			MapAIObj* pObj	= GetMapObj(pData->bType,pData->dwID);
			if(pObj)
			{
				for (int i = 0; i < pData->wCnt; i++)
				{
					pObj->pStateCenter->SetState(pData->xState[i].wStateID,pData->xState[i].bLvl,pData->xState[i].dwTickTime);
				}
			}
		}
		break;
	case	GS_SYNCSKILL:
		{
			SET_DATA(pData, MAPOBJ, GS_SYNCSKILL,pPacket);
			MapAIObj* pObj	= GetMapObj(pData->byType,pData->dwID);
			if(pObj)
			{
				pObj->pSkillCenter->ResetJobSkill();
				for (int i = 0; i < pData->bySCnt; i++)
				{
					pObj->pSkillCenter->SetSkill(pData->szSkill[i].wSkill,pData->szSkill[i].byLv,pData->szSkill[i].bPos,pData->szSkill[i].dwCDTick);
				}
				pObj->_syncSkillChg();
			}
		}
		break;
	case GS_SETENEMY:
		{
			SET_DATA(pData, MAPOBJ, GS_SETENEMY, pPacket);
			MapAIObj* pTagObj	= GetMapObj(pData->bTagType,pData->dwTagID);
			MapAIObj* pObj	= GetMapObj(MAPOBJ_ENEMY,pData->dwID);
			if(!pObj || !pTagObj)
				return;
			if(pData->bActType == 9)
			{
				pObj->byDTargetType	= pData->bTagType;
				pObj->dwDTargetID	= pData->dwTagID;
				if(abs(pObj->nX-pTagObj->nX) + abs(pObj->nY-pTagObj->nY) > 2000)
				{
					pObj->SetCmd(MCMD_RUN,(pTagObj->nY<<16)+pTagObj->nX);
				}
				else
					pObj->SetCmd(MCMD_ATTACK,0,pTagObj);

			}
			else if(pTagObj->byType == MAPOBJ_PLAYER && pTagObj->pCtrl)
			{
				if(pData->bActType <= 4)
				{
					pTagObj->pCtrl->SetCtrl(pData->bActType,pObj);
				}
			}
		}
		break;
	case GS_SYNCTITLE:
		{
			SET_DATA(pData, MAPOBJ, GS_SYNCTITLE,pPacket);
			MapAIObj* pObj	= GetMapObj(pData->byType,pData->dwID);
			if(pObj)
			{
				strcpy(pObj->szTitle,pData->szTitle);
				pObj->_syncTitleChg();
			}
		}
		break;
	case GS_SYNCMARRYTITLE:
		{
			SET_DATA(pData,MAPOBJ,GS_SYNCMARRYTITLE,pPacket);
			MapAIObj* pObj = GetMapObj(pData->byType,pData->dwID);
			if(pObj)
			{
				strcpy(pObj->szMarryTitle,pData->szTitle);
				pObj->_syncMarryTitleChg();
			}
		}
		break;
	case GS_SYNCMARRYEFF:
		{
			SET_DATA(pData,MAPOBJ,GS_SYNCMARRYEFF,pPacket);
			MapAIObj* pObj = GetMapObj(pData->byType,pData->dwID);
			if(pObj)
			{
				pObj->nMarryEff= pData->nEff;
				pObj->_syncMarryEffChg();
			}
		}
		break;
	case GS_SYNCVIPLV:
		{
			SET_DATA(pData, MAPOBJ, GS_SYNCVIPLV,pPacket);
			MapAIObj* pObj	= GetMapObj(pData->byType,pData->dwID);
			if(pObj)
			{
				pObj->byViplv = pData->byViplv;
				pObj->_syncViplvChg();
			}
		}
		break;
	case	GS_SYNCPARAM:
		{
			SET_DATA(pData, MAPOBJ, GS_SYNCPARAM,pPacket);
			MapAIObj* pObj	= GetMapObj(pData->byType,pData->dwID);
			if(pObj)
			{
				memcpy(pObj->wSkin,pData->wSkin,6);
				//switch(pObj->byType)
				//{
				//case MAPOBJ_ENEMY:
				//	pObj->pBase	= EnemyBaseCfg::GetInstance().GetEnemyBase(pObj->wSkin[0]);
				//	break;
				//}
				int nOldHp	= pObj->xParam.nHp;
				int nOldMp	= pObj->xParam.nMp;
				memcpy(&pObj->xParam,&pData->xParam,sizeof(MParam));
				

				if(pData->byLv > pObj->bLv)
				{
					pObj->xParam.nHp	= pObj->xParam.nHpMax;
					pObj->xParam.nMp	= pObj->xParam.nMpMax;
					pObj->bGSSyncInfo	= true;
				}
				else
				{
					pObj->xParam.nHp	= nOldHp;
					//if(pObj->xParam.nHp > pObj->xParam.nHpMax)
					//	pObj->xParam.nHp	= pObj->xParam.nHpMax;
					pObj->xParam.nMp	= nOldMp;
				}
				pObj->bLv		= pData->byLv;
				//同步给客户端
				
				pObj->ResetParam();
			}
		}
		break;
	case 	GS_ADDOBJBYPAM:
		{//以战斗参数形式刷怪, 这边刷的怪物ID要分配好ID段
			SET_DATA(pData, MAPOBJ, GS_ADDOBJBYPAM,pPacket);

			MapAIObj* pObj	= GetMapObj(pData->byType,pData->dwID);
			if(pObj == NULL)
			{
				pObj	= new MapAIObj();
				pObj->byType	= pData->byType;
				pObj->dwUin		= pData->dwID;
				pObj->nX		= pData->wPosX;
				pObj->nY		= pData->wPosY;

				if(pObj->nX >= m_pMCfgObj->m_wMapSizeW)
					pObj->nX	= m_pMCfgObj->m_wMapSizeW - 1;
				if(pObj->nY >= m_pMCfgObj->m_wMapSizeH)
					pObj->nY	= m_pMCfgObj->m_wMapSizeH - 1;

				int	nMaxLoop = 0;
				while(nMaxLoop++ < 20 && GetBlock(pObj->nX,pObj->nY))
				{
					int	nRand	= 200+nMaxLoop*50;
					pObj->nX	= pObj->nX -nRand/2+rand()%nRand;
					pObj->nY	= pObj->nY -nRand/2+rand()%nRand;
				}

				if(GetBlock(pObj->nX,pObj->nY))
				{
					pObj->nX	= m_pMCfgObj->m_wReviveX;
					pObj->nY	= m_pMCfgObj->m_wReviveY;
				}

				if(pObj->nX <= 0)
					pObj->nX	= 1;
				if(pObj->nY <= 0)
					pObj->nY	= 1;

				if(pObj->nX >= m_pMCfgObj->m_wMapSizeW)
					pObj->nX	= m_pMCfgObj->m_wMapSizeW - 1;
				if(pObj->nY >= m_pMCfgObj->m_wMapSizeH)
					pObj->nY	= m_pMCfgObj->m_wMapSizeH - 1;

				pObj->bLv		= pData->byLv;
				pObj->byJob		= pData->byJob;
				pObj->bySex		= pData->bySex;
				memcpy(pObj->wSkin,pData->wSkin,6);
				
				strcpy(pObj->szName,pData->szName);
				strcpy(pObj->szTitle,pData->szTitle);
				strcpy(pObj->szMarryTitle,pData->szMarryTitle);
				memcpy(&pObj->xParam,&pData->xParam,sizeof(MParam));
				pObj->byCountry	= pData->byCountry;
				pObj->wHomeID	= pData->wHomeID;
				pObj->byViplv	= pData->byViplv;
				pObj->nMarryEff = pData->nMarryEff;
				CalcObjForce(pObj);

				switch(pObj->byType)
				{
				case MAPOBJ_ENEMY:
					pObj->pBase	= EnemyBaseCfg::GetInstance().GetEnemyBase(pObj->wSkin[0]);
					break;
				}

				pObj->pSkillCenter->ResetJobSkill();
				for (int i = 0; i < pData->bySCnt; i++)
				{
					pObj->pSkillCenter->SetSkill(pData->szSkill[i].wSkill,pData->szSkill[i].byLv,pData->szSkill[i].bPos,pData->szSkill[i].dwCDTick);
				}
				AIObjEnterMap(pObj);
				m_pBornCenter->GSBornMapObj(pObj);

				if(pObj->byType == MAPOBJ_PLAYER)
				{//进入有重生点的地图设置重生点
					if(m_pMCfgObj->m_wReviveX && m_pMCfgObj->m_wReviveY)
					{
						BuildPacketEx(pPacketex,MAPOBJ,MS_SETREVIVE,buf,256);
						SET_DATA(pSend,MAPOBJ,MS_SETREVIVE,pPacketex);
						pSend->wRM		= m_pMCfgObj->m_wMapIndex;
						pSend->wRX		= m_pMCfgObj->m_wReviveX;
						pSend->wRY		= m_pMCfgObj->m_wReviveY;
						g_pNetCenter->SendPacketToGameSrv(pPacketex,pObj->dwUin);
					}
				}
			}
			else
			{
				memcpy(pObj->wSkin,pData->wSkin,6);

				strcpy(pObj->szName,pData->szName);
				strcpy(pObj->szTitle,pData->szTitle);
				strcpy(pObj->szMarryTitle,pData->szMarryTitle);

				pObj->pSkillCenter->ResetJobSkill();
				for (int i = 0; i < pData->bySCnt; i++)
				{
					pObj->pSkillCenter->SetSkill(pData->szSkill[i].wSkill,pData->szSkill[i].byLv,pData->szSkill[i].bPos,pData->szSkill[i].dwCDTick);
				}
				pObj->byViplv	= pData->byViplv;
				pObj->nMarryEff = pData->nMarryEff;
				if(pObj->bDisappear == true)
				{
					pObj->bDisappear	= false;
					if(pObj->byType == MAPOBJ_PLAYER && GetMapObj(pData->byType,pData->dwID) == pObj)
					{//还没被送入回收站
						memcpy(&pObj->xParam,&pData->xParam,sizeof(MParam));
					}
					
					pObj->nX		= pData->wPosX;
					pObj->nY		= pData->wPosY;

					CalcObjForce(pObj);
					
					
					pObj->SetCmd(0,0);
					AIObjEnterMap(pObj);
				}
				else
				{
					memcpy(&pObj->xParam,&pData->xParam,sizeof(MParam));
				}
			}
			//WORD	wSkin[3];
			//WORD	wSkill[6];
			//BYTE	bySkillLv[6];
		}
		break;
	case	GS_CHGMFORCE:
		{
			SET_DATA(pData, MAPOBJ, GS_CHGMFORCE,pPacket);
			m_byForceType	= pData->nFType;

			for (int i = 0; i < m_wMaxPY; i++)
			{
				for (int j = 0; j < m_wMaxPX; j++)
				{
					m_szMapPlates[i*m_wMaxPX+j].ResetMapForce();
				}
			}
		}
		break;
	case	GS_CHGMPOW:
		{
			SET_DATA(pData, MAPOBJ, GS_CHGMPOW,pPacket);

			m_wExPow	= pData->nPow;
			m_byChgLv	= pData->nLv;
			for (int i = 0; i < m_wMaxPY; i++)
			{
				for (int j = 0; j < m_wMaxPX; j++)
				{
					m_szMapPlates[i*m_wMaxPX+j].ChgPowLv(pData->nPow,pData->nLv);
				}
			}
		}
		break;
	case 	GS_REMOVEOBJ:
		{//
			SET_DATA(pData, MAPOBJ, GS_REMOVEOBJ,pPacket);
			MapAIObj* pObj	= GetMapObj(pData->byType,pData->dwID);
			AIObjLeaveMap(pObj);
		}
		break;
	case 	GS_RESET_OBJSKIN:
		{//
			SET_DATA(pData, MAPOBJ, GS_RESET_OBJSKIN,pPacket);
			MapAIObj* pObj	= GetMapObj(pData->byType,pData->dwID);
			if (pObj)
				pObj->_syncParamChg();
		}
		break;
	case	GS_KILLENEMY:
		{
			SET_DATA(pData, MAPOBJ, GS_KILLENEMY,pPacket);
			MapAIObj*	pRqer	= NULL;
			if(pData->dwObj)
				pRqer	= GetMapObj(MAPOBJ_PLAYER,pData->dwObj);
			xnList*	pListTmp	= xnList::Create();
			GetPlateRangeA(pData->wX/MAPPLATE_SIZE,pData->wY/MAPPLATE_SIZE,pData->wRange/MAPPLATE_SIZE,pListTmp);
			for (int i = 0; i < pListTmp->Count; i++)
			{
				MapPlate* pPlate	= (MapPlate*)pListTmp->Items[i];
				pPlate->KillEnemy(pRqer);
			}
			pListTmp->Free();
		}
		break;
	case	GS_KILL_MAPENEMY_BYID:
		{
			SET_DATA(pData, MAPOBJ, GS_KILL_MAPENEMY_BYID,pPacket);

			xnList*	pListTmp	= xnList::Create();
			GetPlateAll(pListTmp);
			for (int i = 0; i < pListTmp->Count; i++)
			{
				MapPlate* pPlate	= (MapPlate*)pListTmp->Items[i];
				pPlate->KillEnemyByID(pData->wEnemyID);
			}
			pListTmp->Free();
		}
		break;
	case	GS_ADDSTATE:
		{
			SET_DATA(pData, MAPOBJ, GS_ADDSTATE,pPacket);

			MapAIObj* pObj	= GetMapObj(pData->byType,pData->dwID);
			if(pObj)
			{
				if(pData->bRemove)
					pObj->pStateCenter->RemoveState(pData->wState);
				else
					pObj->pStateCenter->AddState(pData->wState);
			}
		}
		break;
	case	GS_CLEAN_RIDE_LOCK:
		{
			SET_DATA(pData, MAPOBJ, GS_CLEAN_RIDE_LOCK,pPacket);

			MapAIObj* pObj	= GetMapObj(pData->byType,pData->dwID);
			if(pObj)
				pObj->pStateCenter->CleanRideLockState();
		}
		break;
	case	GS_CHGPOS:
		{
			SET_DATA(pData, MAPOBJ, GS_CHGPOS,pPacket);

			MapAIObj* pObj	= GetMapObj(pData->byType,pData->dwID);
			if(pObj)
			{
				pObj->JumpInMap(pData->wX,pData->wY);
			}
		}
		break;
	}
}

void MapManager::AIObjEnterMap( MapAIObj* pObj,bool bWild )
{
	pObj->ResetParam();

	WORD	wMX	= pObj->nX/MAPPLATE_SIZE;
	WORD	wMY	= pObj->nY/MAPPLATE_SIZE;
	m_szMapPlates[wMY*m_wMaxPX+wMX].EnterPlate(pObj);
	pObj->nMIndex	= m_dwMapIndex;

	m_nActiveCnt++;
	if(pObj->byType == MAPOBJ_PLAYER)
		m_nUserCnt++;

	if(pObj->pCtrl == NULL && pObj->byType == MAPOBJ_ENEMY && pObj->pBase && pObj->pBase->bType == 2)
	{
		BuildPacketEx(pPacket,MAPOBJ,MS_BOSSBORN,buf,256);
		SET_DATA(pSend,MAPOBJ,MS_BOSSBORN,pPacket);
		pSend->dwMapIndex	= m_dwMapIndex;
		pSend->wBossID	= pObj->pBase->nID;
		pSend->bWild    = bWild;
		g_pNetCenter->SendPacketToGameSrv(pPacket,0);

		OutMessage(0,"BOSSENTER: ID=%d,MapIndex=%d,HP=%d,isDisap=%d,pos=%d:%d",pObj->pBase->nID,m_dwMapIndex,pObj->xParam.nHp,pObj->bDisappear,pObj->nX,pObj->nY);
	}
}

void MapManager::AIObjLeaveMap( MapAIObj* pObj )
{
	if(pObj == NULL)
		return;
	if(pObj->pRunPlate)
		pObj->pRunPlate->LeavePlate(pObj,true);
	pObj->bDisappear	= true;
	pObj->nMIndex	= 0;
	ObjSyncInfo(pObj);
	m_nActiveCnt--;
	if(pObj->byType == MAPOBJ_PLAYER)
		m_nUserCnt--;

	if(pObj->pCtrl == NULL && pObj->byType == MAPOBJ_ENEMY && pObj->pBase && pObj->pBase->bType == 2)
	{
		BuildPacketEx(pPacket,MAPOBJ,MS_BOSSDEAD,buf,256);
		SET_DATA(pSend,MAPOBJ,MS_BOSSDEAD,pPacket);
		pSend->dwMapIndex	= m_dwMapIndex;
		pSend->wBossID	= pObj->pBase->nID;
		g_pNetCenter->SendPacketToGameSrv(pPacket,0);

		OutMessage(0,"BOSSLEAVE: ID=%d,MapIndex=%d,HP=%d,isDisap=%d,pos=%d:%d",pObj->pBase->nID,m_dwMapIndex,pObj->xParam.nHp,pObj->bDisappear,pObj->nX,pObj->nY);
	}
	
}

MapAIObj* MapManager::GetMapObj( BYTE bType, DWORD dwID )
{
	return m_pBornCenter->GetMapObj(bType,dwID);
}

DWORD MapManager::LockEnemyID()
{
	m_dwEnemyIndex++;
	if(m_dwEnemyIndex > 4000000000)
		m_dwEnemyIndex	= 100000000;
	return m_dwEnemyIndex;
}

void MapManager::ObjSyncPos( MapAIObj* pObj )
{
	BuildPacketEx(pPacket,MAPOBJ,MS_OBJPOS,buf,256);
	SET_DATA(pSend,MAPOBJ,MS_OBJPOS,pPacket);
	pSend->byType	= pObj->byType;
	pSend->dwID		= pObj->dwUin;
	pSend->dwMIndex	= pObj->nMIndex;
	pSend->wPosX	= pObj->nX;
	pSend->wPosY	= pObj->nY;
	if(pObj->byType == MAPOBJ_PLAYER)
		g_pNetCenter->SendPacketToGameSrv(pPacket,pObj->dwUin);
	else if(pObj->GetMasterID())
		g_pNetCenter->SendPacketToGameSrv(pPacket,pObj->GetMasterID());
}

void MapManager::ObjSyncInfo( MapAIObj* pObj )
{//下线或HP为0死亡的时候发送
	BuildPacketEx(pPacket,MAPOBJ,MS_OBJINFO,buf,256);
	SET_DATA(pSend,MAPOBJ,MS_OBJINFO,pPacket);
	pSend->byType	= pObj->byType;
	pSend->dwID		= pObj->dwUin;
	pSend->nHp		= pObj->xParam.nHp;
	pSend->nMp		= pObj->xParam.nMp;
	if(pObj->pCtrl)
		pSend->wSp		= pObj->pCtrl->m_nSpVal;
	if(pObj->byType == MAPOBJ_PLAYER)
		g_pNetCenter->SendPacketToGameSrv(pPacket,pObj->dwUin);
	else if(pObj->GetMasterID())
		g_pNetCenter->SendPacketToGameSrv(pPacket,pObj->GetMasterID());
}
bool compareValue(const std::pair<int,int>& x, const std::pair<int,int>& y)
{
	return x.second > y.second;
}
void MapManager::ObjKillReward( MapAIObj* pKill, MapAIObj* pDead )
{
	if(pDead->bIsRewarded)
		return;
	pDead->bIsRewarded	= true;
	if(pKill->pCtrl && pDead->byType == MAPOBJ_ENEMY && pKill->pCtrl->m_pMainAIObj == pKill)
	{
		if(pDead->pBase->bType != 3)
			pKill->pCtrl->ChgSP(2);	//加2点怒气
	}

	if (pDead && pDead->byType == MAPOBJ_ENEMY && pDead->pBase 
		&& pKill && pKill->byType == MAPOBJ_ENEMY)
	{
		BuildPacketEx(pPacket,MAPOBJ,MS_ENEMY_BEKILL,buf,256);
		SET_DATA(pSend,MAPOBJ,MS_ENEMY_BEKILL,pPacket);
		pSend->dwMapIndex	= m_dwMapIndex;
		pSend->wEnemyID  	= pDead->pBase->nID;
		g_pNetCenter->SendPacketToGameSrv(pPacket,0);
	}

	if(pDead->dwLockAward)
	{
		BuildPacketEx(pPacket,MAPOBJ,MS_OBJKILL,buf,256);
		SET_DATA(pSend,MAPOBJ,MS_OBJKILL,pPacket);
		pSend->bType	= MAPOBJ_PLAYER;
		pSend->dwID		= pDead->dwLockAward;
		pSend->byTargetT	= pDead->byType;
		if(pDead->byType == MAPOBJ_ENEMY && pDead->pBase)	//杀死怪物
		{
			pSend->wBaseID	= pDead->pBase->nID;
		}
		else
			pSend->wBaseID		= 0;
		pSend->dwTargetID	= pDead->dwUin;
		g_pNetCenter->SendPacketToGameSrv(pPacket,pDead->dwLockAward);
	}
	else
	{
		if(pKill->pCtrl)
		{
			BuildPacketEx(pPacket,MAPOBJ,MS_OBJKILL,buf,256);
			SET_DATA(pSend,MAPOBJ,MS_OBJKILL,pPacket);
			pSend->bType	= pKill->byType;
			pSend->dwID		= pKill->dwUin;
			pSend->byTargetT	= pDead->byType;
			if(pDead->byType == MAPOBJ_ENEMY && pDead->pBase)	//杀死怪物
			{
				pSend->wBaseID	= pDead->pBase->nID;
			}
			else
				pSend->wBaseID		= 0;
			pSend->dwTargetID	= pDead->dwUin;
			g_pNetCenter->SendPacketToGameSrv(pPacket,pKill->pCtrl->m_dwUin);
		}

		if(pDead->pCtrl)
		{
			BuildPacketEx(pPacket,MAPOBJ,MS_OBJBEKILL,buf,256);
			SET_DATA(pSend,MAPOBJ,MS_OBJBEKILL,pPacket);
			pSend->byType	= pDead->byType;
			pSend->dwID		= pDead->dwUin;
			pSend->byAtkT	= pKill->byType;
			if(pKill->byType == MAPOBJ_ENEMY && pKill->pBase)	//被怪物杀死
				pSend->dwAtkID	= pKill->pBase->nID;
			else
				pSend->dwAtkID	= pKill->dwUin;
			g_pNetCenter->SendPacketToGameSrv(pPacket,pDead->pCtrl->m_dwUin);
		}
	}

	if(pDead->pBase && pDead->pBase->bNoticeGS)
	{//部分怪有脚本的需要通知游戏服务器
		BuildPacketEx(pPacket,MAPOBJ,MS_ENEMYDEAD,buf,256);
		SET_DATA(pSend,MAPOBJ,MS_ENEMYDEAD,pPacket);
		if(pKill)
		{
			pSend->byType	= pKill->byType;
			if(pKill->byType == MAPOBJ_ENEMY && pKill->pBase)	//被怪物杀死
				pSend->dwID	= pKill->pBase->nID;
			else
				pSend->dwID	= pKill->dwUin;
		}
		else
		{
			pSend->byType	= 0;
			pSend->dwID		= 0;
		}
		pSend->nMIndex	= m_dwMapIndex;
		pSend->wBaseMI	= m_pMCfgObj->m_wMapIndex;
		pSend->wBaseID	= pDead->pBase->nID;
		g_pNetCenter->SendPacketToGameSrv(pPacket,0);
	}
	if(pDead->pBase && pDead->pBase->nID == nLichBossID){
		
		vector<pair<int,int> > vec;
		for (map<int,int>::iterator itor = pDead->m_mapDamage.begin();itor != pDead->m_mapDamage.end();itor++)
		{
			vec.push_back(make_pair(itor->first,itor->second));
		}
		sort(vec.begin(),vec.end(),compareValue);
		BuildPacketEx(pPacket,MAPOBJ,MS_LICHKILLED,buf,256);
		SET_DATA(pSend,MAPOBJ,MS_LICHKILLED,pPacket);
		for (int i=0;i<10;i++)
		{
			if (i>=vec.size())
			{
				break;
			}
			pSend->nUin[i] = vec[i].first;
			pSend->nDamage[i] = vec[i].second;
		}
		if (pKill->pCtrl)
		{
			pSend->nUin[10] = pKill->pCtrl->m_dwUin;
			pSend->nDamage[10] = pDead->m_mapDamage[pKill->pCtrl->m_dwUin];
		}
		g_pNetCenter->SendPacketToGameSrv(pPacket,0);
	}
}

void MapManager::LichKingKillBySystReward(MapAIObj* pDead)
{
	if(pDead->pBase && pDead->pBase->nID == nLichBossID){

		vector<pair<int,int> > vec;
		for (map<int,int>::iterator itor = pDead->m_mapDamage.begin();itor != pDead->m_mapDamage.end();itor++)
		{
			vec.push_back(make_pair(itor->first,itor->second));
		}
		sort(vec.begin(),vec.end(),compareValue);
		BuildPacketEx(pPacket,MAPOBJ,MS_LICHKILLED,buf,256);
		SET_DATA(pSend,MAPOBJ,MS_LICHKILLED,pPacket);
		for (int i=0;i<10;i++)
		{
			if (i>=vec.size())
			{
				break;
			}
			pSend->nUin[i] = vec[i].first;
			pSend->nDamage[i] = vec[i].second;
		}
		g_pNetCenter->SendPacketToGameSrv(pPacket,0);
	}
}

int MapManager::GetPlateAll(OUT xnList* pList)
{
	pList->Clear();

	int	nStartPX	= 0;
	int nEndPX		= m_wMaxPX-1;

	int	nStartPY	= 0;
	int nEndPY		= m_wMaxPY-1;

	for (int i = nStartPY; i <= nEndPY; i++)
	{
		for (int j = nStartPX; j <= nEndPX; j++)
		{
			pList->Add(&m_szMapPlates[i*m_wMaxPX+j]);
		}
	}
	return pList->Count;
}

int MapManager::GetPlateRangeA( WORD wMPX,WORD wMPY,WORD wRange,OUT xnList* pList )
{
	pList->Clear();

	int	nStartPX	= (int)wMPX - wRange;
	int nEndPX		= (int)wMPX + wRange;
	if(nStartPX < 0)
		nStartPX	= 0;
	if(nEndPX >= m_wMaxPX)
		nEndPX	= m_wMaxPX-1;

	int	nStartPY	= (int)wMPY - wRange;
	int nEndPY		= (int)wMPY + wRange;
	if(nStartPY < 0)
		nStartPY	= 0;
	if(nEndPY >= m_wMaxPY)
		nEndPY	= m_wMaxPY-1;

	for (int i = nStartPY; i <= nEndPY; i++)
	{
		for (int j = nStartPX; j <= nEndPX; j++)
		{
			if(abs(i - wMPY)+abs(j-wMPX)<= wRange)
				pList->Add(&m_szMapPlates[i*m_wMaxPX+j]);
		}
	}
	return pList->Count;
}

int MapManager::GetPlateRangeB( WORD wMPX,WORD wMPY,WORD wRangeX,WORD wRangeY,OUT xnList* pList )
{
	pList->Clear();

	int	nStartPX	= (int)wMPX - wRangeX;
	int nEndPX		= (int)wMPX + wRangeX;
	if(nStartPX < 0)
		nStartPX	= 0;
	if(nEndPX >= m_wMaxPX)
		nEndPX	= m_wMaxPX-1;

	int	nStartPY	= (int)wMPY - wRangeY;
	int nEndPY		= (int)wMPY + wRangeY;
	if(nStartPY < 0)
		nStartPY	= 0;
	if(nEndPY >= m_wMaxPY)
		nEndPY	= m_wMaxPY-1;

	for (int i = nStartPY; i <= nEndPY; i++)
	{
		for (int j = nStartPX; j <= nEndPX; j++)
		{
			pList->Add(&m_szMapPlates[i*m_wMaxPX+j]);
		}
	}
	return pList->Count;
}

bool MapManager::CheckLineBlock( int nX, int nY, int nEX, int nEY, bool	bAtkTarget )
{
	if(bAtkTarget)
	{
		if(CalcDistance(nX,nY,nEX,nEY) < 500)
		{
			if(m_pMCfgObj->m_pMapBarr->GetBlock(nEX,nEY))
				return false;
			if(m_pMCfgObj->m_pMapBarr->GetBlock(nX,nY))
				return false;
		}
	}
	
	return m_pMCfgObj->m_pMapBarr->CheckLineBlock(nX,nY,nEX,nEY);
}

bool MapManager::GetBlock( int nX, int nY )
{
	return m_pMCfgObj->m_pMapBarr->GetBlock(nX,nY);
}

int MapManager::GetObjRange( WORD wX,WORD wY,WORD wRange,OUT xnList* pList )
{
	pList->Clear();
	int nPRange	= wRange/MAPPLATE_SIZE+1;
	int	nPX		= wX/MAPPLATE_SIZE;
	int	nPY		= wY/MAPPLATE_SIZE;

	xnList*	pLPlates	= xnList::Create();
	GetPlateRangeA(nPX,nPY,nPRange,pLPlates);

	for (int i = 0; i < pLPlates->Count; i++)
	{
		MapPlate* pMPlate	= (MapPlate*)pLPlates->Items[i];
		pMPlate->FillAllObj(pList);
	}

	pLPlates->Free();
	return pList->Count;
}

int MapManager::GetObjLine( WORD wX,WORD wY,WORD wTX,WORD wTY,int wRange,OUT xnList* pList )
{
	int nDis	= CalcDistance(wX,wY,wTX,wTY);

	int	nXChg	= wRange*(wTX - wX)/nDis;
	int	nYChg	= wRange*(wTY - wY)/nDis;

	int nPRange	= wRange/MAPPLATE_SIZE;
	if(nPRange <= 0)
		nPRange	= 1;
	xnList*	pLPlates	= xnList::Create();

	for (int i = 0; i <= nPRange; i++)
	{
		int	nPX		= (wX+nXChg*i/nPRange)/MAPPLATE_SIZE;
		int	nPY		= (wY+nYChg*i/nPRange)/MAPPLATE_SIZE;
		if(nPX >= m_wMaxPX)
			nPX	= m_wMaxPX-1;
		if(nPX < 0)
			nPX	= 0;
		if(nPY >= m_wMaxPY)
			nPY = m_wMaxPY-1;
		if(nPY < 0)
			nPY	= 0;
		pLPlates->Add(&m_szMapPlates[nPY*m_wMaxPX+nPX]);
	}
	MapPlate* pPreMP	= NULL;
	for (int i = 0; i < pLPlates->Count; i++)
	{
		MapPlate* pMPlate	= (MapPlate*)pLPlates->Items[i];
		if(pMPlate != pPreMP)
		{//可能有重复区域
			pPreMP	= pMPlate;
			pMPlate->FillAllObj(pList);
		}
	}

	pLPlates->Free();
	return 0;
}

void MapManager::ObjBeHit( MapAIObj* pObj )
{
	if(pObj->byType == MAPOBJ_PLAYER)
	{
		BuildPacketEx(pPacket,MAPOBJ,MS_OBJBEHIT,buf,256);
		SET_DATA(pSend,MAPOBJ,MS_OBJBEHIT,pPacket);
		pSend->byType	= pObj->byType;
		pSend->dwID		= pObj->dwUin;

		g_pNetCenter->SendPacketToGameSrv(pPacket,pObj->dwUin);
	}
}

void MapManager::MObjCallEnemy( MapAIObj* pMaster, WORD wBaseID, WORD wPow )
{
	EnemyBase* pBase	= EnemyBaseCfg::GetInstance().GetEnemyBase(wBaseID);
	if(pBase == NULL)
		return;

	MapAIObj* pObj	= new MapAIObj();
	pObj->byType	= MAPOBJ_ENEMY;
	pObj->dwUin		= LockEnemyID();
	pObj->bLv		= pBase->bLvl;
	pObj->nX		= pMaster->nX;
	pObj->nY		= pMaster->nY;
	pObj->pBase		= pBase;
	pObj->wSkin[0]	= pBase->nID;
	if(wPow == 0)
		wPow	= 100;
	pObj->xParam.nHpMax	= pMaster->xParam.nHpMax*wPow/100;
	pObj->xParam.nHp	= pMaster->xParam.nHpMax;
	pObj->xParam.nMp	= 100;
	pObj->xParam.nMpMax	= 100;

	pObj->xParam.nAtkW	= pMaster->xParam.nAtkW*wPow/100;
	pObj->xParam.nAtkN	= pMaster->xParam.nAtkN*wPow/100;
	pObj->xParam.nDefW	= pMaster->xParam.nDefW*wPow/100;
	pObj->xParam.nDefN	= pMaster->xParam.nDefN*wPow/100;
	pObj->xParam.nAim	= pMaster->xParam.nAim*wPow/100;
	pObj->xParam.nCrit	= pMaster->xParam.nCrit*wPow/100;
	pObj->xParam.nDodge	= pMaster->xParam.nDodge*wPow/100;
	pObj->xParam.nMSpeed= pBase->nSpeed;//pMaster->xParam.nMSpeed;
	pObj->xParam.nPvpDef	= 0;

	pObj->bForce		= pMaster->bForce;

	
	AIObjEnterMap(pObj);

	for (int i = 0; i < pBase->pLSkill->Count; i++)
	{
		pObj->pSkillCenter->SetSkill((int)pBase->pLSkill->Items[i],1,0,0);
	}
	
	m_pBornCenter->GSBornMapObj(pObj);

	if(pMaster->pCtrl)
	{
		pMaster->pCtrl->SetCtrl(2,pObj);
	}
}

int MapManager::GetPlayerCnt()
{
	return m_pBornCenter->GetPlayerCnt();
}

void MapManager::CalcObjForce( MapAIObj* pObj )
{
	if(pObj->byType == MAPOBJ_PLAYER || pObj->byType == MAPOBJ_ROBOT)
	{
		switch(m_byForceType)
		{
		case 0:
			pObj->bForce	= pObj->byCountry;
			break;
		case 1:
			pObj->bForce	= 1;
			break;
		case 2:
			pObj->bForce	= pObj->wHomeID+1;
			break;
		case 4:
			{
				pObj->bForce	= pObj->dwUin;
				break;
			}
		default:
			pObj->bForce	= 1;
			break;
		}
	}
	else
	{
		if(pObj->pCtrl)
		{
			pObj->bForce	= pObj->pCtrl->m_bForce;
		}
	}

	if(pObj->pCtrl && pObj->pCtrl->m_pMainAIObj == pObj)
	{//控制器的势力根据主控制对象改变
		pObj->pCtrl->m_bForce	= pObj->bForce;
	}
}


