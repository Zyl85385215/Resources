#pragma once

#include <Windows.h>
#include <list>
#include <string>
#include "mhSingleton.hpp"


using namespace std;
class CATLFlashImpl;
class IFlashEventWait4Impl;

// ����Ϸ���ڵ�Flash������
class IGameFlash:public CMSingleton<IGameFlash>
{
public:
	~IGameFlash();

	void			init(HWND hGameWnd);
	void			uninit();

	bool			playFromDisk	( char*  szSwfFileName, IFlashEventWait4Impl* pEvent=NULL);									// pEvent��������ڲ�����release�ͷ�
	bool			playFromMemory	( char*  szSwfFileName, IFlashEventWait4Impl* pEvent=NULL, list<string>* listDepenceFile = NULL);	// pEvent��������ڲ�����release�ͷ�
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