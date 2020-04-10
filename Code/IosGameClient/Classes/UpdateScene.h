#pragma once

#include "cocos2d.h"
#include "GlobalDefine.h"
#include "xnlist.h"
#include "Interface.h"
#include "InterfaceLayer.h"

class PreLoginScene	: public cocos2d::CCScene
{
public:
	PreLoginScene();
	~PreLoginScene();

	CREATE_FUNC(PreLoginScene);
	virtual	void	onEnter ();
	virtual void	onExit ();
	virtual void	update (float dt);

	BYTE		m_bUnZipOk;
private:
	void		_initBackSprite();
	void		_updateBackView();

	cocos2d::CCSprite*	m_pSpriteBack[2];
	cocos2d::CCSprite*	m_pSpriteCloud[2];

	cocos2d::CCSprite*	m_pSprBack;
	cocos2d::CCSprite*	m_pSprLight;
	cocos2d::CCProgressTimer*	m_pProTime;
	CCLabelTTF		*	m_pLabTxt;
};