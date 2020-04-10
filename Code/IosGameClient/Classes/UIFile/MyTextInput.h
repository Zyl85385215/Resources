#pragma once


#include "cocos2d.h"

//���ڼ��������
class KeyboardNotificationLayer : public cocos2d::CCLayer, public cocos2d::CCIMEDelegate
{
public:
	KeyboardNotificationLayer();

	virtual bool onClickTrackNode(bool bClicked) = 0;

	virtual void registerWithTouchDispatcher();

	//������������ʱ����Ҫ�ѿؼ�����
	virtual void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo& info);
	virtual void keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo& info);

	// CCLayer
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	virtual void onExit();
	virtual void onEnter();

	cocos2d::CCRect		m_rcRect;
	int			m_nBottomY;
	bool		m_bActive;
protected:
	cocos2d::CCNode *	m_pTrackNode;
	cocos2d::CCPoint	m_beginPos;
};

//CCTextFieldDelegate	���ڴ����ı������¼�,���괦����,���Ƴ��ȵ�  �麯��  return true ��Ĭ�ϵ�����ʱ�䲻����
class MyTextInput	: public cocos2d::CCTextFieldDelegate, public KeyboardNotificationLayer
{
public:
	MyTextInput(int x,int y, int w, int h,char* szTmp="",int nFontSize = 20);
	~MyTextInput();
	virtual bool onClickTrackNode(bool bClicked);

	//trueΪ��ִ��Ĭ�ϵ�
	virtual bool onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF * sender);
    virtual bool onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF * sender);
    virtual bool onTextFieldInsertText(cocos2d::CCTextFieldTTF * sender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF * sender, const char * delText, int nLen);
    virtual bool onDraw(cocos2d::CCTextFieldTTF * sender);
	void			SetInputText(const char* szTmp);
	const char*		GetInputText();
	int				GetTextWidth();
	void			SetRect(int nX, int nY, int nW, int nH);
	char m_bInputType;

protected:
	cocos2d::CCTextFieldTTF	*		m_pCCInput;		//setAnchorPoint ���������,����
	std::string	*		m_pBackCCInput;
	int			m_nCurEditPos;
	cocos2d::CCTexture2D*					m_pText2D;
	cocos2d::CCSprite*				m_pSprFlash;
	int			m_nDifX;
	bool		bNewline;
};