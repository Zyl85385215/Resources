#pragma once

#include "cocos2d.h"
#include "PacketCenter.h"
#include "xnlist.h"
#include "Interface.h"

#include "LoginMenu.h"
#include "SelLineMenu.h"
#include "CreateRoleMenu.h"
#include "InterfaceLayer.h"

class LoginMenu;
class ServerLine;
class LoginScene	: public	cocos2d::CCScene , public PacketListener
{
public:
	LoginScene();
	~LoginScene();

	CREATE_FUNC(LoginScene);
	virtual	void	onEnter ();
	virtual void	onExit ();
	virtual void update (float dt);

	void		UpdateMovePlay();

	void		InitBackSprite();
	void		UpdateBackView();
	DWORD				m_dwTickDealError;

	virtual	bool PacketProcess(ZPacket* pPacket);

	SelLineMenu*	m_pSelLineMenu;
	LoginMenu*		m_pLoginMenu;
	CreateRoleMenu*	m_pCreateRoleMenu;
	ChooseRoleMenu*	m_pChooseRoleMenu;
	ServerLine*		m_pLineAcc;

	cocos2d::CCSprite*	m_pSpriteMove[4];
	CCLabelTTF*	m_pLableTxt[4];
	char			m_szMoveTxt[4][256];

	cocos2d::CCSprite*	m_pSpriteBack[2];
	cocos2d::CCSprite*	m_pSpriteCloud[2];
	CCEffectSprite*		m_pEffLoginBack[5];


	DWORD			m_dwPlayMoveTick;
	BYTE			m_byMoveStep;

	DWORD			m_dwTickInit;
	DWORD			m_dwErrorCode;
};

extern	char*	g_szLoginAcc;
extern	char*	g_szLoginPswd;
extern	char*	g_szBackKey;
extern	int		g_nServerID;
extern	char*	g_szZoneName;
