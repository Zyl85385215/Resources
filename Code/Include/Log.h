// Log.h: interface for the CLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG_H__9789A39E_6A20_45B8_92C3_7C03C8D6E89C__INCLUDED_)
#define AFX_LOG_H__9789A39E_6A20_45B8_92C3_7C03C8D6E89C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxwin.h>
#define LOGFILE_MAX_SIZE	2*1024*1024					// 日志文件最大字节数
#define MAX_LOGBUF_SIZE		1024*4						// 日志临时缓冲大小
#define EDIT_MAX_TEXT		1024*20						// CEdit 控件最大容纳的文本
// 日志级别
#define L_VERBOSE			0x00000000					// 详细，一般是调试信息
#define L_NORMAL			0x00000001					// 正常，程序运行时显示的一般信息，纪录运行过程
#define L_WARNING			0x00000002					// 警告，程序运行有错误，但不是致命错误，程序仍然可以继续运行下去
#define L_ERROR				0x00000003					// 错误，程序运行出现了严重错误，程序必须立即终止执行
#define L_DEBUG				0x00000004					// 调试，程序在调试版里输出的日志，在Publish版里就没有了
#define L_LOGNUM			5							// 日志级别的数量
// 输出位置
#define L_OUT_CONSOLE		0x00000100					// 输出到控制台
#define L_OUT_DLG			0x00000200					// 输出到对话框
#define L_TRACE				0x00000400					// 用 TRACE() 输出到调试信息栏
#define L_ONLY_LOGFILE		0x00000800					// 只保存到日志文件，不提示任何信息
#define L_BALLOON			0x00001000					// 气泡提示

// 特殊处理
#define L_HND_ENDSYS		0x00010000					// 系统应该结束
#define L_HND_REBOOTSYS		0x00020000					// 系统应该重启
// 日志函数地址
typedef void (*LOG_FUNC)(UINT nLevel, char *format, ...);
// 获取用来给MessageBox()函数做父窗口句柄的回调函数
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
