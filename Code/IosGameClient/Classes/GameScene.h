#pragma once

#include "cocos2d.h"
#include "AmzDataBase.h"

//class CMyLabShade;
class CCEffectSprite;
class  GameScene	: public	cocos2d::CCScene
{
public:
	GameScene();
	~GameScene();
	CREATE_FUNC(GameScene);
	virtual	void	onEnter ();
	virtual void	onExit ();
	virtual void update(float dt);

	void	TryUpdate();

	cocos2d::CCSprite*	pSprUp;
	cocos2d::CCSprite*	pSprDown;
	
	cocos2d::CCSprite*	m_pSprLoading;
	cocos2d::CCSprite*	m_pSprBack;
	CCEffectSprite*		m_pSprEffBell;
	cocos2d::CCSprite*	m_pSprLight;
	cocos2d::CCProgressTimer*	m_pProTime;
	cocos2d::CCLabelTTF*	    m_pLoadingTxt;

	bool   m_bChg;
	bool   m_bShowEff;
	void   _CheckLoadingEff(char* szName);
};