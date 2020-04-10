#include "MapControl.h"
#include "MapAIObj.h"
#include "MainMenu.h"
#include "ios_mapcmd_packet.h"
#include "EnemyConfig.h"
#include "MapMouse.h"
#include "MapStaticCfg.h"
#include "SoundCenter.h"

#include "GuideCenter.h"

class _DelayHitInfo
{
public:
	int		nHpSub;
	BYTE	bType;		//0普通, 1回血, 2暴击, 3躲避
	DWORD	dwTick;
};

MapAIObj::MapAIObj()
{
	m_byState		= OBJ_STATE_STAND;
	nDestX			= 0;
	nDestY			= 0;
	dwHitTick		= 0;
	dwMoveTick		= xnGetTickCount();
	fRemainX		= 0;		//上次移动的小数位
	fRemainY		= 0;
	fSpeedX			= 0;
	fSpeedY			= 0;
	nHP				= 100;
	nMaxHP			= 100;
	enemyID         = 0;
	dwMastUin		= 0;
	wSpeed			= 225;
	bSpeedChged		= false;
	byLv			= 1;
	m_dwIsRush		= 0;
	nMarryEff		= 0;
	memset(m_szName,0,20);
	memset(m_szTitle,0,32);
	memset(m_szMarryTitle,0,32);
	memset(m_szwSkin, 0, MSKIN_MAX*2);

	bForce		= 0;
	pBaseCfg		= NULL;
	pPathObj		= NULL;
	m_byDir			= rand()%4*2;

	objAIModel.SetCmd(MCMD_NULL,0);

	m_pLEffAdd		= xnList::Create();
	m_pLEffDel		= xnList::Create();
	m_pLFightNum	= xnList::Create();
	pLDelayHit		= xnList::Create();
	pLHitRequest	= xnList::Create();
	dwBusyTick		= 0;
	dwAtkLock		= 0;
	pTalkObj		= NULL;
	m_bySPEff		= 0;

	pStateCenter	= new StateCenter(this);
}

MapAIObj::~MapAIObj()
{
	while(m_pLEffAdd->Count)
		delete	(MObjEff*)m_pLEffAdd->Delete(0);
	m_pLEffAdd->Free();
	while(pLDelayHit->Count)
		delete	(_DelayHitInfo*)pLDelayHit->Delete(0);
	pLDelayHit->Free();
	m_pLEffDel->Free();
	while(m_pLFightNum->Count)
		delete (_MuliParamObj*)m_pLFightNum->Delete(0);
	m_pLFightNum->Free();

	if(pTalkObj)
	{
		pMainToopTip->ClearFace(pTalkObj);
		delete pTalkObj;
	}
	if(g_pMapCommand->m_pObjNAtk == this)
		g_pMapCommand->m_pObjNAtk	= NULL;
	if(g_pMapCenter->m_pMObjLockView == this)
		g_pMapCenter->m_pMObjLockView	= NULL;

	delete pStateCenter;
	if(m_byType == MAPOBJ_PLAYER)
	{//离开地图光效
		g_pMapCenter->AddMapEff(26,nPosX,nPosY,-1);
	}
}


BYTE MapAIObj::GetAction()
{
	//return xnGetTickCount() /3000%12;
	if(nHP <= 0)
		return OBJ_ACTION_DEAD;

	if(m_szwSkin[MSKIN_HORSE])
	{
		if(m_byState	== OBJ_STATE_RUN)
			return OBJ_ACTION_HRUN;
		else
			return OBJ_ACTION_HSTAND;
	}
	else
	{
		switch (m_byState)
		{
		case OBJ_STATE_STAND:
			{
				DWORD	dwTickNow	= xnGetTickCount();
				if(dwTickNow - dwHitingTick < 300)
					return OBJ_ACTION_HURT;
				if(dwHitTick && dwTickNow - dwHitTick	< 50)
				{
					dwHitingTick	= dwTickNow;
					return OBJ_ACTION_HURT;
				}
			}
			return OBJ_ACTION_STAND;
		case OBJ_STATE_ATK:			
			//return (OBJ_ACTION_ATK2 /*+ rand()%2*/);
		case OBJ_STATE_RUN:
			return m_byState;
		case OBJ_STATE_SKILL:
			{
				if(m_byType	!= MAPOBJ_PLAYER)
				{
					return OBJ_ACTION_ATK1;
				}
			}
			return OBJ_ACTION_ATK2;
		default:
			return OBJ_ACTION_STAND;
		}
	}
	return OBJ_ACTION_STAND;	
	
}

void MapAIObj::SetRush( int nX, int nY, BYTE bAction )
{
	if(nX	== nPosX && nY	== nPosY)
		return;

	objAIModel.SetCmd(MCMD_NULL,0);
	nDestX	= nX;
	nDestY	= nY;
	m_byState	= OBJ_STATE_STAND;

	switch(bAction)
	{
	case 1:	//冲锋
		if(abs(nDestX - nPosX)+abs(nDestY-nPosY)>10)
			m_byDir	= CalcDirection(nPosX,nPosY,nDestX,nDestY,2);
		fSpeedX		= ((float)(nDestX - nPosX))*1000/500;
		fSpeedY		= ((float)(nDestY - nPosY))*1000/500;
		break;
	case 2:	//闪烁
		if(abs(nDestX - nPosX)+abs(nDestY-nPosY)>10)
			m_byDir	= CalcDirection(nPosX,nPosY,nDestX,nDestY,2);
		fSpeedX		= ((float)(nDestX - nPosX))*1000/200;
		fSpeedY		= ((float)(nDestY - nPosY))*1000/200;
		break;
	case 3:	//被拉过去
		if(abs(nDestX - nPosX)+abs(nDestY-nPosY)>10)
			m_byDir	= CalcDirection(nDestX,nDestY,nPosX,nPosY,2);
		fSpeedX		= ((float)(nDestX - nPosX))*1000/300;
		fSpeedY		= ((float)(nDestY - nPosY))*1000/300;
		break;
	case 4:	//击退
		if(abs(nDestX - nPosX)+abs(nDestY-nPosY)>10)
			m_byDir	= CalcDirection(nDestX,nDestY,nPosX,nPosY,2);
		fSpeedX		= ((float)(nDestX - nPosX))*1000/500;
		fSpeedY		= ((float)(nDestY - nPosY))*1000/500;
		break;
	case 5:
		if(abs(nDestX - nPosX)+abs(nDestY-nPosY)>10)
			m_byDir	= CalcDirection(nDestX,nDestY,nPosX,nPosY,2);
		fSpeedX		= ((float)(nDestX - nPosX))*1000/500;
		fSpeedY		= ((float)(nDestY - nPosY))*1000/500;
		break;
	}

	m_dwIsRush	= xnGetTickCount();
	m_byRushAct	= bAction;
}

int MapAIObj::_setMove( int nX, int nY )
{
	if(nX	== nPosX && nY	== nPosY)
		return 0;

	nDestX	= nX;
	nDestY	= nY;
	m_byState	= OBJ_STATE_RUN;
	if(abs(nDestX - nPosX)+abs(nDestY-nPosY)>10)
	{
		if(m_byType == MAPOBJ_ENEMY && pBaseCfg && pBaseCfg->nID >= 1504 && pBaseCfg->nID <= 1506)
			m_byDir	= CalcDirection(nPosX,nPosY,nDestX,nDestY,200);
		else
			m_byDir	= CalcDirection(nPosX,nPosY,nDestX,nDestY,2);
	}
	//dwMoveTick	= xnGetTickCount();

	int		nTLen	= CalcDistance(nPosX,nPosY,nDestX,nDestY);
	fSpeedX		= ((float)(nDestX - nPosX)*wSpeed)/nTLen;
	fSpeedY		= ((float)(nDestY - nPosY)*wSpeed)/nTLen;
	//fRemainX	= 0;
	//fRemainY	= 0;
	return nTLen;
}

void MapAIObj::_calcMove()
{
	if(m_byState	!= OBJ_STATE_RUN &&	m_dwIsRush == 0)
		return;
	DWORD	dwPassTick	= xnGetTickCount() - dwMoveTick;

	float	fMoveX	= fSpeedX*dwPassTick/1000.0	+ fRemainX;
	float	fMoveY	= fSpeedY*dwPassTick/1000.0	+ fRemainY;

	if(abs(nDestX - nPosX) < myabs(fMoveX))
	{
		nPosX	= nDestX;
		fSpeedX	= 0;
	}
	else
	{
		fRemainX	= fMoveX	- (int)fMoveX;
		nPosX		+= (int)fMoveX;
	}
	if(abs(nDestY - nPosY) < myabs(fMoveY))
	{
		nPosY	= nDestY;
		fSpeedY	= 0;
	}
	else
	{
		fRemainY	= fMoveY	- (int)fMoveY;
		nPosY		+= (int)fMoveY;
	}

	if(fSpeedX == 0 && fSpeedY == 0)
	{
		//已到达目标
		m_dwIsRush	= 0;
		objAIModel.SetCmd(MCMD_NULL,0);
		if(this == pChar->GetMyMapObj())
		{
			pChar->_checkUserCommand(true);
		}
		if(objAIModel.byAIType == MCMD_NULL)
			m_byState	= OBJ_STATE_STAND;
	}
}

BYTE MapAIObj::GetFishDir()
{
	if (pStateCenter&&!pStateCenter->CheckState(8111))
		return m_byDir;

	xnList*	pListTmp	= xnList::Create();
	NpcPathLeadCfg::GetInstance().GetNpcListByMIndex(g_pMapCenter->m_nBaseMapIndex,pListTmp);
	for (int i = 0; i < pListTmp->Count; i++)
	{
		PathObj* pPath	= (PathObj*)pListTmp->Items[i];
		if (pPath->nEff == 262&&CalcDistance(nPosX,nPosY,pPath->nPosX,pPath->nPosY)<250)
		{
			pListTmp->Free();
			return CalcDirection(nPosX,nPosY,pPath->nPosX,pPath->nPosY,2);
		}
	}
	pListTmp->Free();
	return m_byDir;
}

void MapAIObj::Update()
{
	if(m_byType == MAPOBJ_NPC)
	{//NPC特别,只需要说话
		
		_checkNpcAI();
		return;
	}

	if (pStateCenter&&pStateCenter->CheckState(8111))
		m_byDir = GetFishDir();

	pStateCenter->Update();

	//伤害效果展示
	if(pLDelayHit->Count)
	{
		if(pLHitRequest->Count)
		{
			DWORD	dwTick	= (DWORD)pLHitRequest->Items[0];
			if(((int)(xnGetTickCount()-dwTick)) > 0)
			{
				pLHitRequest->Delete(0);
				_DelayHitInfo* pDHit	= (_DelayHitInfo*)pLDelayHit->Delete(0);
				AddFightNum(-pDHit->nHpSub,pDHit->bType);
				if(pDHit->nHpSub > 0)
					dwHitTick	= xnGetTickCount();
				delete pDHit;
			}
		}
		else
		{
			_DelayHitInfo* pDHit	= (_DelayHitInfo*)pLDelayHit->Items[0];
			if(xnGetTickCount() - pDHit->dwTick > 1000)
			{
				AddFightNum(-pDHit->nHpSub,pDHit->bType);

				pLDelayHit->Delete(0);
				delete pDHit;
			}
		}
	}

	if(m_byType == MAPOBJ_ENEMY && byCountry >= 100)
	{//阵列怪
		MapAIObj* pAIObj	= g_pMapCenter->GetMapObj(MAPOBJ_ENEMY,dwMastUin);
		if(pAIObj)
		{
			int	nAIX	= 0;
			int	nAIY	= 0;
			GetBattleGPos(nAIX,nAIY,pAIObj->nPosX,pAIObj->nPosY,pAIObj->m_byDir,byCountry%100);
			if(nPosX == nAIX && nPosY==nAIY)
				m_byState	= OBJ_STATE_STAND;
			else
			{
				nPosX	= nAIX;
				nPosY	= nAIY;
				if(m_byState != OBJ_STATE_RUN)
					m_byState	= OBJ_STATE_RUN;
			}

			m_byDir	= pAIObj->m_byDir;
		}
	}
	else if(IsBusy() == false && IsDead() == false)
	{
		RunAIModel();
		_calcMove();
	}
	else if(m_dwIsRush)
		_calcMove();
	else if(objAIModel.byAIType >= 10)
	{
		RunAIModel();
	}
	

	dwMoveTick	= xnGetTickCount();
}

void MapAIObj::SetSkillAtk( MapAIObj* pObj )
{
	dwBusyTick	= xnGetTickCount() + 600;
	dwAtkLock	= xnGetTickCount() + 1200;

	m_byState	= OBJ_STATE_SKILL;
	if(pObj)
	{
		if(abs(pObj->nPosX - nPosX)+abs(pObj->nPosY-nPosY)>10)
			m_byDir	= CalcDirection(nPosX,nPosY,pObj->nPosX,pObj->nPosY,2);

		pObj->pLHitRequest->Add((void*)(xnGetTickCount()+300));
	}

	objAIModel.dwAITick	= xnGetTickCount();
	pPreAtk		= pObj;
}

void MapAIObj::SetNAtk(MapAIObj* pObj)
{
	if(pObj == NULL || pObj->nHP <= 0)
		return;
	dwBusyTick	= xnGetTickCount() + 600;
	dwAtkLock	= xnGetTickCount() + 1200;

	m_byState	= OBJ_STATE_ATK;
	if(abs(pObj->nPosX - nPosX)+abs(pObj->nPosY-nPosY)>10)
		m_byDir	= CalcDirection(nPosX,nPosY,pObj->nPosX,pObj->nPosY,2);

	pObj->pLHitRequest->Add((void*)(xnGetTickCount()+300));

	objAIModel.dwAITick	= xnGetTickCount();
	pPreAtk		= pObj;
	//pObj->AddEff(7,nPosX-pObj->nPosX,nPosY-pObj->nPosY);//飞行光效
}

void MapAIObj::RunAIModel()
{
	switch(objAIModel.byAIType)
	{
	case MCMD_NULL:
		{
			if(wSpeed > 10 && (m_byType == MAPOBJ_SLAVE || (m_byType == MAPOBJ_ENEMY && byCountry==1 && pBaseCfg && pBaseCfg->bAtkType != 0)))
			{
				if(xnGetTickCount() - objAIModel.dwAITick > objAIModel.dwMark)
				{
					WORD	wMPX	= nPosX/MAPPLATE_SIZE;
					WORD	wMPY	= nPosY/MAPPLATE_SIZE;
					 
					int		nDestX	= wMPX*MAPPLATE_SIZE+ rand()%MAPPLATE_SIZE;
					int		nDestY	= wMPY*MAPPLATE_SIZE+ rand()%MAPPLATE_SIZE;

					objAIModel.SetCmd(MCMD_RUN,(nDestY<<16)+nDestX);
				}
			}
		}
		break;
	case MCMD_RUN:
		{
			WORD wX	= objAIModel.dwMark&0xFFFF;
			WORD wY	= objAIModel.dwMark>>16;
			if(m_byState != OBJ_STATE_RUN)
				m_byState	= OBJ_STATE_RUN;
			if(bSpeedChged || wX != nDestX || wY != nDestY)
			{
				int nLen	= _setMove(wX,wY);
				//objAIModel.SetCmd(MCMD_NULL,nLen*1000/wSpeed);
			}
		}
		break;
	//case MCMD_ATTACK:
	//	{
	//		WORD wX	= objAIModel.dwMark&0xFFFF;
	//		WORD wY	= objAIModel.dwMark>>16;
	//		if(m_byState != OBJ_STATE_RUN)
	//			m_byState	= OBJ_STATE_RUN;
	//		if(bSpeedChged || wX != nDestX || wY != nDestY)
	//		{
	//			int nLen	= _setMove(wX,wY);
	//			//objAIModel.SetCmd(MCMD_NULL,nLen*1000/wSpeed);
	//		}
	//	}
	//	break;
	case MCMD_LOCKVIEW_ENEMY:
		{
			MapAIObj* pObj	= g_pMapCenter->GetMapObj(MAPOBJ_ENEMY,objAIModel.dwMark);
			if(pObj)
			{
				nPosX	= pObj->nPosX;
				nPosY	= pObj->nPosY;
			}
		}
		break;
	}
}

void MapAIObj::AddEff( int nEff, int nDifX, int nDifY,DWORD	dwTick,bool bFly )
{
	if(nEff == 0)
		return;
	MObjEff*	pOEff	= new MObjEff();
	pOEff->nEffID	= nEff;
	pOEff->nDifX	= nDifX;
	pOEff->nDifY	= nDifY;
	pOEff->bFly		= bFly;
	pOEff->dwTickDelay	= xnGetTickCount()+dwTick;
	m_pLEffAdd->Add(pOEff);
}

void MapAIObj::RemoveEff( int nEff )
{
	if(nEff == 0)
		return;
	for (int i = 0; i < m_pLEffAdd->Count; i++)
	{
		MObjEff*	pOEff	= (MObjEff*)m_pLEffAdd->Items[i];
		if(pOEff->nEffID == nEff)
		{
			m_pLEffAdd->Delete(i);
			return;
		}
	}

	m_pLEffDel->Add((void*)nEff);
}

void MapAIObj::AddFightNum( int nNum, BYTE bType )
{
	if(bType == 99)
		return;
	if(bType == 2 && this == pChar->GetMyMapObj())
	{
		if(pChar->m_bySex == 1)
			g_pSoundCenter->PlayerEffect("Sound/male/crit");
		else
			g_pSoundCenter->PlayerEffect("Sound/female/crit");
		return;
	}
	_MuliParamObj*	pMP	= new _MuliParamObj();
	pMP->nParam1	= nNum;
	pMP->nParam2	= bType;
	m_pLFightNum->Add(pMP);
}

void MapAIObj::AddDelayHit( int nDamage,BYTE bType )
{
	_DelayHitInfo* pDelayH	= new _DelayHitInfo();
	pDelayH->nHpSub	= nDamage;
	pDelayH->dwTick	= xnGetTickCount();
	pDelayH->bType	= bType;
	if(this == g_pMapCenter->m_pMObjLockView && bType == 0 && nDamage > 0)
		pDelayH->bType	= 5;

	pLDelayHit->Add(pDelayH);

	objAIModel.dwAITick	= xnGetTickCount();
}

bool MapAIObj::IsBusy()
{
	if(objAIModel.byAIType	== MCMD_LOCKVIEW_ENEMY)
		return true;
	if(dwBusyTick && ((int)(xnGetTickCount() - dwBusyTick)) < 0)
		return true;
	if(m_dwIsRush)
	{
		if(xnGetTickCount() - m_dwIsRush > 500)
		{
			m_dwIsRush	= 0;
			return false;
		}
		return true;
	}
	return false;
}

bool MapAIObj::IsAtkCD()
{
	if(dwAtkLock ==0 || ((int)(xnGetTickCount() - dwAtkLock)) > 0)
		return false;
	return true;
}

void MapAIObj::_checkNpcAI()
{
	//PathObj* pBase	= (PathObj*)pBaseCfg;
	if(pPathObj->bMapTalk && pPathObj->pListTalk->Count)
	{
		if(pTalkObj == NULL)
			pTalkObj	= new ToopTip9Spr(0,120,200,100,2,"9spr2");

		if(((int)(xnGetTickCount()-dwMoveTick )) > 0)
		{
			pMainToopTip->ShowTalkTip(pTalkObj,(char*)pPathObj->pListTalk->Items[rand()%pPathObj->pListTalk->Count]);
			dwMoveTick	= xnGetTickCount() + 8000 + rand()%5000;
		}
	}

}

void MapAIObj::SetTalkInfo(char* pMsg)
{
	if(m_byType != MAPOBJ_PLAYER)
		return;
	if(pTalkObj != NULL)
	{
		pTalkObj->m_bIsDel = true;
	}
	pTalkObj	= new ToopTip9Spr(0,120,200,100,2,"9spr2");
	pMainToopTip->ShowTalkTip(pTalkObj,pMsg);
}

void MapAIObj::SetTalkInfo(ChatInfo* pMsg)
{
	if(m_byType != MAPOBJ_PLAYER)
		return;
	if(pTalkObj != NULL)
	{
		pTalkObj->m_bIsDel = true;
	}
	pTalkObj	= new ToopTip9Spr(0,120,200,100,2,"9spr2");
	pMainToopTip->ShowTalkTip(pTalkObj,pMsg);
}

void MapAIObj::ObjDead()
{
	nHP = 0;
	if(m_byType == MAPOBJ_PLAYER)
	{
		if(((MapPlayer*)this)->bySex == 1)
			g_pSoundCenter->PlayerEffect("Sound/male/die");
		else
			g_pSoundCenter->PlayerEffect("Sound/female/die");
	}
	else
	{
		pStateCenter->ClearState();
		if(m_byType == MAPOBJ_ENEMY && pBaseCfg )
		{//采集怪
			if(pBaseCfg->nID == 1061 || pBaseCfg->nID == 1119)
			{
				AddEff(100);
				g_pMapCenter->m_bySpEff	= 1;
				return;
			}
			if(pBaseCfg->bType == 3)
				return;

		}

		AddEff(27);
		g_pSoundCenter->PlayerEffect("Sound/1/die");
	}
}

bool MapAIObj::IsDead()
{
	return nHP <= 0;
}


bool MObjAIModel::SetCmd( BYTE bT, DWORD dwVal )
{
	if(byAIType == bT && dwMark == dwVal)
		return false;
	byAIType	= bT;
	dwMark		= dwVal;

	dwAITick	= xnGetTickCount();
	if(bT == MCMD_NULL)	//随机移动的间隔时间
		dwMark	= 5000+rand()%5000+dwVal;

	return true;
}
