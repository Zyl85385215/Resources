#include "InterfaceLayer.h"
#include "GlobalFunction.h"
#include "CtrlObj.h"
#include "EffectSprite.h"
#include "MainMenu.h"
#include "SoundCenter.h"
#include "GuideCenter.h"
#include "MapMouse.h"

#ifdef WIN32
#include "debug_def.h"
#endif

//CreateSingleton(CCGLProgramManage);

using namespace cocos2d;


InterfaceLayer*		g_pLayerIF	= NULL;


CCSprite* GFun_InitSpriteByJPG( char* szFile )
{
	CCSprite* pSprTmp	= CCSprite::create ();
	CCImage* pMapImage	= new CCImage();
	CCTexture2D*	pText2D	= new CCTexture2D();
	pMapImage->initWithImageFile(szFile,CCImage::kFmtJpg);
	pText2D->initWithImage (pMapImage);
	pSprTmp->initWithTexture (pText2D);
	pSprTmp->getTexture()->setAliasTexParameters();
	pMapImage->release();
	pText2D->release();
	return pSprTmp;
}

void GFun_ChgSpriteByJPG(CCSprite* pSprTmp, char* szFile)
{
	CCImage* pMapImage	= new CCImage();
	CCTexture2D*	pText2D	= new CCTexture2D();
	pMapImage->initWithImageFile(szFile,CCImage::kFmtJpg);
	pText2D->initWithImage (pMapImage);
	pSprTmp->initWithTexture (pText2D);
	pMapImage->release();
	pText2D->release();
}

void Fun_SpriteSetRota(int nRota, void* pParam)
{
	CObjSSprite*	pSpr	= (CObjSSprite*)pParam;
	pSpr->setRotation(nRota);
}

void Fun_SpriteSetSpr(char* szFile, void* pParam)
{
	CObjSSprite*	pSpr	= (CObjSSprite*)pParam;
	if(strlen (szFile) == 0)
	{
		pSpr->init ();
		if(pSpr->m_pComp->byType == COMPONENT_BUTTON)
			pSpr->setContentSize(CCSize(pSpr->m_pComp->nW/POINT_PIX_CNT,pSpr->m_pComp->nH/POINT_PIX_CNT));
		return;
	}
	
	char	szTmp[256];
	if(pSpr->m_pComp->byType != COMPONENT_MAPGUIDE)
		sprintf(szTmp,"ui/%s",szFile);
	else
		sprintf(szTmp,"%s",szFile);
	
	if(strstr(szTmp,".jpg") != NULL)
	{
		CCImage* pMapImage	= new CCImage();
		CCTexture2D*	pText2D	= new CCTexture2D();
		pMapImage->initWithImageFile(szFile,CCImage::kFmtJpg);
		pText2D->initWithImage (pMapImage);
		pSpr->initWithTexture (pText2D);
		pMapImage->release();
		pText2D->release();
	}
	else
	{
		CCSpriteFrame* pSprFrm	= CCSpriteFrameCache::sharedSpriteFrameCache ()->spriteFrameByName(szTmp);
		if(pSprFrm)
		{
			//pSpr->init ();
			pSpr->initWithSpriteFrame (pSprFrm);
			pSpr->getTexture()->setAliasTexParameters();
		}
		else
		{
			if(pSpr->initWithFile (szTmp) == false)
				pSpr->init ();
		}
	}
	pSpr->m_pComp->nSprPixWidth = pSpr->getContentSize().width*POINT_PIX_CNT;
	pSpr->m_pComp->nSprPixHeight= pSpr->getContentSize().height*POINT_PIX_CNT;
	pSpr->setScale(1);
	pSpr->setAnchorPoint (ccp(0.5*pSpr->m_pComp->bAlignX,1-0.5*pSpr->m_pComp->bAlignY));
	pSpr->setOpacity(255);
}
void Fun_SpriteSetSprByScale(char* szFile, void* pParam,float scalex,float scaley)
{
	CObjSSprite*	pSpr	= (CObjSSprite*)pParam;
	if(strlen (szFile) == 0)
	{
		pSpr->init ();
		return;
	}

	char	szTmp[256];
	sprintf(szTmp,"ui/%s",szFile);
	CCSpriteFrame* pSprFrm	= CCSpriteFrameCache::sharedSpriteFrameCache ()->spriteFrameByName(szTmp);
	if(pSprFrm)
	{
		//pSpr->init ();
		pSpr->initWithSpriteFrame (pSprFrm);
		pSpr->getTexture()->setAliasTexParameters();
	}
	else
	{
		if(pSpr->initWithFile (szTmp) == false)
			pSpr->init ();
	}
	pSpr->setScaleX(scalex);
	pSpr->setScaleY(scaley);
	pSpr->setAnchorPoint (ccp(0.5*pSpr->m_pComp->bAlignX,1-0.5*pSpr->m_pComp->bAlignY));
}
void Fun_AddSpriteByTag(char* szFile,int nPosX, int nPosY,int nTag, void* pParam, int nScale, int nRota,int zOrder)
{
	CObjSSprite*	pSpr	= (CObjSSprite*)pParam;
	CCSprite* pSelSpr	= (CCSprite*)pSpr->getChildByTag (nTag);
	CCSize	size	= pSpr->getContentSize();
	//if(size.width == 0 || strlen(szFile) == 0)
	//{
	//	if(pSelSpr)
	//		pSpr->removeChild(pSelSpr,false);
	//	return;
	//}
	if(pSelSpr == NULL)
	{
		pSelSpr		= CCSprite::create();
		pSpr->addChild (pSelSpr,zOrder,nTag);
	}

	
	char	szTmp[256];
	sprintf(szTmp,"ui/%s",szFile);
	CCSpriteFrame* pSprFrm	= CCSpriteFrameCache::sharedSpriteFrameCache ()->spriteFrameByName(szTmp);
	if(pSprFrm)
	{
		pSelSpr->init ();
		pSelSpr->initWithSpriteFrame (pSprFrm);
		pSelSpr->getTexture()->setAliasTexParameters();
	}
	else
	{
		if(pSelSpr->initWithFile (szTmp) == false)
			pSelSpr->init ();
	}

	pSelSpr->setPosition (ccp(nPosX/POINT_PIX_CNT,nPosY/POINT_PIX_CNT));
	if(nRota)
		pSelSpr->setAnchorPoint (ccp(0,0.5));
	else
		pSelSpr->setAnchorPoint (ccp(0,0));

	if(nScale)
		pSelSpr->setScaleX(nScale/100.0);
	else
		pSelSpr->setScaleX(1);
	if(zOrder == 99)
	{
		if(nScale)
			pSelSpr->setScaleY(nScale/100.0);
		else
			pSelSpr->setScaleY(1);
	}

	int	nDifR	= nRota - pSelSpr->getRotation();
	if(abs(nDifR) > 3)
		pSelSpr->setRotation(nRota);
	//pSpr->setAnchorPoint (ccp(0.5*pSpr->m_pComp->bAlignX,1-0.5*pSpr->m_pComp->bAlignY));
}

void Fun_SpriteSetSelSpr(char* szFile,bool bInTop, void* pParam)
{
	CObjSSprite*	pSpr	= (CObjSSprite*)pParam;
	if(strlen (szFile) == 0)
	{
		pSpr->removeChildByTag (9999,true);
		return;
	}

	CCSprite* pSelSpr	= (CCSprite*)pSpr->getChildByTag (9999);
	if(pSelSpr == NULL)
	{
		pSelSpr		= CCSprite::create();
		pSpr->addChild (pSelSpr,(bInTop?1:-1),9999);
	}

	char	szTmp[256];
	sprintf(szTmp,"ui/%s",szFile);
	CCSpriteFrame* pSprFrm	= CCSpriteFrameCache::sharedSpriteFrameCache ()->spriteFrameByName(szTmp);
	if(pSprFrm)
	{
		//pSelSpr->init ();
		pSelSpr->initWithSpriteFrame (pSprFrm);
		pSelSpr->getTexture()->setAliasTexParameters();
	}
	else
	{
		if(pSelSpr->initWithFile (szTmp) == false)
			pSelSpr->init ();
	}

	pSelSpr->setPosition (CCPointZero);
	pSelSpr->setAnchorPoint (ccp(0,0));
}

void Fun_SpriteSetSprColor(int nR,int nG,int nB, void* pParam)
{
	CObjSSprite*	pSpr	= (CObjSSprite*)pParam;
	if(!pSpr->m_pComp->bLocked){
		pSpr->setColor(ccc3(nR,nG,nB));
		if(pSpr->pLabNum) pSpr->pLabNum->setColor(ccc3(nR,nG,nB));
	}
	else{

		pSpr->setColor(ccc3(nR/2,nG/2,nB/2));
		if(pSpr->pLabNum) pSpr->pLabNum->setColor(ccc3(nR/2,nG/2,nB/2));
	}

}
void Fun_CartoonSetSprColor(int nR,int nG,int nB, void* pParam)
{
	CObjCartoon*	pSpr	= (CObjCartoon*)pParam;
	if(!pSpr->m_pComp->bLocked){
		pSpr->setColor(ccc3(nR,nG,nB));
	}
	else{

		pSpr->setColor(ccc3(nR/2,nG/2,nB/2));
	}

}

void MySetShader(SHADER_TYPE byType,void* pParam){
	//CCostTime time1;
	cocos2d::CCSprite* pBtn	= (cocos2d::CCSprite*)pParam;
	switch (byType)
	{
	case SHADER_NULL:
		{
			CCGLProgram* pProgram = new CCGLProgram();
			pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, ccPositionTextureColor_frag);
			pBtn->setShaderProgram(pProgram);
			pProgram->release();
			break;
		}
	case SHADER_GRAY:
		{
			static GLchar * pszFragSource = 
				"#ifdef GL_ES \n \
				precision mediump float; \n \
				#endif \n \
				uniform sampler2D u_texture; \n \
				varying vec2 v_texCoord; \n \
				varying vec4 v_fragmentColor; \n \
				void main(void) \n \
				{ \n \
				// Convert to greyscale using NTSC weightings \n \
				float grey = dot(texture2D(u_texture, v_texCoord).rgb, vec3(0.299, 0.587, 0.114)); \n \
				gl_FragColor = vec4(grey, grey, grey, texture2D(u_texture, v_texCoord).a); \n \
				}";
			CCGLProgram* pProgram = new CCGLProgram();
			pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, pszFragSource);
			pBtn->setShaderProgram(pProgram);
			pProgram->release();
			break;
		}
	default:
		return;
	}
	//pBtn->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	pBtn->getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
	//pBtn->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	CHECK_GL_ERROR_DEBUG();

	pBtn->getShaderProgram()->link();
	CHECK_GL_ERROR_DEBUG();

	pBtn->getShaderProgram()->updateUniforms();
	CHECK_GL_ERROR_DEBUG();
}

void Fun_SetScale( void* pParam, float nScaleX, float nScaleY)
{
	CCSprite*	pSprite	= (CCSprite*)pParam;

	pSprite->setScaleX(nScaleX);
	pSprite->setScaleY(nScaleY);
}

void MySpriteSetOpacity( void* pParam, BYTE byVal)
{
	if(pParam == NULL)
		return;

	CObjSSprite*	pSprite	= (CObjSSprite*)pParam;

	pSprite->setOpacity(byVal);
	if(pSprite->pLabNum)
	{
		pSprite->pLabNum->setOpacity(byVal);
	}
}

void MyBtnTxtSet(char* szTxt,int nAlignment,void* pParam)
{

	CObjSSprite* pBtn	= (CObjSSprite*)pParam;
	if (!pBtn)
		return;

	if(szTxt == NULL || strlen (szTxt) == 0)
	{
		if(pBtn->pLabNum)
		{
			pBtn->removeChild (pBtn->pLabNum,true);
			pBtn->pLabNum	= NULL;
		}
		return;
	}

	if(pBtn->pLabNum == NULL)
	{
		pBtn->pLabNum	= CCLabelTTF::create (UTEXT_CHINE(""),DEFAULT_FONT,pBtn->m_pComp->m_nFontSize/POINT_PIX_CNT);

		
		pBtn->pLabNum->setColor (ccc3(pBtn->m_pComp->nR,pBtn->m_pComp->nG,pBtn->m_pComp->nB));

		CCSize	sz	= pBtn->getContentSize ();
		
		if(sz.height == 0)
		{//无图片的外部坐标位置不对,这边也只能偏移
			pBtn->pLabNum->setAnchorPoint (ccp(0.5*pBtn->m_pComp->bAlignX,1-0.5*pBtn->m_pComp->bAlignY));
			pBtn->pLabNum->setPosition (ccp(0,0));
		}
		else
		{
			pBtn->pLabNum->setAnchorPoint (ccp(0,0));
			pBtn->pLabNum->setPosition (ccp(0,0));
			//pBtn->pLabNum->setPosition (ccp(4/POINT_PIX_CNT,sz.height - pBtn->m_pComp->nH/POINT_PIX_CNT + 4/POINT_PIX_CNT));
		}

		if(pBtn->m_pComp->bAutoChgLine)
			pBtn->pLabNum->setDimensions (CCSize(pBtn->m_pComp->nW/POINT_PIX_CNT,pBtn->m_pComp->nH/POINT_PIX_CNT));

		pBtn->addChild(pBtn->pLabNum,20);
	}
	{
		//if(pBtn->pLabNum->getHorizontalAlignment() != nAlignment)
			pBtn->pLabNum->setHorizontalAlignment ((CCTextAlignment)nAlignment);
		//if(pBtn->pLabNum->getVerticalAlignment() != nAlignment)
			pBtn->pLabNum->setVerticalAlignment ((CCVerticalTextAlignment)nAlignment);
	}
	CCSize	sz	= pBtn->getContentSize ();

	if(sz.height != 0)
		pBtn->pLabNum->setAnchorPoint (ccp(0,0));
	else
		pBtn->pLabNum->setAnchorPoint (ccp(0.5*pBtn->m_pComp->bAlignX,1-0.5*pBtn->m_pComp->bAlignY));

	pBtn->pLabNum->setString (UTEXT_CHINE(szTxt));
	pBtn->m_pComp->nObjPixWidth	= pBtn->pLabNum->getContentSize().width*POINT_PIX_CNT;
	//pBtn->pLabNum->setVerticalAlignment ((CCVerticalTextAlignment)nAlignment);
	//pBtn->pLabNum->setHorizontalAlignment ((CCTextAlignment)nAlignment);
}

CObjSSprite::CObjSSprite( DComponent* pComp )
{
	char	szTmp[256];
	if(pComp->szSpr && strlen (pComp->szSpr))
	{
		if(pComp->m_pNodeBatch)
		{
			initWithTexture(((CCSpriteBatchNode*)pComp->m_pNodeBatch)->getTexture());
		}
		else
		{
			if(pComp->byType != COMPONENT_MAPGUIDE)
				sprintf(szTmp,"ui/%s",pComp->szSpr);
			else
				sprintf(szTmp,"%s",pComp->szSpr);
			if(strstr(szTmp,".jpg") != NULL)
			{

				CCImage* pMapImage	= new CCImage();
				CCTexture2D*	pText2D	= new CCTexture2D();
				pMapImage->initWithImageFile(szTmp,CCImage::kFmtJpg);
				pText2D->initWithImage (pMapImage);
				initWithTexture (pText2D);
				pMapImage->release();
				pText2D->release();
			}
			else
			{
				CCSpriteFrame* pSprFrm	= CCSpriteFrameCache::sharedSpriteFrameCache ()->spriteFrameByName(szTmp);
				if(pSprFrm)
				{
					//init ();
					initWithSpriteFrame (pSprFrm);
					getTexture()->setAliasTexParameters();
				}
				else
				{
					if(initWithFile (szTmp) == false)
						init ();
				}
			}
		}
 	}
 	else
 	{
 		init ();
 		if(pComp->byType == COMPONENT_BUTTON)
 			setContentSize(CCSize(pComp->nW/POINT_PIX_CNT,pComp->nH/POINT_PIX_CNT));
 	}

	//setAnchorPoint (ccp(0,1));
	setAnchorPoint (ccp(0.5*pComp->bAlignX,1-0.5*pComp->bAlignY));

	pLabNum			= NULL;
	m_pComp	= pComp;
	autorelease ();



	pComp->pParam	= this;
	pComp->fnSetViewSpr =	Fun_SpriteSetSpr;
	pComp->fnSetSprColor = Fun_SpriteSetSprColor;
	pComp->fnSetSelectSpr = Fun_SpriteSetSelSpr;
	pComp->fnAddSprByTag  = Fun_AddSpriteByTag;
	pComp->fnSetShader		= MySetShader;
	pComp->fnSetViewSprByScale = Fun_SpriteSetSprByScale;
	pComp->fnSetEditTxt		= MyBtnTxtSet;
	pComp->fnSetSprRotation	= Fun_SpriteSetRota;
	pComp->fnSetScale = Fun_SetScale;
	pComp->fnSetOpacity = MySpriteSetOpacity;

// 	if(pComp->szSpr && strlen(pComp->szSpr))
// 		Fun_SpriteSetSpr(pComp->szSpr,this);

	if(pComp->szCaption && strlen (m_pComp->szCaption))
		MyBtnTxtSet(pComp->szCaption,pComp->nCaptionAlignment,this);

	if(pComp->szSelectSpr && strlen (pComp->szSelectSpr))
		Fun_SpriteSetSelSpr(pComp->szSelectSpr,pComp->bSelSprTop,this);

	if(pComp->bEnable == false)
		MySetShader(pComp->bEnable?SHADER_NULL:SHADER_GRAY,this);

	Fun_SpriteSetSprColor(pComp->nR,pComp->nG,pComp->nB,this);

	for (int i = 0; i < pComp->m_pListASprTag->Count; i++)
	{
		_CDataAddSprTag* pSprT	= (_CDataAddSprTag*)pComp->m_pListASprTag->Items[i];
		Fun_AddSpriteByTag(pSprT->szSpr,pSprT->nPosX,pSprT->nPosY,pSprT->nTag,this,pSprT->nScalc,pSprT->nRota,pSprT->nZorder);
	}	
	setScaleX(m_pComp->m_fScaleX);
	setScaleY(m_pComp->m_fScaleY);
	m_pComp->nSprPixWidth = getContentSize().width*POINT_PIX_CNT;
	m_pComp->nSprPixHeight= getContentSize().height*POINT_PIX_CNT;
}

CObjSSprite::~CObjSSprite()
{
	m_pComp->fnSetViewSpr = NULL;
	m_pComp->fnSetSprColor = NULL;
	m_pComp->fnSetSelectSpr = NULL;
	m_pComp->fnSetEditTxt	= NULL;
	m_pComp->fnAddSprByTag	= NULL;
	m_pComp->fnSetShader = NULL;
	m_pComp->fnSetViewSprByScale = NULL;
	m_pComp->fnSetSprRotation	= NULL;
	m_pComp->fnSetScale			= NULL;
	m_pComp->pParam = NULL;
}

void CObjSSprite::setPosition( const CCPoint& pos )
{
	if (m_pComp&&m_pComp->m_bChgPosbySpr)
	{
		m_pComp->SetAlignmentX(pos.x*POINT_PIX_CNT);
		m_pComp->SetAlignmentY(-pos.y*POINT_PIX_CNT);
	}
	CCSprite::setPosition(pos);
}

void CObjSSprite::setOpacity( GLubyte opacity )
{
	CCSprite::setOpacity(opacity);
	if(pLabNum) pLabNum->setOpacity(opacity);
}

void MyBtnSetProgress(int nVal, int nMax, char* szTxt,void* pParam)
{
	CObjButton* pBtn	= (CObjButton*)pParam;

	if(nVal == 0)
	{
		if(pBtn->pProCD)
		{
			pBtn->removeChild (pBtn->pProCD,true);
			pBtn->pProCD	= NULL;
		}
		return;
	}

	if(pBtn->pProCD == NULL)
	{
		char* szPSpr	= pBtn->m_pComp->szSpr;
		if(strlen (szTxt))
		{
			szPSpr	= szTxt;
		}
		if(szPSpr)
		{
			char szTmp[256];
			sprintf(szTmp,"ui/%s",szPSpr);
			CCSprite* pSpr	= CCSprite::create ();
			CCSpriteFrame* pSprFrm	= CCSpriteFrameCache::sharedSpriteFrameCache ()->spriteFrameByName(szTmp);
			bool bRtl	= false;
			if(pSprFrm)
			{
				//pSpr->init();
				bRtl	= pSpr->initWithSpriteFrame (pSprFrm);
				pSpr->getTexture()->setAliasTexParameters();
			}
			else
				bRtl	= pSpr->initWithFile (szTmp);
			if(bRtl == false)
				return;
			if(strlen (szTxt) == 0)
				pSpr->setColor (ccc3(55,55,55));
			pBtn->pProCD	= CCProgressTimer::create (pSpr);
		}
		else
			return;

		pBtn->pProCD->setReverseProgress (true);
		pBtn->pProCD->setType(kCCProgressTimerTypeRadial);
		pBtn->pProCD->setBarChangeRate(ccp(0, 0));
		pBtn->pProCD->setMidpoint (ccp(0.5f,0.5f));
		if(nMax)
			pBtn->pProCD->setPercentage (nVal*100/nMax);
		pBtn->pProCD->setAnchorPoint (ccp(0.5,0.5));

		CCSize	sz	= pBtn->pProCD->getContentSize ();
			
		//int nPX	= (pBtn->m_pComp->nW/POINT_PIX_CNT - sz.width)/2;
		//int nPY	= (pBtn->m_pComp->nH/POINT_PIX_CNT - sz.height)/2;
		pBtn->pProCD->setPosition (ccp(sz.width/2,sz.height/2));

		pBtn->addChild(pBtn->pProCD,21);
	}
	if(nMax)
		pBtn->pProCD->setPercentage (nVal*100/nMax);

}

void Fun_BtnTouchMove(char* szFilePrev,char* szFileNext,void* pParam)
{
	CObjButton* pBtn	= (CObjButton*)pParam;
	CCPoint		ptCur	= pBtn->getPosition();
	CCSize		szCur(pBtn->m_pComp->nW/POINT_PIX_CNT,pBtn->m_pComp->nH/POINT_PIX_CNT);
	char	szTmp[256];

	if(pBtn->pSprCur == NULL)
	{
		pBtn->rectTouchM.setRect(ptCur.x-szCur.width/2,SCREEN_SIZE_H+(ptCur.y-szCur.height/2)-pBtn->m_pComp->pParent->nPosY/POINT_PIX_CNT,szCur.width,szCur.height);

		if(nowRateW && nowRateH){
			pBtn->rectTouchM.setRect (pBtn->rectTouchM.origin.x*nowRateW,pBtn->rectTouchM.origin.y*nowRateH,pBtn->rectTouchM.size.width*nowRateW,pBtn->rectTouchM.size.height*nowRateH);
		}

		pBtn->pSprCur	= CCSprite::create();
		sprintf(szTmp,"ui/%s",pBtn->m_pComp->szSpr);
		CCSpriteFrame* pSprFrm	= CCSpriteFrameCache::sharedSpriteFrameCache ()->spriteFrameByName(szTmp);
		if(pSprFrm)
		{
			//pBtn->pSprCur->init ();
			pBtn->pSprCur->initWithSpriteFrame (pSprFrm);
			pBtn->pSprCur->getTexture()->setAliasTexParameters();
		}
		else
		{
			if(pBtn->pSprCur->initWithFile (szTmp) == false)
				pBtn->pSprCur->init ();
		}
		pBtn->pSprCur->setAnchorPoint (ccp(0.5,0.5));

		pBtn->pSprCur->setPosition(ccp(0,0));
		pBtn->addChild(pBtn->pSprCur,99);
		pBtn->init();
	}

	if(strlen(szFilePrev))
	{
		if(pBtn->pSprPrev == NULL)
		{
			pBtn->pSprPrev	= CCSprite::create();

			sprintf(szTmp,"ui/%s",szFilePrev);
			CCSpriteFrame* pSprFrm	= CCSpriteFrameCache::sharedSpriteFrameCache ()->spriteFrameByName(szTmp);
			if(pSprFrm)
			{
				//pBtn->pSprPrev->init ();
				pBtn->pSprPrev->initWithSpriteFrame (pSprFrm);
				pBtn->pSprPrev->getTexture()->setAliasTexParameters();
			}
			else
			{
				if(pBtn->pSprPrev->initWithFile (szTmp) == false)
					pBtn->pSprPrev->init ();
			}
			pBtn->pSprPrev->setAnchorPoint (ccp(0.5,0.5));
			
			pBtn->pSprPrev->setPosition(ccp(-szCur.width,0));
			pBtn->addChild(pBtn->pSprPrev,99);
		}
		
	}
	else
	{
		if(pBtn->pSprPrev)
			pBtn->removeChild(pBtn->pSprPrev,true);
		pBtn->pSprPrev	= NULL;
	}

	if(strlen(szFileNext))
	{
		if(pBtn->pSprNext == NULL)
		{
			pBtn->pSprNext	= CCSprite::create();

			sprintf(szTmp,"ui/%s",szFileNext);
			CCSpriteFrame* pSprFrm	= CCSpriteFrameCache::sharedSpriteFrameCache ()->spriteFrameByName(szTmp);
			if(pSprFrm)
			{
				//pBtn->pSprNext->init ();
				pBtn->pSprNext->initWithSpriteFrame (pSprFrm);
				pBtn->pSprNext->getTexture()->setAliasTexParameters();
			}
			else
			{
				if(pBtn->pSprNext->initWithFile (szTmp) == false)
					pBtn->pSprNext->init ();
			}
			pBtn->pSprNext->setAnchorPoint (ccp(0.5,0.5));
			pBtn->pSprNext->setPosition(ccp(szCur.width,0));
			pBtn->addChild(pBtn->pSprNext,99);
		}
	}
	else
	{
		if(pBtn->pSprNext)
			pBtn->removeChild(pBtn->pSprNext,true);
		pBtn->pSprNext	= NULL;
	}
}

CObjButton::CObjButton( DComponent* pComp )
	:CObjSSprite(pComp)
{
	pSelectText	= NULL;
	m_dwTmClicked	= 0;
	m_dwLastClicked = 0;
	pProCD			= NULL;

	m_pComp->fnProgressSetInfo	= MyBtnSetProgress;
	m_pComp->fnSetTouchMoveSpr	= Fun_BtnTouchMove;
	//m_pComp->fnSetEditTxt		= MyBtnTxtSet;
	m_pComp->bEffChg		= true;

	pListEffs	= xnList::Create ();
	pListNums	= xnList::Create ();

	pSprPrev	= NULL;
	pSprNext	= NULL;
	pSprCur		= NULL;

	if(m_pComp->nProVal != m_pComp->nProMax)
	{
		if(m_pComp->szEdit && strlen (m_pComp->szEdit))
			MyBtnSetProgress(m_pComp->nProVal,m_pComp->nProMax,m_pComp->szEdit,this);
		else
			MyBtnSetProgress(m_pComp->nProVal,m_pComp->nProMax,m_pComp->szCaption,this);
	}
	//if(pComp->szCaption && strlen (m_pComp->szCaption))
	//	MyBtnTxtSet(pComp->szCaption,pComp->nCaptionAlignment,this);
}

CObjButton::~CObjButton()
{
	m_pComp->fnProgressSetInfo	= NULL;
	m_pComp->fnSetTouchMoveSpr	= NULL;
	//m_pComp->fnSetEditTxt		= NULL;
	if(pSelectText)
		CCTextureCache::sharedTextureCache()->removeTexture(pSelectText);
}

void CObjButton::EventCallBack( int nEvent,int x, int y, void* pParam )
{
	CObjButton* pBtn	= (CObjButton*)pParam;

	if(nEvent != INTERFACE_EVENT_UP)	//弹起事件不受影响
		if(pMainInterface->m_pLockForm && pMainInterface->m_pLockForm != pBtn->m_pComp->pParent
			&& pMainInterface->m_pLockForm->bZorde >= pBtn->m_pComp->pParent->bZorde)
			return;	//界面锁定功能

	if(pBtn->m_pComp->bLocked || pBtn->m_pComp->bClickLocked)
	{
		return;
	}

	if(!pBtn->m_pComp->bVisable || !pBtn->m_pComp->bEnable)
	{
		return;
	}

	if(pMainToopTip && pMainToopTip->m_pTipPopMenu && pMainToopTip->m_pTipPopMenu->m_bIsDel == false)
		pMainToopTip->m_pTipPopMenu->m_bRequestDel	= true;

	pMainToopTip->CloseAllInterfaceTip();

	static	InterfaceEvent	IEvent;

	IEvent.x		= x*POINT_PIX_CNT;
	IEvent.y		= y*POINT_PIX_CNT;
	IEvent.pComponent	= pBtn->m_pComp;
	static int	s_downX;
	static int	s_downY;
	switch(nEvent)
	{
	case 0:
		IEvent.event	= INTERFACE_EVENT_DOWN;
		s_downX	= x;
		s_downY	= y;
		pBtn->m_dwTmClicked		= xnGetTickCount ();
		//if(pBtn->pSelectText == NULL && pBtn->m_pComp->szSpr && strlen (pBtn->m_pComp->szSpr))
		//{
		//	char	szTmp[256];
		//	sprintf(szTmp,"ui/%s",pBtn->m_pComp->szSpr);
		//	strcpy(&szTmp[strlen (szTmp)-4],"_S.png");
		//	pBtn->pSelectText	= CCTextureCache::sharedTextureCache()->addImage (szTmp);
		//	if(pBtn->pSelectText)
		//		pBtn->pSelectText->retain ();
		//}

		//if(pBtn->pSelectText)
		//{
		//	CCRect rect = CCRectZero;
		//	rect.size = pBtn->pSelectText->getContentSize();
		//	pBtn->initWithTexture (pBtn->pSelectText,rect,false);
		//	//pBtn->setAnchorPoint (ccp(0,1));
		//	pBtn->setAnchorPoint (ccp(0.5*pBtn->m_pComp->bAlignX,1-0.5* pBtn->m_pComp->bAlignY));
		//}
		//else
		if(!pMainInterface->m_bLockBtnEff)
			pBtn->setColor (ccc3(pBtn->m_pComp->nR*3/5,pBtn->m_pComp->nG*3/5,pBtn->m_pComp->nB*3/5));
		break;
	case 1:
		{
			int nHandsign = g_pHandSign->AddPtCheck(x,y);
			if(nHandsign)
			{
				IEvent.event	= INTERFACE_EVENT_HANDSIGN+nHandsign;
				if(pBtn->m_pComp->eventFunc)
					pBtn->m_pComp->eventFunc (&IEvent,pBtn->m_pComp->pCbParam);
			}
			IEvent.event	= INTERFACE_EVENT_MOVE;
			if(pBtn->pSprCur)
			{
				if(abs(s_downX-x) < pBtn->rectTouchM.size.width)
				{//拖动超过一半就算选择好了,等下次
					if(pBtn->pSprPrev)
						pBtn->pSprPrev->setPositionX(-pBtn->rectTouchM.size.width-s_downX+x);
					if(pBtn->pSprCur)
						pBtn->pSprCur->setPositionX(-s_downX+x);
					if(pBtn->pSprNext)
						pBtn->pSprNext->setPositionX(pBtn->rectTouchM.size.width-s_downX+x);
				}
			}
		}

		break;
	case 2:
		if(pBtn->pSprCur)
		{
			int nChgVal	= s_downX-x;
			pBtn->pSprCur->setTag(nChgVal);

			int nPosY	= pBtn->pSprCur->getPositionY();
			if(nChgVal > pBtn->rectTouchM.size.width/2)
			{
				if(pBtn->pSprNext)
				{
					CCActionInterval* pMove	= CCMoveTo::create(0.3,ccp(0,nPosY));
					pBtn->pSprNext->runAction(CCEaseSineOut::create (pMove));

					pMove	= CCMoveTo::create(0.3,ccp(-pBtn->rectTouchM.size.width,nPosY));
					pBtn->pSprCur->runAction(CCEaseSineOut::create (pMove));
				}
				else
				{
					CCActionInterval* pMove	= CCMoveTo::create(0.3,ccp(0,nPosY));
					pBtn->pSprCur->runAction(CCEaseSineOut::create (pMove));
				}
			}
			else if(nChgVal < -pBtn->rectTouchM.size.width/2)
			{	
				if(pBtn->pSprPrev)
				{
					CCActionInterval* pMove	= CCMoveTo::create(0.3,ccp(0,nPosY));
					pBtn->pSprPrev->runAction(CCEaseSineOut::create (pMove));

					pMove	= CCMoveTo::create(0.3,ccp(pBtn->rectTouchM.size.width,nPosY));
					pBtn->pSprCur->runAction(CCEaseSineOut::create (pMove));
				}
				else
				{
					CCActionInterval* pMove	= CCMoveTo::create(0.3,ccp(0,nPosY));
					pBtn->pSprCur->runAction(CCEaseSineOut::create (pMove));
				}
			}
			else
			{
				if(pBtn->pSprPrev)
				{
					CCActionInterval* pMove	= CCMoveTo::create(0.3,ccp(-pBtn->rectTouchM.size.width,nPosY));
					pBtn->pSprPrev->runAction(CCEaseSineOut::create (pMove));
				}
				if(pBtn->pSprNext)
				{
					CCActionInterval* pMove	= CCMoveTo::create(0.3,ccp(pBtn->rectTouchM.size.width,nPosY));
					pBtn->pSprNext->runAction(CCEaseSineOut::create (pMove));
				}
				CCActionInterval* pMove	= CCMoveTo::create(0.3,ccp(0,nPosY));
				pBtn->pSprCur->runAction(CCEaseSineOut::create (pMove));
			}
		}


		IEvent.event	= INTERFACE_EVENT_UP;
		if(pBtn->pSelectText)
		{
			Fun_SpriteSetSpr (pBtn->m_pComp->szSpr,pBtn);
		}
		else
			pBtn->setColor (ccc3(pBtn->m_pComp->nR,pBtn->m_pComp->nG,pBtn->m_pComp->nB));

		break;
	}

	if(pBtn->m_pComp->eventFunc)
	{
		pBtn->m_pComp->eventFunc (&IEvent,pBtn->m_pComp->pCbParam);
		if(nEvent == INTERFACE_EVENT_UP)
		{
			if(xnGetTickCount() - pBtn->m_dwTmClicked < 1000 && abs(x-s_downX) < 30 && abs(y-s_downY) < 30)
			{
				if (xnGetTickCount() - pBtn->m_dwLastClicked < 500)
				{
					IEvent.event	= INTERFACE_EVENT_DOUBLECLICK;
					pBtn->m_pComp->eventFunc (&IEvent,pBtn->m_pComp->pCbParam);
					pBtn->m_dwLastClicked = 0;
				}

				IEvent.event	= INTERFACE_EVENT_CLICK;
				if(!pMainInterface->m_bLockBtnEff && pBtn->m_pComp->bSound)
					g_pSoundCenter->PlayerEffect ("Sound/ui_click");

				pBtn->m_pComp->eventFunc (&IEvent,pBtn->m_pComp->pCbParam);
				pBtn->m_dwLastClicked = xnGetTickCount();

// 				if(pGuideCenter && pGuideCenter->m_pCurGuide)
// 				{
// 					if(pGuideCenter->m_pCurGuide->pForm)
// 					{
// 						
// 						if(pGuideCenter->m_pCurGuide->pForm->bCocosVisabel)
// 						{
// 							//pGuideCenter->NextGuide();
// 						}
// 					}
// 					else
// 					{
// 						pGuideCenter->NextGuide();
// 					}
// 				}
				//if(pMainToopTip)
				//	pMainToopTip->CheckGuideTip(pBtn->m_pComp->pParent->szName,pBtn->m_pComp->szName);

			}
			pBtn->m_dwTmClicked	= 0;
			 ClientLuaManager::sharedHD()->ButtonClick(pBtn->m_pComp->pParent,pBtn->m_pComp);

		}
	}
}

void CObjButton::Update( )
{
	if(pSprCur)
	{
		if(pSprCur->numberOfRunningActions() == 0 && pSprCur->getTag() != -1)
		{
			static	InterfaceEvent	IEvent;
			int		nChgVal	= pSprCur->getTag();
			bool	bNoChg	= false;
			if(nChgVal > rectTouchM.size.width/2)
				IEvent.event	= INTERFACE_EVENT_TOUCHMOVE_NEXT;
			else if(nChgVal < -rectTouchM.size.width/2)
				IEvent.event	= INTERFACE_EVENT_TOUCHMOVE_PREV;
			else
				bNoChg	= true;

			removeChild(pSprCur,true);
			if(pSprPrev)
				removeChild(pSprPrev,true);
			if(pSprNext)
				removeChild(pSprNext,true);
			pSprCur	= NULL;
			pSprPrev	= NULL;
			pSprNext	= NULL;


			Fun_SpriteSetSpr(m_pComp->szSpr,this);
			
			if(!bNoChg)
			{
				IEvent.pComponent	= m_pComp;
				if(m_pComp->eventFunc)
					m_pComp->eventFunc (&IEvent,m_pComp->pCbParam);
			}

		}
	}

	for (int i = pListNums->Count-1; i >= 0; i--)
	{
		CCNumberSprite* pEff	= (CCNumberSprite*)pListNums->Items[i];
		if(pEff->CheckEnd ())
		{
			removeChild (pEff,true);
			pListNums->Delete (i);
		}
	}

	while(m_pComp->pListEffNum->Count)
	{
		DCompNumEff* pNum	= (DCompNumEff*)m_pComp->pListEffNum->Delete (0);

		CCNumberSprite*	pNumSpr	= CCNumberSprite::create (pNum->nNum,pNum->bType);
		CCPoint	pt	= ccp(m_pComp->nW/POINT_PIX_CNT/2,m_pComp->nH/POINT_PIX_CNT/2);
		pNumSpr->setPosition (pt);
		addChild (pNumSpr,2);
		switch(pNum->bVAct)
		{
		case 0:
			{
				CCActionInterval* pJump1	= CCJumpTo::create (0.5/g_byWarSpeed,ccp(pt.x+40/POINT_PIX_CNT,pt.y-80/POINT_PIX_CNT),80,1);
				pNumSpr->runAction( pJump1);
			}
			break;
		case 1:
			{
				CCActionInterval* pMove		= CCMoveTo::create (0.5/g_byWarSpeed,ccp(pt.x,pt.y+80/POINT_PIX_CNT));
				//CCActionInterval* pAlpha	= CCFadeTo::create (0.5,100);
				//CCFiniteTimeAction* seq = CCSpawn::create (pMove,pAlpha,NULL);
				pNumSpr->runAction(pMove);
			}
			break;
		case 2:
			{
				CCActionInterval* pScale	= CCScaleTo::create (0.1/g_byWarSpeed,1.5,1.5);
				CCActionInterval* pDelay	= CCDelayTime::create (0.2/g_byWarSpeed);
				CCActionInterval* pScale1	= CCScaleTo::create (0.1/g_byWarSpeed,1,1);
				CCFiniteTimeAction* seq = CCSequence::create (pScale,pDelay,pScale1,NULL);
				pNumSpr->runAction(seq);
			}
			break;
		}
		pListNums->Add (pNumSpr);

		delete pNum;
	}


	for (int i = pListEffs->Count-1; i >= 0; i--)
	{
		CCEffectSprite* pEff	= (CCEffectSprite*)pListEffs->Items[i];
		if(pEff->CheckEnd ())
		{
			removeChild (pEff,false);
			pListEffs->Delete (i);
		}
	}
	if(m_pComp->bEffChg)
	{
		//WORD	wCheckEff[3];
		//memcpy(wCheckEff,m_pComp->wEffect,6);
		//删除已经被上层移除的光效
		xnList* pTmpList	= xnList::Create ();
		for (int j = 0; j < m_pComp->pListEff->Count; j++)
		{
			pTmpList->Add (m_pComp->pListEff->Items[j]);
		}

		bool bFind	= false;
		for (int i = pListEffs->Count-1; i >= 0; i--)
		{
			bFind	= false;
			CCEffectSprite* pEff	= (CCEffectSprite*)pListEffs->Items[i];
			for (int j = pTmpList->Count-1; j >= 0; j--)
			{
				DCompEffect* pDE	= (DCompEffect*)pTmpList->Items[j];
				if(pEff->GetEffID () == pDE->nEffId && pEff->m_nSPID == pDE->nPos && pDE->nPos!=-1)
				{
					bFind = true;
					pTmpList->Delete (j);
					break;
				}
			}
			if(!bFind && pEff->m_dwEndTick == 0)
			{
				removeChild (pEff,false);
				pListEffs->Delete (i);
			}
		}
		
		//播放新增的光效
		for (int j = 0; j < pTmpList->Count; j++)
		{
			DCompEffect* pDE	= (DCompEffect*)pTmpList->Items[j];
			EffectObj*	pEO	= EffectCenter::GetInstance ().GetEffObj (pDE->nEffId);

			CCEffectSprite* pEffSpr	= CCEffectSprite::create (pEO);
			if(pEffSpr)
			{
				pEffSpr->m_nSPID	= pDE->nPos;
				addChild (pEffSpr/*,pEffSpr->m_nSPID*/);
				if(/*pEO->m_nRunType == 5 &&*/ pDE->pOthers)
				{
					pEffSpr->setRotation (pDE->pOthers->wAngle);
						
					pEffSpr->setPosition (ccp(pDE->pOthers->nSX/POINT_PIX_CNT,-pDE->pOthers->nSY/POINT_PIX_CNT));
					if(pDE->pOthers->nEX != pDE->pOthers->nSX ||
						pDE->pOthers->nEY != pDE->pOthers->nSY)
					{
						CCActionInterval* goMove = CCMoveTo::create(pDE->pOthers->nTime/1000.0, ccp(pDE->pOthers->nEX/POINT_PIX_CNT,-pDE->pOthers->nEY/POINT_PIX_CNT) );
						pEffSpr->runAction( goMove);
					}
				}
				else{
					CCSize	szCont	= getContentSize();
					pEffSpr->setPosition (ccp(szCont.width/2,szCont.height/2));
				}

				pListEffs->Add (pEffSpr);
			}

			if(/*pDE->pOthers->nTime ||*/ pDE->nPos == -1)
			{//时间状态直接删除上层,这边内部计时
				if(pEffSpr)
				{
					if(pDE->pOthers && pDE->pOthers->nTime)
						pEffSpr->m_dwEndTick	= xnGetTickCount ()+pDE->pOthers->nTime;
					else
						pEffSpr->m_dwEndTick	= -1;
				}
				m_pComp->pListEff->Remove (pDE);
				delete pDE;
			}
			else if(pEO && pEO->m_nLoopTime != 0)
			{
				m_pComp->pListEff->Remove (pDE);
				delete pDE;
			}
		}
		pTmpList->Free ();
		m_pComp->bEffChg	= false;
	}

}

void CObjButton::visit( void )
{
	if(pSprNext || pSprPrev)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor (rectTouchM.origin.x*POINT_PIX_CNT,rectTouchM.origin.y*POINT_PIX_CNT,rectTouchM.size.width*POINT_PIX_CNT,rectTouchM.size.height*POINT_PIX_CNT+50);
		CObjSSprite::visit();

		glDisable(GL_SCISSOR_TEST);
	}
	else
		CObjSSprite::visit();
}

FrameLayer::FrameLayer():m_nSelect(0),m_bLoop(false),m_bMove(false)
{
	m_bZorde	= 10;
	m_bLockX	= false;
	m_bLockY	= false;
	m_nDifStep	= 0;
	m_bSplitTouch	= false;
	m_bDisalbeTouch = false;
	autorelease ();
}

FrameLayer::~FrameLayer()
{

}

cocos2d::CCPoint FrameLayer::PtIF2Cocos( int nX, int nY )
{
	return ccp(nX/POINT_PIX_CNT,-nY/POINT_PIX_CNT);
}

CCNode* FrameLayer::AddDComp( DComponent* pComp )
{
	CCNode* pOBj	= NULL;
	CCPoint	pt	= PtIF2Cocos (pComp->GetAlignmentX (),pComp->GetAlignmentY ());
	switch(pComp->byType)
	{
	case COMPONENT_BUTTON:
		{
			pOBj	= new CObjButton(pComp);

			if(pComp->m_pNodeBatch == NULL)
				addChild (pOBj,pComp->m_nOrder,(int)pComp);
			else
				((CCSpriteBatchNode*)(pComp->m_pNodeBatch))->addChild(pOBj,pComp->m_nOrder,(int)pComp);
			pOBj->setPosition (pt);

			CCPoint ptEv	= PtIF2Cocos (pComp->nX,pComp->nY);
			if(pComp->bVisable)
				AddRegEvent (CCRect(ptEv.x,ptEv.y-pComp->nH/POINT_PIX_CNT,pComp->nW/POINT_PIX_CNT,pComp->nH/POINT_PIX_CNT),pOBj,CObjButton::EventCallBack,pComp->pParent?(pComp->pParent->bZorde*100+pComp->m_nOrder):0);
			else
				AddRegEvent (CCRect(SCREEN_SIZE_W*2000,ptEv.y-pComp->nH/POINT_PIX_CNT,pComp->nW/POINT_PIX_CNT,pComp->nH/POINT_PIX_CNT),pOBj,CObjButton::EventCallBack,pComp->pParent?(pComp->pParent->bZorde*100+pComp->m_nOrder):0);
		}
		break;
	case COMPONENT_SSPR:
		if(1||pComp->szSpr != NULL)
		{
			pOBj	= new CObjSSprite(pComp);
			if(pComp->m_pNodeBatch == NULL)
				addChild (pOBj,pComp->m_nOrder,(int)pComp);
			else
				((CCSpriteBatchNode*)(pComp->m_pNodeBatch))->addChild(pOBj,pComp->m_nOrder,(int)pComp);
			//addChild (pOBj,pComp->m_nOrder,(int)pComp);
			pOBj->setPosition (pt);
		}
		else//文字
		{
			pOBj	= new CObjCaption(pComp);
			addChild (pOBj,pComp->m_nOrder,(int)pComp);
			pOBj->setPosition (pt);
		}
		break;
	case COMPONENT_PROGRESS:
		{
			pOBj	= new CObjProgress(pComp);
			addChild (pOBj,pComp->m_nOrder,(int)pComp);
			pOBj->setPosition (pt);
		}
		break;
	case COMPONENT_EDIT:
		{
#ifdef WIN321111
			pOBj	= new CNewTextInput(pComp);
			addChild (pOBj,pComp->m_nOrder,(int)pComp);
			//pOBj->setPosition (pt);
			pOBj->setPosition(ccp(-pComp->pParent->nPosX/POINT_PIX_CNT,pComp->pParent->nPosY/POINT_PIX_CNT-SCREEN_SIZE_H));
#else
			pOBj	= new CObjTextInput(pComp);
			addChild (pOBj,pComp->m_nOrder,(int)pComp);
			pOBj->setPosition (pt);
#endif
		}
		break;
	case COMPONENT_CARTOON:
		{
			pOBj	= new CObjCartoon(pComp);
			addChild (pOBj,pComp->m_nOrder,(int)pComp);
			pOBj->setPosition (pt);
		}
		break;
	case COMPONENT_LINK:
		{
			pOBj	= new CSpecialText(pComp->pSTextCenter);
			((CSpecialText*)pOBj)->m_bZorde	= m_bZorde;
			CCRect	rect;
			rect.setRect ((pComp->nX+pComp->pParent->nPosX)/POINT_PIX_CNT,SCREEN_SIZE_H-(pComp->pParent->nPosY+pComp->nY+pComp->nH)/POINT_PIX_CNT,pComp->nW/POINT_PIX_CNT,pComp->nH/POINT_PIX_CNT);
			((CSpecialText*)pOBj)->SetRealRect (rect);
			addChild (pOBj,pComp->m_nOrder,(int)pComp);
			//if(nowScreenWidth<640||nowScreenHeight<960){
			//	rect.setRect (rect.origin.x*nowRateW,rect.origin.y*nowRateH,rect.size.width*nowRateW,rect.size.height*nowRateH);
			//}
			pOBj->setContentSize (CCSize(pComp->nW/POINT_PIX_CNT,pComp->nH/POINT_PIX_CNT));
			pOBj->setPosition (pt);

			((CSpecialText*)pOBj)->nDefaulY	= pOBj->getPositionY ();
		}
		break;
	case COMPONENT_SLAB:
		{
			pOBj	= new CBMFCaption(pComp);
			addChild (pOBj,pComp->m_nOrder,(int)pComp);
			pOBj->setPosition (pt);
		}
		break;
	case COMPONENT_MAPGUIDE:
		{
			pOBj	= new CSpBtnSprite(pComp);
			addChild (pOBj,pComp->m_nOrder,(int)pComp);
			
			pOBj->setPosition (pt);

			CCPoint ptEv	= PtIF2Cocos (pComp->nX,pComp->nY);
			CCRect	rect;
			rect.setRect ((pComp->nX+pComp->pParent->nPosX)/POINT_PIX_CNT,SCREEN_SIZE_H-(pComp->pParent->nPosY+pComp->nY+pComp->nH)/POINT_PIX_CNT,pComp->nW/POINT_PIX_CNT,pComp->nH/POINT_PIX_CNT);
			if(nowRateW && nowRateH)
				((CSpBtnSprite*)pOBj)->rectView.setRect (rect.origin.x*nowRateW,rect.origin.y*nowRateH,rect.size.width*nowRateW,rect.size.height*nowRateH);
			else
				((CSpBtnSprite*)pOBj)->rectView	= rect;
			AddRegEvent (CCRect(ptEv.x,ptEv.y-pComp->nH/POINT_PIX_CNT,pComp->nW/POINT_PIX_CNT,pComp->nH/POINT_PIX_CNT),pOBj,CSpBtnSprite::EventCallBack,pComp->pParent?(pComp->pParent->bZorde*100+pComp->m_nOrder):0);
		}
		break;
	}
	return pOBj;
}

bool FrameLayer::ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	if(m_bDisalbeTouch)
		return false;
	if (m_bSplitTouch && m_bMove)
	{
		return false;
	}
	if(g_bIFLayerClicked)
		return false;
	
	CCPoint	pt	= pTouch->getLocation ();
	if(!m_rectReal.containsPoint (pt))
		return false;


	CCPoint	ptCur	= convertTouchToNodeSpace(pTouch);

	ptDown		= getPosition ();
	dwDownTick	= xnGetTickCount ();

	for (int i = m_pListBtnRect->Count-1; i >= 0; i--)
	{
		MyEventRect* pERect	= (MyEventRect*)m_pListBtnRect->Items[i];
		if(pERect->rect.containsPoint (ptCur))
		{
			stopAllActions ();
			pERect->fnEventCB(0,ptCur.x-pERect->rect.getMinX (),pERect->rect.getMaxY ()-ptCur.y,pERect->pParam);
			m_pCurERect	= pERect;
			m_bClicked = true;

			g_bIFLayerClicked	= true;
			
			return true;
		}
	}
	if (fnStepEventFunc)
	{
		fnStepEventFunc(STEP_EVENT_BEGIN);
	}
	m_pCurERect	= NULL;
	m_bClicked	= true;
	g_bIFLayerClicked	= true;
	stopAllActions ();
	return true;
}

void FrameLayer::visit( void )
{

	glEnable(GL_SCISSOR_TEST);
	if(getParent()->numberOfRunningActions())
	{
		glScissor (m_rectReal.origin.x*POINT_PIX_CNT,m_rectReal.origin.y*POINT_PIX_CNT,1,1);
	}
	else
		glScissor (m_rectReal.origin.x*POINT_PIX_CNT,m_rectReal.origin.y*POINT_PIX_CNT,m_rectReal.size.width*POINT_PIX_CNT,m_rectReal.size.height*POINT_PIX_CNT);
	
	MyDragLayer::visit();

	glDisable(GL_SCISSOR_TEST);
}

void FrameLayer::ccTouchMoved( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	if(m_pCurERect)
	{
		CCPoint	ptCur	= convertTouchToNodeSpace(pTouch);
		m_pCurERect->fnEventCB(1,ptCur.x-m_pCurERect->rect.getMinX (),m_pCurERect->rect.getMaxY ()-ptCur.y,m_pCurERect->pParam);
	}
	if(!m_bClicked)
		return;
	if(pGuideCenter && pGuideCenter->m_pCurGuide && pGuideCenter->m_pCurGuide->pForm && pGuideCenter->m_pCurGuide->pForm->bCocosVisabel)
		return;
	CCPoint	ptDiff	= pTouch->getDelta ();
	CCPoint	ptCur	= getPosition ();

	

	CCRect	rect	= m_rectLimit;
	if(!m_bLimitRect)
	{
		rect.setRect(m_rectLimit.getMinX()-m_rectReal.size.width/4-5,m_rectLimit.getMinY()-m_rectReal.size.height/4-5,
			m_rectLimit.size.width+m_rectReal.size.width/2+10,m_rectLimit.size.height+m_rectReal.size.height/2+10);
	}
 	else
 	{
 		rect.setRect(m_rectLimit.getMinX()-5,m_rectLimit.getMinY()-5,
 			m_rectLimit.size.width+10,m_rectLimit.size.height+10);
 	}
	if(rect.containsPoint (ptCur)	== false)
	{
		return;
	}
	CCPoint ptRB	= ccp(ptCur.x + m_rectReal.size.width,ptCur.y-m_rectReal.size.height);
	if(rect.containsPoint (ptRB)	== false)
	{
		return;
	}

	if(m_bLockX)
		ptDiff.x	= 0;
	if(m_bLockY)
		ptDiff.y	= 0;
	if (m_bLoop)
	{
		if(ptDiff.x>m_nDifStep) ptDiff.x = m_nDifStep;
	}
	
	//ptDiff.x = ptDiff.x/nowRateW;
	//ptDiff.y = ptDiff.y/nowRateH;

	
	MovePosition( ccpAdd(ptCur, ptDiff) );
	
}

void FrameLayer::ccTouchEnded( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	g_bIFLayerClicked	= false;
	CCPoint	ptCur	= getPosition ();

	CCPoint ptDiff	= ccpSub (ptCur,ptDown);
	int nChgVal		= ptDiff.x - ptDiff.y;
	if(m_pCurERect)
	{
		CCPoint	ptTmp	= convertTouchToNodeSpace(pTouch);
		m_pCurERect->fnEventCB(2,ptTmp.x-m_pCurERect->rect.getMinX ()+ptDiff.x,m_pCurERect->rect.getMaxY ()-ptTmp.y+ptDiff.y,m_pCurERect->pParam);
		m_pCurERect	= NULL;
	}
	//if(!m_bClicked)
	//	return;
	m_bClicked	= false;


	bool	bNeedMove	= false;
	if(1)	//需要动态减速效果
	{
		int	dwTickDiff	= xnGetTickCount () - dwDownTick;
		if(dwTickDiff)
		{
			int nChgPer	= nChgVal*POINT_PIX_CNT/dwTickDiff;
			if(nChgPer)
			{//快速拖动再移动一倍距离
				bNeedMove	= true;
				if(myabs(ptDiff.x) > m_rectReal.size.width/3)
					ptDiff.x	= ptDiff.x/myabs(ptDiff.x)*m_rectReal.size.width/3;
				if(myabs(ptDiff.y) > m_rectReal.size.height/3)
					ptDiff.y	= ptDiff.y/myabs(ptDiff.y)*m_rectReal.size.height/3;

				ptCur = ccpAdd (ptCur,ptDiff);
			}
		}

	}

	SetStopPositon(ptCur, bNeedMove);
}


void FrameLayer::SetStopPositon(cocos2d::CCPoint& ptCur, bool bNM, bool bSound,float fDuration)
{
	bool	bNeedMove	= bNM;
	int	nWidth	= m_rectReal.size.width;
	int nHeight	= m_rectReal.size.height;
	if(nowRateW && nowRateH)
	{
		nWidth	/= nowRateW;
		nHeight	/= nowRateH;
	}

	if(/*nChgVal < 0 && */m_rectLimit.containsPoint (ptCur)	== false&&!m_bLoop)
	{
		bNeedMove	= true;
		ptCur.x	= m_rectLimit.getMinX ();
		ptCur.y	= m_rectLimit.getMaxY ();
	}
	CCPoint ptRB	= ccp(ptCur.x + nWidth,ptCur.y-nHeight);
	if(/*nChgVal > 0 &&*/ m_rectLimit.containsPoint (ptRB)	== false&&!m_bLoop)
	{
		bNeedMove	= true;
		ptCur.x	= (m_rectLimit.getMaxX () - nWidth);///nowRateW;
		ptCur.y	= (m_rectLimit.getMinY () + nHeight);///nowRateH;
	}
	int nStep = 0;
	if(m_nDifStep)
	{
		int nStartX	= m_rectLimit.getMaxX () - nWidth;
		int nStartY	= m_rectLimit.getMinY () + nHeight;
		float nowX,nowY;
		getPosition(&nowX,&nowY);
		int	nDifX	= (nStartX - (int)ptCur.x)%m_nDifStep;
		int	nDifY	= ((int)ptCur.y - nStartY)%m_nDifStep;
		if((nDifX > m_nDifStep/4&&(-m_nSelect*m_nDifStep+ nStartX)>(int)ptCur.x)
			||(nDifX > m_nDifStep*3/4&&(-m_nSelect*m_nDifStep+ nStartX)<(int)ptCur.x)){
			nDifX	-= m_nDifStep;
		}
		if(nDifY > m_nDifStep/2)
			nDifY	-= m_nDifStep;

		bNeedMove	= true;
		ptCur.x	+= nDifX;
		ptCur.y	-= nDifY;

		if(m_bLockX){
			//nStep = -ptCur.y/m_nDifStep;
			nStep = (ptCur.y-nStartY+m_nDifStep/10)/m_nDifStep;
		}else{
			nStep = (-ptCur.x+nStartX+m_nDifStep/10)/m_nDifStep;
// 			if (-ptCur.x+nStartX-(int)(-ptCur.x+nStartX)>0.9f)
// 			{
// 				nStep++;
// 			}
		}
		m_nSelect = nStep;
	}

	if(bNeedMove)
	{
		CCActionInterval* pMove	= CCMoveTo::create(fDuration,ptCur);
		//MovePosition(ptCur);
		CCAction * pCallback = CCCallFuncND::actionWithTarget(this, callfuncND_selector(FrameLayer::ActionCallBack),(void*)nStep); 
		CCFiniteTimeAction* pRun	= CCSequence::create (CCEaseSineOut::create (pMove),pCallback,NULL);
		runAction(pRun);
		CCPoint ptPos	= getPosition();
		int	nDifX	= ptPos.x - ptCur.x;
		int	nDifY	= ptPos.y - ptCur.y;
		if(abs(nDifX) + abs(nDifY) > 30/POINT_PIX_CNT)
		{
			m_bMove = true;
			if(bSound)
				g_pSoundCenter->PlayerEffect("Sound/slide");
		}
	}
}

void FrameLayer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, -m_bZorde, true);
}

void FrameLayer::ActionCallBack( cocos2d::CCNode *pNode,void* pParam )
{
	if (fnStepEventFunc)
	{
		fnStepEventFunc((int)pParam);
	}
	m_bMove = false;
}

void FrameLayer::Update( )
{
// 	if (m_bLoop)
// 	{
// 		CCPoint	ptCur	= getPosition ();
// 		int nStartX	= m_rectLimit.getMaxX () - m_rectReal.size.width;
// 		//if (!m_bLockX&&(ptCur.x-nStartX)>m_rectLimit.size.width/2&&m_bLoop)
// 		if (!m_bLockX&&(ptCur.x+m_rectLimit.size.width)<(nStartX+m_rectReal.size.width+m_nDifStep/10))
// 		{
// 			ptCur.x += (m_rectLimit.size.width/2-25);
// 			setPositionX(ptCur.x);
// 			CCLOG("1131123111");
// 		}
// 		else if (!m_bLockX&&(ptCur.x-nStartX)>-m_nDifStep)
// 		{
// 			ptCur.x -= (m_rectLimit.size.width/2-25);
// 			setPositionX(ptCur.x);
// 			CCLOG("1131123");
// 		}
// 	}
}
void FrameLayer::setColor(const ccColor3B& var)
{
	m_tColor = var;
	if (m_pChildren && m_pChildren->count() != 0)
	{
		CCObject* child;
		CCARRAY_FOREACH(m_pChildren, child)
		{
			CCSprite* pNode = (CCSprite*) child;
			if (pNode)
			{
				pNode->setColor(m_tColor);
			}
		}
	}
}
const ccColor3B& FrameLayer::getColor()
{
	return m_tColor;
}
void FrameLayer::setOpacity(GLubyte var)
{
	m_cOpacity = var;

	if (m_pChildren && m_pChildren->count() != 0)
	{
		CCObject* child;
		CCARRAY_FOREACH(m_pChildren, child)
		{
			CCNode* pNode = (CCNode*) child;
			if (pNode)
			{
				CCRGBAProtocol *pRGBAProtocol = dynamic_cast<CCRGBAProtocol*>(pNode);
				if (pRGBAProtocol)
				{
					pRGBAProtocol->setOpacity(m_cOpacity);
				}
			}
		}
	}
}
GLubyte FrameLayer::getOpacity()
{
	return m_cOpacity;
}
void FrameLayer::setOpacityModifyRGB(bool var)
{
	m_bIsOpacityModifyRGB = var;
	if (m_pChildren && m_pChildren->count() != 0)
	{
		CCObject* child;
		CCARRAY_FOREACH(m_pChildren, child)
		{
			CCNode* pNode = (CCNode*) child;
			if (pNode)
			{
				CCRGBAProtocol *pRGBAProtocol = dynamic_cast<CCRGBAProtocol*>(pNode);
				if (pRGBAProtocol)
				{
					pRGBAProtocol->setOpacityModifyRGB(m_bIsOpacityModifyRGB);
				}
			}
		}
	}
}
bool FrameLayer::isOpacityModifyRGB()
{
	return m_bIsOpacityModifyRGB;
}

void FrameLayer::setPosition( const CCPoint& pos )
{
	if (m_bLoop)
	{
		CCPoint ptCur = pos;
		int nStartX	= m_rectLimit.getMaxX () - m_rectReal.size.width;
		if (!m_bLockX&&(ptCur.x+m_rectLimit.size.width)<(nStartX+m_rectReal.size.width/*+m_nDifStep/10*/))
		{
			ptCur.x += (m_rectLimit.size.width/2-25);
			//setPositionX(ptCur.x);
			CCLOG("1131123111");
		}
		else if (!m_bLockX&&(ptCur.x-nStartX)>-m_nDifStep)
		{
			ptCur.x -= (m_rectLimit.size.width/2-25);
			//setPositionX(ptCur.x);
			CCLOG("1131123");
		}
		MyDragLayer::setPosition(ptCur);
	}else{

		MyDragLayer::setPosition(pos);
	}
}

void FrameLayer::setPosition( float x, float y )
{
	MyDragLayer::setPosition(x,y);
}

FormLayer::FormLayer(DForm* pFD)
{
	pFD->pFormLayer = this;
	m_pFData	= pFD;
	CCSprite* pSprTmp	= NULL;
	m_bZorde	= pFD->bZorde;
	m_bEventToFrame	= false;
	if(pFD->szSpr)//底图
	{
		char	szTmp[256];
		sprintf(szTmp,"ui/%s",pFD->szSpr);
		if(strstr(szTmp,".jpg") != NULL)
		{
			pSprTmp	= GFun_InitSpriteByJPG(szTmp);
			pSprTmp->setAnchorPoint (ccp(0,1));
			addChild (pSprTmp,0);
		}
		else
		{
			pSprTmp	= CCSprite::create (szTmp);
			if(pSprTmp)
			{
				pSprTmp->setAnchorPoint (ccp(0,1));
				addChild (pSprTmp,0);  
			}
		}
	}
	setAnchorPoint (ccp(0,1));
	setTouchEnabled (true);// (!pFD->bLocked);
	
	for (int i = 0; i < pFD->pListComps->Count ; i++)
	{
		DComponent* pComp	= (DComponent*)pFD->pListComps->Items[i];
		CCNode*	pOBj	= AddDComp (pComp);
		
	}
	InitFrame ();

	m_bCloseAct	= false;

	if(m_pFData->bLocked)
	{
		m_bLockThrough	= true;
	}

	m_bEnableClick	= m_pFData->bUnTrhough;
	if(pFD->nW&&pFD->nH){
		setContentSize (CCSize(pFD->nW,pFD->nH));
	}
	else if(pSprTmp){
		setContentSize (pSprTmp->getContentSize ());
	}

	byStyle		= 0;
}

FormLayer::~FormLayer()
{
	if(m_pFData->pCFrame)
	{
		
		CCNode* pNode	= getChildByTag((int)m_pFData->pCFrame);
		if(pNode)
		{
			CCPoint	ptSrc	= PtIF2Cocos (m_pFData->pCFrame->nFrmX,m_pFData->pCFrame->nFrmY);
			CCPoint ptCur	= pNode->getPosition();
			m_pFData->pCFrame->nChgX	= (ptCur.x-ptSrc.x)*POINT_PIX_CNT;
			m_pFData->pCFrame->nChgY	= (ptCur.y-ptSrc.y)*POINT_PIX_CNT;
		}
	}
	m_pFData->pFormLayer = NULL;
}

void FormLayer::RunOpenEffect(cocos2d::CCPoint pt)
{
	switch(m_pFData->bOpenStyle)
	{
	case 1:
		{
			setPosition (pt);
		}
		break;
	case 2:
		{
			setPosition (ccp(pt.x+getContentSize ().width/2,pt.y-getContentSize ().height));
			setScale(0.1f);

			CCActionInterval* goMove = CCMoveTo::create((float)0.3, pt );
			CCActionInterval* goScale	= CCScaleTo::create((float)0.3,1,1);
			//CCActionInterval* goRotate	= CCRotateTo::create(1,1080);
			//CCFiniteTimeAction* seq = CCSequence::create(
			CCFiniteTimeAction* seq = CCSpawn::create (goMove, goScale, NULL);
			runAction( seq);
			FrameLayer* pFrame	= (FrameLayer*)getChildByTag((int)m_pFData->pCFrame);

		}
		break;
	case 3:		//左往右
		{
			setPosition (ccp(-getContentSize ().width,pt.y));
			CCActionInterval* goMove = CCMoveTo::create((float)0.3, pt );
			runAction( goMove);
		}
		break;
	case 4:		//右往左
		{
			setPosition (ccp(SCREEN_SIZE_W,pt.y));
			CCActionInterval* goMove = CCMoveTo::create((float)0.3, pt );
			runAction( goMove);
		}
		break;
	case 5:		//上往下
		{
			setPosition (ccp(pt.x,SCREEN_SIZE_H+getContentSize ().height));
			CCActionInterval* goMove = CCMoveTo::create((float)0.3, pt );
			runAction( goMove);
		}
		break;
	case 6:		//下往上
		{
			setPosition (ccp(pt.x,1));
			CCActionInterval* goMove = CCMoveTo::create((float)0.3, pt );
			runAction( goMove);
		}
		break;
	case 7:		//渐隐
		{
			setPosition (pt);
// 			CCActionInterval* goMove = CCFadeIn::create((float)0.3 );
// 			runAction( goMove);
			CCActionInterval* pFadeIn	= CCFadeIn::create (0.5);
			CCObject* child;
			CCARRAY_FOREACH(getChildren(), child)
			{
				CCNode* pCNode = (CCNode*) child;
				CCActionInterval* pFadeIn1	= CCFadeIn::create (0.5);
				pCNode->runAction(pFadeIn1);
				CCObject* CCchild;
				CCARRAY_FOREACH(pCNode->getChildren(), CCchild)
				{
					CCNode* pCCNode = (CCNode*) CCchild;
					CCActionInterval* pFadeIn2	= CCFadeIn::create (0.5);
					pCCNode->runAction(pFadeIn2);
					CCObject* CCCchild;
					CCARRAY_FOREACH(pCCNode->getChildren(), CCCchild)
					{
						CCNode* pCCCNode = (CCNode*) CCCchild;
						CCActionInterval* pFadeIn3	= CCFadeIn::create (0.5);
						pCCCNode->runAction(pFadeIn3);
					}
				}
			}
			runAction(pFadeIn);
		}
		break;
	default:
		setPosition (pt);
		break;
	}

}

void FormLayer::RunCloseEffect()
{
	switch(m_pFData->bOpenStyle)
	{
	case 1:
		//{

		//}
		break;
	case 2:
		{
			CCPoint pt	= getPosition();
			CCActionInterval* goMove = CCMoveTo::create((float)0.3, ccp(pt.x+getContentSize ().width/2,pt.y-getContentSize ().height));
			CCActionInterval* goScale	= CCScaleTo::create((float)0.3,0.1f,0.1f);
			//CCActionInterval* goRotate	= CCRotateTo::create(1,1080);
			CCFiniteTimeAction* seq = CCSpawn::create(goScale,goMove, NULL);
			runAction( seq);
		}
		break;
	case 3:
		{
			CCActionInterval* goMove = CCMoveTo::create((float)0.3, ccp(-getContentSize ().width,getPositionY ()) );
			runAction( goMove);
		}
		break;
	case 4:
		{
			CCActionInterval* goMove = CCMoveTo::create((float)0.3, ccp(SCREEN_SIZE_W,getPositionY ()) );
			runAction( goMove);
		}
		break;
	case 5:
		{
			CCActionInterval* goMove = CCMoveTo::create((float)0.3, ccp(getPositionX (),SCREEN_SIZE_H+getPositionY ()) );
			runAction( goMove);
		}
		break;
	case 6:
		{
			CCActionInterval* goMove = CCMoveTo::create((float)0.3, ccp(getPositionX (),1) );
			runAction( goMove);
		}
		break;
	case 7:		//渐隐
		{
// 			CCActionInterval* goMove = CCFadeOut::create((float)0.3 );
// 			runAction( goMove);
			CCActionInterval* pFadeIn	= CCFadeOut::create (0.5);
			CCObject* child;
			CCARRAY_FOREACH(getChildren(), child)
			{
				CCNode* pCNode = (CCNode*) child;
				CCActionInterval* pFadeIn1	= CCFadeOut::create (0.5);
				pCNode->runAction(pFadeIn1);
				CCObject* CCchild;
				CCARRAY_FOREACH(pCNode->getChildren(), CCchild)
				{
					CCNode* pCCNode = (CCNode*) CCchild;
					CCActionInterval* pFadeIn2	= CCFadeOut::create (0.5);
					pCCNode->runAction(pFadeIn2);
					CCObject* CCCchild;
					CCARRAY_FOREACH(pCCNode->getChildren(), CCCchild)
					{
						CCNode* pCCCNode = (CCNode*) CCCchild;
						CCActionInterval* pFadeIn3	= CCFadeOut::create (0.5);
						pCCCNode->runAction(pFadeIn3);
					}
				}
			}
			runAction(pFadeIn);
		}
		break;
	}
}

void FormLayer::MovePosition( cocos2d::CCPoint pt )
{
	m_pFData->nPosX = pt.x;
	m_pFData->nPosY = SCREEN_SIZE_H*POINT_PIX_CNT - pt.y;
	MyDragLayer::MovePosition(pt);
}

void FormLayer::_DCompCheck(CCNode* pFL)
{
	if(pFL->getChildren() && pFL->getChildren()->count() > 0)
	{
		CCObject* child;
		CCARRAY_FOREACH(pFL->getChildren(), child)
		{
			CCNode* pNode = (CCNode*) child;
			if(pNode->getTag() == -1)
			{
				_DCompCheck(pNode);
			}
			else if(pNode->getTag () == (int)m_pFData->pCFrame)
			{
				_DCompCheck(pNode);
			}else if (pNode->getTag () == 9998)
			{

			}
			else if(pNode && pNode->getTag () != -1)
			{
				DComponent* pComp	= (DComponent*)pNode->getTag ();
				pNode->setVisible (pComp->bVisable);
				if (pComp->bChgPos)
				{
					CCPoint	pt	= PtIF2Cocos (pComp->GetAlignmentX (),pComp->GetAlignmentY ());
					if (!pComp->m_bChgPosbySpr)
					{
						if(pComp->nCPTime)
						{
							CCActionInterval* pMove	= CCMoveTo::create (pComp->nCPTime/1000.0,pt);
							pNode->runAction(pMove);
						}
						else
						{
							pNode->setPosition (pt);
						}
					}
					pComp->bChgPos = false;

					if(pComp->pSTextCenter)
					{
						((CSpecialText*)pNode)->SetRealRect (CCRect((pComp->nX+pComp->pParent->nPosX)/POINT_PIX_CNT,SCREEN_SIZE_H-(pComp->pParent->nPosY+pComp->nY+pComp->nH)/POINT_PIX_CNT,pComp->nW/POINT_PIX_CNT,pComp->nH/POINT_PIX_CNT));
					}
					if(pComp->byType == COMPONENT_BUTTON)
					{
						pt	= PtIF2Cocos (pComp->nX,pComp->nY);
						if(pComp->bVisable)
							ChgRegERect (pNode,CCRect(pt.x,pt.y-pComp->nH/POINT_PIX_CNT,pComp->nW/POINT_PIX_CNT,pComp->nH/POINT_PIX_CNT));
						else
							ChgRegERect (pNode,CCRect(SCREEN_SIZE_W*10,pt.y-pComp->nH/POINT_PIX_CNT,pComp->nW/POINT_PIX_CNT,pComp->nH/POINT_PIX_CNT));
					}
					else if(pComp->byType == COMPONENT_EDIT)
					{
						if(pComp->bVisable)
						{
							int nRX	= pComp->pParent->nPosX + pComp->nX;
							int nRY	= pComp->pParent->nPosY + pComp->nY;
							((CObjTextInput*)pNode)->SetRect (nRX/POINT_PIX_CNT, SCREEN_SIZE_H-nRY/POINT_PIX_CNT-pComp->nH/POINT_PIX_CNT,pComp->nW/POINT_PIX_CNT,pComp->nH/POINT_PIX_CNT);
						}
						else
							((CObjTextInput*)pNode)->SetRect (SCREEN_SIZE_W*2, SCREEN_SIZE_H,pComp->nW/POINT_PIX_CNT,pComp->nH/POINT_PIX_CNT);
					}
				}
				else if(m_pFData->bChgPos )
				{
					if(pComp->pSTextCenter)
						((CSpecialText*)pNode)->SetRealRect (CCRect((pComp->nX+pComp->pParent->nPosX)/POINT_PIX_CNT,SCREEN_SIZE_H-(pComp->pParent->nPosY+pComp->nY+pComp->nH)/POINT_PIX_CNT,pComp->nW/POINT_PIX_CNT,pComp->nH/POINT_PIX_CNT));
					else if(pComp->byType == COMPONENT_EDIT)
					{
						int nRX	= pComp->pParent->nPosX + pComp->nX;
						int nRY	= pComp->pParent->nPosY + pComp->nY;
						((CObjTextInput*)pNode)->SetRect (nRX/POINT_PIX_CNT, SCREEN_SIZE_H-nRY/POINT_PIX_CNT-pComp->nH/POINT_PIX_CNT,pComp->nW/POINT_PIX_CNT,pComp->nH/POINT_PIX_CNT);
					}
				}
				if(pComp->byChgScaleY)
				{
					pNode->setScaleY(pComp->byChgScaleY/100.0);
					pComp->byChgScaleY	= 0;
				}

				_runCompAction (pNode);


				switch (pComp->byType)
				{
				case COMPONENT_EDIT:
					{
						CObjTextInput * pInputObj = (CObjTextInput*)pNode;
						pInputObj->m_bInputType = pComp->byInputType;
					}
					break;
				case COMPONENT_BUTTON:
					{
						((CObjButton*)pNode)->Update ();
					}
					break;
				case COMPONENT_LINK:
					{
						if(pComp->pSTextCenter->bNeedRefresh)
						{
							((CSpecialText*)pNode)->ResetChildren ();
							pComp->pSTextCenter->bNeedRefresh	= false;
						}
						else if(pComp->pSTextCenter->m_SameSwitch)
						{
							((CSpecialText*)pNode)->ResetPose ();
							pComp->pSTextCenter->m_SameSwitch = false;
						}
						else
							((CSpecialText*)pNode)->UpdateChildChg ();
					}
					break;
				}

			}
		}
	}
}

bool FormLayer::Update()
{
	//后台数据被关闭后把前端的关闭
	if(m_bCloseAct && numberOfRunningActions () == 0)
		return false;
	if(m_pFData->bOpened == false && !m_bCloseAct)
	{
		m_bCloseAct	= true;
		RunCloseEffect ();
		return true;
	}
	if(m_bCloseAct)
		return true;

	if(m_pFData->pCFrame)
	{
		if(m_pFData->pCFrame->bChged)
		{
			removeChildByTag ((int)m_pFData->pCFrame,true);
			m_pFData->pCFrame->pFrame = NULL;
			InitFrame ();
		}
		FrameLayer* pFrame	= (FrameLayer*)getChildByTag((int)m_pFData->pCFrame);
		if(pFrame) 
		{
			pFrame->Update();
			CCPoint	ptFrm	= pFrame->getPosition();
			if(m_pFData->pCFrame->bIsHoriz)
				m_pFData->pCFrame->nOffChgPos	= ptFrm.x*POINT_PIX_CNT-m_pFData->pCFrame->nFrmX;
			else
				m_pFData->pCFrame->nOffChgPos	= ptFrm.y*POINT_PIX_CNT-m_pFData->pCFrame->nFrmY;
		}

		if(m_pFData->pCFrame->nChgX || m_pFData->pCFrame->nChgY)
		{
			//FrameLayer* pFrame	= (FrameLayer*)getChildByTag((int)m_pFData->pCFrame);
			if(pFrame)
			{
				CCPoint	ptFrm	= pFrame->getPosition();
				ptFrm.x	+= m_pFData->pCFrame->nChgX/POINT_PIX_CNT;
				ptFrm.y	+= m_pFData->pCFrame->nChgY/POINT_PIX_CNT;
				if(m_pFData->pCFrame->byChgFrom == 200)
					pFrame->SetStopPositon(ptFrm,true,false);
				else if(m_pFData->pCFrame->byChgFrom == 1)
					pFrame->SetStopPositon(ptFrm,true,false,0.049);
				else
					pFrame->SetStopPositon(ptFrm,true);
				m_pFData->pCFrame->nChgX	= 0;
				m_pFData->pCFrame->nChgY	= 0;
				m_pFData->pCFrame->byChgFrom= 0;

			}
		}
		if(m_pFData->pCFrame->nSetX || m_pFData->pCFrame->nSetY)
		{
			if(pFrame)
			{
				CCPoint	ptFrm;
				ptFrm.x	= (m_pFData->pCFrame->nFrmX + m_pFData->pCFrame->nSetX)/POINT_PIX_CNT;
				ptFrm.y	= (-m_pFData->pCFrame->nFrmY + m_pFData->pCFrame->nSetY)/POINT_PIX_CNT;
				if(m_pFData->pCFrame->byChgFrom == 200)
					pFrame->SetStopPositon(ptFrm,true,false);
				else
					pFrame->SetStopPositon(ptFrm,true);
				m_pFData->pCFrame->nSetX	= 0;
				m_pFData->pCFrame->nSetY	= 0;
				m_pFData->pCFrame->byChgFrom= 0;

			}
		}

	}

	_DCompCheck(this);

	if(m_pFData->bChgPos)
	{
		if(m_pFData->nCPTime)
		{
			CCActionInterval* pMove	= CCMoveTo::create (m_pFData->nCPTime/1000.0,ccp(m_pFData->nPosX/POINT_PIX_CNT, (SCREEN_SIZE_H*POINT_PIX_CNT - m_pFData->nPosY)/POINT_PIX_CNT));
			runAction(pMove);
		}
		else
			setPosition(ccp(m_pFData->nPosX/POINT_PIX_CNT, (SCREEN_SIZE_H*POINT_PIX_CNT - m_pFData->nPosY)/POINT_PIX_CNT));

		m_pFData->bChgPos	= false;
	}
	return true;
	
}

void FormLayer::_runCompAction( CCNode* pNode )
{
	DComponent* pComp	= (DComponent*)pNode->getTag ();
	//if(pNode->numberOfRunningActions ())
	//	return;
	switch (pComp->byRunAction)
	{
	case 1:		//震动
		{
			CCPoint	pt	= PtIF2Cocos (pComp->GetAlignmentX ()+rand()%7-3,pComp->GetAlignmentY ()+rand()%7-3);
			CCActionInterval* pMove1	= CCMoveTo::create (0.08,pt);
			pt	= PtIF2Cocos (pComp->GetAlignmentX ()+rand()%7-3,pComp->GetAlignmentY ()+rand()%7-3);
			CCActionInterval* pMove2	= CCMoveTo::create (0.07,pt);
			pt	= PtIF2Cocos (pComp->GetAlignmentX ()+rand()%7-3,pComp->GetAlignmentY ()+rand()%7-3);
			CCActionInterval* pMove3	= CCMoveTo::create (0.06,pt);
			pt	= PtIF2Cocos (pComp->GetAlignmentX ()+rand()%7-3,pComp->GetAlignmentY ()+rand()%7-3);
			CCActionInterval* pMove4	= CCMoveTo::create (0.06,pt);
			pt	= PtIF2Cocos (pComp->GetAlignmentX (),pComp->GetAlignmentY ());
			CCActionInterval* pMove	= CCMoveTo::create (0.05,pt);
			
			CCFiniteTimeAction* seq = CCSequence::create (pMove1, pMove2, pMove3, pMove4,pMove,NULL);
			pNode->runAction(seq);
		}
		break;
	case 2:		//缩放
		{
			CCActionInterval* pBig1	= CCScaleTo::create (0.15,1.15f,1.15f);
			CCActionInterval* pBig2	= CCDelayTime::create (0.1);
			CCActionInterval* pBig3	= CCScaleTo::create (0.1,1,1);
			CCFiniteTimeAction* seq = CCSequence::create (CCEaseSineOut::create (pBig1), pBig2,  CCEaseSineOut::create (pBig3),NULL);
			pNode->runAction( seq);
		}
		break;
	case 3:		//跳动
		{
			//CCPoint	pt	= PtIF2Cocos (pComp->GetAlignmentX (),pComp->GetAlignmentY ());
			CCActionInterval* pJump1	= CCJumpBy::create (0.5,ccp(0,0),10/POINT_PIX_CNT,1);
			//CCFiniteTimeAction* pRun	= CCSequence::create (pJump1,NULL);
			pNode->runAction(CCRepeatForever::create(pJump1));
			//pNode->runAction( CCRepeatForever::create(pJump1));

			//CCActionInterval* pRot1	= CCRotateTo::create (0.1,-25);
			//CCActionInterval* pRot2	= CCRotateTo::create (0.1,15);
			//CCActionInterval* pRot3	= CCRotateTo::create (0.1,0);
			//CCFiniteTimeAction* seq = CCSequence::create (CCEaseSineIn::create (pRot1), pRot2, CCEaseSineOut::create (pRot3),NULL);
			//
			//pNode->runAction( seq);
		}
		break;
	case 4:		//缩放
		{
			CCActionInterval* pBig1	= CCScaleTo::create (0.15,0.85f,0.85f);
			CCActionInterval* pBig2	= CCDelayTime::create (0.15);
			CCActionInterval* pBig3	= CCScaleTo::create (0.1,1,1);
			CCFiniteTimeAction* seq = CCSequence::create (pBig1, pBig2, pBig3,NULL);
			pNode->runAction( seq);
		}
		break;
	case 5:
		{
			CCActionInterval* pRota1	= CCRotateTo::create (0.5,1080);
			CCActionInterval* pSmall	= CCScaleTo::create (0.5,0.1f,0.1f);
			CCActionInterval* pFly		= CCMoveTo::create (0.5,ccp(SCREEN_SIZE_W+50/POINT_PIX_CNT,50/POINT_PIX_CNT));
			CCFiniteTimeAction* seq		= CCSpawn::create(pRota1,pSmall, NULL);
			CCFiniteTimeAction* pRun	= CCSequence::create (seq,pFly,NULL);
			pNode->runAction(pRun);
		}
		break;

	case 7:		//旋转
		{
			CCActionInterval* pRota1	= CCRotateTo::create (1,120);
			CCAction * pCallback = CCCallFuncND::actionWithTarget(pNode, callfuncND_selector(FormLayer::ActionCallBack),(void*)7); 
			CCFiniteTimeAction* pRun	= CCSequence::create (pRota1,pCallback,NULL);
			pNode->runAction( pRun);
		}
		break;
	case 8:		//逆时针旋转
		{
			CCActionInterval* pRota1	= CCRotateTo::create (1,-120);
			CCAction * pCallback = CCCallFuncND::actionWithTarget(pNode, callfuncND_selector(FormLayer::ActionCallBack),(void*)8); 
			CCFiniteTimeAction* pRun	= CCSequence::create (pRota1,pCallback,NULL);
			pNode->runAction( pRun);
		}
		break;
	case 11:		//选角色RPG
		{
			if(pNode->numberOfRunningActions ())
				return;
			CCActionInterval* pMove	= CCMoveBy::create (0.2f,ccp(-600/POINT_PIX_CNT,0));
			CCActionInterval* pMove2	= CCMoveBy::create (0.2f,ccp(600/POINT_PIX_CNT,0));
			CCAction * pCallback = CCCallFuncND::actionWithTarget(pNode, callfuncND_selector(FormLayer::ActionCallBack),(void*)11); 
			CCFiniteTimeAction* pRun	= CCSequence::create (pMove,pCallback,pMove2,NULL);
			pNode->runAction(pRun);
		}
		break;
	case 13:
		{
			static int nPreResult= 0;
			int nRote = 360*6 + ((int)pComp->m_pActionParam+(10-nPreResult))*36;
			nPreResult = (int)pComp->m_pActionParam;
			CCLOG("Result:%d",(int)pComp->m_pActionParam);
			const int nCount = 10;
			CCArray* action = CCArray::create();
			int nTotal = (1+nCount)*nCount/2;
			int nCur = 0;
			for (int i=0;i<nCount;i++)
			{
				nCur =( nCount-i);
				CCActionInterval* pSmall1	= CCRotateBy::create (0.3f,nRote*nCur/nTotal);
				action->addObject(pSmall1);
			}


			CCFiniteTimeAction* seq = CCSequence::create (action);
			pNode->runAction(seq);
		}
		break;
	case 15:			//计时用,2秒
		{
			CCActionInterval* pSmall	= CCScaleTo::create (1,1.0f,1.0f);
			CCAction * pCallback = CCCallFuncND::actionWithTarget(pNode, callfuncND_selector(FormLayer::ActionCallBack),(void*)15); 
			CCAction * pCallback2 = CCCallFuncND::actionWithTarget(pNode, callfuncND_selector(FormLayer::ActionCallBack),(void*)16); 
			CCFiniteTimeAction* pRun	= CCSequence::create (pSmall,pCallback,pSmall,pCallback2,NULL);
			pNode->runAction(pRun);

		}
		break;
	case 16:			//循环缩放
		{
			CCActionInterval* pSmall	= CCScaleTo::create (1,0.8f,0.8f);
			CCActionInterval* pSmall1	= CCScaleTo::create (1,1.0f,1.0f);
			CCFiniteTimeAction* pRun	= CCSequence::create (pSmall,pSmall1,NULL);
			pNode->runAction(CCRepeatForever::create(static_cast<CCSequence *>(pRun)));

		}
		break;
	case 20:		//转盘
		{
			int nPreResult=pNode->getRotation();
			nPreResult %= 360;
			CCActionInterval* pSmall1	= CCRotateBy::create (6.0f,(int)pComp->m_pActionParam*45-nPreResult + 360*16);

			pNode->runAction(CCEaseSineOut::create (pSmall1));
			g_pSoundCenter->PlayerEffect("Sound/zhuanzhuan");
		}
		break;
	case 21:		//闪烁
		{
			CCActionInterval* pFadeOut	= CCFadeOut::create (0.95);
			CCActionInterval* pFadeIn	= CCFadeIn::create (0.95);
			CCFiniteTimeAction* seq = CCSequence::create (pFadeOut,pFadeIn,NULL);
			pNode->runAction( seq);
		}
		break;
	case 22:		//公告移动渐隐
		{
			CCActionInterval* pFadeIn	= CCFadeIn::create (0.5);
			CCActionInterval* pFadeOut	= CCFadeOut::create (1.5);
			CCActionInterval* pMove	= CCMoveBy::create (1.5f,ccp(0,75/POINT_PIX_CNT));
			CCActionInterval* pStep	= (CCActionInterval*)CCSpawn::create(pFadeOut,pMove,NULL);
			CCFiniteTimeAction* seq = CCSequence::create (pFadeIn,pStep,NULL);
			CCObject* child;
			CCARRAY_FOREACH(pNode->getChildren(), child)
			{
				CCNode* pCNode = (CCNode*) child;
				CCActionInterval* pFadeIn1	= CCFadeIn::create (0.5);
				CCActionInterval* pFadeOut1	= CCFadeOut::create (1.5);
				CCFiniteTimeAction* seq1 = CCSequence::create (pFadeIn1,pFadeOut1,NULL);
				pCNode->runAction(seq1);
				CCObject* CCchild;
				CCARRAY_FOREACH(pCNode->getChildren(), CCchild)
				{
					CCNode* pCCNode = (CCNode*) CCchild;
					CCActionInterval* pFadeIn2	= CCFadeIn::create (0.5);
					CCActionInterval* pFadeOut2	= CCFadeOut::create (1.5);
					CCFiniteTimeAction* seq2 = CCSequence::create (pFadeIn2,pFadeOut2,NULL);
					pCCNode->runAction(seq2);
				}
			}
			pNode->runAction(seq);
		}
		break;
	case 23:		//渐现
		{
			CCActionInterval* pFadeIn	= CCFadeIn::create (0.5);
			CCObject* child;
			CCARRAY_FOREACH(pNode->getChildren(), child)
			{
				CCNode* pCNode = (CCNode*) child;
				CCActionInterval* pFadeIn1	= CCFadeIn::create (0.5);
				pCNode->runAction(pFadeIn1);
				CCObject* CCchild;
				CCARRAY_FOREACH(pCNode->getChildren(), CCchild)
				{
					CCNode* pCCNode = (CCNode*) CCchild;
					CCActionInterval* pFadeIn2	= CCFadeIn::create (0.5);
					pCCNode->runAction(pFadeIn2);
				}
			}
			pNode->runAction(pFadeIn);
		}
		break;
	case 24:		//渐隐
		{
			CCActionInterval* pFadeOut	= CCFadeOut::create (0.5);
			CCObject* child;
			CCARRAY_FOREACH(pNode->getChildren(), child)
			{
				CCNode* pCNode = (CCNode*) child;
				CCActionInterval* pFadeOut1	= CCFadeIn::create (0.5);
				pCNode->runAction(pFadeOut1);
				CCObject* CCchild;
				CCARRAY_FOREACH(pCNode->getChildren(), CCchild)
				{
					CCNode* pCCNode = (CCNode*) CCchild;
					CCActionInterval* pFadeOut2	= CCFadeIn::create (0.5);
					pCCNode->runAction(pFadeOut2);
				}
			}
			pNode->runAction(pFadeOut);
		}
		break;
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
		{
			pNode->setScale(0.01f);

			CCActionInterval* pDelay	= CCDelayTime::create (2+0.5*(pComp->byRunAction-40));
			CCActionInterval* pScTo		= CCScaleTo::create (0.5,1,1);
			CCFiniteTimeAction* seq = CCSequence::create (pDelay, pScTo,NULL);
			pNode->runAction( seq);
		}
		break;
	case 50:
		{
			if(pNode->numberOfRunningActions ())
				return;
			CCActionInterval* pSmall	= CCScaleTo::create (1,0.85f,0.85f);
			CCActionInterval* pSmall1	= CCScaleTo::create (1,1.1f,1.1f);
			CCActionInterval* pRun	= (CCActionInterval*)CCSequence::create (pSmall,pSmall1,NULL);
			pNode->runAction(pRun);
		}
		break;
	case 60:    //钓鱼
		{
			CCActionInterval* pSmall	= CCScaleTo::create (1,0.8f,0.8f);
			CCActionInterval* pSmall1	= CCScaleTo::create (1,1.0f,1.0f);
			CCFiniteTimeAction* pRun	= CCSequence::create (pSmall,pSmall1,NULL);
			pNode->runAction(CCRepeatForever::create(static_cast<CCSequence *>(pRun)));
		}
		break;
	case 61:
	case 62:
	case 63:
		{
			pNode->stopAllActions();
			pNode->setScale(0.5f);

			CCPoint	pt	= PtIF2Cocos (pComp->GetAlignmentX ()+rand()%7,pComp->GetAlignmentY ()+rand()%7);
			CCActionInterval* pMove1	= CCMoveTo::create (0.12,pt);
			pt	= PtIF2Cocos (pComp->GetAlignmentX ()+rand()%7,pComp->GetAlignmentY ()+rand()%7);
			CCActionInterval* pMove2	= CCMoveTo::create (0.11,pt);
			pt	= PtIF2Cocos (pComp->GetAlignmentX ()+rand()%7,pComp->GetAlignmentY ()+rand()%7);
			CCActionInterval* pMove3	= CCMoveTo::create (0.10,pt);
			pt	= PtIF2Cocos (pComp->GetAlignmentX ()+rand()%7,pComp->GetAlignmentY ()+rand()%7);
			CCActionInterval* pMove4	= CCMoveTo::create (0.10,pt);
			pt	= PtIF2Cocos (pComp->GetAlignmentX (),pComp->GetAlignmentY ());
			CCActionInterval* pMove	= CCMoveTo::create (0.05,pt);

			CCFiniteTimeAction* pRun = CCSequence::create (pMove1, pMove2, pMove3, pMove4,pMove,NULL);
			pNode->runAction(CCRepeat::create(pRun,(pComp->byRunAction-60)*100/50));

			g_pSoundCenter->PlayerEffect("Sound/shark");
		}
		break;
	case 80:
		{
			pNode->resumeSchedulerAndActions();
			if(pNode->numberOfRunningActions() == 0)
			{
				CCActionInterval * pJump1	= CCJumpTo::create(1.5f,pNode->getPosition(),10/POINT_PIX_CNT,1);
				CCActionInterval * pJump2	= CCJumpTo::create(1.5f,pNode->getPosition(),-10/POINT_PIX_CNT,1);
				pNode->runAction(CCSequence::create(	pJump1, pJump2, NULL));
			}
		}
		break;
	case 81:
		{
			pNode->resumeSchedulerAndActions();
			if(pNode->numberOfRunningActions() == 0)
			{
				CCActionInterval * pJump1	= CCJumpTo::create(1.0f,pNode->getPosition(),10/POINT_PIX_CNT,1);
				CCActionInterval * pJump2	= CCJumpTo::create(1.0f,pNode->getPosition(),-10/POINT_PIX_CNT,1);
				pNode->runAction(CCSequence::create(	pJump1, pJump2, NULL));
			}
		}
		break;
	case 82:
		{
			pNode->resumeSchedulerAndActions();
			if(pNode->numberOfRunningActions() == 0)
			{
				CCActionInterval * pJump1	= CCJumpTo::create(1.3f,pNode->getPosition(),8/POINT_PIX_CNT,1);
				CCActionInterval * pJump2	= CCJumpTo::create(1.3f,pNode->getPosition(),-8/POINT_PIX_CNT,1);
				pNode->runAction(CCSequence::create(	pJump1, pJump2, NULL));
			}
		}
		break;
	case 83:
		{
			pNode->resumeSchedulerAndActions();
			if(pNode->numberOfRunningActions() == 0)
			{
				CCActionInterval * pJump1	= CCJumpTo::create(1.5f,pNode->getPosition(),8/POINT_PIX_CNT,1);
				CCActionInterval * pJump2	= CCJumpTo::create(1.5f,pNode->getPosition(),-8/POINT_PIX_CNT,1);
				pNode->runAction(CCSequence::create(	pJump2, pJump1, NULL));
			}
		}
		break;
	case 84:
		{
			pNode->resumeSchedulerAndActions();
			if(pNode->numberOfRunningActions() == 0)
			{
				CCActionInterval * pJump1	= CCJumpTo::create(1.2f,pNode->getPosition(),10/POINT_PIX_CNT,1);
				CCActionInterval * pJump2	= CCJumpTo::create(1.2f,pNode->getPosition(),-10/POINT_PIX_CNT,1);
				pNode->runAction(CCSequence::create(	pJump2, pJump1, NULL));
			}
		}
		break;
	case 87:
		{
			pNode->pauseSchedulerAndActions();
		}
		break;
	case 88:
		{
			CCSize	size	= pNode->getContentSize();
			CCActionInterval * pStep1 = CCOrbitCamera::create(1, 100, 0, 0, 180, 0, 0);

			pNode->runAction(pStep1);
		}
		break;
	case 89:
		{
			CCSize	size	= pNode->getContentSize();
			CCActionInterval * pStep1 = CCOrbitCamera::create(1, 100, 0, 180, 180, 0, 0);

			pNode->runAction(pStep1);
		}
		break;
	}
	pComp->byRunAction	= 0;
}

void FormLayer::visit( void )
{
	//if(m_pFData->bUnTrhough)
	//{
	//	glEnable(GL_SCISSOR_TEST);
	//	glScissor (m_rectReal.origin.x*POINT_PIX_CNT,m_rectReal.origin.y*POINT_PIX_CNT,m_rectReal.size.width*POINT_PIX_CNT,m_rectReal.size.height*POINT_PIX_CNT);
	//	MyDragLayer::visit();

	//	glDisable(GL_SCISSOR_TEST);
	//}
	//else
		MyDragLayer::visit();
}

bool FormLayer::ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	int	nRtl	= MyDragLayer::ccTouchBegan (pTouch, pEvent);
	if(nRtl && !g_bIFLayerClicked && m_pFData->pCFrame && m_pFData->pCFrame->nZorder < 0)
	{
		FrameLayer* pFLayer	= (FrameLayer*)getChildByTag((int)m_pFData->pCFrame);
		if(pFLayer)
		{
			if(pFLayer->ccTouchBegan(pTouch, pEvent))
				m_bEventToFrame	= true;
		}
	}
	return nRtl;
}

void FormLayer::ccTouchMoved( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	if(m_bEventToFrame)
	{
		if(m_pFData->pCFrame)
		{
			FrameLayer* pFLayer	= (FrameLayer*)getChildByTag((int)m_pFData->pCFrame);
			if(pFLayer)
			{
				return pFLayer->ccTouchMoved(pTouch, pEvent);
			}
		}
	}
	return MyDragLayer::ccTouchMoved (pTouch, pEvent);
}

void FormLayer::ccTouchEnded( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	if(m_bEventToFrame)
	{
		if(m_pFData->pCFrame)
		{
			FrameLayer* pFLayer	= (FrameLayer*)getChildByTag((int)m_pFData->pCFrame);
			if(pFLayer)
			{
				pFLayer->ccTouchEnded(pTouch, pEvent);
			}
		}
		m_bEventToFrame	= false;
	}
	
	return MyDragLayer::ccTouchEnded (pTouch, pEvent);
}

void FormLayer::InitFrame()
{
	if(m_pFData->pCFrame)
	{
		m_pFData->pCFrame->bChged = false;
		FrameLayer* pFrameLayer	= NULL;//new FrameLayer();
// 		if (m_pFData->pCFrame->bLoop)
// 		{
// 			pFrameLayer = new LoopLayer();
// 		}else{
			pFrameLayer	= new FrameLayer();
//		}
		m_pFData->pCFrame->pFrame = pFrameLayer;
		pFrameLayer->m_bLoop = m_pFData->pCFrame->bLoop;
		pFrameLayer->m_bZorde = m_pFData->bZorde+m_pFData->pCFrame->nZorder;
		pFrameLayer->m_nDifStep	= m_pFData->pCFrame->nDisStep/POINT_PIX_CNT;
		pFrameLayer->m_bLimitRect	= m_pFData->pCFrame->bLimitRect;
		pFrameLayer->fnStepEventFunc = m_pFData->pCFrame->fnStepEventFunc;
		CCPoint	ptFrm	= PtIF2Cocos (m_pFData->pCFrame->nFrmX,m_pFData->pCFrame->nFrmY);

		pFrameLayer->setAnchorPoint (ccp(0,1));
		pFrameLayer->SetRealRect (CCRect(m_pFData->pCFrame->nFrmX/POINT_PIX_CNT+m_pFData->nPosX/POINT_PIX_CNT, SCREEN_SIZE_H-(m_pFData->pCFrame->nFrmY+m_pFData->pCFrame->nFrmH+m_pFData->nPosY )/POINT_PIX_CNT,m_pFData->pCFrame->nFrmW/POINT_PIX_CNT,m_pFData->pCFrame->nFrmH/POINT_PIX_CNT));
		addChild (pFrameLayer,2,(int)m_pFData->pCFrame);

		pFrameLayer->m_bLockX	= !m_pFData->pCFrame->bIsHoriz;
		pFrameLayer->m_bLockY	= m_pFData->pCFrame->bIsHoriz;
		pFrameLayer->m_bSplitTouch	= m_pFData->pCFrame->bSplitTouch;
		pFrameLayer->m_bDisalbeTouch = m_pFData->pCFrame->bDisableTouch;
		CCtrlFPart*	pBaseFP	= m_pFData->pCFrame->pCtrlFPBase;
		if(pBaseFP)
		{
			pBaseFP->pCompBase->m_pNodeBatch	= NULL;
			for (int j = 0; j < pBaseFP->pListComps->Count; j++)
			{
				DComponent* pFPComp	= (DComponent*)pBaseFP->pListComps->Items[j];
				pFPComp->m_pNodeBatch	= NULL;
			}
		}

		int nTotalH	= 0;
		for (int i = 0; i < m_pFData->pCFrame->pListFP->Count; i++)
		{
			CCtrlFPart* pCFP	= (CCtrlFPart*)m_pFData->pCFrame->pListFP->Items[i];

			//缓存BatchNode
			if(pBaseFP && pCFP->pCompBase->szSpr && pBaseFP->pCompBase->szSpr &&
				strcmp(pCFP->pCompBase->szSpr,pBaseFP->pCompBase->szSpr) == 0)
			{
				if(pBaseFP->pCompBase->m_pNodeBatch == NULL)
				{
					char	szTmp[255];
					sprintf(szTmp,"ui/%s",pCFP->pCompBase->szSpr);
					pBaseFP->pCompBase->m_pNodeBatch	= CCSpriteBatchNode::create(szTmp);
					((CCSpriteBatchNode*)pBaseFP->pCompBase->m_pNodeBatch)->setPosition(CCPointZero);
					pFrameLayer->addChild((CCSpriteBatchNode*)pBaseFP->pCompBase->m_pNodeBatch,pBaseFP->pCompBase->m_nOrder);
				}
				pCFP->pCompBase->m_pNodeBatch	= pBaseFP->pCompBase->m_pNodeBatch;
			}
			pFrameLayer->AddDComp (pCFP->pCompBase);


			if(pCFP->nNewSpace)
			{
				if(m_pFData->pCFrame->bIsHoriz)
					nTotalH	+= pCFP->nNewSpace+m_pFData->pCFrame->nSpace;
				else
					nTotalH	+= pCFP->nNewSpace+m_pFData->pCFrame->nSpace;
			}
			else
			{
				if(m_pFData->pCFrame->bIsHoriz)
					nTotalH	+= pCFP->pCompBase->nW+m_pFData->pCFrame->nSpace;
				else
					nTotalH	+= pCFP->pCompBase->nH+m_pFData->pCFrame->nSpace;
			}



			for (int j = 0; j < pCFP->pListComps->Count; j++)
			{
				DComponent* pFPComp	= (DComponent*)pCFP->pListComps->Items[j];

				////缓存BatchNode
				//if(pBaseFP && pFPComp->szSpr && ((DComponent*)pBaseFP->pListComps->Items[j])->szSpr
				//	&& strcmp(pFPComp->szSpr,((DComponent*)pBaseFP->pListComps->Items[j])->szSpr)==0)
				//{
				//	if(((DComponent*)pBaseFP->pListComps->Items[j])->m_pNodeBatch == NULL)
				//	{
				//		char	szTmp[255];
				//		sprintf(szTmp,"ui/%s",((DComponent*)pBaseFP->pListComps->Items[j])->szSpr);
				//		((DComponent*)pBaseFP->pListComps->Items[j])->m_pNodeBatch	= CCSpriteBatchNode::create(szTmp);
				//		((CCSpriteBatchNode*)((DComponent*)pBaseFP->pListComps->Items[j])->m_pNodeBatch)->setPosition(CCPointZero);
				//		pFrameLayer->addChild((CCSpriteBatchNode*)((DComponent*)pBaseFP->pListComps->Items[j])->m_pNodeBatch,((DComponent*)pBaseFP->pListComps->Items[j])->m_nOrder);
				//	}
				//	pFPComp->m_pNodeBatch	= ((DComponent*)pBaseFP->pListComps->Items[j])->m_pNodeBatch;
				//}


				pFrameLayer->AddDComp (pFPComp);
			}
		}

		nTotalH	= nTotalH/POINT_PIX_CNT + 50/POINT_PIX_CNT;
		if(m_pFData->pCFrame->bIsHoriz&&!m_pFData->pCFrame->bIsCreateByForm)
		{
			if(nTotalH < m_pFData->pCFrame->nFrmW/POINT_PIX_CNT)
				nTotalH	= m_pFData->pCFrame->nFrmW/POINT_PIX_CNT;

			pFrameLayer->SetLimitRect (CCRect(m_pFData->pCFrame->nFrmX/POINT_PIX_CNT-nTotalH+m_pFData->pCFrame->nFrmW/POINT_PIX_CNT,ptFrm.y-m_pFData->pCFrame->nFrmH/POINT_PIX_CNT,nTotalH,m_pFData->pCFrame->nFrmH/POINT_PIX_CNT));
		}
		else if(!m_pFData->pCFrame->bIsCreateByForm)
		{
			if(nTotalH < m_pFData->pCFrame->nFrmH/POINT_PIX_CNT)
				nTotalH	= m_pFData->pCFrame->nFrmH/POINT_PIX_CNT;

			pFrameLayer->SetLimitRect (CCRect(m_pFData->pCFrame->nFrmX/POINT_PIX_CNT,ptFrm.y-m_pFData->pCFrame->nFrmH/POINT_PIX_CNT,m_pFData->pCFrame->nFrmW/POINT_PIX_CNT,nTotalH));
		}else{
			pFrameLayer->SetLimitRect (CCRect(m_pFData->pCFrame->nFrmX/POINT_PIX_CNT,ptFrm.y-m_pFData->pCFrame->nFrmH/POINT_PIX_CNT,m_pFData->pCFrame->nFrmMaxW/POINT_PIX_CNT,m_pFData->pCFrame->nFrmMaxH/POINT_PIX_CNT));
		}
		pFrameLayer->setPosition (ptFrm.x+m_pFData->pCFrame->nChgX/POINT_PIX_CNT,ptFrm.y+m_pFData->pCFrame->nChgY/POINT_PIX_CNT);
		m_pFData->pCFrame->nChgX	= 0;
		m_pFData->pCFrame->nChgY	= 0;
	}
}

void FormLayer::ActionCallBack( cocos2d::CCNode *pNode,void* pParam )
{
	DComponent* pComp	= (DComponent*)pNode->getTag ();
	pComp->ActionCallBack(pParam);
}

int FormLayer::GetNodTagFrame()
{
	if(m_pFData->pCFrame)
		return (int)m_pFData->pCFrame;

	return -1;
}


InterfaceLayer::~InterfaceLayer()
{
	//SAFE_DELETE(m_pDInterface);
	m_pDFChild->Free ();
	m_pTipChild->Free ();
}

bool InterfaceLayer::init()
{
	CCLayer::init ();

	if(pMainInterface == NULL)
    {
		pMainInterface	= new DInterface("ui/Interface.IFP");
        pMainToopTip	= new DToopTipCenter();
    }
	else
		pMainInterface->CloseAllForm ();
		
	m_pDFChild	= xnList::Create ();
	m_pTipChild	= xnList::Create ();
	memset(m_ppGetTip,0,16);
	if(nowRateW && nowRateH){
		this->setAnchorPoint(CCPointZero);
		this->setScaleX(nowRateW);
		this->setScaleY(nowRateH);
	}
	//CCGLProgramManage::GetInstance().init();
	//setKeypadEnabled(true); //android 需要注册返回事件
	return true;
}

void InterfaceLayer::onEnter()
{
	CCLayer::onEnter ();
	scheduleUpdate();
}

void InterfaceLayer::onExit()
{
	CCLayer::onExit ();
	while(m_pDFChild->Count)
	{
		FormLayer* pFL	= (FormLayer*)m_pDFChild->Delete (0);
		pFL->m_pFData->bOpened	= false;
		removeChild (pFL,false);
	}
	while(m_pTipChild->Count)
	{
		TipFormLayer* pTFL	= (TipFormLayer*)m_pTipChild->Delete (0);
		removeChild (pTFL,false);
	}

}

void InterfaceLayer::update( float dt )
{
	CCLayer::update (dt);

#ifdef WIN32

		pMainInterface->Update ();
		if(pMainToopTip)
			pMainToopTip->Update ();
		_checkForm();
		if(pMainToopTip)
		{
			_checkToopTip();
			_checkGetTip ();
		}
		//TryUpdate();

#else
	try
	{
		pMainInterface->Update ();
		if(pMainToopTip)
			pMainToopTip->Update ();
		_checkForm();
		if(pMainToopTip)
		{
			_checkToopTip();
			_checkGetTip ();
		}
	}
	catch (...)
	{

	}
#endif

}

void InterfaceLayer::_checkForm()
{
	for (int i = 0; i < pMainInterface->m_pListRequest->Count; i++)
	{
		DForm* pForm	= (DForm*)pMainInterface->m_pListRequest->Items[i];
		if(pMainInterface->m_pLockForm && pForm != pMainInterface->m_pLockForm)
			continue;

		FormLayer*	pFLayer	= NULL;
		bool bFind	= false;
		for (int j = m_pDFChild->Count-1; j >= 0; j--)
		{
			FormLayer* pFL	= (FormLayer*)m_pDFChild->Items[j];
			if(pFL->m_bCloseAct==false &&  pFL->m_pFData == pForm)
			{
				pFLayer	= pFL;
				break;
			}
		}

		pMainInterface->m_pListRequest->Delete (i--);

		CCPoint ptRl	= PtIF2Cocos (pForm->nPosX,pForm->nPosY);
		pForm->bChgPos	= false;
		if(pFLayer == NULL)
		{
			pFLayer		= new FormLayer(pForm);
			m_pDFChild->Add (pFLayer);
			pForm->bCocosVisabel	= true;
			addChild (pFLayer,pForm->bZorde);
			pFLayer->RunOpenEffect (ptRl);
		}
		else
			pFLayer->setPosition (ptRl);

		//if(pForm->bLocked == false)
		{
			CCSize	size	= pFLayer->getContentSize ();
			CCRect	rect(ptRl.x,ptRl.y-size.height,size.width,size.height);
			pFLayer->SetRealRect (rect);
		}
		
		
	}
	//pMainInterface->m_pListRequest->Clear ();

	for (int i = m_pDFChild->Count-1; i >= 0; i--)
	{
		FormLayer* pFLayer	= (FormLayer*)m_pDFChild->Items[i];
		if(pFLayer && pFLayer->Update () == false)
		{//后台界面已关闭,前端也进行清理
			if(pFLayer->m_pFData->bLocked == false)
			{
				//CCPoint	ptReal	= ccp(pFLayer->GetRealRect().getMinX(),pFLayer->GetRealRect().getMaxY ());
				//PtCocos2IF (ptReal,pFLayer->m_pFData->nPosX,pFLayer->m_pFData->nPosY);	
			}
			pFLayer->m_pFData->bCocosVisabel	= false;
			removeChild (pFLayer,false);	//true条件下内存破坏
			m_pDFChild->Delete (i);

			CCTextureCache::sharedTextureCache ()->removeUnusedTextures ();
			//SAFE_DELETE(pFLayer);
		}
	}
}

void InterfaceLayer::_checkToopTip()
{
	for (int i = 0; i < pMainToopTip->m_pListRequest->Count; i++)
	{
		ToopTip9Spr*	pTipData	= (ToopTip9Spr*)pMainToopTip->m_pListRequest->Items[i];
		TipFormLayer*	pTipFLayer	= new TipFormLayer(pTipData);
		CCPoint ptRl	= PtIF2Cocos (pTipData->m_nX,pTipData->m_nY);
		pTipFLayer->setPosition (ptRl);
		//pTipFLayer->setAnchorPoint (ccp(0,1));
		addChild (pTipFLayer,99,(int)pTipData);
		m_pTipChild->Add (pTipFLayer);
	}

	pMainToopTip->m_pListRequest->Clear ();

	for (int i = m_pTipChild->Count-1; i >= 0; i--)
	{
		TipFormLayer* pTipFLayer	= (TipFormLayer*)m_pTipChild->Items[i];

		if(pTipFLayer->m_pTipData->m_bChgPos)
		{
			pTipFLayer->m_pTipData->m_bChgPos	= false;
			CCPoint ptRl	= PtIF2Cocos (pTipFLayer->m_pTipData->m_nX,pTipFLayer->m_pTipData->m_nY);
			pTipFLayer->setPosition (ptRl);
			pTipFLayer->m_pTipData->m_pDataSTC->bNeedRefresh	= true;
		}

		if(pTipFLayer->CheckUpdate () == false)
		{
			removeChild(pTipFLayer,true);
			m_pTipChild->Delete (i);
			//delete pTipFLayer;
		}
		//
		//delete pTipData;
		//removeChildByTag ((int)pTipData,false);	//这里删除掉的是NODE
	}
}

cocos2d::CCPoint InterfaceLayer::PtIF2Cocos( int nX, int nY)
{
	return ccp(nX/POINT_PIX_CNT, (SCREEN_SIZE_H*POINT_PIX_CNT - nY)/POINT_PIX_CNT);
}

bool InterfaceLayer::PtCocos2IF( cocos2d::CCPoint ptIn, int& nX, int& nY )
{
	nX	= ptIn.x*POINT_PIX_CNT;
	nY	= (SCREEN_SIZE_H-ptIn.y)*POINT_PIX_CNT;

	return true;
}

void InterfaceLayer::_checkGetTip()
{
	for (int i = 0; i < 4; i++)
	{
		if(m_ppGetTip[i] && m_ppGetTip[i]->numberOfRunningActions () == 0)
		{
			removeChild (m_ppGetTip[i],true);
			m_ppGetTip[i]	= NULL;
		}
	}
}

FormLayer* InterfaceLayer::GetFormLayer( DForm* pForm )
{
	for (int i=0;i<m_pDFChild->Count;i++)
	{
		FormLayer* pFL	= (FormLayer*)m_pDFChild->Items[i];
		if (pFL->m_pFData == pForm)
		{
			return pFL;
		}
	}
	return NULL;
}


void MyTxtSetEditTxt(char* szTxt,int nAlignment,void* pEditParam)
{
	CObjTextInput* pObj	= (CObjTextInput*) pEditParam;
	pObj->SetInputText (szTxt);
}

const char*  MyTxtGetEditTxt(void* pEditParam)
{
	CObjTextInput* pObj	= (CObjTextInput*) pEditParam;
	const char* szTxt	= pObj->GetInputText ();
	return szTxt;
}

CObjTextInput::CObjTextInput( DComponent* pComp)
:MyTextInput(0,0,pComp->nW/POINT_PIX_CNT,pComp->nH/POINT_PIX_CNT,UTEXT_CHINE(pComp->szCaption),pComp->m_nFontSize/POINT_PIX_CNT)
{
	int nRX	= pComp->pParent->nPosX + pComp->nX;
	int nRY	= pComp->pParent->nPosY + pComp->nY;

	SetRect (nRX/POINT_PIX_CNT, SCREEN_SIZE_H-nRY/POINT_PIX_CNT-pComp->nH/POINT_PIX_CNT,pComp->nW/POINT_PIX_CNT,pComp->nH/POINT_PIX_CNT);

	m_nBottomY		= SCREEN_SIZE_H-nRY/POINT_PIX_CNT-pComp->nH/POINT_PIX_CNT-50/POINT_PIX_CNT;
	if(nowRateH)
		m_nBottomY	*= nowRateH;
	m_pComp	= pComp;
	pComp->fnSetEditTxt	= MyTxtSetEditTxt;
	pComp->fnGetEditTxt = MyTxtGetEditTxt;
	pComp->pParam	= this;
	m_bInputType = pComp->byInputType;
	if(m_pComp->szEdit)
		SetInputText (m_pComp->szEdit);
	setAnchorPoint (ccp(0.5*pComp->bAlignX,1-0.5*pComp->bAlignY));
	autorelease ();
}

CObjTextInput::~CObjTextInput()
{
	if(m_pComp->szEdit)
		free(m_pComp->szEdit);
	m_pComp->szEdit	= strdup(GetInputText ());
	m_pComp->fnGetEditTxt	= NULL;
	m_pComp->fnSetEditTxt	= NULL;
	m_pComp->pParam	= NULL;
}

void MyProgressSetInfo(int nVal,int nMax, char* szTxt,void * pParam)
{
	if(nMax == 0)
		nMax = 100;
	CObjProgress* pObj	= (CObjProgress*) pParam;
	pObj->setPercentage (((float)nVal)/nMax*100);

	if(pObj->m_pLightSpr)
	{
		CCSize		size	= pObj->getContentSize ();
		int nPos	= nVal*size.width/nMax;
		CCSize szLight	= pObj->m_pLightSpr->getContentSize ();
		if(nPos < szLight.width)
			pObj->m_pLightSpr->setScaleX (nPos/szLight.width);
		else
			pObj->m_pLightSpr->setScaleX (1);
		pObj->m_pLightSpr->setPositionX(nVal*size.width/nMax);

		pObj->m_pLightSpr->setVisible (nVal);
	}
	
	if(strlen (szTxt))
		pObj->m_pLbInfo->setString (UTEXT_CHINE(szTxt));
	else
	{
		char	szTmp[64];
		sprintf(szTmp,"%d/%d",nVal,nMax);
		pObj->m_pLbInfo->setString (UTEXT_CHINE(szTmp));
	}
}

CObjProgress::CObjProgress( DComponent* pComp )
{
	char	szTmp[256];
	if(pComp->szSpr)
	{
		sprintf(szTmp,"ui/%s",pComp->szSpr);
		initWithSprite  (CCSprite::create (szTmp));
	}

	m_pComp	= pComp;
	setAnchorPoint (ccp(0.5*pComp->bAlignX,1-0.5*pComp->bAlignY));
	autorelease ();
	if(pComp->byInputType == 9)
	{
		setType(kCCProgressTimerTypeRadial);

		setBarChangeRate(ccp(0, 0));
		setMidpoint (ccp(0.5f,0.5f));
		setReverseProgress(true);
	}
	else if(pComp->byInputType == 8)
	{
		setType(kCCProgressTimerTypeBar);
		setBarChangeRate(ccp(0, 1));
		setMidpoint (ccp(0.5,0));
	}
	else
	{
		setType(kCCProgressTimerTypeBar);
		setBarChangeRate(ccp(1, 0));
		setMidpoint (ccp(0,0.5f));
	}
	setPercentage (100);

	m_pComp->fnProgressSetInfo	= MyProgressSetInfo;
	m_pComp->pParam	= this;

	sprintf(szTmp,"100/100");

	m_pLbInfo = CCLabelTTF::create(UTEXT_CHINE(szTmp), DEFAULT_FONT, pComp->m_nFontSize/POINT_PIX_CNT);
	m_pLbInfo->setColor (ccc3(pComp->nR,pComp->nG,pComp->nB));
	CCSize	size	= getContentSize ();
	m_pLbInfo->setPosition(ccp(size.width/2,size.height/2));
	addChild(m_pLbInfo,1);
	
	if(pComp->szSelectSpr && strlen (pComp->szSelectSpr))
	{
		m_pLightSpr	= CCSprite::create(pComp->szSelectSpr);
		if(m_pLightSpr)
		{
			addChild (m_pLightSpr,1);
			m_pLightSpr->setAnchorPoint (ccp(1,0));
			//m_pLightSpr->setcon
			m_pLightSpr->setPositionY (0);
			m_pLightSpr->setPositionX (size.width);
		}
	}
	else
		m_pLightSpr	= NULL;
		
	MyProgressSetInfo(m_pComp->nProVal,m_pComp->nProMax,m_pComp->szCaption,this);
}

CObjProgress::~CObjProgress()
{
	m_pComp->fnProgressSetInfo	= NULL;
}

void MyTxtSetCaption(char* szTxt,int nAlignment,void* pEditParam)
{
	CObjCaption* pObj	= (CObjCaption*) pEditParam;
	pObj->setString (UTEXT_CHINE(szTxt));
	pObj->setHorizontalAlignment ((CCTextAlignment)nAlignment);
	//pObj->setVerticalAlignment ((CCVerticalTextAlignment)nAlignment);
}

void MyTxtSetBMFCaption(char* szTxt,int nAlignment,void* pEditParam)
{
	CBMFCaption* pObj	= (CBMFCaption*) pEditParam;
	pObj->setString (UTEXT_CHINE(szTxt));
}
CObjCaption::CObjCaption( DComponent* pComp )
{
	pComp->fnSetEditTxt		= MyTxtSetCaption;
	pComp->pParam	= this;

	initWithString (UTEXT_CHINE(pComp->szCaption),DEFAULT_FONT, pComp->m_nFontSize/POINT_PIX_CNT);
	setHorizontalAlignment ((CCTextAlignment)pComp->nCaptionAlignment);
	setVerticalAlignment ((CCVerticalTextAlignment)pComp->nCaptionAlignment);
	autorelease ();
	setColor (ccc3(pComp->nR,pComp->nG,pComp->nB));
	setAnchorPoint (ccp(0.5*pComp->bAlignX,1-0.5*pComp->bAlignY));

	setDimensions (CCSize(pComp->nW/POINT_PIX_CNT,pComp->nH/POINT_PIX_CNT));
	m_pComp		= pComp;
}

CObjCaption::~CObjCaption()
{
	m_pComp->fnSetEditTxt		=	NULL;
	m_pComp->pParam	=	NULL;
}

void Fun_SetCartoon(char * szPlist,char* szFile,int nNeedTime, void* pParam, int nScale,bool ifFlip)
{
	CObjCartoon*	pCartoon	= (CObjCartoon*)pParam;

	//if(pCartoon->m_pAction)
		//pCartoon->stopAction (pCartoon->m_pAction);
	pCartoon->stopAllActions ();
	if(strlen (szPlist) == 0)
	{
		if(pCartoon->m_pAction)
			pCartoon->m_pAction->release ();
		pCartoon->m_pAction	= NULL;
		pCartoon->init();
		return;
	}

	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();

	CCAnimation* pAnimation	= CCAnimation::create();
	int nTotalFrm	= 0;
	while(1)
	{
		char	szTmp[256];
		sprintf(szTmp,"%s/%d.png",szFile,nTotalFrm+1);
		CCSpriteFrame* frame = cache->spriteFrameByName( szTmp );

		if(frame == NULL && nTotalFrm == 0)
		{
			if(gfun_CheckSprCache(szPlist,true) == false)
				return;
			cache->addSpriteFramesWithFile((const char*)szPlist);
			frame = cache->spriteFrameByName( szTmp );
			if(frame == NULL)
				return;
		}
		if(nTotalFrm == 0)
		{
			//pCartoon->init();
			pCartoon->initWithSpriteFrame (frame);
		}
		if(frame == NULL)
			break;
		pAnimation->addSpriteFrame(frame);
		nTotalFrm++;
	}
	if(nTotalFrm)
	{
		pAnimation->setDelayPerUnit(nNeedTime/1000.0f/nTotalFrm);
		CCAnimate* action = CCAnimate::create(pAnimation);
		if(pCartoon->m_pAction)	pCartoon->m_pAction->release ();
		pCartoon->m_pAction	= /*CCRepeatForever::create*/(action);
		pCartoon->m_pAction->retain ();
		/*CCAction * pActRtl	= */pCartoon->runAction(pCartoon->m_pAction);
		//pActRtl->setTag (ANIMATE_ACTION_TAG);
	}

	pCartoon->setScale (nScale/100.0);
	if(ifFlip)
		pCartoon->setFlipX(true);
}

CObjCartoon::CObjCartoon( DComponent* pComp )
{
	init ();
	setAnchorPoint (ccp(0.5*pComp->bAlignX,1-0.5*pComp->bAlignY));

	m_pComp	= pComp;
	autorelease ();
	scheduleUpdate();

	pComp->pParam	= this;
	pComp->fnSetCartoon =	Fun_SetCartoon;
	pComp->fnSetShader = MySetShader;
	pComp->fnSetSprColor = Fun_CartoonSetSprColor;
	pComp->fnSetScale = Fun_SetScale;
	m_pAction	= NULL;
	if(pComp->szCAPlist)
		Fun_SetCartoon(pComp->szCAPlist,pComp->szCAFile,pComp->nCATime,this,pComp->nCTScale,false);
	if(pComp->bEnable == false)
		MySetShader(pComp->bEnable?SHADER_NULL:SHADER_GRAY,this);
	Fun_CartoonSetSprColor(pComp->nR,pComp->nG,pComp->nB,this);
}

CObjCartoon::~CObjCartoon()
{
	m_pComp->fnSetCartoon = NULL;
	m_pComp->fnSetScale	= NULL;
	m_pComp->fnSetSprColor	= NULL;
	m_pComp->fnSetShader	= NULL;
	//m_pAction->release ();
}

void CObjCartoon::update( float dt )
{
	if(m_pAction && m_pAction->retainCount ()==1)
		runAction (m_pAction);
}

CSpecialText::CSpecialText( SpecialTextCenter* pSTC ,bool bTClose)
{
	setAnchorPoint (ccp(0,1));

	m_pSTCenter	= pSTC;
	pSTC->bNeedRefresh=true;
	autorelease ();
	nStartY	= 0;
	nMaxHeight	= 0;
	bGetTouchCtrl	= false;
	bLimitView		= true;
	m_bEnableClick	= false;	//让MyDragLayer的事件失效
	nDefaulY	= 0;
	bTouchClose	= bTClose;
}

CSpecialText::~CSpecialText()
{

}

void	STextEventCB(int nEvent,int x, int y, void* pParam)
{
	CCNode*	pNode	= (CCNode*)pParam;
	STextObj*	pSTO	= (STextObj*)pNode->getTag ();
	switch(nEvent)
	{
	case 0:	//按下
		{
			if(pSTO->byType == 0)
			{
				((CCLabelTTF*)pNode)->setColor (ccc3(pSTO->byR/2,pSTO->byG/2,pSTO->byB/2));
			}
			else
			{
				((CCSprite*)pNode)->setColor (ccc3(pSTO->byR/2,pSTO->byG/2,pSTO->byB/2));
			}

			if(pSTO->bTouchEvent)
				if(pSTO->funCBLink)
				{
					_MuliParamObjEx	param;
					param.nParam1	= (int)pSTO->pParam;
					param.nParam2	= 1;
					param.nParam3	= x;
					param.nParam4	= y-pSTO->nViewPosY;
					pSTO->funCBLink (&param);
				}
		}
		break;
	case 1:	//滑动

		break;
	case 2:	//松开
		{
			if(pSTO->byType == 0)
			{
				((CCLabelTTF*)pNode)->setColor (ccc3(pSTO->byR,pSTO->byG,pSTO->byB));
			}
			else
			{
				((CCSprite*)pNode)->setColor (ccc3(pSTO->byR,pSTO->byG,pSTO->byB));
			}

			if(pSTO->funCBLink)
			{
				if(pSTO->bTouchEvent)
				{
					_MuliParamObjEx	param;
					param.nParam1	= (int)pSTO->pParam;
					param.nParam2	= 0;
					param.nParam3	= x;
					param.nParam4	= y-pSTO->nViewPosY-pNode->getParent()->getPositionY()*POINT_PIX_CNT-pNode->getParent()->getContentSize().height*POINT_PIX_CNT/2;
					pSTO->funCBLink (&param);
				}
				else
					pSTO->funCBLink (pSTO->pParam);
			}
		}
		break;
	case 9:
		{
			if(pSTO->byType == 0)
			{
				((CCLabelTTF*)pNode)->setColor (ccc3(pSTO->byR,pSTO->byG,pSTO->byB));
			}
			else
			{
				((CCSprite*)pNode)->setColor (ccc3(pSTO->byR,pSTO->byG,pSTO->byB));
			}
			if(pSTO->bTouchEvent)
				if(pSTO->funCBLink)
				{
					_MuliParamObjEx	param;
					param.nParam1	= (int)pSTO->pParam;
					param.nParam2	= 2;
					param.nParam3	= x;
					param.nParam4	= y+pSTO->nViewPosY;
					pSTO->funCBLink (&param);
				}
		}
		break;
	}

}

void CSpecialText::ResetChildren()
{
	//m_pComp 没有说明策划类型不对
	if(m_pSTCenter == NULL)
		return;
	CCSize size	= getContentSize ();
	if(size.width == 0)
		return;
	int		nCurX	= 0,nCurY	= 0;
	int		nLineH	= 0;
	bInTop	= true;
	bInBottom	= true;
	
	this->ClearRegERect ();
	removeAllChildrenWithCleanup (true);
	for (int i = 0; i < m_pSTCenter->m_pList->Count; i++)
	{
		STextObj*	pSTO	= (STextObj*)m_pSTCenter->m_pList->Items[i];

		if(pSTO->nViewPosY && pSTO->nViewPosY + nStartY > 0)
		{
			bInTop	= false;
			continue;
		}
		if(nCurY+nStartY < -size.height/2)
		{
			bInBottom	= false;
			//break;
		}

		if(pSTO->bChgLine || nCurX >= m_rectReal.getMaxX ())
		{
			nCurX	= 0;
			nCurY	-= nLineH+3;
			nLineH	= 0;
		}
		pSTO->nViewPosY	= nCurY;

		switch(pSTO->byType)
		{
		case 0:		//文本
			{
				int nFontSize	= pSTO->nFontSize/POINT_PIX_CNT+2/POINT_PIX_CNT;
			/*
				if(pSTO->funCBLink)
						nFontSize	= pSTO->nFontSize+2/POINT_PIX_CNT;*/
					

				CCLabelTTF*	pLable	=	CCLabelTTF::create (UTEXT_CHINE(pSTO->szContent),DEFAULT_FONT,nFontSize);
				pLable->setHorizontalAlignment (kCCTextAlignmentLeft);
				//pLable->setDimensions (CCSize(-1,-1));
				pLable->setAnchorPoint (ccp(0,1));
				int nRealX	= nCurX+pSTO->nOffX;
				if(pSTO->nOffX >= 100000)
					nRealX	= pSTO->nOffX%100000;
				pLable->setPosition (ccp(nRealX,nCurY-pSTO->nOffY));
				pLable->setColor (ccc3(pSTO->byR,pSTO->byG,pSTO->byB));
				addChild (pLable,0,(int)pSTO);
				CCSize	sz	= pLable->getContentSize ();
				if(pSTO->funCBLink)
					AddRegEvent (CCRect(nRealX,nCurY-pSTO->nOffY-sz.height,sz.width,sz.height),(void*)pLable,STextEventCB);

				nLineH	=	max(nLineH,sz.height+pSTO->nOffY);

				char*	szCheck	= pSTO->szContent;
				while(sz.width+nCurX+pSTO->nOffX > size.width && sz.width)
				{
					int nContentSize	= strlen(szCheck)-1;
					int nCurLen		= (size.width - nCurX-pSTO->nOffX) * nContentSize / sz.width;
					//需要从头开始找,不能中间截断
					int nCheckPos	= 0;
					while(nCheckPos < nCurLen)
					{
						if(szCheck[nCheckPos] >= 0x80 || szCheck[nCheckPos] < 0)
							nCheckPos+=2;
						else
							nCheckPos+=1;
					}

					char	szTmp[512];
					strncpy (szTmp,szCheck,nCheckPos);
					szTmp[nCheckPos]	= 0;
					pLable->setString (UTEXT_CHINE(szTmp));
					szCheck+=nCheckPos;

					nCurX	= 0;
					nCurY	-= nLineH+3;
					nLineH	= 0;
					if(strlen (szCheck) == 0)
						break;


					pLable	=	CCLabelTTF::create (UTEXT_CHINE(szCheck),DEFAULT_FONT,nFontSize);
					pLable->setHorizontalAlignment (kCCTextAlignmentLeft);
					pLable->setAnchorPoint (ccp(0,1));

					nRealX	= nCurX+pSTO->nOffX;
					if(pSTO->nOffX >= 100000)
						nRealX	= pSTO->nOffX%100000;

					pLable->setPosition (ccp(nRealX,nCurY));
					pLable->setColor (ccc3(pSTO->byR,pSTO->byG,pSTO->byB));
					addChild (pLable,0,(int)pSTO);
					sz	= pLable->getContentSize ();
					if(pSTO->funCBLink)
						AddRegEvent (CCRect(nRealX,nCurY-sz.height,sz.width,sz.height),(void*)pLable,STextEventCB);

					nLineH	=	max(nLineH,sz.height);
				}
				
				//if(sz.width > size.width)
				//{
				//	pLable->setDimensions (CCSize(size.width,0));
				//	sz	= pLable->getContentSize ();
				//	//nCurY	-= sz.height;
				//}
				nCurX	= sz.width+nRealX;
				//nCurX	+= sz.width+pSTO->nOffX;
				
			}
			break;
		case 1:		//图片
			{
				CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
				CCSpriteFrame* frame = cache->spriteFrameByName( pSTO->szContent );
				CCSprite*	pSpr	=	NULL;
				if(frame)
					pSpr	= CCSprite::createWithSpriteFrame (frame);
				else
					pSpr	= CCSprite::create (pSTO->szContent);

				if(pSpr)
				{
					pSpr->setAnchorPoint (ccp(0,1));
					pSpr->setPosition (ccp(nCurX+pSTO->nOffX,nCurY-pSTO->nOffY));
					addChild (pSpr,0,(int)pSTO);
					CCSize	sz	= pSpr->getContentSize ();
					if(pSTO->funCBLink)
						AddRegEvent (CCRect(nCurX+pSTO->nOffX,nCurY-pSTO->nOffY-sz.height,sz.width,sz.height),(void*)pSpr,STextEventCB);
					nCurX	+= sz.width+pSTO->nOffX;
					nLineH	=	max(nLineH,sz.height+pSTO->nOffY);
				}

			}
			break;
		}
		pSTO->bReset	= false;

		if(pSTO->nViewPosY + nStartY > 0)
		{
			bInTop	= false;
		}
	}
	nStartY		= 0;
	nMaxHeight	= (-nCurY)+nLineH+3;
	if(m_pSTCenter->m_bReSetPosY && nMaxHeight > m_rectReal.size.height)
	{
		if(m_pSTCenter->bAutoBottom)
			setPositionY (nMaxHeight+nDefaulY-m_rectReal.size.height);
		if(m_pSTCenter->m_bSetBottom)
		{
			m_pSTCenter->m_bSetBottom = false;
			setPositionY (nMaxHeight+nDefaulY-m_rectReal.size.height);
		}
		if(m_pSTCenter->m_pSlideBack && m_pSTCenter->m_pSlideMove)
		{
			int nNewY = (nMaxHeight-m_rectReal.size.height)*(m_pSTCenter->m_pSlideBack->nH-m_pSTCenter->m_pSlideMove->nH)/(nMaxHeight-m_rectReal.size.height/2) + m_pSTCenter->m_pSlideBack->nY;
			if(nNewY < m_pSTCenter->m_pSlideBack->nY)
				nNewY = m_pSTCenter->m_pSlideBack->nY;
			else if(nNewY > m_pSTCenter->m_pSlideBack->nY + m_pSTCenter->m_pSlideBack->nH - m_pSTCenter->m_pSlideMove->nH)
				nNewY = m_pSTCenter->m_pSlideBack->nY + m_pSTCenter->m_pSlideBack->nH - m_pSTCenter->m_pSlideMove->nH;
			m_pSTCenter->m_pSlideMove->SetPos(m_pSTCenter->m_pSlideMove->nX,nNewY);
		}
	}
	else
	{
		if(m_pSTCenter->bAutoBottom)
			setPositionY (nDefaulY);
		if(m_pSTCenter->m_bSetBottom)
		{
			m_pSTCenter->m_bSetBottom = false;
			setPositionY (nDefaulY);
		}
		if(m_pSTCenter->m_pSlideBack && m_pSTCenter->m_pSlideMove)
		{
			m_pSTCenter->m_pSlideMove->SetPos(m_pSTCenter->m_pSlideMove->nX,m_pSTCenter->m_pSlideBack->nY);
		}
	}
	if(nMaxHeight > m_rectReal.size.height/2)
		m_pSTCenter->m_bMoveSwitch = true;
	else
		m_pSTCenter->m_bMoveSwitch = false;
}

void CSpecialText::ResetPose()
{
	if(m_pSTCenter->m_pSlideBack && m_pSTCenter->m_pSlideMove)
	{
		int nNewY = nDefaulY + (nMaxHeight-m_rectReal.size.height/2)*(m_pSTCenter->m_pSlideMove->nY - m_pSTCenter->m_pSlideBack->nY)/(m_pSTCenter->m_pSlideBack->nH-m_pSTCenter->m_pSlideMove->nH);
		setPositionY(nNewY);
	}
}

void CSpecialText::UpdateChildChg()
{
	if(m_pChildren && m_pChildren->count() > 0)
	{
			int	nCntCC	= m_pChildren->count();
		CCObject* child;
		CCARRAY_FOREACH(m_pChildren, child)
		{
			CCNode* pNode = (CCNode*) child;
			if(pNode && pNode->getTag () != -1)
			{
				STextObj* pSTO	= (STextObj*)pNode->getTag ();
				if (pSTO->bReset)
				{
					switch(pSTO->byType)
					{
					case 0:		//文本
						{
							CCLabelTTF*	pLable	=	(CCLabelTTF*)pNode;
							pLable->setColor (ccc3(pSTO->byR,pSTO->byG,pSTO->byB));
							pLable->setString (UTEXT_CHINE(pSTO->szContent));
						}
						break;
					case 1:		//图片
						{
							CCSprite*	pSpr	=	(CCSprite*)pNode;
							CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
							CCSpriteFrame* frame = cache->spriteFrameByName( pSTO->szContent );
							if(frame)
								pSpr->initWithSpriteFrame (frame);
							else
								if(pSpr->initWithFile (pSTO->szContent)==false)
									pSpr->init();

							pSpr->setAnchorPoint (ccp(0,1));
						}
						break;
					}
					pSTO->bReset	= false;
				}
			}
		}
	}
}

bool CSpecialText::ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	if(g_bIFLayerClicked)
		return false;
	if(m_pSTCenter->bDisableTouch)
		return false;
	CCPoint	pt	= pTouch->getLocation ();
	if(!m_rectReal.containsPoint (pt))
		return false;
	bGetTouchCtrl	= false;
	if(MyDragLayer::ccTouchBegan (pTouch,pEvent) == false)
	{
		bGetTouchCtrl	= true;
		if(bTouchClose)
		{
			g_bIFLayerClicked	= true;
			return true;
		}
		if(bInTop && bInBottom)
			return false;

		g_bIFLayerClicked	= true;
		return true;
	}
	return true;
}

void CSpecialText::ccTouchMoved( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	CCPoint	ptDiff	= pTouch->getDelta ();
	if(ptDiff.y > 3 || ptDiff.y < -3)
		bGetTouchCtrl = true;
	if(bGetTouchCtrl)
	{
		if(ptDiff.y > 0 && bInBottom)
			return;
		if(ptDiff.y < 0 && bInTop)
			return;

		nStartY	+= ptDiff.y;

		CCPoint	pt	= getPosition ();
		setPositionY (pt.y+ptDiff.y);

		if(m_pSTCenter->m_pSlideBack && m_pSTCenter->m_pSlideMove)
		{
			int nNewY = (pt.y +ptDiff.y -nDefaulY)*(m_pSTCenter->m_pSlideBack->nH-m_pSTCenter->m_pSlideMove->nH)/(nMaxHeight-m_rectReal.size.height/2) + m_pSTCenter->m_pSlideBack->nY;
			if(nNewY < m_pSTCenter->m_pSlideBack->nY)
				nNewY = m_pSTCenter->m_pSlideBack->nY;
			else if(nNewY > m_pSTCenter->m_pSlideBack->nY + m_pSTCenter->m_pSlideBack->nH - m_pSTCenter->m_pSlideMove->nH)
				nNewY = m_pSTCenter->m_pSlideBack->nY + m_pSTCenter->m_pSlideBack->nH - m_pSTCenter->m_pSlideMove->nH;
			m_pSTCenter->m_pSlideMove->SetPos(m_pSTCenter->m_pSlideMove->nX,nNewY);
		}

		bInTop	= false;
		bInBottom	= false;
		int nPosY	= getPositionY ();
		if(m_pSTCenter->m_pList->Count)
		{
			STextObj*	pSTO	= (STextObj*)m_pSTCenter->m_pList->Items[0];
			if(nPosY-nDefaulY/*+nStartY*/ <= 0)
				bInTop	= true;
			pSTO	= (STextObj*)m_pSTCenter->m_pList->Items[m_pSTCenter->m_pList->Count-1];
			//if(nPosY-nDefaulY+pSTO->nViewPosY/*+nStartY*/ > -m_rectReal.size.height/2)
			if( (nPosY-nDefaulY) > (nMaxHeight -m_rectReal.size.height/2))
				bInBottom	= true;
		}

	}
	//else//MyDragLayer::ccTouchMoved 里的TAG作为DComp处理,会内存破坏
		//MyDragLayer::ccTouchMoved (pTouch,pEvent);	


}

void CSpecialText::ccTouchEnded( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	g_bIFLayerClicked	= false;
	if(bGetTouchCtrl)
	{
		bGetTouchCtrl	= false;
		if(m_pCurERect)
		{
			CCPoint	ptCur	= convertTouchToNodeSpace(pTouch);
			m_pCurERect->fnEventCB(9,ptCur.x-m_pCurERect->rect.getMinX (),m_pCurERect->rect.getMaxY ()-ptCur.y,m_pCurERect->pParam);
			m_pCurERect	= NULL;
		}
	}
	else
		MyDragLayer::ccTouchEnded (pTouch,pEvent);
}

void CSpecialText::visit( void )
{
	if(bLimitView)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor (m_rectReal.origin.x*POINT_PIX_CNT,m_rectReal.origin.y*POINT_PIX_CNT,m_rectReal.size.width*POINT_PIX_CNT,m_rectReal.size.height*POINT_PIX_CNT);
		MyDragLayer::visit();

		glDisable(GL_SCISSOR_TEST);
	}
	else
		MyDragLayer::visit();
}

TipFormLayer::TipFormLayer( ToopTip9Spr* pTipData )
{
	m_pTipData	= pTipData;
	setContentSize (CCSize(pTipData->m_nW/POINT_PIX_CNT,pTipData->m_nH/POINT_PIX_CNT));
	_make9Sprite2Node(pTipData->m_nW/POINT_PIX_CNT,pTipData->m_nH/POINT_PIX_CNT);
	//addChild (pNode,1);

	m_pSpecialText	= new CSpecialText(pTipData->m_pDataSTC,m_pTipData->m_bType &0x01);
	addChild (m_pSpecialText,1);
	m_pSpecialText->setPosition (ccp(10/POINT_PIX_CNT,-20/POINT_PIX_CNT));	//字体所处的区域位置
	m_pSpecialText->setContentSize (CCSize(pTipData->m_nW/POINT_PIX_CNT-20/POINT_PIX_CNT,pTipData->m_nH/POINT_PIX_CNT-20/POINT_PIX_CNT));

	autorelease ();

}

TipFormLayer::~TipFormLayer()
{

}


void TipFormLayer::_reset9SpritePos( int nW, int nH )
{
	if(strcmp(m_pTipData->m_sz9Spr,"9spr1")==0)
	{
		//CCSprite* pSpr	= (CCSprite*)getChildByTag (1);
		//pSpr->setPositionY (-nH);
		return;
	}
	CCSprite* pSpr	= (CCSprite*)getChildByTag (7);
	pSpr->setPositionY (-nH);
	CCSize	szSigle	= pSpr->getContentSize ();

	pSpr	= (CCSprite*)getChildByTag (9);
	pSpr->setPositionY (-nH);

	pSpr	= (CCSprite*)getChildByTag (4);
	pSpr->setScaleY (nH/szSigle.height-2);

	pSpr	= (CCSprite*)getChildByTag (5);
	pSpr->setScaleY (nH/szSigle.height-2);

	pSpr	= (CCSprite*)getChildByTag (6);
	pSpr->setScaleY (nH/szSigle.height-2);

	pSpr	= (CCSprite*)getChildByTag (8);
	pSpr->setPositionY (-nH);
}

void TipFormLayer::_make9Sprite2Node( int nW, int nH)
{
	if(strcmp(m_pTipData->m_sz9Spr,"9spr1")==0)
	{
		char	szSpr[256];
		sprintf(szSpr,"other/%s/1.png",m_pTipData->m_sz9Spr);
		CCSprite* pSpr	= CCSprite::create (szSpr);
		addChild (pSpr,-1,1);
		pSpr->setAnchorPoint (ccp(0,1));
		pSpr->setPosition (ccp(nW/2,0));
		return;
	}
	char	szSpr[256];
	sprintf(szSpr,"other/%s/1.png",m_pTipData->m_sz9Spr);
	CCSprite* pSpr	= CCSprite::create (szSpr);
	addChild (pSpr,-1,1);
	pSpr->setAnchorPoint (ccp(0,1));
	pSpr->setPosition (ccp(0,0));
	CCSize	szSingle	= pSpr->getContentSize ();


	//pSprFrm	= cache->spriteFrameByName("other/9spr2/3.png");
	//pSpr	= CCSprite::createWithSpriteFrame (pSprFrm);
	sprintf(szSpr,"other/%s/3.png",m_pTipData->m_sz9Spr);
	pSpr	= CCSprite::create (szSpr);
	addChild (pSpr,-1,3);
	pSpr->setAnchorPoint (ccp(1,1));
	pSpr->setPosition (ccp(nW,0));

	sprintf(szSpr,"other/%s/7.png",m_pTipData->m_sz9Spr);
	pSpr	= CCSprite::create (szSpr);
	addChild (pSpr,-1,7);
	pSpr->setAnchorPoint (ccp(0,0));
	pSpr->setPosition (ccp(0,-nH));

	sprintf(szSpr,"other/%s/9.png",m_pTipData->m_sz9Spr);
	pSpr	= CCSprite::create (szSpr);
	addChild (pSpr,-1,9);
	pSpr->setAnchorPoint (ccp(1,0));
	pSpr->setPosition (ccp(nW,-nH));

	sprintf(szSpr,"other/%s/2.png",m_pTipData->m_sz9Spr);
	pSpr	= CCSprite::create (szSpr);
	addChild (pSpr,-2,2);
	pSpr->setAnchorPoint (ccp(0,1));
	CCSize	sz	= pSpr->getContentSize ();
	pSpr->setScaleX (nW/szSingle.width - 2);
	pSpr->setPosition (ccp(szSingle.width,0));

	sprintf(szSpr,"other/%s/4.png",m_pTipData->m_sz9Spr);
	pSpr	= CCSprite::create (szSpr);
	addChild (pSpr,-2,4);
	pSpr->setAnchorPoint (ccp(0,1));
	sz	= pSpr->getContentSize ();
	pSpr->setScaleY ((nH)/szSingle.height - 2);
	pSpr->setPosition (ccp(0,-szSingle.height));

	sprintf(szSpr,"other/%s/5.png",m_pTipData->m_sz9Spr);
	pSpr	= CCSprite::create (szSpr);
	addChild (pSpr,-3,5);
	pSpr->setAnchorPoint (ccp(0,1));
	sz	= pSpr->getContentSize ();
	pSpr->setScaleX (nW/szSingle.width - 2);
	pSpr->setScaleY ((nH)/szSingle.height - 2);
	pSpr->setPosition (ccp(szSingle.width,-szSingle.height));

	sprintf(szSpr,"other/%s/6.png",m_pTipData->m_sz9Spr);
	pSpr	= CCSprite::create (szSpr);
	addChild (pSpr,-2,6);
	pSpr->setAnchorPoint (ccp(1,1));
	sz	= pSpr->getContentSize ();
	pSpr->setScaleY ((nH)/szSingle.height - 2);
	pSpr->setPosition (ccp(nW,-szSingle.height));

	sprintf(szSpr,"other/%s/8.png",m_pTipData->m_sz9Spr);
	pSpr	= CCSprite::create (szSpr);
	addChild (pSpr,-2,8);
	pSpr->setAnchorPoint (ccp(0,0));
	sz	= pSpr->getContentSize ();
	pSpr->setScaleX(nW/szSingle.width - 2);
	pSpr->setPosition (ccp(szSingle.width,-nH));
}

bool TipFormLayer::CheckUpdate()
{
	if(m_pTipData->m_bRequestDel)
	{
		m_pTipData->m_bRequestDel	= false;
		m_pTipData->m_bIsDel	= true;
		return false;
	}

	if((m_pSpecialText->bGetTouchCtrl && m_pTipData->m_bType &0x01)
		|| (m_pTipData->m_bType &0x02 && m_pTipData->m_dwMarkVal && (DWORD)(xnGetTickCount()) > m_pTipData->m_dwMarkVal)
		|| m_pTipData->m_bIsDel)
	{//点击关闭
		m_pTipData->m_bIsDel	= true;
		return false;
	}
	else if(m_pTipData->m_pDataSTC->bNeedRefresh)
	{
		m_pSpecialText->ResetChildren ();

		m_pTipData->m_nH	= min((m_pSpecialText->nMaxHeight+40/POINT_PIX_CNT)*POINT_PIX_CNT,SCREEN_SIZE_H*POINT_PIX_CNT-60);
		if (m_pTipData->m_nY+m_pTipData->m_nH>SCREEN_SIZE_H*POINT_PIX_CNT)
		{
			setPositionY(min(m_pTipData->m_nH/POINT_PIX_CNT,SCREEN_SIZE_H));
		}
		CCPoint ptRl	= getPosition();
		CCRect	rect(ptRl.x+10/POINT_PIX_CNT,ptRl.y-m_pTipData->m_nH/POINT_PIX_CNT+10/POINT_PIX_CNT,m_pTipData->m_nW/POINT_PIX_CNT-20/POINT_PIX_CNT,m_pTipData->m_nH/POINT_PIX_CNT-20/POINT_PIX_CNT);
		
		//if(nowScreenWidth<640||nowScreenHeight<960){
		//	rect.setRect (rect.origin.x*nowRateW,rect.origin.y*nowRateH,rect.size.width*nowRateW,rect.size.height*nowRateH);
		//}
		m_pSpecialText->SetRealRect (rect);
		m_pSpecialText->setContentSize (CCSize(m_pTipData->m_nW/POINT_PIX_CNT-20/POINT_PIX_CNT,m_pTipData->m_nH/POINT_PIX_CNT-20/POINT_PIX_CNT));

		setContentSize (CCSize(m_pTipData->m_nW/POINT_PIX_CNT,m_pTipData->m_nH/POINT_PIX_CNT));
		_reset9SpritePos(m_pTipData->m_nW/POINT_PIX_CNT,m_pTipData->m_nH/POINT_PIX_CNT);

		m_pTipData->m_pDataSTC->bNeedRefresh	= false;
	}
	else
		m_pSpecialText->UpdateChildChg();
	return true;
}


//CMyLabShade * CMyLabShade::create( const char *string, const char *fontName, float fontSize )
//{
//	CMyLabShade *pRet = new CMyLabShade();
//	//pRet->m_pLbBack	= CCLabelTTF::create (string,fontName,fontSize);
//	if(pRet && pRet->initWithString(string, fontName, fontSize, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentBottom))
//	{
//		pRet->autorelease();
//		
//		//pRet->m_pLbBack->setColor (ccc3(0,0,0));
//		//pRet->m_pLbBack->setOpacity (180);
//		//pRet->m_pLbBack->setAnchorPoint (ccp(0,0));
//		//pRet->m_pLbBack->setPosition (ccp(1.0/POINT_PIX_CNT,-1.0/POINT_PIX_CNT));
//		//pRet->addChild (pRet->m_pLbBack,-1);
//
//		return pRet;
//	}
//	CC_SAFE_DELETE(pRet);
//	return NULL;
//}
//
//void CMyLabShade::setString( const char *label )
//{
//	CCLabelTTF::setString (label);
//	//m_pLbBack->setString (label);
//}
//void CMyLabShade::setHorizontalAlignment( cocos2d::CCTextAlignment alignment )
//{
//	CCLabelTTF::setHorizontalAlignment (alignment);
//	//m_pLbBack->setHorizontalAlignment (alignment);
//}
//
//void CMyLabShade::setVerticalAlignment( cocos2d::CCVerticalTextAlignment verticalAlignment )
//{
//	CCLabelTTF::setVerticalAlignment (verticalAlignment);
//	//m_pLbBack->setVerticalAlignment (verticalAlignment);
//}
//
//void CMyLabShade::setDimensions( const CCSize &dim )
//{
//	CCLabelTTF::setDimensions (dim);
//	//m_pLbBack->setDimensions (dim);
//}
//
//CMyLabShade::~CMyLabShade()
//{
//
//}
//
//void CMyLabShade::setOpacity( GLubyte opacity )
//{
//	cocos2d::CCLabelTTF::setOpacity(opacity);
//	//if(m_pLbBack) m_pLbBack->setOpacity(opacity*180/255);
//}


#ifdef WIN3211
void NewTxtSetEditTxt(char* szTxt,int nAlignment,void* pEditParam)
{
	CNewTextInput* pObj	= (CNewTextInput*) pEditParam;
	if(szTxt == NULL)
		return;
	if(strlen (szTxt) > pObj->pEditBox->getMaxLength())
		pObj->pEditBox->setMaxLength(strlen (szTxt));

    pObj->pEditBox->setText(UTEXT_CHINE(szTxt));
    //pObj->pEditBox->set
    /*
	if (pObj->m_pComp->byInputType == INPUT_TYPE_PASS)
	{
		if (pObj->szText)
		{
			free(pObj->szText);
		}
		pObj->szText = strdup(szTxt);
		int nLen = strlen(szTxt);
		char szText[1024];
		szText[0] = '\0';
		for (int i=0;i<nLen;i++)
		{
			sprintf(szText+strlen(szText),"*");
		}

		pObj->SetInputText (szText);
	}
	else
		pObj->SetInputText (szTxt);
     */
}

const char*  NewTxtGetEditTxt(void* pEditParam)
{
	CNewTextInput* pObj	= (CNewTextInput*) pEditParam;
    return GBK_CHINE(pObj->pEditBox->getText());
	//if (pObj->m_pComp->byInputType == INPUT_TYPE_PASS)
	//{
		//const char* szTxt	= pObj->szText;
		//return szTxt;
	//}
	//const char* szTxt	= pObj->GetInputText ();
	//return szTxt;
}

CNewTextInput::CNewTextInput( DComponent * pComp )
{
	int nFontH	= pComp->nH/POINT_PIX_CNT;
	int	nChgH	= 0;
	if(nFontH < 28)
	{
		nChgH	= 28-nFontH;
		nFontH	= 28;
	}
    
	pEditBox	= extension::CCEditBox::create(CCSizeMake(pComp->nW/POINT_PIX_CNT,nFontH), extension::CCScale9Sprite::create("ui/EditBox_Back.png"));
	//m_pCCInput->setVerticalAlignment (kCCVerticalTextAlignmentCenter);
    
    
    pEditBox->setPosition(ccp((pComp->nX+pComp->pParent->nPosX+pComp->nW/2)/POINT_PIX_CNT,SCREEN_SIZE_H-(pComp->nY+pComp->pParent->nPosY)/POINT_PIX_CNT-nFontH/2+nChgH));
    //pEditBox->setAnchorPoint (ccp(0,1));
    pEditBox->setFontColor(ccc3(22,222,22));
    if(pComp->szCaption)
        pEditBox->setPlaceHolder(pComp->szCaption);
    pEditBox->setPlaceholderFontColor(ccc3(111,150,130));
    pEditBox->setMaxLength(pComp->nMaxChar);
    
    switch (pComp->byInputType)
    {
    case 2:
        pEditBox->setInputMode(extension::kEditBoxInputModeNumeric);
        pEditBox->setInputFlag(extension::kEditBoxInputFlagSensitive);
        break;
    case 3:
        pEditBox->setInputMode(extension::kEditBoxInputModeSingleLine);
        pEditBox->setInputFlag(extension::kEditBoxInputFlagPassword);
        break;
    default:
        pEditBox->setInputMode(extension::kEditBoxInputModeSingleLine);
        pEditBox->setInputFlag(extension::kEditBoxInputFlagInitialCapsAllCharacters);
        break;
    }
    pEditBox->setDelegate(this);
    addChild(pEditBox);
	//addChild ();

	m_pComp	= pComp;
	pComp->fnSetEditTxt	= NewTxtSetEditTxt;
	pComp->fnGetEditTxt = NewTxtGetEditTxt;
	pComp->pParam	= this;
	//m_bInputType = pComp->byInputType;
	if(m_pComp->szEdit)
		NewTxtSetEditTxt (pComp->szEdit,pComp->nCaptionAlignment,(void*)this);

	autorelease ();
}

CNewTextInput::~CNewTextInput()
{
	m_pComp->fnSetEditTxt	= NULL;
	m_pComp->fnGetEditTxt = NULL;
}

void CNewTextInput::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
    if(m_pComp->byInputType == 2)
    {
        const char * szOldStr = GBK_CHINE(editBox->getText());
        char*    szTmp  = strdup(szOldStr);
        for (int i = 0; i < strlen(szTmp); i++) {
            if(szTmp[i] < '1' || szTmp[i] > '9')
            {
                szTmp[i]    = 0;
                editBox->setText(UTEXT_CHINE(szTmp));
                free(szTmp);
                return;
            }
        }
        free(szTmp);
    }

}
#endif

CBMFCaption::CBMFCaption( DComponent* pComp )
{
	pComp->fnSetEditTxt		= MyTxtSetBMFCaption;
	pComp->pParam	= this;

	initWithString (UTEXT_CHINE(pComp->szCaption),"ui/futura-48.fnt");
	autorelease ();
	m_pComp		= pComp;
}

CBMFCaption::~CBMFCaption()
{
	m_pComp->fnSetEditTxt		=	NULL;
	m_pComp->pParam	=	NULL;
}

CSpBtnSprite::CSpBtnSprite( DComponent* pComp )
	:CObjSSprite(pComp)
{

}

CSpBtnSprite::~CSpBtnSprite()
{

}

void CSpBtnSprite::visit( void )
{
	glEnable(GL_SCISSOR_TEST);
	glScissor (rectView.origin.x*POINT_PIX_CNT,rectView.origin.y*POINT_PIX_CNT,rectView.size.width*POINT_PIX_CNT,rectView.size.height*POINT_PIX_CNT);
	CObjSSprite::visit();

	glDisable(GL_SCISSOR_TEST);
}

void CSpBtnSprite::EventCallBack( int nEvent,int x, int y, void* pParam )
{
	CSpBtnSprite* pBtn	= (CSpBtnSprite*)pParam;

	if(nEvent != INTERFACE_EVENT_UP)	//弹起事件不受影响
		if(pMainInterface->m_pLockForm && pMainInterface->m_pLockForm != pBtn->m_pComp->pParent
			&& pMainInterface->m_pLockForm->bZorde >= pBtn->m_pComp->pParent->bZorde)
			return;	//界面锁定功能

	static	InterfaceEvent	IEvent;

	IEvent.x		= x*POINT_PIX_CNT;
	IEvent.y		= y*POINT_PIX_CNT;
	IEvent.pComponent	= pBtn->m_pComp;
	switch(nEvent)
	{
	case 0:
		IEvent.event	= INTERFACE_EVENT_DOWN;
		break;
	case 1:
		IEvent.event	= INTERFACE_EVENT_MOVE;
		break;
	case 2:
		IEvent.event	= INTERFACE_EVENT_UP;
		break;
	}

	if(pBtn->m_pComp->eventFunc)
	{
		pBtn->m_pComp->eventFunc (&IEvent,pBtn->m_pComp->pCbParam);
	}
}

LoopLayer::LoopLayer()
{
	m_pOutAction = CCFadeOut::create(1.0f);
}

void LoopLayer::SetSize( int nSize )
{
	m_nSize = nSize;
}

void LoopLayer::setPosition( const CCPoint& pos )
{
// 	CCPoint chgPoint = ccpSub(pos,getPosition());
// 	if (m_bLockX)
// 	{
// 		chgPoint.x = 0;
// 	}
// 	if (m_bLockY)
// 	{
// 		chgPoint.y = 0;
// 	}
	FrameLayer::setPosition(pos);
// 	CCObject* child;
// 	CCARRAY_FOREACH(getChildren(), child){
// 		CCNode* pCNode = (CCNode*) child;
// 		pCNode->setPosition(ccpAdd(pCNode->getPosition(),chgPoint));
// 		
// 	}
}
//
//CCGLProgramManage::CCGLProgramManage()
//{
//	
//}
//
//CCGLProgramManage::~CCGLProgramManage()
//{
//	pNull->release();
//	pEnable->release();
//}
//
//void CCGLProgramManage::init()
//{
//	pNull  = new CCGLProgram();
//	pNull->initWithVertexShaderByteArray(ccPositionTextureColor_vert, ccPositionTextureColor_frag);
//
//// 	pNull->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
//// 	pNull->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
//// 	pNull->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);//
//
//	static GLchar * pszFragSource = 
//		"#ifdef GL_ES \n \
//		precision mediump float; \n \
//		#endif \n \
//		uniform sampler2D u_texture; \n \
//		varying vec2 v_texCoord; \n \
//		varying vec4 v_fragmentColor; \n \
//		void main(void) \n \
//		{ \n \
//		// Convert to greyscale using NTSC weightings \n \
//		float grey = dot(texture2D(u_texture, v_texCoord).rgb, vec3(0.299, 0.587, 0.114)); \n \
//		gl_FragColor = vec4(grey, grey, grey, texture2D(u_texture, v_texCoord).a); \n \
//		}";
//	pEnable = new CCGLProgram();
//	pEnable->initWithVertexShaderByteArray(ccPositionTextureColor_vert, pszFragSource);
//
//// 	pEnable->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
//// 	pEnable->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
//// 	pEnable->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);//
//}
