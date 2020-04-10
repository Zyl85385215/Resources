#include "stdafx.h"
#include "DBLogControl.h"
#include "DBIO.h"

#include "FsGameCenter.h"
#include "debug_def.h"
#include "TimeControl.h"
#include "DBTables.h"

#define THREAD_CREATE_LOG_DB  WM_USER + 3000
#define THREAD_CHANGE_LOG_DB  WM_USER + 3001
#define THREAD_EXCUTE_LOG_DB  WM_USER + 3002

DBLogControl* g_pLogDB = NULL;

void	DBLogThreadFun(void * data, MSG * Msg)
{
	DBLogControl * pLogDB = (DBLogControl *)data;
	switch (Msg->message)
	{
	case THREAD_CREATE_LOG_DB: 
		{
			pLogDB->CreateLogDB(true);
		}
		break;
	case THREAD_CHANGE_LOG_DB: 
		{
			pLogDB->ChangeLogDB(true);
		}
		break;
	case THREAD_EXCUTE_LOG_DB:
		{
			SQLCmd* pCmd = (SQLCmd*)Msg->lParam;
			if(pCmd != NULL)
			{
				pLogDB->ExecuteSQL(true,pCmd->szSqlCmd);

				if(pCmd)
					delete pCmd;
			}
		}
		break;
	}
}

BOOLEAN DBLogThreadProc(void * data, MSG * Msg)
{
	__try
	{
		DBLogThreadFun(data,Msg);
	}
	__except(DUMP_CALLSTACK())
	{
		DUMP_STRING("DBLogThreadProc Error!\n");
	}
	return TRUE;
}

DBLogControl::DBLogControl( char * threadName, int nGameLogID /*= -1*/ )
{
	m_pThread = new xnThread(DBLogThreadProc, this, threadName);
	m_pThread->SetPriority(THREAD_PRIORITY_ABOVE_NORMAL);
	pDBServer	= new DBIO("maindblog");
	m_szDBBaseName = NULL;
}

DBLogControl::~DBLogControl( void )
{
	Stop();
	Sleep(1000);
	if(m_pThread)
		delete m_pThread;
	if(pDBServer)
		delete pDBServer;
	if(m_szDBBaseName)
		free(m_szDBBaseName);
}

bool DBLogControl::ConnectToDB(const char * ip, const char * userName, const char *password, const char *dBaseName)
{
	char szName[128];
	tm tmNow = CMTime::GetCurTime();
	sprintf(szName,"%s_log_%04d%02d",dBaseName,tmNow.tm_year+1900,tmNow.tm_mon+1);
	m_szDBBaseName = strdup(dBaseName);
	return pDBServer->ConnectDB(ip, userName, password, szName, true);
}

bool DBLogControl::_CreateGoldLogTable()
{
	pDBServer->ExecuteSQL("CREATE TABLE "RPG_GOLD_LOG" (uin int)");
	AddColumes(RPG_GOLD_LOG, RpgGoldLog_Colume, sizeofrecord(RpgGoldLog_Colume, DB_Colume));
	return TRUE;
}

bool DBLogControl::_CreateRmbLogTable()
{
	pDBServer->ExecuteSQL("CREATE TABLE "RPG_RMB_LOG" (uin int)");
	AddColumes(RPG_RMB_LOG, RpgRMBLog_Colume, sizeofrecord(RpgRMBLog_Colume, DB_Colume));
	return TRUE;
}

bool DBLogControl::_CreateBindRmbLogTable()
{
	pDBServer->ExecuteSQL("CREATE TABLE "RPG_BINDRMB_LOG" (uin int)");
	AddColumes(RPG_BINDRMB_LOG, RpgBindRMBLog_Colume, sizeofrecord(RpgBindRMBLog_Colume, DB_Colume));
	return TRUE;
}

bool DBLogControl::_CreateExpLogTable()
{
	pDBServer->ExecuteSQL("CREATE TABLE "RPG_EXP_LOG" (uin int)");
	AddColumes(RPG_EXP_LOG, RpgExpLog_Colume, sizeofrecord(RpgExpLog_Colume, DB_Colume));
	return TRUE;
}

bool DBLogControl::_CreateSlaveLogTable()
{
	pDBServer->ExecuteSQL("CREATE TABLE "RPG_SLAVE_LOG" (uin int)");
	AddColumes(RPG_SLAVE_LOG, RpgSlaveLog_Colume, sizeofrecord(RpgSlaveLog_Colume, DB_Colume));
	return TRUE;
}

bool DBLogControl::_CreateMkQICntLogTable()
{
	pDBServer->ExecuteSQL("CREATE TABLE "MARK_QICNT_LOG" (objid int)");
	AddColumes(MARK_QICNT_LOG, MkQICntLog_Colume, sizeofrecord(MkQICntLog_Colume, DB_Colume));
	return TRUE;
}

bool DBLogControl::_CreateGMCodeLogTable()
{
	pDBServer->ExecuteSQL("CREATE TABLE "RPG_GMCODE_LOG" (uin int)");
	AddColumes(RPG_GMCODE_LOG, RpgGMCodeLog_Colume, sizeofrecord(RpgGMCodeLog_Colume, DB_Colume));
	return TRUE;
}

bool DBLogControl::_CreateLvupLogTable()
{
	pDBServer->ExecuteSQL("CREATE TABLE "RPG_LVUP_LOG" (uin int)");
	AddColumes(RPG_LVUP_LOG, RpgLvupLog_Colume, sizeofrecord(RpgLvupLog_Colume, DB_Colume));
	return TRUE;
}

bool DBLogControl::_CreateRankLogTable()
{
	pDBServer->ExecuteSQL("CREATE TABLE "RPG_RANK_LOG" (id1 int)");
	AddColumes(RPG_RANK_LOG, RpgRankLog_Colume, sizeofrecord(RpgRankLog_Colume, DB_Colume));
	return TRUE;
}

bool DBLogControl::_CreateUserInfoLogTable()
{
	pDBServer->ExecuteSQL("CREATE TABLE "RPG_USERINFO_LOG" (uin int)");
	AddColumes(RPG_USERINFO_LOG, RpgUserInfoLog_Colume, sizeofrecord(RpgUserInfoLog_Colume, DB_Colume));
	return TRUE;
}

bool DBLogControl::_CreateWarRankLogTable()
{
	pDBServer->ExecuteSQL("CREATE TABLE "RPG_WAR_RANK_LOG" (pos int primary key)");
	AddColumes(RPG_WAR_RANK_LOG, RpgWarRankLog_Colume, sizeofrecord(RpgWarRankLog_Colume, DB_Colume));
	return TRUE;
}

bool DBLogControl::CreateLogDB(bool ifImmediately)
{
	if(!ifImmediately && m_pThread)
	{
		m_pThread->SendMessage(THREAD_CREATE_LOG_DB, 0, 0, true);
		return true;
	}
	if(!pDBServer) return false;
	int nVersion =GetDBVersion();
	if(nVersion == DB_LOG_VERSION) return true;

	_CreateGoldLogTable();
	_CreateRmbLogTable();
	_CreateBindRmbLogTable();
	_CreateExpLogTable();
	_CreateItemTable();
	_CreateSlaveLogTable();
	_CreateGMCodeLogTable();
	_CreateMkQICntLogTable();
	_CreateItemChangeLogTable();
	_CreatePayErrorLogTable();
	_CreateLuaLogTable();
	_CreateLvupLogTable();
	_CreateRankLogTable();
	_CreateUserInfoLogTable();
	_CreateWarRankLogTable();
	_CreateMailLogTable();
	_CreateGoldFromLogTable();
	_CreateEquipStarLogTable();
	_CreateEquipStarExLogTable();
	_CreateSlaveLvLogTable();
	_CreateFortLogTable();

	UpdateDBVersion(nVersion, DB_LOG_VERSION);
	return true;
}

void DBLogControl::UpdateDBVersion(int nPrevVersion, int nCurrVersion)
{
	if(nPrevVersion != -1)
	{
		ExecuteSQL(TRUE,"update "DB_DATA_VERSION" set version = %d where dbname = 'common'", nCurrVersion);
	}
	else
	{
		ExecuteSQL(TRUE,"create table "DB_DATA_VERSION" (dbname varchar(64))");
		AddColumes(DB_DATA_VERSION, DBVersionTable_Colume, sizeofrecord(DBVersionTable_Colume, DB_Colume));
		ExecuteSQL(TRUE,"CREATE INDEX IX_NAME ON "DB_DATA_VERSION" (dbname)");
		ExecuteSQL(TRUE,"insert "DB_DATA_VERSION" (dbname, version) values ('common', %d)", nCurrVersion);
	}
}

bool DBLogControl::ChangeLogDB(bool ifImmediately)
{
	if(!ifImmediately && m_pThread) 
	{
		m_pThread->SendMessage(THREAD_CHANGE_LOG_DB, 0,0, true);
		return true;
	}
	if(!pDBServer) return false;
	char szName[128];
	tm tmNow = CMTime::GetCurTime();
	sprintf(szName,"%s_log_%04d%02d",m_szDBBaseName,tmNow.tm_year+1900,tmNow.tm_mon+1);
	if(!ExecuteSQL(TRUE,"create database if not exists %s character set utf8", szName)) return false;
	return pDBServer->m_pDB->UseDB(szName);
}

int DBLogControl::GetDBVersion()
{
	QueryRlt xRlt;
	QueryRlt xVersionRlt;

	if(pDBServer->Query(&xRlt,"SHOW TABLES LIKE '%s'", DB_DATA_VERSION))
	{
		if(pDBServer->Query(&xVersionRlt,"select * from "DB_DATA_VERSION" where dbname = 'common'"))
		{
			MySqlQuery* pCmd = xVersionRlt.GetRlt();
			if(pCmd->FetchRow()) return atoi(pCmd->GetStr("version"));
		}
	}
	return -1;
}

BOOLEAN DBLogControl::ExecuteSQL( BOOLEAN bImmediately, char* szFormat, ... )
{
	if (!this) return false;

	char szSQL[4096] = {0};
	vsprintf(szSQL, szFormat, (char*) (&szFormat + 1));	

	if(!bImmediately && m_pThread->IsRunning())
	{
		SQLCmd* pCmd = new SQLCmd(szSQL);
		m_pThread->SendMessage(THREAD_EXCUTE_LOG_DB, 0, (LPARAM)pCmd, true);
	}
	else
	{
		
		DWORD dwTick = GetTickCount();
		BOOLEAN rlt = pDBServer->ExecuteSQL(szSQL);
		return rlt;
	}
	return true;
}

void DBLogControl::Start(void)
{
	if (this != NULL)
	{
		printf("LogDB Start.\n");
		m_pThread->Start();
	}
}

void DBLogControl::Stop(void)
{
	if (this != NULL)
	{
		Sleep(1000);
		m_pThread->Stop(60*30*1000);
	}
}

void DBLogControl::Reborn( char * threadName )
{
	if (!m_pThread->IsActive())
	{
		m_pThread->Stop(0);
		delete m_pThread;

		m_pThread = new xnThread(DBLogThreadProc, this, threadName);
		m_pThread->SetPriority(THREAD_PRIORITY_ABOVE_NORMAL);
		m_pThread->Start();
	}
}

void DBLogControl::AddColume(char * tableName, char * columeName, char * columeType, char * defaultValue, char * prevName)
{
	if (strnicmp(columeType, "longBLOB", 8) == 0 || strnicmp(columeType, "TINYBLOB", 8) == 0
		||strnicmp(columeType, "BLOB", 4) == 0 || strnicmp(columeType, "MediumBLOB", 10) == 0)
	{
		pDBServer->ExecuteSQL("ALTER TABLE %s ADD %s %s NOT NULL", tableName, columeName, columeType);
		if(prevName)
			pDBServer->ExecuteSQL("ALTER TABLE %s MODIFY COLUMN %s %s NOT NULL AFTER %s", tableName, columeName, columeType,prevName);
	}
	else	
	{
		pDBServer->ExecuteSQL("ALTER TABLE %s ADD %s %s NOT NULL DEFAULT %s", 
			tableName, columeName, columeType, defaultValue ? defaultValue : "0");
		if(prevName)
			pDBServer->ExecuteSQL("ALTER TABLE %s MODIFY COLUMN %s %s NOT NULL  DEFAULT %s AFTER %s", tableName, columeName, columeType, defaultValue ? defaultValue : "0",prevName);
	}
}

void DBLogControl::AddColumes(char * tableName, DB_Colume * columes, int count)
{
	for (int i=0; i<count; i++)
	{
		AddColume(tableName, columes[i].szName, columes[i].szType, columes[i].szDefault);
	}
}

bool DBLogControl::_CreateItemTable()
{
	pDBServer->ExecuteSQL("CREATE TABLE "RPG_ITEM_LOG" (uin int)");
	AddColumes(RPG_ITEM_LOG, RpgItemLog_Colume, sizeofrecord(RpgItemLog_Colume, DB_Colume));
	return TRUE;
}

bool DBLogControl::_CreateItemChangeLogTable()
{
	pDBServer->ExecuteSQL("CREATE TABLE "RPG_ITEMCHANGE_LOG" (fromuin int)");
	AddColumes(RPG_ITEMCHANGE_LOG, RpgItemChangeLog_Colume, sizeofrecord(RpgItemChangeLog_Colume, DB_Colume));
	return TRUE;
}

bool DBLogControl::_CreatePayErrorLogTable()
{
	pDBServer->ExecuteSQL("CREATE TABLE "RPG_PAYERROR_LOG" (tpid int)");
	AddColumes(RPG_PAYERROR_LOG, RpgPayErrorLog_Colume, sizeofrecord(RpgPayErrorLog_Colume, DB_Colume));
	return TRUE;
}

bool DBLogControl::_CreateLuaLogTable()
{
	pDBServer->ExecuteSQL("CREATE TABLE "RPG_LUA_LOG" (id varchar(32))");
	AddColumes(RPG_LUA_LOG, RpgLuaLog_Colume, sizeofrecord(RpgLuaLog_Colume, DB_Colume));
	return TRUE;
}

bool DBLogControl::_CreateMailLogTable()
{
	pDBServer->ExecuteSQL("CREATE TABLE "RPG_MAIL_LOG" (send int)");
	AddColumes(RPG_MAIL_LOG, RpgMailLog_Colume, sizeofrecord(RpgMailLog_Colume, DB_Colume));
	return TRUE;
}

bool DBLogControl::_CreateGoldFromLogTable()
{
	pDBServer->ExecuteSQL("CREATE TABLE "RPG_GOLD_FROM_LOG" (uin int)");
	AddColumes(RPG_GOLD_FROM_LOG, RpgGoldFromLog_Colume, sizeofrecord(RpgGoldFromLog_Colume, DB_Colume));
	return TRUE;
}

bool DBLogControl::_CreateEquipStarLogTable()
{
	pDBServer->ExecuteSQL("CREATE TABLE "RPG_EQUIP_STAR_LOG" (uin int)");
	AddColumes(RPG_EQUIP_STAR_LOG, RpgEquipStarLog_Colume, sizeofrecord(RpgEquipStarLog_Colume, DB_Colume));
	return TRUE;
}

bool DBLogControl::_CreateEquipStarExLogTable()
{
	pDBServer->ExecuteSQL("CREATE TABLE "RPG_EQUIP_STAREX_LOG" (uin int)");
	AddColumes(RPG_EQUIP_STAREX_LOG, RpgEquipStarExLog_Colume, sizeofrecord(RpgEquipStarExLog_Colume, DB_Colume));
	return TRUE;
}

bool DBLogControl::_CreateSlaveLvLogTable()
{
	pDBServer->ExecuteSQL("CREATE TABLE "RPG_SLAVE_LV_LOG" (uin int)");
	AddColumes(RPG_SLAVE_LV_LOG, RpgSlaveLvLog_Colume, sizeofrecord(RpgSlaveLvLog_Colume, DB_Colume));
	return TRUE;
}

bool DBLogControl::_CreateFortLogTable()
{
	pDBServer->ExecuteSQL("CREATE TABLE "RPG_FORT_LOG" (uin int)");
	AddColumes(RPG_FORT_LOG, RpgFortLog_Colume, sizeofrecord(RpgFortLog_Colume, DB_Colume));
	return TRUE;
}
