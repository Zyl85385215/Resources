#ifndef _XN_LOG_FILE_H_
#define	_XN_LOG_FILE_H_

#include <stdio.h>
#include <WINDOWS.H>

struct xnLogFile {
	char * logFileName;
	FILE*	m_pFile;
	DWORD m_dwBeginTime;
	static xnLogFile * Open(char * logFileName);
	void Close(void);
	void Output(char * format, ...);	
};


#endif