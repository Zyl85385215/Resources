#pragma once
#include "xnlist.h"
#include "GlobalDefine.h"


#define MAX_SKILL_ICON 35


struct SKILLITEM;

enum
{
	COMPONENT_EDIT,		//编辑框
	COMPONENT_BUTTON,	//图片  图片_1.png
	COMPONENT_SSPR,		//静态图片
	COMPONENT_PROGRESS, //一张图片
	COMPONENT_DSPR,		//动态图片, 根据后缀 1- N
	COMPONENT_SLAB,		//静态文本
	COMPONENT_CARTOON,

	COMPONENT_LINK,		//带解析器的
	COMPONENT_MAPGUIDE,
};
enum SHADER_TYPE{
	SHADER_NULL,
	SHADER_GRAY,
};
enum	INTERFACE_EVENT
{
	INTERFACE_EVENT_CLICK,
	INTERFACE_EVENT_DOWN,
	INTERFACE_EVENT_UP,
	INTERFACE_EVENT_MOVE,
	INTERFACE_EVENT_DOUBLECLICK,
	INTERFACE_EVENT_DROP,

	INTERFACE_EVENT_TOUCHMOVE_PREV,
	INTERFACE_EVENT_TOUCHMOVE_NEXT,

	INTERFACE_EVENT_HANDSIGN	= 100,			//101→// 1→  2←  3↑  4↓ 5／ 6\	7 5反,8	6反
};
enum FORM_ZORDER{
	FORM_ZORDER_NORMAL = 0,
	FORM_ZORDER_WARCOPY,
	FORM_ZORDER_WARCOPYMAP,
	FORM_ZORDER_KUANG1, 
	FORM_ZORDER_ROLE,
	FORM_ZORDER_KUANG2,
	FORM_ZORDER_MAZE,
	FORM_ZORDER_XUANREN,
	FORM_ZORDER_CHAT,
	FORM_ZORDER_TALKUI,
	FORM_ZORDER_POPUP,
	FORM_ZORDER_QUEST,
	FORM_ZORDER_REVIVE,
	FORM_ZORDER_CAST,
	FORM_ZORDER_CHAT_TIP,

	FORM_ZORDER_ITEMSHOW=22,		//高于编辑框

	FORM_ZORDER_NET_WAIT	= 999,
};

class DForm;
class DComponent;
class ToopTip9Spr;
class SpecialTextCenter;
class CCtrlFrame;
struct InterfaceEvent
{
	int x, y;
	BYTE event;
	DComponent * pComponent;
};

enum
{
	INPUT_TYPE_NULL,
	INPUT_TYPE_ALL,
	INPUT_TYPE_NUM,

	INPUT_TYPE_PSWD,
};

//界面更新还是采用显示部分检测这边数据的做法
//界面更新还是采用显示部分检测这边数据的做法
class _CDataAddSprTag
{
public:
	int		nTag;
	int		nPosX;
	int		nPosY;
	int		nZorder;
	int		nScalc;
	int		nRota;
	char*	szSpr;
};

class DCEffExInfo
{
public:
	int		nSX, nSY, nEX, nEY;
	int		nTime;	//ms
	WORD	wAngle;
	//WORD	wAngleEnd;	//动作旋转
};

class DCompEffect
{
public:
	DCompEffect()	{pOthers	= NULL;}
	~DCompEffect()	{if(pOthers) delete pOthers;}
	int		nEffId;
	int		nPos;
	DCEffExInfo*	pOthers;
};

class DCompNumEff
{
public:
	int		nNum;
	BYTE	bType;
	BYTE	bVAct;
};
class CLuaState;

class DComponent
{
public:
	DComponent(char* szN);
	~DComponent();


	bool	bChgPos;	//位置,可见,可用性改变
	BYTE	byChgScaleY;	//图片改变
	int		nCPTime;

	BYTE	byType;

	BYTE	byInputType;

	BYTE	byRunAction;

	void	SetVisible(bool bVsb);

	void	*	pCbParam;
	void	*	pData;
	void	*	pParam;
	//各类特殊事件指针之后要做特殊类继承DComponent
	//代码优化: 事件回调可以优化成消息
	//按钮用事件
	void	SetEvent(void (*EventFunc)(InterfaceEvent * pEvent,void * param), void * para = NULL);
	void	(*eventFunc)(InterfaceEvent * pEvent,void * param);

	void	SetActionCallback(void (*EventFunc)(void * param,void* pActionParam), void * para = NULL);
	void	(*eventActionCallback)(void * param,void* pActionParam);


	//编辑框,文本框用事件

	void	SetCaption(const char* szTxt,int nAlignment);
	void	SetCaptionEx(const char* szFormat,int nAlignment,...);
	bool	SetPos(int nPosX,int nPosY, int nMillSecond=0);
	void	SetPosX(int nPosX, int nMillSecond=0);

	void	SetAlignmentX(int nPosX);
	void	SetAlignmentY(int nPosY);

	void	SetEdit(char* szTxt);
	char*	GetEdit();
	void	(*fnSetEditTxt)(char* szTxt,int nAlignment,void* pParam);
	void	(*fnSetShader)(SHADER_TYPE byType,void* pParam);
	const char*	(*fnGetEditTxt)(void* pParam);

	//进度条事件
	void	SetProgress(int nVal, int nMax,char* szTxt="");
	void	(*fnProgressSetInfo)(int nVal, int nMax,	char* szTxt,void* pParam);
	int			nProVal;
	int			nProMax;

	//设置图标
	void	SetViewSpr(const char* szFile);
	void	(*fnSetViewSpr)(char* szFile, void* pParam);
	void	SetSprColor(int nR,int nG,int nB);
	void	(*fnSetSprColor)(int nR,int nG,int nB, void* pParam);
	void	SetViewSprByScale(char* szFile,float scalex,float scaley);
	void	SetScale(float scalex,float scaley);
	void	(*fnSetScale)(void* pParam,float scalex,float scaley);
	void	(*fnSetViewSprByScale)(char* szFile, void* pParam,float scalex,float scaley);
	void	SetSprRotation(int nRotation);
	void	(*fnSetSprRotation)(int nRotation, void* pParam);
	void	SetTouchMoveSpr(char* szFilePrev,char* szFileNext);
	void	(*fnSetTouchMoveSpr)(char* szFilePrev,char* szFileNext,void* pParam);
	void	SetOpacity(BYTE byType);
	void	(*fnSetOpacity)(void* pParam,BYTE byType);

	//设置按钮
	void	SetSelectSpr(char* szFile,bool bTop=false);
	void	(*fnSetSelectSpr)(char* szFile,bool bTop, void* pParam);

	void	SetAddSprByTag(char* szFile,int nPosX, int nPosY,int nTag, int nScale = 100, int nRota = 0,int nZorder = 10);
	void	(*fnAddSprByTag)(char* szFile,int nPosX, int nPosY,int nTag, void* pParam,int nScale, int nRota,int nZorder);
	void	(*fnChgSprByTag)(char* szFile,int nPosX, int nPosY,int nTag, void* pParam,int nScale, int nRota,int nZorder);

	xnList*		m_pListASprTag;

	DCompEffect*	SetEffect(int nVal, int nPos);
	void			ClearEffect();
	bool	bEffChg;
	xnList*	pListEff;

	void	AddNumEffect(int nNum, BYTE bType, BYTE bVAct);
	xnList* pListEffNum;
	//WORD	wEffect[3];

	//动画控件
	void	SetCartoonAction(char	* szPlist, char* szFilePre,int nNeedTime, int nScale=100,bool ifFlip = false);
	void	(*fnSetCartoon)(char	* szPlist,char* szFile,int nNeedTime, void* pParam,int nScale,bool ifFlip);
	char*	szCAPlist;
	char*	szCAFile;
	int		nCATime;
	int		nCTScale;

	void	SetEnable(bool bEnable);
	void	SetLock(bool	bVal);

	int		GetAlignmentX();
	int		GetAlignmentY();
	//------------------------
	char*		szName;
	char*		szSpr;
	char*		szCaption;
	char*		szEdit;
	char*		szSelectSpr;
	bool		bSelSprTop;
	BYTE		bAlignX;		//0左 1中  2右
	BYTE		bAlignY;		//0 上  1中 2下
	bool		bSound;
	int			nR,nG,nB;
	int			nCaptionAlignment;

	DForm	*	pParent;

	bool		bAutoChgLine;
	int			nObjPixWidth;
	int			nSprPixWidth;
	int			nSprPixHeight;
	int			nMaxChar;

	int			nX;
	int			nY;
	int			nW,nH;
	bool		bVisable;

	bool		bEnable;
	bool		bLocked;
	bool		bClickLocked;

	int			m_nFontSize;

	int			m_nOrder;

	SpecialTextCenter*	pSTextCenter;
	void*		pSpecialDataEx;

	void		ActionCallBack(void* pParam);
	DComponent*		BeCopy();
	void*		m_pActionParam;
	bool		m_bDraged;

	void*		m_pNodeBatch;

	float		m_fScaleX;
	float		m_fScaleY;
	int			m_nMark;

	void*		m_pTag;
	void*		GetTag(){return m_pTag;};
	void		SetTag(void* pTag){m_pTag = pTag;};

	bool		m_bChgPosbySpr;
	bool		m_bCocosVsb;

	char*	GetName(){return szName;}
	int		GetPosX(){return nX;}
	int		GetPosY(){return nY;}

};

class DInterface;
class DForm
{
	friend class DInterface;
public:
	DForm(char* szN);
	~DForm();

	void	Open(BYTE bEffet	=1);
	void	Close();
	DComponent*	GetComponent(char* szNID);
	char*	GetName(){return szName;}
	int		GetPosX(){return nPosX;}
	int		GetPosY(){return nPosY;}

	void	SetPos(int nX, int nY, int nMillSecond=0);

	char*	szName;

	bool	bChgPos;	//位置,可见,可用性改变
	int		nCPTime;
	bool	bChgSpr;	//图片改变

	bool		bOpened;	//可采用缩放飞出的样式
	BYTE		bOpenStyle;
	char*		szSpr;

	int			nPosX;
	int			nPosY;
	bool		bLocked;
	bool		bUnTrhough;
	xnList*		pListComps;
	BYTE		bZorde;
	bool		bCocosVisabel;

	//界面内拖动部分
	CCtrlFrame*	pCFrame;

	//xnList*		pListFrame;
	//int			nFrmX,nFrmY,nFrmW,nFrmH;
	//bool		bFrmLoop;

	//bool		bIsFrame;
	int			nW;
	int			nH;

	void*		pFormLayer;
protected:
	int			nCX;
	int			nCY;
	int			nX;
	int			nY;
//

	//FormLayer*	pCSObj;		//后台数据的前端映射,对象内部用0,1的 AC点
};
extern	DInterface*	pMainInterface;
class DInterface
{
public:
	DInterface(char* szFile);	//后台还是960*640. 前端用480*320
	~DInterface();
	static	DInterface*	GetDefault(){return pMainInterface;}

	DForm	*	GetDForm(char* szNID);
	DComponent*	GetComponent(char* szNF, char* szNC);
	void 		Update();
	void		RequestOpen(DForm* pF);
	void		CloseAllForm();
	void		CloseAllFormBut(int* pZorder,int nCount);


	DForm	*	m_pLockForm;
	bool		m_bLockBtnEff;
	xnList	*	m_pListRequest;		//请求打开的界面,界面层访问用
private:
	xnList	*	m_pListForms;
};


class GetTipInfo
{
public:
	char*	szName;
	char*	szTxt;
	int		nSpr;
	int		nNum;
	BYTE	bType;
};


class ChatInfo;
class DToopTipCenter
{
public:
	DToopTipCenter();
	~DToopTipCenter();

	void			Update ();

	void		ShowTalkTip(ToopTip9Spr* pTTip,char* szTip);
	void		ShowTalkTip(ToopTip9Spr* pTTip,ChatInfo* pMsg);
	void		UpdateFace();
	void		ClearFace(ToopTip9Spr* pTTip);
	void		ShowInterfaceTip(char* szTip);
	void		CloseAllInterfaceTip();

	void		ShowPopMenus(ToopTip9Spr* pTipD);

	void		ShowMsgTipOnComp(char * szMsg, DComponent* pComp);
	void		ShowCornerSysMsg(char * szMsg);

	void		AddGetTipObj(char*	szName, char* szTxt, int nSpr, int nNum, BYTE bType);

	xnList*		m_pListRequest;   //等待弹出的

	ToopTip9Spr*	m_pTipMsgCJ;
	ToopTip9Spr*	m_pTipCornerMsg;
	ToopTip9Spr* m_pWaitingSpr;
	ToopTip9Spr* m_pWaitingSpr2;
	DComponent* m_pShowComp;
	ToopTip9Spr*	m_pTipPopMenu;

	DForm*		m_szpForms[5];
	int			m_nGFOpened;

	DForm*		m_pFormGetTip;
	xnList*		m_pListGTips;
	bool		m_bGTipMarkChg;
private:
	xnList*		m_pListToopTips;
	xnList*		m_pListTipMsg;
	xnList*		m_pListFace;	//人物气泡上的表情
};
extern	DToopTipCenter*	pMainToopTip;
extern	xnList*		g_pListFPartCache;