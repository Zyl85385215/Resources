#ifndef __XN_MOUSE_H__
#define __XN_MOUSE_H__

// for system =================================================================
#include "xngrp16.h"

enum {
	MOUSE_NO_COMMAND,
	MOUSE_LB_ON,
	MOUSE_LB_RELEASE,
	MOUSE_RB_ON,
	MOUSE_RB_RELEASE,
	MOUSE_LB_DBLCLK,
	MOUSE_RB_DBLCLK,
	MOUSE_LB_CLK,
	MOUSE_RB_CLK
};

void xnMouseInit(void);
void xnMouseShutDown(void);

void xnMouseSetCursor(xnSpr * sprite);
void xnMouseSetCursorFrame(int frameNo);
void xnMouseSetDrawCursorFunc(void (* drawFunc)(xnSpr * cursorSpr, int cursorFrame, int x, int y));
void xnMouseShowOff(void);
void xnMouseShowOn(void);

void xnMouseDrawCursor(void);
void xnMouseBackupCursorArea(void);
void xnMouseRestoreCursorArea(void);

int xnMouseGetCmd(void);

// for xninput ================================================================
#include "xninput.h"

enum
{
	MOUSE_RIGHT_BUTTON = 0,
	MOUSE_LEFT_BUTTON = 1,
	MOUSE_WHEELUP_BUTTON,
	MOUSE_WHEELDOWN_BUTTON
};

void xnMousePoll(void);

xnInput * xnMouseAlloc(int keyNumber, BYTE * keyMap);
void xnMouseFree(xnInput * kGroup);

void xnMousePosition(xnInput * kGroup, int *x, int *y);
void xnMouseSetPosition(xnInput * kGroup, int x, int y);

int xnMouseInHWND();	// 获取目前鼠标位于哪个窗口内, 0 MainWindow, 1 ChatWindow

#endif
