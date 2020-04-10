#pragma once

#include "cocos2d.h"

using namespace cocos2d;
class ParticleLayer : public cocos2d::CCLayer
{
public:
	virtual ~ParticleLayer();

	CREATE_FUNC(ParticleLayer);

	virtual bool init();  
	virtual void update(float dt);

private:
	CCParticleSystem*	_startParticle(char*	szFile, int nX, int nY);
	void	_stopParticle(CCParticleSystem*	pParticle);

	void		_checkParticle();
};