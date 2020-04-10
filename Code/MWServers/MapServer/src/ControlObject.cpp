#include "stdafx.h"

#include "ControlObject.h"
#include "MapAIObj.h"
#include "MapManager.h"
#include "ios_mapcmd_packet.h"
#include "ZNetServer.h"
#include "SkillCenter.h"
#include "EnemyConfig.h"

ControlObj::ControlObj(DWORD	dwK)
{
	m_dwKey		= dwK;
	m_nMIndex	= 0;
	m_pRunMap	= NULL;
	m_pMainAIObj	= NULL;
	m_dwUin		= 0;
	m_nSpVal	= 0;
	m_bForce	= 0;
	m_dwDamageTotal	= 0;
	m_dwHealthTotal	= 0;
	m_dwHitTotal	= 0;
	m_pListChild	= xnList::Create();
	m_wPX		= 10000;
	m_wPY		= 10000;
}

ControlObj::~ControlObj()
{
	if(m_pMainAIObj)
		m_pMainAIObj->pCtrl	= NULL;


	if(m_pRunMap)
	{
		xnList* pListOld	= xnList::Create();
		m_pRunMap->GetPlateRangeB(m_wPX,m_wPY,CTRLVIEW_WIDTH/2,CTRLVIEW_HEIGHT/2,pListOld);

		for (int i = 0; i < pListOld->Count; i++)
		{
			MapPlate* pMPlate	= (MapPlate*)pListOld->Items[i];
			pMPlate->RemoveCtrlView(this);
		}
		pListOld->Free();
	}


	for (int i = 0; i < m_pListChild->Count; i++)
	{
		MapAIObj* pObj	= (MapAIObj*)m_pListChild->Items[i];
		pObj->pCtrl	= NULL;
		if(m_pRunMap)
			m_pRunMap->AIObjLeaveMap(pObj);
	}

	m_pListChild->Free();
}

void ControlObj::RecvPacket( ZPacket* pPacket )
{
	if(pPacket->bCmdGroup	!= MAPCMD)
		return;
	if(m_pMainAIObj == NULL)
		return;
	switch(pPacket->bCmd)
	{
	case C_SETCMD:
		{
			SET_DATA(pData, MAPCMD, C_SETCMD,pPacket);
			if(pData->dwID	== m_pMainAIObj->dwUin && pData->bType	== m_pMainAIObj->byType)
			{//控制主要角色

			}
			else
			{

			}
		}
		break;
	case C_SETMOVE:
		{
			SET_DATA(pData, MAPCMD, C_SETMOVE,pPacket);
			m_pMainAIObj->SetCmd(MCMD_RUN,(pData->wY<<16)+pData->wX);
		}
		break;
	case C_SETATK:
		{
			SET_DATA(pData, MAPCMD, C_SETATK,pPacket);
			//m_pMainAIObj->SetCmd(MCMD_RUN,(pData->wY<<16)+pData->wX);
			MapAIObj* pObj	= m_pRunMap->GetMapObj(pData->bType,pData->dwID);
			if(pObj)
			{
				for (int i = 0; i < m_pListChild->Count; i++)
				{
					MapAIObj* pSlave	= (MapAIObj*)m_pListChild->Items[i];
					if(pSlave->bContrlType == 1)
					{
						if(pSlave->bCmd	!= MCMD_ATTACK)
							pSlave->SetCmd(MCMD_ATTACK,0,pObj);
					}
				}

				m_pMainAIObj->SetCmd(MCMD_ATTACK,pData->wSkill,pObj);
			}
			else
			{
				m_pMainAIObj->SetCmd(MCMD_ATTACK,pData->wSkill,NULL);
			}
		}
		break;
	case CHECK_MAPSVR_ALIVE:
		{
			SendPacket(pPacket);
		}
		break;
	case C_GET_PARAM:
		{
			if(m_pMainAIObj == NULL)
				return;
			BuildPacketEx(pSendPacket,MAPCMD,MS_USER_PARAMS,buffex,256);
			SET_DATA(pSend, MAPCMD, MS_USER_PARAMS,pSendPacket);
			pSend->nHpMax	= m_pMainAIObj->xParam.nHpMax;
			pSend->nMpMax	= m_pMainAIObj->xParam.nMpMax;
			pSend->nAim		= m_pMainAIObj->xParam.nAim;
			pSend->nAtkN	= m_pMainAIObj->xParam.nAtkN;
			pSend->nAtkW	= m_pMainAIObj->xParam.nAtkW;
			pSend->nCrit	= m_pMainAIObj->xParam.nCrit;
			pSend->nDefN	= m_pMainAIObj->xParam.nDefN;
			pSend->nDefW	= m_pMainAIObj->xParam.nDefW;
			pSend->nDodge	= m_pMainAIObj->xParam.nDodge;
			pSend->nMSpeed	= m_pMainAIObj->xParam.nMSpeed;
			pSend->nPvpJSDef	= m_pMainAIObj->xParam.nPvpJSDef;
			SendPacket(pSendPacket);
		}
		break;
	case C_SHOW_FISHACT:
		{
			BuildPacketEx(pPacket, MAPCMD, MS_ATKINFO, buf, 256);
			SET_DATA(pData, MAPCMD, MS_ATKINFO, pPacket);
			pData->bTSrc	= m_pMainAIObj->byType;
			pData->dwIDSrc	= m_pMainAIObj->dwUin;
			pData->bAoeHit	= 0;
			pData->bTDest	= 0;
			pData->dwIDDest	= 0;
			pData->nDamage	= 0;
			pData->wSkill	= 6520;
			pData->bModel	= 0;

			m_pMainAIObj->pRunPlate->SendPacketOutCtrl(pPacket);
		}
		break;
	}
}

void ControlObj::SendPacket( ZPacket* pPacket )
{//发送给网关服务器
	if(this == NULL && pPacket == NULL)
		return;
	g_pNetCenter->SendPacketToNetGate(pPacket,m_dwKey);
}

void ControlObj::SetCtrl( BYTE bControl, MapAIObj* pObj )
{
	switch(bControl)
	{
	case 0:
		if(m_pMainAIObj == pObj)
			m_pMainAIObj	= NULL;
		else
			RemoveCtrlChild(pObj);
		pObj->SetControl(NULL,0);
		break;
	case 1:		//被动攻击型
	case 2:		//主动攻击型
	case 3:		//跟随类
	case 4:		//跟随,嘲讽
		AddCtrlChild(pObj);
		pObj->SetControl(this,bControl);
		pObj->SetCmd(MCMD_FOLLOW,0,m_pMainAIObj);
		break;
	case 8:		//所有权怪
		pObj->dwLockAward	= m_dwUin;
		break;
	case 10:
		_setMainCtrl(pObj,bControl);
		break;
	case 11:
		_setMainCtrl(pObj,bControl);
		break;
	}
}

void ControlObj::Update()
{
	if(m_pMainAIObj == NULL || m_pRunMap == NULL)
		return;

	if(m_nMIndex != m_pMainAIObj->nMIndex || m_wPX != m_pMainAIObj->wMPX || m_wPY	!= m_pMainAIObj->wMPY)
	{//区域发生变化
		xnList*		pListOld	= xnList::Create();
		xnList*		pListNew	= xnList::Create();
		xnList*		pListTmp	= xnList::Create();
		m_pRunMap->GetPlateRangeB(m_wPX,m_wPY,CTRLVIEW_WIDTH/2,CTRLVIEW_HEIGHT/2,pListOld);
		//_getVPlates(pListOld);
		m_nMIndex	= m_pMainAIObj->nMIndex;
		m_wPX		= m_pMainAIObj->wMPX;
		m_wPY		= m_pMainAIObj->wMPY;
		//_getVPlates(pListNew);
		m_pRunMap->GetPlateRangeB(m_wPX,m_wPY,CTRLVIEW_WIDTH/2,CTRLVIEW_HEIGHT/2,pListNew);

		pListOld->CheckMyOnly(pListNew,pListTmp);
		for (int i = 0; i < pListTmp->Count; i++)
		{
			MapPlate* pMPlate	= (MapPlate*)pListTmp->Items[i];
			pMPlate->RemoveCtrlView(this);
		}
		pListNew->CheckMyOnly(pListOld,pListTmp);
		for (int i = 0; i < pListTmp->Count; i++)
		{
			MapPlate* pMPlate	= (MapPlate*)pListTmp->Items[i];
			pMPlate->AddCtrlView(this);
		}

		pListTmp->Free();
		pListNew->Free();
		pListOld->Free();
	}
}

void ControlObj::_setMainCtrl( MapAIObj* pObj, BYTE bCtrl )
{
	m_bForce			= pObj->bForce;
	pObj->pCtrl			= this;
	pObj->bContrlType	= bCtrl;
	m_pMainAIObj		= pObj;

	BuildPacketEx(pPacket,MAPCMD, MS_CTRLINFO, buf, 1024);
	SET_DATA(pSend, MAPCMD, MS_CTRLINFO, pPacket);
	strcpy(pSend->szName,pObj->szName);
	pSend->byType	= pObj->byType;
	pSend->dwID		= pObj->dwUin;
	pSend->nHp		= pObj->xParam.nHp;
	pSend->nHpMax	= pObj->xParam.nHpMax;
	pSend->nMp		= pObj->xParam.nMp;
	pSend->nMpMax	= pObj->xParam.nMpMax;
	pSend->byWeath	= m_pRunMap->m_byWeather;
	pSend->wSP		= m_nSpVal;
	pSend->dwMIndex	= m_pRunMap->m_dwMapIndex;
	pSend->wRMIndex	= m_pRunMap->m_pMCfgObj->m_wMapIndex;
	pSend->wAtkDis	= pObj->GetObjAtkDis();
	pSend->bSCnt	= 0;
	DWORD	dwTickCur	= GetTickCount();
	for( int i = 0; i < MAX_ACTIVE_SKILL; i++)
	{
		if(pObj->pSkillCenter->m_ppActSkill[i] && pObj->pSkillCenter->m_ppActSkill[i]->byPos)
		{
			pSend->szSkill[pSend->bSCnt].wSkillID	= pObj->pSkillCenter->m_ppActSkill[i]->pSBase->wSkillID;
			pSend->szSkill[pSend->bSCnt].bLv		= pObj->pSkillCenter->m_ppActSkill[i]->bySLv;
			pSend->szSkill[pSend->bSCnt].bPos	= pObj->pSkillCenter->m_ppActSkill[i]->byPos-1;
			if(pObj->pSkillCenter->m_ppActSkill[i]->dwCDTick)
				pSend->szSkill[i].nCDTick	= pObj->pSkillCenter->m_ppActSkill[i]->dwCDTick-dwTickCur;
			else
				pSend->szSkill[i].nCDTick	= 0;
			pSend->bSCnt++;
		}
	}

	pPacket->wSize	+= (pSend->bSCnt-1)*sizeof(MAPCMD_MS_SKILLCHG_Data);
	SendPacket(pPacket);
}

bool ControlObj::AddCtrlChild( MapAIObj* pObj )
{
	if(m_pListChild->IndexOf(pObj) == -1)
		m_pListChild->Add(pObj);

	pObj->bForce	= m_bForce;
	//if(pObj->xParam.nPvpDef == 0)
	//	pObj->xParam.nPvpDef	= 10;
	return true;
}

bool ControlObj::RemoveCtrlChild( MapAIObj* pObj )
{
	//pObj->xParam.nPvpDef	= 0;
	return m_pListChild->Remove(pObj);
}
extern int CalcDistance(int nX1,int nY1,int nX2,int nY2);
bool ControlObj::GetCtrlAICommand( MapAIObj* pObj )
{
	if(!m_pMainAIObj)
		return false;

	if(pObj->xParam.nMSpeed != 0)
	{
		int		nDisFlw	= CalcDistance(m_pMainAIObj->nX,m_pMainAIObj->nY,pObj->nX,pObj->nY);
		if(nDisFlw > 400)
		{
			if(nDisFlw > 700)
			{
				pObj->JumpInMap(m_pMainAIObj->nX,m_pMainAIObj->nY);
				return true;
			}

			if(pObj->bCmd != MCMD_FOLLOW)
				pObj->SetCmd(MCMD_FOLLOW,0,m_pMainAIObj);
			return true;
		}
	}

	switch(pObj->bContrlType)
	{
	case 1:		//被动
		if(pObj->bCmd == MCMD_NULL)
			pObj->SetCmd(MCMD_FOLLOW,0,m_pMainAIObj);
		break;
	case 2:		//主动
		{
			
			//方圆三格以内为索敌范围
			if(pObj->pBase==NULL || pObj->pBase->bAtkType == 2)
			{
				xnList* pList	= xnList::Create();
				m_pRunMap->GetPlateRangeB(m_pMainAIObj->wMPX,m_pMainAIObj->wMPY,3,3,pList);
				while(pList->Count)
				{
					//从列表中间开始查找
					MapPlate* pPlate	= (MapPlate*)pList->Delete(pList->Count/2);
					MapAIObj* pTarget	= pPlate->GetAtkTarget(pObj);
					if(pTarget)
					{
						pObj->SetCmd(MCMD_ATTACK,0,pTarget);
						pList->Free();
						return true;
					}
				}
				pList->Free();
			}


			if(pObj->xParam.nMSpeed == 0)
			{
				pObj->SetCmd(MCMD_ATTACK,0,0);
				return true;
			}

			if(pObj->bCmd == MCMD_NULL)
				pObj->SetCmd(MCMD_FOLLOW,0,m_pMainAIObj);
		}
		break;
	case 3:		//跟随
		if(pObj->bCmd != MCMD_FOLLOW)
			pObj->SetCmd(MCMD_FOLLOW,0,m_pMainAIObj);
		break;
	case 4:
		if(pObj->bCmd != MCMD_FOLLOW)
			pObj->SetCmd(MCMD_FOLLOW,0,m_pMainAIObj);
		xnList* pList	= xnList::Create();
		//方圆三格以内为索敌范围
		m_pRunMap->GetPlateRangeB(pObj->wMPX,pObj->wMPY,1,1,pList);
		while(pList->Count)
		{
			//从列表中间开始查找
			MapPlate* pPlate	= (MapPlate*)pList->Delete(pList->Count/2);

			for (int n	= 0; n < pPlate->m_pListEnemy->Count; n++)
			{
				MapAIObj* pTarget	= (MapAIObj*)pPlate->m_pListEnemy->Items[n];
				if(pTarget->CheckCanLockAttack(pObj) &&	pTarget->pTarget != pObj)
				{
					pTarget->SetCmd(MCMD_ATTACK,0,pObj);
				}
			}
		}
		pList->Free();
		break;
	}

	return true;
}

void ControlObj::ChgSP( int nSp )
{
	if(nSp + m_nSpVal > 50)
		nSp	= 50 - m_nSpVal;
	if(nSp + m_nSpVal <= 0)
		nSp	= -m_nSpVal;

	if(nSp == 0)
		return;

	m_nSpVal	+= nSp;
	if(m_nSpVal < 0)
		m_nSpVal	= 0;

	if(m_pMainAIObj)
		m_pMainAIObj->bGSSyncInfo	= true;

	BuildPacketEx(pPacket, MAPCMD, MS_SPVAL, buf, 256);
	SET_DATA(pSend, MAPCMD, MS_SPVAL,pPacket);
	pSend->wSpVal	= m_nSpVal;
	SendPacket(pPacket);
}

void ControlObj::StrikeSkill( WORD wSkill )
{
	BuildPacketEx(pPacket, MAPCMD, MS_STRIKESKILL, buf, 256);
	SET_DATA(pSend, MAPCMD, MS_STRIKESKILL,pPacket);
	pSend->wSkill	= wSkill;
	SendPacket(pPacket);
}
