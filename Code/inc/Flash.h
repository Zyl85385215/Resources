#pragma once

#include <Windows.h>
#include <list>
#include <string>
#include "mhSingleton.hpp"


using namespace std;
class CATLFlashImpl;
class IFlashEventWait4Impl;

// 绑定游戏窗口的Flash播放器
class IGameFlash:public CMSingleton<IGameFlash>
{
public:
	~IGameFlash();

	void			init(HWND hGameWnd);
	void			uninit();

	bool			playFromDisk	( char*  szSwfFileName, IFlashEventWait4Impl* pEvent=NULL);									// pEvent传入后由内部调用release释放
	bool			playFromMemory	( char*  szSwfFileName, IFlashEventWait4Impl* pEvent=NULL, list<string>* listDepenceFile = NULL);	// pEvent传入后由内部调用release释放
	bool			getDepenceFilesFromConfig ( char* DepenceFilesFromConfig, list<string>& listDepenceFile );

	void			stopFlash		( );
	bool			isPlaying		( );

	void			setVar(char* szVar, char* szVal);
	char*			getVar(char* szVar);
	char*			callFunction(char* szFunction);
private:
	CATLFlashImpl*	m_pImpl;
	HWND			m_hGameWnd;
};

class IFlashEventWait4Impl
{
public:
	virtual HRESULT onReadyStateChange (long newState );
	virtual HRESULT onProgress (long percentDone );						
	virtual HRESULT fSCommand  (const char* command,const char*  args );
	virtual HRESULT flashCall  (const char*  request );
	virtual void	release    ();
};