#pragma once

#include "dxalphachannel_helper.h"
#include "xngrp16.h"
#include "GraphicCache.h"

class CDXACSprite : public IDXACSprite, public I2StepLoader
{
public:
	CDXACSprite(void);
	~CDXACSprite(void);

	void Release(void);
	bool Load(const char * szFile);
	void RenderBmp(int x, int y, xnBmp * pBmp);
	void RenderColor(int x, int y, WORD color);

	bool LoadReady();
	int	 GetCenterX();
	int	 GetCenterY();
	int	 GetSizeW();
	int	 GetSizeH();
	void GetCenter(int& nCenterX, int& nCenterY);


protected:
	virtual bool	_onLoadStep1 ( char* szKey);					// 加载器第一步加载,返回是否成功
	virtual bool	_onLoadStep2 ( char* szKey, bool isMutiThread);	// 加载器第二步加载,返回是否成功

private:
	HDXACINFO m_hInfo;
};

typedef COnlyReadCacheNodeHandle<CDXACSprite> CDXACHandle;
