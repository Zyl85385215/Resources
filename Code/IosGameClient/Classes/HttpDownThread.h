#pragma once

#include "xnlist.h"
#include <pthread.h>
#include "singleton.h"
#include "GlobalDefine.h"

class UrlTaskInfo
{
public:
	UrlTaskInfo();
	~UrlTaskInfo();
	int		nVID;
	int		nFileCnt;
	char*	szUrl;
	char*	szFile;
	int		nSize;

	//-------------------
	pthread_t *pThread;
	bool	bDownOK;
	int		nDownSize;
	int		nUnZipCnt;
};


class HttpVersionManager
{
public:
	HttpVersionManager();
	~HttpVersionManager();

	void		RequestFile(UrlTaskInfo* pUTI);
	void		StartUnZip();
	int			GetProgress();	//获取整体进度
	int			GetEndVersion();
	xnList*		m_pListDU;

	int			m_nTotalFile;
	DWORD		m_dwTotalSize;
	DWORD		m_dwStartTick;
};


class UrlUpdateCfg	: public CSingleton<UrlUpdateCfg>
{
public:
	UrlUpdateCfg();
	~UrlUpdateCfg();

	void		Clear();
	void		LoadMemory(char*	szMemory);

	xnList*		m_pListUrls;
};