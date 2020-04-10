#ifndef __XN_SYSTEM_H__
#define __XN_SYSTEM_H__

#include <windows.h>

extern BOOLEAN	xnSystemExitFlag;
extern DWORD	xnSystemSpeedTick;	
extern DWORD	xnUpdateSpeedTick;	
extern DWORD	xnSystemFPS;
extern DWORD	xnUpdateFPS;

struct xnSystemChunk {
	BOOLEAN	(*Init)(int Inval);
	void	(*ShutDown)(void);
	void	(*Update)(void);
	void	(*PutScr)(void);
};

#define SetChunkEntry(ChunkName,Init, ShutDown, Update, PutScr) \
static BOOLEAN Init(int inVal);									\
static void ShutDown(void);										\
static void Update(void);										\
static void PutScr(void);										\
xnSystemChunk ChunkName = {										\
    Init,                                                       \
    ShutDown,                                                   \
    Update,                                                     \
    PutScr,                                                     \
}

extern	void (*xnSystemDrawBegin)(void);
extern	void (*xnSystemDrawEnd)(void);

void xnSystemExit(void);
BOOLEAN xnSystemLoop(void);
void xnSystemSetSpeed(int UpdateSpeed, int timeBase = 1000);

void xnSystemSetChunk(xnSystemChunk * chunk, int inVal);
void xnSystemRunChunk(xnSystemChunk * chunk, int inVal);
void xnSystemExitChunk(void);

#endif
