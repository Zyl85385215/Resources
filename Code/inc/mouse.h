#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "xngrp16.h"

#pragma pack (push)
#pragma pack (1)

struct mouseType {
	int x, y;
	int oldX, oldY;
	BOOLEAN showFlag;
	BOOLEAN backupFlag;
	xnSpr * sprite;
	int frameNo;
	bmpType * BG_Backup;
	void (* draw)(mouseType * p);
	void (* backupBG)(mouseType * p);
	void (* restoreBG)(mouseType * p);
};

enum {
	NO_COMMAND,
	LB_ON,
	LB_RELEASE,
	RB_ON,
	RB_RELEASE,
	LB_DBLCLK,
	RB_DBLCLK,
};
#pragma pack (pop)

extern mouseType mouseData;

#define	mouseOn()	mouseData.showFlag = TRUE
#define	mouseOff()	mouseData.showFlag = FALSE

void mouseSetCursor(xnSpr * sprite);
void mouseFreeCursor(void);
void mouseSetCursorFrame(int frameNo);
void mouseSaveCommand(BYTE command);
BYTE mouseGetCommand(void);

// =================================================================
void mouseBackupBG(mouseType * p);
void mouseRestoreBG(mouseType * p);
void mouseDrawCursor(mouseType * p);


#endif
