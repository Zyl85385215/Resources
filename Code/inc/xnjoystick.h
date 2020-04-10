#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include "xninput.h"

enum
{
	JOYSTICK_RIGHT = 0,
	JOYSTICK_LEFT,
	JOYSTICK_UP,
	JOYSTICK_DOWN,
	JOYSTICK_BUTTON = 4
};

void xnJoystickInit(void);
void xnJoystickShutDown(void);
void xnJoystickPoll(void);

xnInput * xnJoystickAlloc(int buttonNumber, BYTE * keyMap);
void xnJoystickFree(xnInput * kGroup);

void xnJoystickPosition(xnInput * kGroup, int *x, int *y);

#endif
