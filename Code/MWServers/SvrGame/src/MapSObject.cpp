#include "stdafx.h"
#include "MapSObject.h"
#include "ios_mapobj_packet.h"
#include "ZNetGamesvr.h"
#include "ServerCfg.h"
#include "UserData.h"
#include "FsGameCenter.h"
#include "UserQuest.h"

MapSObject::MapSObject()
{
	bParamChged	= false;
	bSkinChged	= false;
	bSkillChged	= false;
	bTitleChged = false;
	bVipChged = false;
	bMarryTitleChgd = false;
	bMarryEffChgd = false;
	byViplv = 0;
	nMarryEff = 0;
	memset(szName,0,16);
	memset(szTitle,0,22);
	memset(szMarryTitle,0,32);
	memset(wSkin,0,6);
	pListSkill	= xnList::Create();
	pListState	= xnList::Create();
}

MapSObject::~MapSObject()
{
	while(pListSkill->Count)
		delete (_NetSkillInfo*)pListSkill->Delete(0);
	pListSkill->Free();

	while(pListState->Count)
		delete (_netStateInfo*)pListState->Delete(0);
	pListState->Free();
}

void MapSObject::SetSkill( WORD wSkill, BYTE bLv, BYTE bPos,DWORD	dwCDTick )
{
	if(this == NULL || wSkill == 0)
		return;

	_NetSkillInfo* pNSkill	= NULL;
	for (int i = 0; i < pListSkill->Count; i++)
	{
		_NetSkillInfo* pTmp	= (_NetSkillInfo*)pListSkill->Items[i];
		if(pTmp->wSkill == wSkill)
		{
			pNSkill	= pTmp;
			break;
		}
	}
	if(pNSkill == NULL)
	{
		pNSkill	= new _NetSkillInfo();
		pNSkill->wSkill	= wSkill;
		pNSkill->dwCDTick	= 0;
		pListSkill->Add(pNSkill);
	}
	pNSkill->bPos	= bPos;
	pNSkill->byLv	= bLv;
	if(dwCDTick)
		pNSkill->dwCDTick	= dwCDTick;
	bSkillChged		= true;
}

void MapSObject::ChangeTitle(char* pTitle)
{
	sprintf(szTitle,pTitle);
	bTitleChged = true;
}

void MapSObject::ChangeMarryTitle(char* pTitle)
{
	sprintf(szMarryTitle,pTitle);
	bMarryTitleChgd = true;
}

void MapSObject::ChangeMarryEff(int nID)
{
	nMarryEff = nID;
	bMarryEffChgd = true;;
}

void MapSObject::ChangeViplv(int nLv)
{
	byViplv = nLv;
	bVipChged = true;
}

void MapSObject::ResetSkill()
{
	while(pListSkill->Count)
		delete (_NetSkillInfo*)pListSkill->Delete(0);
	bSkillChged		= true;
}


UserMObjCenter::UserMObjCenter(UserData* pUser)
{
	m_pSlave	= NULL;
	m_pUser		= pUser;
	m_pNetSrvLink	= NULL;
	m_dwLockPos	= 0;
	m_wFuryVal	= 0;
	m_pPlayer	= new MapSObject();
	m_pListMark	= xnList::Create();
}

UserMObjCenter::~UserMObjCenter()
{
	ClearMarks();

	m_pListMark->Free();
	delete		m_pPlayer;
}

void UserMObjCenter::InitPlayer( DWORD dwUin,char* szName,DWORD dwMapIndex,WORD wX, WORD wY, BYTE bJob, BYTE bSex )
{
	m_pPlayer->nPosX		= wX;
	m_pPlayer->nPosY		= wY;
	m_pPlayer->byType		= MAPOBJ_PLAYER;
	m_pPlayer->dwID			= dwUin;
	m_pPlayer->byLv			= 1;
	m_pPlayer->byCountry	= 0;
	m_pPlayer->byViplv		= 0;
	m_pPlayer->byJob		= bJob;
	m_pPlayer->bySex		= bSex;
	m_pPlayer->wSkin[0]		= 1001;
	m_pPlayer->xParam.nHp	= 14235;
	m_pPlayer->xParam.nHpMax	= 15000;
	m_pPlayer->xParam.nMSpeed	= 225;
	m_pPlayer->xParam.nPvpDef	= 50;
	m_pPlayer->xParam.nMp		= 150;
	m_pPlayer->xParam.nMpMax	= 200;

	m_pPlayer->xParam.nAtkW		= 500;
	m_pPlayer->xParam.nAtkN		= 100;
	m_pPlayer->xParam.nDefW		= 300;
	m_pPlayer->xParam.nDefN		= 300;
	m_pPlayer->xParam.nAim		= 400;
	m_pPlayer->xParam.nDodge	= 300;
	m_pPlayer->xParam.nCrit		= 3300;
	strcpy(m_pPlayer->szName,szName);
}

void UserMObjCenter::ObjEnterMap( MapSObject* pMObj )
{
	if(pMObj == NULL || m_pNetSrvLink == NULL)
	{
		OutMessage(msg_type_error,"ObjEnterMap UIN=%d----pMObj=%d,m_pNetSrvLink=%d",m_dwUin,pMObj,m_pNetSrvLink);
		return;
	}
	pMObj->bParamChged	= false;
	pMObj->bSkillChged	= false;
	pMObj->bTitleChged	= false;
	pMObj->bVipChged	= false;
	pMObj->bMarryEffChgd = false;
	pMObj->bMarryTitleChgd = false;
	//GS_ADDOBJBYPAM,		//以战斗参数形式刷怪
	BuildPacketEx(pPacket, MAPOBJ,GS_ADDOBJBYPAM,buf,1024);
	SET_DATA(pSend, MAPOBJ,GS_ADDOBJBYPAM, pPacket);
	pSend->byType		= pMObj->byType;
	pSend->dwID			= pMObj->dwID;
	pSend->wPosX		= pMObj->nPosX;
	pSend->wPosY		= pMObj->nPosY;
	pSend->byLv			= pMObj->byLv;
	pSend->byJob		= pMObj->byJob;
	pSend->bySex		= pMObj->bySex;
	pSend->byCountry	= pMObj->byCountry;
	pSend->wHomeID		= m_pUser->m_dwHomeID;
	pSend->byViplv		= m_pUser->m_xUWealth[UW_VIPLV];
	pSend->nMarryEff	= pMObj->nMarryEff;
	strcpy(pSend->szName,pMObj->szName);
	strcpy(pSend->szTitle,pMObj->szTitle);
	strcpy(pSend->szMarryTitle,pMObj->szMarryTitle);
	if(pMObj->IsDead())
	{
		pMObj->xParam.nHp	= pMObj->xParam.nHpMax;
		pMObj->xParam.nMp	= pMObj->xParam.nMpMax;

		m_pUser->OnValChange();
	}
	else
	{
		//if(pMObj->xParam.nHp>pMObj->xParam.nHpMax)
		//	pMObj->xParam.nHp	= pMObj->xParam.nHpMax;
		//if(pMObj->xParam.nMp>pMObj->xParam.nMpMax)
		//	pMObj->xParam.nMp	= pMObj->xParam.nMpMax;
	}

	memcpy(&pSend->xParam,&pMObj->xParam,sizeof(MParam));
	if(ServerCfg::GetInstance().nVersionDef)
	{
		if(pMObj->wSkin[0] >= 1021 && pMObj->wSkin[0] <= 1028)
			pMObj->wSkin[0] -= 20;

		if(pMObj->wSkin[1] >= 3011 && pMObj->wSkin[1] <= 3028)
			pMObj->wSkin[1] = 3000+pMObj->wSkin[1]%10;
	}
	if(pMObj->wSkin[0] >= 1011 && pMObj->wSkin[0] <= 1018)
		pMObj->wSkin[0] -= 10;
	memcpy(pSend->wSkin,pMObj->wSkin,6);

	pSend->bySCnt		= pMObj->pListSkill->Count;
	pPacket->wSize		+= (pSend->bySCnt-1)*sizeof(_NetSkillInfo);
	DWORD	dwTick	= CMTime::GetCurTickCount();
	for (int i = 0; i < pMObj->pListSkill->Count; i++)
	{
		_NetSkillInfo*	pNS	= (_NetSkillInfo*)pMObj->pListSkill->Items[i];
		memcpy(&(pSend->szSkill[i]),pNS,sizeof(_NetSkillInfo));
		if(pNS->dwCDTick > dwTick)
			pSend->szSkill[i].dwCDTick	= (pNS->dwCDTick-dwTick)*1000;
		else
			pSend->szSkill[i].dwCDTick	= 0;
	}
	
	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);

	_syncObjState(pMObj);
}

void UserMObjCenter::ReSetMapObjSkin(MapSObject* pMObj)
{
	if(pMObj == NULL || m_pNetSrvLink == NULL)
		return;
	BuildPacketEx(pPacket, MAPOBJ,GS_RESET_OBJSKIN,buf,1024);
	SET_DATA(pSend, MAPOBJ,GS_RESET_OBJSKIN, pPacket);
	pSend->byType		= pMObj->byType;
	pSend->dwID			= pMObj->dwID;
	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
}

void UserMObjCenter::ObjLeaveMap( MapSObject* pMObj )
{
	if(pMObj == NULL || m_pNetSrvLink == NULL)
		return;
	BuildPacketEx(pPacket, MAPOBJ,GS_REMOVEOBJ,buf,1024);
	SET_DATA(pSend, MAPOBJ,GS_REMOVEOBJ, pPacket);
	pSend->byType		= pMObj->byType;
	pSend->dwID			= pMObj->dwID;
	//WORD	wSkill[6];
	//BYTE	bySkillLv[6];
	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
}

void UserMObjCenter::SetCtrl( MapSObject* pMObj,BYTE bCtrl )
{
	if(m_pNetSrvLink == NULL)
	{
		OutMessage(msg_type_error,"SetCtrl UIN=%d----pMObj=%d,m_pNetSrvLink=0",m_dwUin,pMObj);
		return;
	}
	BuildPacketEx(pPacket, MAPOBJ,GS_SETCTRL,buf,1024);
	SET_DATA(pSend, MAPOBJ,GS_SETCTRL, pPacket);
	pSend->bControl		= bCtrl;
	pSend->bType		= pMObj->byType;
	pSend->dwID			= pMObj->dwID;
	pSend->dwKey		= m_dwKey;
	pSend->wMapIndex	= m_dwMapIndex;
	pSend->wFury		= m_wFuryVal;
	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
}

void UserMObjCenter::InitCtrl(  )
{
	if(m_pNetSrvLink == NULL)
	{
		return;
	}
	BuildPacketEx(pPacket, MAPOBJ,GS_INITCTRL,buf,1024);
	SET_DATA(pSend, MAPOBJ,GS_INITCTRL, pPacket);
	pSend->dwKey		= m_dwKey;
	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
}

MObjSMark* UserMObjCenter::SetCtrl( DWORD dwID, BYTE bType,BYTE bCtrl, bool bMark )
{
	if(m_pNetSrvLink == NULL)
		return NULL;
	
	BuildPacketEx(pPacket, MAPOBJ,GS_SETCTRL,buf,1024);
	SET_DATA(pSend, MAPOBJ,GS_SETCTRL, pPacket);
	pSend->bControl		= bCtrl;
	pSend->bType		= bType;
	pSend->dwID			= dwID;
	pSend->dwKey		= m_dwKey;
	pSend->wMapIndex	= m_dwMapIndex;
	pSend->wFury		= m_wFuryVal;
	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);

	if(bMark && m_pPlayer)
	{//关注该怪物的信息
		MObjSMark* pMark	= GetMark(dwID);
		if(pMark == NULL)
		{
			pMark	= new MObjSMark();
			pMark->dwID		= dwID;
			pMark->nPosX	= m_pPlayer->nPosX;
			pMark->nPosY	= m_pPlayer->nPosY;
			m_pListMark->Add(pMark);
		}
		return pMark;
	}
	return NULL;

}

void UserMObjCenter::OffCtrl()
{
	if(m_pNetSrvLink == NULL)
		return;
	ClearMarks();
	BuildPacketEx(pPacket, MAPOBJ,GS_CTRLOFF,buf,1024);
	SET_DATA(pSend, MAPOBJ,GS_CTRLOFF, pPacket);
	pSend->dwKey		= m_dwKey;
	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
}


void UserMObjCenter::LockMapAtk( DWORD dwMIndex, BYTE bLock )
{
	if(m_pNetSrvLink == NULL)
		return;

	BuildPacketEx(pPacket, MAPOBJ,GS_MAPLOCKATK,buf,1024);
	SET_DATA(pSend, MAPOBJ,GS_MAPLOCKATK, pPacket);
	pSend->dwMapIndex		= dwMIndex;
	pSend->bLock			= bLock;
	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
}


void UserMObjCenter::SetMapIndex(DWORD dwMIndex,DWORD dwRMIndex )
{
	m_dwMapIndex	= dwMIndex;
	m_pNetSrvLink	= MapSelCfg::GetInstance().GetByMapIndex(dwRMIndex);
}

bool UserMObjCenter::PacketProcess( ZPacket* pPacket )
{
	if(pPacket->bCmdGroup != MAPOBJ)
		return false;

	switch(pPacket->bCmd)
	{
	case MS_OBJPOS:
		{
			SET_DATA(pData, MAPOBJ, MS_OBJPOS, pPacket);
			
			if(pData->dwMIndex != m_dwMapIndex)
				return true;
			if(pData->byType == MAPOBJ_PLAYER)
			{
				if(m_pPlayer && m_pPlayer->dwID == pData->dwID)
				{
					if(m_dwLockPos==0 || GetTickCount() - m_dwLockPos > 1000)
					{
						m_pPlayer->nPosX	= pData->wPosX;
						m_pPlayer->nPosY	= pData->wPosY;
					}
				}
			}
			else if(pData->byType == MAPOBJ_ENEMY)
			{//镖车等跟随物的坐标同步
				MObjSMark* pMark	= GetMark(pData->dwID);
				if(pMark)
				{
					pMark->nPosX	= pData->wPosX;
					pMark->nPosY	= pData->wPosY;
				}
			}
		}
		return true;
	case MS_OBJINFO:
		{
			SET_DATA(pData, MAPOBJ, MS_OBJINFO, pPacket);
			if(pData->byType == MAPOBJ_PLAYER)
			{
				if(m_pPlayer && m_pPlayer->dwID == pData->dwID)
				{
					m_pPlayer->xParam.nHp	= pData->nHp;
					m_pPlayer->xParam.nMp	= pData->nMp;
					m_pUser->OnValChange();
				}
			}
			else if(pData->byType == MAPOBJ_SLAVE)
			{
				if(m_pSlave && m_pSlave->dwID	== pData->dwID)
				{
					m_pSlave->xParam.nHp	= pData->nHp;
					if(m_pUser->m_pSlaveCenter->m_pActiveSlave && m_pUser->m_pSlaveCenter->m_pActiveSlave->m_dwMapID == m_pSlave->dwID)
					{
						m_pUser->m_pSlaveCenter->m_pActiveSlave->SetHp(m_pSlave->xParam.nHp);
						if(m_pUser->m_pSlaveCenter->m_pActiveSlave->m_nHP <= 0 && m_pUser->m_pSlaveCenter->m_dwReviveTick == 0)
						{
							int nPoint = rand()%5+1;
							m_pUser->m_pSlaveCenter->m_pActiveSlave->m_nLifePoint -= nPoint;
							m_pUser->m_pSlaveCenter->m_pActiveSlave->m_ifChange = true;
							if(m_pUser->m_pSlaveCenter->m_pActiveSlave->m_nLifePoint <= 0)
							{
								m_pUser->m_pSlaveCenter->m_pActiveSlave->m_nLifePoint = 0;
								m_pUser->m_pSlaveCenter->m_pActiveSlave->m_ifActive = false;
								m_pUser->m_pSlaveCenter->m_pActiveSlave = NULL;
							//	m_pUser->m_pSlaveCenter->SetMapObj();
								m_pUser->m_pSlaveCenter->SendActive();
							}
							else
								m_pUser->m_pSlaveCenter->m_dwReviveTick = GetTickCount()+10000;
// 							m_pUser->m_pSlaveCenter->m_pActiveSlave->m_ifActive = false;
// 							m_pUser->m_pSlaveCenter->m_pActiveSlave = NULL;
// 							m_pUser->m_pSlaveCenter->SetMapObj();
// 							m_pUser->m_pSlaveCenter->SendActive();
// 							m_pUser->SendPopMsg("您的宠物已经死亡!");
						}
					}
				}
			}
			m_wFuryVal	= pData->wSp;
		}
		return true;
	case MS_OBJKILL:
		{
			SET_DATA(pData, MAPOBJ, MS_OBJKILL, pPacket);
			if(pData->byTargetT == MAPOBJ_ENEMY)
			{
				//EnemyBase* pBase	= EnemyBaseCfg::GetInstance().GetEnemyBase(pData->dwTargetID);
				//if(pBase)
				m_pUser->m_pQuestCenter->CheckCtrlDead(pData->dwTargetID);
				m_pUser->KillEnemyEvent(pData->wBaseID,pData->dwTargetID);
			}
			if(pData->byTargetT == MAPOBJ_PLAYER)
			{
				m_pUser->KillPlayerEvent(pData->dwTargetID);
			}
		}
		return true;
	case MS_OBJBEKILL:
		{
			SET_DATA(pData, MAPOBJ, MS_OBJBEKILL, pPacket);
			if(pData->byType == MAPOBJ_ENEMY)
			{
				m_pUser->m_pQuestCenter->CheckCtrlDead(pData->dwID);
				if(m_pUser->m_pMapCopy && m_pUser->m_nMIndex >= 1000)
				{
					for (int i = 0; i < m_pUser->m_pMapCopy->m_pListHelper->Count; i++)
					{
						MapHelpObj* pHelp	= (MapHelpObj*)m_pUser->m_pMapCopy->m_pListHelper->Items[i];
						if(pHelp->dwHelpID == pData->dwID)
						{
							pHelp->dwHelpID	= 0;
							pHelp->dwHelpTick	= GetTickCount();
							m_pUser->SendWarnMsg("你的助战队友将在10秒后复活");
						}
					}
				}
			}
			else if(pData->byType == MAPOBJ_PLAYER && pData->dwID == m_pUser->m_dwUin)
			{
				m_pUser->m_pUserExpCenter->CheckWhenDie();
				m_pUser->OnPlayerDeath();
			}
		}
		return true;
		//	MS_OBJINFO,			//离开地图或下线的时候需要同步对象状态 (血量蓝量,特殊状态)
	case MS_OBJBEHIT:
		{
			SET_DATA(pData, MAPOBJ, MS_OBJBEHIT, pPacket);
			m_pUser->OnPlayerBeHit();
		}
		return true;
	case MS_SETSKILLCD:
		{
			SET_DATA(pData, MAPOBJ, MS_SETSKILLCD, pPacket);
			DWORD	dwTick		= CMTime::GetCurTickCount();
			for (int i = 0; i < m_pPlayer->pListSkill->Count; i++)
			{
				_NetSkillInfo*	pNS	= (_NetSkillInfo*)m_pPlayer->pListSkill->Items[i];
				if(pNS->wSkill == pData->wSkillID)
				{
					pNS->dwCDTick	= dwTick+pData->dwTickCD/1000;
					break;
				}
			}
		}
		return true;
	case MS_SETREVIVE:
		{
			SET_DATA(pData, MAPOBJ, MS_SETREVIVE, pPacket);
			m_pUser->SetRevivePos(pData->wRM,pData->wRX,pData->wRY);
		}
		return true;
	case MS_SYNCSTATE:
		{
			SET_DATA(pData, MAPOBJ, MS_SYNCSTATE, pPacket);

			if(m_pPlayer)
			{
				for(int i = 0; i < m_pPlayer->pListState->Count; i++)
				{
					_netStateInfo* pState	= (_netStateInfo*)m_pPlayer->pListState->Items[i];
					if(pState->wStateID	== pData->xState.wStateID)
					{
						if(pData->xState.dwTickTime	== 0)
						{
							m_pPlayer->pListState->Delete(i);
							delete pState;
							return true;
						}
						else
						{
							pState->bLvl	= pData->xState.bLvl;
							if(pData->xState.dwTickTime == -1)
								pState->dwTickTime	= 0;
							else
								pState->dwTickTime	= pData->xState.dwTickTime + GetTickCount();
							return true;
						}
					}
				}

				if(pData->xState.dwTickTime != 0)
				{
					_netStateInfo* pState	= new _netStateInfo();
					pState->wStateID	= pData->xState.wStateID;
					pState->bLvl		= pData->xState.bLvl;
					if(pData->xState.dwTickTime == -1)
						pState->dwTickTime	= 0;
					else
						pState->dwTickTime	= pData->xState.dwTickTime + GetTickCount();
					m_pPlayer->pListState->Add(pState);
				}
			}
		}
		return true;
	}
	return false;
}

void	UserMObjCenter::Update()
{
	if(m_pPlayer && m_pPlayer->bSkinChged)
	{
		_syncObjParams(m_pPlayer);
		m_pPlayer->bParamChged	= false;
		m_pPlayer->bSkinChged	= false;
		m_pUser->CalcGroupEquipState();
	}
}

void UserMObjCenter::Update25()
{
	if(m_pPlayer && m_pPlayer->bParamChged)
	{
		_syncObjParams(m_pPlayer);
		m_pPlayer->bParamChged	= false;
		m_pPlayer->bSkinChged	= false;
		m_pUser->CalcGroupEquipState();
	}
	if(m_pPlayer && m_pPlayer->bSkillChged)
	{
		_syncObjSkill(m_pPlayer);
		m_pPlayer->bSkillChged	= false;
	}

	if(m_pPlayer && m_pPlayer->bTitleChged)
	{
		_syncObjTitle(m_pPlayer);
		m_pPlayer->bTitleChged	= false;
	}

	if(m_pPlayer && m_pPlayer->bMarryTitleChgd)
	{
		_syncObjMarryTitle(m_pPlayer);
		m_pPlayer->bMarryTitleChgd	= false;
	}

	if(m_pPlayer && m_pPlayer->bMarryEffChgd)
	{
		_syncObjMarryEff(m_pPlayer);
		m_pPlayer->bMarryEffChgd	= false;
	}

	if(m_pSlave && m_pSlave->bParamChged)
	{
		_syncObjParams(m_pSlave);
		m_pSlave->bParamChged	= false;
	}
	if(m_pSlave && m_pSlave->bSkillChged)
	{
		_syncObjSkill(m_pSlave);
		m_pSlave->bSkillChged	= false;
	}

	if(m_pSlave && m_pSlave->bTitleChged)
	{
		_syncObjTitle(m_pSlave);
		m_pSlave->bTitleChged	= false;
	}
}

void UserMObjCenter::_syncObjParams( MapSObject* pMObj )
{
	if(m_pNetSrvLink == NULL)
		return;
	BuildPacketEx(pPacket, MAPOBJ,GS_SYNCPARAM,buf,2048);
	SET_DATA(pSend, MAPOBJ,GS_SYNCPARAM, pPacket);
	pSend->byType		= pMObj->byType;
	pSend->dwID			= pMObj->dwID;
	pSend->byLv			= pMObj->byLv;
	memcpy(&pSend->xParam,&pMObj->xParam,sizeof(MParam));
	memcpy(pSend->wSkin,pMObj->wSkin,6);

	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
}

void UserMObjCenter::AddState( MapSObject* pMObj,WORD wState,bool bRemove )
{
	if(m_pNetSrvLink == NULL)
		return;
	BuildPacketEx(pPacket, MAPOBJ,GS_ADDSTATE,buf,1024);
	SET_DATA(pSend, MAPOBJ,GS_ADDSTATE, pPacket);
	pSend->byType		= pMObj->byType;
	pSend->dwID			= pMObj->dwID;
	pSend->wState		= wState;
	pSend->bRemove		= bRemove;
	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
}

void UserMObjCenter::ClearRideLock(MapSObject* pMObj)
{
	BuildPacketEx(pPacket, MAPOBJ,GS_CLEAN_RIDE_LOCK,buf,1024);
	SET_DATA(pSend, MAPOBJ,GS_CLEAN_RIDE_LOCK, pPacket);
	pSend->byType		= pMObj->byType;
	pSend->dwID			= pMObj->dwID;
	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
}

void UserMObjCenter::ObjChgPos( MapSObject* pMObj )
{
	if(m_pNetSrvLink == NULL)
		return;
	BuildPacketEx(pPacket, MAPOBJ,GS_CHGPOS,buf,1024);
	SET_DATA(pSend, MAPOBJ,GS_CHGPOS, pPacket);
	pSend->byType		= pMObj->byType;
	pSend->dwID			= pMObj->dwID;
	pSend->wX			= pMObj->nPosX;
	pSend->wY			= pMObj->nPosY;
	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
}

void UserMObjCenter::_syncObjTitle(MapSObject* pMObj)
{
	if(m_pNetSrvLink == NULL)
		return;
	BuildPacketEx(pPacket, MAPOBJ,GS_SYNCTITLE,buf,2048);
	SET_DATA(pSend, MAPOBJ,GS_SYNCTITLE, pPacket);
	strcpy(pSend->szTitle,pMObj->szTitle);
	pSend->byType = pMObj->byType;
	pSend->dwID = pMObj->dwID;

	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
}

void UserMObjCenter::_syncObjMarryTitle(MapSObject* pMObj)
{
	if(m_pNetSrvLink == NULL)
		return;
	BuildPacketEx(pPacket, MAPOBJ,GS_SYNCMARRYTITLE,buf,2048);
	SET_DATA(pSend, MAPOBJ,GS_SYNCMARRYTITLE, pPacket);
	strcpy(pSend->szTitle,pMObj->szMarryTitle);
	pSend->byType = pMObj->byType;
	pSend->dwID = pMObj->dwID;

	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
}

void	UserMObjCenter::ChgWeather(int nw,int nSec)
{
	if(m_pNetSrvLink == NULL)
		return;
	BuildPacketEx(pPacket, MAPOBJ,GS_CHGWEATHER_TIME,buf,2048);
	SET_DATA(pSend, MAPOBJ,GS_CHGWEATHER_TIME, pPacket);
	pSend->nSec = nSec;
	pSend->byWeather = nw;

	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
}

void	UserMObjCenter::SetMyCtrlLock(bool ifLock,bool ifHide,BYTE byType,int nID)
{
	if(m_pNetSrvLink == NULL)
		return;
	BuildPacketEx(pPacket, MAPOBJ,GS_SETLOCKVIEW,buf,2048);
	SET_DATA(pSend, MAPOBJ,GS_SETLOCKVIEW, pPacket);
	pSend->bLock = (ifLock?1:0);
	pSend->bHideSelf = (ifHide?1:0);
	pSend->byType = MAPOBJ_PLAYER;
	pSend->dwID = m_dwUin;
	pSend->byObjType = byType;
	pSend->dwObjID = nID;

	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
}

void	UserMObjCenter::SetEnemyRunPath(int nID,int nCnt,POINT* szPoint)
{
	if(m_pNetSrvLink == NULL)
		return;
	BuildPacketEx(pPacket, MAPOBJ,GS_SETRUNPATH,buf,2048);
	SET_DATA(pSend, MAPOBJ,GS_SETRUNPATH, pPacket);
	pSend->dwID	= nID;
	pSend->wPtCnt = nCnt;
	memcpy(pSend->szPt,szPoint,nCnt*sizeof(POINT));
	pPacket->wSize += nCnt*sizeof(POINT);

	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
}

void	UserMObjCenter::SetEnemyRunGroup(int nID,int nPos,int nGroup)
{
	if(m_pNetSrvLink == NULL)
		return;
	BuildPacketEx(pPacket, MAPOBJ,GS_SETBATTLEGROUP,buf,2048);
	SET_DATA(pSend, MAPOBJ,GS_SETBATTLEGROUP, pPacket);
	pSend->dwID	= nID;
	pSend->byPos = nPos;
	pSend->dwMaster = nGroup;

	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
}	

void UserMObjCenter::_syncObjMarryEff(MapSObject* pMObj)
{
	if(m_pNetSrvLink == NULL)
		return;
	BuildPacketEx(pPacket, MAPOBJ,GS_SYNCMARRYEFF,buf,2048);
	SET_DATA(pSend, MAPOBJ,GS_SYNCMARRYEFF, pPacket);
	pSend->nEff = pMObj->nMarryEff;
	pSend->byType = pMObj->byType;
	pSend->dwID = pMObj->dwID;

	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
}

void UserMObjCenter::_syncObjViplv(MapSObject* pMObj)
{
	if(m_pNetSrvLink == NULL)
		return;
	BuildPacketEx(pPacket, MAPOBJ,GS_SYNCVIPLV,buf,2048);
	SET_DATA(pSend, MAPOBJ,GS_SYNCVIPLV, pPacket);
	pSend->byViplv = pMObj->byViplv;
	pSend->byType = pMObj->byType;
	pSend->dwID = pMObj->dwID;

	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
}

void UserMObjCenter::_syncObjSkill( MapSObject* pMObj )
{
	if(m_pNetSrvLink == NULL)
		return;
	BuildPacketEx(pPacket, MAPOBJ,GS_SYNCSKILL,buf,2048);
	SET_DATA(pSend, MAPOBJ,GS_SYNCSKILL, pPacket);
	pSend->byType		= pMObj->byType;
	pSend->dwID			= pMObj->dwID;

	pSend->bySCnt		= pMObj->pListSkill->Count;
	pPacket->wSize		+= (pSend->bySCnt-1)*sizeof(_NetSkillInfo);
	DWORD	dwTick		= CMTime::GetCurTickCount();
	for (int i = 0; i < pMObj->pListSkill->Count; i++)
	{
		_NetSkillInfo*	pNS	= (_NetSkillInfo*)pMObj->pListSkill->Items[i];
		memcpy(&(pSend->szSkill[i]),pNS,sizeof(_NetSkillInfo));
		if(pNS->dwCDTick > dwTick)
			pSend->szSkill[i].dwCDTick	= (pNS->dwCDTick-dwTick)*1000;
		else
			pSend->szSkill[i].dwCDTick	= 0;
	}

	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
}

void UserMObjCenter::HealthHpMp( MapSObject* pMObj,int nHp, int nMp,bool bRevive )
{
	if(m_pNetSrvLink == NULL)
		return;
	BuildPacketEx(pPacket, MAPOBJ,GS_HEALTHHPMP,buf,2048);
	SET_DATA(pSend, MAPOBJ,GS_HEALTHHPMP, pPacket);
	pSend->byType		= pMObj->byType;
	pSend->dwID			= pMObj->dwID;
	pSend->nAddHp		= nHp;
	pSend->nAddMp		= nMp;
	pSend->bRevive		= bRevive;
	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
}

void UserMObjCenter::ClearMarks()
{
	if(!m_pNetSrvLink)
		return;
	while(m_pListMark->Count)
	{
		MObjSMark* pMark	= (MObjSMark*)m_pListMark->Delete(0);
		BuildPacketEx(pPacket, MAPOBJ,GS_REMOVEOBJ,buf,1024);
		SET_DATA(pSend, MAPOBJ,GS_REMOVEOBJ, pPacket);
		pSend->byType		= MAPOBJ_ENEMY;
		pSend->dwID			= pMark->dwID;
		g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);

		delete pMark;
	}
}

bool UserMObjCenter::RemoveMark( DWORD dwID )
{
	if(m_pNetSrvLink == NULL)
		return false;
	for (int i = 0; i < m_pListMark->Count; i++)
	{
		MObjSMark* pMark	= (MObjSMark*)m_pListMark->Items[i];

		if(pMark->dwID	== dwID)
		{
			BuildPacketEx(pPacket, MAPOBJ,GS_REMOVEOBJ,buf,1024);
			SET_DATA(pSend, MAPOBJ,GS_REMOVEOBJ, pPacket);
			pSend->byType		= MAPOBJ_ENEMY;
			pSend->dwID			= pMark->dwID;
			g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
			m_pListMark->Delete(i);
			delete pMark;
			return true;
		}
	}
	return false;
}

MObjSMark* UserMObjCenter::GetMark( DWORD dwID )
{
	for (int i = 0; i < m_pListMark->Count; i++)
	{
		MObjSMark* pMark	= (MObjSMark*)m_pListMark->Items[i];

		if(pMark->dwID	== dwID)
		{
			return pMark;
		}
	}
	return NULL;
}

void UserMObjCenter::_syncObjState( MapSObject* pObj )
{
	BuildPacketEx(pPacket, MAPOBJ,GS_INITSTATE,buf,1024);
	SET_DATA(pSend, MAPOBJ,GS_INITSTATE, pPacket);
	pSend->bType	= pObj->byType;
	pSend->dwID		= pObj->dwID;
	pSend->wCnt	= 0;
	for (int i = 0; i < pObj->pListState->Count; i++)
	{
		_netStateInfo* pState	= (_netStateInfo*)pObj->pListState->Items[i];
		int	nRmTick	= pState->dwTickTime - GetTickCount();

		if(pState->dwTickTime==0)
		{
			pSend->xState[pSend->wCnt].wStateID	= pState->wStateID;
			pSend->xState[pSend->wCnt].bLvl		= pState->bLvl;
			pSend->xState[pSend->wCnt].dwTickTime	= 0;
			pSend->wCnt++;
		}
		else if(nRmTick > 1000)
		{
			pSend->xState[pSend->wCnt].wStateID	= pState->wStateID;
			pSend->xState[pSend->wCnt].bLvl		= pState->bLvl;
			pSend->xState[pSend->wCnt].dwTickTime	= nRmTick;
			pSend->wCnt++;
		}
		else
		{
			pObj->pListState->Delete(i);
			i--;
			delete pState;
		}
	}
	if(pSend->wCnt == 0)
		return;
	pPacket->wSize	+= pSend->wCnt*sizeof(_netStateInfo);
	g_pNetCenter->SendMSvrPacket(pPacket,m_dwMapIndex,m_pNetSrvLink->pMapSvr);
}

bool UserMObjCenter::CheckState( MapSObject* pMObj,WORD wState )
{
	for (int i = 0; i < pMObj->pListState->Count; i++)
	{
		_netStateInfo* pState	= (_netStateInfo*)pMObj->pListState->Items[i];
		if (pState->wStateID == wState)
		{
			return true;
		}
	}
	return false;
}
