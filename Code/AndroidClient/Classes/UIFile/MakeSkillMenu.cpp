#include "MainMenu.h"
#include "MakeSkillMenu.h"
#include "cocos2d.h"
#include "InterfaceLayer.h"
#include "ItemConfig.h"
#include "ios_user_packet.h"
const char* const szMakeSkillIcon[] = {"","BT_ZHIZUO2.png","BT_ZHIZUO3.png","BT_ZHIZUO4.png"
									,"BT_ZHIZUO1.png","BT_ZHIZUO5.png","BT_ZHIZUO6.png"
									,"BT_ZHIZUO7.png","BT_ZHIZUO8.png"};

const char* const szDescTxt[] = {"学习挖矿技能后，可在野外地图怒焰峡谷、熔岩洞穴、赤炎沙漠、艾尔文森林、银松森林、洛新山等地图挖掘铜矿，矿石可用来制作史诗装备。",
									"学习采集技能后，可在野外地图怒焰峡谷、熔岩洞穴、赤炎沙漠、艾尔文森林、银松森林、洛新山等地图采集亚麻和金银花，用来制作装备和日常消耗所需要的药水。"
									,"学习钓鱼技能后，可在主城、城镇废墟、艾尔文森林、银松森林、奎尔萨拉斯、艾索拉、避难海滩、海底走廊和贸易港湾的鱼群处钓鱼，钓鱼可获得烹饪用的鲑鱼和制作史诗首饰装备的鱼骨材料。"
									,"锻造：锻造装备，制作战士和火枪的史诗级装备。"
								,"裁缝：通过裁缝，制作法师和牧师的史诗级装备。","珠宝：珠宝加工，制作所有职业的史诗级项链和戒指。"
								,"炼金：炼金之术，制作日常消耗所需要的药剂。","烹饪：烹饪食物，制作可食物的各种食物。"};
using namespace cocos2d;
extern void ClickShowCompEvent(InterfaceEvent* pEvent,void* param);
extern void SetItemNameToDCom(DComponent* pDom,ItemExBase* pObj);
extern void SetItemNameToDCom(DComponent* pDom,ItemExBase* pObj,int nAlignment);
extern void	SetBagItemToDComByID(DComponent* pDom,int nID,int nNeedCount = 0);
extern void	SetItemToDCom(DComponent* pDom,int nID);
static void SwitchJob(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	int nPos = atoi(&pEvent->pComponent->szName[2]);
	MakeSkillMenu* pMenu = (MakeSkillMenu*)param;
	pMenu->Select(nPos-1);
	
}
static void SwitchMenu(InterfaceEvent* pEvent,void* param){
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if (!strcmp(pEvent->pComponent->szSpr,szMakeSkillIcon[MAKE_SKILL_TYPE_DIG]))
	{
		pMainMenu->m_pMakeSkillMenu->OpenMenu(MAKE_SKILL_TYPE_DIG);
	}else if (!strcmp(pEvent->pComponent->szSpr,szMakeSkillIcon[MAKE_SKILL_TYPE_COLLECT]))
	{
		pMainMenu->m_pMakeSkillMenu->OpenMenu(MAKE_SKILL_TYPE_COLLECT);
	}else if (!strcmp(pEvent->pComponent->szSpr,szMakeSkillIcon[MAKE_SKILL_TYPE_FISH]))
	{
		pMainMenu->m_pMakeSkillMenu->OpenMenu(MAKE_SKILL_TYPE_FISH);
	}
// 	if (!strcmp(pEvent->pComponent->szName,"dz"))
// 	{
// 		pMainMenu->m_pMakeSkillMenu->OpenMenu(0);
// 	}else{
// 		pMainMenu->m_pMakeSkillMenu->OpenMenu(1);
// 
// 	}
}
static void CBFrameFBEvent(int nPos,char* szName)
{
	if(strncmp (szName, "zz1",3) == 0)
	{
		pMainMenu->m_pMakeSkillMenu->SelectItem(nPos);
	}
}

void cb_ClickCheckGoMakeItem(void* param,int nVal)
{
	if (nVal)
	{
		pChar->m_pItemCenter->UseMakeSkill(pMainMenu->m_pMakeSkillMenu->m_pNowTag);
	}
}

static void UseMakeSkill(InterfaceEvent* pEvent,void* param){
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if (pMainMenu->m_pMakeSkillMenu->m_nMakeSelect == -1)
	{
		pMainMenu->PopTipMsg("请先选择一个制作物品");
		return;
	}
	MakeSkillTag* pTag = (MakeSkillTag*)pMainMenu->m_pMakeSkillMenu->m_pForm->pCFrame->GetFPTag(pMainMenu->m_pMakeSkillMenu->m_nMakeSelect);
	if (pTag->nVigor>pChar->m_makeSkill.nPower)
	{
		pMainMenu->PopTipMsg("精力不足");
		return;
	}
	if (pChar->m_byLvl<pTag->nLv)
	{
		pMainMenu->PopTipMsg("制作需要人物等级%d",pTag->nLv);
		return;
	}
	if (pTag->nGold > 0 && pChar->m_dwMoney < pTag->nGold)
	{
		pMainMenu->PopTipMsg("金币不足，制作需要消耗金币%d",pTag->nGold);
		return;
	}

	pMainMenu->m_pMakeSkillMenu->m_pNowTag = pTag;

	if (pTag->nGold > 0)
	{
		char szTxt[128];
		sprintf(szTxt,"是否花费%d金币，制作该物品",pTag->nGold);
		pMainMenu->PopAskMsgBox(szTxt,cb_ClickCheckGoMakeItem,(void*)0);
	}else
		pChar->m_pItemCenter->UseMakeSkill(pTag);
	//pMainMenu->m_pMakeSkillMenu->StartMake(pTag);
}
void GiveUpSkill(void* param,int nVal)
{
	if(nVal == 1)
	{
		BuildPacketEx(pPacket, USER, C_GIVEUP_MAKESKILL, buffer,1024);
		SET_DATA(pdata, USER, C_GIVEUP_MAKESKILL, pPacket);
		pdata->nType	= pMainMenu->m_pMakeSkillMenu->m_nSkillType;
		pChar->SendPacket(pPacket);
		pMainMenu->m_pMakeSkillMenu->m_pForm->Close();
		pMainMenu->m_pMakeSkillMenu->m_pFormCollect->Close();
	}
}
void fnLearnSkill(void* param,int nVal)
{
	if(nVal == 1)
	{
		pChar->m_pItemCenter->LearnMakeSkill(pMainMenu->m_pMakeSkillMenu->m_nSkillType);
	}
}
void fnBuyMakeSkillPower(void* param,int nVal)
{
	if(nVal == 1)
	{
		pChar->SendCmd(USER,C_BUY_MAKESKILL_POWER);
		//pChar->m_pItemCenter->LearnMakeSkill(pMainMenu->m_pMakeSkillMenu->m_nSkillType);
	}
}
static void GiveUpSkill(InterfaceEvent* pEvent,void* param){
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return; 
	if (pMainMenu->m_pMakeSkillMenu->m_nSkillType == 0)
	{
		return;
	}
	pMainMenu->PopAskMsgBox("是否放弃该技能?",GiveUpSkill,NULL);
}
static void OpenItemInfo(InterfaceEvent* pEvent,void* param){
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return; 
	MakeSkillTag* pTag = (MakeSkillTag*)pMainMenu->m_pMakeSkillMenu->m_pForm->pCFrame->GetFPTag(pMainMenu->m_pMakeSkillMenu->m_nMakeSelect);
	if (!pTag)
	{
		return;
	}
	ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(pTag->tagGet.nItemID);
	if (!pBase)
	{
		return;
	}
	if (pBase->byType == ITEMTYPE_EQUIP)
	{
		pMainMenu->m_pEquipInfoMenu->Open(pBase,FORM_EX);
		pMainMenu->m_pEquipInfoMenu->SetButtonSpr(NULL);
	}else{
		pMainMenu->m_pItemInfoMenu->Open(pBase,FORM_EX);
		pMainMenu->m_pItemInfoMenu->SetButtonSpr(NULL);
	}

}
static void SwitchPageMenu(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nType = (int)param;
	pMainMenu->m_pMakeSkillMenu->m_pFormCollect->Close();
	pMainMenu->m_pSkillCtrl->Open(0,nType);
}
static void OpenSkillMenu(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nType = (int)param;
	pMainMenu->m_pMakeSkillMenu->m_pFormCollect->Close();
	pMainMenu->m_pMakeSkillMenu->OpenMenu(nType);
}
static void ReturnSkillMenu(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pMakeSkillMenu->m_pFormCollect->Close();
	pMainMenu->m_pMakeSkillMenu->OpenMenu(0);
}
static void FilterSkillMenu(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nType = (int)param;
	pMainMenu->m_pMakeSkillMenu->OpenCombo(nType);
}
static void LearnMakeSkill(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->PopAskMsgBox("是否学习该生活技能?",fnLearnSkill,NULL);
}
static void AddMakeSkillPower(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	if (pChar->m_dwRmb<20)
	{
		pMainMenu->PopTipMsg("钻石不足20");
		return;
	}
	pMainMenu->PopAskMsgBox("是否花费20钻石购买精力?",fnBuyMakeSkillPower,NULL);
}
static void CBFrameEventSelect(int nPos,char* szName,void* param)
{
	MakeSkillMenu* pMenu = (MakeSkillMenu*)param;

	if (strncmp (szName, "qy1",3) == 0&&pMenu->m_nSelectFilterType >= 0&&pMenu->m_nSelectFilterType<2)
	{
 		
		switch(pMenu->m_nSelectFilterType)
		{
		case 0:
			{
				pMenu->m_nSelect = nPos;
				pMenu->SetMakeList();
				//pMainMenu->m_pNaSaleMenu->m_pForm->GetComponent(szFilterTypeTxt[pMenu->m_nFilterType])->SetCaption(NASaleFilter1[nPos],0);
				//pMainMenu->m_pNaSaleMenu->FilterUpdata();
			}
			break;
		case 1:
			{
				pMenu->m_szFilter = pMenu->m_pSelectForm->pCFrame->GetFPComp(nPos,szName)->szCaption;
				pMenu->SetMakeList();
// 				pMainMenu->m_pNaSaleMenu->m_nFilterType[1] = nPos;
// 				pMainMenu->m_pNaSaleMenu->m_pForm->GetComponent(szFilterTypeTxt[pMenu->m_nFilterType])->SetCaption(NASaleFilter2[nPos],0);
// 				pMainMenu->m_pNaSaleMenu->FilterUpdata();
			}
			break;
		}
	}
	pMenu->m_pSelectForm->Close();
}
MakeSkillMenu::MakeSkillMenu():m_nSelect(0),m_nChgPos(0),m_nMakeSelect(-1),m_nSkillType(0),m_pNowTag(NULL),m_nMake1(0),m_nMake2(0),m_nSelectFilterType(-1)
{
	m_pForm = pMainInterface->GetDForm ("zhizuo1");

	m_pForm->pCFrame	= new CCtrlFrame(340,366,210,200);
	DComponent*	pComp	= m_pForm->GetComponent ("zz2");

	DComponent* pComItem[2];
	pComItem[0] = m_pForm->GetComponent("zz1");
	pComItem[1] = m_pForm->GetComponent("zz3");

	m_pForm->pCFrame->InitFParts(pComp,pComItem,2);
	m_pForm->pCFrame->SetEvent (CBFrameFBEvent);
	//m_pForm->pCFrame->SetStepEvent(CBFrameFBStepEvent);
	m_pForm->pCFrame->nSpace	= 8;
	m_pForm->pCFrame->nDisStep	= 50;

	char sztxt[32];
	for (int i=0;i<2;i++)
	{
		sprintf(sztxt,"zy%d",i+1);
		m_pSwitch[i] = m_pForm->GetComponent(sztxt);
		m_pSwitch[i]->SetEvent(SwitchJob,this);
		m_pSwitch[i]->m_bChgPosbySpr = true;
	}

	//m_pForm->GetComponent("dz")->SetEvent(SwitchMenu);
	m_pForm->GetComponent("wk")->SetEvent(SwitchMenu);
	m_pForm->GetComponent("wk1")->SetEvent(SwitchMenu);
	m_pForm->GetComponent("wk2")->SetEvent(SwitchMenu);
	m_pForm->GetComponent("zz")->SetEvent(UseMakeSkill);
	m_pForm->GetComponent("xx")->SetEvent(LearnMakeSkill);
	m_pForm->GetComponent("wang")->SetEvent(GiveUpSkill);
	m_pForm->GetComponent("icon")->SetEvent(OpenItemInfo);
	m_pForm->GetComponent("fh")->SetEvent(ReturnSkillMenu);


	m_pForm->GetComponent("xia1")->SetEvent(FilterSkillMenu,(void*)0);
	m_pForm->GetComponent("xia2")->SetEvent(FilterSkillMenu,(void*)1);
	m_pForm->GetComponent("add")->SetEvent(AddMakeSkillPower);

	m_pFormCollect = pMainInterface->GetDForm("zhizuo2");

	m_pFormCollect->GetComponent("dz")->SetEvent(OpenSkillMenu,(void*)MAKE_SKILL_TYPE_FORGE);
	m_pFormCollect->GetComponent("fr")->SetEvent(OpenSkillMenu,(void*)MAKE_SKILL_TYPE_SEWING);
	m_pFormCollect->GetComponent("zb")->SetEvent(OpenSkillMenu,(void*)MAKE_SKILL_TYPE_STONE);
	m_pFormCollect->GetComponent("lj")->SetEvent(OpenSkillMenu,(void*)MAKE_SKILL_TYPE_METALLURGY);
	m_pFormCollect->GetComponent("pr")->SetEvent(OpenSkillMenu,(void*)MAKE_SKILL_TYPE_FOOD);

	for (int i=0;i<3;i++)
	{
		sprintf(sztxt,"page%d",i+1);
		if(i!=1) m_pFormCollect->GetComponent(sztxt)->SetSprColor(127,127,127);
		m_pFormCollect->GetComponent(sztxt)->SetEvent(SwitchPageMenu,(void*)i);
	}
	m_pFormCollect->GetComponent("wang")->SetEvent(GiveUpSkill);


	m_pFormMake = pMainInterface->GetDForm("zhizuo3");

	m_pSelectForm = pMainInterface->GetDForm("zhizuo4");
	DComponent*	pCompBase	= m_pSelectForm->GetComponent("d1");
	m_pSelectForm->pCFrame	= new CCtrlFrame(pCompBase->nX,pCompBase->nY,pCompBase->nW,(pCompBase->nH+15)*5);
	DComponent*	ppCompPt[2];
	ppCompPt[0]		= m_pSelectForm->GetComponent("qy1");
	ppCompPt[1]		= m_pSelectForm->GetComponent("x1");
	m_pSelectForm->pCFrame->InitFParts(pCompBase,ppCompPt,2);
	m_pSelectForm->pCFrame->nSpace	= 15;
	m_pSelectForm->pCFrame->nDisStep	= pCompBase->nH+15;
	m_pSelectForm->pCFrame->SetEventEx(CBFrameEventSelect,this);


	m_pFormCollectEx = pMainInterface->GetDForm("zhizuo5");
	m_pMakeLayer = new MakeLayer;
	
}

MakeSkillMenu::~MakeSkillMenu()
{

}

void MakeSkillMenu::SetMakeList()
{
	//m_byType = byType;
	int nJob = 0;
	if (m_byType == 0)//锻造
	{
		nJob = m_nSelect?3:1;
	}else if (m_byType == 1)//裁缝
	{
		nJob = m_nSelect?4:2;
	}else if (m_byType>=5)
	{
		return;
	} 
	static char* szJobtxt[]={"全部","战士","法师","火枪手","牧师"};
	m_pForm->GetComponent("zy")->SetCaption(szJobtxt[nJob],1);
	xnList* pList = xnList::Create();
	MakeSkillConfig::GetInstance().GetMakeList(m_byType,nJob,m_szFilter,pList);
	m_pForm->GetComponent("lv")->SetCaption(m_szFilter.c_str(),1);
	int nCount = pList->Count;
	m_pForm->pCFrame->nFrmY -= m_nChgPos;
	m_pForm->pCFrame->bChged = true;
	m_pForm->pCFrame->SetFPCnt(nCount);
	m_nChgPos = 0;
	for (int i=0;i<nCount;i++)
	{
		MakeSkillTag* pItem = (MakeSkillTag*)pList->Items[i];
		SetItemNameToDCom(m_pForm->pCFrame->GetFPComp(i,"zz1"),ItemConfig::GetInstance().GetItemBase(pItem->tagGet.nItemID),1);
		m_pForm->pCFrame->SetFPTag(i,pItem);
		m_pForm->pCFrame->GetFPComp(i,"zz3")->SetVisible(false);
		m_pForm->pCFrame->GetFPComp(i,"zz1")->SetSprColor(pChar->m_byLvl>=pItem->nLv?255:70,pChar->m_byLvl>=pItem->nLv?255:70,pChar->m_byLvl>=pItem->nLv?255:70);
		//m_pForm->pCFrame->GetFPCompBase(i)->SetCaptionEx("aaa%d",0,i);
	}
	m_nMakeSelect = -1;
	if (nCount)
	{
		SelectItem(0);
	}else{
		SetMakeItem(NULL);
	}
	pList->Free();
	
}

// void MakeSkillMenu::Open()
// {
// 	
// }

void MakeSkillMenu::Open( int nT )
{
	m_pForm->GetComponent("dz")->SetViewSpr(szMakeSkillIcon[pChar->m_makeSkill.bySkill[0]]);
	//m_pForm->GetComponent("wk")->SetViewSpr(szMakeSkillIcon[pChar->m_makeSkill.bySkill[1]]);
	//m_pFormCollect->GetComponent("dz")->SetViewSpr(szMakeSkillIcon[pChar->m_makeSkill.bySkill[0]]);
	//m_pFormCollect->GetComponent("wk")->SetViewSpr(szMakeSkillIcon[pChar->m_makeSkill.bySkill[1]]);
	OpenMenu(pChar->m_makeSkill.bySkill[0]);
}

void MakeSkillMenu::Update()
{
	if (m_pFormMake->bOpened)
	{
		FormLayer* pFormLayer = g_pLayerIF->GetFormLayer(m_pFormMake);
		if (pFormLayer)
		{
			if (!pFormLayer->getChildByTag(9998))
			{
				pMainMenu->PlayEffect(261);
				m_pMakeLayer->setPosition(ccp(43/POINT_PIX_CNT,-585/POINT_PIX_CNT));
				pFormLayer->addChild(m_pMakeLayer,999,9998);
			}
		}
	}
}

void MakeSkillMenu::Select( int nSelect )
{
	if (m_nSelect == nSelect)
	{
		return;
	}
	int nDistance = 255+(abs(m_nSelect-nSelect)-1)*40;
	CCActionInterval * pJump	= CCMoveBy::create(0.2f,ccp(0,m_nSelect<nSelect?(nDistance/POINT_PIX_CNT):(-nDistance/POINT_PIX_CNT)));
	CCAction * pCallback1 = CCCallFunc::create(this, callfunc_selector(MakeSkillMenu::_afterMove)); 
	CCFiniteTimeAction* pRun1	= CCSequence::create (pJump,pCallback1,NULL);
	if (m_nSelect<nSelect)
	{
		for (int i=m_nSelect+1;i<=nSelect;i++)
		{
			if(m_pSwitch[i]->pParam){ 
				if(((CCSprite*)m_pSwitch[i]->pParam)->runAction(pRun1))	m_nMoveCount++;
			}
		}
	}else{
		for (int i=m_nSelect;i>nSelect;i--)
		{
			if(m_pSwitch[i]->pParam){
				if(((CCSprite*)m_pSwitch[i]->pParam)->runAction(pRun1)) m_nMoveCount++;
			}
		}
	}

	FrameLayer* pLayer = (FrameLayer*)m_pForm->pCFrame->pFrame;
	if (pLayer)
	{
		m_nChgPos = (m_nSelect-nSelect)*53/POINT_PIX_CNT;
		CCActionInterval * pFade	= CCFadeOut::create(0.2f);
		CCActionInterval * pFadeMove	= CCMoveBy::create(0.2f,ccp(100/POINT_PIX_CNT,0));
		CCAction * pCallback2 = CCCallFunc::create(this, callfunc_selector(MakeSkillMenu::_afterMove)); 
		CCFiniteTimeAction* pRun	= CCSequence::create (CCSpawn::createWithTwoActions(pFade,pFadeMove),pCallback2,NULL);
		pLayer->runAction(pRun);
		m_nMoveCount++;
	}

// 	for (int i = 0;i<m_pForm->pCFrame->GetFPCnt();i++)
// 	{
// 		CCActionInterval * pFade	= CCFadeOut::create(0.2f);
// 		CCActionInterval * pFadeMove	= CCMoveBy::create(0.2f,ccp(100/POINT_PIX_CNT,0));
// 		CCAction * pCallback2 = CCCallFunc::create(this, callfunc_selector(MakeSkillMenu::_afterMove)); 
// 		CCFiniteTimeAction* pRun	= CCSequence::create (CCDelayTime::create(nTime),CCSpawn::createWithTwoActions(pFade,pFadeMove),pCallback2,NULL);
// 		//CCFiniteTimeAction* pRun	= CCSequence::create (pFadeMove,pCallback1,NULL);
// 		DComponent* pCom = m_pForm->pCFrame->GetFPCompBase(i);
// 		if (pCom->pParam)
// 		{
// 			((CCSprite*)pCom->pParam)->runAction(pRun);
// 			m_nMoveCount++;
// 		}
// 		nTime += 0.05f;
// 	}
	m_nSelect = nSelect;
}

void MakeSkillMenu::_afterMove()
{
	m_nMoveCount--;
	if (!m_nMoveCount)
	{
// 		float nTime = 0;
// 
// 		for (int i = 0;i<pMainMenu->m_pMakeSkillMenu->m_pForm->pCFrame->GetFPCnt();i++)
// 		{
// 			CCActionInterval * pFade	= CCFadeIn::create(0.2f);
// 			CCActionInterval * pFadeMove	= CCMoveBy::create(0.2f,ccp(-100/POINT_PIX_CNT,0));
// 			CCFiniteTimeAction* pRun	= CCSequence::create (CCDelayTime::create(nTime),CCSpawn::createWithTwoActions(pFade,pFadeMove),NULL);
// 			//CCFiniteTimeAction* pRun	= CCSequence::create (pFadeMove,NULL);
// 			DComponent* pCom = pMainMenu->m_pMakeSkillMenu->m_pForm->pCFrame->GetFPCompBase(i);
// 			if (pCom->pParam)
// 			{
// 				((CCSprite*)pCom->pParam)->runAction(pRun);
// 			}
// 			nTime += 0.1f;
// 		}
		pMainMenu->m_pMakeSkillMenu->SetMakeList();
// 		FrameLayer* pLayer = (FrameLayer*)pMainMenu->m_pMakeSkillMenu->m_pForm->pCFrame->pFrame;
// 		if (pLayer)
// 		{
// 			CCActionInterval * pFade	= CCFadeIn::create(0.2f);
// 			CCActionInterval * pFadeMove	= CCMoveBy::create(0.2f,ccp(-100/POINT_PIX_CNT,0));
// 			//CCAction * pCallback2 = CCCallFunc::create(this, callfunc_selector(MakeSkillMenu::_afterMove)); 
// 			CCFiniteTimeAction* pRun	= CCSequence::create (CCSpawn::createWithTwoActions(pFade,pFadeMove),NULL);
// 			pLayer->runAction(pRun);
// 		}
	}
}

void MakeSkillMenu::SetMakeType( BYTE byType )
{
	static char* szSelectTxt[] = {"BT_ZHIZUO_ZY5.png","BT_ZHIZUO_ZY1.png","BT_ZHIZUO_ZY2.png","BT_ZHIZUO_ZY3.png","BT_ZHIZUO_ZY4.png"};
	static char* szSkillTypeTxt[] = {"BT_ZHIZUO1.png","BT_ZHIZUO_ZY1.png","BT_ZHIZUO_ZY2.png","BT_ZHIZUO_ZY3.png","BT_ZHIZUO_ZY4.png"};
	m_byType = byType;
	m_pSwitch[1]->SetVisible(true);
	m_pForm->GetComponent("dz")->SetViewSpr(szMakeSkillIcon[byType+MAKE_SKILL_TYPE_FORGE]);
	m_pForm->GetComponent("wk1")->SetVisible(false);
	m_pForm->GetComponent("wk2")->SetVisible(false);
	if (byType == MAKE_SKILL_TYPE_FORGE-MAKE_SKILL_TYPE_FORGE)
	{
		m_pSwitch[0]->SetViewSpr(szSelectTxt[1]);
		m_pSwitch[1]->SetViewSpr(szSelectTxt[3]);
		m_pForm->GetComponent("wk")->SetViewSpr(szMakeSkillIcon[MAKE_SKILL_TYPE_DIG]);
		m_pForm->GetComponent("wk1")->SetViewSpr(szMakeSkillIcon[MAKE_SKILL_TYPE_COLLECT]);
		m_pForm->GetComponent("wk1")->SetVisible(true);
	}else if (byType == MAKE_SKILL_TYPE_SEWING-MAKE_SKILL_TYPE_FORGE)
	{
		m_pSwitch[0]->SetViewSpr(szSelectTxt[2]);
		m_pSwitch[1]->SetViewSpr(szSelectTxt[4]);
		m_pForm->GetComponent("wk")->SetViewSpr(szMakeSkillIcon[MAKE_SKILL_TYPE_DIG]);
		m_pForm->GetComponent("wk1")->SetViewSpr(szMakeSkillIcon[MAKE_SKILL_TYPE_COLLECT]);
		m_pForm->GetComponent("wk1")->SetVisible(true);
	}else if(byType == MAKE_SKILL_TYPE_STONE-MAKE_SKILL_TYPE_FORGE)
	{
		m_pSwitch[0]->SetViewSpr(szSelectTxt[0]);
		m_pSwitch[1]->SetVisible(false);
		m_pForm->GetComponent("wk")->SetViewSpr(szMakeSkillIcon[MAKE_SKILL_TYPE_DIG]);
		m_pForm->GetComponent("wk1")->SetVisible(true);
		m_pForm->GetComponent("wk1")->SetViewSpr(szMakeSkillIcon[MAKE_SKILL_TYPE_COLLECT]);
		m_pForm->GetComponent("wk2")->SetVisible(true);
		m_pForm->GetComponent("wk2")->SetViewSpr(szMakeSkillIcon[MAKE_SKILL_TYPE_FISH]);
	}else if(byType == MAKE_SKILL_TYPE_METALLURGY-MAKE_SKILL_TYPE_FORGE)
	{
		m_pSwitch[0]->SetViewSpr(szSelectTxt[0]);
		m_pSwitch[1]->SetVisible(false);
		m_pForm->GetComponent("wk")->SetViewSpr(szMakeSkillIcon[MAKE_SKILL_TYPE_COLLECT]);
	}else if(byType == MAKE_SKILL_TYPE_FOOD-MAKE_SKILL_TYPE_FORGE)
	{
		m_pSwitch[0]->SetViewSpr(szSelectTxt[0]);
		m_pSwitch[1]->SetVisible(false);
		m_pForm->GetComponent("wk")->SetViewSpr(szMakeSkillIcon[MAKE_SKILL_TYPE_FISH]);
	}
}

void MakeSkillMenu::SetMakeItem( MakeSkillTag* pTag )
{
	SetItemToDCom(m_pForm->GetComponent("icon"),pTag?pTag->tagGet.nItemID:0);
	SetItemNameToDCom(m_pForm->GetComponent("name12"),ItemConfig::GetInstance().GetItemBase(pTag?pTag->tagGet.nItemID:0));
	if (pTag)
	{
		m_pForm->GetComponent("icon")->SetCaptionEx("%d",2,pTag->tagGet.nItemCount);
	}else{
		m_pForm->GetComponent("icon")->SetCaptionEx("",2);
	}

	char sztxt[32];
	for (int i=0;i<4;i++)
	{
		sprintf(sztxt,"icon%d",i+1);
		m_pForm->GetComponent(sztxt)->SetVisible(pTag&&i<pTag->pListCost->Count);
		sprintf(sztxt,"name%d",i+1);
		m_pForm->GetComponent(sztxt)->SetVisible(pTag&&i<pTag->pListCost->Count);
	}
	if (pTag)
	{
		for (int i=0;i<pTag->pListCost->Count;i++)
		{
			CommonItemTag* pItem = (CommonItemTag*)pTag->pListCost->Items[i];
			sprintf(sztxt,"icon%d",i+1);
			SetItemToDCom(m_pForm->GetComponent(sztxt),pItem->nItemID);
			m_pForm->GetComponent(sztxt)->SetCaptionEx("%d/%d",2,pChar->m_pItemCenter->GetItemCountByID(pItem->nItemID),pItem->nItemCount);
			m_pForm->GetComponent(sztxt)->SetEvent(ClickShowCompEvent,(void*)pItem->nItemID);
			sprintf(sztxt,"name%d",i+1);
			SetItemNameToDCom(m_pForm->GetComponent(sztxt),ItemConfig::GetInstance().GetItemBase(pItem->nItemID));
		}
	}
	m_pForm->GetComponent("sx")->SetCaptionEx("%d/%d",1,pChar->m_makeSkill.nPower,GetPowerMax());
	m_pForm->GetComponent("xh")->SetCaptionEx("%d",1,pTag?pTag->nVigor:0);

}

void MakeSkillMenu::OpenMenu( int nSkill )
{
// 	if (nSkill<0||nSkill>=nMakeSkillCount)
// 	{
// 		return;
// 	}
	if (nSkill==MAKE_SKILL_TYPE_NULL)
	{
		m_pFormCollect->Open(1);
		return;
	}
	static const char* const szNameTxt[] = {"挖矿","采集","钓鱼","锻造","裁缝","珠宝","炼金","烹饪"};
	if (nSkill>=MAKE_SKILL_TYPE_FORGE&&nSkill<=MAKE_SKILL_TYPE_FOOD)
	{
		SetMakeType(nSkill-MAKE_SKILL_TYPE_FORGE);
		m_nSelect = 0;
		m_szFilter = "";
		SetMakeList();
		bool bLearn = (pChar->m_makeSkill.bySkill[0] == nSkill);
		m_pForm->GetComponent("fh")->SetVisible(!bLearn);
		m_pForm->GetComponent("xx")->SetVisible(!bLearn);
		m_pForm->GetComponent("wang")->SetVisible(bLearn);
		m_pForm->GetComponent("zz")->SetVisible(bLearn);
		int nCnt = pChar->m_pClientFlag->GetFlag(nMakeSkillResetTimesFlag);
		if (nCnt == 0)
		{
			m_pForm->GetComponent("money")->SetCaptionEx("需要消耗10000金币",1);
		}else if (nCnt == 1)
		{
			m_pForm->GetComponent("money")->SetCaptionEx("需要消耗50000金币",1);
		}else{
			m_pForm->GetComponent("money")->SetCaptionEx("需要消耗50钻石",1);
		}
		//m_pForm->GetComponent("zz")->
		//m_pForm->GetComponent("miaoshu")->SetCaption(szDescTxt[pChar->m_makeSkill.bySkill[nSkill]-1],0);
		m_pForm->GetComponent("name")->SetCaption(szNameTxt[nSkill-1],1);
		m_pForm->Open(1);
		m_pFormCollect->Close();

	}else{
		m_pFormCollectEx->GetComponent("zi")->SetCaption(szDescTxt[nSkill-1],0);
		m_pFormCollectEx->GetComponent("name")->SetCaption(szNameTxt[nSkill-1],1);
		m_pFormCollectEx->Open(1);
		return;
		//m_pForm->Close();
	}
	m_nSkillType = nSkill;
}

void MakeSkillMenu::SelectItem( int nSelect )
{
	for (int i=0;i<m_pForm->pCFrame->GetFPCnt();i++)
	{
		m_pForm->pCFrame->GetFPComp(i,"zz3")->SetVisible(i == nSelect);
		if(i == nSelect) SetMakeItem((MakeSkillTag*)m_pForm->pCFrame->GetFPTag(i));
	}
	m_nMakeSelect = nSelect;
}
static void UpdateVal(){
	MakeSkillTag* pTag = pMainMenu->m_pMakeSkillMenu->m_pNowTag;
	if (!pTag)
	{
		return;
	}
	if (pMainMenu->m_pMakeSkillMenu->m_nMake1<=0&&pMainMenu->m_pMakeSkillMenu->m_nMake2<=0)
	{
		pChar->m_pItemCenter->MakeSkillResult(1);
		pMainMenu->m_pMakeSkillMenu->SetMakeItem(pTag);
	}
	MakeSkillMenu* pMenu = pMainMenu->m_pMakeSkillMenu;
	pMenu->m_pFormMake->GetComponent("num1")->SetCaptionEx("%d/%d",0,pMenu->m_nMake1>0?pTag->nMake1-pMenu->m_nMake1:pTag->nMake1,pTag->nMake1);
	pMenu->m_pFormMake->GetComponent("num2")->SetCaptionEx("%d/%d",0,pMenu->m_nMake2>0?pTag->nMake2-pMenu->m_nMake2:pTag->nMake2,pTag->nMake2);
}
static void fnCallBackEvent(int nEvent,int nVal){
	if (nEvent == 0)
	{
		if (nVal == 0)
		{
			pMainMenu->m_pMakeSkillMenu->m_nMake1--;
			UpdateVal();
		}else if (nVal == 1)
		{
			pMainMenu->m_pMakeSkillMenu->m_nMake2--;
			UpdateVal();
		}
	}else if (nEvent == 1)
	{
		pChar->m_pItemCenter->MakeSkillResult(0);
	}
}

void MakeSkillMenu::StartMake( MakeSkillTag* pTag )
{
	if (!pTag)
	{
		return;
	}
	char sztxt[32];
	sprintf(sztxt,"MB_SANXIAO%d.png",m_byType+1);
	m_pFormMake->GetComponent("ico1")->SetViewSpr(sztxt);
	sprintf(sztxt,"MB_SANXIAO%d.png",m_byType+6);
	m_pFormMake->GetComponent("ico2")->SetViewSpr(sztxt);
	m_pFormMake->GetComponent("num1")->SetCaptionEx("0/%d",0,pTag->nMake1);
	m_pFormMake->GetComponent("num2")->SetCaptionEx("0/%d",0,pTag->nMake2);
	m_nMake1 = pTag->nMake1;
	m_nMake2 = pTag->nMake2;
	m_pFormMake->Open();
	m_pMakeLayer->SetType(m_byType,fnCallBackEvent);

}

int MakeSkillMenu::GetPowerMax()
{
	int nlv = pChar->m_byLvl;
	if (nlv<40)
	{
		return 70;
	}else if (nlv<50)
	{
		return 90;
	}else if (nlv<60)
	{
		return 110;
	}else if (nlv<70)
	{
		return 130;
	}else if (nlv<80)
	{
		return 150;
	}else if (nlv<90)
	{
		return 170;
	}else if (nlv<100)
	{
		return 190;
	}else
	{
		return 210;
	}
}

void MakeSkillMenu::OpenCombo( BYTE byType )
{
	if (m_nSelectFilterType == byType&&m_pSelectForm->bOpened)
	{

		m_pSelectForm->Close();
		return;
	}
	if (byType == 0)
	{
		switch (m_nSkillType)
		{
		case MAKE_SKILL_TYPE_FORGE:
			{
				m_pSelectForm->pCFrame->SetFPCnt(2);
				m_pSelectForm->pCFrame->GetFPComp(0,"qy1")->SetCaption("战士",1);
				m_pSelectForm->pCFrame->GetFPComp(1,"qy1")->SetCaption("火枪手",1);
			}
			break;
		case MAKE_SKILL_TYPE_SEWING:
			{
				m_pSelectForm->pCFrame->SetFPCnt(2);
				m_pSelectForm->pCFrame->GetFPComp(0,"qy1")->SetCaption("法师",1);
				m_pSelectForm->pCFrame->GetFPComp(1,"qy1")->SetCaption("牧师",1);
			}
			break;
		default:
			{
				m_pSelectForm->pCFrame->SetFPCnt(1);
				m_pSelectForm->pCFrame->GetFPComp(0,"qy1")->SetCaption("全部",1);
			}
			break;
		}
		m_pSelectForm->SetPos(m_pForm->GetComponent("xia1")->nX,m_pForm->GetComponent("xia1")->nY+m_pForm->GetComponent("xia1")->nH);
		m_pSelectForm->Open();
	}else if (byType == 1)
	{
		map<tempstring,map<int,MakeSkillTag*> >& smap = MakeSkillConfig::GetInstance().m_mapInfo[m_nSkillType-MAKE_SKILL_TYPE_FORGE];
		m_pSelectForm->pCFrame->SetFPCnt(smap.size());
		int i=0;
		for (map<tempstring,map<int,MakeSkillTag*> >::iterator itor = smap.begin();itor!= smap.end();itor++)
		{
			m_pSelectForm->pCFrame->GetFPComp(i,"qy1")->SetCaption(itor->first.c_str(),1);
			i++;
		}
		m_pSelectForm->Open();
		m_pSelectForm->SetPos(m_pForm->GetComponent("xia2")->nX,m_pForm->GetComponent("xia2")->nY+m_pForm->GetComponent("xia2")->nH);
	}
	m_nSelectFilterType = byType;
}

void MakeSkillMenu::Close()
{
	m_pSelectForm->Close();
	m_pFormCollect->Close();
	m_pForm->Close();
}

bool MakeSkillMenu::CheckCanOpen()
{
	if (pChar->m_byLvl<35)
	{
		pMainMenu->PopTipMsg("需要等级达到35级才能学习生活技能");
		return false;
	}
	return true;
}


int MakeSkillMenu::m_nMoveCount = 0;
