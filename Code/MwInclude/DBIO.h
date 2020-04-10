#ifndef _DBSERVER_H_
#define _DBSERVER_H_
#include "MysqlDB.h"
#include "xnExtstr.h"
#include "xnlist.h"


class SQLCmd
{
public:
	char*			szSqlCmd;
	DWORD			dwTick;

	SQLCmd(const char* szCmd);
	~SQLCmd();
};

struct DB_BLOB
{
	BYTE *		data;
	int			size;
	SQLCmd *	sql;
};

class SQLCmdBLOB
{
public:
	char *			szSqlCmd;
	void **			params;
	int *			paramsizes;
	int				paramcount;
	DWORD			dwTick;

	SQLCmdBLOB(const char* szCmd, void* params[], int paramsizes[] ,int paramcount);
	~SQLCmdBLOB();
};

class DBIO;
class QueryRlt
{
	friend			DBIO;
	MySqlQuery*		m_pRlt;
	DBIO*			m_pDB;
	DWORD			m_dwUseTime;
	DWORD			m_dwRecordCnt;
	//char*			m_szText;
public:
	QueryRlt();
	~QueryRlt();
	MySqlQuery*		GetRlt();
	DWORD			GetRecordCnt();
};
class DBIO 
{
	friend			QueryRlt;

	void			_ReleaseQuery(MySqlQuery* pQuery);
	char			m_szName[MAX_PATH];

private:
	bool			_ConnectDB(const char * ip, const char * userName, const char * password, const char * dBaseName, bool create);
	bool			_StartProtect(void);
	bool			_ExecuteSQL(const char * szSQL);
	bool			_SetBLOB(BYTE * pData, DWORD dwSize, const char * szCmd);


public:
	MySqlDB*		m_pDB;
	int				m_nLastTag;
	DWORD			m_dwID;

	CRITICAL_SECTION m_xLock;
	DBIO(const char * szName);
	virtual ~DBIO();
	bool			ConnectDB(const char * ip, const char * userName, const char *password, const char *dBaseName, bool create = TRUE);
	void			DisconnectDB();
    bool			ExecuteSQL(const char* szFormat,...);
	bool 			QueryWithParams(xnList* pListReturn, char* szSQl, int objsize, int* fieldsizes, int* dbIndexs, int fieldcount);
    bool			Query(QueryRlt* pRlt,const char* szFormat,...);	
    bool			GetBLOB(BYTE * pRltData, int nBufferSize, const char * szCmd);
	bool			GetBLOB(BYTE * pRltData, int nBufferSize, int & nDataSize, const char * szCmd);
    bool			SetBLOB(BYTE * pData, DWORD dwSize, const char * szCmd);
	bool			CheckDBConnect(void);
	bool			IsConnected(void);
};

#endif
