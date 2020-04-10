// Log.h: interface for the CLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG_H__9789A39E_6A20_45B8_92C3_7C03C8D6E89C__INCLUDED_)
#define AFX_LOG_H__9789A39E_6A20_45B8_92C3_7C03C8D6E89C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxwin.h>
#define LOGFILE_MAX_SIZE	2*1024*1024					// ��־�ļ�����ֽ���
#define MAX_LOGBUF_SIZE		1024*4						// ��־��ʱ�����С
#define EDIT_MAX_TEXT		1024*20						// CEdit �ؼ�������ɵ��ı�
// ��־����
#define L_VERBOSE			0x00000000					// ��ϸ��һ���ǵ�����Ϣ
#define L_NORMAL			0x00000001					// ��������������ʱ��ʾ��һ����Ϣ����¼���й���
#define L_WARNING			0x00000002					// ���棬���������д��󣬵������������󣬳�����Ȼ���Լ���������ȥ
#define L_ERROR				0x00000003					// ���󣬳������г��������ش��󣬳������������ִֹ��
#define L_DEBUG				0x00000004					// ���ԣ������ڵ��԰����������־����Publish�����û����
#define L_LOGNUM			5							// ��־���������
// ���λ��
#define L_OUT_CONSOLE		0x00000100					// ���������̨
#define L_OUT_DLG			0x00000200					// ������Ի���
#define L_TRACE				0x00000400					// �� TRACE() �����������Ϣ��
#define L_ONLY_LOGFILE		0x00000800					// ֻ���浽��־�ļ�������ʾ�κ���Ϣ
#define L_BALLOON			0x00001000					// ������ʾ

// ���⴦��
#define L_HND_ENDSYS		0x00010000					// ϵͳӦ�ý���
#define L_HND_REBOOTSYS		0x00020000					// ϵͳӦ������
// ��־������ַ
typedef void (*LOG_FUNC)(UINT nLevel, char *format, ...);
// ��ȡ������MessageBox()�����������ھ���Ļص�����
typedef HWND (*FUNC_GetHwndForMsgBox) ();

#ifdef _DEBUG
#define HwDbgLog Log
#else
#define HwDbgLog
#endif

void Log(UINT nLevel, char *format, ...);
void GetLastLogStrAry(CStringArray &StrAry, CUIntArray &UIntAry);
/*void AddLogTextToEditCtrl(CRichEditCtrl *pEditLog, LPCTSTR lpszLogText, int nLevel);*/
int hwMsgBox (
		HWND hWnd,
		LPCTSTR lpCaption, 
		UINT uType,
		LPCTSTR lpText,
		...
	);
void Set_RecvLogMsgHwnd(HWND hWnd_RecvLogMsg);

#endif // !defined(AFX_LOG_H__9789A39E_6A20_45B8_92C3_7C03C8D6E89C__INCLUDED_)
