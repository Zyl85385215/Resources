#pragma once

#include "cocos2d.h"
#include "MapObjManagers.h"

#include "InterfaceLayer.h"
//#include "TxtLinkView.h"

using namespace cocos2d;
class CCEffectSprite;
//class CMyLabShade;
class GuideLayer : public cocos2d::CCLayer
{
public:
	virtual ~GuideLayer();

	CREATE_FUNC(GuideLayer);
	 
	virtual bool init();  
	virtual void update(float dt);
	void	TryUpdate();

	virtual void registerWithTouchDispatcher();

	virtual bool ccTouchBegan (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	bool	UpdateGuideForm();
	void	ResetViewWindow();
	void	ResetChildren(int nRealPosX,int nRealPosY);
	
private:

	bool		m_bWorked;
	CCRect		m_rcClick;

	CCSprite*	m_szpSprHold[4];
	CCSprite*	m_pSprBack;    
	CCSprite*	m_pSprBack2;    
	CCSprite*	m_pSprMid; 

	CCEffectSprite*	m_pSprEff;
	CCEffectSprite*	m_pSprEff2;

	CCLabelTTF*	m_pMyLabS;
	CCLabelTTF*	m_pMyLabS2;

	int			m_nCurGID;
	int			m_nTestTime;

};

//“°∏ÀΩÁ√Ê
class ControlLayer	: public cocos2d::CCLayer
{
public:
	virtual ~ControlLayer();

	CREATE_FUNC(ControlLayer);

	virtual void onEnter();
	virtual bool init();  
	virtual void update(float dt);

	virtual void registerWithTouchDispatcher();

	virtual bool ccTouchBegan (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);


	cocos2d::CCSprite*	m_pSprCtrlBtn;
	cocos2d::CCSprite*	m_pSprCtrlBack;
	cocos2d::CCPoint	m_ptDefaul;

	cocos2d::CCPoint	m_ptChged;
	int			m_nChgLen;
	bool		m_bClickHold;
};

extern ControlLayer*	pCtrlLayer;