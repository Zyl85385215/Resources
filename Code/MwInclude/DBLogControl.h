#pragma once

#include <windows.h>
#include "xnthread.h"
#include "xnlist.h"
#include "xnExtStr.h"
#include "MysqlDB.h"
#include "DBTables.h"
#include "DBIO.h"

#include "ZNetPacket.h"

#define sizeofrecord(a, b) (sizeof(a)/sizeof(b))

class DBLogControl
{
public:
	DBLogControl(char * threadName, int nGameLogID = -1);
	~DBLogControl(void);

	/************************************************************************/
	/* db system															*/
	/************************************************************************/
	bool			CreateLogDB(bool ifImmediately);
	bool			ChangeLogDB(bool ifImmediately);
	int				GetDBVersion();
	void			UpdateDBVersion(int nPrevVersion, int nCurrVersion);
	bool			ConnectToDB(const char * ip, const char * userName, const char *password, const char *dBaseName);
	void			AddColume(char * tableName, char * columeName, char * columeType, char * defaultValue, char * prevName = NULL);
	void			AddColumes(char * tableName, DB_Colume * columes, int count);
	void			Start(void);
	void			Stop(void);
	void			Reborn(char * threadNmae);

	BOOLEAN			ExecuteSQL(BOOLEAN bImmediately, char* szFormat, ...);

	////////////////////////////create table/////////////////////////////////////////////
	bool			_CreateGoldLogTable();
	bool			_CreateRmbLogTable();
	bool			_CreateBindRmbLogTable();
	bool			_CreateExpLogTable();
	bool			_CreateItemTable();
	bool			_CreateSlaveLogTable();
	bool			_CreateMkQICntLogTable();
	bool			_CreateGMCodeLogTable();
	bool			_CreateItemChangeLogTable();
	bool			_CreatePayErrorLogTable();
	bool			_CreateLuaLogTable();
	bool			_CreateLvupLogTable();
	bool			_CreateRankLogTable();
	bool			_CreateUserInfoLogTable();
	bool			_CreateWarRankLogTable();
	bool			_CreateMailLogTable();
	bool			_CreateGoldFromLogTable();
	bool			_CreateEquipStarLogTable();
	bool			_CreateEquipStarExLogTable();
	bool			_CreateSlaveLvLogTable();
	bool			_CreateFortLogTable();

	DBIO *			pDBServer;
	xnThread *          m_pThread;
	char*			m_szDBBaseName;
};

BOOLEAN DBLogThreadProc(void * data, MSG * Msg);
extern DBLogControl* g_pLogDB;