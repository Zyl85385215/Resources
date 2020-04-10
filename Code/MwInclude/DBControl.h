#pragma once

#include <windows.h>
#include "xnthread.h"
#include "xnlist.h"
#include "xnExtStr.h"
#include "MysqlDB.h"
#include "DBTables.h"
#include "DBIO.h"

#include "ZNetPacket.h"

enum 
{
	WM_EXCUTE_SQL_PARAMS = WM_USER + 2000,
	WM_EXCUTE_SQL,
	WM_SET_BOLB,
	WM_JOIN_GAME,
	WM_CREATE_USER,
};


class UserData;
class SQLCmdBLOB;
#define	DB_Cnt     5

struct DBControl
{
public:
	DBIO *			pDBServer;

	xnThread *          m_pThread;

	/************************************************************************/
	/* struct/destruct														*/
	/************************************************************************/
	DBControl(char * threadName, int nGameLogID = -1);
	~DBControl(void);

	bool				UserJoinGame( ZPacket* p, BOOLEAN immediately = FALSE);
	BYTE				CreateUser(ZPacket* p, BOOLEAN immediately = FALSE);

	/************************************************************************/
	/* db system															*/
	/************************************************************************/
	bool				ConnectToDB(const char * ip, const char * userName, const char *password, const char *dBaseName);
	void				Start(void);
	void				Stop(void);
	void				Reborn(char * threadNmae);

	BOOLEAN				ExecuteSQL(BOOLEAN bImmediately, char* szFormat, ...);
	bool				SetBLOB(BOOLEAN waitReply, void * data, int size, char* szFormat, ...);
	
private:
	/************************************************************************/
	/* db system															*/
	/************************************************************************/
	BOOLEAN				_GetBLOB(void* data, int bufSize, char* szFormat, ...);
	BOOLEAN				_GetBLOB(void* data, int bufSize, int &dataSize, char* szFormat, ...);

};

BOOLEAN DBaseThreadProc(void * data, MSG * Msg);

extern DBControl * g_pDBase;
