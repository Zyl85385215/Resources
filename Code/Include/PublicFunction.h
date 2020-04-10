// PublicFunction.h: interface for the CPublicFunction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PUBLICFUNCTION_H__654F176C_F11F_4843_AEA0_F53676D52341__INCLUDED_)
#define AFX_PUBLICFUNCTION_H__654F176C_F11F_4843_AEA0_F53676D52341__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxwin.h>
#include <afxdtctl.h>
#define SLEEP_RETURN(x)\
{\
	if(::WaitForSingleObject(m_hEvtEndModule, x) == WAIT_OBJECT_0)\
		return FALSE;\
}
#define SLEEP_BREAK(x)\
{\
	if(::WaitForSingleObject(m_hEvtEndModule, x) == WAIT_OBJECT_0)\
		break;\
}
// 进入临间区
#define EnterCRIT(pCRIT,CRITName,FileName,Line) \
{\
	(pCRIT)->Lock ();\
	TRACE("--------> %s , EnterCriticalSection ,%s(%d)\n", CRITName, FileName, Line);\
}
// 离开临间区
#define LeaveCRIT(pCRIT,CRITName,FileName,Line) \
{\
	TRACE("<-------- %s , LeaveCriticalSection ,%s(%d)\n", CRITName, FileName, Line);\
	(pCRIT)->Unlock();\
}

class CHwDir  
{
public:
	CHwDir(
		LPCTSTR lpszBasePathOrFile,
		BOOL bSerachSubDir = TRUE,		// 搜索子目录
		BOOL bAbsolutePath =TRUE,		// 是绝对路径
		CHwDir **ppHwDir=NULL			// 将这个类的指针传出去给调用者
	);
	virtual ~CHwDir();

public:
	ULONGLONG GetAmountBytes();
	CStringArray *m_pStrAryResFile, *m_pStrArySubDirectory;
	char m_szBasePathFile[MAX_PATH];
	void Cancel();
private:
	DWORD FindAllFileUnderOneDir(LPCTSTR lpszFileName,LPCTSTR lpszDirectory,BOOL bFindDir);
	DWORD Dir();
	int HandleOneFile(LPCTSTR lpszDirectory, WIN32_FIND_DATA* pFindData,BOOL bFindDir);
	DWORD FindDirAndFile(LPCTSTR lpszDirectory);
private:
	BOOL m_bCancel;
	CStringArray *m_pStrArySubDirectory_Private;
	DWORD m_dwDirNum;
	ULONGLONG m_AmountBytes;
	BOOL m_bSerachSubDir;
	BOOL m_bAbsolutePath;			//是否要保存绝对路径
	DWORD m_dwRelativePathStartPos;	//相对路径的起始位置
	char m_strFilter[MAX_PATH/2];
};


class CHwDirEx  
{
public:
	CHwDirEx(
		LPCTSTR lpszMultiFindPath,					// 要搜索的路径，如“E:\\winnt\\;d:\temp\\;”
		LPCTSTR lpszMultiFindFilter,				// 要搜索的过滤条件，如“*.bmp;*.exe;”
		LPCTSTR lpszMultiExcludeFilter=NULL,		// 要排除的过滤条件，如“*.bmp;*.exe;”
		BOOL bSerachSubDir = TRUE,					// 搜索子目录
		BOOL bAbsolutePath =TRUE,					// 是绝对路径
		CHwDirEx **ppHwDirEx=NULL					// 将这个类的指针传出去给调用者
);
	ULONGLONG GetAmountBytes();
	virtual ~CHwDirEx();
	void Cancel();

	CStringArray *m_pStrAryResFile, *m_pStrArySubDirectory;
private:
	CHwDir* m_pHwDir;
	BOOL m_bCancel;
	void DirAll(
		LPCTSTR lpszFindPath,						// 要搜索的路径，如“E:\\winnt\\”
		LPCTSTR lpszMultiFindFilter,				// 要搜索的过滤条件，如“*.bmp;*.exe;”
		LPCTSTR lpszMultiExcludeFilter=NULL,		// 要排除的过滤条件，如“*.bmp;*.exe;”
		BOOL bSerachSubDir = TRUE,					// 搜索子目录
		BOOL bAbsolutePath =TRUE					// 是绝对路径
);
	void Dir(
		LPCTSTR lpszFindPath,						// 要搜索的路径，如“E:\\winnt\\”
		LPCTSTR lpszMultiFindFilter,				// 要搜索的过滤条件，如“*.bmp;*.exe;”
		CStringArray &StrAryResFile,				// 搜索的文件保存到此
		CStringArray &StrArySubDirectory,			// 搜索的子目录保存到此
		DWORD &dwAmountBytes,						// 总字节数保存到此
		BOOL bSerachSubDir = TRUE,					// 搜索子目录
		BOOL bAbsolutePath =TRUE					// 是绝对路径
);
	DWORD m_dwDirNum;
	ULONGLONG m_AmountBytes;
};
//==========================================================================
// 常用操作宏
//==========================================================================
#define GET_VALID_STRING_FROM_TOW(cs1,cs2)((cs1.GetLength()>0)?cs1:cs2)
#define GET_SAFE_STRING(str)((str)?(str):"")
#define NULL_STRING_FOR_DB ""
#define GET_VALID_CSTRING(cs)((cs).GetLength()>0?(cs):((cs),(cs)=NULL_STRING_FOR_DB))
#define GET_VALID_CSTRING_P(csp)((csp)?(*(csp)):"")
#define STRNCPY_CS(sz,cs) strncpy((char*)(sz),(cs).GetBuffer(0),sizeof(sz))
#define STRNCPY_SZ(sz1,sz2) strncpy(((char*)(sz1)),(sz2)?((char*)(sz2)):"",sizeof(sz1))
#define STRNCPY(sz1,sz2,size) \
{\
	strncpy(((char*)(sz1)),(sz2)?((char*)(sz2)):"",(size));\
	((char*)(sz1))[(size)-1] = '\0';\
}
// 时间类型的数据长度
#define DATETIME_TYPE_LENGTH				20
#define STRCPY(sz1,sz2) strcpy((char*)(sz1), (char*)((sz2)?(sz2):""))
#define STRLEN_SZ(sz) ((sz)?strlen((char*)(sz)):0)
#define COPMNC_CS_SZ(cs,sz)((sz) && ((cs).CompareNoCase(sz)==0))
#define STRCMP_SAFE(sz1,sz2) (strcmp((char*)GET_SAFE_STRING(sz1),(char*)GET_SAFE_STRING(sz2)))
#define STRLEN_SAFE(sz) ((sz)?strlen((char*)(sz)):0)
#define ATOI_SAFE(sz) (atoi((const char*)(GET_SAFE_STRING((char*)(sz)))))
#define ASSERT_ADDRESS(p,size) ASSERT((p)!=NULL && AfxIsValidAddress((p),(size),TRUE))
#define VALID_IP_PORT(ip,port) ((STRLEN_SAFE(ip)>0) && (port)>1000)
#define LENGTH(x) sizeof(x)/sizeof(x[0])
#define MIN(x,y) (((DWORD)(x)<(DWORD)(y))?(x):(y))
#define MAX(x,y) (((DWORD)(x)>(DWORD)(y))?(x):(y))
// 有效的句柄
#define HANDLE_IS_VALID(h) ((HANDLE)(h) && ((HANDLE)(h)!=INVALID_HANDLE_VALUE))
// 关闭句柄
#define CLOSE_HANDLE(h)\
{\
	if(HANDLE_IS_VALID(h))\
	{\
		::CloseHandle(h);\
		(h) = NULL;\
	}\
}
//
// 本软件所用到自定义消息都放到这里了，以保证唯一性
//
enum
{
	WM_SHOWLOG = WM_USER+0x01000,
};
int ConvertStrToCTime(char *chtime, CTime &cTime);
CString GetOneLine(CString &str);
int GetMouthByShortStr(LPCTSTR lpszShortMonth);
CString hwFormatMessage(DWORD dwErrorCode);
int WriteDataToFile(LPCTSTR filename,char* data,long size,LPCTSTR mode, int nStartPos=-1);
CString Data2HexString(int nOffset, char *data, int size);
void DbgLog(LPCTSTR lpszFormat, ...);
int ReadDataFromFile(LPCTSTR filename,char* data,long size, int nStartPos=-1);
char* MakeSureDirectory(LPCTSTR lpszDirName);
int hwSnprintf(char *buffer, int count, const char *format, ...);
BOOL PartPathAndFileAndExtensionName (
		IN LPCTSTR lpszFilePath,			// 全路径名(包含文件名)
		OUT CString *pcsOnlyPath,			// 输出光路径（没有文件名）
		OUT CString *pcsOnlyFileName,		// 输出光文件名（没有路径）
		OUT CString *pcsExtensionName		// 输出扩展名
	);
BOOL PartFileAndPathByFullPath (
		LPCTSTR lpszFilePath,
		char *szOnlyFileName,
		int nFileNameSize,
		char *szOnlyPath =NULL,
		int nPathSize=0
);
DWORD GetCurTimeString(char *buf, time_t tNow=0);
CString GetCurTimeString(time_t tNow=0);

template<class T1, class T2>
int FindFromArray(IN T1 &Ary, IN T2 Find)
{
	int nCount = Ary.GetSize();
	for(int i=0; i<nCount; i++)
	{
		if(Ary.GetAt(i) == Find)
			return i;
	}
	return -1;
}

template<class T1, class T2, class T3, class T4>
int FindFromArray(IN T1 &Ary, IN T2 Find, IN T3 &AppAry, IN T4 AppFind)
{
	int nCount = Ary.GetSize();
	for(int i=0; i<nCount; i++)
	{
		if(Ary.GetAt(i) == Find && 
			AppAry.GetAt(i) == AppFind)
		{
			return i;
		}
	}
	return -1;
}
void StandardizationPathBuffer(char *szPath, int nSize, char cFlagChar='\\');
CString StandardizationFileForPathName(LPCTSTR lpszFileOrPathName, BOOL bIsFileName, char cReplaceChar='_');
char *hwStrrChr(const char *string, int c);
char *hwStrChr(const char *string, int c);
BOOL PartFileAndExtensionName (
		IN LPCTSTR lpszFileName,
		OUT char *szFileName,
		IN int nFileNameSize,
		OUT char *szExtensionName=NULL,
		IN int nExtensionNameSize=0);
BOOL hwDeleteFolder(LPCTSTR lpszFolder);
BOOL CopyFileAppend(LPCTSTR lpszFileName_Src, LPCTSTR lpszFileName_Dst, int nOffset);
BOOL CreateNullFile(LPCTSTR lpszFileName, int nFileSize);
BOOL WaitForThreadEnd(HANDLE hThread, DWORD dwWaitTime=5000);
DWORD SelectPathByCommonDlg(LPSTR lpszPathName,HWND hwndOwner=NULL);
BOOL SelectPathByCommonDlg(CWnd *pDlg, UINT nEditID);
CString FormatFileSize(double fFileSize);

#endif // !defined(AFX_PUBLICFUNCTION_H__654F176C_F11F_4843_AEA0_F53676D52341__INCLUDED_)
