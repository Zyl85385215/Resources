#ifndef _DBASE_PACKET_H_
#define _DBASE_PACKET_H_

#include <windows.h>
#include "groupcmd.h"

#pragma pack (push)
#pragma pack (1)

enum {
	DBASE_CONNECT,
	DBASE_DISCONNECT,
	SQL_EXEC,
	SQL_SET_BLOB,
	SQL_QUERY,
	SQL_GET_BLOB,

	SQL_QUERY_RESULT,
	SQL_BLOB_RESULT,
	SQL_ERROR,
};

struct DBASE_CMD_DBASE_CONNECT_Data {
	char dbaseName[1];
};

struct DBASE_CMD_SQL_EXEC_Data {
	DWORD tag;
	char sqlCmd[1];
};

struct DBASE_CMD_SQL_QUERY_Data {
	DWORD tag;
	char sqlCmd[1];
};

struct DBASE_CMD_SQL_GET_BLOB_Data {
	DWORD tag;
	char sqlQuery[1];
};

struct DBASE_CMD_SQL_SET_BLOB_Data {
	DWORD tag;
	char sqlCmd[1]; // + DWORD(blobsize) + blobData
};

struct DBASE_CMD_SQL_QUERY_RESULT_Data {
	DWORD tag;
	DWORD realSize;
	DWORD compressSize;
	BYTE data[1];
};

struct DBASE_CMD_SQL_BLOB_RESULT_Data {
	DWORD tag;
	DWORD realSize;
	DWORD compressSize;
	DWORD fieldNameSize;
	char data[1];
};

struct DBASE_CMD_SQL_ERROR_Data {
	DWORD tag;
	char errMsg[1];
};

#pragma pack (pop)


#endif