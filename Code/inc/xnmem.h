#ifndef __XENON_MEM_H__
#define __XENON_MEM_H__

#define	xnMemAlloc(a)		malloc(a)
#define	xnMemFree(a)		free(a)

#ifdef _DEBUG
	void xnMemFillByte(void * p, BYTE fillVal, long len);
	void xnMemFillWord(void * p, WORD fillVal, long len);
	void xnMemFillDword(void * p, DWORD fillVal, long len);
#else
	void _fastcall xnMemFillByte(void * p, BYTE fillVal, long len);
	void _fastcall xnMemFillWord(void * p, WORD fillVal, long len);
	void _fastcall xnMemFillDword(void * p, DWORD fillVal, long len);
#endif

#define	xnMemZero(p, len) memset(p, 0, len)

#endif
