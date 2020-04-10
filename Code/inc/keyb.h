#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

struct useKeyData
{
	int 	useKeyNo;
	BYTE *	useKey;
	BOOLEAN *  keyState;
};

void keybCallBack(void);
void keybGetStatus(useKeyData * useKey);

#endif
