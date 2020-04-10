#pragma once


#include "cocos2d.h"
#include "xnlist.h"
#include "GlobalDefine.h"

class MyEventRect
{
public:
	~MyEventRect();
	cocos2d::CCRect	rect;
	void	(*fnEventCB)(int nEvent,int x, int y, void*	pParam);
	void*	pParam;
	int		nOrder;
};

//用于检测点击焦点
class MyDragLayer	: public cocos2d::CCLayer
{
public:
	MyDragLayer();
	virtual ~MyDragLayer();

	virtual void registerWithTouchDispatcher();

	// CCLayer
	virtual bool ccTouchBegan (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual int		GetNodTagFrame()	{return -1;}
	void	SetRealRect(cocos2d::CCRect rc);
	cocos2d::CCRect&	GetRealRect()	{return m_rectReal;}
	void	SetLimitRect(cocos2d::CCRect rc);

	void	AddRegEvent(cocos2d::CCRect rc,void* pParam,void (*Fun)(int nEvent,int x, int y, void*	pParam),int nOrder = 0);
	bool	ChgRegERect(void* pParam, cocos2d::CCRect rc);
	void	ClearRegERect();

	virtual	void onExit();

	virtual void MovePosition(cocos2d::CCPoint pt);
	int					m_bZorde;
protected:
	cocos2d::CCRect		m_rectLimit;
	cocos2d::CCRect		m_rectReal;
	bool				m_bClicked;
	bool				m_bEnableDrag;
	bool				m_bEnableClick;
	bool				m_bLockThrough;
	xnList*				m_pListBtnRect;
	MyEventRect*		m_pCurERect;
	


};

extern	bool	g_bIFLayerClicked;		//限制界面层的多点触控