#include "GuideLayer.h"
#include "GlobalDefine.h"
#include "GuideCenter.h"
#include "Interface.h"
#include "EffectSprite.h"
#include "InterfaceLayer.h"

#include "UserData.h"
#include "CreateRoleMenu.h"
#include "LoginScene.h"
#include "MainMenu.h"

#ifdef WIN32
#include "debug_def.h"
#endif

ControlLayer*	pCtrlLayer	= NULL;

bool GuideLayer::init()
{
	//return true;
	CCLayer::init();
	scheduleUpdate();
	m_bWorked	= false;

	//pGuideCenter	= new GuideCenter();

	for (int i = 0; i < 4; i++)
	{
		m_szpSprHold[i]		= CCSprite::create();
		m_szpSprHold[i]->initWithFile("ui/MB_HEIMU.png");
		m_szpSprHold[i]->retain();
	}
	m_pSprMid		= CCSprite::create();
	m_pSprMid->initWithFile("ui/MB_HEIMUGB.png");
	m_pSprMid->retain();
	
	m_pSprBack		= CCSprite::create();
	m_pSprBack->initWithFile("ui/MB_MAN1.png");
	m_pSprBack->retain();	
	m_pSprBack2		= CCSprite::create();
	m_pSprBack2->initWithFile("ui/MB_RWXZ.png");
	m_pSprBack2->retain();	

	m_pMyLabS	= CCLabelTTF::create("null",DEFAULT_FONT,24/POINT_PIX_CNT);
	m_pMyLabS->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
	m_pMyLabS->setHorizontalAlignment(kCCTextAlignmentLeft);
	m_pMyLabS->setDimensions(CCSize(330/POINT_PIX_CNT,150/POINT_PIX_CNT));//原先150
	m_pMyLabS->setColor(ccc3(229,151,28));
	m_pSprBack->addChild(m_pMyLabS);
	m_pMyLabS->setAnchorPoint(ccp(0,1));
	m_pMyLabS->setPosition(ccp(30/POINT_PIX_CNT,180/POINT_PIX_CNT));
	m_pMyLabS->retain();

	m_pMyLabS2	= CCLabelTTF::create("null",DEFAULT_FONT,24/POINT_PIX_CNT);
	m_pMyLabS2->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
	m_pMyLabS2->setHorizontalAlignment(kCCTextAlignmentLeft);
	m_pMyLabS2->setDimensions(CCSize(330/POINT_PIX_CNT,150/POINT_PIX_CNT));//原先150
	m_pMyLabS2->setColor(ccc3(229,151,28));
	m_pSprBack->addChild(m_pMyLabS2);
	m_pMyLabS2->setAnchorPoint(ccp(0,1));
	m_pMyLabS2->setPosition(ccp(30/POINT_PIX_CNT,180/POINT_PIX_CNT));
	m_pMyLabS2->retain();	

	EffectObj*	pEO	= EffectCenter::GetInstance ().GetEffObj (38);
	m_pSprEff	= CCEffectSprite::create(pEO);
	m_pSprEff->retain();	
	EffectObj*	pEO2	= EffectCenter::GetInstance ().GetEffObj (39);
	m_pSprEff2	= CCEffectSprite::create(pEO2);
	m_pSprEff2->retain();

	if(nowRateW && nowRateH){
		this->setAnchorPoint(CCPointZero);
		this->setScaleX(nowRateW);
		this->setScaleY(nowRateH);
	}
	setTouchEnabled(true);
	return true;
}

GuideLayer::~GuideLayer()
{
	for (int i = 0; i < 4; i++)
	{
		m_szpSprHold[i]->release();
	}
	m_pMyLabS->release();
	m_pMyLabS2->release();
	m_pSprBack->release();
	m_pSprBack2->release();
	m_pSprEff->release();
	m_pSprEff2->release();
	m_pSprMid->release();
}

bool GuideLayer::UpdateGuideForm()
{
	DForm*	pForm = NULL;
	if (strncmp(pGuideCenter->m_pCurGuide->szForm, "zhiye", 5)==0)
	{
		char szTtemp[30] = {0};
		if(pChar->m_byJob > 0 && pChar->m_byJob <= 4)
			sprintf(szTtemp,"zhiye%d",pChar->m_byJob);			
		pForm	= pMainInterface->GetDForm(szTtemp);		
	}
	else
	{
		pForm	= pMainInterface->GetDForm(pGuideCenter->m_pCurGuide->szForm);	
	}

	if(pForm && !pForm->bOpened)
	{
		return false;
	}
	else if(pForm == NULL)
	{
		pGuideCenter->m_pCurGuide->pForm = NULL;
		return true;
	}
	pGuideCenter->m_pCurGuide->pForm	= pForm;

	if(strcmp(pGuideCenter->m_pCurGuide->szComp, "beng") == 0)
	{							
		char szTomp[20] = {0};
		sprintf(szTomp,"bag%d", pGuideCenter->nAwardItemPos + 1);
		if(pGuideCenter->m_pCurGuide->szComp)
			free(pGuideCenter->m_pCurGuide->szComp);
		pGuideCenter->m_pCurGuide->szComp = strdup(szTomp);
	}

	if(strcmp(pGuideCenter->m_pCurGuide->szComp, "inlay") == 0)
	{							
		char szTomp[20] = {0};
		sprintf(szTomp,"bag%d", pGuideCenter->m_nAwardInlayPos + 1);
		if(pGuideCenter->m_pCurGuide->szComp)
			free(pGuideCenter->m_pCurGuide->szComp);
		pGuideCenter->m_pCurGuide->szComp = strdup(szTomp);
	}

	if(strcmp(pGuideCenter->m_pCurGuide->szComp, "eat") == 0)
	{							
		char szTomp[20] = {0};
		sprintf(szTomp,"bag%d", (pGuideCenter->m_nAwardEatPos + 1));
		if(pGuideCenter->m_pCurGuide->szComp)
			free(pGuideCenter->m_pCurGuide->szComp);
		pGuideCenter->m_pCurGuide->szComp = strdup(szTomp);
	}

	static int nCurStarLv = 0;
	if (strcmp(pGuideCenter->m_pCurGuide->szComp, "star") == 0)
	{				
		char szStar[20] = {0};				
		nCurStarLv = pChar->m_wStarLv + 1;
		sprintf(szStar,"q%d", nCurStarLv%10);	
		if(pGuideCenter->m_pCurGuide->szComp)
			free(pGuideCenter->m_pCurGuide->szComp);
		pGuideCenter->m_pCurGuide->szComp = strdup(szStar);		
	}	

	if (strcmp(pGuideCenter->m_pCurGuide->szComp, "stars") == 0)
	{				
		char szStars[20] = {0};				
		sprintf(szStars,"q%d", (nCurStarLv + 1)%10);
		if(pGuideCenter->m_pCurGuide->szComp)
			free(pGuideCenter->m_pCurGuide->szComp);
		pGuideCenter->m_pCurGuide->szComp = strdup(szStars);		
	}
	DComponent*  pComp = NULL;
	if (strcmp(pGuideCenter->m_pCurGuide->szForm, "huodong") == 0 && strcmp(pGuideCenter->m_pCurGuide->szComp, "go") == 0)
	{
		int nDis = 0;
		if (pGuideCenter->nActivityPos < 3)
		{
			pComp  = pMainMenu->m_pActivityMenu->m_pForm->pCFrame->GetFPComp((pGuideCenter->nActivityPos+2),"go");
			nDis = pGuideCenter->nActivityPos*pMainMenu->m_pActivityMenu->m_pForm->pCFrame->nDisStep;
		}else
		{
			pComp  = pMainMenu->m_pActivityMenu->m_pForm->pCFrame->GetFPComp((pGuideCenter->nActivityPos),"go");
			nDis = pMainMenu->m_pActivityMenu->m_pForm->pCFrame->nDisStep;
		}
		if(pComp)
		{ 
			DComponent* ppComp = pMainMenu->m_pActivityMenu->m_pForm->GetComponent("go");
			int nDataX	= pForm->nPosX + ppComp->nX;
			int	nDataY	= pForm->nPosY + ppComp->nY + nDis;
			//int	nDataY	= pForm->nPosY + ppComp->nY + (pComp->nY - ppComp->nY)/pGuideCenter->nActivityPos + ppComp->nH;
			m_rcClick.setRect(nDataX/POINT_PIX_CNT,SCREEN_SIZE_H - (nDataY+ppComp->nH)/POINT_PIX_CNT,ppComp->nW/POINT_PIX_CNT,ppComp->nH/POINT_PIX_CNT);	
		}	
		else 
			return false;
	}else
	{
		pComp	= pForm->GetComponent(pGuideCenter->m_pCurGuide->szComp);
		if(pComp)
		{
			int nDataX	= pForm->nPosX + pComp->nX;
			int	nDataY	= pForm->nPosY + pComp->nY;
			m_rcClick.setRect(nDataX/POINT_PIX_CNT,SCREEN_SIZE_H - (nDataY+pComp->nH)/POINT_PIX_CNT,pComp->nW/POINT_PIX_CNT,pComp->nH/POINT_PIX_CNT);	
		}	
		else 
			return false;
	}
	return true;
}

void GuideLayer::ResetViewWindow()
{
	char szTepp[128];
	sprintf(szTepp, "ui/%s", pGuideCenter->m_pCurGuide->mapId);
	m_pSprBack->initWithFile(szTepp);
	if (strncmp(pGuideCenter->m_pCurGuide->mapId, "MB_GuideBegin.png", 17)==0||strncmp(pGuideCenter->m_pCurGuide->mapId, "MB_GuideBegin1.png", 18)==0)
	{		
		m_pSprBack->setScaleX(SCREEN_SIZE_W*POINT_PIX_CNT/1024.0);				
		m_pSprBack->setScaleY(SCREEN_SIZE_H*POINT_PIX_CNT/768.0);		
	}
	else
	{
		m_pSprBack->setScale(1);				
	}
	//m_pSprBack->retain();		

	int nPosX = 0,nPosY = 0;
	if (strncmp(pGuideCenter->m_pCurGuide->map2, "zhiyesex", 8)==0)
	{
		char szTepp2[128];	
		if (pChar->m_bySex == 1) //nan
		{
			if(pChar->m_byJob > 0 && pChar->m_byJob <= 4)
				sprintf(szTepp2,"ui/MB_MAN%d.png",pChar->m_byJob);
		}
		else if (pChar->m_bySex == 0)
		{
			if(pChar->m_byJob > 0 && pChar->m_byJob <= 4)
				sprintf(szTepp2,"ui/MB_WOMAN%d.png",pChar->m_byJob);
		}
		//sprintf(szTepp2, "ui/%s", szCurUserPic);			
		m_pSprBack2->initWithFile(szTepp2);
		/*m_pSprBack2->setScaleX(SCREEN_SIZE_W*POINT_PIX_CNT/1024.0);				
		m_pSprBack2->setScaleY(SCREEN_SIZE_H*POINT_PIX_CNT/768.0);*/
	//	m_pSprBack2->retain(); 
		nPosX = 300;
		nPosY = 170;
	}
	else
	{
		char szTepp2[128];
		sprintf(szTepp2, "ui/%s", pGuideCenter->m_pCurGuide->map2);
		m_pSprBack2->initWithFile(szTepp2);
		m_pSprBack2->setScale(1);		
	//	m_pSprBack2->retain();
		if (strncmp(pGuideCenter->m_pCurGuide->mapId, "MB_GuideBegin.png", 17)==0)
		{
			if (pGuideCenter->m_pCurGuide->nID == 1)
			{
				int nLen = strlen(pChar->m_szNick);
				nPosX = 120+(nLen/2-2)*22;
				nPosY = 180;
			}
			else
			{
				nPosX = 50;
				nPosY = 180;
			}
		}
		else 
		{
			nPosX = 0;
			nPosY = 65;
		}
	}	

	//m_pMyLabS	= CMyLabShade::create("null",DEFAULT_FONT,24/POINT_PIX_CNT);
	m_pMyLabS->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
	m_pMyLabS->setHorizontalAlignment(kCCTextAlignmentLeft);		
	m_pMyLabS->setDimensions(CCSize(pGuideCenter->m_pCurGuide->txtX/POINT_PIX_CNT,pGuideCenter->m_pCurGuide->txtY/POINT_PIX_CNT));
	m_pMyLabS->setColor(ccc3(255,255,255));
	//m_pSprBack->addChild(m_pMyLabS);			
	m_pMyLabS->setAnchorPoint(ccp(-0.2,1.3));
	m_pMyLabS->setPosition(ccp(nPosX/POINT_PIX_CNT,nPosY/POINT_PIX_CNT));//8种职业形象 文本1的位置
	//m_pMyLabS->retain();

	//第二文本
	//m_pMyLabS2	= CMyLabShade::create("",DEFAULT_FONT,24/POINT_PIX_CNT);
	if (pGuideCenter->m_pCurGuide->nID == 1)
	{
		m_pMyLabS2->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
		m_pMyLabS2->setHorizontalAlignment(kCCTextAlignmentLeft);
		m_pMyLabS2->setDimensions(CCSize(pGuideCenter->m_pCurGuide->txtX/POINT_PIX_CNT,pGuideCenter->m_pCurGuide->txtY/POINT_PIX_CNT));
		m_pMyLabS2->setColor(ccc3(255,0,0)); 
		//m_pSprBack->addChild(m_pMyLabS2);
		m_pMyLabS2->setAnchorPoint(ccp(-0.2,1.3));
		m_pMyLabS2->setPosition(ccp(50/POINT_PIX_CNT,180/POINT_PIX_CNT));//第二图片文本的位置
	}
	
	//m_pMyLabS2->retain();

	if(nowScreenHeight<640)
	{
		this->setAnchorPoint(CCPointZero); 
		this->setScaleX(nowRateW);
		this->setScaleY(nowRateH);
	}	
}

void GuideLayer::ResetChildren(int nRealPosX,int nRealPosY)
{
	removeAllChildrenWithCleanup(false);

	m_szpSprHold[0]->setAnchorPoint(ccp(0,0));
	m_szpSprHold[0]->setScaleX(SCREEN_SIZE_W/10.0*POINT_PIX_CNT);
	m_szpSprHold[0]->setScaleY(m_rcClick.getMinY()/10.0*POINT_PIX_CNT);
	m_szpSprHold[0]->setPosition(ccp(0,0));
	addChild(m_szpSprHold[0],1);

	m_szpSprHold[1]->setAnchorPoint(ccp(0,1));
	m_szpSprHold[1]->setScaleX(SCREEN_SIZE_W/10.0*POINT_PIX_CNT);
	m_szpSprHold[1]->setScaleY((nowScreenWidth/POINT_PIX_CNT-m_rcClick.getMaxY())/10.0*POINT_PIX_CNT);
	m_szpSprHold[1]->setPosition(ccp(0,nowScreenWidth/POINT_PIX_CNT));
	addChild(m_szpSprHold[1],1); 

	m_szpSprHold[2]->setAnchorPoint(ccp(0,0));
	m_szpSprHold[2]->setScaleX(m_rcClick.getMinX()/10.0*POINT_PIX_CNT);
	m_szpSprHold[2]->setScaleY(m_rcClick.size.height/10.0*POINT_PIX_CNT);
	m_szpSprHold[2]->setPosition(ccp(0,m_rcClick.getMinY()));
	addChild(m_szpSprHold[2],1);

	m_szpSprHold[3]->setAnchorPoint(ccp(0,0));
	m_szpSprHold[3]->setScaleX((SCREEN_SIZE_W-m_rcClick.getMaxX())/10.0*POINT_PIX_CNT);
	m_szpSprHold[3]->setScaleY(m_rcClick.size.height/10.0*POINT_PIX_CNT);
	m_szpSprHold[3]->setPosition(ccp(m_rcClick.getMaxX(),m_rcClick.getMinY()));
	addChild(m_szpSprHold[3],1);

	m_pSprMid->setAnchorPoint(ccp(0.5,0.5));
	m_pSprMid->setScaleX((m_rcClick.size.width)/100.0*POINT_PIX_CNT);
	m_pSprMid->setScaleY(m_rcClick.size.height/100.0*POINT_PIX_CNT);
	m_pSprMid->setPosition(ccp(m_rcClick.getMidX() ,m_rcClick.getMidY()));		
	addChild(m_pSprMid,1);

	if (strlen(pGuideCenter->m_pCurGuide->mapId))
	{
		m_pSprBack->setAnchorPoint(ccp(0,1));//0.5

		if (strcmp(pGuideCenter->m_pCurGuide->mapId, "MB_Guide.png")==0)
		{
			m_pSprBack->setPosition(ccp(m_rcClick.getMidX()+pGuideCenter->m_pCurGuide->nSprX/POINT_PIX_CNT,
				m_rcClick.getMidY()+pGuideCenter->m_pCurGuide->nSprY/POINT_PIX_CNT));
		}
		else
		{	
			m_pSprBack->setPosition(ccp( nRealPosX/POINT_PIX_CNT ,
				SCREEN_SIZE_H - nRealPosY/POINT_PIX_CNT ));				
		}
		addChild(m_pSprBack,3);
	}	
	if (strlen(pGuideCenter->m_pCurGuide->map2))//第二张图片
	{				
		if (strcmp(pGuideCenter->m_pCurGuide->map2, "zhiyesex")==0)//8种形象图片
		{		

			m_pSprBack2->setAnchorPoint(ccp(0,1));
			if(pChar->m_byJob ==1&&pChar->m_bySex ==1)
			{
				m_pSprBack2->setPosition(ccp(-150/POINT_PIX_CNT,
					nowScreenHeight/POINT_PIX_CNT/2+100/POINT_PIX_CNT));
			}
			else if(pChar->m_byJob ==3&&pChar->m_bySex ==0)
			{
				m_pSprBack2->setPosition(ccp(-50/POINT_PIX_CNT,
					nowScreenHeight/POINT_PIX_CNT/2+100/POINT_PIX_CNT));
			}
			else
			{
				m_pSprBack2->setPosition(ccp(-80/POINT_PIX_CNT,
					nowScreenHeight/POINT_PIX_CNT/2+100/POINT_PIX_CNT));
			}				
			addChild(m_pSprBack2,3);				
		}
		else
		{//main7图片  任务选中图片
			m_pSprBack2->setAnchorPoint(ccp(0,1));					
			m_pSprBack2->setPosition(ccp(m_rcClick.getMinX(),m_rcClick.getMaxY()));
			addChild(m_pSprBack2,3);
		}
	}
	if (pGuideCenter->m_pCurGuide->nEffID)
	{
		if(m_pSprEff)
		{
			m_pSprEff->release();
			m_pSprEff	= NULL;
		}
		EffectObj*	pEO	= EffectCenter::GetInstance ().GetEffObj (pGuideCenter->m_pCurGuide->nEffID);
		if(pEO)
		{
			int nBaseHeight = 0;
// 			if(pGuideCenter->m_pCurGuide->nEffID == 40)
// 				nBaseHeight = 58;
			if(pGuideCenter->m_pCurGuide->nEffID == 38)
				nBaseHeight = 68;
			if (strncmp(pGuideCenter->m_pCurGuide->szForm, "zhiye", 5)==0)
			{
				char szTtemp[30] = {0};
				if(pChar->m_byJob > 0 && pChar->m_byJob <= 4)
					sprintf(szTtemp,"zhiye%d",pChar->m_byJob);	
				if (pGuideCenter->m_pCurGuide->szForm)				
					free(pGuideCenter->m_pCurGuide->szForm);
				pGuideCenter->m_pCurGuide->szForm = strdup(szTtemp);	
			}
			DComponent* pComp = pMainInterface->GetComponent(pGuideCenter->m_pCurGuide->szForm,pGuideCenter->m_pCurGuide->szComp);
			pEO->m_nScale = 100;
			if(pComp && nBaseHeight)
			{
				if (pGuideCenter->m_pCurGuide->nID == 1301)
				{
					pEO->m_nScale = 140;
				}else
					pEO->m_nScale = pComp->nH*100/nBaseHeight;
			}
			if (strcmp(pGuideCenter->m_pCurGuide->szComp, "close") == 0)
				pEO->m_nScale = 105;
// 			if (pGuideCenter->m_pCurGuide->nEffID == 38)
// 			{
// 				if (strncmp(pGuideCenter->m_pCurGuide->szForm, "xingchen", 8) == 0)
// 				{
// 					pEO->m_nScale = 100;
// 				}else if (pGuideCenter->m_pCurGuide->nID==2||pGuideCenter->m_pCurGuide->nID == 2001)
// 				{
// 					pEO->m_nScale = 180;
// 				}else if ((strncmp(pGuideCenter->m_pCurGuide->szComp, "chongwu", 7)==0)||(strncmp(pGuideCenter->m_pCurGuide->szComp, "skill", 5)==0)||(strncmp(pGuideCenter->m_pCurGuide->szComp, "tx1", 3)==0)||(strncmp(pGuideCenter->m_pCurGuide->szComp, "bag", 3)==0))
// 				{
// 					pEO->m_nScale = 140;
// 				}else if ((strncmp(pGuideCenter->m_pCurGuide->szComp, "gj", 2)==0)||(strncmp(pGuideCenter->m_pCurGuide->szComp, "xc", 2)==0))
// 				{
// 					pEO->m_nScale = 140;
// 				}else if ((strncmp(pGuideCenter->m_pCurGuide->szComp, "g2", 2)==0)||(strncmp(pGuideCenter->m_pCurGuide->szComp, "fq", 2)==0)||(strncmp(pGuideCenter->m_pCurGuide->szComp, "zy", 2)==0))
// 				{
// 					pEO->m_nScale = 140;
// 				}else if ((pGuideCenter->m_pCurGuide->nID == 2201))
// 				{
// 					pEO->m_nScale = 160;
// 				}
// 				else
// 					pEO->m_nScale = 100;
// 			}else
// 				pEO->m_nScale = 100;
// 			if (strncmp(pGuideCenter->m_pCurGuide->szForm, "messagebox", 10)==0)
// 			{
// 				pEO->m_nScale = 125;
// 			}
// 			if (strncmp(pGuideCenter->m_pCurGuide->szForm, "zb1", 3)==0)
// 			{
// 				pEO->m_nScale = 85;					
// 			}
			m_pSprEff	= CCEffectSprite::create(pEO);
			m_pSprEff->retain();
			m_pSprEff->setPosition(ccp(m_rcClick.getMidX(),m_rcClick.getMidY()));
			addChild(m_pSprEff,3);
		}

	}
	if (pGuideCenter->m_pCurGuide->nEffID2)
	{
		if(m_pSprEff2)
		{
			m_pSprEff2->release();
			m_pSprEff2	= NULL;
		}			
		EffectObj*	pEO2	= EffectCenter::GetInstance ().GetEffObj (pGuideCenter->m_pCurGuide->nEffID2);
		if (pEO2)
		{
			m_pSprEff2	= CCEffectSprite::create(pEO2);
			m_pSprEff2->retain();
			m_pSprEff2->setPosition(ccp((m_rcClick.getMidX()+pGuideCenter->m_pCurGuide->eff2X/POINT_PIX_CNT),(m_rcClick.getMidY()+pGuideCenter->m_pCurGuide->eff2Y/POINT_PIX_CNT)));
			addChild(m_pSprEff2,3);
		}			
	}

	m_pMyLabS->setString(UTEXT_CHINE(pGuideCenter->m_pCurGuide->szTxt));
	if (pGuideCenter->m_pCurGuide->nID == 1)
	{
		char sztxt2[32];
		sprintf(sztxt2, "%s,", pChar->m_szNick);
		m_pMyLabS2->setString(UTEXT_CHINE(sztxt2));
	}
	else
		m_pMyLabS2->setString(UTEXT_CHINE(""));

	m_nCurGID	= pGuideCenter->m_pCurGuide->nID;
	m_nTestTime	= 0;

	if(nowRateW && nowRateH){
		m_rcClick.setRect (m_rcClick.origin.x*nowRateW,m_rcClick.origin.y*nowRateH,m_rcClick.size.width*nowRateW,m_rcClick.size.height*nowRateH);
	}	
}

void GuideLayer::update( float dt )
{
	CCLayer::update(dt);

#ifdef WIN32
	TryUpdate();

#else
	try
	{
		TryUpdate();
	}
	catch (...)
	{
		
	}
#endif
}


void GuideLayer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, -999, true);
}

bool GuideLayer::ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	if(!m_bWorked)
		return false;
	if(isVisible() == false)
		return false;
	if(!pGuideCenter->m_pCurGuide)
		return false;

	CCPoint	ptClick	= pTouch->getLocation ();
	//===zjk
	if(pGuideCenter->m_pCurGuide->freeDian == 1)
	{
		m_nTestTime = 0;		
		pGuideCenter->NextGuide();
		return true;
	}

	if(!m_rcClick.containsPoint (ptClick))
	{		
		m_nTestTime++;
		if (m_nTestTime==3)
		{
			m_nTestTime = 0;
			pGuideCenter->NextGuide();	
			return false;
		}
		return true;
	}	
	m_nTestTime++;
	if(pGuideCenter->m_pCurGuide && m_nTestTime >=1)
	{
		m_nTestTime = 0;
		pGuideCenter->NextGuide();	
		return false;
	}	
		
	return false;
}

void GuideLayer::TryUpdate()
{
	pGuideCenter->Update();
	m_pSprEff->CheckEnd();	
	m_pSprEff2->CheckEnd();

	if(pGuideCenter->m_pCurGuide && (!m_bWorked || m_nCurGID != pGuideCenter->m_pCurGuide->nID))
	{
		m_bWorked	=	true;
		if(!UpdateGuideForm())
		{
			m_bWorked = false;
			removeAllChildrenWithCleanup(false);
			return;
		}

		int nRealX	= 0;
		int nRealY	= 0;
		int nRealPosX	= 0;
		int nRealPosY	= 0;	
		nRealX	= pGuideCenter->m_pCurGuide->nCX*nowScreenWidth/100 +pGuideCenter->m_pCurGuide->nX ;
		nRealY	= pGuideCenter->m_pCurGuide->nCY*nowScreenHeight/100+pGuideCenter->m_pCurGuide->nY ;
		nRealPosX	= pGuideCenter->m_pCurGuide->nCX*nowScreenWidth/100 +pGuideCenter->m_pCurGuide->nSprX ;
		nRealPosY	= pGuideCenter->m_pCurGuide->nCY*nowScreenHeight/100+pGuideCenter->m_pCurGuide->nSprY ;

		ResetViewWindow();
		ResetChildren(nRealPosX,nRealPosY);		
	}
	else if(m_bWorked && pGuideCenter->m_pCurGuide	== NULL)
	{
		m_nTestTime	= 0;
		m_bWorked	= false;
		removeAllChildrenWithCleanup(false);
	}

	if(pGuideCenter->m_pCurGuide && pGuideCenter->m_pCurGuide->pForm)
	{
		if(pGuideCenter->m_pCurGuide->pForm->bCocosVisabel == false)
			setVisible(false);
		else
			setVisible(true);
	}
	else
		setVisible(true);	

	if(nowRateW && nowRateH){
		this->setAnchorPoint(CCPointZero);
		this->setScaleX(nowRateW);
		this->setScaleY(nowRateH);
	}
}


//----------------------------------
bool ControlLayer::init()
{
	//return true;
	CCLayer::init();
	scheduleUpdate();
	setTouchEnabled(true);

	m_pSprCtrlBack	= CCSprite::create("ui/MB_YAOGAN.png");
	m_pSprCtrlBack->retain();
	m_pSprCtrlBack->setPosition(ccp(149/POINT_PIX_CNT,126/POINT_PIX_CNT));

	addChild(m_pSprCtrlBack,10);
	
	m_ptDefaul.setPoint(150/POINT_PIX_CNT,130/POINT_PIX_CNT);
	m_pSprCtrlBtn	= CCSprite::create("ui/BT_YAOGAN.png");
	m_pSprCtrlBtn->retain();
	m_pSprCtrlBtn->setPosition(m_ptDefaul);
	addChild(m_pSprCtrlBtn,11);

	m_bClickHold	= false;
	m_pSprCtrlBtn->setOpacity(150);

	return true;
}

ControlLayer::~ControlLayer()
{
	m_pSprCtrlBack->release();
	m_pSprCtrlBtn->release();
}

void ControlLayer::update( float dt )
{
	CCLayer::update(dt);


}

void ControlLayer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 5, true);
}

bool ControlLayer::ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	if(isVisible() == false)
		return false;

	if(pMainInterface->m_pLockForm)
		return false;

	CCPoint	pt		= pTouch->getLocation();
	if(nowRateW && nowRateH)
	{
		pt.x	/= nowRateW;
		pt.y	/= nowRateH;
	}
	
	if(CalcDistance2(pt.x,pt.y,m_ptDefaul.x,m_ptDefaul.y) < 5000/POINT_PIX_CNT/POINT_PIX_CNT)
	{
		m_pSprCtrlBtn->stopAllActions();
		CCActionInterval*	pAct	= CCMoveTo::create(0.2f,pt);
		m_pSprCtrlBtn->runAction(CCEaseSineOut::create(pAct));

		m_nChgLen	= CalcDistance(pt.x,pt.y,m_ptDefaul.x,m_ptDefaul.y);
		m_ptChged	= ccp(pt.x-m_ptDefaul.x,pt.y-m_ptDefaul.y);
		m_bClickHold	= true;
		m_pSprCtrlBtn->setOpacity(255);
		return true;
	}
	return false;
}

void ControlLayer::ccTouchMoved( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	CCPoint	pt	= pTouch->getLocation();
	if(nowRateW && nowRateH)
	{
		pt.x	/= nowRateW;
		pt.y	/= nowRateH;
	}
	int nDisCur		= CalcDistance(pt.x,pt.y,m_ptDefaul.x,m_ptDefaul.y);
	int	nDisLimit	= 70/POINT_PIX_CNT;
	if(nDisCur > nDisLimit)
	{
		pt.x	= (pt.x - m_ptDefaul.x) * nDisLimit / nDisCur + m_ptDefaul.x;
		pt.y	= (pt.y - m_ptDefaul.y) * nDisLimit / nDisCur + m_ptDefaul.y;

		nDisCur	= nDisLimit;
	}
	m_nChgLen	= nDisCur;
	m_ptChged	= ccp(pt.x-m_ptDefaul.x,pt.y-m_ptDefaul.y);
	m_pSprCtrlBtn->stopAllActions();
	m_pSprCtrlBtn->setPosition(pt);
}

void ControlLayer::ccTouchEnded( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	//m_bClickHold	= false;
	m_pSprCtrlBtn->setOpacity(150);
	m_nChgLen		= 0;
	CCActionInterval*	pAct	= CCMoveTo::create(0.3f,m_ptDefaul);
	m_pSprCtrlBtn->runAction(CCEaseSineOut::create(pAct));

}



void ControlLayer::onEnter()
{
	CCLayer::onEnter();
	setAnchorPoint(CCPointZero);
	if(nowRateW && nowRateH)
	{
		this->setScaleX(nowRateW);
		this->setScaleY(nowRateH);
	}
}
