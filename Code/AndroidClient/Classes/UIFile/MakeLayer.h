#ifndef MakeLayer_h__
#define MakeLayer_h__
#include <cocos2d.h>
#include <list>
//#include <stack>
#include "GlobalDefine.h"
#define TILE_NULL 255
using namespace cocos2d;
using namespace std;
class CCEffectSprite;
class CCClainSprite:public CCSprite{
public:
	CCClainSprite();
	BYTE m_byTile;
	virtual bool initWithFile();

	void	SetFallDown(int nDistance);
	static	int	nFallCount;
	static	int	nCleanCount;

	void	RunFadeOutEffect();
	//CCEffectSprite*		m_pEffSpr;
private:
	void	_onFallCallBack();
};
class MakeLayer:public CCLayer
{
public:
	static int nTileSize;
	MakeLayer();
	~MakeLayer();
	virtual void onEnter();
	virtual void registerWithTouchDispatcher();

	virtual bool ccTouchBegan (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded (cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void update(float dt);

	void	AddEff(int nX,int nY);

	std::list<CCEffectSprite*>	m_pEff;
public:

	static const int nMaxWidth = 6;
	static const int nMaxHeight = 6;


	//BYTE		m_byTile[nMaxWidth][nMaxHeight];
	CCClainSprite*	m_pTile[nMaxWidth][nMaxHeight];


	void		CleanUp();

	void		CleanUp(CCClainSprite* pTile);
	static	int	nExchangeCount;

	void		RandomMake();

	void		SetType(BYTE byType,void (*fnCallBackEvent)(int nEvent,int nVal));
private:
	void		(*m_fnCallBackEvent)(int nEvent,int nVal);
	CCPoint		m_ClickPoint;
	bool		m_bClicked;
	bool		m_bExchange;
	bool		m_bCleaning;

	void		_initTile();
	void		_exchange(int nFormX,int nFormY,int nToX,int nToY);
	bool		_checkExchange(int nFormX,int nFormY,int nToX,int nToY);
	void		_afterExchange();
	void		_afterExchangeFailed();

	bool		_checkClain(list<BYTE>& listTile);
	void		_getClainTile(list<CCClainSprite*>& fromListTile,list<CCClainSprite*>& toStackTile);

	list<CCClainSprite*> m_ResetSpr;
	void		_addResetSpr(CCNode* pNode,void* pParam);

	void		_resetSpr();

	enum{
		DIRECT_NORTH,
		DIRECT_SOUTH,
		DIRECT_WEST,
		DIRECT_EAST,
	};
	struct		_tileGroup
	{
		//
		_tileGroup(int tx,int ty,BYTE tdirect){x=tx;y=ty;byDirect=tdirect;};
		int		x;
		int		y;
		BYTE		byDirect;		//…œœ¬◊Û”“
	};

	void		_getTileGroup(int x,int y,list<_tileGroup>& resultList);
	inline	BYTE _getTileType(int x,int y);
	
	void		_checkPos();
	
};


#endif // MakeLayer_h__