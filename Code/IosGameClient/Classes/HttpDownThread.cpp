#include "HttpDownThread.h"
#include "cocos2d.h"
#include "xnExtstr.h"
#include "IOSThirdPlat.h"

BYTE	g_bUnZipOK	= 0;
int		g_nUnZipCnt	= 0;

static void* ThreadHttpDown(void* data)
{
	UrlTaskInfo* pUTI	= (UrlTaskInfo*)data;

	if(gfun_TPDownZipFile(pUTI->szUrl,pUTI->szFile))
	{
		pUTI->bDownOK	= true;
	}
	else
		g_bUnZipOK	= 99;

	return 0;
}

static void* ThreadUnZip(void* data)
{
	xnList*	pList	= (xnList*)data;
	for (int i = 0; i < pList->Count; i++)
	{
		UrlTaskInfo* pUTI	= (UrlTaskInfo*)pList->Items[i];
		if(!gfun_TPUnZipFile(pUTI->szFile))
		{
			g_bUnZipOK	= 99;
			return 0;
		}
		g_nUnZipCnt	+= pUTI->nFileCnt;
	}
	g_bUnZipOK	= 2;
	return 0;
}

HttpVersionManager::HttpVersionManager()
{
	g_bUnZipOK	= 0;
	m_nTotalFile	= 0;
	m_dwTotalSize	= 0;
	m_pListDU	= xnList::Create();
	m_dwStartTick	= xnGetTickCount();
}

HttpVersionManager::~HttpVersionManager()
{
	g_bUnZipOK	= 9;

	m_pListDU->Free();
}

void HttpVersionManager::RequestFile( UrlTaskInfo* pUTI )
{//下载URL的任务
	m_pListDU->Add(pUTI);
	pUTI->bDownOK	= false;
#ifdef USE_ANDROID
	cocos2d::CCLog("111111111HttpVersionManager");
	gfun_TPDownZipFile(pUTI->szUrl,pUTI->szFile);
	//	pUTI->bDownOK	= true;
	//else
	//	g_nUnZipCnt		= 99;
#else
	pUTI->pThread	= new pthread_t();
	pthread_create(pUTI->pThread, NULL, ThreadHttpDown, pUTI);
#endif
	
	m_nTotalFile	+= pUTI->nFileCnt;
	m_dwTotalSize	+= pUTI->nSize;
}

void HttpVersionManager::StartUnZip()
{
	if(g_bUnZipOK > 0)
		return;
	g_bUnZipOK	= 1;
	g_nUnZipCnt	= 0;

#ifdef USE_ANDROID
	for (int i = 0; i < m_pListDU->Count; i++)
	{
		UrlTaskInfo* pUTI	= (UrlTaskInfo*)m_pListDU->Items[i];
		if(!gfun_TPUnZipFile(pUTI->szFile))
		{
			g_bUnZipOK	= 99;
			return;
		}
		g_nUnZipCnt	+= pUTI->nFileCnt;
	}
	g_bUnZipOK	= 2;
#else
	pthread_t*	pThread	= new pthread_t();
	pthread_create(pThread, NULL, ThreadUnZip, m_pListDU);
#endif
	//gfun_TPDownZipFile();
}

int HttpVersionManager::GetProgress()
{
	switch(g_bUnZipOK)
	{
	case 0:	//80%
		{
			DWORD	dwPass	= xnGetTickCount() - m_dwStartTick;
			int nRtl	= dwPass*100*80/m_dwTotalSize;
			if(nRtl > 80)
				nRtl	= 80;
			return nRtl;
		}
		break;
	case 1:	//2%
		{
			int	nRtl	= 80+g_nUnZipCnt*20/m_nTotalFile;
			if(nRtl >= 100)
				nRtl	= 99;
			return 99;
		}
		break;
	case 2:
		return 100;
	case 99:
		return -1;
	}

	return 100;
}

int HttpVersionManager::GetEndVersion()
{
	if(m_pListDU->Count)
	{
		UrlTaskInfo* pUTI	= (UrlTaskInfo*)m_pListDU->Items[m_pListDU->Count-1];
		return pUTI->nVID;
	}
	return 0;
}


//---------------------------------------------------------------
CreateSingleton(UrlUpdateCfg);

UrlUpdateCfg::UrlUpdateCfg()
{
	m_pListUrls	= xnList::Create();
}

UrlUpdateCfg::~UrlUpdateCfg()
{
	while(m_pListUrls->Count)
		delete (UrlTaskInfo*)m_pListUrls->Delete(0);
	m_pListUrls->Free();
}

void UrlUpdateCfg::LoadMemory( char* szMemory )
{
	int bufSize	= strlen(szMemory);
	char* szStrBuf = (char *)malloc(bufSize+1);
	memcpy(szStrBuf,szMemory,bufSize);
	szStrBuf[bufSize]	= 0;
	xnExtstr * pStr = xnExtstr::ImportFromMemory(szStrBuf,bufSize);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[32];
	while (pStr->SetSection(index++) != NULL)
	{
		UrlTaskInfo*	pDUrl	= new UrlTaskInfo();
		pDUrl->nVID			= atoi (pStr->sectionName);
		pDUrl->szUrl		= strdup(pStr->ReadString("url",""));
		pDUrl->szFile		= strdup(pStr->ReadString("file",""));
		pDUrl->nFileCnt		= atoi (pStr->ReadString("filecnt","0"));
		pDUrl->nSize		= atoi (pStr->ReadString("size","1"));
		m_pListUrls->Add(pDUrl);
	}
	pStr->Free();
}

void UrlUpdateCfg::Clear()
{
	while(m_pListUrls->Count)
		delete (UrlTaskInfo*)m_pListUrls->Delete(0);
}

UrlTaskInfo::UrlTaskInfo()
{
	bDownOK		= false;
	nUnZipCnt	= 0;
	nDownSize	= 0;

	szFile		= NULL;
	szUrl		= NULL;
	pThread		= NULL;
}

UrlTaskInfo::~UrlTaskInfo()
{
	if(szFile)
		free(szFile);
	if(szUrl)
		free(szUrl);
}
