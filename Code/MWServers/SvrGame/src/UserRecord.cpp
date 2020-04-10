#include "stdafx.h"
#include "UserRecord.h"
#include "UserData.h"
#include "ios_activity_packet.h"
#include "ActivedegreeCfg.h"
#include "FsGameCenter.h"
#include "DBLogControl.h"
#include "ZLogFile.h"
#include "ServerCfg.h"

extern ZLogFile* g_pLogFile;
extern CLuaStateManager * pLuaStateManager;

UserRecord::UserRecord(UserData* pUser)
{
	m_pList		= xnList::Create();
	m_pParent	= pUser;

	CLuaClassObject<UserRecord>::Init(this);
}

UserRecord::~UserRecord()
{
	while(m_pList->Count)
		delete (URInfoObj*)m_pList->Delete(0);
	m_pList->Free();
}

void UserRecord::CreateScriptClass( CLuaState * pLuaState )
{
	CLuaClass<UserRecord> * pscRecordCenter = pLuaState->CreateScriptClass((UserRecord*)0);

// 	pscItemCenter->DefObjectFunction("GetBlankBag", &ItemCenter::GetBlankBag)
// 		.DefObjectFunction("AddItemByID", &ItemCenter::AddItemByID)
// 		.DefObjectFunction("RemoveItemByIndex", &ItemCenter::RemoveItemByIndex)
// 		.DefObjectFunction("GetItemByIndex", &ItemCenter::GetItemByIndex)
// 		.DefObjectFunction("CheckItemBlankBagByID", &ItemCenter::CheckItemBlankBagByID)
// 		.DefObjectFunction("GetUser", &ItemCenter::GetUser)
// 		.DefObjectFunction("AddBagSize", &ItemCenter::_addBagSize)
// 		.DefObjectFunction("GetBagSize", &ItemCenter::GetBagSize)
// 		.DefObjectFunction("GetItemCountByID", &ItemCenter::GetItemCountByID);
}

void UserRecord::ReleaseScriptClass( CLuaState * pLuaState )
{
	pLuaState->ReleaseScriptClass((UserRecord*)0);
}

void UserRecord::Update()
{
	while(m_pList->Count > 0)
	{
		URInfoObj* pObj = (URInfoObj*)m_pList->Delete(0);
		_checkDegree(pObj);
		_checkAchievement(pObj);
		delete pObj;
	}
}

void UserRecord::AddRecord( BYTE	bUA,BYTE byUO, int nID, int nVal )
{
	URInfoObj* pObj = new URInfoObj();
	pObj->bAction = bUA;
	pObj->bObject = byUO;
	pObj->nID = nID;
	pObj->nVal = nVal;
	pObj->dwTick = GetTickCount();
	m_pList->Add((void*)pObj);
	if(byUO == UO_QUEST)
		RunLuaQuestFunc(bUA,nID,nVal);
	else if(byUO == UO_MESSAGE)
		RunLuaChatFunc(nID);
}

bool UserRecord::CheckRecord( BYTE bUR, int nTick )
{


	return false;
}


bool UserRecord::_checkDegree(URInfoObj* pObj)
{
	ADDaylyPart* pPartA = NULL;
	ADPartChoose* pPartB = NULL;
	for(int i = 0; i < m_pParent->m_pDegreeList->Count; i ++)
	{
		UserDegreePart* pTemp = (UserDegreePart*)m_pParent->m_pDegreeList->Items[i];
		if(pTemp->ifDone)
			continue;
		if(i >= ActiveDegree::GetInstance().m_pDaylyList->Count)
			continue;
		pPartA = (ADDaylyPart*)ActiveDegree::GetInstance().m_pDaylyList->Items[i];
		if(pTemp->byType >= pPartA->pList->Count)
			continue;
		pPartB = (ADPartChoose*)pPartA->pList->Items[pTemp->byType];
		if(pPartB->byAction == pObj->bAction && pPartB->byObject == pObj->bObject)
		{
			if((pPartB->nID == -1 || pPartB->nID == pObj->nID) && (pPartB->nVal == -1 || pPartB->nVal == pObj->nVal))
				g_pGameCenter->m_pActivityCenter->AddDegreeVal(m_pParent,i,1);
			RunLuaDegreeFunc(g_pGameCenter->m_pActivityCenter->GetDegreeRlt(m_pParent));
		}
	}
	return true;
}

bool UserRecord::_checkAchievement(URInfoObj* pObj)
{
	return true;
}

void UserRecord::SaveAllLog()
{
	AddGoldLog(m_pParent->m_xUWealth[UW_GOLD]);
	AddExpLog(m_pParent->m_xUWealth[UW_EXP],m_pParent->m_xUWealth[UW_LV]);
	if(m_pParent->m_xUWealth[UW_RMB] > 0)
		AddRmbLog(m_pParent->m_xUWealth[UW_RMB],0,RMB_SAVE_NULL);
	if(m_pParent->m_xUWealth[UW_BINDRMB] > 0)
		AddBindRmbLog(m_pParent->m_xUWealth[UW_BINDRMB],0,RMB_SAVE_NULL);
}

bool UserRecord::AddGoldLog(int nVal)
{
	return g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_GOLD_LOG" (uin,money,ctime) values (%d,%d,now())",m_pParent->m_dwUin,nVal);
}

bool UserRecord::AddExpLog(int nVal,BYTE byLvl)
{
	return g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_EXP_LOG" (uin,exp,lvl,ctime) values (%d,%d,%d,now())",m_pParent->m_dwUin,nVal,byLvl);
}

bool UserRecord::AddLvupLog(BYTE byLv)
{
	return g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_LVUP_LOG" (uin,lvl,ctime) values (%d,%d,now())",m_pParent->m_dwUin,byLv);
}

bool UserRecord::AddRmbLog(int nLeftVal,int nVal,BYTE byType,char* szDesc)
{
	return g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_RMB_LOG" (uin,accuin,leftrmb,addval,type,ctime,logdesc) values (%d,%d,%d,%d,%d,now(),'%s')",m_pParent->m_dwUin,m_pParent->m_dwAccount,nLeftVal,nVal,byType,szDesc?szDesc:szRmbSaveName[byType]);
}

bool UserRecord::AddBindRmbLog(int nLeftVal,int nVal,BYTE byType)
{
	return g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_BINDRMB_LOG" (uin,leftrmb,addval,type,ctime,logdesc) values (%d,%d,%d,%d,now(),'%s')",m_pParent->m_dwUin,nLeftVal,nVal,byType,szRmbSaveName[byType]);
}

bool UserRecord::AddItemLog( int nItemID,int nCount,int nType )
{
	return g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_ITEM_LOG" (uin,itemid,itemcnt,itemtype,ctime) values (%d,%d,%d,%d,now())",m_pParent->m_dwUin,nItemID,nCount,nType);

}

bool UserRecord::AddSlaveLog(int nID,int nBaseid,BYTE bylvl,BYTE byVariatlv,BYTE byAction)
{
	return g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_SLAVE_LOG" (uin,slaveid,baseid,lvl,variatlv,action,ctime,logdesc) values (%d,%d,%d,%d,%d,%d,now(),'%s')",m_pParent->m_dwUin,nID,nBaseid,bylvl,byVariatlv,byAction,szSlaveSaveName[byAction]);
}

bool UserRecord::AddGMCodeLog(const char* szMsg)
{
	return g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_GMCODE_LOG" (uin,name,ctime,code) values (%d,'%s',now(),'%s')",m_pParent->m_dwUin,m_pParent->m_szNick,szMsg);
}

bool	UserRecord::RunLuaQuestFunc(BYTE byAction,int nQID,BYTE byType)
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	if(byAction == UA_GAIN)
		sprintf(sztxt,"OnGainQuestAction");
	else if(byAction == UA_COMPLETE)
		sprintf(sztxt,"OnCompleteQuestAction");
	else if(byAction == UA_LOST)
		sprintf(sztxt,"OnGiveupQuestAction");
	else
		return false;
	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);
	CLuaPlusSafeFunction_4<bool,UserData*,int,int> func;
	if(!func(pLuaState, lf, &bRet,m_pParent, nQID,byType))
	{
		g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
		return false;
	}
	return bRet;
}

bool	UserRecord::RunLuaChatFunc(BYTE byChannel)
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	sprintf(sztxt,"OnUseChatAction");
	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);
	CLuaPlusSafeFunction_3<bool,UserData*,int> func;
	if(!func(pLuaState, lf, &bRet,m_pParent,byChannel))
	{
		g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
		return false;
	}
	return bRet;
}

bool	UserRecord::RunLuaDegreeFunc(int nPoint)
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	sprintf(sztxt,"OnGainDegreeAction");
	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);
	CLuaPlusSafeFunction_3<bool,UserData*,int> func;
	if(!func(pLuaState, lf, &bRet,m_pParent,nPoint))
	{
		g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
		return false;
	}
	return bRet;
}

bool	UserRecord::RunLuaActivityFunc(int nID,int nVal1,int nVal2)
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	if(nID == 1001)
		sprintf(sztxt,"OnAnswerAction");
	else if(nID == 1002)
		sprintf(sztxt,"OnTurntableAction");
	else
		return false;
	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);
	CLuaPlusSafeFunction_4<bool,UserData*,int,int> func;
	if(!func(pLuaState, lf, &bRet,m_pParent, nVal1,nVal2))
	{
		g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
		return false;
	}
	return bRet;
}

bool	UserRecord::RunLuaKillEnemyFunc(int nID,bool ifWBoss)
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	sprintf(sztxt,"OnKillEnemyAction");
	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);
	CLuaPlusSafeFunction_4<bool,UserData*,int,bool> func;
	if(!func(pLuaState, lf, &bRet,m_pParent, nID,ifWBoss))
	{
		g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
		return false;
	}
	return bRet;
}

bool	UserRecord::RunLuaRelationFunc(int nAction,int nTarid)
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	switch(nAction)
	{
	case 1:
		sprintf(sztxt,"OnGainStudentAction");		//收徒成功
		break;
	case 2:
		sprintf(sztxt,"OnOfferTeacherAction");		//供奉师傅
		break;
	case 3:
		sprintf(sztxt,"OnBetrayTeacherAction");		//背叛师傅
		break;
	case 4:
		sprintf(sztxt,"OnCongraduateAction");		//出师
		break;
	case 5:
		sprintf(sztxt,"OnOfferStudentAction");		//传承
		break;
	}
	
	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);
	CLuaPlusSafeFunction_3<bool,UserData*,int> func;
	if(!func(pLuaState, lf, &bRet,m_pParent,nTarid))
	{
		g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
		return false;
	}
	return bRet;
}

bool	UserRecord::RunLuaFamilyFunc(int nAction,int nVal)
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	switch(nAction)
	{
	case 1:
		sprintf(sztxt,"OnJoinFamilyAction");		//加入军团
		break;
	case 2:
		sprintf(sztxt,"OnLevelUpFamilyAction");		//军团升级
		break;
	case 3:
		sprintf(sztxt,"OnOfferFamilyAction");		//贡献军团
		break;
	case 4:
		sprintf(sztxt,"OnLevelUpFamilySkillAction");	//军团技能
		break;
	}

	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);
	CLuaPlusSafeFunction_3<bool,UserData*,int> func;
	if(!func(pLuaState, lf, &bRet,m_pParent,nVal))
	{
		g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
		return false;
	}
	return bRet;
}

bool	UserRecord::RunLuaStarFunc(int nLv)
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	sprintf(sztxt,"OnLevelUpStarAction");
	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);
	CLuaPlusSafeFunction_3<bool,UserData*,int> func;
	if(!func(pLuaState, lf, &bRet,m_pParent, nLv))
	{
		g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
		return false;
	}
	return bRet;
}

bool	UserRecord::RunLuaEquipFunc(int nAction,ItemObj* pItem,ItemObj* pItem2)
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	switch(nAction)
	{
	case 1:
		sprintf(sztxt,"OnGainEquipAction");		//获得装备
		break;
	case 2:
		sprintf(sztxt,"OnLevelUpEquipAction");		//炼星装备
		break;
	case 3:
		sprintf(sztxt,"OnStoneEquipAction");		//镶嵌装备
		break;
	case 4:
		sprintf(sztxt,"OnEatEquipAction");		//精炼装备
		break;
	case 5:
		sprintf(sztxt,"OnPutOnEquipAction");	//穿戴装备
		break;
	}

	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);
	CLuaPlusSafeFunction_3<bool,UserData*,ItemObj*> func1;
	CLuaPlusSafeFunction_4<bool,UserData*,ItemObj*,ItemObj*> func2;
	if(nAction == 3)
	{
		if(!func2(pLuaState, lf, &bRet,m_pParent,pItem,pItem2))
		{
			g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
			return false;
		}
	}
	else
	{
		if(!func1(pLuaState, lf, &bRet,m_pParent,pItem))
		{
			g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
			return false;
		}
	}
	return bRet;
}

bool	UserRecord::RunLuaPetFunc(int nAction,SlaveBaseData* pPet)
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	switch(nAction)
	{
	case 1:
		sprintf(sztxt,"OnGainPetAction");		//获得宠物
		break;
	case 2:
		sprintf(sztxt,"OnPetLearnSkillAction");	//学习技能
		break;
	case 3:
		sprintf(sztxt,"OnStrengthPetAction");	//强化宠物
		break;
	case 4:
		sprintf(sztxt,"OnVariatPetAction");		//变异宠物
		break;
	}

	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);
	CLuaPlusSafeFunction_3<bool,UserData*,SlaveBaseData*> func;
	if(!func(pLuaState, lf, &bRet,m_pParent,pPet))
	{
		g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
		return false;
	}
	return bRet;
}

bool	UserRecord::RunLuaHorseFunc(int nAction,HorseBaseData* pPet,int nNum)
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	switch(nAction)
	{
	case 1:
		sprintf(sztxt,"OnGainHorseAction");		//获得坐骑
		break;
	case 2:
		sprintf(sztxt,"OnHorseLevelUpAction");	//坐骑升级
		break;
	case 3:
		sprintf(sztxt,"OnHorseEatAction");	//坐骑吞噬
		break;
	}

	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);
	CLuaPlusSafeFunction_3<bool,UserData*,HorseBaseData*> func1;
	CLuaPlusSafeFunction_4<bool,UserData*,HorseBaseData*,int> func2;
	if(nAction == 3)
	{
		if(!func2(pLuaState, lf, &bRet,m_pParent,pPet,nNum))
		{
			g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
			return false;
		}
	}
	else
	{
		if(!func1(pLuaState, lf, &bRet,m_pParent,pPet))
		{
			g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
			return false;
		}
	}
	return bRet;
}

bool	UserRecord::RunLuaMapCopyFunc(int nAction,int nID,int nLevel,int nScore)
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	switch(nAction)
	{
	case 1:
		sprintf(sztxt,"OnCompleteMapCopyAction");		//通关副本
		break;
	case 2:
		sprintf(sztxt,"OnOpenCardsAction");				//副本翻牌
		break;
	}

	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);
	CLuaPlusSafeFunction_5<bool,UserData*,int,int,int> func;

	if(!func(pLuaState, lf, &bRet,m_pParent,nID,nLevel,nScore))
	{
		g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
		return false;
	}
	return bRet;
}

bool	UserRecord::RunLuaPVPFunc(int nCount)
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	
	sprintf(sztxt,"OnWinContinousAction");				//竞技场连胜

	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);
	CLuaPlusSafeFunction_3<bool,UserData*,int> func;

	if(!func(pLuaState, lf, &bRet,m_pParent,nCount))
	{
		g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
		return false;
	}
	return bRet;
}

bool	UserRecord::RunLuaWealthFunc(int nType,int nGet,int nTotal)
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	switch(nType)
	{
	case 1:
		sprintf(sztxt,"OnGainGoldAction");					//获得金币
		break;
	case 2:
		sprintf(sztxt,"OnGainHonourAction");				//获得荣誉
		break;
	case 3:
		sprintf(sztxt,"OnLevelUpVipAction");				//升级VIP
		break;
	case 4:
		sprintf(sztxt,"OnLevelUpPlayerAction");				//人物升级
		break;
	case 5:
		sprintf(sztxt,"OnUnlockBagAction");				//解锁包裹
		break;
	}

	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);
	CLuaPlusSafeFunction_4<bool,UserData*,int,int> func;

	if(!func(pLuaState, lf, &bRet,m_pParent,nGet,nTotal))
	{
		g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
		return false;
	}
	return bRet;
}

bool	UserRecord::RunLuaSkillFunc(int nAction,int nID,int nType,int nLv)
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	switch(nAction)
	{
	case 1:
		sprintf(sztxt,"OnLevelUpSkillAction");				//升级技能
		break;
	}

	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);
	CLuaPlusSafeFunction_5<bool,UserData*,int,int,int> func;

	if(!func(pLuaState, lf, &bRet,m_pParent,nID,nType,nLv))
	{
		g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
		return false;
	}
	return bRet;
}

bool	UserRecord::RunLuaFarmFunc(int nAction,int nVal)
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	switch(nAction)
	{
	case 1:
		sprintf(sztxt,"OnGainCropsAction");				//收取农作物
		break;
	case 2:
		sprintf(sztxt,"OnStealCropsAction");				//偷窃农作物
		break;
	case 3:
		sprintf(sztxt,"OnOpenFieldsAction");				//开垦田地
		break;
	}

	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);
	CLuaPlusSafeFunction_3<bool,UserData*,int> func;

	if(!func(pLuaState, lf, &bRet,m_pParent,nVal))
	{
		g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
		return false;
	}
	return bRet;
}

bool	UserRecord::RunLuaMarryFunc(BYTE byAction,int nTarget,int nPoint1,int nPoint2)
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	switch(byAction)
	{
	case 1:
		sprintf(sztxt,"OnGetEngage");				//订婚
		break;
	case 2:
		sprintf(sztxt,"OnGetMarried");				//结婚
		break;
	case 3:
		sprintf(sztxt,"OnBreakRelation");				//离婚
		break;
	case 4:
		sprintf(sztxt,"OnAddMarryPoint");				//增加友好度
		break;
	}

	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);
	CLuaPlusSafeFunction_3<bool,UserData*,int> func1;
	CLuaPlusSafeFunction_5<bool,UserData*,int,int,int> func2;

	if(byAction == 4)
	{
		if(!func2(pLuaState, lf, &bRet,m_pParent,nTarget,nPoint1,nPoint2))
		{
			g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
			return false;
		}
	}
	else
	{
		if(!func1(pLuaState, lf, &bRet,m_pParent,nTarget))
		{
			g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
			return false;
		}
	}
	
	return bRet;
}

bool	UserRecord::RunLuaDeadFunc()
{
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	char sztxt[64];
	
	sprintf(sztxt,"OnPlayerDeadAction");				//角色死亡

	LuaObject loFunction = pLuaState->GetGlobal(sztxt);
	if(!loFunction.IsFunction()||loFunction.IsNil()){
		return true;
	}
	bool bRet = false; 
	LuaFunction<bool> lf(loFunction);
	CLuaPlusSafeFunction_2<bool,UserData*> func;

	if(!func(pLuaState, lf, &bRet,m_pParent))
	{
		g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
		return false;
	}
	return bRet;
}

bool UserRecord::AddItemChangeLog( int nItemID,int nCount,int nFrom,BYTE byType )
{
	return g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_ITEMCHANGE_LOG" (fromuin,itemid,itemcnt,touin,type,ctime) values (%d,%d,%d,%d,%d,now())",nFrom,nItemID,nCount,m_pParent->m_dwUin,byType);
}

bool UserRecord::AddGoldFromLog( int nAddVal, int nVal, BYTE byType )
{
	return g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_GOLD_FROM_LOG" (uin,addval,money,type,ctime) values (%d,%d,%d,%d,now())",
		m_pParent->m_dwUin,nAddVal,nVal,byType);
}

bool UserRecord::AddEquipStarLog( int nItemID )
{
	bool	bFind	= false;
	QueryRlt xRlt;
	MySqlQuery* pCmd = NULL;
	if(g_pLogDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_EQUIP_STAR_LOG" where uin = %d and itemid = %d",
		m_pParent->m_dwUin,nItemID))
	{
		pCmd = xRlt.GetRlt();
		if(pCmd != NULL && pCmd->FetchRow())
		{
			bFind	= true;
		}
	}
	if(!bFind)
	{
		return g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_EQUIP_STAR_LOG" (uin,itemid,starcnt,ctime) values (%d,%d,%d,now())",
			m_pParent->m_dwUin,nItemID,1);
	}else
	{
		return g_pLogDB->ExecuteSQL(FALSE, "update "RPG_EQUIP_STAR_LOG" set starcnt=starcnt+1,ctime = now() where uin = %d and itemid = %d",
			m_pParent->m_dwUin, nItemID);
	}	
}

bool UserRecord::AddEquipStarLogEx( int nItemID,int nCnt,int nStarLv )
{
	return g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_EQUIP_STAREX_LOG" (uin,itemid,changecnt,starlv,ctime) values (%d,%d,%d,%d,now())",
			m_pParent->m_dwUin,nItemID,nCnt,nStarLv);
}

bool UserRecord::AddSlaveLvLog( int nSlaveID,int nBaseID,int nPos,int nPosLv )
{
	return g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_SLAVE_LV_LOG" (uin,slaveid,baseid,npos,poslvl,ctime) values (%d,%d,%d,%d,%d,now())",
		m_pParent->m_dwUin,nSlaveID,nBaseID,nPos,nPosLv);
}

bool UserRecord::AddFortLog( int nChgScore,int nWarScore,int nCurPos,int nManorLv,int nManorExp,BYTE byType )
{
	return g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_FORT_LOG" (uin,chgscore,warscore,curpos,manorlv,manorexp,forttype,ctime) values (%d,%d,%d,%d,%d,%d,%d,now())",
		m_pParent->m_dwUin,nChgScore,nWarScore,nCurPos,nManorLv,nManorExp,byType);
}

WorldLog::WorldLog()
{
	ifSave = false;
	dwSaveTick = 0;
}

void	WorldLog::SaveAll()
{
	ifSave = true;
	dwSaveTick = 30;
}

void	WorldLog::UpdateSec()
{
	if(ifSave)
	{
		if(dwSaveTick > 0)
		{
			dwSaveTick --;
		}
		else
		{
			ifSave = false;
			SaveRankLog();
			SaveUserInfoLog();
			SaveWarRankLog();
		}
	}
}

bool	WorldLog::SaveRankLog()
{
	return g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_RANK_LOG" (id1,id2,type,pos,check1,check2,val1,val2,val3,val4,desc1,desc2,desc3,desc4,score) select id1,id2,type,pos,check1,check2,val1,val2,val3,val4,desc1,desc2,desc3,desc4,score from %s."RPG_RANKDATA_TABLE"",ServerCfg::GetInstance().szDBName);
}

bool	WorldLog::SaveUserInfoLog()
{
	return g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_USERINFO_LOG" (uin,AccountUin,lvl,exp,job,country,sex,gold,nickname,homeid,homeduty,viplvl,honour,starlv,onlineawardsec) select uin,AccountUin,lvl,exp,job,country,sex,gold,nickname,homeid,homeduty,viplvl,honour,starlv,onlineawardsec from %s."RPG_USER_TABLE" where %s."RPG_USER_TABLE".lvl>=30",ServerCfg::GetInstance().szDBName,ServerCfg::GetInstance().szDBName);
}

bool WorldLog::SaveWarRankLog()
{
	return g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_WAR_RANK_LOG" (pos,uin,lv,name,job,sex,score,wincnt,param) select pos,uin,lv,name,job,sex,score,wincnt,param from %s."RPG_RANK_WAR_TABLE"",ServerCfg::GetInstance().szDBName);
}
