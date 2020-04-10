#pragma once

#include "cocos2d.h"
#include "GlobalDefine.h"
#include "xnlist.h"
#include "InterfaceLayer.h"
#include "HttpDownThread.h"

class GameLoadScene	: public cocos2d::CCScene
{
public:
	GameLoadScene();
	~GameLoadScene();

	CREATE_FUNC(GameLoadScene);
	virtual	void	onEnter ();
	virtual void	onExit ();
	virtual void	update (float dt);

	void		StartGame();
private:
	void		_initBackSprite();
	void		_updateCheck();

	cocos2d::CCSprite*	m_pSpriteBack[2];
	cocos2d::CCSprite*	m_pSpriteCloud[2];

	cocos2d::CCSprite*	m_pSprBack;
	//cocos2d::CCSprite*	m_pSprLight;
	cocos2d::CCProgressTimer*	m_pProTime;
	CCLabelTTF		*	m_pLabTxt;
	HttpVersionManager*	m_pHttpVManager;
};