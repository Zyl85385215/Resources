#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "xngrp16.h"

BOOLEAN xnConsoleInit(xnSpr * skin = NULL);
void xnConsoleShutDown(void);

void xnConsoleUpdate(void);
void xnConsolePutScr(void);

void xnConsoleEnable(void);
void xnConsoleDisable(void);

void xnConsoleOutput(char * format, ...);

void xnConsoleRegisterCommand(char * commandStr, BOOLEAN (*ExecFunc)(char * paramstr));
void xnConsoleUnRegisterCommand(char * commandStr);

#ifdef _DEBUG
#define xnConsoleDebugOut	xnConsoleOutput
#else
#define	xnConsoleDebugOut
#endif

void xnConsoleAddStatus(char * format, void * data, int statusSize);
void xnConsoleDelStatus(void * data);


#endif
