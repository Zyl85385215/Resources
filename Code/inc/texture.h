#pragma once

#include <Windows.h>

class IDxTexture
{
public:
	virtual ~IDxTexture(void) {}
	virtual void Render(int x, int y, BYTE alpha, int frame = 0,int nColor=0) = 0;
	virtual void LightRender(int x, int y, BYTE alpha, int frame = 0) = 0;
	virtual void GetRect(int &x, int &y, int & w, int & h, int frame = 0) = 0;
	virtual bool CheckOn(int x, int y, int frame = 0) = 0;
	virtual bool CheckLoaded(void) = 0;
	virtual bool CheckTransparent(void) = 0;
	virtual int GetFrameCount(void) = 0;
};
IDxTexture * LoadDxTexture(const char * szFileName, bool bMultiThread = true);

void RenderDxTexture(const char * szFileName, int nFrameIndex, int x, int y, BYTE alpha, bool bMultiThread = true);
void LightRenderDxTexture(const char * szFileName, int nFrameIndex, int x, int y, BYTE alpha, bool bMultiThread = true);

void GetDxTextureRect(const char * szFileName, int nFrameIndex, int &x, int &y, int &w, int &h, bool bMultiThread = true);
bool CheckDxTextureOn(const char * szFileName, int nFrameIndex, int x, int y, bool bMultiThread = true);

struct xnBmp;
void SetDxTextureDXACBmp(xnBmp * pBmp);	//	dxac格式图片绘制时需要该图片作为像素参考