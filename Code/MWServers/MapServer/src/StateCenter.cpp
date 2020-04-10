#include "stdafx.h"
#include "StateCenter.h"
#include "MapAIObj.h"
#include "ios_mapcmd_packet.h"
#include "ZNetPacket.h"
#include "MapPlate.h"
#include "ControlObject.h"
#include "ZNetServer.h"

StateCenter::StateCenter( MapAIObj* pObj )
{
	m_pParent	= pObj;

	memset(&m_xStateParam,0,sizeof(MParam));
	memset(&m_xBaseParam,0,sizeof(MParam));
	memset(m_szwStateSkin,0,54);
	memset(m_sznLockAction,0,28);
	m_bSkinChged	= false;
	m_nShildVal		= 0;
	m_wShildState	= 0;
	m_nRemoveStateMark	= 0;
	m_ifUnhurt	= false;
	m_ifUnvisable = false;
}

StateCenter::~StateCenter()
{
	std::map<WORD,StateObj*>::iterator	itor	= m_mapStates.begin();
	while (itor	!= m_mapStates.end())
	{
		delete itor->second;
		m_mapStates.erase(itor++);
	}
}

void StateCenter::Update()
{
	DWORD	dwTickCur	= GetTickCount();
	std::map<WORD,StateObj*>::iterator	itor	= m_mapStates.begin();
	while (itor	!= m_mapStates.end())
	{
		StateObj* pState	= itor->second;

		
		if(pState->pSBase->bDisType	== 3)
		{
			if(m_pParent->xParam.nHp <= 0)
			{
				if(m_pParent->pCtrl)
					_syncControl(pState,true);
				_stateWork(pState->pSBase,pState->bLv,true);
				delete itor->second;
				m_mapStates.erase(itor++);
				continue;
			}
		}
		
		if(m_nRemoveStateMark && pState->pSBase->wRemoveMark && pState->pSBase->wRemoveMark < m_nRemoveStateMark)
		{
			if(m_pParent->pCtrl)
				_syncControl(pState,true);
			_stateWork(pState->pSBase,pState->bLv,true);
			delete itor->second;
			m_mapStates.erase(itor++);
			continue;
		}
		else if(pState->pSBase->dwPerTick && dwTickCur - pState->dwTickStart >= pState->pSBase->dwPerTick)
		{//状态时间到了
			if(pState->bWorkTimeRm <= 1)
			{//次数用完
				if(m_pParent->pCtrl)
					_syncControl(pState,true);
				_stateWork(pState->pSBase,pState->bLv,true);
				delete itor->second;
				m_mapStates.erase(itor++);
				continue;
			}
			else
			{//多次触发每次触发加一层
				pState->bWorkTimeRm--;
				pState->dwTickStart	= GetTickCount();
				if(pState->bLv < pState->pSBase->bLayerMax)
					pState->bLv++;

				_stateWork(pState->pSBase,1,false,false,false);
			}
		}
		itor++;
	}
	m_nRemoveStateMark	= 0;
}

StateObj* StateCenter::SetState( WORD wState,BYTE bLv, DWORD dwTickRm )
{
	StateBase*	pBase	= StateConfig::GetInstance().GetStateBase(wState);
	if(!pBase)
		return NULL;
	StateObj*	pState	= _getStateByPos(pBase->wWorkPos);
	if(pState)
	{//状态位上已有状态
		if(pState->pSBase->wStateID == pBase->wStateID)
		{
			int nLvChg	= (((int)pState->bLv)-bLv);
			if(pState->bLv > bLv)
				_stateWork(pBase,pState->bLv-bLv,true);
			else if(bLv > pState->bLv)
				_stateWork(pBase,bLv-pState->bLv,false);
			pState->bLv	= bLv;
		}
		else if(pState->pSBase->bWorkVal >= pBase->bWorkVal)
		{//替换状态
			_removeStateByPos(pBase->wWorkPos);

			return _setState(pBase,bLv,dwTickRm);
		}
		else
			return NULL;
	}
	else
	{
		return _setState(pBase,bLv,dwTickRm);
	}

	return pState;
}

StateObj* StateCenter::AddState( WORD wState ,BYTE bType,DWORD dwUin)
{
	StateBase*	pBase	= StateConfig::GetInstance().GetStateBase(wState);
	if(!pBase)
		return NULL;
	StateObj*	pState	= _getStateByPos(pBase->wWorkPos);
	if(pState)
	{//状态位上已有状态
		if(pState->pSBase->wStateID == pBase->wStateID)
		{
			if(_addStateLayer(pState,bType,dwUin))
				return pState;
			else
				return NULL;
		}
		else if(pState->pSBase->bWorkVal >= pBase->bWorkVal)
		{//替换状态
			_removeStateByPos(pBase->wWorkPos);

			return _addState(pBase,bType,dwUin);
		}
		else
			return NULL;
	}
	else
	{
		return _addState(pBase,bType,dwUin);
	}

	return pState;
}

StateObj* StateCenter::GetState( WORD wState )
{
	std::map<WORD,StateObj*>::iterator	itor	= m_mapStates.begin();
	while (itor	!= m_mapStates.end())
	{
		if(itor->second->pSBase->wStateID	== wState)
			return itor->second;

		itor++;
	}
	return NULL;
}

StateObj* StateCenter::_getStateByPos( WORD wPos )
{
	std::map<WORD,StateObj*>::iterator	itor	= m_mapStates.find(wPos);
	if(itor != m_mapStates.end())
	{
		return itor->second;
	}
	return NULL;
}


bool StateCenter::RemoveState( WORD wState )
{
	std::map<WORD,StateObj*>::iterator	itor	= m_mapStates.begin();
	while (itor	!= m_mapStates.end())
	{
		if(itor->second->pSBase->wStateID	== wState)
		{
			if(m_pParent->pCtrl)
				_syncControl(itor->second,true);
			_stateWork(itor->second->pSBase,itor->second->bLv,true);
			delete itor->second;
			m_mapStates.erase(itor);
			return true;
		}

		itor++;
	}
	return NULL;
}

bool StateCenter::_removeStateByPos( WORD wPos )
{
	std::map<WORD,StateObj*>::iterator	itor	= m_mapStates.find(wPos);
	if(itor != m_mapStates.end())
	{
		if(m_pParent->pCtrl)
			_syncControl(itor->second,true);
		_stateWork(itor->second->pSBase,itor->second->bLv,true);
		delete itor->second;
		m_mapStates.erase(itor);
		return true;
	}
	return false;
}

StateObj* StateCenter::_setState( StateBase* pBase,BYTE bLv, DWORD dwRmTick )
{
	if(pBase->dwPerTick && pBase->dwPerTick <= 500)
	{//瞬时状态
		_stateWork(pBase,bLv,false);
		return NULL;
	}
	else
	{
		StateObj* pState	= new StateObj();
		pState->pSBase	= pBase;
		pState->bLv		= bLv;
		int nWorkTime	= 0;
		if(pBase->dwPerTick)
		{
			nWorkTime	= dwRmTick/pBase->dwPerTick;
			if(nWorkTime > pBase->bWorkTime)
				nWorkTime	= pBase->bWorkTime-1;
			dwRmTick	= dwRmTick%pBase->dwPerTick;
		}
		
		pState->bWorkTimeRm	= nWorkTime;
		pState->dwTickStart	= GetTickCount()-pBase->dwPerTick+dwRmTick;
		m_mapStates[pBase->wWorkPos]	= pState;
		if(m_pParent->pCtrl)
			_syncControl(pState,false);
		_stateWork(pBase,bLv,false);
		return pState;
	}
	return NULL;
}

StateObj* StateCenter::_addState( StateBase* pBase ,BYTE bType,DWORD dwUin)
{
	if(pBase->dwPerTick && pBase->dwPerTick <= 500)
	{//瞬时状态
		_stateWork(pBase,pBase->bLayerStart,false,false,true,bType,dwUin);
		return NULL;
	}
	else
	{
		StateObj* pState	= new StateObj();
		pState->pSBase	= pBase;
		pState->bLv		= pBase->bLayerStart;
		pState->bWorkTimeRm	= pBase->bWorkTime;
		pState->dwTickStart	= GetTickCount();
		m_mapStates[pBase->wWorkPos]	= pState;
		if(m_pParent->pCtrl)
			_syncControl(pState,false);
		_stateWork(pBase,pBase->bLayerStart,false,false,true,bType,dwUin);
		return pState;
	}
	return NULL;
}

bool StateCenter::_addStateLayer( StateObj* pState ,BYTE bType,DWORD dwUin)
{
	pState->bWorkTimeRm	= pState->pSBase->bWorkTime;
	pState->dwTickStart	= GetTickCount();
	if(pState->bLv >= pState->pSBase->bLayerMax)
		return true;

	pState->bLv++;
	if(m_pParent->pCtrl)
		_syncControl(pState,false);
	_stateWork(pState->pSBase,1,false,false,false,bType,dwUin);
	return true;
}

void StateCenter::_stateWork( StateBase* pBase, BYTE bLv,bool bBack,bool bRecal,bool	bNew ,BYTE bType,DWORD dwUin)
{
	if(pBase->bSyncClient)
	{
		if(bBack)
			_syncClient(pBase->wStateID,0);
		else if(bNew)
			_syncClient(pBase->wStateID,bLv);
	}

	//形象
	if(pBase->bSkinPos && pBase->bSkinPos <= 8)
	{
		if(bBack)
		{
			m_szwStateSkin[pBase->bSkinPos][0]	= 0;
			m_szwStateSkin[pBase->bSkinPos][1]	= 0;
			m_szwStateSkin[pBase->bSkinPos][2]	= 0;
		}
		else
		{
			m_szwStateSkin[pBase->bSkinPos][0]	= pBase->wChgSkin;
			m_szwStateSkin[pBase->bSkinPos][1]	= pBase->wChgWeapon;
			m_szwStateSkin[pBase->bSkinPos][2]	= pBase->wChgHorse;
		}
		m_bSkinChged	= true;
	}

	for (int i = 0; i < 8; i++)
	{
		if(pBase->szbSpEff[i])
		{
			if(bBack == false)
			{
				if(m_sznLockAction[i] == 0)
				{//生效
					if(m_pParent->pCtrl && m_pParent->byType == MAPOBJ_PLAYER)
					{
						BuildPacketEx(pSendPacket, MAPCMD, MS_SETLOCKEFF, buf, 256);
						SET_DATA(pSend, MAPCMD,MS_SETLOCKEFF, pSendPacket);
						pSend->byType	= i;
						pSend->byLocked	= 1;
						m_pParent->pCtrl->SendPacket(pSendPacket);
					}
					if(pBase->wStateID > 9000)
					{
						if(i == 1)
							m_ifUnhurt = true;
						else if(i == 7)
							m_ifUnvisable = true;
					}
				}
				m_sznLockAction[i]	= GetTickCount();
			}
			else
			{
				if(m_sznLockAction[i])
				{//解除
					if(m_pParent->pCtrl && m_pParent->byType == MAPOBJ_PLAYER)
					{
						BuildPacketEx(pSendPacket, MAPCMD, MS_SETLOCKEFF, buf, 256);
						SET_DATA(pSend, MAPCMD,MS_SETLOCKEFF, pSendPacket);
						pSend->byType	= i;
						pSend->byLocked	= 0;
						m_pParent->pCtrl->SendPacket(pSendPacket);
					}
					if(pBase->wStateID > 9000)
					{
						if(i == 1)
							m_ifUnhurt = false;
						else if(i == 7)
							m_ifUnvisable = false;
					}
				}
				m_sznLockAction[i]	= 0;

			}
		}
	}
	
	for (int i = 0; i < pBase->pListStateEff->Count; i++)
	{
		StateEffVal*	pEff	= (StateEffVal*)pBase->pListStateEff->Items[i];

		if(bBack || bRecal)
		{//重新计算属性的时候扣血的不用算
			//血蓝不会回复
			if(pEff->bType == 0 || pEff->bType == 2)
				continue;
		}
		int nVal	= 0;
		if(pEff->nPer)
		{
			if(pEff->nSpVal)
				nVal	= m_xBaseParam.GetParam(pEff->nSpVal)*pEff->nPer/10000;
			else
				nVal	= m_xBaseParam.GetParam(pEff->bType)*pEff->nPer/10000;
		}
		else
			nVal	= pEff->nSpVal;

		if(pEff->bType == 0)
		{
			m_pParent->ChgHp(nVal);

			if (m_pParent->xParam.nHp > 0 && nVal >= m_pParent->xParam.nHp)
			{
				StateObj* pOwerState = GetState(pBase->wStateID);
				if (pOwerState)
				{
					m_pParent->KillRewardByState(pOwerState->byFromType,pOwerState->dwFromID);
				}else
					m_pParent->KillRewardByState(bType,dwUin);
			}

		}
		else if(pEff->bType == 2)
			m_pParent->ChgMp(nVal);
		else if(pEff->bType == 12)
		{
			if(bBack)
				m_nShildVal	= 0;
			else
			{
				m_nShildVal	+= nVal;
				m_wShildState	= pBase->wStateID;
			}
		}
		else if(pEff->bType == 13)
		{
			if(!bBack)
				m_nRemoveStateMark	= nVal;
		}
		else
		{
			if(bBack)
				nVal	= -nVal;

			m_xStateParam.AddParam(pEff->bType,nVal);
			m_bParamChged	= true;
		}
	}

}

void StateCenter::CleanRideLockState()
{
	BuildPacketEx(pSendPacket, MAPCMD, MS_SETLOCKEFF, buf, 256);
	SET_DATA(pSend, MAPCMD,MS_SETLOCKEFF, pSendPacket);
	pSend->byType	= 6;
	pSend->byLocked	= 0;
	m_pParent->pCtrl->SendPacket(pSendPacket);
}

void StateCenter::ResetBaseParam( MParam* pMP )
{
	memset(&m_xStateParam,0,sizeof(MParam));
	memcpy(&m_xBaseParam,pMP,sizeof(MParam));
	std::map<WORD,StateObj*>::iterator	itor	= m_mapStates.begin();
	while (itor	!= m_mapStates.end())
	{
		_stateWork(itor->second->pSBase,itor->second->bLv,false,true);
		itor++;
	}
}

void StateCenter::FillParam( MParam* pMP )
{
	pMP->nHpMax		= m_xBaseParam.nHpMax+m_xStateParam.nHpMax;
	pMP->nMpMax		= m_xBaseParam.nMpMax+m_xStateParam.nMpMax;

	pMP->nAtkW		= m_xBaseParam.nAtkW+m_xStateParam.nAtkW;
	pMP->nAtkN		= m_xBaseParam.nAtkN+m_xStateParam.nAtkN;
	pMP->nDefW		= m_xBaseParam.nDefW+m_xStateParam.nDefW;
	pMP->nDefN		= m_xBaseParam.nDefN+m_xStateParam.nDefN;
	pMP->nAim		= m_xBaseParam.nAim+m_xStateParam.nAim;
	pMP->nDodge		= m_xBaseParam.nDodge+m_xStateParam.nDodge;
	pMP->nCrit		= m_xBaseParam.nCrit+m_xStateParam.nCrit;
	pMP->nMSpeed	= m_xBaseParam.nMSpeed+m_xStateParam.nMSpeed;
	pMP->nPvpDef	= m_xBaseParam.nPvpDef+m_xStateParam.nPvpDef;

	if(pMP->nHp > pMP->nHpMax)
		pMP->nHp	= pMP->nHpMax;
	if(pMP->nMp > pMP->nMpMax)
		pMP->nMp	= pMP->nMpMax;

}

WORD StateCenter::GetViewSkin( BYTE bPos )
{
	if(bPos > 2)
		return 0;
	for (int i = 8; i >= 0; i--)
	{
		if(m_szwStateSkin[i][0] || m_szwStateSkin[i][bPos])
		{
			return m_szwStateSkin[i][bPos];
		}
	}
	return 0;
}

void StateCenter::_syncClient( WORD wState, BYTE bLayer )
{
	if(m_pParent->pRunPlate == NULL)
		return;
	std::map<WORD,BYTE>::iterator	itor	= m_mapSClient.find(wState);
	if(itor	!= m_mapSClient.end())
	{
		if(bLayer == 0)
		{
			m_mapSClient.erase(itor);

			//发客户端删除
			BuildPacketEx(pPacket,MAPCMD,MS_STATEDEL,buf,256);
			SET_DATA(pSend,MAPCMD,MS_STATEDEL,pPacket);
			pSend->bType	= m_pParent->byType;
			pSend->dwID		= m_pParent->dwUin;
			pSend->wState	= wState;
			m_pParent->pRunPlate->SendPacketExCtrl(pPacket,m_pParent->pCtrl);
		}
		else
			m_mapSClient[wState]	= bLayer;
	}
	else
	{
		if(bLayer)
		{
			m_mapSClient[wState]	= bLayer;

			BuildPacketEx(pPacket,MAPCMD,MS_STATECHG,buf,256);
			SET_DATA(pSend,MAPCMD,MS_STATECHG,pPacket);
			pSend->bType	= m_pParent->byType;
			pSend->dwID		= m_pParent->dwUin;
			pSend->wState	= wState;
			m_pParent->pRunPlate->SendPacketExCtrl(pPacket,m_pParent->pCtrl);
		}
	}
}

void StateCenter::_syncControl( StateObj* pState, bool bRemove )
{//状态过地图需要保持同步
	if(!m_pParent->pCtrl)
		return;
	if(m_pParent->pCtrl->m_pMainAIObj != m_pParent)
		return;

	int nTotalTick	= pState->pSBase->dwPerTick;
	if(pState->bWorkTimeRm)
		nTotalTick	= pState->pSBase->dwPerTick*pState->bWorkTimeRm;

	if(!bRemove && pState->pSBase->dwPerTick != 0 && nTotalTick < 3000)
		return;

	BuildPacketEx(pPacket, MAPOBJ, MS_SYNCSTATE, buf, 256);
	SET_DATA(pSend, MAPOBJ, MS_SYNCSTATE, pPacket);
	pSend->xState.wStateID	= pState->pSBase->wStateID;
	pSend->xState.bLvl		= pState->bLv;
	if(bRemove)
		pSend->xState.dwTickTime	= 0;
	else
	{
		if(pState->pSBase->dwPerTick == 0)
			pSend->xState.dwTickTime	= -1;
		else
		{
			int	nRmTime	= nTotalTick + pState->dwTickStart - GetTickCount();
			if(nRmTime < 1000)
				return;

			pSend->xState.dwTickTime	= nRmTime;
		}

	}
	g_pNetCenter->SendPacketToGameSrv(pPacket,m_pParent->pCtrl->m_dwUin);
}

void StateCenter::DealDamager( int &nVal )
{
	if(nVal >= 0)
		return;
	if(m_nShildVal)
	{
		m_nShildVal += nVal;
		if(m_nShildVal < 0)
		{
			nVal	= m_nShildVal;
			m_nShildVal	= 0;
			RemoveState(m_wShildState);
			m_wShildState	= 1;
		}
		else
			nVal	= 0;
	}
}

int StateCenter::FillClientStates( xnList* pList )
{
	pList->Clear();
	std::map<WORD,StateObj*>::iterator	itor	= m_mapStates.begin();
	while (itor	!= m_mapStates.end())
	{
		if(itor->second->pSBase->bSyncClient)
		{
			pList->Add((void*)itor->second->pSBase->wStateID);
		}

		itor++;
	}
	return pList->Count;
}

