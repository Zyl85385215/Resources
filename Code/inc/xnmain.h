#ifndef __XN_MAIN_H__
#define __XN_MAIN_H__

#include "xnsystem.h"

extern HWND MainWinHandle;
extern HWND ChatWinHandle;
extern HINSTANCE MainProgInstance;
extern BOOLEAN bActive;				// is application active?
extern BOOLEAN bFocus;

extern HWND g_hwndParent;

typedef void (*GFun_DownLoadSpr)(char* szFile);
extern	GFun_DownLoadSpr	gfun_DownLoadSpr;

//****************************************************************

enum
{
	XNSYSTEM_GAME_WINNAME,
	XNSYSTEM_GAME_TITLE,
	XNSYSTEM_GAME_EXITTXT,
	XNSYSTEM_GAME_EXITTITLE,

	XNSYSTEM_SINGLE_INSTANCE,
	XNSYSTEM_SINGLE_ERRMSG,
	XNSYSTEM_CAN_OPENWNDCNT,
	XNSYSTEM_TOOMANY_ERRMSG,

	XNSYSTEM_HWNDPARENT,
	XNSYSTEM_WIDTH,
	XNSYSTEM_HEIGHT,

	XNSYSTEM_CHAT_WIDTH,

	XNSYSTEM_HIDE_MOUSE,
};

void xnSystemSetState(int nType, int nValue);
void xnSystemSetState(int nType, const char * szValue);
void xnSystemSetState(int nType, bool bValue);
void xnSystemSetState(int nType, HWND hwnd);

bool xnSystemInit(void);
bool xnSystemStart(void);
void xnSystemShutDown(void);
void xnSystemSetDownLoadFun(GFun_DownLoadSpr	fnDownSpr);

#endif
