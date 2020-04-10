#include "MyTextInput.h"
#include "GlobalDefine.h"
#include "Interface.h"

using namespace cocos2d;

//////////////////////////////////////////////////////////////////////////
// implement KeyboardNotificationLayer
//////////////////////////////////////////////////////////////////////////

//static CCRect getRect(CCNode * pNode)
//{
//	CCRect rc;
//	rc.origin = pNode->getPosition();
//	rc.size = pNode->getContentSize();
//	rc.origin.x -= rc.size.width / 2;
//	rc.origin.y -= rc.size.height / 2;
//	return rc;
//}

static CCSprite* s_pOldFlash	= NULL;

KeyboardNotificationLayer::KeyboardNotificationLayer()
	: m_pTrackNode(0)
{
	setTouchEnabled(true);
}

void KeyboardNotificationLayer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, -20, true);
}

void KeyboardNotificationLayer::keyboardWillShow(CCIMEKeyboardNotificationInfo& info)
{
	if (! m_pTrackNode || m_pTrackNode->getTag() == 0)
	{
		return;
	}
	// if the keyboard area doesn't intersect with the tracking node area, nothing need to do.
	int	nMaxY	= info.end.getMaxY();

	if(m_nBottomY < nMaxY)
	{
		float adjustVert = nMaxY-m_nBottomY;

		CCDirector* pDirector = CCDirector::sharedDirector();
		CCActionInterval* pMove	= CCMoveTo::create ((float)0.2,ccp(0,adjustVert));
		pDirector->getRunningScene ()->runAction (pMove);
	}
	// move all the children node of KeyboardNotificationLayer
}

void KeyboardNotificationLayer::keyboardWillHide( cocos2d::CCIMEKeyboardNotificationInfo& info )
{
	if(m_pTrackNode)
		m_pTrackNode->setTag(0);
	CCDirector* pDirector = CCDirector::sharedDirector();
	CCActionInterval* pMove	= CCMoveTo::create ((float)0.2,ccp(0,0));
	pDirector->getRunningScene ()->runAction (pMove);

	if(s_pOldFlash)
		s_pOldFlash->setPositionX (30000);
}

// CCLayer function

bool KeyboardNotificationLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if(m_bActive == false)
		return false;
	m_beginPos = pTouch->getLocation();
	//if(m_bActive)
	//	return true;
	//CCPoint point = convertTouchToNodeSpaceAR(pTouch);
	return onClickTrackNode(m_rcRect.containsPoint(m_beginPos/*point*/));
	//return m_rcRect.containsPoint(m_beginPos/*point*/);
}

void KeyboardNotificationLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	return ;
	if (! m_pTrackNode)
	{
		return;
	}

	CCPoint endPos = pTouch->getLocation();    

	//float delta = 5.0f;
	////手指滑动了下就不算点击
	//if (::abs(endPos.x - m_beginPos.x) > delta
	//	|| ::abs(endPos.y - m_beginPos.y) > delta)
	//{
	//	// not click
	//	m_beginPos.x = m_beginPos.y = -1;
	//	return;
	//}

	// decide the trackNode is clicked.
	//CCPoint point = convertTouchToNodeSpaceAR(pTouch);
	this->onClickTrackNode(m_rcRect.containsPoint(endPos/*point*/));
}

void KeyboardNotificationLayer::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	//onClickTrackNode(false);
	//CCDirector* pDirector = CCDirector::sharedDirector();
	//CCActionInterval* pMove	= CCMoveTo::create ((float)0.2,ccp(0,0));
	//pDirector->getRunningScene ()->runAction (pMove);
	m_bActive	= false;
}

void KeyboardNotificationLayer::onEnter()
{
	CCLayer::onEnter();
	m_bActive	= true;
}


//--------------------------------------------------------------

MyTextInput::MyTextInput(int x,int y, int w, int h,char* szTmp,int nFontSize):bNewline(false)
{
	m_pBackCCInput = new std::string("");
	nFontSize	= 24/POINT_PIX_CNT;
	m_pCCInput	= CCTextFieldTTF::textFieldWithPlaceHolder(szTmp,DEFAULT_FONT,nFontSize);
	m_pCCInput->setVerticalAlignment (kCCVerticalTextAlignmentCenter);
	addChild (m_pCCInput);
	//m_pCCInput->setPosition(ccp(0,0));
	m_pCCInput->setPosition(ccp(x-w/2-5,y+h/2-5));
	m_nDifX	= w/2;
	m_pCCInput->setDelegate(this);
	m_pCCInput->setAnchorPoint (ccp(0,1));
	m_pCCInput->setColor(ccc3(255,255,255));
	
	SetRect(x,y-h,w,h);
	//m_pCCInput->setDimensions (CCSize(w,h));
	m_pTrackNode	= m_pCCInput;
	m_pCCInput->setTag(0);
	m_pCCInput->setDelegate(this);

	BYTE*	pBuf	= (BYTE*)malloc(2*nFontSize*POINT_PIX_CNT*4);
	CCSize	size(2,nFontSize*POINT_PIX_CNT);
	memset(pBuf,255,2*nFontSize*POINT_PIX_CNT*4);
	m_pText2D	= new CCTexture2D();
	m_pText2D->initWithData (pBuf,kCCTexture2DPixelFormat_RGBA8888,2,nFontSize*POINT_PIX_CNT,size);
	
	m_pSprFlash	= CCSprite::createWithTexture (m_pText2D);
	//m_pSprFlash->setPosition (ccp(100,-10/POINT_PIX_CNT));
	m_pSprFlash->setPosition(ccp(-m_nDifX,0));
	addChild (m_pSprFlash);
	m_pSprFlash->runAction (CCRepeatForever::create(CCBlink::create (10,10)));
	m_pSprFlash->setPositionX (10000);
}

MyTextInput::~MyTextInput()
{
	delete m_pText2D;
	delete m_pBackCCInput;
}

bool MyTextInput::onClickTrackNode( bool bClicked )
{
	if(bClicked)
	{
		m_pCCInput->setTag(m_nBottomY);
		if(s_pOldFlash)
			s_pOldFlash->setPositionX (30000);
		m_pCCInput->attachWithIME ();
		//m_pCCInput->setPlaceHolder("");
		if(m_pBackCCInput->length() == 0)
			m_pSprFlash->setPositionX (-m_nDifX);
		else
			m_pSprFlash->setPositionX (GetTextWidth()-m_nDifX);
		s_pOldFlash	= m_pSprFlash;

	}
	else
	{
		m_pCCInput->setTag(0);
		m_pCCInput->detachWithIME ();
		m_pSprFlash->setPositionX (30000);
		if(s_pOldFlash)
			s_pOldFlash->setPositionX (30000);
		s_pOldFlash	= NULL;
	}
	return bClicked;
}

bool MyTextInput::onTextFieldAttachWithIME( CCTextFieldTTF * sender )
{
#ifdef USE_ANDROID
	sender->setPlaceHolder("");
	int	nMaxY	= min(SCREEN_SIZE_H/2,300);

	if(m_nBottomY < nMaxY)
	{
		float adjustVert = nMaxY-m_nBottomY;

		CCDirector* pDirector = CCDirector::sharedDirector();
		if(pDirector->getRunningScene ()->getPositionY() ==0 )
		{
			CCActionInterval* pMove	= CCMoveTo::create ((float)0.2,ccp(0,adjustVert));
			pDirector->getRunningScene ()->runAction (pMove);
		}
	}
#endif
	return false;
}

bool MyTextInput::onTextFieldDetachWithIME( CCTextFieldTTF * sender )
{
#ifdef USE_ANDROID
	CCDirector* pDirector = CCDirector::sharedDirector();
	CCActionInterval* pMove	= CCMoveTo::create ((float)0.2,ccp(0,0));
	pDirector->getRunningScene ()->runAction (pMove);
#endif
	return false;
}

bool MyTextInput::onTextFieldInsertText( CCTextFieldTTF * sender, const char * text, int nLen )
{
	if (nLen==0)
		return true;
	if(strcmp(text,"\n")==0)
	{
		m_pCCInput->detachWithIME ();
		m_pSprFlash->setPositionX (30000);
		s_pOldFlash	= NULL;
		return true;
	}

	switch (m_bInputType)
	{
	case INPUT_TYPE_NULL:
		{
			return true;
		}
		break;
	case INPUT_TYPE_ALL:
		{

		}
		break;
	case INPUT_TYPE_NUM:
		{
			for (int i=0;i<nLen;i++)
			{
				if (text[i]<'0'||text[i]>'9')
				{
					return true;
				}
			}
			const char * szOldStr = sender->getString();
			if (szOldStr)
			{
				if (szOldStr[0]>='1'&&szOldStr[0]<='9' )
				{

				}
				else if(text[0]>='1'&&text[0]<='9')
				{
					sender->setString("");
				}
				else
				{
					return true;
				}
			}
		}
		break;
	}

	char*	szTmp	= new char[strlen(sender->getString ()) + nLen+1];
	sprintf(szTmp,"%s%s",sender->getString (),text);
	//sender->setString (szTmp);

	char*	szTmpB	= new char[strlen(m_pBackCCInput->c_str()) + nLen+1];
	sprintf(szTmpB,"%s%s",m_pBackCCInput->c_str(),text);

	m_pBackCCInput = new std::string(szTmpB); 

	std::string* localText =  new std::string(szTmp);  
	if(GetTextWidth() > m_rcRect.size.width)
	{  
		if (bNewline)
		{
			int	nWidth = sender->getContentSize().width/sender->getFontSize()*2-2;
			int nPos = localText->find_last_of("\n");
			std::string nowText = *localText;
			if (nPos != std::string::npos)
			{
				if(localText->size() - nPos >=nWidth+1)
					nowText = localText->insert(nPos+nWidth+1,"\n"); 
			}else{
				if(localText->size()>=nWidth)
					nowText = localText->insert(nWidth,"\n"); 
			}
			sender->setString(nowText.c_str());
			int nLine = (localText->size()/(nWidth+1));
			m_pSprFlash->setPositionY (-nLine*(sender->getFontSize()+7));
			float nX = (nowText.size()%(nWidth+1))*(sender->getFontSize()+2)/2;
			m_pSprFlash->setPositionX (nX-m_nDifX);
			delete[] szTmp;
			delete[] szTmpB;
			delete localText;
			return true;
		}else{
			float gap = localText->size() - nLen;  
			std::string nowText = localText->substr(nLen,gap);  
			sender->setString(nowText.c_str()); 
		}	
	}else{   
		sender->setString(szTmp);  
	} 

	m_pSprFlash->setPositionX (GetTextWidth()-m_nDifX);
	delete[] szTmp;
	delete[] szTmpB;
	delete localText;
	return true;
}

bool MyTextInput::onTextFieldDeleteBackward( CCTextFieldTTF * sender, const char * delText, int nLen )
{
	char*	szTmp	= strdup(sender->getString ());
	//szTmp[strlen (szTmp)-nLen]	= 0;
	//sender->setString (szTmp);

	std::string* nowText =  new std::string(szTmp);
	std::string* localText = m_pBackCCInput;  
	if(localText->size() > nowText->size())
	{  
		float fStartCur = localText->size() - nowText->size() - nLen;
		const std::string text = localText->substr(fStartCur,nowText->size());  
		sender->setString(text.c_str());

		char*	szBackTmp	= strdup(localText->c_str());
		szBackTmp[strlen (szBackTmp)-nLen]	= 0;
		m_pBackCCInput = new std::string(szBackTmp);
		free(szBackTmp);

	}else{  
		szTmp[strlen (szTmp)-nLen]	= 0;
		sender->setString(szTmp);

		char*	szBackTmp	= strdup(localText->c_str());
		szBackTmp[strlen (szBackTmp)-nLen]	= 0;
		m_pBackCCInput = new std::string(szBackTmp);
		free(szBackTmp);
		//m_pBackCCInput = new std::string(szTmp);
	} 

	if (m_bInputType == INPUT_TYPE_NUM)
	{
		if (strlen(sender->getString())==0)
		{
			sender->setString("0");
		}
	}
	if(m_pBackCCInput->length() == 0)
		m_pSprFlash->setPositionX (-m_nDifX);
	else
		m_pSprFlash->setPositionX (GetTextWidth()-m_nDifX);
	free(szTmp);
	delete nowText;
	delete localText;
	return true;
}

bool MyTextInput::onDraw( CCTextFieldTTF * sender )
{
	//密码显示为*  可以这边处理,还有光标的显示
	if(m_bInputType == INPUT_TYPE_PSWD)
	{
		char	szTmp[32]	= {0};
		for (int i = 0; i < m_pBackCCInput->length() && i < 12; i++)
		{
			szTmp[i]	= '*';
		}
		m_pCCInput->setString (szTmp);
	}

	return false;
}

void MyTextInput::SetInputText( const char* szTmp )
{
	m_pBackCCInput = new std::string(UTEXT_CHINE(szTmp));
	{
		m_pCCInput->setString (UTEXT_CHINE(szTmp));
		int nWidth = GetTextWidth();
		
		if(nWidth > m_rcRect.size.width)
		{
			std::string strTemp(szTmp);
			int nTotal = strTemp.size();
			int nCutLen = strTemp.size()*(nWidth - m_rcRect.size.width)/nWidth;
			//检查字符，防止截半个汉字
			int nCount = 0;	
			for(int i = 0; i < nCutLen; i ++)
			{
				if(szTmp[i] < 0)
					nCount ++;
			}
			if(nCount % 2 == 1)
				nCutLen ++;
			m_pCCInput->setString(UTEXT_CHINE(strTemp.substr(nCutLen,nTotal-nCutLen).c_str()));
		}
		
	}
	
}

const char* MyTextInput::GetInputText()
{
	return GBK_CHINE(m_pBackCCInput->c_str());
	//return GBK_CHINE(m_pCCInput->getString ());
}

int MyTextInput::GetTextWidth()
{
	if(m_pBackCCInput->length() == 0)
		return 0;
	else
		return m_pCCInput->getContentSize().width;
}

void MyTextInput::SetRect( int nX, int nY, int nW, int nH )
{
	m_rcRect.setRect(nX,nY,nW,nH);
	if(nowRateW && nowRateH){
		m_rcRect.setRect (m_rcRect.origin.x*nowRateW,m_rcRect.origin.y*nowRateH,m_rcRect.size.width*nowRateW,m_rcRect.size.height*nowRateH);
	}
}
