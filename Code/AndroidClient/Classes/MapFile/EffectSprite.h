#pragma once

#include "cocos2d.h"
#include "EffectObj.h"
#include "GlobalDefine.h"

//���������ļ��Ĺ�Ч���ϲ���ʵ�ʹ�Ч
class EffectObj;
class CCEffectSprite	: public cocos2d::CCSprite
{
public:
	static CCEffectSprite* create(EffectObj* pEff, int nDir = -1,bool bSound=true);
	~CCEffectSprite();

	bool	InitSprAction(int nDir);
	void	SetMoveAction(cocos2d::CCPoint	ptTarget);

	bool	CheckEnd(bool bSound = true,int nDis = 0);			//����ǲ���
	int		GetEffID()	{return m_pEff->m_nEffID;}
	int		GetEffZorder()	{return m_pEff->m_nZorder;}

	int				m_nLoopTime;
	int				m_nSPID;
	DWORD			m_dwEndTick;
	void*			m_pSpVal;

	EffectObj*		m_pEff;
private:
	
	cocos2d::CCAnimate*		m_pAnimate;
	char*			m_szFrame;
};



class CCNumberSprite	: public cocos2d::CCSprite
{
public:
	//������ʽ, ��ֵ, ǰ׺
	static CCNumberSprite* create( int nNum, BYTE bType);
	~CCNumberSprite();

	bool	InitSprAction(int nNum, BYTE bType);
	bool	CheckEnd();

	void	SetAlphaAction(float fTime,float fDelay);

	int		m_nSizeW;
};