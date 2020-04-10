#ifndef DXSPRITE_H
#define DXSPRITE_H

class CDxSprite
{
public:
	virtual DWORD	GetColor(int i = 0) = 0;
	virtual void	SetColor(DWORD col, int i = -1) = 0;
	virtual void	RenderEx(float x, float y, float rot, float hscale = 1.0f, float vscale = 0.0f) = 0;
};

#endif