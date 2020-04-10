#pragma once
#include "singleton.h"
#include "xnlist.h"
#include "FamilyObject.h"
#include "ios_home_packet.h"
struct BattleStepInfo{
	~BattleStepInfo();
	HomeBattleObj BattleObjAtk[6];
	HomeBattleObj BattleObjDef[6];
	xnList* pBattleStep;
	byte byResult;
};

const int BattlePosMax = 6;
const int BattleAllPosMax = 18;

class HomeBattleCenter:public CSingleton<HomeBattleCenter>{
public:
	bool GetBattleResult(HomeBattleObj** pAtkObj,HomeBattleObj** pDefObj,xnList* pList);
	bool GetAllBattleResult(HomeBattleObj** pAtkObj,HomeBattleObj** pDefObj,xnList* pList);
private:
	HomeBattleObj* ChooseTarget(HomeBattleObj** pObj,int nNowPos);
	BattleStep* AttackTarget(HomeBattleObj* pAtkObj,HomeBattleObj* pDefObj,bool bAtk);
	bool ResetBattleObj(HomeBattleObj** pAtkObj,HomeBattleObj** pDefObj);
};