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
	char		szIP[16];	//IP��Ϣ
	void*		pExtend;	//��չ��Ϣ	
};

class CHttpURL2IPInfo
{
public:
	JArray<IPInfo>	m_xIP;	//����������IP�б�
	char			m_szUrl[MAX_PATH];
	char			m_szFolder[MAX_PATH];

	CHttpURL2IPInfo(char* szUrl);
	~CHttpURL2IPInfo();
	
	BOOL	GetIPFromServerUrl();//��ȡ��������Ӧ��IP,��ȡʧ��,����������б�
};

class CHttpMulUrlList
{
	//�����ӷ�������Ϣ
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
	int				   m_nMaxNum; //��ӵ��б������ӵ���
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