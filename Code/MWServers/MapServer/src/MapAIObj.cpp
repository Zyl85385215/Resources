#include "stdafx.h"

#include "MapAIObj.h"
#include "ios_mapcmd_packet.h"
#include "ZNetPacket.h"
#include "MapPlate.h"
#include <math.h>
#include "ControlObject.h"
#include "BornCenter.h"
#include "MapManager.h"
#include "EnemyConfig.h"
#include "SkillCenter.h"
#include "StateCenter.h"

int	CalcDirection(int nX1,int nY1,int nX2,int nY2)
{
	int dx = nX2 - nX1;
	int dy = nY2 - nY1;
	if ((dx == 0) && (dy == 0))
		return 4;
	if (dx != 0)
	{
		double r = atan2((float)dy, (float)dx);
		int angle;
		if (r >= 0.0)
			angle = (int)(180.0f * r / 3.141593f + 22.5f);
		else
			angle = (int)(180.0f + 180.0f * (3.141593f + r) / 3.141593f + 22.5f);
		return ((angle / 45 + 2) % 8);
	}
	else
	{
		if (dy >= 0)
			return 4;
		else 
			return 0;
	}
	return 0;
}

void GetBattleGPos( int& nX, int& nY,int nDestX, int nDestY, int nDir , int nPos)
{
	int	nChgH	= nPos%2*60;
	int	nChgV	= nPos/2*60;
	int	nChgSH	= nPos/2*60-nPos%2*60;
	int	nChgSV	= nPos/2*30+nPos%2*30;



	switch(nDir)
	{
	case 0:
		nX	= nDestX + nChgH;
		nY	= nDestY - nChgV;
		break;
	case 2:
		nX	= nDestX - nChgV;
		nY	= nDestY + nChgH;
		break;
	case 4:
		nX	= nDestX - nChgH;
		nY	= nDestY + nChgV;
		break;
	case 6:
		nX	= nDestX + nChgV;
		nY	= nDestY - nChgH;
		break;
	case 1:
		nX	= nDestX - nChgSH;
		nY	= nDestY + nChgSV;
		break;
	case 3:
		nX	= nDestX - nChgSH;
		nY	= nDestY - nChgSV;
		break;
	case 5:
		nX	= nDestX + nChgSH;
		nY	= nDestY - nChgSV;
		break;
	case 7:
		nX	= nDestX + nChgSH;
		nY	= nDestY + nChgSV;
		break;
	}
}

int CalcDistance(int nX1,int nY1,int nX2,int nY2)
{
	int	nRtl	= (int)sqrt((float)((nX1-nX2)*(nX1-nX2)+(nY1-nY2)*(nY1-nY2)));
	if(nRtl <= 0)
		nRtl	= 1;
	return nRtl;
}

int CalcDistance2(int nX1,int nY1,int nX2,int nY2)
{
	return (nX1-nX2)*(nX1-nX2)+(nY1-nY2)*(nY1-nY2);
}

MapAIObj::MapAIObj()
{
	pRunPlate	= NULL;
	pBase		= NULL;
	pBornInfo	= NULL;
	pTarget		= NULL;
	pRunActPt	= NULL;
	memset(wSkin,0,6);
	bDisappear	= false;
	memset(szName,0,16);
	memset(szTitle,0,22);
	memset(szMarryTitle,0,32);
	nMarryEff	= 0;
	bGSSyncPos		= false;
	bGSSyncInfo		= false;
	bSyncHpClient	= false;
	bSyncHpMaxClient	= false;
	bSyncMpClient	= false;
	bSyncSpeed		= false;
	pCtrl			= NULL;
	bForce			= 0;
	wLockCnt		= 0;
	dwBusyTick		= 0;
	dwAtkTick		= 0;
	wTrySkill		= 0;
	m_dwWarTick		= GetTickCount();
	m_dwHealthTick	= GetTickCount();
	pSkillCenter	= new SkillCenter(this);
	pStateCenter	= new StateCenter(this);
	m_dwUpdateTime	= 0;
	dwLockAward		= 0;
	wLastLineX		= 0;
	wLastLineY		= 0;
	dwCmdValOld		= 0;
	m_byAutoAtk		= 0;
	pListHitCnt		= xnList::Create();
	byBattelPos		= 0;
	dwBattelGroup	= 0;
	byDTargetType	= 0;
	dwDTargetID		= 0;
	bIsRewarded		= false;
	m_dwTickUnActive	= GetTickCount();
	pPtPath			= NULL;
	SetCmd(MCMD_NULL,0);
}

MapAIObj::~MapAIObj()
{
	if(pTarget)
		pTarget->wLockCnt--;
	if(pPtPath)
		delete[] pPtPath;

	while(pListHitCnt->Count)
		delete (_HitCountObj*)pListHitCnt->Delete(0);
	pListHitCnt->Free();
	delete pSkillCenter;
	delete pStateCenter;
	bDisappear	= true;
	if(pCtrl)
		pCtrl->SetCtrl(0,this);
}

void MapAIObj::SetCmd( BYTE bC, DWORD dwVal ,MapAIObj* pObj )
{
	if(bC!= MCMD_NULL)
		m_dwTickUnActive	= GetTickCount();

	if(bC == bCmd && dwVal == dwCmdVal && pTarget == pObj)
		return;	//重复的指令
	if(dwTickCmd == 0 || bC != bCmd)	//连续移动命令的时候防止以为刷新移动速度变慢,不改dwTickCmd
		dwTickCmd	= GetTickCount();
	bCmd	= bC;
	dwCmdVal	= dwVal;
	if(pTarget)
	{
		pTarget->wLockCnt--;
		wLastLineX	= 0;
		wLastLineY	= 0;
	}
	pTarget		= pObj;
	if(pTarget)
	{
		pTarget->wLockCnt++;
		wLastLineX	= pTarget->nX;
		wLastLineY	= pTarget->nY;
	}
	if(bC == MCMD_NULL)
	{//下次随机移动的时间
		dwCmdVal	= 12000+rand()%10000;
	}
	if(bC == MCMD_ATTACK)
		wTrySkill	= dwVal;
	
	fXMSpeed	= 0;
	fYMSpeed	= 0;
}

void MapAIObj::RunCmd()
{
	if(pTarget && pTarget->CheckCanBeLock(this) == false)
	{//目标死亡后需要清理目标
		SetCmd(MCMD_NULL,0);
		return;
	}
	if(CheckCanRunCmd() == false)
	{
		return;
	}
	if(dwBusyTick && dwTickCmd < dwBusyTick)	//忙碌完的时间才是行动开始时间
		dwTickCmd	= GetTickCount();
	dwBusyTick	= 0;
	
	switch(bCmd)
	{
	case MCMD_NULL:
		{

		}
		break;
	case MCMD_RUN:
		{
			if(pBase == NULL || pBase->bAtkType != 0)
				_executeRun();
		}
		break;
	case MCMD_FOLLOW:
		{
			_executeFollow();
		}
		break;
	case MCMD_ATTACK:
		{
			if(pTarget)
			{
				_executeAttack();
			}
			else
			{
				int	nRtlDis	= 0;
				SkillObj* pSkill	= _getAtkSkillByDis(10000,nRtlDis);
				if(pSkill)
					_useSkill(pSkill,NULL);
			}
		}
		break;
	case MCMD_LOCKVIEW_ENEMY:	//锁定在对象上
		{
			if(pTarget)
			{
				if(pTarget->bDisappear)
				{
					_syncClientCmd(MCMD_NULL,0);
					SetCmd(MCMD_NULL,0);
				}
				else
				{
					nX	= pTarget->nX;
					nY	= pTarget->nY;
				}
			}
		}
		break;
	}
}

DWORD MapAIObj::GetMasterID()
{
	if(pCtrl)
		return pCtrl->m_dwUin;
	else
		return 0;
}

void MapAIObj::_executeRun(bool bOnlyRun)
{
	if(dwCmdVal == 0)
		return;
	if(pStateCenter->m_sznLockAction[2] && GetTickCount() - pStateCenter->m_sznLockAction[2] < 10000)
	{
		dwTickCmd	= GetTickCount();
		return;
	}

	int	nDestX	= dwCmdVal&0xFFFF;
	int	nDestY	= dwCmdVal>>16;

	if((fXMSpeed == 0 && fYMSpeed == 0) || (dwCmdValOld != dwCmdVal))
	{
		double	 dbTLen	= CalcDistance(nX,nY,nDestX,nDestY);
		if(dbTLen < 1)
			dbTLen	= 1;
		fXMSpeed	= (nDestX - nX)/dbTLen;
		fYMSpeed	= (nDestY - nY)/dbTLen;
		//确定跑步动作的时候同步客户端
		_syncClientCmd(MCMD_RUN,dwCmdVal);
		dwCmdValOld	= dwCmdVal;
	}

	DWORD	dwPass	= GetTickCount() - dwTickCmd;
	if(dwPass != 0)
	{
		dwTickCmd		= GetTickCount();
		nX		+= xParam.nMSpeed*dwPass*fXMSpeed/1000;
		nY		+= xParam.nMSpeed*dwPass*fYMSpeed/1000;
		if(nX < 0)
			nX	= 0;
		if(nY < 0)
			nY	= 0;

		if(nX >= pRunPlate->m_pMap->m_pMCfgObj->m_wMapSizeW)
			nX	= pRunPlate->m_pMap->m_pMCfgObj->m_wMapSizeW-1;
		if(nY >= pRunPlate->m_pMap->m_pMCfgObj->m_wMapSizeH)
			nY	= pRunPlate->m_pMap->m_pMCfgObj->m_wMapSizeH-1;

		bool bXOk	= (nX >= nDestX && fXMSpeed>=0) ||(nX <= nDestX && fXMSpeed<0);
		bool bYOk	= (nY >= nDestY && fYMSpeed>=0)||(nY <= nDestY && fYMSpeed<0);
		if(bXOk && bYOk)
		{
			if(bOnlyRun)
			{
				dwTickCmd	= 0;
				SetCmd(MCMD_NULL,0);
			}
			else
			{
				dwTickCmd	= 0;
				//_syncClientCmd(MCMD_NULL,0);
			}

			nX		= nDestX;
			nY		= nDestY;
		}
		else if(bXOk)
		{//当某方向变化率小的时候永远走不到,怪就跑出屏幕外了
			fXMSpeed	= 0;
			fYMSpeed	= 1;
		}
		else if(bYOk)
		{
			fXMSpeed	= 1;
			fYMSpeed	= 0;
		}
	}
	//对象移动过之后需要同步
	bGSSyncPos		= true;
}

void MapAIObj::_executeFollow()
{
	if(pTarget == NULL || pTarget->CheckCanBeLock(this) == false)
	{
		SetCmd(MCMD_NULL,0);
		return;
	}

	if(pRunPlate->m_pMap->CheckLineBlock(nX,nY,pTarget->nX,pTarget->nY))
	{
		if(wLastLineX && wLastLineY && pRunPlate->m_pMap->CheckLineBlock(nX,nY,wLastLineX,wLastLineY) == false)
		{
			int nDistance	= CalcDistance(nX,nY,wLastLineX,wLastLineY);
			if(nDistance	> 10)
			{
				int nDestY	= wLastLineY;
				dwCmdVal	= (nDestY<<16)+wLastLineX;
				//确定跑步动作的时候同步客户端
				//_syncClientCmd(MCMD_RUN,dwCmdVal);
				_executeRun(false);
				return;
			}
		}
	}
	//else
	{//继续追击
		wLastLineX	= pTarget->nX;
		wLastLineY	= pTarget->nY;

		int	nDestX	= dwCmdVal&0xFFFF;
		int	nDestY	= dwCmdVal>>16;
		int nDistance	= CalcDistance(nDestX,nDestY,pTarget->nX,pTarget->nY);

		if(nDistance > 120)
		{//重新设定命令
			fXMSpeed	= ((float)(pTarget->nX - nX))/nDistance;
			fYMSpeed	= ((float)(pTarget->nY - nY))/nDistance;
			int nDestX	= nX+fXMSpeed*(nDistance - 30+rand()%100)-15+rand()%30;
			int nDestY	= nY+fYMSpeed*(nDistance - 30+rand()%100)-15+rand()%30;
			dwCmdVal	= (nDestY<<16)+nDestX;
		}

		//确定跑步动作的时候同步客户端
		//_syncClientCmd(MCMD_RUN,dwCmdVal);
		_executeRun(false);
	}
}

bool MapAIObj::CheckCanAddState(MapAIObj* pTarget,SkillObj* pSkill)
{
	if (!pTarget||!pSkill||!pSkill->pSBase)
		return false;

	if (pTarget->byType == MAPOBJ_ENEMY && pTarget->pBase && pTarget->pBase->bType == 2)
	{
		int nRand = rand()%100;
		if (nRand < 80)
		{
			_skillMiss(pTarget,pSkill);
			return false;
		}
	}

	return true;
}

void MapAIObj::KillRewardByState(BYTE bType,DWORD dwUin)
{
	MapAIObj* pOwer	= pRunPlate->m_pMap->GetMapObj(bType,dwUin);
	if (pOwer && pOwer->pCtrl)
	{
		pRunPlate->m_pMap->ObjKillReward(pOwer,this);
	}
}

void MapAIObj::_useSkill( SkillObj* pSkill,MapAIObj* pFstTarget )
{
	if(pStateCenter->m_sznLockAction[3] && GetTickCount() - pStateCenter->m_sznLockAction[3] < 10000)
		return;

	if(pSkill->pSBase->byOnlyEnemy==2 && pFstTarget && pFstTarget->byType!=MAPOBJ_PLAYER)
	{//只对人使用的技能不能对怪放
		return;
	}

	int nStateID = 0;
	if(pSkill->pSBase->bDmgType != 3)
	{
		if(CheckCanBeAtk(pFstTarget) == false)
			return;
		int	nTExCnt	= pSkill->pSBase->bMaxTarget + pSkill->pLvBase->bTargetAdd;
		if(pFstTarget)
			nTExCnt--;
		if(nTExCnt > 0)
		{
			xnList*	pListObj	= xnList::Create();
			switch(pSkill->pSBase->bAoeType)
			{
			case 0:
				pRunPlate->m_pMap->GetObjRange(nX,nY,pSkill->pSBase->wAoeRange+pSkill->pLvBase->wRangeAdd,pListObj);
				break;
			case 1:
				//单体技能
				break;
			case 2:
				if(pFstTarget == NULL)
					return;
				pRunPlate->m_pMap->GetObjRange(pFstTarget->nX,pFstTarget->nY,pSkill->pSBase->wAoeRange+pSkill->pLvBase->wRangeAdd,pListObj);
				break;
			case 3:
				if(pFstTarget == NULL)
					return;
				pRunPlate->m_pMap->GetObjLine(nX,nY,pFstTarget->nX,pFstTarget->nY,pSkill->pSBase->wAoeRange+pSkill->pLvBase->wRangeAdd,pListObj);
				break;
			}

			for (int i = 0; i < pListObj->Count; i++)
			{
				MapAIObj* pTObj	= (MapAIObj*)pListObj->Items[i];
				if(pTObj != pFstTarget)
				{
					if(CheckCanBeAtk(pTObj) == false)
						continue;
					if(pSkill->pSBase->bDmgType == 0)
					{//治疗
						if(CheckCanBeLock(pTObj) && pTObj->bForce == bForce)
						{
							_skillHurt(pTObj,pSkill);
							nTExCnt--;
							//技能的状态效果附加
							for (int s = 0; s < pSkill->pSBase->pListStateAdd->Count; s++)
							{
								_MuliParamObj*	pMP	= (_MuliParamObj*)pSkill->pSBase->pListStateAdd->Items[s];
								if(pMP->nParam1	== 1)
								{
									nStateID = pMP->nParam2+pSkill->pLvBase->bAddStateID;
									if (CheckCanAddState(pTObj,pSkill))
										pTObj->pStateCenter->AddState(nStateID);
								}
								else if(pMP->nParam1 == 0)
								{
									nStateID = pMP->nParam2+pSkill->pLvBase->bAddStateID;
									if (CheckCanAddState(this,pSkill))
										pStateCenter->AddState(nStateID);
								}
							}
						}
					}
					else if(CheckCanLockAttack(pTObj))
					{
						_skillHurt(pTObj,pSkill);
						nTExCnt--;
						//技能的状态效果附加
						for (int s = 0; s < pSkill->pSBase->pListStateAdd->Count; s++)
						{
							_MuliParamObj*	pMP	= (_MuliParamObj*)pSkill->pSBase->pListStateAdd->Items[s];
							if(pMP->nParam1	== 1)
							{
								nStateID = pMP->nParam2+pSkill->pLvBase->bAddStateID;
								if (CheckCanAddState(pTObj,pSkill))
								{
									StateObj* pStateAdd	= pTObj->pStateCenter->AddState(nStateID,byType,dwUin);
									if(pStateAdd)
									{
										pStateAdd->dwFromID		= dwUin;
										pStateAdd->byFromType	= byType;
									}
								}
							}
							else
							{
								nStateID = pMP->nParam2+pSkill->pLvBase->bAddStateID;
								if (CheckCanAddState(this,pSkill))
									pStateCenter->AddState(nStateID);
							}
						}
					}

					if(nTExCnt == 0)
						break;
				}
			}

			pListObj->Free();
		}
		_skillAttack(pFstTarget,pSkill);
	}
	else
	{//召唤
		pRunPlate->m_pMap->MObjCallEnemy(this,pSkill->pLvBase->nP_X,pSkill->pLvBase->nP_Y1);

		if(pCtrl && pCtrl->m_pMainAIObj == this)
		{
			BuildPacketEx(pSendPacket,MAPCMD,MS_CESKILLCD,buf, 256);
			SET_DATA(pSend, MAPCMD, MS_CESKILLCD, pSendPacket);
			pSend->wSkill	= pSkill->pSBase->wSkillID;
			pCtrl->SendPacket(pSendPacket);
		}
	}
	pSkill->ResetCD(this);

	//一次攻击600ms不能行动
	dwBusyTick	= GetTickCount() + 600;
	//一次攻击1200ms不能再攻击
	dwAtkTick	= GetTickCount() + 1200;

	if(byType == MAPOBJ_ENEMY && pSkill->pSBase->nHpCost == 100 && pSkill->pSBase->bCostNoPer == false)
	{
		ChgHp(-xParam.nHp);
		if(pFstTarget)
			pRunPlate->m_pMap->ObjKillReward(pFstTarget,this);
	}
	//技能的状态效果附加

	for (int s = 0; s < pSkill->pSBase->pListStateAdd->Count; s++)
	{
		_MuliParamObj*	pMP	= (_MuliParamObj*)pSkill->pSBase->pListStateAdd->Items[s];
		if(pMP->nParam1	== 0)
		{
			nStateID = pMP->nParam2+pSkill->pLvBase->bAddStateID;
			if (CheckCanAddState(this,pSkill))
				pStateCenter->AddState(nStateID);
		}
		else if( pFstTarget)
		{
			nStateID = pMP->nParam2+pSkill->pLvBase->bAddStateID;
			if (CheckCanAddState(pFstTarget,pSkill))
			{
				StateObj* pSObj	= pFstTarget->pStateCenter->AddState(nStateID,byType,dwUin);
				if(pSObj)
				{
					pSObj->dwFromID	= dwUin;
					pSObj->byFromType	= byType;
				}
			}
		}
	}

	pSkill->wUserTCnt++;
	for (int i = 0; i < pSkill->pSBase->pListTrigEff->Count; i ++)
	{
		SEffTrig*	pSETrip	= (SEffTrig*)pSkill->pSBase->pListTrigEff->Items[i];
		switch(pSETrip->bTrigType)
		{
		case 1:
			{
				if(pSETrip->wTrigVal > rand()%10000)
				{
					SkillObj* pSkillEx	= pSkillCenter->GetSkill(pSETrip->wTrigID+pSkill->pLvBase->bAddSkillID);
					if(!pSkillEx)
						continue;
					if(pSETrip->bTrigEffType == 1 || byType != MAPOBJ_PLAYER)
					{
						if(pSkillEx->pSBase->bAoeType == 0)
						{
							if(pSkillEx->pSBase->bDmgType == 0)
								_useSkill(pSkillEx,this);
							else
								_useSkill(pSkillEx,NULL);
						}
						else
						{
							if(pFstTarget)
								_useSkill(pSkillEx,pFstTarget);
						}
					}
					else
					{

						if(pSkillEx->dwCDTick && ((int)(GetTickCount() - pSkillEx->dwCDTick))<0)
						{

						}
						else
							pSkillCenter->StrikeSkill(pSETrip->wTrigID);
					}
				}
			}
			break;
		case 7:		//次数
			if(pSkill->wUserTCnt >= pSETrip->wTrigVal)
			{
				SkillObj* pSkillEx	= pSkillCenter->GetSkill(pSETrip->wTrigID+pSkill->pLvBase->bAddSkillID);
				if(!pSkillEx)
					continue;
				pSkill->wUserTCnt	= 0;
				if(pSETrip->bTrigEffType == 1 || byType != MAPOBJ_PLAYER)
				{
					if(pSkillEx->pSBase->bAoeType == 0)
					{
						_useSkill(pSkillEx,this);
					}
					else
					{
						if(pFstTarget)
							_useSkill(pSkillEx,pFstTarget);
					}

				}
				else
				{
					pSkillCenter->StrikeSkill(pSETrip->wTrigID);
				}
			}
			break;
		}
	}

}

void MapAIObj::_executeAttack()
{
	if(pStateCenter->m_sznLockAction[4] && GetTickCount() - pStateCenter->m_sznLockAction[4] < 10000)
		return;

	bool	bNeedBack	= false;
	if(pRunPlate->m_pMap->CheckLineBlock(nX,nY,pTarget->nX,pTarget->nY,true))
	{
		if(wLastLineX && wLastLineY && pRunPlate->m_pMap->CheckLineBlock(nX,nY,wLastLineX,wLastLineY) == false)
		{
			int nDistance	= CalcDistance(nX,nY,wLastLineX,wLastLineY);
			if(nDistance	< 10)
			{//追踪目标失败,等待3秒再回去
				if(GetTickCount()-m_dwWarTick >= 5000)
				{
					bNeedBack	= true;
				}
				else
					return;
				//bNeedBack	= true;
			}
			else
			{
				int nDestY	= wLastLineY;
				dwCmdVal	= (nDestY<<16)+wLastLineX;
				//确定跑步动作的时候同步客户端
				//_syncClientCmd(MCMD_RUN,dwCmdVal);
				_executeRun(false);
			}
			
		}
	}


	if(bNeedBack || !CheckCanLockAttack(pTarget))	//暂时躲障碍就回家
	{//不能打了就散步去了
		//SetCmd(MCMD_NULL,0);
		if(pBornInfo)
		{
			int nDX	= nX;
			int nDY	= nY;
			pBornInfo->GetRandPoint(nDX,nDY,pRunPlate->m_pMap,m_byAutoAtk);
			SetCmd(MCMD_RUN,(nDY<<16)+nDX);
		}
		else
			SetCmd(MCMD_NULL,0);
		return;
	}

	int nDistance	= CalcDistance(nX,nY,pTarget->nX,pTarget->nY);
	if(nDistance < 1)
		nDistance = 1;
	int	nMinDis	= 0;
	SkillObj*	pSkill	= _getAtkSkillByDis(nDistance,nMinDis);

	if(nMinDis > 0)
	{//继续追击, 这边需要障碍寻路
		wLastLineX	= pTarget->nX;
		wLastLineY	= pTarget->nY;
		fXMSpeed	= ((float)(pTarget->nX - nX))/nDistance;
		fYMSpeed	= ((float)(pTarget->nY - nY))/nDistance;

		int nDestX	= nX+fXMSpeed*(nDistance - nMinDis);
		int nDestY	= nY+fYMSpeed*(nDistance - nMinDis);
		dwCmdVal	= (nDestY<<16)+nDestX;
		//确定跑步动作的时候同步客户端
		//_syncClientCmd(MCMD_RUN,dwCmdVal);
		_executeRun(false);
	}
	else if(CheckCanAtkCmd())
	{
		m_dwWarTick	= GetTickCount();
		if(pTarget->pBase && pTarget->pBase->bType == 3)
		{//采集
			if(byType == MAPOBJ_PLAYER)
				_castAttack(pTarget);
		}
		else if(pSkill == NULL)
		{//普通攻击
			if(nMinDis != -1)
				_normalAttack(pTarget);
		}
		else
		{//技能攻击
			//对主要目标的攻击
			if(pSkill->pSBase->bAoeType == 0)
			{
				if(pSkill->pSBase->bDmgType ==0)
					_useSkill(pSkill,this);
				else
					_useSkill(pSkill,NULL);
			}
			else
				_useSkill(pSkill,pTarget);
		}
	}
}

void MapAIObj::_syncClientCmd( BYTE bCmd, DWORD dwVal,bool bForce )
{//不管什么指令,dwVal就是目标地点
	switch(byType)
	{
	case MAPOBJ_ENEMY:
		{
			BuildPacketEx(pSPacket, MAPCMD,MS_ENEMYCMD,buf,256);
			SET_DATA(pSend, MAPCMD, MS_ENEMYCMD, pSPacket);
			pSend->dwID		= dwUin;
			pSend->bCmd		= bCmd;
			pSend->dwCmdVal = dwCmdVal;
			if(pSend->bCmd == MCMD_ATTACK)
				pSend->bCmd	= MCMD_RUN;
			pRunPlate->SendPacketOutCtrl(pSPacket);
		}
		break;
	case MAPOBJ_PLAYER:
	case MAPOBJ_ROBOT:
		{
			BuildPacketEx(pSPacket, MAPCMD,MS_USERCMD,buf,256);
			SET_DATA(pSend, MAPCMD, MS_USERCMD, pSPacket);
			pSend->dwID		= dwUin;
			pSend->bCmd		= bCmd;
			pSend->dwCmdVal = dwCmdVal;
			if(bForce)
				pRunPlate->SendPacketOutCtrl(pSPacket);
			else
				pRunPlate->SendPacketOutCtrl(pSPacket,pCtrl);
		}
		break;
	case MAPOBJ_SLAVE:
		{
			BuildPacketEx(pSPacket, MAPCMD,MS_SLAVECMD,buf,256);
			SET_DATA(pSend, MAPCMD, MS_SLAVECMD, pSPacket);
			pSend->dwID		= dwUin;
			pSend->bCmd		= bCmd;
			pSend->dwCmdVal = dwCmdVal;
			pRunPlate->SendPacketOutCtrl(pSPacket);
		}
		break;
	}
}

bool MapAIObj::CheckCanLockAttack( MapAIObj* pTarget )
{
	if(pTarget == NULL)
		return false;
// 	if (pTarget->bForce == 201)
// 		return true;
	if(pTarget->bForce == 0 || bForce == pTarget->bForce)
		return false;
	if(pTarget->CheckCanBeLock(this) == false)
		return false;
	if(pRunPlate->m_pMap->CheckLineBlock(nX,nY,pTarget->nX,pTarget->nY,true))
		return false;	//躲障碍就回去, 后期加入追击脚印系统
	if(pTarget->pStateCenter->m_ifUnvisable)
		return false;
	else if(pTarget->pStateCenter->m_sznLockAction[7] && GetTickCount() - pTarget->pStateCenter->m_sznLockAction[7] < 30000)
		return false;

	if(pBornInfo)
	{//追击范围
		if(abs(pTarget->nY - pBornInfo->wPosY) > pBornInfo->wRangeAtk || abs(pTarget->nX - pBornInfo->wPosX) > pBornInfo->wRangeAtk)
			return false;	
	}
	if(pCtrl && pTarget->pCtrl&&pCtrl->m_pRunMap->m_pMCfgObj->m_wMapIndex != 50)
	{
		if((pCtrl->m_pMainAIObj &&pCtrl->m_pMainAIObj->bLv < 40) ||
			(pTarget->pCtrl->m_pMainAIObj && pTarget->pCtrl->m_pMainAIObj->bLv < 40))
			return false;	//等级保护
	}
	if(pCtrl &&  pTarget->byType == MAPOBJ_ROBOT && pTarget->bLv < 40 )
		return false;	//40级以下的机器人不可被攻击
	if(byType == MAPOBJ_ROBOT && bLv < 40 && pTarget->pCtrl)
		return false;	//40级以下的机器人不可攻击人控制的东西

	return true;
}

bool MapAIObj::CheckCanRunCmd()
{
	if(bDisappear || (dwBusyTick && ((int)(GetTickCount() - dwBusyTick))) < 0 || xParam.nHp <= 0)
		return false;
	if(pStateCenter->m_sznLockAction[0] && GetTickCount() - pStateCenter->m_sznLockAction[0] < 10000)
		return false;

	return true;
}

bool MapAIObj::CheckCanAtkCmd()
{
	if(xParam.nHp <= 0 || dwAtkTick && ((int)(GetTickCount() - dwAtkTick)) < 0)
		return false;
	return true;
}

bool MapAIObj::CheckCanBeAtk(MapAIObj* pObj)
{
	if(pObj && pObj->pBase && (pObj->pBase->bType == 3 || pObj->pBase->bType == 5))
		return false;
	if(pRunPlate && pRunPlate->m_pMap->m_bLockAtk)
		return false;
	if (pObj == NULL)
		return true;

	return CheckCanBeLock(pObj);
	//return true;
}

bool MapAIObj::CheckCanBeLock(MapAIObj* pObj)
{
	if(bDisappear || xParam.nHp <= 0)
		return false;
	if(pObj->nMIndex != nMIndex)
		return false;

	if(abs(pObj->nX-nX) + abs(pObj->nY-nY) > 2000)
		return false;

	return true;
}

static	int	s_sznJobAtkDIs[]	= {300,120,300,300,250,120,250,300};

int MapAIObj::GetObjAtkDis()
{
	switch(byType)
	{
	case MAPOBJ_ENEMY:
		if(pBase)
		{
			return pBase->wAtkRange;
		}
		break;
	case MAPOBJ_PLAYER:
	case MAPOBJ_ROBOT:
		{
			if(byJob < 5)
			{
				return s_sznJobAtkDIs[byJob];
			}
		}
		break;
	case MAPOBJ_SLAVE:
		{
			if(byJob < 7)
				return s_sznJobAtkDIs[byJob];
		}
		break;
	}
	return 300;
}

SkillObj* MapAIObj::_getAtkSkillByDis( int nDis, int& nRqDis )
{//返回-1表示不能攻击, 0普通攻击, 其他为技能ID
	SkillObj*	pSkill	= NULL;
	if(pCtrl==NULL || bContrlType != 10)
	{//怪物,宠物自己找技能用
		pSkill	= pSkillCenter->AutoUserSkill();
	}
	else
	{//先检测是否有CD好的技能可用
		pSkill	= pSkillCenter->CheckUserSkill(wTrySkill);
	}

	if(pSkill)
	{
		if(pSkill->CheckTargetRange(nDis) == false)
		{
			nRqDis	= pSkill->pSBase->wAtkRange;
		}
		else
		{
			wTrySkill	= 0;
			return pSkill;
		}
	}

	//检测普通攻击
	int	nAtkDis	= GetObjAtkDis();
	if(nAtkDis == 0)
		nRqDis	= -1;
	else if(pCtrl && bContrlType == 10)
	{
		if(nDis > nAtkDis+120)
			nRqDis = nAtkDis;
	}
	else if(nDis > nAtkDis+30)
		nRqDis = nAtkDis;

	return NULL;
}

int	_calcNormalDamage(MapAIObj* pA, MapAIObj* pB, int& nType)
{
	int nHit	= pA->xParam.nAim*165/(pA->xParam.nAim+pB->xParam.nDodge);
	if(nHit > 100)
		nHit	= 100;
	if(nHit < 65)
		nHit	= 65;
	if(nHit <= rand()%100)
	{//躲避
		nType	= 3;
		return 0;
	}

	int nAtk	= pA->xParam.nAtkW;
	int	nDef	= pB->xParam.nDefW;
	if(nAtk < pA->xParam.nAtkN)
	{
		nAtk	= pA->xParam.nAtkN;
		nDef	= pB->xParam.nDefN;
	}

	float	fDmgPer	= ((float)nAtk)/(nAtk+nDef);
	int nDamage	= nAtk*fDmgPer;

	int	nCrit	= (pA->xParam.nCrit+pB->bLv*5)*100/(pA->xParam.nCrit+4000+pB->bLv*28) + 3;
	if(nCrit >= 30)
		nCrit	= 30;
	if(nCrit > rand()%100)
	{
		nDamage	 = nDamage*3/2;
		nType	= 2;
	}
	nDamage	= nDamage*(90+rand()%21)/100;

	if(pA->xParam.nPvpDef && pB->xParam.nPvpDef)
	{
		int nPvpDef	= 100*(pB->xParam.nPvpDef+pA->bLv)/(pB->xParam.nPvpDef+pA->bLv*10+100);
		if(nPvpDef < 10)
			nPvpDef	= 10;
		nDamage	= nDamage*nPvpDef/100;
	}
	else if (pA->byType==MAPOBJ_ENEMY && pA->xParam.nPvpDef==0 && pB->xParam.nPvpDef>0 && ((pA->bLv-pB->bLv)>3))
		nDamage	= nDamage*(100+pA->bLv)/100;

	if ((pA->byType == MAPOBJ_PLAYER || pA->byType == MAPOBJ_SLAVE) && pB->byType == MAPOBJ_PLAYER && pA->dwUin != pB->dwUin)
	{
		if (pB->xParam.nPvpJSDef)
			nDamage -= nDamage*pB->xParam.nPvpJSDef/1000;
	}

	if(nDamage < 1)
		nDamage	= 1;
	return nDamage;
}

int	_calcSkillDamage(MapAIObj* pA, MapAIObj* pB, SkillObj* pSkill)
{
	if(pSkill->pSBase->byOnlyEnemy==1 && pB->byType==MAPOBJ_PLAYER)
		return 0;	//只能

	int	nDef	= 0;
	if(pSkill->pSBase->bDmgType == 1)
		nDef	= pB->xParam.nDefW;
	if(pSkill->pSBase->bDmgType == 2)
		nDef	= pB->xParam.nDefN;

	if(nDef && pSkill->pLvBase->wIgnoDef)
		nDef	= nDef*(10000-pSkill->pLvBase->wIgnoDef)/10000;

	int nAtk	= 0;
	if(pSkill->pLvBase->nP_Y2Type == 99)
	{
		nAtk	= pSkill->pLvBase->nP_X	+ pSkill->pLvBase->nP_Y1*pA->xParam.GetParam(pSkill->pLvBase->nP_Y1Type)/100;

		StateObj* pState	= pB->pStateCenter->GetState(pSkill->pLvBase->nP_Y2);
		if(pState)
		{
			nAtk	*= pState->bLv;
		}
	}
	else
	{
		nAtk	= pSkill->pLvBase->nP_X	+ pSkill->pLvBase->nP_Y1*pA->xParam.GetParam(pSkill->pLvBase->nP_Y1Type)/100
										+ pSkill->pLvBase->nP_Y2*pA->xParam.GetParam(pSkill->pLvBase->nP_Y2Type)/100;
	}
	if(nAtk == 0)
		return 0;
	float	fDmgPer	= ((float)nAtk)/(nAtk+nDef);
	int nDamage	= nAtk*fDmgPer;
	//WORD	wAtkRange;
	//BYTE	bAoeType;			//0自身周边范围, 1单体, 2范围, 3直线
	//WORD	wAoeRange;			
	//BYTE	bMaxTarget;			//最大作用个数
	//BYTE	bActType;			//0无,1冲锋  2闪烁  3吸引
	//BYTE	bDmgType;			//1物攻, 2魔攻,0治疗
	nDamage	= nDamage*(90+rand()%21)/100;
	if(pSkill->pSBase->bDmgType == 0)
		nDamage	= -nDamage;
	else if(pA->xParam.nPvpDef && pB->xParam.nPvpDef)
	{
		int nPvpDef	= 100*(pB->xParam.nPvpDef+pA->bLv)/(pB->xParam.nPvpDef+pA->bLv*10+100);
		if(nPvpDef < 10)
			nPvpDef	= 10;
		nDamage	= nDamage*nPvpDef/100;
		if(nDamage < 1)
			nDamage	= 1;
	}
	else if (pA->byType==MAPOBJ_ENEMY && pA->xParam.nPvpDef==0 && pB->xParam.nPvpDef>0 && ((pA->bLv-pB->bLv)>3))
		nDamage	= nDamage*(100+pA->bLv)/100;
	if ((pA->byType == MAPOBJ_PLAYER || pA->byType == MAPOBJ_SLAVE) && pB->byType == MAPOBJ_PLAYER && pSkill->pSBase->bDmgType != 0 && pA->dwUin != pB->dwUin)
	{
		if (pB->xParam.nPvpJSDef)
			nDamage -= nDamage*pB->xParam.nPvpJSDef/1000;
		if(nDamage < 1)
			nDamage	= 1;
	}
	
	return nDamage;
}

void MapAIObj::_skillMiss(MapAIObj* pTarget, SkillObj* pSkill)
{
	BuildPacketEx(pPacket, MAPCMD, MS_ATKINFO, buf, 256);
	SET_DATA(pData, MAPCMD, MS_ATKINFO, pPacket);
	pData->bTSrc	= byType;
	pData->dwIDSrc	= dwUin;
	pData->bTDest	= pTarget->byType;
	pData->dwIDDest	= pTarget->dwUin;
	pData->wSkill	= pSkill->pSBase->wSkillID;
	pData->bModel	= 3;
	pData->nDamage	= 0;
	pData->bAoeHit	= 1;
	pRunPlate->SendPacketOutCtrl(pPacket);
}

void MapAIObj::_skillHurt( MapAIObj* pTarg, SkillObj* pSkill )
{
	int nModel	= 0;
	int nDamage	= _calcSkillDamage(this, pTarg,pSkill);

	//if(nDamage == 0)
	//	return;

	switch(pSkill->pSBase->bActType)
	{
	case 3:	//吸引
		pTarg->RushToTarget(this,3);
		break;
	case 4:
		pTarg->RushToTarget(this,4);
		break;
	}
	pTarg->HurtByObj(this,nDamage);

	//视野关注者都需要知道该过程
	BuildPacketEx(pPacket, MAPCMD, MS_ATKINFO, buf, 256);
	SET_DATA(pData, MAPCMD, MS_ATKINFO, pPacket);
	pData->bTSrc	= byType;		//非主要目标只需要播放受击光效
	pData->dwIDSrc	= dwUin;
	pData->bTDest	= pTarg->byType;
	pData->dwIDDest	= pTarg->dwUin;
	pData->wSkill	= pSkill->pSBase->wSkillID;
	pData->bModel	= nModel;
	pData->nDamage	= nDamage;
	pData->bAoeHit	= 1;
	pRunPlate->SendPacketOutCtrl(pPacket);

	if(nDamage > 0)
	{
		if(pTarg->xParam.nHp <= 0)
		{//
			pTarg->dwTickCmd	= GetTickCount();
			//通知游戏服务器
			pRunPlate->m_pMap->ObjKillReward(this,pTarg);
		}
		else
			pRunPlate->m_pMap->ObjBeHit(pTarg);
	}

}

void MapAIObj::_skillAttack( MapAIObj* pTarg, SkillObj* pSkill )
{
	if(pSkill->pSBase->wSkillID == pSkillCenter->m_wStrikeSkill)
		pSkillCenter->m_wStrikeSkill	= 0;	//触发类技能一次性

	if(pTarg)
	{
		switch(pSkill->pSBase->bActType)
		{
		case 1:	//冲锋
			RushToTarget(pTarg,1);
			break;
		case 2:	//闪烁
			RushToTarget(pTarg,2);
			break;
		case 3:	//吸引
			pTarg->RushToTarget(this,3);
			break;
		case 4:	//击退
			pTarg->RushToTarget(this,4);
			break;
		case 5:
			RushToTarget(pTarg,5);
			break;
		}
	}

	//视野关注者都需要知道该过程
	BuildPacketEx(pPacket, MAPCMD, MS_ATKINFO, buf, 256);
	SET_DATA(pData, MAPCMD, MS_ATKINFO, pPacket);
	pData->bTSrc	= byType;
	pData->dwIDSrc	= dwUin;
	pData->bAoeHit	= 0;
	int nModel	= 0;
	int nDamage	= 0;
	if(pTarg)
	{
		nDamage	= _calcSkillDamage(this, pTarg,pSkill);

		pTarg->HurtByObj(this,nDamage);
		pData->bTDest	= pTarg->byType;
		pData->dwIDDest	= pTarg->dwUin;
		
	}
	else
	{
		pData->bTDest	= 0;
		pData->dwIDDest	= 0;
	}
	pData->nDamage	= nDamage;
	pData->wSkill	= pSkill->pSBase->wSkillID;
	pData->bModel	= nModel;
	
	pRunPlate->SendPacketOutCtrl(pPacket);

	//看不到攻击者的玩家只需要看到怪少血
	if(pTarg)
	{
		if(pTarg->xParam.nHp <= 0)
		{//
			pTarg->dwTickCmd	= GetTickCount();
			//通知游戏服务器
			pRunPlate->m_pMap->ObjKillReward(this,pTarg);

			//客户端自己判断血量为0的情况,建议同步是否出错
		}
		else
			pRunPlate->m_pMap->ObjBeHit(pTarg);
	}


	if(pCtrl && bContrlType == 10)
		SetCmd(MCMD_NULL,0);
}

void MapAIObj::_castAttack( MapAIObj* pTarg )
{
	pTarg->HurtByObj(this,pTarg->xParam.nHp);
	pRunPlate->m_pMap->ObjKillReward(this,pTarg);
}

void MapAIObj::_normalAttack( MapAIObj* pTarg )
{
	//一次攻击600ms不能行动
	dwBusyTick	= GetTickCount() + 600;
	//一次攻击1200ms不能再攻击
	dwAtkTick	= GetTickCount() + 1200;

	int nModel	= 0;
	int nDamage	= _calcNormalDamage(this, pTarg,nModel);

	pTarg->HurtByObj(this,nDamage);

	//视野关注者都需要知道该过程
	BuildPacketEx(pPacket, MAPCMD, MS_ATKINFO, buf, 256);
	SET_DATA(pData, MAPCMD, MS_ATKINFO, pPacket);
	pData->bTSrc	= byType;
	pData->dwIDSrc	= dwUin;
	pData->bTDest	= pTarg->byType;
	pData->dwIDDest	= pTarg->dwUin;
	pData->wSkill	= 0;
	pData->bAoeHit	= 0;
	pData->bModel	= nModel;
	pData->nDamage	= nDamage;
	pRunPlate->SendPacketOutCtrl(pPacket);

	if(pTarg->xParam.nHp <= 0)
	{//
		pTarg->dwTickCmd	= GetTickCount();
		//通知游戏服务器
		pRunPlate->m_pMap->ObjKillReward(this,pTarg);

		//客户端自己判断血量为0的情况,建议同步是否出错
	}
	else
	{
		pRunPlate->m_pMap->ObjBeHit(pTarg);
	}
	if(pCtrl && bContrlType == 10)
		SetCmd(MCMD_NULL,0);
}

bool MapAIObj::ChgMp( int nVal )
{
	if(xParam.nHp <= 0)	//死亡后不能被加扣血
		return false;
	if(nVal + xParam.nMp < 0)
		nVal	= -xParam.nMp;
	if(nVal + xParam.nMp > xParam.nMpMax)
		nVal	= xParam.nMpMax - xParam.nMp;
	if(nVal == 0)
		return false;

	xParam.nMp	+= nVal;
	bGSSyncInfo	= true;

	bSyncMpClient	= true;
	return true;
}

void MapAIObj::ChgHp( int nVal )
{
	if(xParam.nHp <= 0)	//死亡后不能被加扣血
		return;

	pStateCenter->DealDamager(nVal);

	if(nVal + xParam.nHp > xParam.nHpMax)
		nVal	= xParam.nHpMax - xParam.nHp;
	if(nVal == 0)
		return;

	xParam.nHp	+= nVal;

	if(xParam.nHp < 0)
		xParam.nHp	= 0;

	bGSSyncInfo	= true;

	bSyncHpClient	= true;
}

void MapAIObj::HurtByObj( MapAIObj* pObj,int nDamage )
{
	if(pCtrl	== NULL && dwLockAward == 0 && nDamage > 0 && pObj->pCtrl)
	{//怪物归属看第一刀
		dwLockAward	= pObj->pCtrl->m_dwUin;
	}
	if(pCtrl	== NULL&&pObj->pCtrl&&nDamage>0&&pBase&&pBase->nID == nLichBossID){
		m_mapDamage[pObj->pCtrl->m_dwUin] += nDamage;
	}
	m_dwWarTick	= GetTickCount();
	if(bCmd != MCMD_ATTACK && pCtrl == NULL && nDamage > 0 && byType != MAPOBJ_ROBOT)
	{//无控制者的怪物自动反击
		if(pObj->pBase == NULL || pObj->pBase->bAtkType !=0)
		{
			SetCmd(MCMD_ATTACK,0,pObj);
		}
	}
	if(nDamage	== 0)
		return;

	if(nDamage > 0)
	{
		if(pStateCenter->m_ifUnhurt)
			return;
		else if(pStateCenter->m_sznLockAction[1] && GetTickCount() - pStateCenter->m_sznLockAction[1] < 10000)
			return;
	}

	ChgHp(-nDamage);

	if(nDamage < 0)
	{//治疗
		if(byType == MAPOBJ_PLAYER && pCtrl)
		{
			pCtrl->m_dwHealthTotal	+= -nDamage;
		}
	}
	else 
	{
		if(byType == MAPOBJ_PLAYER && pCtrl)
		{
			pCtrl->m_dwHitTotal		+= nDamage;
		}
		if(pObj->byType	== MAPOBJ_PLAYER && pObj->pCtrl)
		{
			pObj->pCtrl->m_dwDamageTotal	+= nDamage;
		}
	}

	if(byType == MAPOBJ_ENEMY && pCtrl == NULL && pBase->bType == 2)
	{//仇恨系统
		if(nDamage > 10)
			AddHitCnt(pObj->byType,pObj->dwUin,nDamage);
	}
}

void MapAIObj::_syncTitleChg()
{
	if(!pRunPlate)
		return;
	BuildPacketEx(pPacket,MAPCMD,MS_TITLECHG,buf,1024);
	SET_DATA(pData,MAPCMD,MS_TITLECHG,pPacket);
	strcpy(pData->szTitle,szTitle);
	pData->byType = byType;
	pData->dwID = dwUin;
	pRunPlate->SendPacketExCtrl(pPacket,pCtrl);
}

void MapAIObj::_syncMarryTitleChg()
{
	if(!pRunPlate)
		return;
	BuildPacketEx(pPacket,MAPCMD,MS_MARRYTITLECHG,buf,1024);
	SET_DATA(pData,MAPCMD,MS_MARRYTITLECHG,pPacket);
	strcpy(pData->szTitle,szMarryTitle);
	pData->byType = byType;
	pData->dwID = dwUin;
	pRunPlate->SendPacketExCtrl(pPacket,pCtrl);
}

void MapAIObj::_syncMarryEffChg()
{
	if(!pRunPlate)
		return;
	BuildPacketEx(pPacket,MAPCMD,MS_MARRYEFFCHG,buf,1024);
	SET_DATA(pData,MAPCMD,MS_MARRYEFFCHG,pPacket);
	pData->nEff = nMarryEff;
	pData->byType = byType;
	pData->dwID = dwUin;
	pRunPlate->SendPacketExCtrl(pPacket,pCtrl);
}

void MapAIObj::_syncViplvChg()
{
	if(!pRunPlate)
		return;
	BuildPacketEx(pPacket,MAPCMD,MS_VIPLVCHG,buf,1024);
	SET_DATA(pData,MAPCMD,MS_VIPLVCHG,pPacket);
	pData->byViplv = byViplv;
	pData->byType = byType;
	pData->dwID = dwUin;
	pRunPlate->SendPacketExCtrl(pPacket,pCtrl);
}

void MapAIObj::_syncSkillChg()
{
	if(!pCtrl || bContrlType != 10)
		return;
	BuildPacketEx(pSPacket, MAPCMD,MS_ACTSKILL,buf,1024);
	SET_DATA(pSend, MAPCMD, MS_ACTSKILL, pSPacket);
	pSend->bSCnt	= 0;
	DWORD	dwTickCur	= GetTickCount();
	for (int i = 0; i < MAX_ACTIVE_SKILL; i++)
	{
		if(pSkillCenter->m_ppActSkill[i] && pSkillCenter->m_ppActSkill[i]->byPos)
		{
			pSend->szSkill[pSend->bSCnt].wSkillID	= pSkillCenter->m_ppActSkill[i]->pSBase->wSkillID;
			pSend->szSkill[pSend->bSCnt].bLv		= pSkillCenter->m_ppActSkill[i]->bySLv;
			pSend->szSkill[pSend->bSCnt].bPos		= pSkillCenter->m_ppActSkill[i]->byPos-1;
			if(pSkillCenter->m_ppActSkill[i]->dwCDTick)
				pSend->szSkill[pSend->bSCnt].nCDTick	= pSkillCenter->m_ppActSkill[i]->dwCDTick - dwTickCur;
			else
				pSend->szSkill[pSend->bSCnt].nCDTick	= 0;
			pSend->bSCnt++;
		}
	}

	pSPacket->wSize	+= (pSend->bSCnt-1)*sizeof(MAPCMD_MS_SKILLCHG_Data);
	pCtrl->SendPacket(pSPacket);
}

void MapAIObj::_syncParamChg()
{
	if(!pRunPlate/* || !pCtrl*/)
		return;
	BuildPacketEx(pSPacket, MAPCMD,MS_PARAMCHG,buf,1024);
	SET_DATA(pSend, MAPCMD, MS_PARAMCHG, pSPacket);
	pSend->dwID		= dwUin;
	pSend->bType	= byType;
	pSend->nHp		= xParam.nHp;
	pSend->nMaxHp	= xParam.nHpMax;
	pSend->wSpeed	= xParam.nMSpeed;
	pSend->byLv		= bLv;
	pSend->wSkin[0]	= wSkin[0];
	pSend->wSkin[1]	= wSkin[1];
	pSend->wSkin[2]	= wSkin[2];

	pRunPlate->SendPacketExCtrl(pSPacket,pCtrl);
	bSyncSpeed		= false;
	bSyncHpMaxClient	= false;

	//BuildPacketEx(pSPacketex, MAPCMD,MS_MPINFO,bufex,1024);
	//SET_DATA(pSendex, MAPCMD, MS_MPINFO, pSPacketex);
	//pSendex->nMp	= xParam.nMp;
	//pSendex->nMaxMp	= xParam.nMpMax;
	//pCtrl->SendPacket(pSPacketex);
	//bSyncMpClient	= false;

}

void MapAIObj::JumpInMap(int wX, int wY)
{
	nX	= wX;
	nY	= wY;

	int	nMaxLoop = 10;
	while(nMaxLoop-- > 0 && pRunPlate->m_pMap->GetBlock(nX,nY))
	{
		nX	= nX -100+rand()%200;
		nY	= nY -100+rand()%200;
	}

	if(nX >= pRunPlate->m_pMap->m_pMCfgObj->m_wMapSizeW)
		nX	= pRunPlate->m_pMap->m_pMCfgObj->m_wMapSizeW-1;
	if(nX < 0)
		nX	= 0;
	if(nY >= pRunPlate->m_pMap->m_pMCfgObj->m_wMapSizeH)
		nY	= pRunPlate->m_pMap->m_pMCfgObj->m_wMapSizeH-1;
	if(nY < 0)
		nY	= 0;

	if(bCmd == MCMD_RUN)
		SetCmd(MCMD_NULL,0);

	BuildPacketEx(pSPacket, MAPCMD,MS_JUMPINMAP,buf,256);
	SET_DATA(pSend, MAPCMD, MS_JUMPINMAP, pSPacket);
	pSend->dwUin		= dwUin;
	pSend->byType	= byType;
	pSend->wPosX	= nX;
	pSend->wPosY	= nY;
	pRunPlate->SendPacketExCtrl(pSPacket,pCtrl);
}

void MapAIObj::Update()
{
	m_dwUpdateTime++;
	pStateCenter->Update();

	if(m_dwUpdateTime%10==0)
	{//second
		CheckHealth();

		if(byType == MAPOBJ_ENEMY && pBase->bType == 2 && pCtrl == NULL)
		{
			if(m_dwUpdateTime%50 == 0)
			{//5秒一次选目标
				MapAIObj*	pAIObj	= PopTarget();
				if(pAIObj)
				{
					SetCmd(MCMD_ATTACK,0,pAIObj);
					if(pAIObj->pCtrl)
						dwLockAward	= pAIObj->pCtrl->m_dwUin;
					else
						dwLockAward	= 0;
				}
				else
					dwLockAward	= 0;
			}
		}
		if(byType == MAPOBJ_ROBOT)
		{
			if(m_dwUpdateTime%600 == 0)
			{//机器人一分钟换一个名字
				szTitle[0]	= rand();
				szTitle[1]	= rand();
				szTitle[2]	= rand();
				szTitle[3]	= 0;
			}
		}
	}


	if(pStateCenter->m_bParamChged)
	{
		int	nOldMaxMp	= xParam.nMpMax;
		int	nOldSpeed	= xParam.nMSpeed;
		int	nOldMaxHp	= xParam.nHpMax;
		pStateCenter->FillParam(&xParam);

		if(nOldMaxHp != xParam.nHpMax)
			bSyncHpMaxClient	= true;
		if(nOldMaxMp != xParam.nMpMax)
			bSyncMpClient	= true;
		if(nOldSpeed != xParam.nMSpeed)
			bSyncSpeed		= true;
		pStateCenter->m_bParamChged	= false;
	}
	if(pStateCenter->m_bSkinChged)
	{
		wSkin[0]	= pStateCenter->GetViewSkin(0);
		wSkin[1]	= pStateCenter->GetViewSkin(1);
		wSkin[2]	= pStateCenter->GetViewSkin(2);
		pStateCenter->m_bSkinChged	= false;

		_syncParamChg();
	}

	pStateCenter->m_bParamChged	= false;
	if(bSyncHpClient)
	{
		bSyncHpClient	= false;
		
		//看不到攻击者的玩家只需要看到怪少血
		BuildPacketEx(pPacket, MAPCMD, MS_HPINFO, bufEx, 256);
		SET_DATA(pSend, MAPCMD, MS_HPINFO, pPacket);
		pSend->bType	= byType;
		pSend->dwID		= dwUin;
		pSend->nHp		= xParam.nHp;
		pRunPlate->SendPacketOutCtrl(pPacket);
	}
	if(bSyncHpMaxClient)
	{
		bSyncHpMaxClient	= false;
		
		//看不到攻击者的玩家只需要看到怪少血
		BuildPacketEx(pPacket, MAPCMD, MS_HPMAXINFO, bufEx, 256);
		SET_DATA(pSend, MAPCMD, MS_HPMAXINFO, pPacket);
		pSend->bType	= byType;
		pSend->dwID		= dwUin;
		pSend->nHp		= xParam.nHp;
		pSend->nMaxHp	= xParam.nHpMax;
		pRunPlate->SendPacketOutCtrl(pPacket);
	}


	if(bSyncSpeed)
	{
		bSyncSpeed	= false;
		BuildPacketEx(pPacket, MAPCMD, MS_SPEEDCHG, bufEx, 256);
		SET_DATA(pSend, MAPCMD, MS_SPEEDCHG, pPacket);
		pSend->byType	= byType;
		pSend->dwID		= dwUin;
		pSend->wSpeed	= xParam.nMSpeed;
		pRunPlate->SendPacketOutCtrl(pPacket);
	}
	if(byType == MAPOBJ_PLAYER && bSyncMpClient)
	{//只需要给自己发
		bSyncMpClient	= false;
		BuildPacketEx(pPacket, MAPCMD, MS_MPINFO, bufEx, 256);
		SET_DATA(pSend, MAPCMD, MS_MPINFO, pPacket);
		pSend->nMp		= xParam.nMp;
		pSend->nMaxMp	= xParam.nMpMax;
		pCtrl->SendPacket(pPacket);
	}

}

void MapAIObj::ResetParam()
{
	pStateCenter->ResetBaseParam(&xParam);

	pStateCenter->m_szwStateSkin[0][0]	= wSkin[0];
	pStateCenter->m_szwStateSkin[0][1]	= wSkin[1];
	pStateCenter->m_szwStateSkin[0][2]	= wSkin[2];

	if(pStateCenter->m_bParamChged)
	{

		pStateCenter->FillParam(&xParam);
		pStateCenter->m_bParamChged	= false;
	}
	if(pStateCenter->m_bSkinChged && byType == MAPOBJ_PLAYER)
	{
		wSkin[0]	= pStateCenter->GetViewSkin(0);
		wSkin[1]	= pStateCenter->GetViewSkin(1);
		wSkin[2]	= pStateCenter->GetViewSkin(2);
		pStateCenter->m_bSkinChged	= false;
	}

	_syncParamChg();
}

void MapAIObj::SetControl( ControlObj* pC,BYTE bType )
{
	pCtrl		= pC;
	bContrlType	= bType;

	if(bDisappear == false && pRunPlate)
	{
		BuildPacketEx(pPacket, MAPCMD, MS_OBJCHGCTRL, buf, 256);
		SET_DATA(pSend, MAPCMD, MS_OBJCHGCTRL, pPacket);
		pSend->bType	= byType;
		pSend->dwID		= dwUin;
		pSend->bForce	= bForce;
		if(pCtrl)
			pSend->dwMast	= pCtrl->m_dwUin;
		else
			pSend->dwMast	= 0;
		pRunPlate->SendPacketExCtrl(pPacket,pCtrl);
	}
}

void MapAIObj::CheckHealth()
{
	if(xParam.nHp <= 0)
	{
		if(byType == MAPOBJ_ROBOT)
		{//机器人自动复活??
			if(GetTickCount()-m_dwWarTick >= 5000)
			{
				xParam.nHp	= 1;
				ChgHp(xParam.nHpMax);
			}
		}
		return;
	}
	DWORD	dwCurTick	= GetTickCount();
	if(dwCurTick-m_dwWarTick >= 5000)
	{//脱离战斗
		if(dwCurTick - m_dwHealthTick >= 3000)
		{
			if(byType == MAPOBJ_ENEMY)
			{
				ChgHp(xParam.nHpMax);
			}
			else
			{
				ChgHp(xParam.nHpMax*3/100);
				ChgMp(xParam.nMpMax*3/100);
			}
			m_dwHealthTick	= GetTickCount();
		}
	}
}

bool MapAIObj::RushToTarget( MapAIObj* pObj, BYTE bAction )
{//1, 冲锋  2闪烁  3被拉过去  4击退300, 5拉开距离
	if(pBase && (pBase->bAtkType == 0 || pBase->nSpeed == 0))
		return false;	//不动怪不能被改变位置

	int nDistance	= CalcDistance(nX,nY,pObj->nX,pObj->nY);
	dwBusyTick		= GetTickCount()+800;
	int nDestX	= 0;
	int nDestY	= 0;

	switch(bAction)
	{
	case 4:
		if(nDistance > 10)
		{
			fXMSpeed	= ((float)(nX-pObj->nX))/nDistance;
			fYMSpeed	= ((float)(nY-pObj->nY))/nDistance;

			nDestX	= nX+fXMSpeed*( 300);
			nDestY	= nY+fYMSpeed*( 300);

			int nTmpX	= 0;
			int nTmpY	= 0;
			if(pRunPlate->m_pMap->m_pMCfgObj->m_pMapBarr->CheckLineBlock(nX,nY,nDestX,nDestY,&nTmpX,&nTmpY))
			{
				if(nTmpX || nTmpY)
				{
					nDestX	= nTmpX;
					nDestY	= nTmpY;
				}
			}
		}
		break;
	case 5:
		if(nDistance > 10)
		{
			fXMSpeed	= ((float)(nX-pObj->nX))/nDistance;
			fYMSpeed	= ((float)(nY-pObj->nY))/nDistance;

			nDestX	= nX+fXMSpeed*( 200);
			nDestY	= nY+fYMSpeed*( 200);

			int nTmpX	= 0;
			int nTmpY	= 0;
			if(pRunPlate->m_pMap->m_pMCfgObj->m_pMapBarr->CheckLineBlock(nX,nY,nDestX,nDestY,&nTmpX,&nTmpY))
			{
				if(nTmpX || nTmpY)
				{
					nDestX	= nTmpX;
					nDestY	= nTmpY;
				}
			}
		}
		break;
	case 1:
		if(nDistance > 150)
		{//继续追击
			fXMSpeed	= ((float)(pObj->nX - nX))/nDistance;
			fYMSpeed	= ((float)(pObj->nY - nY))/nDistance;

			nDestX	= nX+fXMSpeed*(nDistance - 80);
			nDestY	= nY+fYMSpeed*(nDistance - 80);
		}
		else if(nDistance > 10)
		{
			if(pObj->RushToTarget(this,4))
			{
				RushToTarget(pObj,10);
				return true;
			}
			else
				return false;
		}
		break;
	case 10:
		if(nDistance > 150)
		{//继续追击
			fXMSpeed	= ((float)(pObj->nX - nX))/nDistance;
			fYMSpeed	= ((float)(pObj->nY - nY))/nDistance;

			nDestX	= nX+fXMSpeed*(nDistance - 80);
			nDestY	= nY+fYMSpeed*(nDistance - 80);
			bAction	= 1;
		}
		break;
	case 2:
	case 3:
		if(nDistance > 80)
		{//继续追击
			fXMSpeed	= ((float)(pObj->nX - nX))/nDistance;
			fYMSpeed	= ((float)(pObj->nY - nY))/nDistance;

			nDestX	= nX+fXMSpeed*(nDistance - 80);
			nDestY	= nY+fYMSpeed*(nDistance - 80);
		}
		break;
	}

	if(nDestX || nDestY)
	{
		nX	= nDestX;
		nY	= nDestY;

		if(nX >= pRunPlate->m_pMap->m_pMCfgObj->m_wMapSizeW)
			nX	= pRunPlate->m_pMap->m_pMCfgObj->m_wMapSizeW-1;
		if(nX < 0)
			nX	= 0;
		if(nY >= pRunPlate->m_pMap->m_pMCfgObj->m_wMapSizeH)
			nY	= pRunPlate->m_pMap->m_pMCfgObj->m_wMapSizeH-1;
		if(nY < 0)
			nY	= 0;

		if(bCmd == MCMD_RUN)
			SetCmd(MCMD_NULL,0);

		BuildPacketEx(pSPacket, MAPCMD,MS_RUSHINMAP,buf,256);
		SET_DATA(pSend, MAPCMD, MS_RUSHINMAP, pSPacket);
		pSend->dwUin		= dwUin;
		pSend->byType	= byType;
		pSend->bAction	= bAction;
		pSend->wPosX	= nX;
		pSend->wPosY	= nY;
		pRunPlate->SendPacketExCtrl(pSPacket,pCtrl);
	}

	return true;
}

void MapAIObj::AddHitCnt( BYTE byType, DWORD dwID, int nAdd )
{
	for (int i = 0; i < pListHitCnt->Count; i++)
	{
		_HitCountObj*	pHit	= (_HitCountObj*)pListHitCnt->Items[i];
		if(pHit->bType == byType && pHit->dwID == dwID)
		{
			pHit->dwHitCnt	+= nAdd;
			return;
		}
	}
	_HitCountObj* pHit	= new _HitCountObj();
	pHit->bType	= byType;
	pHit->dwID	= dwID;
	pHit->dwHitCnt	= nAdd;
	pListHitCnt->Add(pHit);
}

MapAIObj* MapAIObj::PopTarget()
{
	if(pListHitCnt->Count==0 || pRunPlate == NULL)
		return NULL;

	_HitCountObj*	pHFind	= NULL;

	for (int i = 0; i < pListHitCnt->Count; i++)
	{
		_HitCountObj* pHit	= (_HitCountObj*)pListHitCnt->Items[i];

		if(pHFind)
		{
			if(pHFind->dwHitCnt >= pHit->dwHitCnt)
				continue;
		}
		pHFind	= pHit;
	}
	if(!pHFind)
		return NULL;

	MapAIObj* pRtl	= pRunPlate->m_pMap->GetMapObj(pHFind->bType,pHFind->dwID);
	if(pRtl == NULL || CheckCanBeAtk(pRtl) == false)
	{
		pListHitCnt->Remove(pHFind);
		delete pHFind;
		return NULL;
	}
	pHFind->dwHitCnt	= pHFind->dwHitCnt/10*9;
	return pRtl;

}




