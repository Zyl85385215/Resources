#pragma once

#include "xnlist.h"
#include "GlobalDefine.h"
#include "Interface.h"
#include <map>

class STextObj
{
public:
	STextObj();
	~STextObj();

	int		nPID;
	bool	bChgLine;
	bool	bReset;
	BYTE	byR,byG,byB;
	BYTE	byType;			//0文本, 1图片, 2 可点击
	char*	szContent;
	int		nOffX;
	int		nOffY;

	int		nFontSize;

	void*	pParam;
	void (*funCBLink)(void* pParam);

	int		nViewPosY;

	bool	bTouchEvent;
	//RECT	rcRng;			//所占区域
};

class SpecialTextCenter
{
public:
	SpecialTextCenter();
	~SpecialTextCenter();

	STextObj*	AddSText(int nPID,BYTE bType,bool bChgLine,char* szContent, BYTE R, BYTE G, BYTE B,void* pParam	= NULL,void (*Fun)(void*	pParam)=NULL, int nOffY=0);
	void	RemoveSText(int	nPIDF,int nPIDT);
	void	ClearSText();
	bool	ChgSText(int nPID, char* szContent, BYTE R, BYTE G, BYTE B);
	STextObj*	GetSText(int nPID);
	void	AnalyText(char* szTxt);		//
	void	SetSlide(DComponent* pBack,DComponent* pMove, bool bResetPY = true);

	bool	bDisableTouch;
	bool	bNeedRefresh;

	bool    m_bReSetPosY;

	bool	bAutoBottom;	//自动拉到最下面
	BYTE	m_byFontSize;
	xnList	*	m_pList;		//需要绘制的列表,各元素

	DComponent* m_pSlideBack;
	DComponent* m_pSlideMove;
	bool	m_SameSwitch;
	bool	m_bSetBottom;
	bool	m_bMoveSwitch;
};


class ToopTip9Spr
{
public:
	ToopTip9Spr(int nX, int nY, int nW, int nH,BYTE  bType,char* sz9Spr);
	~ToopTip9Spr();

	int			m_nX;
	int			m_nY;
	int			m_nW;
	int			m_nH;
	SpecialTextCenter*	m_pDataSTC;
	char*		m_sz9Spr;
	BYTE		m_bType;	//0不消失	0x01 点击消失 0x02 时间消失0x01&0x02
	DWORD		m_dwMarkVal;

	bool		m_bRequestDel;
	bool		m_bIsDel;	//弹出的界面对象被关闭后设置TRUE 只能被COCOS操作
	bool		m_bChgPos;
	char*		m_szTip;
};
class CPopMenuCtrl;
class MenuItemData
{
public:
	CPopMenuCtrl*	pParat;
	int				nIndex;
};

class CPopMenuCtrl
{
public:
	CPopMenuCtrl(int nX, int nY, int nW, int nH, void (*cbMenuClick)(void* param,int nMenuIndex),void* param);
	~CPopMenuCtrl();

	void		AddMenuItem(char* szMItem);
	void		PopMenu(BYTE bCloseType);
	void		ClearMItem();

	ToopTip9Spr*	m_pTipData;
	void	*	m_pParam;
	void (*Func_MenuClick)(void* param,int nMenuIndex);
private:
	xnList*			m_pListMItem;
	int				m_nMICount;

};

//有动态增加效果的进度条
class CCtrlProgress
{
public:
	CCtrlProgress(DComponent* pComp);
	~CCtrlProgress();

	void	SetProgress(int	nVal, int	nMax, int nTick = -1);
	void	AddProgress(int	nVal, int  nNextMax, int nTick);
	void	Update();

	DComponent*		pCompPro;
	int		m_nAddSID;
	int		m_nCurVal;
	int		m_nMaxVal;
	int		m_nAddVal;
	int		m_nNextVal;
	DWORD	m_dwSTick;
	DWORD	m_dwRunTick;
	bool	m_bHideTxt;
};

class CCtrlRadio;
class	CRadioParam
{
public:
	CCtrlRadio*	pParent;
	DComponent*	pComp;
	int			nPos;
};

class CCtrlRadio
{
public:
	CCtrlRadio(DComponent* pComp[], int nCnt);
	~CCtrlRadio();

	void	SetSel(int nSel);
	int		GetSel()	{return m_nSelRadio;}

private:
	int			m_nSelRadio;
	CRadioParam**	m_ppRadioParams;
	int			m_nCnt;
};


class CCtrlCheck
{
public:
	CCtrlCheck(DComponent* pComp, BYTE bType);
	~CCtrlCheck();

	void	SetCheck(bool bCheck);
	bool	GetCheck();
	void	SetEnable(bool bOn);
	bool		m_ifEnable;
private:
	
	BYTE		m_bType;
	bool		m_bCheck;
	DComponent*	m_pComp;
};

class CCtrlSlide
{
public:
	CCtrlSlide(DComponent* pCompBack, DComponent* pCompMove);
	~CCtrlSlide();

	void	SetSlide(int nVal);	//0-100
	int		GetSlide();	//0-100

	int		m_nVal;

	DComponent*	m_pCompBack;
	DComponent*	m_pCompMove;
};

class _CLItemTxt
{
public:
	_CLItemTxt()	
	{
		szTxt = NULL;
		R	= 255;
		G	= 255;
		B	= 255;
	}
	~_CLItemTxt();
	BYTE	bType;		//0文本, 1图片
	char*	szTxt;
	BYTE	R,G,B;

};
class _CtrlListItem
{
public:
	_CtrlListItem()	{param	= NULL;}
	~_CtrlListItem();
	std::map<int,_CLItemTxt*>	mapChildren;
	void*	param;

	_CLItemTxt*	GetItemTxt(int nRow);
};

class CCtrlList
{
public:
	CCtrlList(DComponent* pCpLink);
	~CCtrlList();

	void	SetRowCnt(DComponent* ppCP[],int nCnt);
	void	SetTxtCenter(bool bCenter);

	int		InsertItem(char* szTxt, int nPos=-1);
	void	SetItemText(int nPos, int nRow,char* szTxt);
	void	SetItemData(int nPos, void* pData);

	void	UpdateView();
private:

	DComponent*		m_pCompLink;
	xnList*			m_pListItems;
	char*			m_szSelSpr;
	int*			m_pnRow;
	int				m_nRowCnt;
	bool			m_bTxtCenter;
};

class TabMenuObj
{
public:
	virtual		void	Open(int nT)	{m_pForm->Open (nT);}
	virtual		void	Close();
	virtual		bool	CheckCanOpen()	{return true;}	//上层处理条件
	virtual		bool	CheckColor(int nCnt)	{return false;}
	DForm*		m_pForm;
};

class TabMenuCtrl
{
public:
	TabMenuCtrl(TabMenuObj* pObjs[], int nCnt);
	~TabMenuCtrl();

	void	Open(int nType,int nPos	= -1);
	void	Close();
	bool	IsOpened();

	int			m_nCnt;
	TabMenuObj**	m_ppChildren;
	int		m_nCurSel;

	bool	(*funCheckLimint)(int nPos);
private:
	int			m_nOpenType;
};

class _CtrlCombObj
{
public:
	int		nParam;
	char*	szTxt;
};

class CCtrlComb
{
public:
	CCtrlComb(DComponent* pTxt, DComponent* pDown);
	~CCtrlComb();

	void	AddString(char* szStr, int nParam);
	void	ClearString();

	int		GetCurSel()		{return m_nSelPos;}
	int		GetSelParam();
	char*	GetSelTxt();

	void	SetCurSel(int nSel);
	void	ClickDown();

	int		GetCurStringCnt()	{return m_pList->Count;}

	CPopMenuCtrl*	m_pPopMenuCtrl;
private:
	int				m_nSelPos;
	DComponent*		m_pCmpTxt;
	xnList*			m_pList;
};

//绑定成一块的控件
/*-----------------------------------------------------
内部空间采用模板拷贝产生,name跟原控件相同,外部可以注册事件
控件坐标都是pbase的相对坐标,再加上控件的偏移位置
--------------------------------------------------------*/
class CCtrlFrame;
class CCtrlFPart
{
	friend class CCtrlFrame;
public:
	CCtrlFPart(CCtrlFPart* pCF);	//copy已有的控件块
	CCtrlFPart(DComponent* pCB,DComponent** ppComp, int nCnt);
	~CCtrlFPart();

	void	MoveXY(int nX, int nY, int nMTime);
	void	SetXY(int nX,int nY,int nMTime);
	void	SetVisable(bool bVsb);

	//设置某一成员的外部事件,para参数通常直接设置为编号或数据指针

	void	SetEvent (void (*EventFunc)(InterfaceEvent * pEvent,void * param), void * para = NULL);

	bool		bCopyed;
	DComponent*	pCompBase;
	xnList*		pListComps;
	DWORD		m_dwTick;
	int			nID;
	int			nNewSpace;
	CCtrlFrame*	pParent;
	void*		m_pTag;
protected:
	DComponent*		GetDComp(char*	szName);
};


void MoveFrmLeft(InterfaceEvent * pEvent,void * param);
void MoveFrmRight(InterfaceEvent * pEvent,void * param);

enum{
	STEP_EVENT_BEGIN = -1,
	STEP_EVENT_MOVE = -2,
	STEP_EVENT_END = -3,
};

class CCtrlFrame
{
public:
	CCtrlFrame(int nX, int nY, int nW, int nH);
	~CCtrlFrame();

	bool	InitFParts(DComponent* pCB, DComponent** ppComp, int nCompCnt);
	bool	SetFPCnt(int nCnt,bool bHoriz=false,bool bLoop=false);	//修改对象数量
	void	ResetParts();
	int		GetFPCnt()	{return pListFP->Count;}
	bool	InitByForm(DForm* pForm,bool bHoriz=false);	//根据FORM创建

	void	SetEvent(void (*EventFunc)(int nPos,char* szName));	//点了第几项的哪个控件
	void	SetEventEx(void (*EventFunc)(int nPos,char* szName,void* param),void* pParam);
	void	SetEventAdv(void (*EventFunc)(InterfaceEvent* pEvent, int nPos,char* szName,void* param),void* pParam);
	void	SetStepEvent(void (*StepEventFunc)(int nStep));	//停止移动时调用 nStep>=0为停止时第X项,<0其他调用
	void	SetFPCompSpr(int nFPID, char* szName, char* szSpr);
	void	SetFPNewSpace(int nFPID,int nVal);
	void	SetFPCompCaption(int nFPID, char* szName, char* szCaption,BYTE bAlign=1);
	DComponent*		GetFPComp(int nFPID, char* szName);
	//DComponent*		GetFPComp(char* szName);
	DComponent*		GetFPCompBase(int nFPID);
	bool		SetFPTag(int nFPID,void* pTag);
	void*		GetFPTag(int nFPID);

	void (*fnEventFunc)(int nPos,char* szName);
	void (*fnEventFuncEx)(int nPos,char* szName,void* param);
	void (*fnEventFuncAdv)(InterfaceEvent* pEvent, int nPos,char* szName,void* param);
	void*	m_pParam;

	void (*fnStepEventFunc)(int nStep);

	int		nFrmX,nFrmY;
	int		nFrmW,nFrmH;
	int		nFrmMaxW,nFrmMaxH;
	int		nSpace;
	int		nDisStep;		//锁定的格子
	int		nChgX, nChgY;	//上次关闭的偏移值
	int		nSetX, nSetY;	//设置偏移值
	BYTE	byChgFrom;
	bool	bIsHoriz;		//是否水平
	bool	bIsCreateByForm;
	bool	bLimitRect;
	bool	bSplitTouch;
	bool	bDisableTouch;

	CCtrlFPart*	pCtrlFPBase;
	xnList*		pListFP;

	bool	bChged;
	int		nZorder;
	bool	bLoop;

	int		nOffChgPos;

	void*	pFrame;

	int		GetStepPos();
};
class ItemObj;
class CCtrlItem{
public:
	CCtrlItem(DComponent* pCom);
	~CCtrlItem();
	void		SetItem(ItemObj* pItem);
	DComponent* m_pCom;
	ItemObj*	m_pItem;
};