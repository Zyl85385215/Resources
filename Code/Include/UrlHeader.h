#ifndef _URL_HEADER_H_
#define _URL_HEADER_H_
#include <afxwin.h>
#include <Afxmt.h>
#include <afxtempl.h>
#include "JArray.h"
#include "SocketClient.h"
class CDownloadPub;
struct IPInfo
{
	char		szIP[16];	//IP信息
	void*		pExtend;	//扩展信息	
};

class CHttpURL2IPInfo
{
public:
	JArray<IPInfo>	m_xIP;	//解析出来的IP列表
	char			m_szUrl[MAX_PATH];
	char			m_szFolder[MAX_PATH];

	CHttpURL2IPInfo(char* szUrl);
	~CHttpURL2IPInfo();
	
	BOOL	GetIPFromServerUrl();//获取域名所对应的IP,获取失败,不添加下载列表
};

class CHttpMulUrlList
{
	//多链接服务器信息
public:
	CHttpMulUrlList(){};
	~CHttpMulUrlList(){};
	JArray<CHttpURL2IPInfo> m_xUrlList;

	char	m_szFile[MAX_PATH];
	void	SetFile(char* szFile);
	void	AddInfo(CHttpURL2IPInfo* pInfo);
};

class CServerThreadList;
struct SERVERTHREADINFO
{
	CDownloadPub*		pPub;
	char*				szUrl;
	int					nPort;
	HANDLE				hThread;
	CSocketClient*		pSocketClient;
	SOCKET				sock;
	char*				pExtendData;
	CServerThreadList*	pParent;
	BOOL				bConned;
	SERVERTHREADINFO();
	~SERVERTHREADINFO();;
};

class CServerThreadList
{
	static		int	   _nSendCnt;
	static	    int	   _nConnCnt;
	BOOL		m_bConn;
public:
	SERVERTHREADINFO** m_pList;
	int				   m_nCount;
	int				   m_nMaxNum; //添加到列表中连接的量
	BOOL			   m_bEnd;
	HANDLE			   m_hEvent;
	CCriticalSection   m_csEventLock;
	CCriticalSection   m_csListLock;


//	CServerThreadList();
	CServerThreadList(BOOL	bConn = FALSE);
	~CServerThreadList();;

	BOOL			Add(SERVERTHREADINFO* pInfo);
	void			Delete(SERVERTHREADINFO* pInfo,BOOL bSaveSocket=FALSE);
	BOOL			DisConnByIndex(int nIndex);
};
#endif