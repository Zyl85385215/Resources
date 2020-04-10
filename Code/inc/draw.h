#ifndef _DX_DRAW_H_
#define _DX_DRAW_H_

#include <windows.h>
#include <stdio.h>
#include <ddraw.h>

//======================================================================
//
//		DIRECT Draw
//
//======================================================================

#ifdef	__cplusplus
extern  "C" {
#endif

#define SCALE_BASE	256

/*
enum {
	Mode320x240x16,
    Mode640x480x16,
    Mode800x600x16,
	Mode1024x768x16,
    TOTAL_MODE,
};
*/

BOOL DrawInit(BYTE ModeFlag);
void DrawShutDown(void);

bool DrawInit_Chat(int nWidth, int nHeight,int nBits);

void DrawShowScrBuf(WORD * scrBuf);
void DrawShowEnd(void);

void * DrawCreateSurface(DWORD W, DWORD H);
void DrawDeleteSurface(void * lpDD_Ptr);
void DrawCleanSurface(void * lpDD_Ptr, DWORD color);
void DrawGetSurfaceDDSD(void * lpDD_Ptr, DDSURFACEDESC * ddsd);

void DrawSurfaceCopy(int x, int y, void * lpDD_Ptr);
void DrawSurfaceFastCopy(int x, int y, void * lpDD_Ptr);
void DrawSurfaceScaleCopy(int x, int y, int scaleX, int ScaleY, void * lpDD_Ptr);

//void * DrawLockSurface(void * lpDD_Ptr, DWORD * Pitch);
#define DrawLockSurface(surfaceBuf, lpDD_Ptr, Pitch)	\
{														\
	HRESULT r;											\
	DDSURFACEDESC		ddsd;							\
	LPDIRECTDRAWSURFACE lpDD_Tmp;						\
	lpDD_Tmp = (LPDIRECTDRAWSURFACE)lpDD_Ptr;			\
	ZeroMemory(&ddsd, sizeof(ddsd));					\
	ddsd.dwSize = sizeof(ddsd); 						\
	while (1)											\
	{													\
		r = lpDD_Tmp->Lock(0, &ddsd, DDLOCK_WAIT, 0);	\
		if (r == DD_OK) 								\
		{												\
			surfaceBuf = (BYTE *)ddsd.lpSurface;		\
			Pitch = ddsd.lPitch;						\
			break;										\
		}												\
		if (r == DDERR_SURFACELOST) 					\
		{												\
			while (r != DD_OK)							\
				r = lpDD_Tmp->Restore();				\
		}												\
	}													\
}

// void DrawUnlockSurface(void * lpDD_Ptr);
#define DrawUnlockSurface(lpDD_Ptr) 					\
{														\
	LPDIRECTDRAWSURFACE lpDD_Tmp;						\
	lpDD_Tmp = (LPDIRECTDRAWSURFACE)lpDD_Ptr;			\
	lpDD_Tmp->Unlock(0);								\
}

#ifdef	__cplusplus
}
#endif

#ifdef  __cplusplus

/*======================================================================*/

#endif  /* __cplusplus */

#pragma comment(lib,"ddraw.lib")

#endif	/* _DXDRAW_H_ */
