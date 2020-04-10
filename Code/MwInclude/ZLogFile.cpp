#include "stdafx.h"
#include "ZLogFile.h"
#include <direct.h>
#include <io.h>

ZLogFile::ZLogFile(DWORD dwInterval /* = 60000 */,bool bCutFileByDate)
{
	m_dwFlushInterval	= dwInterval;
	m_dwLastFlushTime	= 0;
	m_pFile				= NULL;
	m_bCutFileByDate	= bCutFileByDate;
	InitializeCriticalSection(&m_xLock);
}

ZLogFile::~ZLogFile()
{
	if(m_pFile != NULL)
		fclose(m_pFile);
	DeleteCriticalSection(&m_xLock);
}

bool ZLogFile::Open(const char* szFileName)
{
	EnterCriticalSection(&m_xLock);
	if(m_pFile != NULL)
		fclose(m_pFile);
	strcpy(m_szFileName, szFileName);
	GetLocalTime(&m_xDate);
	char szFileNameWhole[MAX_PATH] = {0};
	if(_access("log",0) == -1)
		mkdir("log");
	sprintf(szFileNameWhole,"log\\%s_%d_%d_%d.txt",m_szFileName,m_xDate.wYear,m_xDate.wMonth,m_xDate.wDay);
	m_pFile = fopen(szFileNameWhole,"a");
	if(m_pFile == NULL) 
	{
		LeaveCriticalSection(&m_xLock);
		return false;
	}
	m_dwLastFlushTime = GetTickCount();
	LeaveCriticalSection(&m_xLock);
	return true;
}

void ZLogFile::OutPut(const char* szFormat,...)
{
	EnterCriticalSection(&m_xLock);

	if(m_pFile == NULL) 
	{
		LeaveCriticalSection(&m_xLock);
		return;
	}
	char	szMsg[4096];
	memset(szMsg, 0, sizeof(szMsg));
	vsprintf(szMsg, szFormat, (char*) (&szFormat + 1));

	char* szCheck	= strstr(szMsg,"%");
	if(szCheck)
		szCheck[0]	= 0;

	SYSTEMTIME xSys;
	::GetLocalTime(&xSys);
	if(m_pFile != NULL)
	{
		fprintf(m_pFile,"%d-%d-%d %d:%d:%d ",xSys.wYear,xSys.wMonth,xSys.wDay,xSys.wHour,xSys.wMinute,xSys.wSecond);
		fprintf(m_pFile,szMsg);
		fprintf(m_pFile,"\n");
		if(m_bCutFileByDate && xSys.wDay != m_xDate.wDay)
		{
			fclose(m_pFile);
			char szFileName[MAX_PATH] = {0};
			sprintf(szFileName,"log\\%s_%d_%d_%d.txt",m_szFileName,xSys.wYear,xSys.wMonth,xSys.wDay);
			m_pFile = fopen(szFileName,"a");
			m_dwLastFlushTime = GetTickCount();
		}
		else if(GetTickCount() - m_dwLastFlushTime > m_dwFlushInterval)
		{
			fflush(m_pFile);
			m_dwLastFlushTime = GetTickCount();
		}
	}
	LeaveCriticalSection(&m_xLock);
}

void ZLogFile::Flush()
{
	EnterCriticalSection(&m_xLock);
	if(m_pFile != NULL)
	{
		fflush(m_pFile);
		m_dwLastFlushTime = GetTickCount();
	}
	LeaveCriticalSection(&m_xLock);
}
