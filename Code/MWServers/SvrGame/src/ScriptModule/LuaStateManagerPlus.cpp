#include "stdafx.h"
#include "LuaStateManagerPlus.h"
#include "ItemCenter.h"
#include "UserData.h"
#include "FsGameCenter.h"
#include "MapObjCenter.h"
#include "CallMapEnemy.h"
#include "GMActiveCfg.h"
#include "WorldFlag.h"
#include "SlaveData.h"
#include "MailCenter.h"
#include <map>
#include <set>
#include "RankCenter.h"
using namespace std;

DEFINE_SCRIPT_CLASS(CTM);
DEFINE_SCRIPT_CLASS(ItemCenter);
DEFINE_SCRIPT_CLASS(ItemObj);
DEFINE_SCRIPT_CLASS(UserData);
DEFINE_SCRIPT_CLASS(MapObjCenter);
DEFINE_SCRIPT_CLASS(MapServerObj);
DEFINE_SCRIPT_CLASS(SlaveBaseData);
DEFINE_SCRIPT_CLASS(HorseBaseData);
DEFINE_SCRIPT_CLASS(SlaveCenter);
DEFINE_SCRIPT_CLASS(HorseCenter);
DEFINE_SCRIPT_CLASS(UserRecord);

// 	全局变量控制
// static void SetGlobalVarInt(const char * szVarName, int nVar);
// static void AddGlobalVarInt(const char * szVarName, int nVar);
// static int GetGlobalVarInt(const char * szVarName);
// static void SendGlobalVarToAllClient(const char * szVarName);
// static void PlusGlobalVarXiayimengShiqi(int nVal);
// static void SetGlobalVarString(const char * szVarName, const char * szVar);
// static const char * GetGlobalVarString(const char * szVarName);
// static void SetActivePos(int nMapIndex, int x, int y, DWORD dwSecond,const char* szTitil,int nLv );
// static void SetActiveById(DWORD id);
// static void NotifyHomeMemberJoinWar(WORD wHomeID,int nMapIndex,int x,int y,DWORD dwSecond,int nLimitLv,const char* szPopMsg);// 最多七个参数，否则编译不过
// 
// 	公告功能
// static void SendWorldMessage(const char * szMessage, int nCnt, bool bGM, BYTE bCountry);
// static void SendAllLineMessage(const char * szMessage, int nCnt, bool bGM, BYTE bCountry);
// static void PrintMessage(const char * szMessage);
// 
// 	工具
// static int	SplitWStr(const char * szSrc, const char * szTag);		//字符串分割
// static bool BitOperator(DWORD dwInput, BYTE byBitCnt);
// static bool SendMail(DWORD uin, WORD wItemID, int iCnt, const char* szMailBody);
// static bool SendMail2Item(DWORD uin, WORD wItemID1, int iCnt1, WORD wItemID2, int iCnt2, const char* szMailBody);
// 
// 
// static bool SetCountryHonour(BYTE bCountry, int nHonour);
// static bool SetHomeICON(DWORD dwHomeID, int nHomeICON);
// static bool AddLUAEventLog(const char * szEvent);
// static EnemyObj * ServerObjectToEnemy(ServerObject * pServerObject);
// static PlayerData * ServerObjectToPlayer(ServerObject * pServerObject);
// static SlaveObj * ServerObjectToSlave(ServerObject * pServerObject);

//	时间机制
static CTM * CreateTM(void);
static void ReleaseTM(CTM * pTM);
static void SendGameMessage(const char* msg,int nType,int nid,int nCnt);
static void FreeActivityMap(int nID,int nMIndex);

static void AddActive(WORD wActiveID,bool bRemove);
static void KillMapEnemyByID(DWORD dwMIndex,WORD wEnemyID);

static DWORD	CallEnemy(int nBase, int nMapIndex, int nPosX, int nPosY,int nForce,int nPow);
static DWORD	RemoveEnemy( int nID, int nMapIndex );
static void		SetEnemy(DWORD dwID,int nMapIndex, int nType, DWORD	dwTarget,int nTagType);	//nType,  0取消, 1 被动, 2主动, 3跟随, 9攻击   nTagType,1玩家, 3怪物 

static MapObjCenter* GetMapObjCenter();

static int	GetWorldFlagInt(int nType,int nDefault);
static const char*	GetWorldFlagString(int nType,const char* szDefault);
static void	SetWorldFlagInt(int nType,int nValue);
static void	SetWorldFlagString(int nType,const char* szValue);
static void	GetUserByRecharge(int nVal1,int nVal2,LuaStackObject luaTable);	//获取充值范围在nVal1和nVal2间的accountUin,放在luaTable这个table,nVal2为0的话无上限
static void	GetUserByLeague(int nLeague,LuaStackObject luaTable);		//获得军团里的Uin
static void	AddRmbToUin(int nUin,int nVal);								//根据accountUin给钱
static void	ClearTempRmb();												////重置累积的充值
static void	SendMailToUin(int	dwRecv,const char* szT,const char* szB, int nMoney ,int nItemId ,int nItemCnt ,bool bBind );	//发邮件
static int	GetRankIDbyPos(int nType,int nPos);					//获取排行榜的UIN或军团ID, 0等级1财富2战略3军团   , nPos>=0

static void AddSysMail(const char* szT, const char* szB,DWORD dwTime,int nMoney, int nItemId,int nItemCnt,BYTE byBind);
static void		ForbidPlayer(int nUin,DWORD dwTime);
static void		EverForbidPlayer(int nUin,bool byType);
static void		UnablePlayer(int nUin,bool bForbid);
static void		KickPlayer(int nUin,DWORD dwTime);

static	UserData*	GetUserData(DWORD dwUin);
static	void	SetLockCountry(int nCountry){
	g_pGameCenter->m_nLockCountry = nCountry;
}
// 	服务器配置
// static int GetServerIndex(void);
// 
// 	世界国家系统
// static void SetCountryHonourIconIndex(BYTE bCountryID, int iHonour);
// static void SetHomeID(BYTE bCountryID, int nHomeID);
// static void SetKingUin(BYTE bCountryID, DWORD dwUin);
// static void SetProtector1Uin(BYTE bCountryID, DWORD dwUin);
// static void SetProtector2Uin(BYTE bCountryID, DWORD dwUin);
// static void SetGenUin(BYTE bCountryID, DWORD dwUin);
// static void SetCountryPower(BYTE bCountryID, int nPower);
// static void ChangeCountryPower(BYTE bCountryID, int nPower);
// static void SetAppointTimes(BYTE bCountryID, int nAppointTimes);
// static DWORD GetKingUin(BYTE bCountryID);
// static DWORD GetProtector1Uin(BYTE bCountryID);
// static DWORD GetProtector2Uin(BYTE bCountryID);
// static DWORD GetGenUin(BYTE bCountryID);
// static int GetCountryPower(BYTE bCountryID);
// static int GetAppointTimes(BYTE bCountryID);
// static int GetHomeID(BYTE bCountryID);
static int test1111(int zzz){
	return 123;
};

class CLuaStatePlus : public CLuaState
{
public:
	void Init(void)
	{
		//	常用库加载
		CLuaModule(m_pLuaState, "CommonModule")
			.def("CreateTM",		CreateTM)
			.def("test1111",		test1111)
			.def("CallEnemy",		CallEnemy)
			.def("RemoveEnemy",		RemoveEnemy)
			.def("SetEnemy",		SetEnemy)
			.def("GetMapObjCenter",		GetMapObjCenter)
			.def("ReleaseTM",		ReleaseTM)
			.def("FreeActivityMap",		FreeActivityMap)
 			.def("GetWorldFlagInt",		GetWorldFlagInt)
 			.def("GetWorldFlagString",		GetWorldFlagString)
 			.def("SetWorldFlagInt",		SetWorldFlagInt)
			.def("AddRmbToUin",		AddRmbToUin)
			.def("ClearTempRmb",		ClearTempRmb)
			.def("SendMailToUin",		SendMailToUin)
			.def("SetWorldFlagString",		SetWorldFlagString)
			.def("GetUserData",		GetUserData)
			.def("ForbidPlayer",		ForbidPlayer)
			.def("EverForbidPlayer",		EverForbidPlayer)
			.def("UnablePlayer",		UnablePlayer)
			.def("SetLockCountry",		SetLockCountry)
			.def("KickPlayer",		KickPlayer)
 			.def("AddSysMail",		AddSysMail)
			.def("GetUserByRecharge",		GetUserByRecharge)
			.def("GetUserByLeague",		GetUserByLeague)
			.def("GetRankIDbyPos",		GetRankIDbyPos)
			.def("SendGameMessage",		SendGameMessage)
			.def("AddActive",		    AddActive)
			.def("KillMapEnemy",		KillMapEnemyByID);


		CTM::CreateScriptClass(this);
		ItemCenter::CreateScriptClass(this);
		ItemObj::CreateScriptClass(this);
		UserData::CreateScriptClass(this);
		MapObjCenter::CreateScriptClass(this);
		MapServerObj::CreateScriptClass(this);
		SlaveCenter::CreateScriptClass(this);
		SlaveBaseData::CreateScriptClass(this);
		HorseCenter::CreateScriptClass(this);
		HorseBaseData::CreateScriptClass(this);
		UserRecord::CreateScriptClass(this);
// 		m_pLuaState->DoFile("data\\lua_script\\itemscript.lua");
// 		m_pLuaState->DoFile("data\\lua_script\\questscript.lua");
// 		m_pLuaState->DoFile("data\\lua_script\\mapscript.lua");
		LUA_DOFILE(m_pLuaState,"data\\lua_script\\itemscript.lua");
		LUA_DOFILE(m_pLuaState,"data\\lua_script\\questscript.lua");
		LUA_DOFILE(m_pLuaState,"data\\lua_script\\mapscript.lua");
		LUA_DOFILE(m_pLuaState,"data\\lua_script\\enemyscript.lua");
		LUA_DOFILE(m_pLuaState,"data\\lua_script\\extend.lua");

		LUA_DOFILE(m_pLuaState,"data\\lua_script\\actionscript\\activityscript.lua");
		LUA_DOFILE(m_pLuaState,"data\\lua_script\\actionscript\\chatscript.lua");
		LUA_DOFILE(m_pLuaState,"data\\lua_script\\actionscript\\enemyscript.lua");
		LUA_DOFILE(m_pLuaState,"data\\lua_script\\actionscript\\equipscript.lua");
		LUA_DOFILE(m_pLuaState,"data\\lua_script\\actionscript\\familyscript.lua");
		LUA_DOFILE(m_pLuaState,"data\\lua_script\\actionscript\\farmerscript.lua");
		LUA_DOFILE(m_pLuaState,"data\\lua_script\\actionscript\\mapcopyscript.lua");
		LUA_DOFILE(m_pLuaState,"data\\lua_script\\actionscript\\marryscript.lua");
		LUA_DOFILE(m_pLuaState,"data\\lua_script\\actionscript\\playerscript.lua");
		LUA_DOFILE(m_pLuaState,"data\\lua_script\\actionscript\\questscript.lua");
		LUA_DOFILE(m_pLuaState,"data\\lua_script\\actionscript\\relationscript.lua");
		LUA_DOFILE(m_pLuaState,"data\\lua_script\\actionscript\\skillscript.lua");
		LUA_DOFILE(m_pLuaState,"data\\lua_script\\actionscript\\slavescript.lua");
	}

	void DeInit(void)
	{
		ItemCenter::ReleaseScriptClass(this);
		CTM::ReleaseScriptClass(this);
		ItemObj::ReleaseScriptClass(this);
		UserData::ReleaseScriptClass(this);
		MapObjCenter::ReleaseScriptClass(this);
		MapServerObj::ReleaseScriptClass(this);
		SlaveCenter::ReleaseScriptClass(this);
		SlaveBaseData::ReleaseScriptClass(this);
		HorseCenter::ReleaseScriptClass(this);
		HorseBaseData::ReleaseScriptClass(this);
		UserRecord::ReleaseScriptClass(this);
		// 		EnemyObj::ReleaseScriptClass(this);
		// 		ServerObject::ReleaseScriptClass(this);
		// 		GameWorld::ReleaseScriptClass(this);
		// 		WorldCountryList::ReleaseScriptClass(this);
	}
};

CLuaState * CLuaStateManagerPlus::CreateLuaState(void)
{
	CLuaState * pLuaState = new CLuaStatePlus();
	return pLuaState;
}


CTM * CreateTM(void)
{
	time_t t = time(NULL);
	CTM * pTM = new CTM(*localtime(&t));
	return pTM;
}

void ReleaseTM(CTM * pTM)
{
	SAFE_DELETE(pTM);
}

void AddActive(WORD wActiveID,bool bRemove)
{
	g_pGameCenter->m_pGMActiveCenter->AddActive(wActiveID,bRemove);
}

void KillMapEnemyByID(DWORD dwMIndex,WORD wEnemyID)
{
	g_pGameCenter->m_pCallEnemyCenter->KillMapEnemyByID(dwMIndex,wEnemyID);
}

void SendGameMessage(const char* msg,int nType,int nid,int nCnt)
{
	ChatCenter::GetInstance().SendSystemMsg((char*)msg,nType,nid,nCnt);
}

void FreeActivityMap(int nID,int nMIndex)
{
	g_pGameCenter->m_pActivityCenter->FreeActivityMap(nID,nMIndex);
}

DWORD CallEnemy( int nBase, int nMapIndex, int nPosX, int nPosY,int nForce,int nPow )
{
	int	nRtlID	= g_pGameCenter->m_pCallEnemyCenter->CallEnemyByCfg(nBase,nMapIndex,nPosX,nPosY,nForce,nPow);
	OutMessage(msg_type_normal,"CALLENEMY:mapinde=%d,ID=%d,baseid=%d,pos=%d:%d,force=%d,pow=%d",nMapIndex,nRtlID,nBase,nPosX,nPosY,nForce,nPow);
	return nRtlID;
}

DWORD RemoveEnemy( int nID, int nMapIndex )
{
	OutMessage(msg_type_normal,"RemoveENEMY:mapindex=%d,ID=%d",nMapIndex,nID);
	return g_pGameCenter->m_pCallEnemyCenter->RemoveEnemy(nID,nMapIndex);
}


void SetEnemy( DWORD dwID,int nMapIndex, int nType, DWORD dwTarget,int nTagType )
{
	g_pGameCenter->m_pCallEnemyCenter->SetEnemyAct(dwID,nMapIndex,nType,dwTarget,nTagType);
}

static MapObjCenter* GetMapObjCenter(){
	return g_pGameCenter->m_pMapObjCenter;
}

static int	GetWorldFlagInt(int nType,int nDefault){
	return g_pGameCenter->m_pWorldFlag->GetFlagInt(nType,nDefault);
}
static const char*	GetWorldFlagString(int nType,const char* szDefault){
	return g_pGameCenter->m_pWorldFlag->GetFlagString(nType,szDefault);

}
static void	SetWorldFlagInt(int nType,int nValue){
	g_pGameCenter->m_pWorldFlag->SetFlagInt(nType,nValue);
}
static void	SetWorldFlagString(int nType,const char* szValue){
	g_pGameCenter->m_pWorldFlag->SetFlagString(nType,szValue);
}
static void	AddRmbToUin(int nUin,int nVal){
	g_pGameCenter->AddUserRmbByScript(nUin,nVal);
}
static void	SendMailToUin(int	dwRecv,const char* szT, const char* szB, int nMoney ,int nItemId ,int nItemCnt ,bool bBind ){
	g_pGameCenter->m_pMailCenter->RequestSendItemMail(0,dwRecv,szT,szB,nMoney,nItemId,nItemCnt,bBind);
}
static void	ClearTempRmb(){
	g_pGameCenter->ClearTempPay();
}
static void	GetUserByLeague(int nLeague,LuaStackObject luaTable){
	LuaState * pLuaState = g_pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	if(!luaTable.IsTable()){
		return;
	}
	set<int> setData;
	g_pGameCenter->GetUserByLeague(nLeague,setData);
	for (set<int>::iterator itor = setData.begin();itor!=setData.end();itor++)
	{
		LPCD::Push(pLuaState->GetCState(),*itor);
		LPCD::Push(pLuaState->GetCState(),*itor);
		luaTable.SetTable();
	}
}

static void	GetUserByRecharge(int nVal1,int nVal2,LuaStackObject luaTable){
	LuaState * pLuaState = g_pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	if(!luaTable.IsTable()){
		return;
	}
	map<int,int> mapData;
	g_pGameCenter->GetUserPayData(nVal1,nVal2,mapData);

	for (map<int,int>::iterator itor = mapData.begin();itor!=mapData.end();itor++)
	{
		LPCD::Push(pLuaState->GetCState(),itor->first);
		LPCD::Push(pLuaState->GetCState(),itor->second);
		luaTable.SetTable();
	}
}

static int	GetRankIDbyPos(int nType,int nPos)
{
	return g_pGameCenter->m_pRankCenter->GetRankIDByPos(nType,nPos);
}
static void AddSysMail(const char* szT,const char* szB,DWORD dwTime,int nMoney, int nItemId,int nItemCnt,BYTE byBind){
	g_pGameCenter->m_pSysMailCenter->AddSysMail(szT,szB,dwTime,nMoney,nItemId,nItemCnt,byBind);
}
static void		ForbidPlayer(int nUin,DWORD dwTime){
	g_pGameCenter->ForbidPlayer(nUin,dwTime);
}

static void		EverForbidPlayer(int nUin,bool byType){
	g_pGameCenter->EverForbidPlayer(nUin,byType);
}

static void		KickPlayer(int nUin,DWORD dwTime){
	g_pGameCenter->KickPlayer(nUin,dwTime);
}
static void		UnablePlayer(int nUin,bool bForbid){
	g_pGameCenter->UnablePlayer(nUin,bForbid);
}
static	UserData*	GetUserData(DWORD dwUin){
	return g_pGameCenter->GetPlayer(dwUin);
}

// 
// int GetServerIndex(void)
// {
// 	return g_pGameSetting->m_wServerIndex;
// }
// 
// void PrintMessage(const char * szMessage)
// {
// 	OutMessage(msg_type_normal, (char*)szMessage);
// }
// 
// int	SplitWStr(const char * szSrc, const char * szTag)
// {
// 	return SplitWString(szSrc, szTag);
// }
// 
// bool BitOperator(DWORD dwInput, BYTE byBitCnt)
// {
// 	return BitOperate(dwInput, byBitCnt);
// }
// 
// bool SendMail2Item( DWORD uin, WORD wItemID1, int iCnt1, WORD wItemID2, int iCnt2, const char* szMailBody )
// {
// 	PlayerItem* pItem1 = PlayerItem::CreateItem(wItemID1, iCnt1);
// 	if (pItem1 == NULL)
// 		return false;
// 	PlayerItem* pItem2 = PlayerItem::CreateItem(wItemID2, iCnt2);
// 	if (pItem2 == NULL)
// 		return false;
// 
// 	CMailServer * pSystem = CMailItemsStorage::GetSystemMailInstance();
// 	CPlayerMail * pMail = pSystem->CreateSystemMail(uin, "邮件获得");
// 	if(pSystem->AddSystemMailItem(pMail, pItem1))
// 	{
// 		//	添加邮件日志
// #ifdef _SAVE_ITEM_LOG
// 		char szID[64] = {0};
// 		sprintf(szID, "%d-%d-%d", GetTickCount(), pMail->m_miData.dwSenderUin, pMail->m_miData.dwRecverUin);
// 		g_pLogDB->SaveItemlog(uin, pItem1, item_action_give_from_mail, szID);
// #endif
// 	}
// 	if(pSystem->AddSystemMailItem(pMail, pItem2))
// 	{
// 		//	添加邮件日志
// #ifdef _SAVE_ITEM_LOG
// 		char szID[64] = {0};
// 		sprintf(szID, "%d-%d-%d", GetTickCount(), pMail->m_miData.dwSenderUin, pMail->m_miData.dwRecverUin);
// 		g_pLogDB->SaveItemlog(uin, pItem2, item_action_give_from_mail, szID);
// #endif
// 	}
// 	char szMallMailBody[1024] = {0};
// 	sprintf(szMallMailBody, szMailBody);
// 	pMail->SetBody(szMallMailBody);
// 	pSystem->PostSystemMail(pMail);
// 	return true;
// }
// 
// bool SendMail(DWORD uin, WORD wItemID, int iCnt, const char* szMailBody)
// {
// 	PlayerItem* pItem = PlayerItem::CreateItem(wItemID, iCnt);
// 	if (pItem == NULL)
// 		return false;
// 
// 	CMailServer * pSystem = CMailItemsStorage::GetSystemMailInstance();
// 	CPlayerMail * pMail = pSystem->CreateSystemMail(uin, "邮件获得");
// 	if(pSystem->AddSystemMailItem(pMail, pItem))
// 	{
// 		//	添加邮件日志
// #ifdef _SAVE_ITEM_LOG
// 		char szID[64] = {0};
// 		sprintf(szID, "%d-%d-%d", GetTickCount(), pMail->m_miData.dwSenderUin, pMail->m_miData.dwRecverUin);
// 		g_pLogDB->SaveItemlog(uin, pItem, item_action_give_from_mail, szID);
// #endif
// 	}
// 	char szMallMailBody[1024] = {0};
// 	sprintf(szMallMailBody, szMailBody);
// 	pMail->SetBody(szMallMailBody);
// 	pSystem->PostSystemMail(pMail);
// 	return true;
// }
// 
// bool SetCountryHonour(BYTE bCountry, int nHonour)
// {
// 	char buffer[1024] = {0};
// 	Packet* pPacket = NULL;
// 	BuildPacket(pPacket,COUNTRY_GOURP,COUNTRY_HONOUR,buffer);
// 	SET_DATA(pData,COUNTRY_GOURP,COUNTRY_HONOUR,pPacket);
// 	pData->bCountry = bCountry;
// 	pData->nHonour = nHonour;
// 	g_pNetCenter->SendPacketToCenterServerEx(pPacket);
// 	return true;
// }
// 
// bool SetHomeICON(DWORD dwHomeID, int nHomeICON)
// {
// 	char buffer[1024] = {0};
// 	Packet* pPacket = NULL;
// 	BuildPacket(pPacket,HOME,CHANGE_HOME_ICON,buffer);
// 	SET_DATA(pData,HOME,CHANGE_HOME_ICON,pPacket);
// 	pData->dwHomeID = dwHomeID;
// 	pData->nHomeICON = nHomeICON;
// 	g_pNetCenter->SendPacketToCenterServerEx(pPacket);
// 	return true;
// }
// 
// bool AddLUAEventLog(const char * szEvent)
// {
// 	return g_pLogDB->SaveLUAEventLog((char*)szEvent);
// }
// 
// EnemyObj * ServerObjectToEnemy(ServerObject * pServerObject)
// {
// 	if(pServerObject->GetObjType() != OBJ_TYPE_ENEMY) return NULL;
// 	return (EnemyObj*)pServerObject;
// }
// 
// PlayerData * ServerObjectToPlayer(ServerObject * pServerObject)
// {
// 	if(pServerObject->GetObjType() != OBJ_TYPE_PLAYER) return NULL;
// 	return (PlayerData*)pServerObject;
// }
// 
// SlaveObj * ServerObjectToSlave(ServerObject * pServerObject)
// {
// 	if(pServerObject->GetObjType() != OBJ_TYPE_SLAVE) return NULL;
// 	return (SlaveObj*)pServerObject;
// }
// 
// //	worldcountrylist
// void SetCountryHonourIconIndex(BYTE bCountryID, int iHonour)
// {
// 	pWorld->m_pWorldCountryList->SetCountryHonourIconIndex(bCountryID, iHonour);
// }
// 
// void SetHomeID(BYTE bCountryID, int nHomeID)
// {
// 	pWorld->m_pWorldCountryList->SetHomeID(bCountryID, nHomeID);
// }
// 
// void SetKingUin(BYTE bCountryID, DWORD dwUin)
// {
// 	pWorld->m_pWorldCountryList->SetKingUin(bCountryID, dwUin);
// }
// 
// void SetProtector1Uin(BYTE bCountryID, DWORD dwUin)
// {
// 	pWorld->m_pWorldCountryList->SetProtector1Uin(bCountryID, dwUin);
// }
// 
// void SetProtector2Uin(BYTE bCountryID, DWORD dwUin)
// {
// 	pWorld->m_pWorldCountryList->SetProtector2Uin(bCountryID, dwUin);
// }
// 
// void SetGenUin(BYTE bCountryID, DWORD dwUin)
// {
// 	pWorld->m_pWorldCountryList->SetGenUin(bCountryID, dwUin);
// }
// 
// void SetCountryPower(BYTE bCountryID, int nPower)
// {
// 	pWorld->m_pWorldCountryList->SetCountryPower(bCountryID, nPower);
// }
// 
// void ChangeCountryPower(BYTE bCountryID, int nPower)
// {
// 	pWorld->m_pWorldCountryList->ChangeCountryPower(bCountryID, nPower);
// }
// 
// void SetAppointTimes(BYTE bCountryID, int nAppointTimes)
// {
// 	pWorld->m_pWorldCountryList->SetAppointTimes(bCountryID, nAppointTimes);
// }
// 
// DWORD GetKingUin(BYTE bCountryID)
// {
// 	return pWorld->m_pWorldCountryList->GetKingUin(bCountryID);
// }
// 
// DWORD GetProtector1Uin(BYTE bCountryID)
// {
// 	return pWorld->m_pWorldCountryList->GetProtector1Uin(bCountryID);
// }
// 
// DWORD GetProtector2Uin(BYTE bCountryID)
// {
// 	return pWorld->m_pWorldCountryList->GetProtector2Uin(bCountryID);
// }
// 
// DWORD GetGenUin(BYTE bCountryID)
// {
// 	return pWorld->m_pWorldCountryList->GetGenUin(bCountryID);
// }
// 
// int GetCountryPower(BYTE bCountryID)
// {
// 	return pWorld->m_pWorldCountryList->GetCountryPower(bCountryID);
// }
// 
// int GetAppointTimes(BYTE bCountryID)
// {
// 	return pWorld->m_pWorldCountryList->GetAppointTimes(bCountryID);
// }
// 
// int GetHomeID(BYTE bCountryID)
// {
// 	return pWorld->m_pWorldCountryList->GetHomeID(bCountryID);
// }
// 
// void SetActiveById(DWORD id)
// {
// 	ActivityInfo* pInfo = NewActivityConfig::GetInstance().GetActivityInfo(id);
// 	if(NULL != pInfo)
// 	{
// 		DWORD	dwTimeCur	= CMTime::GetCurTickCount();
// 		if(dwTimeCur < pWorld->m_dwUntilTime)
// 			return;
// 		pWorld->m_dwActivityId = pInfo->id;
// 		pWorld->m_nMapIndex	= pInfo->nMapIndex;
// 		pWorld->m_nMapX		= pInfo->nPosx*64;
// 		pWorld->m_nMapY		= pInfo->nPosy*64;
// 		pWorld->m_nLvLimit	= pInfo->nLvLimit;
// 		if(pWorld->m_szActTitle)
// 		{
// 			free(pWorld->m_szActTitle);
// 			pWorld->m_szActTitle = NULL;
// 		}
// 		pWorld->m_szActTitle	= strdup(pInfo->szDesc);
// 		pWorld->m_dwUntilTime	= dwTimeCur+pInfo->nSignTime;
// 	}
// }
// 
// void SetActivePos( int nMapIndex, int x, int y, DWORD dwSecond,const char* szTitil,int nLv )
// {
// 	DWORD	dwTimeCur	= CMTime::GetCurTickCount();
// 	if(dwTimeCur < pWorld->m_dwUntilTime)
// 		return;
// 	pWorld->m_nMapIndex	= nMapIndex;
// 	pWorld->m_nMapX		= x;
// 	pWorld->m_nMapY		= y;
// 	pWorld->m_nLvLimit	= nLv;
// 	if(pWorld->m_szActTitle)
// 	{
// 		free(pWorld->m_szActTitle);
// 		pWorld->m_szActTitle = NULL;
// 	}
// 	pWorld->m_szActTitle	= strdup(szTitil);
// 	pWorld->m_dwUntilTime	= dwTimeCur+dwSecond;
// }
// 
// 
// void NotifyHomeMemberJoinWar(WORD wHomeID,int nMapIndex,int x,int y,DWORD dwSecond,int nLimitLv,const char* szPopMsg)
// {
// 
// 	//szPopMsg  得到是用“/”分割的两个字符窜，分别为提示框的标题和内容
// 
// 	JSplitStr theJSplitStr;
// 	theJSplitStr.SetStr(szPopMsg,"/");
// 	char *com = theJSplitStr.SeekFirst();
// 
// 	if(com == "" || com == NULL) return;
// 
// 	std::vector<char*> comstr;
// 	comstr.push_back(com);
// 	char* a = theJSplitStr.SeekNext();
// 	while(a != NULL)
// 	{
// 		comstr.push_back(a);
// 		a = theJSplitStr.SeekNext();
// 	}
// 
// 	if (2 != comstr.size())
// 		return;//目前只能传标题和内容两个字符串
// 
// 	HOME_CHALLENGE_TRANS hct;
// 	
// 	hct.wHomeID = wHomeID;
// 	hct.nMapIndex	= nMapIndex;
// 	hct.nMapX		= x;
// 	hct.nMapY		= y;
// 	hct.dwUntilTime	= CMTime::GetCurTickCount()+dwSecond;
// 	hct.nLvLimit	= nLimitLv;
// 	hct.szChallengeTitle	= comstr[0];
// 	hct.szContent	= comstr[1];	
// 	
// 	if (false == hct.IsValueValidate())
// 		return;
// 
// 	HomeChallengeManager::GetInstance()->AddChallengInfo(hct);
// 
// 	pWorld->sendHomeJoinWar2CS(wHomeID,nLimitLv);//通知中心服务器（由中心服务器来处理），因为帮会的所有成员信息在中心服务器
// }
// //////////////////////////////////////////////////////////////////////////