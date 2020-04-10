#ifndef _GAME_TEXT_H_
#define _GAME_TEXT_H_
#include <windows.h>
#include "xnlist.h"
#include "xninput.h"
#define MAX_GTLINE 6   //可容納的文本行數
#define MAX_VIRTURLLINE 100 //整個文本的最大行

class CGameTextView;
class CGameText;
class CGamePhrase;

enum				//文本類型
{
	//可以從類CGameString的成員判斷
	GS_NORMAL,   //No Have Data    普通字符		m_pData   == NULL
	GS_LINK,     //HinkDraw Draw Data And Mouse Event In It  鏈接字符
	GS_GRAPH,    //No Have String  圖片			m_szValue == NULL
	GS_NEWLINE,  //m_pData   == NULL			m_szValue == NULL
};

struct  Vernier		//游標:確定元素內位置,用于定位元素內m_szValue的字符
{
	WORD	wCharPos;   //游標所在元素中的位置
	WORD	wItemPos;   //游標所在GS元素的索引
};

struct LineInfo		//行信息
{
	//View 屬性:游標位置;高度;自動換新行的縮進;
	WORD	wHeight;							//行高,	
	WORD	wLineY;								//行所在Y位置(相對0) 
	int		iIndent;							//+的表示該行  -的表示回車前行
	Vernier	vernierLineHead;					//行頭位置
};

enum
{
	GTV_MODE_NULL,
	//gametextview mode
	GTV_MODE_VM,
	GTV_MODE_HS,
};

struct CharFunc
{
	BOOL			(*DrawChar)(WORD wCharCode, int x, int y, COLORREF colr,int alpha);
	int				(*GetCharWidth)(WORD wCharCode);
	int				(*GetCharHeight)(WORD wCharCode);
};

struct GraphFunc
{
	BOOL			(*DrawGraph)(DWORD pData, int x, int y, COLORREF colr,int alpha);
	int				(*GetGraphWidth)(DWORD pData);
	int				(*GetGraphHeight)(DWORD pData);
};

struct EventFunc
{
	BOOL			(*MouseEvent)(xnInputCmd* cmd,void* pData,DWORD param); 
	BOOL			(*HintDraw)(void* pData,DWORD param);//對pData進行處理
	BOOL			(*Draw)();
};

union	DrawFunc
{
	CharFunc*		m_pCharFunc;
	GraphFunc*		m_pGraphFunc;
};

class CGamePhrase//短語
{//
	friend class CGameText;
	friend class CGameTextView;
public:
	CGamePhrase(CGameText* pText);
	CGamePhrase(CGameText* pText,char* pszName,void* pData = NULL);
	~CGamePhrase();
	//doc method	
	void			InitItem(int nColor,char* pszName,void* pData = NULL);
	char*			GetValue();
	void*			GetData();
	//view method
	void			SetView(LineInfo* pLine,int iX);
	void			SetViewEnd(LineInfo* pLine,int iX);	
	BOOL			DrawPhrase(			int x, int y, int w,
										COLORREF colr,int alpha,
						/*多行y單行x*/	int offset = 0,
										int BeginPos = 0,int EndPos = -1,
							   /*多行*/	xnList* pLineList = NULL   
										);
	void			DrawPhraseBar(int x, int y, int offset);
	void			DrawPhraseHint(void);
	BOOL			SetDrawChar(CharFunc * pCharFunc);
	BOOL			SetDrawGraph(GraphFunc * pGraphFunc);
	//event
	void			SetEvent(EventFunc* Event,DWORD paramMouseEvent,DWORD paramHintDraw);
	BOOL			Update(POINT ptMouse,xnInputCmd* cmd);

	void			ResetRect(int nSetPosy, int nOffsetHeight);
private:
	//Doc  記錄數值
	char*			m_szValue;
	void*			m_pData;

	BYTE			m_bType;
	CGameText*      m_pText;
	//View 記錄這個值在改行的哪個X位置
	LineInfo*		m_pLine;		//開始所在行
	int				m_viewX;        //行首X坐標為0,這個是相對于行首的
	LineInfo*		m_pNextItemLine;		
	int				m_viewNextItemX;		//結束位置的行和X坐標
	int				m_nColor;
	
	DrawFunc		m_DrawFunc;			//Draw Function

	//Event
	xnList*			m_pRectList;//實際坐標矩形
	BOOL			m_isHint;
	BOOL			m_bDrawBar;

	EventFunc*		m_Event;			//Event Function
	DWORD			m_dwMEventParam;
	DWORD			m_dwHDrawParam;

	POINT           *m_pHintPt;
	POINT			m_ptLast;
	int				m_nUpdateCount;
};

class CGameText//文本
{
	friend class CGamePhrase;
	friend class CGameTextView;
public:
	//初始化對象
	CGameText();
	~CGameText();
	//文本光標移動
	void			VernierPrev();
	void			VernierNext();
	int				VernierPrev(Vernier*	vernier);
	int				VernierNext(Vernier*	vernier);
	//元素操作函數
	void			Insert(CGamePhrase* pPhrase);				//普通文字
	void			Insert(int nColor,void* pData,BOOL bIsChangeLine = FALSE);													//圖片
	void			Insert(int nColor,void* pData,char* format, ...);									//鏈接
	void			Delete();
	void			Clear();
	xnList*			m_pCGamePhraseList;
	int				m_editX;
protected:
	//編輯文本的編輯游標在于普通字符內游動,其它元素游在頭部即wCharPos=0
	//靜態文本的編輯游標只在文章尾部
	Vernier			m_vernierEdit;							//初始化游標
	Vernier			m_vernierUpdate;
	//顯示的最后一個字符游標,從最后一個游標推至(填滿視區或者到起點)繪制視區
	Vernier			m_vernierViewS;
	Vernier			m_vernierViewE;
	int				m_LinePos;
	xnList*			m_pLineList;
};

class CGameTextView
{
public:
	//初始化對象
	CGameTextView();
	~CGameTextView();	
	//外部接口
	int				GetLineCount();
	void			SetLinePos(int pos);
	void			InitView(BYTE mode,WORD wWidth,WORD wHeight,CharFunc* pCharFunc,GraphFunc* m_pGraphFunc);										//普通文字
	void			Insert(int nColor,void* pData,BOOL bIsChangeLine = FALSE);													//圖片
	void			Insert(int nColor,void* pData,char* format, ...);									//鏈接
	BOOL			Update(POINT ptMouse,xnInputCmd* cmd);
	void			Draw(int x,int y,COLORREF color,int alpha = 255);
	void			HintDraw(int x, int y);
	void			InitVirtualWindow();

	void			Clear();
	CGameText*		m_pGameText;
	//SetEventFunc
	void			SetLinkEvent(EventFunc* Event,DWORD paramM,DWORD paramH);
	void			SetNormalEvent(EventFunc* Event,DWORD paramM,DWORD paramH);
	void			SetGraphEvent(EventFunc* Event,DWORD paramM,DWORD paramH);
	void			SetMaxViewLine(int maxViewLine);
protected:
	BOOL			SetDrawChar(CharFunc*		pCharFunc);	
	BOOL			SetDrawGraph(GraphFunc*		pGraphFunc);
	BOOL			Show(BYTE mode,WORD wWidth,WORD wHeight);								//TRUE:建立完畢,FALSE建立失敗(沒有初始化繪制函數)	
	BOOL			UpdateV(POINT ptMouse,xnInputCmd* cmd);									//進行數據處理及時間處理,處理失敗則不進行繪制
	void			DrawV(int x,int y,COLORREF color,int alpha = 255);						//繪制
	void			HintDrawV(int x, int y);
	BOOL			UpdateH(POINT ptMouse,xnInputCmd* cmd);
	void			DrawH(int x,int y,COLORREF color,int alpha = 255);
	//虛擬窗口初始化

	void			InitVirtualWindowV();
	void			InitVirtualWindowH();
protected:
	void			_ResetLineRect(LineInfo* pInfo,LineInfo* pNextInfo);
	int				m_viewW;
	int				m_viewH;
	BYTE			m_bMode;
	BOOL			m_bInitVirWin;
	BOOL			m_bCanView;
	int				m_viewX;
	int				m_viewY;
	int				m_viewXS;
	int				m_viewYS;
	//Viewfunc
	CharFunc*		m_pCharFunc;
	GraphFunc*		m_pGraphFunc;
	//event
	EventFunc*		m_LinkEvent;
	DWORD			m_LinkParam;
	DWORD			m_LinkHParam;
	EventFunc*		m_NormalEvent;
	DWORD			m_NormalParam;
	DWORD			m_NormalHParam;
	EventFunc*		m_GraphEvent;
	DWORD			m_GraphParam;
	DWORD			m_GraphHParam;
	int				m_maxViewLine;
};
#endif