#ifndef _EXTERN_STRING_H_
#define _EXTERN_STRING_H_

#include <windows.h>

struct xnExtstr {
	char * sectionName;
	int	sectionIndex;
	int keyCount;
	int	sectionCount;
	static xnExtstr * Import(char * fileName);
	static xnExtstr * ImportFromMemory(void * buffer, int bufSize);
	void	Free(void);
	BOOLEAN	SetSection(char * section);
	BOOLEAN SetSection(int sectionIndex);
	char *	ReadString(int index, char * defaultResult = NULL);
	char *	ReadString(char * key, char * defaultResult = NULL);
	char * GetKey(int index, char * defaultResult);
};

#endif