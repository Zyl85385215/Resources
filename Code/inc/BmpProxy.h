#pragma once

#include "GraphicCache.h"


struct xnBmp;

class CBmpProxy: public I2StepLoader
{
public:
	CBmpProxy();
	~CBmpProxy();

	bool			BmpLoadReady(void);
	void			SetBmp(xnBmp * pBmp);
	void			Draw(int x, int y, BYTE alpha);
	void			AddDraw(int x, int y, BYTE alpha);
	void			GetSize(int & w, int & h);
	bool			CheckOn(int x, int y);

protected:
	virtual bool	_onLoadStep1 ( char* szKey);					// 加载器第一步加载,返回是否成功
	virtual bool	_onLoadStep2 ( char* szKey, bool isMutiThread);	// 加载器第二步加载,返回是否成功
private:
	xnBmp *			m_pBmp;
};

typedef COnlyReadCacheNodeHandle<CBmpProxy> CBmpHandle;