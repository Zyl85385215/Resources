#pragma once

#include "cocos2d.h"
#include "Interface.h"
#include "MyTextInput.h"
#include "TxtLinkView.h"
#ifndef WIN32
#include "cocos-ext.h"
#endif
#include "singleton.h"
using namespace cocos2d;

class InterfaceLayer;

//class CMyLabShade	: public cocos2d::CCLabelTTF
//{
//public:
//	static CMyLabShade * create(const char *string, const char *fontName, float fontSize);
//	virtual ~CMyLabShade();
//	cocos2d::CCLabelTTF* m_pLbBack;
//
//	void setHorizontalAlignment(cocos2d::CCTextAlignment alignment);
//	void setVerticalAlignment(cocos2d::CCVerticalTextAlignment verticalAlignment);
//	void setDimensions(const cocos2d::CCSize &dim);
//	//void setAnchorPoint (cocos2d::CCPoint & pt);
//	virtual void setString(const char *label);
//	virtual	void setOpacity(GLubyte opacity);
//};

class CObjProgress	: public cocos2d::CCProgressTimer
{
public:
	CObjProgress(DComponent* pComp);
	virtual ~CObjProgress();

	CCLabelTTF* m_pLbInfo;
	cocos2d::CCSprite*	m_pLightSpr;
	DComponent*		m_pComp;
};

class CSpecialText	: public MyDragLayer
{
public:
	CSpecialText(SpecialTextCenter* pSTC,bool bTClose = false);
	virtual	~CSpecialText();

	virtual bool ccTouchBegan (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void visit(void);

	SpecialTextCenter*	m_pSTCenter;

	void	ResetChildren();
	void	ResetPose();
	void	UpdateChildChg();

	bool	bGetTouchCtrl;
	bool	bTouchClose;
	int		nStartY;

	int		nMaxHeight;
	bool	bLimitView;
	int		nDefaulY;

private:
	bool	bInTop;
	bool	bInBottom;
};

class CObjSSprite	: public cocos2d::CCSprite
{
public:
	CObjSSprite(DComponent* pComp);
	virtual ~CObjSSprite();

	virtual void setPosition(const CCPoint& pos);

	virtual	void setOpacity(GLubyte opacity);
	DComponent*		m_pComp;
	CCLabelTTF*		pLabNum;
};

class CObjButton	: public CObjSSprite
{
public:
	CObjButton(DComponent* pComp);		//事件自己处理吧/*, CCObject* target, SEL_MenuHandler selector*/
	virtual ~CObjButton();

	void Update ();
	static	void EventCallBack(int nEvent,int x, int y, void* pParam);
	virtual void visit(void);

	cocos2d::CCTexture2D*	pSelectText;
	int				m_dwTmClicked;
	int				m_dwLastClicked;

	cocos2d::CCSprite*		pSprPrev;
	cocos2d::CCSprite*		pSprNext;
	cocos2d::CCSprite*		pSprCur;
	cocos2d::CCRect			rectTouchM;

	cocos2d::CCProgressTimer*	pProCD;
	xnList*						pListEffs;
	xnList*						pListNums;
};

class CSpBtnSprite		: public CObjSSprite
{
public:
	CSpBtnSprite(DComponent* pComp);
	virtual ~CSpBtnSprite();

	virtual void visit(void);
	static	void EventCallBack(int nEvent,int x, int y, void* pParam);
	cocos2d::CCRect			rectView;
};

#ifndef WIN32
class CNewTextInput	: public cocos2d::CCLayer, public cocos2d::extension::CCEditBoxDelegate
{
public:
	CNewTextInput(DComponent * pComp);
	virtual ~CNewTextInput();

	DComponent*		m_pComp;
	cocos2d::extension::CCEditBox*	pEditBox;

	virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox)  {}
	virtual void editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text);
};
#endif

class CObjTextInput : public MyTextInput
{
public:
	CObjTextInput( DComponent* pComp);
	virtual ~CObjTextInput();

	DComponent*		m_pComp;
};

class CObjCaption	: public cocos2d::CCLabelTTF
{
public:
	CObjCaption( DComponent* pComp);
	virtual ~CObjCaption();
	DComponent*		m_pComp;
};
class CBMFCaption	:public cocos2d::CCLabelBMFont
{
public:
	CBMFCaption( DComponent* pComp);
	virtual ~CBMFCaption();
	DComponent*		m_pComp;
};
class CObjCartoon	: public cocos2d::CCSprite
{
public:
	CObjCartoon(DComponent* pComp);
	virtual ~CObjCartoon();

	virtual void update (float dt);

	DComponent	*	m_pComp;
	cocos2d::CCAction*	m_pAction;
};
class FrameLayer : public MyDragLayer, public cocos2d::CCRGBAProtocol
{
	/** conforms to CCRGBAProtocol protocol */
	CC_PROPERTY(GLubyte, m_cOpacity, Opacity)
		/** conforms to CCRGBAProtocol protocol */
	CC_PROPERTY_PASS_BY_REF(ccColor3B, m_tColor, Color)
		/** conforms to CCRGBAProtocol protocol */
	bool m_bIsOpacityModifyRGB;
	bool isOpacityModifyRGB();
	void setOpacityModifyRGB(bool isOpacityModifyRGB);
public:
	FrameLayer();
	virtual ~FrameLayer();

	cocos2d::CCPoint	PtIF2Cocos(int nX, int nY);
	CCNode*		AddDComp(DComponent* pComp);
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void visit(void);

	void	Update();

	void		SetStopPositon(cocos2d::CCPoint& pt, bool bNM,bool bSound= true,float fDuration=0.5);
	void		(*fnStepEventFunc)(int nStep);
	void		ActionCallBack(cocos2d::CCNode *pNode,void* pParam);

	virtual void setPosition(const CCPoint& pos);
	virtual void setPosition(float x, float y);


	bool				m_bLockX;
	bool				m_bLockY;
	bool				m_bLimitRect;
	int					m_nSelect;

	int					m_nDifStep;
	DWORD				dwDownTick;
	cocos2d::CCPoint	ptDown;

	bool				m_bLoop;
	bool				m_bMove;
	bool				m_bSplitTouch;
	bool				m_bDisalbeTouch;
};
class LoopLayer : public FrameLayer
{
public:
	LoopLayer();

	CCAction*	m_pOutAction;
	int			m_nSelect;
	int			m_nSize;

	void		SetSize(int nSize);
	virtual void setPosition(const CCPoint& pos);

	std::list<CCSprite*>	m_listSpr;
};
class FormLayer	: public FrameLayer
{
public:
	FormLayer(DForm* pFD);
	~FormLayer();

	//CCNode*		AddDComp(DComponent* pComp);
	
	bool		Update();		//返回false就删除
	void		InitFrame();
	void		_DCompCheck(CCNode* pFL);

	void MovePosition(cocos2d::CCPoint pt);

	virtual bool ccTouchBegan (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void		RunOpenEffect(cocos2d::CCPoint pt);
	void		RunCloseEffect();

	void		ActionCallBack(cocos2d::CCNode *pNode,void* pParam);
	virtual int		GetNodTagFrame();

	virtual void visit(void);

	DForm*		m_pFData;
	bool		m_bCloseAct;
	bool		m_bEventToFrame;
private:
	
	void		_runCompAction(CCNode* pNode);
	//xnList*		m_pListObjs;

	BYTE		byEffect;
	BYTE		byStyle;
};

class TipFormLayer	: public cocos2d::CCNode
{
public:
	TipFormLayer(ToopTip9Spr* pTipData);
	~TipFormLayer();

	bool		CheckUpdate();


	CSpecialText*	m_pSpecialText;
	ToopTip9Spr*	m_pTipData;
private:
	void		_make9Sprite2Node( int nW, int nH);
	void		_reset9SpritePos( int nW, int nH);
	
};

class CCEffectSprite;
class InterfaceLayer : public cocos2d::CCLayer
{
public:
	virtual ~InterfaceLayer();

	CREATE_FUNC(InterfaceLayer);

	virtual bool init();  

	virtual void onEnter();	//场景切换先A->init B->onExit A->onEnter  用了特殊效果后事A->init A->onEnter  B->onExit
	virtual void onExit();

	virtual void update(float dt);

	cocos2d::CCPoint	PtIF2Cocos(int nX, int nY);
	bool	PtCocos2IF(cocos2d::CCPoint ptIn, int& nX, int& nY);

	cocos2d::CCSprite*			m_ppGetTip[4];
	xnList*			m_pDFChild;
	xnList*			m_pTipChild;

	FormLayer*		GetFormLayer(DForm* pForm);
private:
	void		_checkForm();
	void		_checkToopTip();
	void		_checkGetTip();
};

extern	InterfaceLayer*		g_pLayerIF;
cocos2d::CCSprite*	GFun_InitSpriteByJPG(char* szFile);


//class CCGLProgramManage:public CSingleton<CCGLProgramManage>{
//public:
//	CCGLProgramManage();
//	~CCGLProgramManage();
//	void	init();
//
//	CCGLProgram*	pEnable;
//	CCGLProgram*	pNull;
//
//};