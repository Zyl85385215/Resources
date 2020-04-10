#if !defined(SOCKET_CLIENT_H)
#define SOCKET_CLIENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SocketClient.h : header file
//

//
// 注意：
// 在 InitInstance() 函数中调用 AfxSocketInit();函数来初始化网络接口，在
// ExitInstance() 函数中调用 WSACleanup (); 关闭网络接口
//
/////////////////////////////////////////////////////////////////////////////
// CSocketClient command target
#include <afxwin.h> 
#include <afxsock.h>
#include "PublicFunction.h"
#include "Log.h"

#define NET_BUFFER_SIZE			4096		// 默认缓冲大小

class CSocketClient : public CSocket
{
// Attributes
public:
	BOOL Is_Connected() { return m_bConnected; }
// Operations
public:
	CSocketClient ();
	virtual ~CSocketClient();
	CString GetDigitStrAtHead(LPCTSTR lpszStr);
	int GetResponse(CString *pcsResponseStr=NULL, BOOL bBlock=TRUE);
	BOOL GetResponse(int nVerifyCode, CString *pcsResponseStr=NULL);
	CString GetResponseHistoryString () { return m_csResponseHistoryString; }
	void ClearResponseHistoryString () { m_csResponseHistoryString.Empty(); }

// Overrides
public:
	int m_nIndex;
	void Disconnect();
	int Receive(char *szBuf, int size, BOOL bBlock=TRUE);
	BOOL GetIPAndPortByPasvString(LPCTSTR lpszPasvString, OUT CString &csIP, OUT USHORT &nPort);
	void SetEventOfEndModule(HANDLE hEvtEndModule);
	BOOL Send(char *data, int size);
	BOOL SendString(LPCTSTR lpszData, ...);
	BOOL Connect(LPCTSTR lpszHost, USHORT nPort);

	BYTE Connecting(LPCTSTR lpszHostAddress, UINT nHostPort);
// Implementation
protected:
	virtual BOOL ConnectHelper(const SOCKADDR* lpSockAddr, int nSockAddrLen);
private:
	BOOL PumpMessagesMy(UINT uStopFlag);
	CString m_csResponseHistoryString;
	HANDLE m_hEvtEndModule;
	BOOL m_bConnected;
	char m_szServer[256];
	int	m_nPort;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETCLIENT_H__F25829F8_EFFB_4D11_9D58_889B45139518__INCLUDED_)
