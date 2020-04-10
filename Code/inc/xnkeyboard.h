#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "xninput.h"

enum {
	VK_KEY_0 = 0x30,	VK_KEY_1,	VK_KEY_2,	VK_KEY_3, 	VK_KEY_4,	
	VK_KEY_5,			VK_KEY_6, 	VK_KEY_7, 	VK_KEY_8,	VK_KEY_9, 
	VK_KEY_A = 0x41,	VK_KEY_B,	VK_KEY_C,	VK_KEY_D, 	VK_KEY_E, 
	VK_KEY_F,			VK_KEY_G, 	VK_KEY_H, 	VK_KEY_I, 	VK_KEY_J, 
	VK_KEY_K,			VK_KEY_L, 	VK_KEY_M, 	VK_KEY_N,	VK_KEY_O,			
	VK_KEY_P, 			VK_KEY_Q, 	VK_KEY_R, 	VK_KEY_S,	VK_KEY_T,			
	VK_KEY_U,			VK_KEY_V, 	VK_KEY_W, 	VK_KEY_X, 	VK_KEY_Y, 
	VK_KEY_Z,
	VK_KEY_BILLOW = 0xC0
};

void xnKeyboardInit(void);
void xnKeyboardShutDown(void);
void xnKeyboardPoll(void);
void xnKeyboardClear(void);

xnInput * xnKeyboardAlloc(int keyNumber, BYTE * keyMap);
void xnKeyboardFree(xnInput * kGroup);

#endif
