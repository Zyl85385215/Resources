#include "MainMenu.h"
#include "SoundCenter.h"
#include "ios_user_packet.h"
#include "ios_home_packet.h"
#include "SlaveFormCtrl.h"
#include "GuideLayer.h"
#include "InterfaceLayer.h"
#include "MapMouse.h"
#include "RankMenu.h"
#include "UserData.h"
#include "EffectSprite.h"
#include "TextConfig.h"

MainMenu*	pMainMenu	= NULL;

//CLuaStateManager * pLuaStateManager = 0;

void cb_ClickCheckChgName(void* pParam, int nVal)
{
	char* szName = (char*)pParam;
	if (nVal)
	{
		BuildPacketEx(pPacket,USER,C_CHG_USER_NAME,buf,1024);
		SET_DATA(pData,USER,C_CHG_USER_NAME,pPacket);
		strncpy(pData->szName,szName,20);
		pChar->SendPacket(pPacket);
	}

	free(szName);
};

static void ClickChangeCharName(InterfaceEvent* pEvent, void* param)
{
	if (pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	char szTmpName[64];
	strcpy(szTmpName,pMainMenu->m_pChangeNameF->GetComponent("name")->GetEdit());
	if(strlen(szTmpName) < 4 ||strlen(szTmpName) > 12)
	{
		pMainMenu->PopTipMsg("角色名字长度需要在4-12个字符之内，请重新输入");
		return;
	}
	if (atoi(szTmpName)&&szTmpName[0]!='0')
	{
		pMainMenu->PopTipMsg("角色名第一位不能为数字，请重新输入");
		return;
	}
	if(RandNameConfig::GetInstance().CheckForbid(szTmpName))
	{
		pMainMenu->PopTipMsg("名字中含有屏蔽词，请重新输入");
		return;
	}
	char* szChgName = strdup(szTmpName);
	pMainMenu->PopAskMsgBox("改名后将重新登入游戏，是否立即改名？",cb_ClickCheckChgName,(void*)szChgName);
}

static void OpenBagMenuEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pBagMainCtrl->Open(0,0);

}	
static void OpenFriendEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pTabSocialityCtrl->Open(1,0);

}

static void OpenRankEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pUserTabCtrl->Open(1,2);

}

static void OpenWtestEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	if(pChar->CheckInFight())
	{
		pMainMenu->PopTipMsg("战斗状态下不可打开该界面!");
		return;
	}
	if(g_pMapCenter->m_nBaseMapIndex == 66)
	{
		pMainMenu->PopTipMsg("该活动地图内，无法打开该界面!");
		return;
	}
	pMainMenu->m_pActivityCtrl->Open(2,0);

}	

static void OpenTempEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	DForm* pForm = pMainInterface->GetDForm((char*)param);
	if (pForm)
	{
		pForm->Open();
	}

}	
static void OpenTeamMenuEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	pMainMenu->m_pMainFormHide->Close();
	pMainMenu->m_pMainFormLB->Close();
	pMainMenu->m_pTeamMenu->Open(3);
}	

static void OpenMainFormLBEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	pMainMenu->m_pMainFormHide->Close();
	pMainMenu->m_pTeamMenu->Close();
	pMainMenu->m_pMainFormLB->Open(3);
	pMainMenu->m_ifQMenuShow = true;
}

static void HideTeamMenuEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	pMainMenu->m_pMainFormLB->Close();
	pMainMenu->m_pTeamMenu->Close();
	pMainMenu->m_pMainFormHide->Open();
	pMainMenu->m_ifQMenuShow = false;
}

static void OpenMallMenuEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pMallMenu->Open();

}
static void OpenShopMenuEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pShopMenu->Open(0);
	//	pMainMenu->m_pShopMenu->m_pShopEx->m_nShopType = 0;

}	
static void OpenUserParamMenuEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pUserTabCtrl->Open(2,0);

}	
static void OpenBankMenuEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pBankMenu->Open();

}	


void SwitchMainBtn(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(pMainMenu->m_pMainFormLA->bOpened)
	{
		pMainMenu->m_pMainFormLA->Close();
		pMainMenu->m_pMainFormTA->Close();
		if(pMainMenu->m_ifQMenuShow)
			pMainMenu->m_pMainFormLB->Open(3);
		else
			pMainMenu->m_pMainFormHide->Open();
		pMainMenu->m_pMainFormTB->Open(5);
		pCtrlLayer->setVisible(true);

	}
	else
	{
		pMainMenu->m_pMainFormLB->Close();
		pMainMenu->m_pMainFormHide->Close();
		pMainMenu->m_pMainFormTB->Close();
		pMainMenu->m_pTeamMenu->Close();
		pMainMenu->m_pMainFormLA->Open(3);
		pMainMenu->m_pMainFormTA->Open(5);
		pCtrlLayer->setVisible( false);
	}
}

void ClickMainButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	int nType = (int)param;
	switch(nType)
	{
	case 1:
		pMainMenu->m_pPetMainCtrl->Open(2,0);
		break;
	case 2:
		pMainMenu->m_pHorseMainCtrl->Open(2,0);
		break;
	case 3:
		pMainMenu->m_pMissionMenu->Open();
		break;
	case 5:
		//pMainMenu->m_pSkillMenu->OpenForm(pChar->m_byJob-1);
		pMainMenu->m_pSkillCtrl->Open(2,0);
		break;
	case 4:
		pMainMenu->m_pMallMenu->Open();
		break;
	case 6:
		pMainMenu->m_pSettingMenu->m_pForm->Open(2);
		break;
	case 7:
		//pMainMenu->m_pAutoMenu->m_pFrom->Open(2);
		break;
	case 8:
		pMainMenu->m_pMapCopyMenu->OpenForm();
		//pMainMenu->m_pMCAwardMenu->OpenForm(101);
		break;
	case 9:
		if (pChar->m_byLvl < 10)
		{
			pMainMenu->PopTipMsg("等级达到10级开启要塞玩法");
		}else
			pMainMenu->m_pFortMenu->Open();
		break;
	case 10:
		{
			pMainMenu->m_pTabAwardCtrl->Open(2,0);
		}
		break;
	case 11:
		{
			int nOpenPos = 1;
			if(pChar->m_pMounthAwardCenter->m_ifShine)
				nOpenPos = 1;
			if(pChar->m_pLvupAwardCenter->m_ifShine)
				nOpenPos = 2;
			if(pChar->m_pLoginAwardCenter->m_ifShine)
				nOpenPos = 5;
			pMainMenu->m_pTabAwardCtrl->Open(2,nOpenPos);
		}
		break;
	case 12:
		pMainMenu->m_pStarSysMenu->Open();
		break;
	case 13:
		pMainMenu->m_pRechargeAwardMenu->Open(2);
		break;
	case 14:
		{
		    bool bOpen = false;
		    for(int i = 0; i < 10; i++)
		    {
		    	ItemObj* pItem = pChar->m_pItemCenter->GetEquipBySlot(i);
	            if (pItem)
	               {
			           pMainMenu->m_pEquipStarMenu->SetEquipItem(pItem);
		               pMainMenu->m_pTabEquipCtrl->Open(0,0);
			     	   bOpen = true;
			    	   break;
		           }
		    }
		    if(!bOpen)
			    pMainMenu->PopTipMsg("身上未穿戴装备");
		}
		break;
	case 100:
		pChar->SendCmd(JGSLAVE,C_TEST);
		break;
	}
}
MainMenu::MainMenu()
{
	m_dwServerTime = 0;
	m_ifQMenuShow = true;
	m_pScore = pMainInterface->GetDForm("zhanlizhi");
	m_pScore->GetComponent("up")->SetSprColor(nItemLimitColor[1][0],nItemLimitColor[1][1],nItemLimitColor[1][2]);
	g_pSlaveMenuCtrl= new SlaveMenuCtrl();
	g_pRankMenuCenter= new RankMenuCenter();
	m_pCastMenu		= new CastMenu();
	m_pInputBox	= NULL;
	m_pMessageBox	= NULL;
	//m_pInputBox		= new MyInputBox();
	m_pMessageBox	= new MyMessageBox();
	m_pMiniMap		= new MiniMapMenu();

	m_pBagMenu		= new BagMenu();
	m_pMailMenu = new MailMenu;
	TabMenuObj*	ppTabBag[3];
	ppTabBag[0] = m_pBagMenu;
	ppTabBag[1] = NULL;
	ppTabBag[2] = m_pMailMenu;
	m_pBagMainCtrl	= new TabMenuCtrl(ppTabBag,3);

	m_pEquipInfoMenu = new EquipInfoMenu();
	m_pItemInfoMenu = new ItemInfoMenu();
	m_pSlaveInfoMenu = new SlaveInfoMenu();
	m_pBankMenu		= new BankMenu();
	m_pLuckBetMenu	= new LuckBetMenu();
	m_pFastEquipMenu = new FastEquipMenu();


	m_pSkillMenu	= new SkillMenu();
	m_pSPSkillMenu	= new SkillSPMenu();
	m_pMakeSkillMenu = new MakeSkillMenu();

	TabMenuObj*	ppTabSkill[3];
	ppTabSkill[0] = m_pSkillMenu;
	ppTabSkill[1] = m_pMakeSkillMenu;
	ppTabSkill[2] = m_pSPSkillMenu;
	m_pSkillCtrl		= new TabMenuCtrl(ppTabSkill,3);
	m_pSkillMenu->InitTabEvent(3);


	m_pPetMainMenu	= new PetMainMenu("chongwu");
	m_pPetSkillMenu = new PetSkillMenu("cwskill");
	m_pPetStreMenu	= new PetStrengthenMenu("cwqianghua");
	m_pPetExchMenu	= new PetExchangeMenu("cwjinhua");
	m_pPetVariaMenu = new PetVariatMenu("cwbianyi");
	g_pHandSign		= new HandSignCmd();
	m_pVipMenu		= new VipMenu();
	m_pRechargeMenu = new RechargeMenu();
	TabMenuObj*	ppTabPet[5];
	ppTabPet[0] = m_pPetMainMenu;
	ppTabPet[1] = m_pPetSkillMenu;
	ppTabPet[2] = m_pPetStreMenu;
	ppTabPet[3] = m_pPetExchMenu;
	ppTabPet[4] = m_pPetVariaMenu;
	m_pPetMainCtrl	= new TabMenuCtrl(ppTabPet,5);
	m_pPetChooseMenu = new PetChooseMenu();
	m_pPetGetMenu = new PetGetMenu();
	m_pAutoMenu		= new AutoMenu();
	m_pChooseAutoDrug = new ChooseAutoDrug();
	m_pSettingMenu	= new SettingMenu();
	m_pShowPetGetMenu = new ShowPetGetMenu();
	m_pChangePNameMenu = new ChangePNameMenu();
	m_pMissionMenu	= new MissionMenu();
	m_pNaSaleMenu	= new NASaleMenu();
	m_pNASalePriceMenu = new NASalePriceMenu();
	m_pHorseMainMenu	= new HorseMainMenu("zuoqi");
	m_pHorseFeedMenu	= new HorseFeedMenu("zqweiyang");
	m_pHorseCulMenu		= new HorseCultivateMenu("zqpeiyang");
	m_pHorseEatMenu		= new HorseEatMenu("zqtuanshi");
	TabMenuObj*	ppTabHorse[4];
	ppTabHorse[0] = m_pHorseMainMenu;
	ppTabHorse[1] = m_pHorseFeedMenu;
	ppTabHorse[2] = m_pHorseCulMenu;
	ppTabHorse[3] = m_pHorseEatMenu;
	m_pHorseMainCtrl	= new TabMenuCtrl(ppTabHorse,4);
	m_pHorseChooseMenu	= new HorseChooseMenu();

	m_pFamilyNoticeMenu = new FamilyNoticeMenu();
	m_pWTestMenu = new WTestMenu();
	m_pWTestRankMenu = new WTestRankMenu();
	m_pTurntableMenu = new TurntableMenu();
	m_pChatMenu = new ChatMenu();
	m_pSysMsgMenu = new SystemMessageMenu();
	m_pWarMsgMenu = new WarMessageMenu();
	m_pChatMenu2 = new SimpleChatMenu();
	m_pEquipStarMenu = new EquipStarMenu;
	m_pEquipBoreMenu = new EquipBoreMenu;
	m_pEquipInlayMenu = new EquipInlayMenu;
	m_pEquipEatMenu = new EquipEatMenu;
	m_pEquipTransferMenu = new EquipTransferMenu;
	m_pMapCopyMenu		= new MapCopyMenu();
	m_pMapCopyDropMenu  = new MapCopyDropMenu();
	m_pStarSysMenu		= new StarSysMenu();
	m_pMCAwardMenu		= new MCAwardMenu();
	m_pGreenModeMenu	= new GreenModeMenu();
	m_pGreenAwardMenu	= new GreenAwardMenu();
	m_pActivityMenu		= new ActivityMenu();
	m_pACDegreeMenu		= new ACDegreeMenu();
	m_pACBossMenu       = new ACBossMenu();
	m_pAppraiseMenu     = new AppraiseMenu();
	m_pNoticeMenu       = new NoticeMenu();
	TabMenuObj*	ppTabActivity[5];
	ppTabActivity[0] = m_pActivityMenu;
	ppTabActivity[1] = m_pACDegreeMenu;
	ppTabActivity[2] = m_pACBossMenu;
	ppTabActivity[3] = m_pAppraiseMenu;
	ppTabActivity[4] = m_pNoticeMenu;
	m_pActivityCtrl		= new TabMenuCtrl(ppTabActivity,5);

	m_pActivityJoinMenu = new ActivityJoinMenu();

	TabMenuObj*	ppTabEquip[5];
	ppTabEquip[0] = m_pEquipStarMenu;
	ppTabEquip[1] = m_pEquipBoreMenu;
	ppTabEquip[2] = m_pEquipInlayMenu;
	ppTabEquip[3] = m_pEquipEatMenu;
	ppTabEquip[4] = m_pEquipTransferMenu;

	m_pTabEquipCtrl = new TabMenuCtrl(ppTabEquip,5);

	m_pAchievementListMenu = new AchievementListMenu();
	m_pAchievementShowMenu = new AchievementShowMenu();
	m_pAchievementMenu1 = new AchievementMenu(1);
	m_pAchievementMenu2 = new AchievementMenu(2);
	m_pAchievementAwardMenu = new AchievementAwardMenu();
	TabMenuObj* ppTabAchieve[3];
	ppTabAchieve[0] = m_pAchievementMenu1;
	ppTabAchieve[1] = m_pAchievementMenu2;
	ppTabAchieve[2] = m_pAchievementAwardMenu;
	m_pAchievementTabCtrl = new TabMenuCtrl(ppTabAchieve,3);

	m_pStoneComposeMenu = new StoneComposeMenu;

	m_pGetNumMenu = new GetNumMenu;

	m_pUserParamMenu = new UserParamMenu;
	m_pUserTitleMenu = new UserTitleMenu();
	m_pFortMenu = new FortMenu();
	m_pAchievementOpenMenu = new AchievementOpenMenu();
	char szName[16];
	for(int i = 0; i < 7; i ++)
	{
		sprintf(szName,"bang%d",i+1);
		m_pRankMenu[i] = new RankMenuObj(szName,i);
	}
	TabMenuObj*	ppTabUser[11];
	ppTabUser[0] = m_pUserParamMenu;
	ppTabUser[1] = m_pUserTitleMenu;
	ppTabUser[2] = m_pRankMenu[0];
	ppTabUser[3] = m_pAchievementOpenMenu;
	ppTabUser[4] = m_pRankMenu[0];
	ppTabUser[5] = m_pRankMenu[1];
	ppTabUser[6] = m_pRankMenu[2];
	ppTabUser[7] = m_pRankMenu[3];
	ppTabUser[8] = m_pRankMenu[4];
	ppTabUser[9] = m_pRankMenu[5];
	ppTabUser[10] = m_pRankMenu[6];
	m_pUserTabCtrl = new TabMenuCtrl(ppTabUser,11);

	m_pMallMenu = new MallMenu;
	m_pShopMenu = new ShopMenu;

	m_pFastSellMenu = new FastSellMenu;
	m_pTeamMenu = new TeamMenu;

	m_pFriendMenu = new FriendMenu;
	m_pFamilyListMenu = new FamilyListMenu;
	m_pAddFriendMenu= new AddFriendMenu;
	m_pFRequestMenu = new FRequestMenu();
	m_pMentoringMenu = new MentoringMenu;
	m_pMRequestMenu = new MRequestMenu;
	m_pMarryListMenu = new MarryListMenu();
	TabMenuObj*	ppTabSociality[4];
	ppTabSociality[0] = m_pFriendMenu;
	ppTabSociality[1] = m_pMarryListMenu;
	ppTabSociality[2] = m_pMentoringMenu;
	ppTabSociality[3] = m_pFamilyListMenu;

	m_pTabSocialityCtrl = new TabMenuCtrl(ppTabSociality,4);
	m_pMentoringMenu->InitTabEvent(4);

	m_pMarryInfoMenu	= new MarryInfoMenu();
	m_pMarrySkillMenu	= new MarrySkillMenu();
	m_pMarryQuestMenu	= new MarryQuestMenu();
	m_pMarryAskMenu		= new MarryAskMenu();
	m_pSendRingMenu		= new SendRingMenu();
	m_pMarryNpcMenu		= new MarryNpcMenu();
	TabMenuObj*	ppTabMarry[3];
	ppTabMarry[0]	= m_pMarryInfoMenu;
	ppTabMarry[1]	= m_pMarrySkillMenu;
	ppTabMarry[2]	= m_pMarryQuestMenu;
	m_pMarryCtrl	= new TabMenuCtrl(ppTabMarry,3);

	m_pFamilyInfoMenu = new FamilyInfoMenu;
	m_pFamilyMemberMenu = new FamilyMemberMenu;
	m_pFamilySkillMenu = new FamilySkillMenu;
	m_pFamilyBuildMenu = new FamilyBuildMenu;
	m_pFamilyQuestMenu = new FamilyQuestMenu;
	TabMenuObj*	ppTabFamily[5];
	ppTabFamily[0] = m_pFamilyInfoMenu;
	ppTabFamily[1] = m_pFamilyMemberMenu;
	ppTabFamily[2] = m_pFamilySkillMenu;
	ppTabFamily[3] = m_pFamilyBuildMenu;
	ppTabFamily[4] = m_pFamilyQuestMenu;

	m_pTabFamilyCtrl= new TabMenuCtrl(ppTabFamily,5);

	m_pFamilyCreateMenu = new FamilyCreateMenu;
	m_pFamilyJoinMenu = new FamilyJoinMenu;

	m_pManorMenu = new ManorMenu;
	m_pManorAwardMenu = new ManorAwardMenu();

	m_pOnlineAwardMenu = new OnlineAwardMenu();
	m_pMounthAwardMenu = new MounthAwardMenu();
	m_pLvupAwardMenu = new LvupAwardMenu();
	m_pLoginAwardMenu = new LoginAwardMenu;
	m_pRechargeAwardMenu = new RechargeAwardMenu();
	m_pInviteAwardMenu	= new InviteAwardMenu();
	TabMenuObj* ppTabAward[6];
	ppTabAward[0] = m_pOnlineAwardMenu;
	ppTabAward[1] = m_pMounthAwardMenu;
	ppTabAward[2] = m_pLvupAwardMenu;
	ppTabAward[3] = NULL;
	ppTabAward[4] = m_pInviteAwardMenu;
	ppTabAward[5] = m_pLoginAwardMenu;
	m_pTabAwardCtrl = new TabMenuCtrl(ppTabAward,6);

	m_pFriendChooseMenu = new FriendChooseMenu;
	m_pSelObjHandleMenu = new SelObjHandleMenu();
	m_pSelObjInfoMenu = new SelObjInfoMenu();
	m_pTeamDropMenu = new TeamDropMenu();
	m_pDropShowMenu = new DropShowMenu();

	m_pWarRankMenu = new WarRankMenu();
	m_pWarRankMenuEx = new WarRankMenuEx();
	m_pWarRankRewardMenu = new WarRankRewardMenu();
	m_pEDuraMenu = new EDuraMenu();
	m_pNASaleFilterMenu = new NASaleFilterMenu();

	m_pFishMenu = new FishMenu();

	m_pActivateMenu = new ActivateMenu();
	m_pFaqHelpMenu		= new FaqHelpMenu();
	m_pFaqHelpMenuEx	= new FaqHelpMenuEx();
	m_pRankDamageAwardMenu = new RankDamageAwardMenu();

	m_pManorRecordMenu = new ManorRecordMenu();

	pMainInterface->GetDForm("main")->GetComponent("chongwu")->SetEvent(ClickMainButton,(void*)1);
	pMainInterface->GetDForm("main")->GetComponent("fq")->SetEvent(ClickMainButton,(void*)2);
	pMainInterface->GetDForm("main")->GetComponent("skill")->SetEvent(ClickMainButton,(void*)5);
	pMainInterface->GetDForm("team")->GetComponent("tx1")->SetEvent(SwitchMainBtn,this);


	m_pMainFormLA	= pMainInterface->GetDForm ("main");
	m_pMainFormLA->GetComponent("chongwu")->SetEvent(ClickMainButton,(void*)1);
	m_pMainFormLA->GetComponent("sys")->SetEvent(ClickMainButton,(void*)6);
	m_pMainFormLA->GetComponent("fq")->SetEvent(ClickMainButton,(void*)2);
	m_pMainFormLA->GetComponent("qianghua")->SetEvent(ClickMainButton,(void*)14);

	m_pMainFormTA	= pMainInterface->GetDForm("main6");
	m_pMainFormTA->GetComponent("renwu")->SetEvent(ClickMainButton,(void*)3);
	m_pMainFormTA->GetComponent("bag")->SetEvent(OpenBagMenuEvent);
	m_pMainFormTA->GetComponent("jiaoji")->SetEvent(OpenFriendEvent);
	m_pMainFormTA->GetComponent("static")->SetEvent(OpenUserParamMenuEvent);
	m_pMainFormTA->GetComponent("ph")->SetEvent(OpenRankEvent);

	m_pMainFormLB	= pMainInterface->GetDForm ("main7");
	m_pMainFormLB->GetComponent("page2")->SetEvent(OpenTeamMenuEvent);
	m_pMainFormLB->GetComponent("page3")->SetEvent(HideTeamMenuEvent);
	m_pMainFormTB	= pMainInterface->GetDForm ("main5");
	m_pFormTalk		= pMainInterface->GetDForm ("npcchat");
	m_pMainFormHide = pMainInterface->GetDForm ("renwuxs");
	m_pMainFormHide->GetComponent("xs")->SetEvent(OpenMainFormLBEvent);

	m_pMainFormTB->GetComponent("vip")->SetEvent(OpenTempEvent,(void*)"VIP");
	m_pMainFormTB->GetComponent("fb")->SetEvent(ClickMainButton,(void*)8);
	m_pMainFormTB->GetComponent("zy")->SetEvent(ClickMainButton,(void*)9);
	m_pMainFormTB->GetComponent("xc")->SetEvent(ClickMainButton,(void*)12);
	//m_pMainFormTB->GetComponent("hd")->SetEvent(OpenTempEvent,(void*)"dati");
	m_pMainFormTB->GetComponent("hd")->SetEvent(OpenWtestEvent,NULL);
	//m_pMainFormTB->GetComponent("gj")->SetEvent(ClickMainButton,(void*)7);
	m_pMainFormTB->GetComponent("zx")->SetEvent(ClickMainButton,(void*)10);
	m_pMainFormTB->GetComponent("jl")->SetEvent(ClickMainButton,(void*)11);
	m_pMainFormTB->GetComponent("sc")->SetEvent(ClickMainButton,(void*)13);
	m_pMainFormTB->GetComponent("shang")->SetEvent(ClickMainButton,(void*)4);
	m_pCtrlMenu			= new ControlMenu();

	m_pFormNetWaiting	= pMainInterface->GetDForm("quanquan");
	m_pFormNetWaiting->bZorde	= FORM_ZORDER_NET_WAIT;

	//g_pSoundCenter->PlayerBackMp3 ("music/Normal2.mp3",true);
	//m_pMainForm->GetComponent("sys")->SetEvent(OpenBankMenuEvent);
	m_pUserDataUpdate = true;

	m_pEffectForm = pMainInterface->GetDForm("eff");
	m_pEffectForm->bZorde = 998;
	m_pListMainEff	= xnList::Create();
	m_bBackLogin = false;
	m_wFstHomeID	= 0;

	m_pChangeNameF = pMainInterface->GetDForm("rwgaiming");
	m_pChangeNameF->GetComponent("ok")->SetEvent(ClickChangeCharName);
}

MainMenu::~MainMenu()
{
	delete m_pMiniMap;
	delete m_pCastMenu;
	delete m_pInputBox;
	delete m_pMessageBox;
	delete g_pSlaveMenuCtrl;
	delete g_pRankMenuCenter;
	delete m_pMentoringMenu;
	delete m_pMRequestMenu;

	delete m_pMarryListMenu;
	delete m_pMarryInfoMenu;
	delete m_pMarrySkillMenu;
	delete m_pMarryQuestMenu;
	delete m_pMarryAskMenu;
	delete m_pSendRingMenu;
	delete m_pMarryNpcMenu;
	delete m_pMarryCtrl;

	delete m_pWarMsgMenu;
	delete m_pPetMainMenu;
	delete m_pPetSkillMenu;
	delete m_pPetStreMenu;
	delete m_pPetExchMenu;
	delete m_pPetVariaMenu;
	delete m_pPetChooseMenu;
	delete m_pPetMainCtrl;
	delete m_pMissionMenu;
	delete m_pNaSaleMenu;
	delete m_pNASalePriceMenu;
	delete m_pHorseMainMenu;
	delete m_pHorseFeedMenu;
	delete m_pHorseCulMenu;
	delete m_pHorseEatMenu;
	delete m_pHorseChooseMenu;
	delete m_pHorseMainCtrl;
	delete m_pActivityMenu;
	delete m_pActivityJoinMenu;
	delete m_pFamilyNoticeMenu;
	delete m_pACDegreeMenu;
	delete m_pActivityCtrl;
	delete m_pSkillMenu;
	delete m_pSPSkillMenu;
	delete m_pSkillCtrl;
	delete m_pEDuraMenu;
	delete m_pAchievementListMenu;
	delete m_pAchievementMenu1;
	delete m_pAchievementMenu2;
	delete m_pAchievementAwardMenu;
	delete m_pAchievementTabCtrl;
	delete m_pAchievementShowMenu;

	delete m_pVipMenu;
	delete m_pWTestMenu;
	delete m_pWTestRankMenu;
	delete m_pTurntableMenu;
	delete m_pBagMenu;
	delete m_pEquipInfoMenu;
	delete m_pItemInfoMenu;
	delete m_pSlaveInfoMenu;
	delete m_pBankMenu;
	delete m_pLuckBetMenu;

	delete m_pOnlineAwardMenu;
	delete m_pMounthAwardMenu;
	delete m_pLvupAwardMenu;
	delete m_pLoginAwardMenu;
	delete m_pRechargeAwardMenu;
	delete m_pTabAwardCtrl;
	delete m_pFastEquipMenu;

	delete m_pEquipStarMenu;
	delete m_pEquipBoreMenu;
	delete m_pEquipInlayMenu;
	delete m_pEquipEatMenu;
	delete m_pEquipTransferMenu;
	delete m_pStoneComposeMenu;
	delete m_pWarRankMenu;
	delete m_pWarRankMenuEx;
	delete m_pWarRankRewardMenu;
	delete m_pGetNumMenu;
	delete m_pUserParamMenu;
	delete m_pUserTitleMenu;
	delete m_pUserTabCtrl;
	delete m_pMallMenu;
	delete m_pShopMenu;
	delete m_pFortMenu;
	delete m_pFastSellMenu;
	delete m_pTeamMenu;
	delete m_pFriendMenu;
	delete m_pFamilyListMenu;
	delete m_pAddFriendMenu;
	delete m_pFRequestMenu;
	delete m_pFamilyCreateMenu;
	delete m_pFamilyInfoMenu;
	delete m_pFamilyMemberMenu;
	delete m_pFamilySkillMenu;
	delete m_pFamilyBuildMenu;
	delete m_pFamilyJoinMenu;
	delete m_pFamilyQuestMenu;
	delete m_pManorMenu;
	delete m_pManorAwardMenu;
	delete m_pFriendChooseMenu;
	delete m_pMailMenu;
	delete m_pMapCopyMenu;
	delete m_pMapCopyDropMenu;
	delete m_pStarSysMenu;
	delete m_pMCAwardMenu;
	delete m_pGreenModeMenu;
	delete m_pGreenAwardMenu;
	delete g_pHandSign;
	for(int i = 0; i < 6; i ++)
		delete m_pRankMenu[i];
	delete m_pChatMenu;
	delete m_pChatMenu2;
	delete m_pSysMsgMenu;
	delete m_pTabEquipCtrl;
	delete m_pPetGetMenu;
	delete m_pAutoMenu;
	delete m_pShowPetGetMenu;
	delete m_pSettingMenu;
	delete m_pChangePNameMenu;
	delete m_pChooseAutoDrug;
	delete m_pRechargeMenu;
	delete m_pSelObjHandleMenu;
	delete m_pSelObjInfoMenu;
	delete m_pTeamDropMenu;
	delete m_pNASaleFilterMenu;
	delete m_pACBossMenu;
	delete m_pAppraiseMenu;
	delete m_pNoticeMenu;
	delete m_pDropShowMenu;
	delete m_pFishMenu;
	delete m_pMakeSkillMenu;
	delete m_pActivateMenu;
	delete	m_pFaqHelpMenu;
	delete	m_pFaqHelpMenuEx;
	delete m_pRankDamageAwardMenu;
	delete m_pChangeNameF;
	delete m_pManorRecordMenu;
	m_pListMainEff->Free();
}

extern bool g_bNeedNotify[];

void MainMenu::Update()
{
	m_pMiniMap->Update();
	m_pBagMenu->Update();
	m_pEquipInfoMenu->Update();
	m_pItemInfoMenu->Update();
	m_pCtrlMenu->Update();
	m_pEquipStarMenu->Update();
	m_pEquipBoreMenu->Update();
	m_pEquipInlayMenu->Update();
	m_pEquipEatMenu->Update();
	m_pEquipTransferMenu->Update();
	m_pMapCopyMenu->Update();
	m_pMCAwardMenu->Update();
	m_pGreenModeMenu->Update();
	m_pBankMenu->Update();
	m_pLuckBetMenu->Update();
	m_pWarMsgMenu->Update();
	m_pSkillMenu->Update();
	m_pSPSkillMenu->Update();
	m_pNaSaleMenu->Update();
	m_pStoneComposeMenu->Update();
	m_pGetNumMenu->Update();
	m_pUserParamMenu->Update();
	m_pUserTitleMenu->Update();
	m_pAchievementAwardMenu->Update();
	m_pMallMenu->Update();
	m_pShopMenu->Update();
	m_pWarRankMenu->Update();
	m_pWarRankRewardMenu->Update();
	m_pFastSellMenu->Update();
	m_pTeamMenu->Update();
	m_pTurntableMenu->Update();
	m_pActivityMenu->Update();
	m_pACBossMenu->Update();
	m_pAppraiseMenu->Update();
	m_pActivityJoinMenu->Update();
	m_pOnlineAwardMenu->Update();
	m_pLvupAwardMenu->Update();
	m_pLoginAwardMenu->Update();
	m_pMounthAwardMenu->Update();
	m_pStarSysMenu->Update();
	m_pAddFriendMenu->Update();
	m_pFRequestMenu->Update();
	//	g_packetCenter.Update ();
	m_pAchievementShowMenu->Update();
	m_pACDegreeMenu->Update();
	m_pChatMenu->Update();
	m_pWTestRankMenu->Update();
	m_pMissionMenu->Update();
	m_pFriendMenu->Update();
	m_pFamilyListMenu->Update();
	m_pFamilyInfoMenu->Update();
	m_pFamilyMemberMenu->Update();
	m_pFamilySkillMenu->Update();
	m_pFamilyBuildMenu->Update();
	m_pFamilyJoinMenu->Update();
	m_pFamilyQuestMenu->Update();
	m_pChangePNameMenu->Update();
	m_pManorMenu->Update();
	m_pMailMenu->Update();
	m_pFriendChooseMenu->Update();
	m_pEDuraMenu->Update();
	m_pMentoringMenu->Update();
	m_pMRequestMenu->Update();
	m_pFastEquipMenu->Update();
	m_pFishMenu->Update();
	m_pMakeSkillMenu->Update();
	m_pMarryNpcMenu->Update();

	m_pMarryListMenu->Update();
	m_pMarryInfoMenu->Update();
	m_pMarrySkillMenu->Update();
	m_pMarryQuestMenu->Update();
	m_pSendRingMenu->Update();

	m_pWTestMenu->Update();
	m_pSysMsgMenu->Update();
	m_pVipMenu->Update();
	m_pRechargeMenu->Update();
	//	g_packetCenter.Update ();
	if(g_pSlaveMenuCtrl)
		g_pSlaveMenuCtrl->Update();
	if(g_pRankMenuCenter)
		g_pRankMenuCenter->Update();
	if (m_pUserDataUpdate)
	{
		m_pUserDataUpdate = false;

		//m_pShipMngMenu->m_pForm->pCFrame->SetFPCnt(pTag->nCardNum,true);
		//pChar->m_nLimitCard	= pTag->nCardNum;
	}
	m_pPetSkillMenu->Update();
	m_pPetStreMenu->Update();
	m_pPetExchMenu->Update();
	m_pPetVariaMenu->Update();
	m_pPetGetMenu->Update();
	m_pAutoMenu->Update();
	m_pSettingMenu->Update();
	m_pHorseFeedMenu->Update();
	m_pHorseCulMenu->Update();
	m_pHorseEatMenu->Update();
	m_pChatMenu->m_pItemMenu->Update();
	m_pDropShowMenu->Updata();
	m_pTeamDropMenu->Updata();
	m_pRechargeAwardMenu->Update();
	if (m_pMainFormLB->bOpened)
	{
		m_pMainFormLB->GetComponent("page2")->SetVisible(m_pTeamMenu->GetTeamedCount());
	}
	m_pMainFormTB->GetComponent("zy")->SetVisible(true);
	m_pCastMenu->Update();
	_checkEffect();

	m_pFaqHelpMenu->Update();
	m_pFaqHelpMenuEx->Update();

	if(m_pFormNetWaiting->bOpened)
	{
		int nNWRote	= 360-xnGetTickCount()/100%12 * 30;
		m_pFormNetWaiting->GetComponent("eff")->SetSprRotation(nNWRote);
	}
	//奖励按钮 begin
	DComponent* pCompTime = m_pMainFormTB->GetComponent("time");
	if(pChar->m_pOnlineAwardCenter->m_byFlag >= 5)
		pCompTime->SetVisible(false);
	else if(pChar->m_pOnlineAwardCenter->m_dwLeftsec == 0)
	{
		pCompTime->SetVisible(true);
		pCompTime->SetCaption("可领取",1);
	}
	else
	{
		pCompTime->SetVisible(true);
		pCompTime->SetCaptionEx("%02d:%02d",1,pChar->m_pOnlineAwardCenter->m_dwLeftsec/60,pChar->m_pOnlineAwardCenter->m_dwLeftsec%60);
	}
	bool ifShine = false;
	if(pChar->m_pOnlineAwardCenter->m_ifShine)
		ifShine = true;
	DComponent* pTimeButton = m_pMainFormTB->GetComponent("zx");
	if(ifShine)
		pTimeButton->SetEffect(140,1);
	else
		pTimeButton->SetEffect(0,1);

	ifShine = false;
	if(pChar->m_pMounthAwardCenter->m_ifShine)
		ifShine = true;
	if(pChar->m_pLvupAwardCenter->m_ifShine)
		ifShine = true;
	if(pChar->m_pLoginAwardCenter->m_ifShine)
		ifShine = true;
	pTimeButton = m_pMainFormTB->GetComponent("jl");
	if(ifShine)
		pTimeButton->SetEffect(140,1);
	else
		pTimeButton->SetEffect(0,1);

	ifShine = false;
	int nStar	= pChar->m_wStarLv;
	if(nStar < 10)
		nStar	= 10;
	if(nStar < pChar->m_byLvl)
	{
		ifShine = true;
		m_pFortMenu->m_pForm->GetComponent("ps")->SetEffect(271,1);
	}else
		m_pFortMenu->m_pForm->GetComponent("ps")->SetEffect(0,1);

	if (m_pGreenModeMenu->CheckShine())
	{
		ifShine = true;
		m_pFortMenu->m_pForm->GetComponent("jjc2")->SetEffect(271,1);
	}else
		m_pFortMenu->m_pForm->GetComponent("jjc2")->SetEffect(0,1);

	pTimeButton = m_pMainFormTB->GetComponent("zy");
	if(ifShine)
	{
		pTimeButton->SetEffect(140,1);
	}
	else
	{
		pTimeButton->SetEffect(0,1);
	}
	//end
	ifShine = false;
	if (pChar->m_pRechargeAwardCenter->m_ifShine)
		ifShine = true;
	pTimeButton = m_pMainFormTB->GetComponent("sc");
	if(pChar->m_pRechargeAwardCenter->m_ifGet[2])
	{
		pTimeButton->SetVisible(false);
		pTimeButton->SetEffect(0,1);
	}else
	{
		pTimeButton->SetVisible(true);
		if (ifShine)
			pTimeButton->SetEffect(140,1);
		else
			pTimeButton->SetEffect(270,1);
	}

	pTimeButton = m_pMainFormTB->GetComponent("zx");
	if(pChar->m_pOnlineAwardCenter->m_byFlag >= 5)
		pTimeButton->SetVisible(false);
	else
		pTimeButton->SetVisible(true);

	if(pChar->m_pItemCenter->m_ifNeedFix && !m_pEDuraMenu->m_pForm->bOpened)
		m_pEDuraMenu->Open();
	else if(!pChar->m_pItemCenter->m_ifNeedFix && m_pEDuraMenu->m_pForm->bOpened)
		m_pEDuraMenu->Close();

	static int nScoreFormTime = 0;
	if (m_pScore->bOpened)
	{
		if (!nScoreFormTime)
		{
			nScoreFormTime = xnGetTickCount();
		}else if(xnGetTickCount() - nScoreFormTime >= 4000){
			m_pScore->Close();
			nScoreFormTime = 0;
			int nScore = pChar->GetScore();
		}else{
			int nScore = pChar->GetScore();
			int nChg = (int)m_pScore->GetComponent("moneyC3")->GetTag();
			int nChgTime = xnGetTickCount() - nScoreFormTime;
			if (nChgTime>=2500)
			{
				nChgTime = 2500;
				m_pScore->GetComponent("moneyC3")->SetCaptionEx("%d",0,nScore);
				m_pScore->GetComponent("up")->SetCaptionEx("",0);
			}else if(nChgTime<1500){
				m_pScore->GetComponent("moneyC3")->SetCaptionEx("%d",0,nScore-nChg);
				m_pScore->GetComponent("up")->SetCaptionEx("+%d",0,nChg);
			}else{
				nChgTime-=1500;
				m_pScore->GetComponent("moneyC3")->SetCaptionEx("%d",0,nScore-nChg+nChg*nChgTime/1000);
				m_pScore->GetComponent("up")->SetCaptionEx("+%d",0,nChg-nChg*nChgTime/1000);
			}
		}
	}
	//end

	if(m_dwTickNetWaiting && xnGetTickCount() - m_dwTickNetWaiting > 1000)
		HideNetWaiting();

	if(!m_pNaSaleMenu->m_pForm->bOpened&&m_pNASaleFilterMenu->m_pForm->bOpened)
	{
		m_pNASaleFilterMenu->m_pForm->Close();
	}

	if (m_pCtrlMenu->m_pCompLvUp->bVisable)
	{
		int nChgTime = xnGetTickCount()/150%2;
		if (nChgTime == 0)
		{
			m_pCtrlMenu->m_pCompLvUp->SetViewSpr("BT_LVUP.png");
		}
		if (nChgTime == 1)
		{
			m_pCtrlMenu->m_pCompLvUp->SetViewSpr("BT_LVUP1.png");
		}
	}

	if (pChar->IsNewMail())
	{
		int nChgTime = xnGetTickCount()/150%2;
		if (nChgTime == 0)
		{
			m_pCtrlMenu->m_pCompMail->SetViewSpr("BT_MALL.png");
		}
		if (nChgTime == 1)
		{
			m_pCtrlMenu->m_pCompMail->SetViewSpr("BT_MALL1.png");
		}
	}

	m_pCtrlMenu->m_pHarvest->SetVisible(pChar->m_bHarvest);
	if (pChar->m_bHarvest)
	{
		int nChgTime = xnGetTickCount()/150%2;
		if (nChgTime == 0)
		{
			m_pCtrlMenu->m_pHarvest->SetViewSpr("BT_ZY_CS1.png");
		}
		if (nChgTime == 1)
		{
			m_pCtrlMenu->m_pHarvest->SetViewSpr("BT_ZY_CS2.png");
		}
	}

	if (pChar->m_bFightRequest)
	{
		if (xnGetSecCount() - pChar->m_dwFightRequestTick < 15)
		{
			m_pFastEquipMenu->m_pTipListForm->Open();
			m_pFastEquipMenu->m_pFightTip->SetVisible(true);
		}
		else
			pChar->DealFightRequest(false);
		
	}

	if (pChar->m_bFish)
	{
		if(pChar->m_pHorseCenter->m_ifRide)
			pChar->m_pHorseCenter->RequestRide(false);

		m_pFishMenu->Open();
		pChar->m_bFish = false;
	}

	if (pChar->m_bTeamRequest)
	{
		if (xnGetSecCount() - pChar->m_dwTeamRequestTick < 30)
		{
			m_pFastEquipMenu->m_pTipListForm->Open();
			m_pFastEquipMenu->m_pTeamInviteTip->SetVisible(true);
		}
		else
			pChar->DealTeamRequest(false);
	}

	if (pChar->m_pMarryCenter->m_bMarryRingRequest)
	{
		m_pFastEquipMenu->m_pTipListForm->Open();
		m_pFastEquipMenu->m_pMarryRingTip->SetVisible(true);
	}

	if (pMainMenu->m_pFriendMenu->m_bGift)
	{
		m_pFastEquipMenu->m_pTipListForm->Open();
		m_pFastEquipMenu->m_pFriendGiftTip->SetVisible(true);
	}

	if (pChar->m_pMentoringClient->CheckGift())
	{
		m_pFastEquipMenu->m_pTipListForm->Open();
		m_pFastEquipMenu->m_pMentoringAwardTip->SetVisible(true);
	}
	else
		m_pFastEquipMenu->m_pMentoringAwardTip->SetVisible(false);
	
	m_pFastEquipMenu->UpdateActivityIcon();
	pChar->m_pActivityCenter->OpenActWarnMenu();
}


void MainMenu::PopTalkMsg( char* szTitle,char* szMsg,int nSkin )
{
	m_pFormTalk->Open(2);
	m_pFormTalk->GetComponent("name")->SetCaption(szTitle,1);
	m_pFormTalk->GetComponent("txt")->SetCaption(szMsg,0);

	DComponent*	pDComp	= m_pFormTalk->GetComponent("tx");

	if(nSkin == 0)
	{
		pDComp->SetVisible(false);
	}
	else
	{
		char szList[64];
		char szFile[64];
		sprintf(szList,"skin/%d/1.plist",nSkin);
		sprintf(szFile,"skin/%d/%s/5",nSkin,"stand");
		pDComp->SetCartoonAction(szList,szFile,600,100);
		pDComp->SetVisible(true);
	}


	pMainInterface->m_pLockForm	= m_pFormTalk;
}

void MainMenu::PopSystemMsg( char* szMsg,bool bBtn )
{
	if(this == NULL || m_pMessageBox == NULL)
		return;
	if(bBtn)
		m_pMessageBox->PopMsgbox ("系统提示",szMsg,9,true);
	else
		m_pMessageBox->PopMsgbox ("系统提示",szMsg,0,true);
	m_pMessageBox->Func_CBMsgBox	= NULL;
	m_pMessageBox->pParamMsgbox		= NULL;
	//PopTipMsg (szMsg);
}

void MainMenu::PopAskMsgBox( char* szMsg, void (*cb_MsgBox)(void * pParam,int nVal), void* param,BYTE bType )
{
	if(this == NULL)
		return;
	m_pMessageBox->PopMsgbox ("操作提示",szMsg,bType,true);
	m_pMessageBox->Func_CBMsgBox	= cb_MsgBox;
	m_pMessageBox->pParamMsgbox		= param;
}

void MainMenu::PopInputBox(char * szTitle, char* szMsg,BYTE byInputType, void (*cb_MsgBox)(void * pParam,int nVal,char * szInput), void* param )
{
	m_pInputBox->m_pInput->byInputType = byInputType;
	m_pInputBox->Func_CBMsgBox	= cb_MsgBox;
	m_pInputBox->pParamMsgbox		= param;
	m_pInputBox->PopMsgbox (szTitle,szMsg,0,true);
}

void MainMenu::CloseMessageBox()
{
	if(!m_pMessageBox)
		return;
	pMainInterface->m_pLockForm = m_pMessageBox->m_pLastLockForm;
	m_pMessageBox->m_pForm->Close ();	
	m_pMessageBox->Func_CBMsgBox	= NULL;
	m_pMessageBox->pParamMsgbox		= NULL;
}

void MainMenu::StopCast()
{
	if(m_pCastMenu->m_dwCastSTick && xnGetTickCount() - m_pCastMenu->m_dwCastSTick > 300)
		m_pCastMenu->m_dwCastSTick	= 0;
}

bool MainMenu::IsCasting()
{
	return m_pCastMenu->m_dwCastSTick;
}

void MainMenu::StartCast( DWORD dwTime,char* szTxt,void (*func)(void * param)/*=NULL*/,void* param/*=NULL*/ , bool bLock)
{
	m_pCastMenu->m_dwCastSTick	= xnGetTickCount ();
	m_pCastMenu->m_dwCastTime	= dwTime;
	strncpy(m_pCastMenu->m_szCastTxt,szTxt,32);
	m_pCastMenu->m_pCastParam	= param;
	m_pCastMenu->m_bLock	= bLock;
	m_pCastMenu->fnCastComplete = func;
}

void MainMenu::PopTipMsg( char* szMsg,... )
{
	char msg[1024];
	va_list argptr;
	va_start(argptr,szMsg);
	vsprintf(msg, szMsg, argptr);
	pMainToopTip->ShowInterfaceTip (msg);
	va_end(argptr);
}

void MainMenu::UpdateSec()
{
	static int nTick = 0;
	nTick++;
	if (nTick>=60)
	{
		nTick = 0;
	//	UpdateMin();
	}

}
void fun_cbLeaveWarRank(void * pParam,int nVal)
{
	if(nVal == 1)
		pChar->SendCmd(USER,C_LEAVEMCOPY);
	else{
		if (!pParam)
		{
			pChar->SendCmd(USER,C_LEAVEMCOPY);
		}
	}
}
extern int nTempPos;
void MainMenu::PlayEffect( int nEffID )
{
	m_pListMainEff->Add((void*)nEffID);
	m_pEffectForm->GetComponent("eff")->SetEffect(nEffID,1);
	m_pEffectForm->Open();
}

void MainMenu::_checkEffect()
{
	if (!m_pEffectForm->bOpened)
	{
		return;
	}
	CObjButton* pObj = (CObjButton*)m_pEffectForm->GetComponent("eff")->pParam;
	if(pObj && pObj->pListEffs->Count == 0)
		m_pEffectForm->Close();
	
	if(pObj)
	{
		for (int n = m_pListMainEff->Count-1; n >= 0; n--)
		{
			int	nEffID	= (int)m_pListMainEff->Items[n];
			bool	bFind	= false;
			for (int i = 0; i < pObj->pListEffs->Count; i++)
			{
				CCEffectSprite* pEff	= (CCEffectSprite*)pObj->pListEffs->Items[i];
				if(nEffID == pEff->GetEffID())
				{
					bFind	= true;
					break;
				}
			}
			if(!bFind)
			{
				m_pListMainEff->Delete(n);
				switch(nEffID){
				case 219:
					pMainMenu->PlayEffect(220);
					break;
				case 221:
					char sztxt[128];
					sprintf(sztxt,"你已经战胜了对手,排名升至%d,点击确定离开竞技场?",nTempPos);
					pMainMenu->PopAskMsgBox(sztxt,fun_cbLeaveWarRank,this,0);
					break;
				case 222:
					if(g_pMapCenter->m_nBaseMapIndex >= 51 && g_pMapCenter->m_nBaseMapIndex <= 56)
						pMainMenu->PopAskMsgBox("你已经战败,点击确定离开翡翠梦境",fun_cbLeaveWarRank,NULL,0);
					else
						pMainMenu->PopAskMsgBox("你已经战败,点击确定离开竞技场",fun_cbLeaveWarRank,NULL,0);
					break;
				case 269:
					{
						if(pChar->m_byLvl >= 9)
						{
							for (int i = 0; i < 3; i++)
							{
								g_pMapCenter->PlayerParticle(SCREEN_SIZE_W/2,SCREEN_SIZE_H/2,2000,i+2);
							}
						}
					}
					break;
				}
			}

		}
	}
	
		
	
}

void MainMenu::ShowNetWaiting(int nSubTick)
{
	m_pFormNetWaiting->Open();
	pMainInterface->m_pLockForm	= m_pFormNetWaiting;
	m_dwTickNetWaiting	= xnGetTickCount()-nSubTick;
}

void MainMenu::HideNetWaiting()
{
	m_pFormNetWaiting->Close();
	m_dwTickNetWaiting	= 0;
}

void MainMenu::UpdateMin()
{
// 	for (int i = 0; i < 9; i++)
// 	{
// 		ItemObj* pItemSlot = pChar->m_pItemCenter->GetEquipBySlot(i);
// 		if (pItemSlot&&pItemSlot->pItemBase->bySlot != EQUIP_SLOT_TO_INDEX(ITEM_SLOT_EQUIP_SKIN)&&pItemSlot->GetVal(ITEM_AD_NUM) == 0)
// 		{ 
// 			pMainMenu->PopTipMsg("装备:%s的耐久为0", pItemSlot->pItemBase->szName);
// 		}
// 	}
}

void MainMenu::IBtnDefaulEvent( int nEvent )
{
	BuildPacketEx(pPacket,USER,C_BTN_DEFAULEVENT,buf,256);
	SET_DATA(pData,USER,C_BTN_DEFAULEVENT,pPacket);
	pData->nEvent	= nEvent;
	pChar->SendPacket(pPacket);
}

void ClickOkEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	MyMessageBox* pMenu	= (MyMessageBox*)param;
	pMenu->ResetBox();
	pMenu->ClickAnswer (true);
}

void ClickInputOkEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	MyInputBox* pMenu	= (MyInputBox*)param;
	pMenu->ClickAnswer(true);
}

void ClickCancelEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	MyMessageBox* pMenu	= (MyMessageBox*)param;
	pMenu->ResetBox();
	pMenu->ClickAnswer (false);
}

void ClickCloseEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;	
	pMainInterface->GetDForm("messagebox")->Close();
}

void ClickInputCancelEvent(InterfaceEvent * pEvent,void * param) 
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	MyInputBox* pMenu	= (MyInputBox*)param;
	pMenu->ClickAnswer (false);
}

MyMessageBox::MyMessageBox()
{
	m_pForm		= pMainInterface->GetDForm ("messagebox");

	m_pCompTitle	= m_pForm->GetComponent ("title");
	m_pCompTitle->m_nFontSize	= 22;
	m_pCompTitle->SetSprColor(200,180,33);
	m_pCompMsg		= m_pForm->GetComponent ("txt");
	m_pCompMsg->m_nFontSize	= 22;
	m_pCompMsg->SetSprColor(210,200,200);

	m_pCompOk		= m_pForm->GetComponent ("ok");
	m_pCompCancel	= m_pForm->GetComponent ("close");
	m_pCompClose	= m_pForm->GetComponent ("close1");

	m_pCompOk->SetEvent (ClickOkEvent,this);
	m_pCompCancel->SetEvent (ClickCancelEvent,this);
	m_pCompClose->SetEvent (ClickCloseEvent,this);

	m_pCompClose->SetVisible(false);

	pParamMsgbox		= NULL;
	Func_CBMsgBox		= NULL;
	m_pLastLockForm		= NULL;
	m_pForm->bZorde	= 100;
}

MyMessageBox::~MyMessageBox()
{

}

void MyMessageBox::PopMsgbox( char* szTitle, char* szMsg,int nType, bool bLock )
{
	m_pForm->Open ();

	static char* s_pNameOK = strdup(m_pCompOk->szSpr);
	static char* s_pNameCancel = strdup(m_pCompCancel->szSpr);
	static int s_nPosXOK = m_pCompOk->nX;
	static int s_nPosXCancel = m_pCompCancel->nX;
	m_pCompOk->SetPos(s_nPosXOK,247);
	m_pCompCancel->SetPos(s_nPosXCancel,247);
	m_pCompCancel->SetVisible(true);
	m_pCompCancel->SetViewSpr(s_pNameCancel);
	m_pCompOk->SetVisible(true);
	m_pCompOk->SetViewSpr(s_pNameOK);
	m_pCompClose->SetVisible(false);

	m_pCompTitle->SetCaption (szTitle,1);
	m_pCompMsg->SetCaption (szMsg,1);
	if(bLock)
	{
		m_pLastLockForm = pMainInterface->m_pLockForm;
		pMainInterface->m_pLockForm	= m_pForm;
	}
	switch(nType)
	{
	case 0:		//单独确定按钮
		{
			m_pCompOk->SetPos(250,247);
			m_pCompCancel->SetVisible(false);
			m_pCompOk->SetVisible(true);
		}
		break;
	case 1:		//确定取消
		{
			m_pCompOk->SetPos(80,247);
			m_pCompCancel->SetPos(390,247);
			m_pCompCancel->SetVisible(true);
			m_pCompOk->SetVisible(true);
		}
		break;
	case 2:
		{
			m_pCompCancel->SetPos(250,247);
			m_pCompCancel->SetVisible(true);
			m_pCompOk->SetVisible(false);
		}
		break;
	case 3:		//充值返回
		{
			m_pCompOk->SetPos(80,247);
			m_pCompCancel->SetPos(390,247);
			m_pCompCancel->SetVisible(true);
			m_pCompCancel->SetViewSpr("BT_FANHUI.png");
			m_pCompOk->SetVisible(true);
			m_pCompOk->SetViewSpr("BT_CHONGZHI.png");
		}
		break;
	case 4:		//商城返回
		{
			m_pCompOk->SetPos(80,247);
			m_pCompCancel->SetPos(390,247);
			m_pCompCancel->SetVisible(true);
			m_pCompCancel->SetViewSpr("BT_FANHUI.png");
			m_pCompOk->SetVisible(true);
			m_pCompOk->SetViewSpr("BT_TIP_SHANGCHENG.png");
		}
		break;
	case 5:		//===商城活动
		{
			m_pCompOk->SetPos(80,247);
			m_pCompCancel->SetPos(390,247);
			m_pCompCancel->SetVisible(true);
			m_pCompCancel->SetViewSpr("BT_TIP_HUODONG.png");
			m_pCompOk->SetVisible(true);
			m_pCompOk->SetViewSpr("BT_TIP_SHANGCHENG.png");
			m_pCompClose->SetVisible(false);

			m_pCompClose->SetVisible(true);
		}
		break;
	case 6:		//===商城副本
		{
			m_pCompOk->SetPos(80,247);
			m_pCompCancel->SetPos(390,247);
			m_pCompCancel->SetVisible(true);
			m_pCompCancel->SetViewSpr("BT_TIP_FUBEN.png");
			m_pCompOk->SetVisible(true);
			m_pCompOk->SetViewSpr("BT_TIP_SHANGCHENG.png");
			m_pCompClose->SetVisible(false);

			m_pCompClose->SetVisible(true);
		}
		break;
	case 7:		//生命之源使用 商城 金币使用
		{
			m_pCompOk->SetPos(80,247);
			m_pCompCancel->SetPos(390,247);
			m_pCompCancel->SetVisible(true);
			m_pCompCancel->SetViewSpr("BT_TIP_JINBI.png");
			m_pCompOk->SetVisible(true);
			m_pCompOk->SetViewSpr("BT_TIP_SHANGCHENG.png");
			m_pCompClose->SetVisible(true);
		}
		break;
	case 9:
		{
			m_pCompCancel->SetVisible(false);
			m_pCompOk->SetVisible(false);
		}
		break;
	}
}

void MyMessageBox::ClickAnswer( bool bOk )
{
	pMainInterface->m_pLockForm = m_pLastLockForm;
	if(Func_CBMsgBox)
		Func_CBMsgBox (pParamMsgbox,bOk);
	m_pForm->Close ();
}

void MyMessageBox::ResetBox()
{
	m_pCompCancel->SetViewSpr("BT_FANHUI.png");
	m_pCompOk->SetViewSpr("BT_QUEDING.png");
	m_pCompClose->SetVisible(false);
}

CastMenu::CastMenu()
{
	m_pForm	= pMainInterface->GetDForm ("dutiao");
	m_pForm->bZorde	= FORM_ZORDER_CAST;
	m_pCompCast		= m_pForm->GetComponent ("dj");
	//m_pCompCast->SetSelectSpr("ui/MB_CAIJI.png");
	m_dwCastSTick	= 0;
	m_pCastParam	= NULL;
	m_dwCastTime	= 0;
	memset(m_szCastTxt,0,32);
	fnCastComplete	= NULL;
}

void CastMenu::Update()
{
	if(m_dwCastSTick)
	{
		m_pForm->Open();
		if(m_bLock)
			pMainInterface->m_pLockForm	= m_pForm;
		DWORD	dwCurTick	= xnGetTickCount ();
		DWORD	dwPassTick	= dwCurTick-m_dwCastSTick;
		char	szTmp[64];
		if(strlen(m_szCastTxt) == 0)
		{
			int	nRmTime	= dwPassTick;
			if(nRmTime < 0)
				nRmTime	= 0;
			if(nRmTime > m_dwCastTime)
				nRmTime	= m_dwCastTime;
			sprintf(szTmp,"%d.%ds/%d.%ds",nRmTime/1000,nRmTime%1000/100,m_dwCastTime/1000,m_dwCastTime%1000/100);
		}
		else
			strncpy(szTmp,m_szCastTxt,32);
		m_pCompCast->SetProgress (dwPassTick,m_dwCastTime,szTmp);
		if(dwPassTick >= m_dwCastTime)
		{//完成
			m_dwCastSTick	= 0;
			if(fnCastComplete)
				fnCastComplete (m_pCastParam);
		}

	}
	else
	{
		m_pForm->Close();
	}
}

MyInputBox::MyInputBox()
{
	m_pForm = pMainInterface->GetDForm("messagebox2");
	m_pOk = m_pForm->GetComponent("queding");
	m_pCancel = m_pForm->GetComponent("quxiao");
	m_pInput = m_pForm->GetComponent("txt");
	m_pTitle = m_pForm->GetComponent("title");

	m_pOk->SetEvent (ClickInputOkEvent,this);
	m_pCancel->SetEvent (ClickInputCancelEvent,this);
	m_pForm->GetComponent ("close")->SetEvent (ClickInputCancelEvent,this);
	pParamMsgbox		= NULL;
	Func_CBMsgBox		= NULL;
	m_pLastLockForm		= NULL;
}

MyInputBox::~MyInputBox()
{

}

void MyInputBox::PopMsgbox( char* szTitle, char* szMsg,int nType, bool bLock )
{
	m_pForm->Open ();

	m_pTitle->SetCaption (szTitle,1);
	m_pInput->SetEdit(szMsg);
	char * fdsagdsa = m_pInput->GetEdit();
	if(bLock)
	{
		m_pLastLockForm = pMainInterface->m_pLockForm;
		pMainInterface->m_pLockForm	= m_pForm;
	}
}

void MyInputBox::ClickAnswer( bool bOk )
{
	pMainInterface->m_pLockForm = m_pLastLockForm;
	if(Func_CBMsgBox)
		Func_CBMsgBox (pParamMsgbox,bOk,m_pInput->GetEdit());
	m_pForm->Close ();
}
