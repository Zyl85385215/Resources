#pragma once

#include "PacketCenter.h"
#include "CtrlObj.h"
#include "UserData.h"
#include "MiniRadio.h"
#include "PetObjMenu.h"
#include "HorseObjMenu.h"
#include "BagMenu.h"
#include "ItemInfoMenu.h"
#include "ControlMenu.h"

#include "BankMenu.h"
#include "EquipStarMenu.h"
#include "StoneComposeMenu.h"
#include "GetNumMenu.h"
#include "UserParamMenu.h"
#include "MallMenu.h"
#include "ShopMenu.h"
#include "ChatMenu.h"
#include "MissonMenu.h"
#include "TeamMenu.h"
#include "SkillMenu.h"
#include "FriendMenu.h"
#include "WTestMenu.h"
#include "MapCopyMenu.h"
#include "FamilyMenu.h"
#include "VipMenu.h"
#include "AutoMenu.h"
#include "ManorMenu.h"
#include "MailMenu.h"
#include "RankMenu.h"
#include "MCAwardMenu.h"
#include "FriendChooseMenu.h"
#include "ActivityMenu.h"
#include "SkillSPMenu.h"
#include "LuckBetMenu.h"
#include "GameAwardMenu.h"
#include "StarSysMenu.h"
#include "NASaleMenu.h"
#include "EquipDuraMenu.h"
#include "MentoringMenu.h"
#include "GreenModeMenu.h"
#include "MakeSkillMenu.h"
#include "AchievementMenu.h"
#include "MarryMenu.h"
#include "ActivateMenu.h"
#include "FaqHelpMenu.h"
#include "ClientLuaManager.h"

class MyMessageBox
{
public:
	MyMessageBox();
	~MyMessageBox();

	void	PopMsgbox(char* szTitle, char* szMsg,int nType, bool bLock);
	void	ClickAnswer(bool bOk);
	void	ResetBox();
	DForm	*	m_pForm;

	DForm	*	m_pLastLockForm;
	void (*Func_CBMsgBox)(void * pParam,int nVal);
	void	*	pParamMsgbox;
private:
	DComponent*	m_pCompTitle;
	DComponent*	m_pCompMsg;
	DComponent*	m_pCompOk;
	DComponent*	m_pCompCancel;
	DComponent*	m_pCompClose;
};

class MyInputBox
{
public:
	MyInputBox();
	~MyInputBox();
	void	PopMsgbox(char* szTitle, char* szMsg,int nType, bool bLock);
	void	ClickAnswer(bool bOk);
	DForm * m_pForm;
	DComponent * m_pInput;
	DForm * m_pLastLockForm;
	void (*Func_CBMsgBox)(void * pParam,int nVal,char * szInput);
	void*			pParamMsgbox;
private:
	DComponent * m_pTitle;

	DComponent * m_pOk;
	DComponent * m_pCancel;
};

class CastMenu
{
public:
	CastMenu();

	void	Update();

	DComponent *	m_pCompCast;
	DWORD			m_dwCastSTick;
	void*			m_pCastParam;
	DWORD			m_dwCastTime;
	char			m_szCastTxt[32];
	DForm *			m_pForm;
	bool			m_bLock;
	void (*fnCastComplete)(void * param);
};

class MainMenu;
extern	MainMenu*	pMainMenu;

class MainMenu
{
public:
	MainMenu();
	~MainMenu();

	void	PopSystemMsg(char* szMsg,bool bBtn = false);
	void	PopTalkMsg(char* szTitle,char* szMsg,int nSkin);
	void	PopAskMsgBox(char* szMsg, void (*cb_MsgBox)(void * pParam,int nVal), void* param,BYTE bType=1);
	void	PopTipMsg(char* szMsg,...);

	void	StartCast(DWORD dwTime,char* szTxt,void (*func)(void * param)=NULL,void* param=NULL, bool bLock	= false);
	void	StopCast();
	bool	IsCasting();
	void	CloseMessageBox();
	void	Update();
	void	UpdateSec();
	void	UpdateMin();

	void	PopInputBox(char * szTitle, char* szMsg,BYTE byInputType = INPUT_TYPE_ALL, void (*cb_MsgBox)(void * pParam,int nVal,char * szInput) = NULL, void* param = NULL );

	void	ShowNetWaiting(int nSubTick	= 0);
	void	HideNetWaiting();

	void	IBtnDefaulEvent(int nEvent);
	static MainMenu*	GetDefault(){return pMainMenu;}
	DForm*			m_pFormNetWaiting;
	DWORD			m_dwTickNetWaiting;

	DWORD			m_dwServerTime;
	bool			m_pUserDataUpdate;

	MyInputBox*		m_pInputBox;
	MyMessageBox*	m_pMessageBox;


	MiniMapMenu*	m_pMiniMap;

	TabMenuCtrl*	m_pBagMainCtrl;
	BagMenu*		m_pBagMenu;
	EquipInfoMenu*	m_pEquipInfoMenu;
	ItemInfoMenu*	m_pItemInfoMenu;
	SlaveInfoMenu*  m_pSlaveInfoMenu;
	FastEquipMenu*  m_pFastEquipMenu;

	PetMainMenu*	m_pPetMainMenu;
	PetSkillMenu*	m_pPetSkillMenu;
	PetStrengthenMenu* m_pPetStreMenu;
	PetExchangeMenu*m_pPetExchMenu;
	PetVariatMenu*	m_pPetVariaMenu;
	TabMenuCtrl*	m_pPetMainCtrl;
	PetChooseMenu*	m_pPetChooseMenu;
	MissionMenu*	m_pMissionMenu;

	ControlMenu*	m_pCtrlMenu;
	NASaleMenu*		m_pNaSaleMenu;
	NASalePriceMenu* m_pNASalePriceMenu;
	HorseMainMenu*	m_pHorseMainMenu;
	HorseFeedMenu*	m_pHorseFeedMenu;
	HorseCultivateMenu* m_pHorseCulMenu;
	HorseEatMenu*	m_pHorseEatMenu;
	HorseChooseMenu*m_pHorseChooseMenu;
	AchievementShowMenu*	m_pAchievementShowMenu;
	AchievementListMenu*	m_pAchievementListMenu;
	AchievementMenu*	m_pAchievementMenu1;
	AchievementMenu*	m_pAchievementMenu2;
	AchievementAwardMenu* m_pAchievementAwardMenu;
	AchievementOpenMenu* m_pAchievementOpenMenu;
	TabMenuCtrl*		m_pAchievementTabCtrl;

	EDuraMenu*		m_pEDuraMenu;
	VipMenu*		m_pVipMenu;
	WTestMenu*		m_pWTestMenu;
	WTestRankMenu*	m_pWTestRankMenu;
	TurntableMenu*	m_pTurntableMenu;
	ChatMenu*		m_pChatMenu;
	SimpleChatMenu*	m_pChatMenu2;
	SystemMessageMenu* m_pSysMsgMenu;
	WarMessageMenu*	m_pWarMsgMenu;
	TabMenuCtrl*	m_pHorseMainCtrl;
	BankMenu*		m_pBankMenu;
	LuckBetMenu*	m_pLuckBetMenu;

	SkillMenu*		m_pSkillMenu;
	SkillSPMenu*	m_pSPSkillMenu;
	TabMenuCtrl*	m_pSkillCtrl;

	MentoringMenu*	m_pMentoringMenu;
	MRequestMenu*	m_pMRequestMenu;

	MarryListMenu*	m_pMarryListMenu;
	MarryInfoMenu*	m_pMarryInfoMenu;
	MarrySkillMenu*	m_pMarrySkillMenu;
	MarryQuestMenu*	m_pMarryQuestMenu;
	MarryAskMenu*	m_pMarryAskMenu;
	SendRingMenu*	m_pSendRingMenu;
	MarryNpcMenu*	m_pMarryNpcMenu;
	TabMenuCtrl*	m_pMarryCtrl;

	MapCopyMenu*	m_pMapCopyMenu;
	MapCopyDropMenu* m_pMapCopyDropMenu;
	StarSysMenu*	m_pStarSysMenu;

	NoticeMenu*     m_pNoticeMenu;
	AppraiseMenu*   m_pAppraiseMenu;
	ACBossMenu*     m_pACBossMenu;
	ActivityMenu*	m_pActivityMenu;
	ACDegreeMenu*	m_pACDegreeMenu;
	TabMenuCtrl*	m_pActivityCtrl;

	ActivityJoinMenu* m_pActivityJoinMenu;

	MCAwardMenu*	m_pMCAwardMenu;
	GreenModeMenu*	m_pGreenModeMenu;
	GreenAwardMenu*	m_pGreenAwardMenu;

	DForm*			m_pMainFormLA;
	DForm*			m_pMainFormTA;
	DForm*			m_pMainFormLB;
	DForm*			m_pMainFormTB;
	DForm*			m_pMainFormHide;

	DForm*			m_pFormTalk;

	DForm*			m_pChangeNameF;

	TabMenuCtrl*	m_pTabEquipCtrl;
	EquipStarMenu*	m_pEquipStarMenu;
	EquipBoreMenu*	m_pEquipBoreMenu;
	EquipInlayMenu*	m_pEquipInlayMenu;
	EquipEatMenu*	m_pEquipEatMenu;
	EquipTransferMenu*	m_pEquipTransferMenu;

	StoneComposeMenu*	m_pStoneComposeMenu;

	GetNumMenu*		m_pGetNumMenu;
	UserParamMenu*	m_pUserParamMenu;
	UserTitleMenu*	m_pUserTitleMenu;
	RankMenuObj*	m_pRankMenu[RANK_MAX];
	TabMenuCtrl*	m_pUserTabCtrl;

	MallMenu*		m_pMallMenu;
	ShopMenu*		m_pShopMenu;
	FastSellMenu*	m_pFastSellMenu;

	TeamMenu*		m_pTeamMenu;
	CastMenu*		m_pCastMenu;

	TabMenuCtrl*	m_pTabSocialityCtrl;
	FriendMenu*		m_pFriendMenu;
	FamilyListMenu*	m_pFamilyListMenu;
	AddFriendMenu*	m_pAddFriendMenu;
	FRequestMenu*	m_pFRequestMenu;
	PetGetMenu*     m_pPetGetMenu;
	AutoMenu*		m_pAutoMenu;
	ShowPetGetMenu* m_pShowPetGetMenu;
	SettingMenu*	m_pSettingMenu;
	ChangePNameMenu* m_pChangePNameMenu;

	FamilyInfoMenu*	m_pFamilyInfoMenu;
	FamilyMemberMenu*	m_pFamilyMemberMenu;
	FamilySkillMenu*	m_pFamilySkillMenu;
	FamilyBuildMenu*	m_pFamilyBuildMenu;
	FamilyQuestMenu*	m_pFamilyQuestMenu;
	TabMenuCtrl*	m_pTabFamilyCtrl;

	FamilyCreateMenu*	m_pFamilyCreateMenu;

	FamilyJoinMenu*		m_pFamilyJoinMenu;

	OnlineAwardMenu*	m_pOnlineAwardMenu;
	MounthAwardMenu*	m_pMounthAwardMenu;
	LvupAwardMenu*		m_pLvupAwardMenu;
	LoginAwardMenu*     m_pLoginAwardMenu;
	RechargeAwardMenu*	m_pRechargeAwardMenu;
	InviteAwardMenu*	m_pInviteAwardMenu;
	TabMenuCtrl*		m_pTabAwardCtrl;	

	ManorMenu*		m_pManorMenu;
	ManorAwardMenu*		m_pManorAwardMenu;
	ChooseAutoDrug* m_pChooseAutoDrug;
	MailMenu*		m_pMailMenu;
	FriendChooseMenu*	m_pFriendChooseMenu;
	RechargeMenu*    m_pRechargeMenu;
	SelObjHandleMenu* m_pSelObjHandleMenu;
	SelObjInfoMenu*   m_pSelObjInfoMenu;
	TeamDropMenu*     m_pTeamDropMenu;
	DropShowMenu*     m_pDropShowMenu;

	FortMenu*		m_pFortMenu;
	WarRankMenu*	m_pWarRankMenu;
	WarRankMenuEx*	m_pWarRankMenuEx;
	WarRankRewardMenu*	m_pWarRankRewardMenu;
	NASaleFilterMenu* m_pNASaleFilterMenu;
	FamilyNoticeMenu*	m_pFamilyNoticeMenu;

	FishMenu*     m_pFishMenu;

	ManorRecordMenu* m_pManorRecordMenu;

	DForm*			m_pScore;
	DForm*			m_pEffectForm;
	xnList*			m_pListMainEff;
	void			PlayEffect(int nEffID);
	void			_checkEffect();

	bool            m_bBackLogin;
	bool			m_ifQMenuShow;
	WORD			m_wFstHomeID;

	MakeSkillMenu*	m_pMakeSkillMenu;
	ActivateMenu*	m_pActivateMenu;
	FaqHelpMenu*	m_pFaqHelpMenu;
	FaqHelpMenuEx*	m_pFaqHelpMenuEx;
	RankDamageAwardMenu* m_pRankDamageAwardMenu;
};
