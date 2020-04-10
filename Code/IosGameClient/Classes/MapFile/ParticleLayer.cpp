#include "ParticleLayer.h"
#include "GlobalDefine.h"
#include "ParticleCenter.h"
#ifdef WIN32
#include "debug_def.h"
#endif
bool ParticleLayer::init()
{
	//return true;
	CCLayer::init();
	scheduleUpdate();

	//CCParticleSystem* m_emitter = CCParticleSnow::create();
	//m_emitter->retain();
	//CCPoint p = m_emitter->getPosition();
	//m_emitter->setPosition( ccp( p.x, p.y) );
	//m_emitter->setLife(10);
	//m_emitter->setLifeVar(1);

	//// gravity
	//m_emitter->setGravity(ccp(0,-10));

	//// speed of particles
	//m_emitter->setSpeed(130);
	//m_emitter->setSpeedVar(30);


	//ccColor4F startColor = m_emitter->getStartColor();
	//startColor.r = 0.9f;
	//startColor.g = 0.9f;
	//startColor.b = 0.9f;
	//m_emitter->setStartColor(startColor);

	//ccColor4F startColorVar = m_emitter->getStartColorVar();
	//startColorVar.b = 0.1f;
	//m_emitter->setStartColorVar(startColorVar);

	//m_emitter->setEmissionRate(m_emitter->getTotalParticles()/m_emitter->getLife());

	//m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("ui/snow.png") );

	if(pParticleCenter)
	{
		pParticleCenter->Clear();
	}

	return true;
}

ParticleLayer::~ParticleLayer()
{
	//delete pParticleCenter;
}

CCParticleSystem* ParticleLayer::_startParticle( char* szFile, int nX, int nY )
{
	CCParticleSystem* pEmitter = CCParticleSystemQuad::create(szFile);
	//m_emitter->retain();
	pEmitter->setBlendAdditive(true);//是否添加混合
	pEmitter->setPosition(ccp( nX,nY));//设置发射粒子的位置
	pEmitter->setAutoRemoveOnFinish(false);//完成后制动移除pEmitter
	pEmitter->setScale(1.0f/POINT_PIX_CNT);
	addChild(pEmitter, 10);

	return pEmitter;
}


void ParticleLayer::_stopParticle( CCParticleSystem* pParticle )
{
	removeChild(pParticle,true);
	pParticle->stopSystem();
}

void ParticleLayer::update( float dt )
{
	CCLayer::update(dt);

#ifdef WIN32

		pParticleCenter->Update();

		_checkParticle();

#else
	try
	{
		pParticleCenter->Update();
		_checkParticle();
	}
	catch (void* e)
	{

	}
#endif
}

void ParticleLayer::_checkParticle()
{
	for (int i = pParticleCenter->m_pList->Count-1; i >= 0; i--)
	{
		ParticleObj*	pParticle	= (ParticleObj*)pParticleCenter->m_pList->Items[i];
		//int nPosX	= (pParticleCenter->m_nX+pParticle->nPosX)/POINT_PIX_CNT;
		//int nPosY	= (pParticleCenter->m_nMapSizeH-pParticleCenter->m_nY-pParticle->nPosY)/POINT_PIX_CNT+SCREEN_SIZE_H;

		if(pParticle->dwStartTick	== 0 || pParticle->pParam == NULL)
		{
			pParticle->pParam		= _startParticle(pParticle->szFile,pParticle->nPosX/POINT_PIX_CNT,SCREEN_SIZE_H - pParticle->nPosY/POINT_PIX_CNT);
			pParticle->dwStartTick	= xnGetTickCount();
			pParticle->dwPosTick	= xnGetTickCount();
		}
		else if(pParticle->dwRunTick && pParticle->pParam && xnGetTickCount() - pParticle->dwStartTick > pParticle->dwRunTick)
		{
			_stopParticle((CCParticleSystem*)pParticle->pParam);
			pParticleCenter->m_pList->Delete(i);
			delete pParticle;
		}
		else if(pParticle->bDel)
		{
			_stopParticle((CCParticleSystem*)pParticle->pParam);
			pParticleCenter->m_pList->Delete(i);
			delete pParticle;
		}
		else
		{
			switch(pParticle->bActionType)
			{
			case 1:
				{
					ccBezierConfig bezier;
					bezier.controlPoint_1 = ccp(SCREEN_SIZE_W*(10+rand()%10)/30, SCREEN_SIZE_H*(10+rand()%10)/30);  //赛尔曲线第一控制点
					bezier.controlPoint_2 = ccp(SCREEN_SIZE_W*(30+rand()%10)/50, SCREEN_SIZE_H*(10+rand()%10)/30);   //赛尔曲线第二控制点
					if(nowRateH)
						bezier.endPosition = ccp((nowScreenWidth-60)/POINT_PIX_CNT,(320*nowRateH)/POINT_PIX_CNT);    //赛尔曲线结束点
					else
						bezier.endPosition = ccp((nowScreenWidth-60)/POINT_PIX_CNT,320/POINT_PIX_CNT);    //赛尔曲线结束点

					CCActionInterval*  bezierForward = CCBezierTo::create(1.5f, bezier);
					if(bezierForward && pParticle->pParam)
						((CCParticleSystem*)pParticle->pParam)->runAction(bezierForward);
				}
				break;
			case 2:
				{
					ccBezierConfig bezier;
					bezier.controlPoint_1 = ccp(SCREEN_SIZE_W/3, SCREEN_SIZE_H);  //赛尔曲线第一控制点
					bezier.controlPoint_2 = ccp(SCREEN_SIZE_W/5, SCREEN_SIZE_H);   //赛尔曲线第二控制点

					bezier.endPosition = ccp((nowScreenWidth-60)/POINT_PIX_CNT,SCREEN_SIZE_H-200/POINT_PIX_CNT);    //赛尔曲线结束点

					CCActionInterval*  bezierForward = CCBezierTo::create(1.5f, bezier);
					if(bezierForward && pParticle->pParam)
						((CCParticleSystem*)pParticle->pParam)->runAction(bezierForward);
				}
				break;
			case 3:
				{
					ccBezierConfig bezier;
					bezier.controlPoint_1 = ccp(SCREEN_SIZE_W/3, SCREEN_SIZE_H/5);  //赛尔曲线第一控制点
					bezier.controlPoint_2 = ccp(SCREEN_SIZE_W/5, SCREEN_SIZE_H/5);   //赛尔曲线第二控制点

					bezier.endPosition = ccp((nowScreenWidth-60)/POINT_PIX_CNT,SCREEN_SIZE_H-200/POINT_PIX_CNT);    //赛尔曲线结束点

					CCActionInterval*  bezierForward = CCBezierTo::create(1.5f, bezier);
					if(bezierForward && pParticle->pParam)
						((CCParticleSystem*)pParticle->pParam)->runAction(bezierForward);
				}
				break;
			case 4:
				{
					ccBezierConfig bezier;
					bezier.controlPoint_1 = ccp(SCREEN_SIZE_W/3, SCREEN_SIZE_H/2);  //赛尔曲线第一控制点
					bezier.controlPoint_2 = ccp(SCREEN_SIZE_W/5, SCREEN_SIZE_H/2);   //赛尔曲线第二控制点

					bezier.endPosition = ccp((nowScreenWidth-60)/POINT_PIX_CNT,SCREEN_SIZE_H-200/POINT_PIX_CNT);    //赛尔曲线结束点

					CCActionInterval*  bezierForward = CCBezierTo::create(1.5f, bezier);
					if(bezierForward && pParticle->pParam)
						((CCParticleSystem*)pParticle->pParam)->runAction(bezierForward);
				}
				break;
			case 5:		//位置刷新
				{
					if(xnGetTickCount() - pParticle->dwPosTick > 1000 && pParticle->pParam)
					{
						CCParticleSystem* pObj	= (CCParticleSystem*)pParticle->pParam;
						pObj->setPosition(ccp(pParticle->nPosX/POINT_PIX_CNT,SCREEN_SIZE_H - pParticle->nPosY/POINT_PIX_CNT));

						pParticle->dwPosTick	= xnGetTickCount();
					}

				}
				break;
			}
			pParticle->bActionType	= 0;
		}
	}
}
