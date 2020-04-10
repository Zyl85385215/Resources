#include "TxtLinkView.h"
#include "GlobalDefine.h"
#include "CtrlObj.h"
#include "InterfaceLayer.h"

using namespace cocos2d;
bool	g_bIFLayerClicked	= false;

MyEventRect::~MyEventRect()
{

}

MyDragLayer::MyDragLayer()
{
	setTouchEnabled (true);
	//autorelease ();

	m_rectLimit.setRect (0,0,SCREEN_SIZE_W,SCREEN_SIZE_H);
	m_bClicked	= false;
	m_rectReal.setRect (0,0,SCREEN_SIZE_W/3,SCREEN_SIZE_H/2);

	m_bEnableDrag	= false;
	m_bLockThrough	= false;
	m_pListBtnRect	= xnList::Create ();
	m_pCurERect	= NULL;
	m_bZorde	= 0;
}

MyDragLayer::~MyDragLayer()
{
	while(m_pListBtnRect->Count)
	{
		delete (MyEventRect*)m_pListBtnRect->Delete (0);
	}
	m_pListBtnRect->Free ();
}

void MyDragLayer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, -m_bZorde, true);
}

bool MyDragLayer::ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	if(g_bIFLayerClicked)
		return false;
	CCPoint	ptCur	= convertTouchToNodeSpace(pTouch);
	MyEventRect* pERectMax = NULL;
	for (int i = m_pListBtnRect->Count-1; i >= 0; i--)
	{
		MyEventRect* pERect	= (MyEventRect*)m_pListBtnRect->Items[i];
		if(pERect->rect.containsPoint (ptCur))
		{
			if(pERectMax&&pERectMax->nOrder<pERect->nOrder){
				pERectMax = pERect;
			}else if(!pERectMax){
				pERectMax = pERect;
			}	
		}
	}
	if (pERectMax)
	{
		pERectMax->fnEventCB(0,ptCur.x-pERectMax->rect.getMinX (),pERectMax->rect.getMaxY ()-ptCur.y,pERectMax->pParam);
		m_pCurERect	= pERectMax;
		g_bIFLayerClicked	= true;
		return true;
	}
	m_pCurERect	= NULL;

	if(m_bLockThrough)
		return true;

	if(!m_bEnableDrag && !m_bEnableClick)	//CSpecialText必须不处理下面的内容
		return false;
	CCPoint	pt	= pTouch->getLocation ();
	if(!m_rectReal.containsPoint (pt))
		return false;

	if(m_bEnableDrag)
		m_bClicked	= true;
	g_bIFLayerClicked	= true;
	return true;
}

void MyDragLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	if(m_pCurERect)
	{
		CCPoint	ptCur	= convertTouchToNodeSpace(pTouch);
		m_pCurERect->fnEventCB(1,ptCur.x-m_pCurERect->rect.getMinX (),m_pCurERect->rect.getMaxY ()-ptCur.y,m_pCurERect->pParam);
		CCNode* pNode = (CCNode*)m_pCurERect->pParam;
		DComponent* pCom = (DComponent*)pNode->getTag();
		if (pCom&&pCom->m_bDraged)
		{
			CCPoint	ptDiff	= pTouch->getDelta ();

			CCPoint	ptCur	= pNode->getPosition();
			CCPoint	ptMove;
			if(nowRateW && nowRateH){
				ptMove	= ccpAdd(ccp(pCom->nX/POINT_PIX_CNT,pCom->nY/POINT_PIX_CNT), ccp(ptDiff.x/nowRateW,-ptDiff.y/nowRateH));
			}else{
				ptMove	= ccpAdd(ccp(pCom->nX/POINT_PIX_CNT,pCom->nY/POINT_PIX_CNT), ccp(ptDiff.x,-ptDiff.y));
			}
			pCom->SetPos(ptMove.x*POINT_PIX_CNT,ptMove.y*POINT_PIX_CNT);
		}
		return;
	}
	if(!m_bClicked)
		return;
	CCPoint	ptDiff	= pTouch->getDelta ();

	CCPoint	ptCur	= getPosition ();
	MovePosition( ccpAdd(ptCur, ptDiff) );
}

void MyDragLayer::ccTouchEnded( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{	
	g_bIFLayerClicked	= false;
	if(m_pCurERect)
	{
		CCPoint	ptCur	= convertTouchToNodeSpace(pTouch);
		m_pCurERect->fnEventCB(2,ptCur.x-m_pCurERect->rect.getMinX (),m_pCurERect->rect.getMaxY ()-ptCur.y,m_pCurERect->pParam);
		m_pCurERect	= NULL;

		return;
	}
	if(!m_bClicked)
		return;
	m_bClicked	= false;

	CCPoint	ptCur	= getPosition ();
	bool	bNeedMove	= false;
	CCPoint	ptCenter		= ptCur;
	ptCenter.x		+=	m_rectReal.size.width/2;
	ptCenter.y		-=	m_rectReal.size.height/2;
	int nDestX	= ptCenter.x,nDestY	= ptCenter.y;
	if(m_rectLimit.containsPoint (ptCenter)	== false)
	{
		if(ptCenter.x < m_rectLimit.getMinX ()+m_rectReal.size.width/2)
		{
			nDestX	= m_rectLimit.getMinX ()+m_rectReal.size.width/2;
			bNeedMove	= true;
		}
		if(ptCenter.x > m_rectLimit.getMaxX ()-m_rectReal.size.width/2)
		{
			nDestX	= m_rectLimit.getMaxX ()-m_rectReal.size.width/2;
			bNeedMove	= true;
		}
		if(ptCenter.y < m_rectLimit.getMinY ()+m_rectReal.size.height/2)
		{
			nDestY	= m_rectLimit.getMinY ()+m_rectReal.size.height/2;
			bNeedMove	= true;
		}
		if(ptCenter.y > m_rectLimit.getMaxY ()-m_rectReal.size.height/2)
		{
			nDestY	= m_rectLimit.getMaxY ()-m_rectReal.size.height/2;
			bNeedMove	= true;
		}
		if(bNeedMove)
		{
			int		nRtlX	= nDestX - m_rectReal.size.width/2;
			int		nRtlY	= nDestY + m_rectReal.size.height/2;
			MovePosition(ccp(nRtlX,nRtlY));
		}
	}
	m_rectReal.setRect (nDestX - m_rectReal.size.width/2,nDestY - m_rectReal.size.height/2,m_rectReal.size.width,m_rectReal.size.height);
	
}

void MyDragLayer::AddRegEvent( cocos2d::CCRect rc,void* pParam,void (*Fun)(int nEvent,int x, int y, void* pParam) ,int nOrder)
{
	MyEventRect*	pER	= new MyEventRect();
	pER->rect	= rc;
	pER->rect.setRect (rc.origin.x-5/POINT_PIX_CNT,rc.origin.y-5/POINT_PIX_CNT,rc.size.width + 10/POINT_PIX_CNT,rc.size.height+10/POINT_PIX_CNT);
	pER->pParam	= pParam;
	pER->fnEventCB = Fun;
	pER->nOrder = nOrder;
	m_pListBtnRect->Add (pER);
}

bool MyDragLayer::ChgRegERect( void* pParam, cocos2d::CCRect rc )
{
	for (int i = 0; i < m_pListBtnRect->Count; i++)
	{
		MyEventRect* pER	= (MyEventRect*)m_pListBtnRect->Items[i];
		if(pER->pParam	== pParam)
		{
			pER->rect	= rc;
			return true;
		}
	}
	return false;
}

void MyDragLayer::ClearRegERect()
{
	while(m_pListBtnRect->Count)
	{
		delete (MyEventRect*)m_pListBtnRect->Delete (0);
	}
	m_pCurERect = NULL;
}

void MyDragLayer::MovePosition( cocos2d::CCPoint pt )
{
	setPosition (pt);

	if(m_pChildren && m_pChildren->count() > 0)
	{
		CCObject* child;
		CCARRAY_FOREACH(m_pChildren, child)
		{
			CCNode* pNode = (CCNode*) child;
			if(pNode && pNode->getTag () != -1 && pNode->getTag() != GetNodTagFrame())
			{
				DComponent* pComp	= (DComponent*)pNode->getTag ();
				if(pComp->pSTextCenter)
				{
					((CSpecialText*)pNode)->SetRealRect (CCRect(pComp->nX/POINT_PIX_CNT +pt.x,pt.y-pComp->nY/POINT_PIX_CNT-pComp->nH/POINT_PIX_CNT,pComp->nW/POINT_PIX_CNT,pComp->nH/POINT_PIX_CNT));
				}
			}
		}
	}
}

void MyDragLayer::SetRealRect( cocos2d::CCRect rc )
{
	if(nowRateW && nowRateH){
		rc.setRect (rc.origin.x*nowRateW,rc.origin.y*nowRateH,rc.size.width*nowRateW,rc.size.height*nowRateH);
	}
	m_rectReal		= rc;
}

void MyDragLayer::SetLimitRect( cocos2d::CCRect rc )
{
	//if(nowScreenWidth<640||nowScreenHeight<960){
	//	rc.setRect (rc.origin.x*nowRateW,rc.origin.y*nowRateH,rc.size.width*nowRateW,rc.size.height*nowRateH);
	//}
	m_rectLimit	= rc;
}
   
void MyDragLayer::onExit()
{
	CCLayer::onExit();
	g_bIFLayerClicked	= false;

}
