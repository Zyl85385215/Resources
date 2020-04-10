#ifndef __MOUSE_EVEN_H__
#define __MOUSE_EVEN_H__

#include "xngrp16.h"

#define MOUSE_ON_AREA		0x01
#define MOUSE_PRESS 		0x02
#define MOUSE_RELEASE		0x04

#pragma pack (push)
#pragma pack (1)

struct SprObj {
	xnSpr * spr;
	int		frame;
	int x, y;
};

struct MouseEvent {
	SprObj	*evenObj;
	void	*localData;
	void    (* MouseOn)(MouseEvent * p);
	void	(* MouseNormal)(MouseEvent * p);
	void	(* MouseHold)(MouseEvent * p);
	void	(* MousePress)(MouseEvent * p);
	void	(* MouseRelease)(MouseEvent * p);
};

#pragma pack (pop)

void MouseEventAdd(MouseEvent * p);
void MouseEventDelete(MouseEvent * p);
BYTE MouseEventCheck(void);

#endif
