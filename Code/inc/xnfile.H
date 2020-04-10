#ifndef _XN_FILE_H_
#define _XN_FILE_H_

#include <stdio.h>

typedef void * HANDLE;

enum {
	FILE_READ,
	FILE_WRITE
};
#define use_apifile
struct xnFile {
	DWORD offset; 
	DWORD position;
	DWORD fsize;
	char * fileName;
#ifdef use_apifile
	HANDLE handle;
#else
	FILE * handle;
#endif
	static xnFile * Open(char * fileName, int openType = FILE_READ);
	void	Close(void);
	void	Seek(int seekPos);
	int		Read(void * dataBuf, int readSize);
	int		Write(void * dataBuf, int writeSize);
};

/*
void * xnFileOpenGroup(char * groupName);
void xnFileCloseGroup(void * groupHandle);

void xnFileSetPath(char * PathName);
*/

BOOLEAN xnFileExist(char * fileName);
void * xnFileRead(char * fileName, int * fileSize = NULL,bool blMultithread  = false );
void xnFileReadEnd(void * pData);
int xnFileReadBuf(char * fileName, void * buf, int ReadSize);
int xnFileWrite(char * fileName, void * buf, int writeSize);
int xnFileSize(char * fileName);

void xnFileInit(void);
void xnFileShutDown(void);
void xnFileAddPath(char * path, bool bTop = false);		//	默认插尾部，从头到尾，优先级递减
void xnFileRemovePath(char * path);
BOOLEAN xnFileOpenGroup(char * groupName, BOOLEAN loadInRam = FALSE);
void xnFileCloseGroup(char * groupName);

#endif
