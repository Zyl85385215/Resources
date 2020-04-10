#pragma once

#include <windows.h>
#include <stdio.h>

class ZLogFile
{
private:
	CRITICAL_SECTION	m_xLock;
	DWORD				m_dwFlushInterval;
	DWORD				m_dwLastFlushTime;
	FILE*				m_pFile;
	char				m_szFileName[MAX_PATH];
	SYSTEMTIME			m_xDate;
	bool				m_bCutFileByDate;
public:
	ZLogFile(DWORD dwInterval = 60000,bool bCutFileByDate = true);
	~ZLogFile();
	bool				Open(const char* szFileName);
	void				OutPut(const char* szFormat,...);
	void				Flush();
};