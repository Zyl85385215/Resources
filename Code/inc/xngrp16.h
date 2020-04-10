#ifndef _16BITS_GRAPHIC_H_
#define _16BITS_GRAPHIC_H_

#include <string.h>
#include <stdio.h>

#define	KEY_COLOR_ON		TRUE
#define	KEY_COLOR_OFF		FALSE

extern	int		SCR_WIDTH;
extern	int		SCR_HEIGHT;
extern	int		SCR_BITS;
extern	BOOLEAN	is565;

extern	int		WINDOW_WIDTH;
extern	int		WINDOW_HEIGHT;

enum {
	LR_FLIP = 0x01, 
	UD_FLIP = 0x02
};

enum
{
	GRP_MIX_FUNCTION_NORMAL,
	GRP_MIX_FUNCTION_MMX,
	GRP_MIX_FUNCTION_SSE2,
	GRP_MIX_FUNCTION_MAX,
};

#pragma pack (push)
#pragma pack (1)

//=========================
// clipType
struct clipType {
	int x1, y1;
	int x2, y2;
	int	vw, vh;
};

// =========================
// bmp && jpeg-->bmp && save to jpeg

// bmpType  bmp文件格式 宽-高-内容
struct bmpType {
	int		w;
	int		h;
	WORD	data[1];
};

struct xnBmp {
	int				Width;
	int				Height;
	BOOLEAN			KeyColorFlag;
	bool			m_blReverse;
	void	*		Data;
	WORD    *		Line[1]; // bmp是上下反得,line指向data中倒序行的首地址

	static xnBmp *	Create(int w, int h,bool blReverse = false);
	static xnBmp *	LoadBmp(char * bmpFileName);
	static xnBmp *	LoadJpeg(char * jpegFileName);
	void			SaveToJpeg(char * jpegFileName, int quality);

	xnBmp *			Get(int x1, int y1, int x2, int y2);
	xnBmp *			Duplicate(void);
	void			CopyBmp(xnBmp** ppBmp);
	void			Free(void);
	void			Draw(int x, int y, int alpha = 255, int flipFlag = 0);
	void			DrawTransBmp(xnBmp * BmpSrc,int x, int y, int w, int h ,int alpha = 255, int flipFlag=0, bool bAll=false);
	void			StretchDraw(int x, int y, int w, int h, int alpha = 255,  int flipFlag = 0);
	void			AddDraw(int x, int y, int alpha = 255, int flipFlag = 0);
	void			AddStretchDraw(int x, int y, int w, int h, int alpha = 255,  int flipFlag = 0);
	void			MoveImage(int dx,int dy);

	void			RenderAlpha(BYTE alpha, int x, int y, int len);
};

// =========================
// dxac
class IDXACSprite
{
public:
	virtual void Release(void) = 0;
	virtual void RenderBmp(int x, int y, xnBmp * pBmp) = 0;
	virtual void RenderColor(int x, int y, WORD color) = 0;
};
IDXACSprite * LoadDXACSprite(const char * szFile);

// =======================
// sprite
// xnSprExt2 没有用到哦
struct xnSprExt2 {
    char    objName[32];
    int     objWidth;
	int		objHeight;
    int     objDirection;
	int     objFrameRate;
    int     objFrameCount[1];
};

// 调色板
struct xnSpr8Bit {
	int		paletteCount;	// 多少种颜色
	WORD	palette[1];		// 颜色值
};


enum
{
	R,
	G,
	B,
};


struct xnFrame {
	int			w;
	int			h;
	int			cx;
	int			cy;
	WORD	*	lineData[1];

	xnFrame *	MakeMask(WORD maskColor);
	xnFrame *	Duplicate(void);

	void		GetSize(int *width, int *height);
	void	    GetCenter(int& nCenterX, int& nCenterY);
	BOOLEAN		CheckOn(int x, int y, int ox, int oy);
	void		DrawMask(int x, int y, WORD maskColor, int alpha = 255, int flipFkag = 0);
	void		Draw(int x, int y, int alpha = 255, int flipFlag = 0);
	void		StretchDraw(int x, int y, int w, int h, int alpha = 255, int flipFlag = 0);
	void		DrawEdge(int x, int y, WORD color, int edgew, int alpha = 255);
	void		AddDraw(int x, int y, int alpha = 255, int flipFlag = 0);
	void		DrawCombinationColor(int x, int y, WORD color ,int alpha = 255, int colorAlpha = 0,int flipFlag = 0);
	void		StretchAddDraw(int x, int y, int w, int h, int alpha = 255, int flipFlag = 0);
	void		RemapGrayToColorDraw(int x, int y, WORD * remapTable, int alpha = 255, int flipFlag = 0);
	void		RemapGrayToColorStretchDraw(int x, int y, int w, int h, WORD *remapTable, int alpha = 255, int flipFlag = 0);
	void		RemapColorToGrayDraw(int x, int y, WORD * remapTable, int alpha = 255, int flipFlag = 0);
	void		RemapColorToGrayStretchDraw(int x, int y, int w, int h, WORD *remapTable, int alpha = 255, int flipFlag = 0);
	void		RemapColorSequenceDraw(int x, int y, BYTE bR, BYTE bG, BYTE bB, int alpha = 255, int flipFlag = 0);
	void		RemapColorSequenceStretchDraw(int x, int y, int w, int h, BYTE bR, BYTE bG, BYTE bB, int alpha = 255, int flipFlag = 0);
	void		GetFrame(int x, int y);
	void		Brightness(int k);
	void		Free(void);
};

struct xnSpr {

	enum
	{
		R_G_B,
		R_B_G,
		B_R_G,
		B_G_R,
		G_R_B,
		G_B_R,
		GRAY,
		GRAY0,
		R_G_B_MAX,
	};

	int			totalFrame;
    int			version;
    void *		extData;
	xnFrame	**	frame;
	BYTE	*	spriteData;
	WORD		grayRemapTable[32];


	static xnSpr *	LoadSpr(char * spriteFileName, bool bIsMutiThread=false);
	static xnSpr *	LoadOriginalSpr(char * spriteFileName, bool bIsMutiThread=false);
	xnSpr	*		DuplicateSprite();
	xnSpr   *		DuplicateSprite(int paletteIndex);
	xnSpr   *		DuplicateSprite(WORD wArrPalette[]);
	xnSpr   *		DuplicateSprite(WORD wRRebate, WORD wGRebate, WORD wBRebate, BYTE byColorType );
	xnFrame *		MakeMask(int frame, WORD maskColor);
	xnFrame *		DuplicateFrame(int frame);
	xnFrame *		DuplicateFrame(int frame, int	paletteIndex);
	xnFrame *		DuplicateFrame(int frame, WORD wArrPalette[]);

	WORD	*		GetPalette(int	paletteIndex);

	void			Free(void);
	void			GetSize(int frame, int *width, int *height);
	void			GetSize(int *width, int *height);
	void			GetCenter(int nFrame, int& nCenterX, int& nCenterY);
	void			SetGrayRemapColor(BYTE r, BYTE g, BYTE b);
	BOOLEAN			CheckOn(int frame, int x, int y, int ox, int oy);
	void			DrawMask(int frame, int x, int y, WORD maskColor, int alpha = 255, int flipFlag = 0);
	void			Draw(int frame, int x, int y, int alpha = 255, int flipFlag = 0);
	void			StretchDraw(int frame, int x, int y, int w, int h, int alpha = 255, int flipFlag = 0);
	void 			DrawEdge(int frame, int x, int y, WORD color, int edgew, int alpha = 255);
	void 			AddDraw(int frame, int x, int y, int alpha = 255, int flipFlag = 0);
	void			DrawCombinationColor(int frame, int x, int y,WORD color,int alpha = 255, int colorAlpha = 0,int flipFlag = 0);
	void 			StretchAddDraw(int frame, int x, int y, int w, int h, int alpha = 255, int flipFlag = 0);
	void 			RemapGrayToColorDraw(int frame, int x, int y, int alpha = 255, int flipFlag = 0);
	void 			RemapGrayToColorStretchDraw(int frame, int x, int y, int w, int h, int alpha = 255, int flipFlag = 0);
	void 			RemapColorToGrayDraw(int frame, int x, int y, int alpha = 255, int flipFlag = 0);
	void 			RemapColorToGrayStretchDraw(int frame, int x, int y, int w, int h, int alpha = 255, int flipFlag = 0);
	void			RemapColorSequenceDraw(int frame, int x, int y, BYTE bR, BYTE bG, BYTE bB, int alpha = 255, int flipFlag = 0);
	void			RemapColorSequenceStretchDraw(int frame, int x, int y, int w, int h, BYTE bR, BYTE bG, BYTE bB, int alpha = 255, int flipFlag = 0);
};

// ==============
// 字体
typedef int			(*drawface_func)(int x,int y,BYTE bIndex);
typedef int			(*GetFaceWidth_Func)(BYTE bIndex);
typedef void		(*drawfaceex_func)(int x,int y, BYTE byEventIndex, char* szEventParam);


class FontEventParam
{
public:
	FontEventParam(char* szStr);
	FontEventParam(DWORD dwVal);

private:
	char	m_szStr[6];
	DWORD	m_dwVal;
};

struct xnFont {
	int		charWidth, charHeight;
	BYTE	language;
	WORD	color;
	WORD	boundColor;
	BOOLEAN	antialiased;
	BYTE	boundSize;
	int		alpha;
	int		lineSpace;
	BOOLEAN	enableLineReturn;
	BOOLEAN	shadow;
	drawface_func drawfacefunc;
	GetFaceWidth_Func getFaceWidthFunc;
	int		nFaceW;
	drawfaceex_func		drawfaceexfunc;
	static	xnFont * Load(char * fontName, int loadType);
//	static  xnFont * Load(char * fontName, int loadType, BYTE language = FONT_CHINESE);
	void	Free(void);
	void	SetColor(BYTE R, BYTE G, BYTE B);
	void	SetBoundColor(BYTE R, BYTE G, BYTE B);
	int		ShowText(int x, int y, int w, char * format, ...);
	int		GetTextLastPos(int &x, int &y, int w, char * format, ...);
	int		ShowEnglishText(int x, int y, int w, char * format, ...);
	int		ShowTextArea(int x1, int x2, int y, char * format, ...);
	int		GetTextLen(char * format, ...);
	int		GetTextHeight(int w, char * format, ...);
	BOOLEAN	IsSingleChar(char * charBuf);
	void	SetFaceDrawFunc(drawface_func func);
	void	SetFaceDrawFuncEX(drawfaceex_func func);
	int	    GetCharWidth(WORD wCharCode);	
	void	SetFaceWidthFunc(GetFaceWidth_Func func);
	int     GetFaceWidth(BYTE bIndex);
	
	static	void	insertFontEvent(char* szContent, BYTE byEventIndex, char* szEventParamFormat, ...);
};

// ============
// 图像绘制相关函数,起始就是一个对象的虚函数表,但是好处是,函数可以赋值来赋值去,但缺少对象性
struct xnGrpFuncObj{
	// 图片颜色一般都是565的,如果显示是555的就要转颜色了(下面三个函数的作用)
	void    (* convSprite)(xnSpr * sprite); 
	void	(* convBmp)(bmpType * bmp);
	void	(* convPal)(WORD * pal, int palSize);

	WORD	(* getColor)(BYTE R, BYTE G, BYTE B);					// 获取RGB 16位值
	void	(* getColorRBG)(WORD Color, BYTE &R, BYTE &G, BYTE &B);	// 获取颜色的各自RGB
	WORD	(* brightColor)(WORD color, int bright);				// RGB都乘以bright,高亮
	WORD	(* getMixColor)(WORD src, WORD color, int alpha);		// color混合到src中
	WORD	(* getAddColor)(WORD src, WORD color, int alpha);		// color
	void	(* MixBuffer)(WORD * dest, WORD * src, int mixSize, int alpha);
	void	(* MixBufferNormal)(WORD * dest, WORD * src, int mixSize, int alpha);
	void	(* MixBufferMMX)(WORD * dest, WORD * src, int mixSize, int alpha);
	void	(* MixBufferSSE2)(WORD * dest, WORD * src, int mixSize, int alpha);
	void	(* AddBuffer)(WORD * dest, WORD * src, int addSize, int alpha);
	void	(* CombinationBuffer)(WORD * dest, WORD * src, WORD addSize, int alpha, int colorAlpha, WORD color);
	void	(* MixBufferColor)(WORD * dest, WORD color, int mixSize, int alpha);
	void	(* MixBufferColorNormal)(WORD * dest, WORD color, int mixSize, int alpha);
	void	(* MixBufferColorMMX)(WORD * dest, WORD color, int mixSize, int alpha);
	void	(* MixBufferColorSSE2)(WORD * dest, WORD color, int mixSize, int alpha);
};

#pragma pack (pop)

extern xnBmp        *	xnGrpScreenBmp;
extern xnGrpFuncObj *	xnGrpFunc;
extern clipType			xnGrpScrClip;

// normal function
enum EGAME_WINDOW_MODE
{
	GAME_WINDOW_MODE_WINDOW,
	GAME_WINDOW_MODE_FULLSCREEN,
	GAME_WINDOW_MODE_FADE_FULLSCREEN,
};

#define GAME_WINDOW_MODE_MAX (GAME_WINDOW_MODE_FADE_FULLSCREEN + 1)

bool	xnGrpSetWindowMode(EGAME_WINDOW_MODE eWindowMode);
EGAME_WINDOW_MODE	xnGrpGetWindowMode(void);
void	xnGrpSetWindowSizeBox(bool bSizeBox);

bool	xnGrpInit(int nWidth, int nHeight, int nBits, EGAME_WINDOW_MODE eWindowMode = GAME_WINDOW_MODE_WINDOW, 
				  int nWindowWidth = 0, int nWindowHeight = 0, bool bUseDD = false, xnBmp * pBlankBmp = 0);
void	xnGrpShutDown(void);
void	xnGrpShowScreen(void);
void	xnGrpCleanScrBuf(BYTE R, BYTE G, BYTE B);
void	xnGrpSetScrClip(int x1, int y1, int x2, int y2);
void	xnGrpSetCurrentScreen(xnBmp* pBmp);
void	xnGrpRestoreOrgScreen();

void	xnGrpSetStretch(xnBmp* pBmp);
void	xnGrpBackStretch(xnBmp* pBmp);

// basic graphic function =====================================================
void	xnGrpSetPixel(int x, int y, WORD color, int alpha = 255);
void	xnGrpDrawHline(int x1, int x2, int y, WORD color, int alpha = 255);
void	xnGrpDrawVline(int x, int y1, int y2, WORD color, int alpha = 255);
void	xnGrpDrawLine(int x1, int y1, int x2, int y2, WORD color, int alpha = 255);
void	xnGrpDrawDiamond(int x1, int y1, int x2, int y2, WORD color, int alpha = 255);
void	xnGrpDrawBar(int x, int y, int w, int h, WORD color, int alpha = 255);
void	xnGrpRoundRect(
					int nLeftRect,   // x-coord of upper-left corner of rectangle
					int nTopRect,    // y-coord of upper-left corner of rectangle
					int nRightRect,  // x-coord of lower-right corner of rectangle
					int nBottomRect, // y-coord of lower-right corner of rectangle
					int nWidth,      // width of ellipse
					int nHeight      // height of ellipse   
					);
void	xnGrpSetMixFunction(int nGrpMixFunctionIndex = GRP_MIX_FUNCTION_NORMAL);

// put chat window outside =====================================================
bool	xnGetChatWindowVisible();
void	xnSetChatWindowVisible(bool bDrawOnChatWin);
void	xnAttachSreenBmpToChatWindow();
void	xnAttachSreenBmpToMainWindow();

#ifdef _DEBUG
	#pragma comment(lib,"jpeglib2_d.lib")
#else
	#pragma comment(lib,"jpeglib2.lib")
#endif

#endif
