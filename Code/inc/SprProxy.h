#pragma once

#include "GraphicCache.h"
class	CSpriteOutline;
struct	xnSpr;

class	CSprProxy:public I2StepLoader
{
public:
	enum
	{
		NULLPTR,
		SPRITE,
		OUTLINE,
	};

	struct DrawContent
	{
		CSpriteOutline	*	pOutline;
		xnSpr			*	pRenderSpr;
		xnSpr			*	pOriSpr;
		unsigned char		ucType;

		DrawContent();
		~DrawContent();
	};

	CSprProxy();
	~CSprProxy();

	void			Attach(CSpriteOutline	*	pOutline);
	void			Attach(xnSpr			*	pSpr);
	void			Detach();
	void			Switch();

	void			GetSize(int  nFrame, int& nWidth, int& nHeight);
	void			GetCenter(int nFrame, int& nCenterX, int& nCenterY);
	void			GetSize(int& nWidth, int& nHeight);
	int				GetSizeW(int  nFrame);
	int				GetSizeH(int  nFrame);
	int				GetCenterX(int  nFrame);
	int				GetCenterY(int  nFrame);

	int 			GetTotalFrame();

	void			SaveOutline2File(char * szFileName);

	void			SetGrayRemapColor(BYTE r, BYTE g, BYTE b);
	BOOLEAN			CheckOn(int frame, int x, int y, int ox, int oy);
	void			DrawMask(int frame, int x, int y, WORD maskColor, int alpha = 255, int flipFlag = 0);
	void			Draw(int frame, int x, int y, int alpha = 255, int flipFlag = 0, int nColorType = 0, WORD wRScale256=255, WORD wGScale256=255, WORD wBScale256=255);
	void			StretchDraw(int frame, int x, int y, int w, int h, int alpha = 255, int flipFlag = 0, int nColorType = 0, WORD wRScale256=255, WORD wGScale256=255, WORD wBScale256=255);
	void 			DrawEdge(int frame, int x, int y, WORD color, int edgew, int alpha = 255, int nColorType = 0, WORD wRScale256=255, WORD wGScale256=255, WORD wBScale256=255);
	void 			AddDraw(int frame, int x, int y, int alpha = 255, int flipFlag = 0, int nColorType = 0, WORD wRScale256=255, WORD wGScale256=255, WORD wBScale256=255);
	void			DrawCombinationColor(int frame, int x, int y, WORD color,int alpha = 255,int colorAlpha = 0, int flipFlag = 0, int nColorType = 0, WORD wRScale256=255, WORD wGScale256=255, WORD wBScale256=255); 
	void 			StretchAddDraw(int frame, int x, int y, int w, int h, int alpha = 255, int flipFlag = 0, int nColorType = 0, WORD wRScale256=255, WORD wGScale256=255, WORD wBScale256=255);
	void 			RemapGrayToColorDraw(int frame, int x, int y, int alpha = 255, int flipFlag = 0);
	void 			RemapGrayToColorStretchDraw(int frame, int x, int y, int w, int h, int alpha = 255, int flipFlag = 0);
	void 			RemapColorToGrayDraw(int frame, int x, int y, int alpha = 255, int flipFlag = 0);
	void 			RemapColorToGrayStretchDraw(int frame, int x, int y, int w, int h, int alpha = 255, int flipFlag = 0);
	void			RemapColorSequenceDraw(int frame, int x, int y, BYTE bR, BYTE bG, BYTE bB, int alpha = 255, int flipFlag = 0);
	void			RemapColorSequenceStretchDraw(int frame, int x, int y, int w, int h, BYTE bR, BYTE bG, BYTE bB, int alpha = 255, int flipFlag = 0);

	WORD*			GetPalette(int paletteIndex);
	xnSpr*			GetOriSpr();

protected:
	virtual bool	_onLoadStep1 ( char* szKey);					// 加载器第一步加载,返回是否成功
	virtual bool	_onLoadStep2 ( char* szKey, bool isMutiThread);	// 加载器第二步加载,返回是否成功
private:
	DrawContent		m_xContent;
	WORD			grayRemapTable[32];
	char*			m_szFileName;
};


class CSprHandle:public COnlyReadCacheNodeHandle<CSprProxy>
{
public:
	CSprHandle(char* szKey, WORD wRScale256, WORD wGScale256, WORD wBScale256, int nColorType=0, bool isMutiThread=true);
	CSprHandle(char* szKey, bool isMutiThread=true);
	CSprHandle(const CSprHandle& pHandle);
	CSprHandle();

	bool SetEX(char* szKey, WORD wRScale256, WORD wGScale256, WORD wBScale256, int nColorType=0, bool isMutiThread=true);
};
