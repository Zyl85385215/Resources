#include "stdafx.h"
#include "HomeBattleCenter.h"
#include "FamilyObject.h"

CreateSingleton(HomeBattleCenter);
extern int Random_Int(int min, int max);
bool HomeBattleCenter::GetBattleResult( HomeBattleObj** pAtkObj,HomeBattleObj** pDefObj,xnList* pList )
{
	int i=0;
	HomeBattleObj* pTarget = NULL;
	while(1){
		if (i%2 == 0&&pAtkObj[i/2%6]&&pAtkObj[i/2%6]->nNowHp>0)	//¹¥»÷·½
		{
			pTarget = ChooseTarget(pDefObj,i/2%6);
			if(!pTarget) return true;
			BattleStep* pStep = AttackTarget(pAtkObj[i/2%6],pTarget,true);
			pList->Add(pStep);
		}else if (i%2 != 0&&pDefObj[i/2%6]&&pDefObj[i/2%6]->nNowHp>0)
		{
			pTarget = ChooseTarget(pAtkObj,i/2%6);
			if(!pTarget) return false;
			BattleStep* pStep = AttackTarget(pDefObj[i/2%6],pTarget,false);
			pList->Add(pStep);
		}
		i++;
	}
	return false;
}

HomeBattleObj* HomeBattleCenter::ChooseTarget( HomeBattleObj** pObj ,int nNowPos)
{
	if (nNowPos>=BattlePosMax||nNowPos<0)
	{
		return NULL;
	}
	HomeBattleObj* pTarget = NULL;
	for (int i=0;i<BattlePosMax;i++)
	{
		if (!pTarget&&pObj[i]&&pObj[i]->nNowHp>0)
		{
			pTarget = pObj[i];
		}
		if (i%3 == nNowPos%3&&pObj[i]&&pObj[i]->nNowHp>0)
		{
			pTarget = pObj[i];
			break;
		}
	}
	return pTarget;
}

BattleStep* HomeBattleCenter::AttackTarget( HomeBattleObj* pAtkObj,HomeBattleObj* pDefObj ,bool bAtk)
{
	BattleStep* pStep = new BattleStep;
	int nAtk = Random_Int(pAtkObj->nAtkMin,pAtkObj->nAtkMax);
	int nDef = Random_Int(pDefObj->nDefMin,pDefObj->nDefMax);
	int nDamage = nAtk-nDef;
	if (nDamage<=0)
	{
		nDamage = 1;
	}
	pDefObj->nNowHp -= nDamage;
	pStep->nDamage = nDamage;
	pStep->nAtkPos = pAtkObj->nPos;
	pStep->nDefPos = pDefObj->nPos;
	pStep->bAtk = bAtk;
	return pStep;
}

bool HomeBattleCenter::GetAllBattleResult( HomeBattleObj** pAtkObj,HomeBattleObj** pDefObj,xnList* pList )
{
	bool bAtkWin = false;
	while(ResetBattleObj(pAtkObj,pDefObj)){
		BattleStepInfo* pInfo = new BattleStepInfo;
		for (int i=0;i<6;i++)
		{
			if (pAtkObj[i]) {
				pInfo->BattleObjAtk[i] = (*pAtkObj[i]);
			}else{
				pInfo->BattleObjAtk[i].dwUin = 0;
			}
			if (pDefObj[i]) {
				pInfo->BattleObjDef[i] = (*pDefObj[i]);
			}else{
				pInfo->BattleObjDef[i].dwUin = 0;
			}
		}
		pInfo->pBattleStep = xnList::Create();
		pList->Add(pInfo);
		bAtkWin = GetBattleResult(pAtkObj,pDefObj,pInfo->pBattleStep);
		pInfo->byResult = bAtkWin;
	}
	for (int i=0;i<18;i++)
	{
		if (pAtkObj[i])
		{
			delete pAtkObj[i];
			pAtkObj[i] = NULL;
		}
		if (pDefObj[i])
		{
			delete pDefObj[i];
			pDefObj[i] = NULL;
		}
	}
	return bAtkWin;
}

bool HomeBattleCenter::ResetBattleObj( HomeBattleObj** pAtkObj,HomeBattleObj** pDefObj )
{
	for (int i=0;i<6;i++)
	{
		if (pAtkObj[i]&&pAtkObj[i]->nNowHp<=0)
		{
			delete pAtkObj[i];
			pAtkObj[i] = NULL;
		}
		if (!pAtkObj[i])
		{
			for (int n=1;3*n+i<18;n++)
			{
				if (pAtkObj[3*n+i]&&pAtkObj[3*n+i]->nNowHp>0)
				{
					pAtkObj[i] = pAtkObj[3*n+i];
					pAtkObj[i]->nPos = i;
					pAtkObj[3*n+i] = NULL;
					break;
				}
			}
		}
	}
	for (int i=0;i<6;i++)
	{
		if (pDefObj[i]&&pDefObj[i]->nNowHp<=0)
		{
			delete pDefObj[i];
			pDefObj[i] = NULL;
		}
		if (!pDefObj[i])
		{
			for (int n=1;3*n+i<18;n++)
			{
				if (pDefObj[3*n+i]&&pDefObj[3*n+i]->nNowHp>0)
				{
					pDefObj[i] = pDefObj[3*n+i];
					pDefObj[i]->nPos = i;
					pDefObj[3*n+i] = NULL;
					break;
				}
			}
		}
	}
	return (pAtkObj[0]||pAtkObj[1]||pAtkObj[2]||pAtkObj[3]||pAtkObj[4]||pAtkObj[5])&&(pDefObj[0]||pDefObj[1]||pDefObj[2]||pDefObj[3]||pDefObj[4]||pDefObj[5]);
}

BattleStepInfo::~BattleStepInfo()
{
	while(pBattleStep->Count) delete pBattleStep->Delete(0);
}
