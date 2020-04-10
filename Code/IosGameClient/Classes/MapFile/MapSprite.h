#pragma once

#include "cocos2d.h"
#include "MapControl.h"

class MapLayer;
//class CMyLabShade;
class CCEffectSprite;
class TipFormLayer;
class CCMObjSprite	: public cocos2d::CCSprite
{
public:
	static CCMObjSprite* create(MapSprObj* pMObj);
	CCMObjSprite();
	virtual	~CCMObjSprite();

	void	AddSprEff(CCEffectSprite* pEffSpr, int nZorder);
	void	DelSprEff(CCEffectSprite* pEffSpr);
	bool	CheckUpdate();

	MapSprObj*	m_pMSprObj;

	bool	NoNeedShadow();
	void	SetAlphaVal(int nAlpha);
	void	ClearShadow();
	void	AddShadow(cocos2d::CCSprite*	pSrcSpr,cocos2d::CCSpriteFrame* pSprFrm);

private:
	void	_checkName();
	void	_checkMyInfo();
	void	_checkHpBar();
	void	_checkMark();
	cocos2d::CCSpriteFrame*	_checkBody();
	cocos2d::CCSpriteFrame*	_checkWeopon();
	cocos2d::CCSpriteFrame*	_checkRide();
	void	_checkEff();
	void	_checkHitNum();

	void	_CheckToopTip();

	cocos2d::CCProgressTimer* m_pHpBar;
	cocos2d::CCSprite*		m_pHpBack;
	cocos2d::CCLabelTTF*	m_pLbName;
	cocos2d::CCLabelTTF*	m_pLbTitle;
	cocos2d::CCLabelTTF*	m_pLbMTitle;

	cocos2d::CCSprite*	m_pSprBody;
	cocos2d::CCSprite*	m_pSprWeopon;
	cocos2d::CCSprite*	m_pSprRide;
	cocos2d::CCSprite*	m_pSprRideEx;

	cocos2d::CCSprite*	m_pSprShadow;

	cocos2d::CCSprite*	m_pSprCountry;
	cocos2d::CCSprite*	m_pSprMark;
	cocos2d::CCSprite*	m_pSprVip;
	cocos2d::CCSprite*	m_pSprFstHome;
	//cocos2d::CCSprite*	m_pSprSPEff;


	TipFormLayer*		m_pTipForm;

	xnList*				m_pLEffSpr;
	xnList*				m_pLNumSpr;

	ObjSkinInfo*		m_pCacheSkin;
	ObjSkinInfo*		m_pCacheSkinA;
	ObjSkinInfo*		m_pCacheSkinB;
	xnList*				m_pLSprShadow;		//²ÐÓ°Ð§¹û

	void			_clearSkinPList(ObjSkinInfo* pSkin,BYTE bType);
};