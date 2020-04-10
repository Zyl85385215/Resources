#include "stdafx.h"

#include "MapPlate.h"
#include "ZNetPacket.h"
#include "ios_mapcmd_packet.h"
#include "ControlObject.h"
#include "MapManager.h"
#include "EnemyConfig.h"
#include "StateCenter.h"

//--------------------------------------------------------------------------
MapPlate::MapPlate()
{
	m_pListEnemy		= xnList::Create();
	m_pListSlave		= xnList::Create();
	m_pListPlayer		= xnList::Create();
	m_pListEff			= xnList::Create();
	m_pListViews		= xnList::Create();
	m_pListRobot		= xnList::Create();
	m_nRestSec			= 0;
}

MapPlate::~MapPlate()
{
	m_pListEnemy->Free();
	m_pListSlave->Free();
	m_pListPlayer->Free();
	m_pListEff->Free();
	m_pListViews->Free();
	m_pListRobot->Free();
}

bool MapPlate::EnterPlate( MapAIObj* pObj )
{
	xnList*	pList	= _getListByType(pObj->byType);
	if(pList)
	{
		if(pList->IndexOf(pObj) != -1)
			return false;
		pList->Add(pObj);
	}
	if(pObj->pRunPlate)
	{
		xnList*	pListNewView	= xnList::Create();
		xnList*	pListOutView	= xnList::Create();

		m_pListViews->CheckMyOnly(pObj->pRunPlate->m_pListViews,pListNewView);
		pObj->pRunPlate->m_pListViews->CheckMyOnly(m_pListViews,pListOutView);

		_sendSyncPacketList(pObj,pListNewView);
		_sendOutPacketList(pObj,pListOutView);
		pListNewView->Free();
		pListOutView->Free();
	}
	else
	{
		_sendSyncPacketList(pObj,m_pListViews);
	}
	pObj->pRunPlate	= this;
	pObj->wMPX		= m_wMX;
	pObj->wMPY		= m_wMY;
	return true;
}

bool MapPlate::LeavePlate( MapAIObj* pObj,bool bDisapear )
{
	xnList*	pList	= _getListByType(pObj->byType);
	if(pList)
		pList->Remove(pObj);

	if(bDisapear)
	{
		pObj->pRunPlate	= NULL;
		_sendOutPacketList(pObj,m_pListViews);
	}

	return true;
}

void MapPlate::AddCtrlView( ControlObj* pObj )
{
	if(m_pListViews->IndexOf(pObj) != -1)
		return;
	
	m_pListViews->Add(pObj);

	_sendSyncListPacket(m_pListPlayer,pObj);
	_sendSyncListPacket(m_pListSlave,pObj);
	_sendSyncListPacket(m_pListEnemy,pObj);
	_sendSyncListPacket(m_pListRobot,pObj);
}

void MapPlate::RemoveCtrlView( ControlObj* pObj )
{
	m_pListViews->Remove(pObj);

	_sendOutListPacket(m_pListPlayer,pObj);
	_sendOutListPacket(m_pListSlave,pObj);
	_sendOutListPacket(m_pListEnemy,pObj);
	_sendOutListPacket(m_pListRobot,pObj);
}

xnList* MapPlate::_getListByType( BYTE bType )
{
	xnList*	pList	= NULL;
	switch (bType)
	{
	case MAPOBJ_PLAYER:
		pList	= m_pListPlayer;
		break;
	case MAPOBJ_ENEMY:
		pList	= m_pListEnemy;
		break;
	case MAPOBJ_SLAVE:
		pList	= m_pListSlave;
		break;
	case MAPOBJ_ROBOT:
		pList	= m_pListRobot;
		break;
	}
	return pList;
}

void MapPlate::_sendSyncPacketList( MapAIObj* pObj, xnList* pList )
{
	if(pList->Count == 0)
		return;
	char buffer[1024] = {0};
	ZPacket *	packet		= _getSyncPacket(pObj,buffer);

	for (int i = 0; i < pList->Count; i++)
	{
		ControlObj* pCtrl	= (ControlObj*)pList->Items[i];
		pCtrl->SendPacket(packet);
	}
}

void MapPlate::_sendOutPacketList( MapAIObj* pObj, xnList* pList )
{
	BuildPacketEx(packet, MAPCMD, MS_OBJOUT, buffer,256);
	SET_DATA(pSend,MAPCMD, MS_OBJOUT,packet);
	pSend->byType	= pObj->byType;
	pSend->dwID		= pObj->dwUin;
	pSend->dwMIndex	= m_pMap->m_dwMapIndex;
	for (int i = 0; i < pList->Count; i++)
	{
		ControlObj* pCtrl	= (ControlObj*)pList->Items[i];
		pCtrl->SendPacket(packet);
	}
}

void MapPlate::_sendSyncListPacket( xnList* pList, ControlObj* pCtrl )
{
	for (int i = 0; i < pList->Count; i++)
	{
		MapAIObj* pObj	= (MapAIObj*)pList->Items[i];
		char buffer[1024] = {0};
		if(pObj->bDisappear == false)
		{
			if((m_pMap->m_dwMapIndex==103 || m_pMap->m_dwMapIndex==203) && pObj->byType == MAPOBJ_PLAYER && pObj->pCtrl != pCtrl && GetTickCount()-pObj->m_dwTickUnActive>30000)
				continue;		//临时屏蔽新手村的刷榜玩家
			ZPacket *	packet		= _getSyncPacket(pObj,buffer);
			pCtrl->SendPacket(packet);
		}

	}
}

void MapPlate::_sendOutListPacket( xnList* pList, ControlObj* pCtrl )
{
	BuildPacketEx(packet, MAPCMD, MS_OBJOUT, buffer,256);
	SET_DATA(pSend,MAPCMD, MS_OBJOUT,packet);
	for (int i = 0; i < pList->Count; i++)
	{
		MapAIObj* pObj	= (MapAIObj*)pList->Items[i];

		pSend->byType	= pObj->byType;
		pSend->dwID		= pObj->dwUin;
		pSend->dwMIndex	= m_pMap->m_dwMapIndex;
		pCtrl->SendPacket(packet);
	}
}

ZPacket* MapPlate::_getSyncPacket( MapAIObj* pObj , char* buffer)
{
	ZPacket* packet	= NULL;
	xnList*		pLTmp	= xnList::Create();
	pObj->pStateCenter->FillClientStates(pLTmp);

	switch (pObj->byType)
	{
	case MAPOBJ_PLAYER:
	case MAPOBJ_ROBOT:
		{
			BuildPacket(packet, MAPCMD, MS_USERSYNC, buffer);
			SET_DATA(pSend,MAPCMD, MS_USERSYNC,packet);
			strcpy(pSend->szName,pObj->szName);
			strcpy(pSend->szTitle,pObj->szTitle);
			strcpy(pSend->szMarryTitle,pObj->szMarryTitle);
			pSend->dwUin	= pObj->dwUin;
			pSend->nHp		= pObj->xParam.nHp;
			pSend->nHpMax	= pObj->xParam.nHpMax;
			pSend->nMarryEff= pObj->nMarryEff;
			pSend->wPosX	= pObj->nX;
			pSend->wPosY	= pObj->nY;
			pSend->wSpeed	= pObj->xParam.nMSpeed;
			memcpy(pSend->wSkin,pObj->wSkin,6);
			pSend->bCmd		= pObj->bCmd;
			pSend->dwCmdVal	= pObj->dwCmdVal;
			pSend->bLv		= pObj->bLv;
			pSend->bJob		= pObj->byJob;
			pSend->bSex		= pObj->bySex;
			pSend->bForce	= pObj->bForce;
			pSend->byCountry= pObj->byCountry;
			pSend->byViplv	= pObj->byViplv;

			pSend->byStateCnt	= pLTmp->Count;
			packet->wSize		+= (pLTmp->Count-1)*2;
			for (int s = 0; s < pLTmp->Count; s++)
			{
				pSend->wStateID[s]	= (int)pLTmp->Items[s];
			}
		}
		break;
	case MAPOBJ_ENEMY:
		{
			if (pObj->pBase->nID >= 8002&&pObj->pBase->nID <= 8005)
			{
				BuildPacket(packet, MAPCMD, MS_ENEMYSYNCEX, buffer);
				SET_DATA(pSend,MAPCMD, MS_ENEMYSYNCEX,packet);
				pSend->dwID		= pObj->dwUin;
				pSend->wBaseID  = pObj->pBase->nID;
				pSend->szSkin[0]	= pObj->wSkin[0];
				pSend->szSkin[1]	= pObj->wSkin[1];
				pSend->szSkin[2]	= pObj->wSkin[2];
				pSend->byLv			= pObj->bLv;
				pSend->nHp		= pObj->xParam.nHp;
				pSend->nMaxHp	= pObj->xParam.nHpMax;
				pSend->wPosX	= pObj->nX;
				pSend->wPosY	= pObj->nY;
				
				pSend->bCmd		= pObj->bCmd;
				if(pSend->bCmd == MCMD_ATTACK)
					pSend->bCmd	= MCMD_RUN;
				pSend->dwCmdVal	= pObj->dwCmdVal;

				pSend->wSpeed	= pObj->xParam.nMSpeed;
				pSend->bForce	= pObj->bForce;
				if(pObj->pCtrl)
					pSend->dwMastID	= pObj->pCtrl->m_dwUin;
				else
					pSend->dwMastID	= 0;

				pSend->byStateCnt	= pLTmp->Count;
				packet->wSize		+= (pLTmp->Count-1)*2;

				for (int s = 0; s < pSend->byStateCnt; s++)
				{
					pSend->wStateID[s]	= (int)pLTmp->Items[s];
				}
			}
			else
			{
				BuildPacket(packet, MAPCMD, MS_ENEMYSYNC, buffer);
				SET_DATA(pSend,MAPCMD, MS_ENEMYSYNC,packet);
				strcpy(pSend->szTitle,pObj->szTitle);
				pSend->dwID		= pObj->dwUin;
				pSend->wBaseID	= pObj->wSkin[0];
				pSend->nHp		= pObj->xParam.nHp;
				pSend->wPosX	= pObj->nX;
				pSend->wPosY	= pObj->nY;
				pSend->bCmd		= pObj->bCmd;
				if(pSend->bCmd == MCMD_ATTACK)
					pSend->bCmd	= MCMD_RUN;
				pSend->dwCmdVal	= pObj->dwCmdVal;
				pSend->wSpeed	= pObj->xParam.nMSpeed;
				pSend->bForce	= pObj->bForce;

				if(pObj->dwBattelGroup)
					pSend->bRandMove	= 100+pObj->byBattelPos;	//阵列
				else if(pObj->pBornInfo && (pObj->pBornInfo->bPtRunCnt || pObj->pBornInfo->byBattleGroup))
					pSend->bRandMove	= 0;
				else
					pSend->bRandMove	= 1;

				if(pObj->dwBattelGroup)
					pSend->dwMastID	= pObj->dwBattelGroup;
				else if(pObj->pCtrl)
					pSend->dwMastID	= pObj->pCtrl->m_dwUin;
				else
					pSend->dwMastID	= 0;

				pSend->byStateCnt	= 0;

				pSend->byNewSCnt	= pLTmp->Count;
				packet->wSize		+= pLTmp->Count*2;

				for (int s = 0; s < pSend->byNewSCnt; s++)
				{
					pSend->wNewStateID[s]	= (int)pLTmp->Items[s];
				}
			}


		}
		break;
	//case MAPOBJ_NPC:
	//	{
	//		BuildPacket(packet, MAPCMD, MS_NPCSYNC, buffer);
	//		SET_DATA(pSend,MAPCMD, MS_NPCSYNC,packet);
	//		pSend->dwID		= pObj->dwUin;
	//		pSend->wBaseID	= pObj->wSkin[0];
	//		pSend->wPosX	= pObj->nX;
	//		pSend->wPosY	= pObj->nY;
	//	}
	//	break;
	case MAPOBJ_SLAVE:
		{
			BuildPacket(packet, MAPCMD, MS_SLAVESYNC, buffer);
			SET_DATA(pSend,MAPCMD, MS_SLAVESYNC,packet);
			strcpy(pSend->szName,pObj->szName);
			pSend->dwID		= pObj->dwUin;
			pSend->wSkin	= pObj->wSkin[0];
			pSend->nHpMax	= pObj->xParam.nHpMax;
			pSend->wPosX	= pObj->nX;
			pSend->wPosY	= pObj->nY;
			pSend->bCmd		= pObj->bCmd;
			pSend->dwCmdVal	= pObj->dwCmdVal;
			pSend->wSpeed	= pObj->xParam.nMSpeed;
			pSend->nHp		= pObj->xParam.nHp;
			pSend->nHpMax	= pObj->xParam.nHpMax;
			pSend->bLv		= pObj->bLv;
			pSend->bForce	= pObj->bForce;
			pSend->nMarryEff= pObj->nMarryEff;
			if(pObj->pCtrl)
				pSend->dwMastID	= pObj->pCtrl->m_dwUin;
			else
				pSend->dwMastID	= 0;

			pSend->byStateCnt	= pLTmp->Count;
			packet->wSize		+= (pLTmp->Count-1)*2;
			for (int s = 0; s < pLTmp->Count; s++)
			{
				pSend->wStateID[s]	= (int)pLTmp->Items[s];
			}
		}
		break;
	}

	pLTmp->Free();
	return packet;
}

//void MapPlate::Update()
//{//驱动生命体行动
//	_checkListCmd(m_pListPlayer);
//	_checkListCmd(m_pListEnemy);
//	_checkListCmd(m_pListSlave);
//	_checkListCmd(m_pListRobot);
//}

void MapPlate::UpdateSec()
{
	//附近没人怪物不思考
	DWORD	dwCurTick	= GetTickCount();
	if(m_pMap->GetPlayerCnt() != 0)
	{
		//怪物思考策略, 宠物跟离线人物的策略由玩家客户端控制
		for (int i = 0; i < m_pListEnemy->Count; i++)
		{
			MapAIObj* pEnemy	= (MapAIObj*)m_pListEnemy->Items[i];
			if(m_nRestSec)
			{
				pEnemy->dwTickCmd	+= m_nRestSec*1000;
				if(pEnemy->dwTickCmd > dwCurTick)
					pEnemy->dwTickCmd	= dwCurTick;
			}
			if(pEnemy->pCtrl)
				pEnemy->pCtrl->GetCtrlAICommand(pEnemy);
			else
				m_pMap->m_pAICenter->GetEnemyAICommand(pEnemy);
		}

		for (int i = 0; i < m_pListSlave->Count; i++)
		{
			MapAIObj* pSlave	= (MapAIObj*)m_pListSlave->Items[i];
			
			if(pSlave->pCtrl)
				pSlave->pCtrl->GetCtrlAICommand(pSlave);
		}

		m_nRestSec	= 0;
	}
	else
	{
		
		m_nRestSec	++;
	}
	//触发区域效果
	for (int i = 0; i < m_pListRobot->Count; i++)
	{
		MapAIObj* pRobot	= (MapAIObj*)m_pListRobot->Items[i];

		m_pMap->m_pAICenter->GetRobotAICommand(pRobot);
	}
}

void MapPlate::SendPacketOutCtrl( ZPacket* pPacket,ControlObj* pC )
{//不发自己,照成伤害的血量包也不发自己
	for (int i = 0; i < m_pListViews->Count; i++)
	{
		ControlObj* pCtrl	= (ControlObj*)m_pListViews->Items[i];
		if(pCtrl != pC)
			pCtrl->SendPacket(pPacket);
	}
}

void MapPlate::SendPacketExCtrl( ZPacket* pPacket,ControlObj* pC )
{//不发自己,照成伤害的血量包也不发自己
	for (int i = 0; i < m_pListViews->Count; i++)
	{
		ControlObj* pCtrl	= (ControlObj*)m_pListViews->Items[i];
		if(pCtrl != pC)
			pCtrl->SendPacket(pPacket);
	}
	if(pC)
		pC->SendPacket(pPacket);
}

void MapPlate::SendOutPlatePacket( ZPacket* pPacket,MapPlate* pPlate )
{
	xnList*	pList	= xnList::Create();
	m_pListViews->CheckMyOnly(pPlate->m_pListViews,pList);
	for (int i = 0; i < pList->Count; i++)
	{
		ControlObj* pCtrl	= (ControlObj*)pList->Items[i];
		pCtrl->SendPacket(pPacket);
	}
	pList->Free();
}

void MapPlate::_checkListCmd( xnList* pList )
{
	for (int i = 0; i < pList->Count; i++)
	{
		MapAIObj* pObj	= (MapAIObj*)pList->Items[i];
		pObj->RunCmd();
		pObj->Update();
		WORD wMPX	= pObj->nX/MAPPLATE_SIZE;
		WORD wMPY	= pObj->nY/MAPPLATE_SIZE;
		if(wMPX != pObj->wMPX || wMPY != pObj->wMPY)
		{
			m_pMap->m_szMapPlates[pObj->wMPY*m_pMap->m_wMaxPX+pObj->wMPX].LeavePlate(pObj);
			m_pMap->m_szMapPlates[wMPY*m_pMap->m_wMaxPX+wMPX].EnterPlate(pObj);
		}
	}
}

void	MapPlate::KillEnemyByID(WORD wEnemyID)
{
	for (int i = 0; i < m_pListEnemy->Count; i++)
	{
		MapAIObj*	pObj	= (MapAIObj*)m_pListEnemy->Items[i];
		if(pObj&&pObj->pCtrl == NULL&&pObj->pBase&&pObj->pBase->nID == wEnemyID)
		{
			pObj->ChgHp(-pObj->xParam.nHp);
		}
	}
}

void	MapPlate::KillEnemy(MapAIObj* pRqer)
{
	for (int i = 0; i < m_pListEnemy->Count; i++)
	{
		MapAIObj*	pObj	= (MapAIObj*)m_pListEnemy->Items[i];
		if(pRqer == NULL || pRqer->CheckCanLockAttack(pObj))	//检测是否有直线障碍
		{
			if(pObj->pCtrl == NULL)
			{
				pObj->ChgHp(-pObj->xParam.nHp);
				if(pRqer)
					m_pMap->ObjKillReward(pRqer,pObj);
				else if (pObj->byType == MAPOBJ_ENEMY &&pObj->pBase&&pObj->pBase->nID == 7043)//系统kill 巫妖发奖励
					m_pMap->LichKingKillBySystReward(pObj);
			}
		}
	}
}

MapAIObj* MapPlate::GetAtkTarget( MapAIObj* pRqer )
{//根据势力关系获取敌对目标
	for (int i = 0; i < m_pListPlayer->Count; i++)
	{
		MapAIObj*	pObj	= (MapAIObj*)m_pListPlayer->Items[i];
		if(pRqer->CheckCanLockAttack(pObj) && !m_pMap->CheckLineBlock(pRqer->nX,pRqer->nY,pObj->nX,pObj->nY))	//检测是否有直线障碍
		{
			return pObj;
		}
	}
	for (int i = 0; i < m_pListRobot->Count; i++)
	{
		MapAIObj*	pObj	= (MapAIObj*)m_pListRobot->Items[i];
		if(pRqer->CheckCanLockAttack(pObj) && !m_pMap->CheckLineBlock(pRqer->nX,pRqer->nY,pObj->nX,pObj->nY))	//检测是否有直线障碍
		{
			return pObj;
		}
	}

	for (int i = 0; i < m_pListSlave->Count; i++)
	{
		MapAIObj*	pObj	= (MapAIObj*)m_pListSlave->Items[i];
		if(pRqer->CheckCanLockAttack(pObj) && !m_pMap->CheckLineBlock(pRqer->nX,pRqer->nY,pObj->nX,pObj->nY))	//检测是否有直线障碍
		{
			return pObj;
		}
	}
	for (int i = 0; i < m_pListEnemy->Count; i++)
	{
		MapAIObj*	pObj	= (MapAIObj*)m_pListEnemy->Items[i];
		if(pRqer->CheckCanLockAttack(pObj) && !m_pMap->CheckLineBlock(pRqer->nX,pRqer->nY,pObj->nX,pObj->nY))	//检测是否有直线障碍
		{
			if(pObj->pBase && pObj->pBase->bType != 3 && pObj->pBase->bType != 5)
				return pObj;
		}
	}
	return NULL;
}

void MapPlate::FillAllObj( OUT xnList* pList )
{
	for(int j = 0; j < m_pListPlayer->Count; j++)
	{
		pList->Add(m_pListPlayer->Items[j]);
	}
	for(int j = 0; j < m_pListSlave->Count; j++)
	{
		pList->Add(m_pListSlave->Items[j]);
	}
	for(int j = 0; j < m_pListEnemy->Count; j++)
	{
		pList->Add(m_pListEnemy->Items[j]);
	}
	for(int j = 0; j < m_pListRobot->Count; j++)
	{
		pList->Add(m_pListRobot->Items[j]);
	}
}

void MapPlate::ResetMapForce()
{
	for (int t = 0; t < 10; t++)
	{
		xnList*	pList	= _getListByType(t);
		if(!pList)
			continue;
		for (int i = 0; i < pList->Count; i++)
		{
			MapAIObj*	pObj	= (MapAIObj*)pList->Items[i];
			WORD	wOldF	= pObj->bForce;
			m_pMap->CalcObjForce(pObj);
			if(wOldF != pObj->bForce)
			{
				BuildPacketEx(pSendPacket,MAPCMD,MS_OBJCHGFORCE,buf,256 );
				SET_DATA(pSend, MAPCMD,MS_OBJCHGFORCE, pSendPacket);
				pSend->bType	= pObj->byType;
				pSend->dwID		= pObj->dwUin;
				pSend->bForce	= pObj->bForce;
				SendPacketExCtrl(pSendPacket);
			}
		}
	}
}

void MapPlate::ChgPowLv( int nPow, int nLv )
{
	for (int t = 0; t < 10; t++)
	{
		xnList*	pList	= _getListByType(t);
		if(!pList)
			continue;
		for (int i = 0; i < pList->Count; i++)
		{
			MapAIObj*	pObj	= (MapAIObj*)pList->Items[i];
			if(pObj->byType == MAPOBJ_ENEMY && pObj->pCtrl == NULL)
			{
				pObj->xParam.nAim	= pObj->xParam.nAim*nPow/100;
				pObj->xParam.nAtkN	= pObj->xParam.nAtkN*nPow/100;
				pObj->xParam.nAtkW	= pObj->xParam.nAtkW*nPow/100;
				pObj->xParam.nDefN	= pObj->xParam.nDefN*nPow/100;
				pObj->xParam.nDefW	= pObj->xParam.nDefW*nPow/100;
				pObj->xParam.nCrit	= pObj->xParam.nCrit*nPow/100;
				pObj->xParam.nHp	= pObj->xParam.nHp*nPow/100;
				pObj->xParam.nHpMax	= pObj->xParam.nHpMax*nPow/100;
				pObj->xParam.nDodge	= pObj->xParam.nDodge*nPow/100;
				if(nLv)
					pObj->bLv	= nLv;
			}
		}
	}
}

