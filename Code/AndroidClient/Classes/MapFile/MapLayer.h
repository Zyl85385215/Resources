#pragma once

#include "cocos2d.h"
#include "xnlist.h"
#include "GlobalDefine.h"

class CCEffectSprite;
class MapEventCenter;
class ControlLayer;
class ParticleLayer;
class MapLayer : public cocos2d::CCLayer
{
public:
	MapLayer();
	~MapLayer();
	virtual bool init();  

	// implement the "static node()" method manually
	CREATE_FUNC(MapLayer);

	virtual void onEnter();	//�����л���A->init B->onExit A->onEnter  ��������Ч������A->init A->onEnter  B->onExit
	virtual void onExit();

	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void update(float dt);
	void	TryUpdate();

	cocos2d::CCPoint	PtMap2Cocos(int nX, int nY);
	bool	PtCocos2Map(cocos2d::CCPoint ptIn, int& nX, int& nY);

	void	InitMap();	//����ͼ����Ҫ������

	ControlLayer*		m_pCtrlLayer;
private:
	void	_updateTile();		//�ر�
	void	_updateMObj();		//NPC�����������
	void	_updateEffs();		//����ر��Ч
	void	_updateOther();		//��괥��Ч��

	void	_updateControl();

	xnList*		m_pLTileOld;
	xnList*		m_pLMObjOld;

	xnList*		m_pLSprEff;
	cocos2d::CCLayer*	m_pLayerTile;
	cocos2d::CCLayer*	m_pLayerMObj;
	//ParticleLayer*		m_pLayerParticle;

	cocos2d::CCPoint	m_ptClick;
	MapEventCenter*		m_pMapEvent;

	//cocos2d::CCSprite*	m_pSprSel;
	CCEffectSprite*		m_pEffMove;
	CCEffectSprite*		m_pEffSel;
	DWORD	m_dwMEffTick;

	bool	m_bClickHold;
	int		m_nRunMIndex;

	xnList*		m_pListGroundEff;
	//cocos2d::CCLayer*	m_pLayerFarPic;		//Զ����
	//cocos2d::CCLayer*	m_pLayerWeath;		//����������
};

extern MapLayer*	pMapLayer;