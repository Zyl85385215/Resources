#ifndef _GAME_TEXT_H_
#define _GAME_TEXT_H_
#include <windows.h>
#include "xnlist.h"
#include "xninput.h"
#define MAX_GTLINE 6   //���ݼ{���ı��Д�
#define MAX_VIRTURLLINE 100 //�����ı��������

class CGameTextView;
class CGameText;
class CGamePhrase;

enum				//�ı����
{
	//���ԏ��CGameString�ĳɆT�Д�
	GS_NORMAL,   //No Have Data    ��ͨ�ַ�		m_pData   == NULL
	GS_LINK,     //HinkDraw Draw Data And Mouse Event In It  朽��ַ�
	GS_GRAPH,    //No Have String  �DƬ			m_szValue == NULL
	GS_NEWLINE,  //m_pData   == NULL			m_szValue == NULL
};

struct  Vernier		//�Θ�:�_��Ԫ�؃�λ��,���ڶ�λԪ�؃�m_szValue���ַ�
{
	WORD	wCharPos;   //�Θ�����Ԫ���е�λ��
	WORD	wItemPos;   //�Θ�����GSԪ�ص�����
};

struct LineInfo		//����Ϣ
{
	//View ����:�Θ�λ��;�߶�;�ԄӓQ���еĿs�M;
	WORD	wHeight;							//�и�,	
	WORD	wLineY;								//������Yλ��(����0) 
	int		iIndent;							//+�ı�ʾԓ��  -�ı�ʾ��܇ǰ��
	Vernier	vernierLineHead;					//���^λ��
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
	BOOL			(*HintDraw)(void* pData,DWORD param);//��pData�M��̎��
	BOOL			(*Draw)();
};

union	DrawFunc
{
	CharFunc*		m_pCharFunc;
	GraphFunc*		m_pGraphFunc;
};

class CGamePhrase//���Z
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
						/*����y����x*/	int offset = 0,
										int BeginPos = 0,int EndPos = -1,
							   /*����*/	xnList* pLineList = NULL   
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
	//Doc  ӛ䛔�ֵ
	char*			m_szValue;
	void*			m_pData;

	BYTE			m_bType;
	CGameText*      m_pText;
	//View ӛ��@��ֵ�ڸ��е��Ă�Xλ��
	LineInfo*		m_pLine;		//�_ʼ������
	int				m_viewX;        //����X���˞�0,�@�������������׵�
	LineInfo*		m_pNextItemLine;		
	int				m_viewNextItemX;		//�Y��λ�õ��к�X����
	int				m_nColor;
	
	DrawFunc		m_DrawFunc;			//Draw Function

	//Event
	xnList*			m_pRectList;//���H���˾���
	BOOL			m_isHint;
	BOOL			m_bDrawBar;

	EventFunc*		m_Event;			//Event Function
	DWORD			m_dwMEventParam;
	DWORD			m_dwHDrawParam;

	POINT           *m_pHintPt;
	POINT			m_ptLast;
	int				m_nUpdateCount;
};

class CGameText//�ı�
{
	friend class CGamePhrase;
	friend class CGameTextView;
public:
	//��ʼ������
	CGameText();
	~CGameText();
	//�ı�����Ƅ�
	void			VernierPrev();
	void			VernierNext();
	int				VernierPrev(Vernier*	vernier);
	int				VernierNext(Vernier*	vernier);
	//Ԫ�ز�������
	void			Insert(CGamePhrase* pPhrase);				//��ͨ����
	void			Insert(int nColor,void* pData,BOOL bIsChangeLine = FALSE);													//�DƬ
	void			Insert(int nColor,void* pData,char* format, ...);									//朽�
	void			Delete();
	void			Clear();
	xnList*			m_pCGamePhraseList;
	int				m_editX;
protected:
	//��݋�ı��ľ�݋�Θ�������ͨ�ַ����΄�,����Ԫ�������^����wCharPos=0
	//�o�B�ı��ľ�݋�Θ�ֻ������β��
	Vernier			m_vernierEdit;							//��ʼ���Θ�
	Vernier			m_vernierUpdate;
	//�@ʾ�����һ���ַ��Θ�,�����һ���Θ�����(��Mҕ�^���ߵ����c)�L��ҕ�^
	Vernier			m_vernierViewS;
	Vernier			m_vernierViewE;
	int				m_LinePos;
	xnList*			m_pLineList;
};

class CGameTextView
{
public:
	//��ʼ������
	CGameTextView();
	~CGameTextView();	
	//�ⲿ�ӿ�
	int				GetLineCount();
	void			SetLinePos(int pos);
	void			InitView(BYTE mode,WORD wWidth,WORD wHeight,CharFunc* pCharFunc,GraphFunc* m_pGraphFunc);										//��ͨ����
	void			Insert(int nColor,void* pData,BOOL bIsChangeLine = FALSE);													//�DƬ
	void			Insert(int nColor,void* pData,char* format, ...);									//朽�
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
	BOOL			Show(BYTE mode,WORD wWidth,WORD wHeight);								//TRUE:�����ꮅ,FALSE����ʧ��(�]�г�ʼ���L�ƺ���)	
	BOOL			UpdateV(POINT ptMouse,xnInputCmd* cmd);									//�M�Д���̎���r�g̎��,̎��ʧ���t���M���L��
	void			DrawV(int x,int y,COLORREF color,int alpha = 255);						//�L��
	void			HintDrawV(int x, int y);
	BOOL			UpdateH(POINT ptMouse,xnInputCmd* cmd);
	void			DrawH(int x,int y,COLORREF color,int alpha = 255);
	//̓�M���ڳ�ʼ��

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