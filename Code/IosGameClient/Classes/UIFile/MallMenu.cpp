#include "MainMenu.h"
#include "UserData.h"
#include "MallMenu.h"
#include "ios_item_packet.h"
#include "MallConfig.h"
extern void	SetWeaponToDCom(DComponent* pDom,int nID,char* szAction = "stand",int nDirection = 5);
extern void	SetCharToDCom(DComponent* pDom,BYTE byJob,BYTE bySex,char* szAction = "stand",int nDirection = 5);
extern void	SetCharToDCom(DComponent* pDom,int	nSkin,char* szAction = "stand",int nDirection = 5);
extern void	SetItemNameToDCom(DComponent* pDom,ItemExBase* pObj);
static void CBFrameFBEvent(int nPos,char* szName)
{
	if(strncmp (szName, "sp1",3) == 0||strncmp (szName, "sp2",3) == 0)
	{
		MallItemCfg* pCfg = (MallItemCfg*)pMainMenu->m_pMallMenu->m_pForm->pCFrame->GetFPComp(nPos,szName)->GetTag();
		ItemExBase* pItem = ItemConfig::GetInstance().GetItemBase(pCfg?pCfg->m_nItemID:0);
		if (pItem)
		{
			if (pItem->byType == ITEMTYPE_EQUIP)
			{
				/*if (pItem->bySlot == 9)
				{
					pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_MALL,(void*)pCfg);
				}else
				{*/
				pMainMenu->m_pEquipInfoMenu->Open(pItem,FORM_MALL,(void*)pCfg);
			}else{
				pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_MALL,(void*)pCfg);
			}
		}
// 		ItemExBase* pItem = (ItemExBase*)pMainMenu->m_pMallMenu->m_pForm->pCFrame->GetFPComp(nPos,szName)->GetTag();
// 		if (pItem)
// 		{
// 			pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_MALL);
// 		}
	}else if (strncmp (szName, "S1",2) == 0||strncmp (szName, "S2",2) == 0)
	{
		int nPos = 1;
		sscanf(szName,"S%d",&nPos);
		char sztxt[32];
		sprintf(sztxt,"sp%d",nPos);
		MallItemCfg* pCfg = (MallItemCfg*)pMainMenu->m_pMallMenu->m_pForm->pCFrame->GetFPComp(nPos,sztxt)->GetTag();
		ItemExBase* pItem = ItemConfig::GetInstance().GetItemBase(pCfg?pCfg->m_nItemID:0);
		if (pItem)
		{
			if (pItem->byType == ITEMTYPE_EQUIP)
			{
				pMainMenu->m_pEquipInfoMenu->Open(pItem,FORM_MALL,(void*)pCfg);
			}else{
				pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_MALL,(void*)pCfg);
			}
		}
	}else if (strncmp (szName, "XX",2) == 0)
	{
		int nPosss = 1;
		sscanf(szName,"XX%d",&nPosss);
		char sztxt[32];
		sprintf(sztxt,"sp%d",nPosss);
		//ItemExBase* pItem = (ItemExBase*)pMainMenu->m_pMallMenu->m_pForm->pCFrame->GetFPComp(nPos,sztxt)->GetTag();
		MallItemCfg* pCfg = (MallItemCfg*)pMainMenu->m_pMallMenu->m_pForm->pCFrame->GetFPComp(nPos,sztxt)->GetTag();
		ItemExBase* pItem = ItemConfig::GetInstance().GetItemBase(pCfg?pCfg->m_nItemID:0);
		if (pItem)
		{
			//MallItemCfg* pCfg = MallConfig::GetInstance().GetCfg(pItem->nTypeID);
			if(pCfg) pMainMenu->PopTipMsg("%s限购:%d/%d",pCfg->m_byType==0?(char*)"":(pCfg->m_byType==1?(char*)"每日":(char*)"每周"),pChar->m_pClientFlag->GetSLimitF(pCfg->m_nFlag),pCfg->m_nLimit);
		}
	}
}

static void CBFrameFBEventMallEx(int nPos,char* szName)
{
	if(strncmp (szName, "sp3",3) == 0)
	{
		MallItemCfg* pCfg = (MallItemCfg*)pMainMenu->m_pMallMenu->m_pMallEx->m_pForm->pCFrame->GetFPComp(nPos,"sp3")->GetTag();
		ItemExBase* pItem = ItemConfig::GetInstance().GetItemBase(pCfg->m_nItemID);
		//ItemExBase* pItem = (ItemExBase*)pMainMenu->m_pMallMenu->m_pMallEx->m_pForm->pCFrame->GetFPComp(nPos,szName)->GetTag();
		if (pItem)
		{		
			//MallItemCfg* pItems = MallConfig::GetInstance().GetCfg(pItem->nTypeID); 
			int nFlag = pChar->m_pClientFlag->GetSLimitF(pCfg->m_nFlag);
			if (pCfg->m_byType == 1&&nFlag>=pCfg->m_nLimit)
			{
					pMainMenu->PopTipMsg("您已达到购买上限");
					return;				
			}
			if (pItem->byType == ITEMTYPE_EQUIP)
			{
				pMainMenu->m_pEquipInfoMenu->Open(pItem,FORM_MALL,(void*)pCfg);
			}else{
				pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_MALL,(void*)pCfg);		
			}							
		}
	}else if (strncmp (szName, "m2",2) == 0)
	{
		MallItemCfg* pCfg = (MallItemCfg*)pMainMenu->m_pMallMenu->m_pMallEx->m_pForm->pCFrame->GetFPComp(nPos,"sp3")->GetTag();
		ItemExBase* pItem = ItemConfig::GetInstance().GetItemBase(pCfg->m_nItemID);
		if (pItem&&pCfg)
		{		
			//MallItemCfg* pItems = MallConfig::GetInstance().GetCfg(pItem->nTypeID); 
			int nFlag = pChar->m_pClientFlag->GetSLimitF(pCfg->m_nFlag);
			if (pCfg->m_byType == 1&&nFlag>=pCfg->m_nLimit)
			{				
					pMainMenu->PopTipMsg("您已达到购买上限");
					return;				
			}
					
			if (pItem->byType == ITEMTYPE_EQUIP)
			{
				pMainMenu->m_pEquipInfoMenu->Open(pItem,FORM_MALL,(void*)pCfg);
			}else{
				pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_MALL,(void*)pCfg);		
			}
		}
	}else if (strncmp (szName, "XX",2) == 0)
	{
		MallItemCfg* pCfg = (MallItemCfg*)pMainMenu->m_pMallMenu->m_pMallEx->m_pForm->pCFrame->GetFPComp(nPos,"sp3")->GetTag();
		ItemExBase* pItem = ItemConfig::GetInstance().GetItemBase(pCfg->m_nItemID);
		//ItemExBase* pItem = (ItemExBase*)pMainMenu->m_pMallMenu->m_pMallEx->m_pForm->pCFrame->GetFPComp(nPos,"sp3")->GetTag();
		if (pItem)
		{
			//MallItemCfg* pCfg = MallConfig::GetInstance().GetCfg(pItem->nTypeID);
			if(pCfg) pMainMenu->PopTipMsg("%s限购:%d/%d",pCfg->m_byType==0?(char*)"":(pCfg->m_byType==1?(char*)"每日":(char*)"每周"),pChar->m_pClientFlag->GetSLimitF(pCfg->m_nFlag),pCfg->m_nLimit);
		}
	}
}
static void PageSwitch(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pMallMenu->m_bMallUpdate = true;
	pMainMenu->m_pMallMenu->m_nNowSelectPage = (int)param;
}
static void TurnDirection(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pMallMenu->ChangeDirection((bool)param);
}
static void ResetSkinEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pMallMenu->ResetSkin();
}
static void RechargeEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pRechargeMenu->OpenForm(2);
}
static void CloseEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	MallMenu* pMenu = (MallMenu*)param;
	if (pMenu)
	{
		pMenu->Close();
	}
}
extern void	SetItemToDCom(DComponent* pDom,int nID);
MallMenu::MallMenu():m_bMallUpdate(true),m_nNowSelectPage(1),m_nWeapon(0),m_nCloth(0),m_nDirection(5),m_nHorse(0),m_bifPianYi(true),m_bHorseStand(true),m_nHorseHeight(0)
{
	m_pForm = pMainInterface->GetDForm ("mall");
	char sztxt[32];

	DComponent* pComItem[15];
	pComItem[2] = m_pForm->GetComponent ("sp1");
	pComItem[3] = m_pForm->GetComponent ("sp2");
	pComItem[0] = m_pForm->GetComponent ("icon1");
	pComItem[1] = m_pForm->GetComponent ("icon2");
	pComItem[4] = m_pForm->GetComponent ("name1");
	pComItem[5] = m_pForm->GetComponent ("name2");
	pComItem[6] = m_pForm->GetComponent ("zuan1");
	pComItem[7] = m_pForm->GetComponent ("zuan2");
	pComItem[8] = m_pForm->GetComponent ("Z1");
	pComItem[9] = m_pForm->GetComponent ("Z2");
	pComItem[10] = m_pForm->GetComponent ("M");
	pComItem[11] = m_pForm->GetComponent ("ZZ1");
	pComItem[12] = m_pForm->GetComponent ("ZZ2");
	pComItem[13] = m_pForm->GetComponent ("XX1");
	pComItem[14] = m_pForm->GetComponent ("XX2");

	DComponent*	pComp	= m_pForm->GetComponent ("m1");
	m_pForm->pCFrame	= new CCtrlFrame(pComp->nX,pComp->nY,pComp->nW,(pComp->nH+5)*3);

	m_pForm->pCFrame->InitFParts(pComp,pComItem,15);
	m_pForm->pCFrame->SetEvent (CBFrameFBEvent);
	m_pForm->pCFrame->nSpace	= 5;
	m_pForm->pCFrame->nDisStep	= 10;

	//m_pForm->pCFrame->SetFPCnt(nBagPageMax);

	//m_pForm->GetComponent("zhengli")->SetEvent(ItemTest);

	for (int i=0;i<6;i++)
	{
		sprintf(sztxt,"page%d",i+1);
		m_pForm->GetComponent(sztxt)->SetEvent(PageSwitch,(void*)(i+1));
	}
	m_pForm->GetComponent("left1")->SetEvent(TurnDirection,(void*)false);
	m_pForm->GetComponent("right1")->SetEvent(TurnDirection,(void*)true);
	m_pForm->GetComponent("hy")->SetEvent(ResetSkinEvent);
	m_pForm->GetComponent("cz")->SetEvent(RechargeEvent);
	m_pForm->GetComponent("close")->SetEvent(CloseEvent,this);

	m_pMallEx = new MallMenuEx();
}

MallMenu::~MallMenu()
{
	delete m_pMallEx;
}

void MallMenu::Open()
{
	m_pForm->Open();
	m_pMallEx->Open();

}

void MallMenu::Close()
{
	ResetSkin();
	m_pMallEx->Close();
	m_pForm->Close();
}

void MallMenu::Update()
{
	if (!m_pForm->bOpened&&m_pMallEx->m_pForm->bOpened)
	{
		m_pMallEx->Close();
	}
	if (!m_pForm->bOpened||!m_pMallEx->m_pForm->bOpened)
	{
		return;
	}
	if (m_bMallUpdate)
	{
		m_bMallUpdate = false;
		bool bSwitch = false;
		if (m_nCloth>1500||pChar->m_xUserParam.nSkin>1500)
		{
			bSwitch = true;
			if (pChar->m_byJob==2)
			{
				bSwitch = false;
			}
		}else
		{
			if (pChar->m_byJob == 2&&m_nDirection == 5) 
			{
				bSwitch = true;
			}else
				bSwitch = false;
			if (pChar->m_byJob == 4)
			{
				if (m_nDirection == 1)
				{
					bSwitch = true;
				}
				if (pChar->m_bySex == 1)
				{
					if (m_nDirection == 3||m_nDirection == 7)
					{
						bSwitch = true;
					}
				}			
			}
		}
		if (!m_nHorse)
		{
			if (m_nHorseHeight&&!m_bHorseStand)
			{
				m_bHorseStand = true;
				m_pForm->GetComponent("model")->SetPos(m_pForm->GetComponent("model")->nX,m_pForm->GetComponent("model")->nY+m_nHorseHeight);
				m_pForm->GetComponent("model2")->SetPos(m_pForm->GetComponent("model2")->nX,m_pForm->GetComponent("model2")->nY+m_nHorseHeight);
				//m_pForm->GetComponent("model3")->SetPos(m_pForm->GetComponent("model3")->nX,m_pForm->GetComponent("model3")->nY+m_nHorseHeight);
				//m_pForm->GetComponent("model4")->SetPos(m_pForm->GetComponent("model4")->nX,m_pForm->GetComponent("model4")->nY+m_nHorseHeight);
			}
			m_pForm->GetComponent("model3")->SetCartoonAction("","",0);
			m_pForm->GetComponent("model4")->SetCartoonAction("","",0);
			if (m_nCloth>1500||pChar->m_xUserParam.nSkin>1500)
			{
				SetCharToDCom(m_pForm->GetComponent(bSwitch?(char*)"model2":(char*)"model"),m_nCloth?m_nCloth:pChar->m_xUserParam.nSkin,"stand",m_nDirection);
				m_pForm->GetComponent(bSwitch?(char*)"model":(char*)"model2")->SetVisible(false);
			}else{
				m_pForm->GetComponent("model")->SetVisible(true);
				m_pForm->GetComponent("model2")->SetVisible(true);
				SetCharToDCom(m_pForm->GetComponent(bSwitch?(char*)"model2":(char*)"model"),pChar->m_xUserParam.nSkin,"stand",m_nDirection);
				SetWeaponToDCom(m_pForm->GetComponent(bSwitch?(char*)"model":(char*)"model2"),m_nWeapon>1000?m_nWeapon:pChar->m_pItemCenter->GetWeaponSkin(),"stand",m_nDirection);
			}
		}else
		{
			ObjSkinInfo* pSkin = CfgObjSkin::GetInstance().GetObjSkin(m_nHorse);
			if (m_bHorseStand)
			{
				m_bHorseStand = false;
				m_nHorseHeight = pSkin->pnVal[3];
				m_pForm->GetComponent("model")->SetPos(m_pForm->GetComponent("model")->nX,m_pForm->GetComponent("model")->nY-pSkin->pnVal[3]);
				m_pForm->GetComponent("model2")->SetPos(m_pForm->GetComponent("model2")->nX,m_pForm->GetComponent("model2")->nY-pSkin->pnVal[3]);
				//m_pForm->GetComponent("model3")->SetPos(m_pForm->GetComponent("model3")->nX,m_pForm->GetComponent("model3")->nY-pSkin->pnVal[3]);
				//m_pForm->GetComponent("model4")->SetPos(m_pForm->GetComponent("model4")->nX,m_pForm->GetComponent("model4")->nY-pSkin->pnVal[3]);
			}
			if (pSkin->nTitleH != 80&&m_bifPianYi)
			{
				m_bifPianYi = false;
				m_pForm->GetComponent("model2")->SetPos(m_pForm->GetComponent("model2")->nX,m_pForm->GetComponent("model2")->nY + (80 - pSkin->nTitleH));
				m_pForm->GetComponent("model")->SetPos(m_pForm->GetComponent("model")->nX,m_pForm->GetComponent("model")->nY + (80 - pSkin->nTitleH));
			}else if (pSkin->nTitleH == 80&&!m_bifPianYi)
			{
			    m_bifPianYi = true;
				m_pForm->GetComponent("model2")->SetPos(m_pForm->GetComponent("model3")->nX,m_pForm->GetComponent("model3")->nY - m_nHorseHeight);
				m_pForm->GetComponent("model")->SetPos(m_pForm->GetComponent("model3")->nX,m_pForm->GetComponent("model3")->nY - m_nHorseHeight);
			}
			if (pChar->m_byJob == 2)
			{
				if (m_nDirection == 1)
				{
					bSwitch = true;
				}else if (m_nDirection == 5)
				{
					bSwitch = false;
				}
			}
			m_pForm->GetComponent("model")->SetVisible(true);
			m_pForm->GetComponent("model2")->SetVisible(true);
			SetCharToDCom(m_pForm->GetComponent(bSwitch?(char*)"model2":(char*)"model"),m_nCloth?m_nCloth:pChar->m_xUserParam.nSkin,"hstand",m_nDirection);
			m_pForm->GetComponent(bSwitch?(char*)"model":(char*)"model2")->SetVisible(pChar->m_xUserParam.nSkin>1500||m_nCloth>1500? false:true);
			SetWeaponToDCom(m_pForm->GetComponent(bSwitch?(char*)"model":(char*)"model2"),m_nWeapon>1000?m_nWeapon:pChar->m_pItemCenter->GetWeaponSkin(),"hstand",m_nDirection);
			if (m_nDirection == 5)
			{
				m_pForm->GetComponent("model3")->SetCartoonAction("","",0);
				SetCharToDCom(m_pForm->GetComponent("model4"),m_nHorse,"hstandex",m_nDirection);
			}else
			{
				m_pForm->GetComponent("model4")->SetCartoonAction("","",0);
				SetCharToDCom(m_pForm->GetComponent("model3"),m_nHorse,"hstand",m_nDirection);
			}
		}
		// 		char szList[64];
		// 		char szFile[64];
		// 		sprintf(szList,"skin/%d/1.plist",1000+pChar->m_byJob*2+(pChar->m_bySex?0:1));
		// 		sprintf(szFile,"skin/%d/stand/5",1000+pChar->m_byJob*2+(pChar->m_bySex?0:1));
		// 		m_pForm->GetComponent("model")->SetCartoonAction(szList,szFile,600,100);


		char sztxt[32];
		char szSpr[32];

		xnList* pList = xnList::Create();
		pChar->m_pItemCenter->GetMallItemByType(pList,m_nNowSelectPage);
		//MallConfig::GetInstance().GetMallItemByType(pList,m_nNowSelectPage);
		if (pList->Count)
		{
			m_pForm->pCFrame->SetFPCnt((pList->Count-1)/2+1);
			m_pForm->pCFrame->GetFPComp((pList->Count-1)/2,"name2")->SetVisible(pList->Count%2 != 1);
			m_pForm->pCFrame->GetFPComp((pList->Count-1)/2,"sp2")->SetVisible(pList->Count%2 != 1);
			m_pForm->pCFrame->GetFPComp((pList->Count-1)/2,"zuan2")->SetVisible(pList->Count%2 != 1);
			m_pForm->pCFrame->GetFPComp((pList->Count-1)/2,"icon2")->SetVisible(pList->Count%2 != 1);
			m_pForm->pCFrame->GetFPComp((pList->Count-1)/2,"Z2")->SetVisible(pList->Count%2 != 1);
			m_pForm->pCFrame->GetFPComp((pList->Count-1)/2,"ZZ2")->SetVisible(pList->Count%2 != 1);
			m_pForm->pCFrame->GetFPComp((pList->Count-1)/2,"XX2")->SetVisible(pList->Count%2 != 1);
			m_pForm->pCFrame->GetFPComp((pList->Count-1)/2,"M")->SetVisible(false);
		}else{
			m_pForm->pCFrame->SetFPCnt(0);
		}
		for (int i=0;i<pList->Count;i++)
		{
			MallItemCfg* pCfg = (MallItemCfg*)pList->Items[i];
			SetItemToDCom(m_pForm->pCFrame->GetFPComp(i/2,i%2?(char*)"sp2":(char*)"sp1"),pCfg->m_nItemID);
			m_pForm->pCFrame->GetFPComp(i/2,i%2?(char*)"sp2":(char*)"sp1")->SetTag(pCfg);
			ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(pCfg->m_nItemID);
			if (pBase)
			{
				SetItemNameToDCom(m_pForm->pCFrame->GetFPComp(i/2,i%2?(char*)"name2":(char*)"name1"),pBase);
				//->SetCaption(pBase->szName,0);
			}
			if (pCfg->m_nLimit == -1)
			{
				m_pForm->pCFrame->GetFPComp(i/2,i%2?(char*)"zuan2":(char*)"zuan1")->SetCaption("展示",0);
				m_pForm->pCFrame->GetFPComp(i/2,i%2?(char*)"Z2":(char*)"Z1")->SetVisible(false);
			}
			else
			{
				m_pForm->pCFrame->GetFPComp(i/2,i%2?(char*)"zuan2":(char*)"zuan1")->SetCaptionEx("%d",0,pCfg->m_nPrice*pCfg->m_nDiscount/100);
				m_pForm->pCFrame->GetFPComp(i/2,i%2?(char*)"Z2":(char*)"Z1")->SetVisible(true);
			}

			if (pCfg->m_nFlag != -1)
			{
				m_pForm->pCFrame->GetFPComp(i/2,i%2?(char*)"XX2":(char*)"XX1")->SetVisible(true);
				m_pForm->pCFrame->GetFPComp(i/2,i%2?(char*)"ZZ2":(char*)"ZZ1")->SetVisible(false);
			}else if (pCfg->m_nDiscount<100)
			{
				m_pForm->pCFrame->GetFPComp(i/2,i%2?(char*)"XX2":(char*)"XX1")->SetVisible(false);
				m_pForm->pCFrame->GetFPComp(i/2,i%2?(char*)"ZZ2":(char*)"ZZ1")->SetVisible(true);
				m_pForm->pCFrame->GetFPComp(i/2,i%2?(char*)"ZZ2":(char*)"ZZ1")->SetCaptionEx("%d",1,pCfg->m_nDiscount/10);
			}else{
				m_pForm->pCFrame->GetFPComp(i/2,i%2?(char*)"XX2":(char*)"XX1")->SetVisible(false);
				m_pForm->pCFrame->GetFPComp(i/2,i%2?(char*)"ZZ2":(char*)"ZZ1")->SetVisible(false);
			}
			m_pForm->pCFrame->GetFPComp(i/2,i%2?(char*)"Z2":(char*)"Z1")->SetViewSpr(m_nNowSelectPage == 6?"MB_BANGZUAN.png":"MB_ZUANSHI.png");
		}


		for (int i=0;i<6;i++)
		{
			sprintf(sztxt,"page%d",i+1);
			int nColor = (i+1 == m_nNowSelectPage)?255:70;
			m_pForm->GetComponent(sztxt)->SetSprColor(nColor,nColor,nColor);
		}

	}
	m_pForm->GetComponent("zuan")->SetCaptionEx("%d",1,pChar->m_dwRmb);
	m_pForm->GetComponent("money")->SetCaptionEx("%d",1,pChar->m_dwBindRmb);
	m_pForm->GetComponent("z")->SetVisible(m_nNowSelectPage == 6);
	m_pMallEx->Update();
}

void MallMenu::ChangeDirection( bool bRight )
{
	//5正面 3右 7左 1背	
	if (m_nDirection == 5)
	{
		m_nDirection = bRight?3:7;
	}else if (m_nDirection == 3)
	{
		m_nDirection = bRight?1:5;
	}else if (m_nDirection == 1)
	{
		m_nDirection = bRight?7:3;
	}else{
		m_nDirection = bRight?5:1;
	}
	m_bMallUpdate = true;
}

void MallMenu::ResetSkin()
{
	m_nWeapon = 0;
	m_nCloth = 0;
	m_nHorse = 0;
	m_nDirection = 5;
	m_bMallUpdate = true;
	m_pForm->GetComponent("model3")->SetCartoonAction("","",0);
	m_pForm->GetComponent("model4")->SetCartoonAction("","",0);
}

MallMenuEx::MallMenuEx():m_bMallExUpdate(true)
{
	m_pForm = pMainInterface->GetDForm ("mall2");
	char sztxt[32];

	DComponent* pComItem[6];
	pComItem[0] = m_pForm->GetComponent ("icon3");
	pComItem[1] = m_pForm->GetComponent ("sp3");
	pComItem[2] = m_pForm->GetComponent ("name3");
	pComItem[3] = m_pForm->GetComponent ("zuan3");
	pComItem[4] = m_pForm->GetComponent ("ZZ3");
	pComItem[5] = m_pForm->GetComponent ("XX3");
	m_pForm->pCFrame	= new CCtrlFrame(730,140,189,360);
	DComponent*	pComp	= m_pForm->GetComponent ("m2");

	m_pForm->pCFrame->InitFParts(pComp,pComItem,6);
	m_pForm->pCFrame->SetEvent (CBFrameFBEventMallEx);
	m_pForm->pCFrame->nSpace	= 5;
	m_pForm->pCFrame->nDisStep	= 10;



}

MallMenuEx::~MallMenuEx()
{

}

void MallMenuEx::Open()
{
	m_pForm->Open();
}

void MallMenuEx::Close()
{
	m_pForm->Close();
}

void MallMenuEx::Update()
{
	if (m_bMallExUpdate)
	{
		m_bMallExUpdate = false;

		char szSpr[32];

		xnList* pList = xnList::Create();
		pChar->m_pItemCenter->GetMallItemByType(pList,0);
		//MallConfig::GetInstance().GetMallItemByType(pList,0);
		m_pForm->pCFrame->SetFPCnt(pList->Count);
		for (int i=0;i<pList->Count;i++)
		{
			MallItemCfg* pCfg = (MallItemCfg*)pList->Items[i];
			SetItemToDCom(m_pForm->pCFrame->GetFPComp(i,"sp3"),pCfg->m_nItemID);
			m_pForm->pCFrame->GetFPComp(i,"sp3")->SetTag(pCfg);
			ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(pCfg->m_nItemID);
			if (pBase)
			{
				SetItemNameToDCom(m_pForm->pCFrame->GetFPComp(i,"name3"),pBase);
				//m_pForm->pCFrame->GetFPComp(i,"name3")->SetCaption(pBase->szName,0);
				//char sztxt[32];
				//sprintf(sztxt,"item/%d.png",pBase->nSpr);
				//m_pForm->pCFrame->GetFPComp(i,"sp3")->SetViewSpr(sztxt);
			}
			m_pForm->pCFrame->GetFPComp(i,"zuan3")->SetCaptionEx("%d",0,pCfg->m_nPrice*pCfg->m_nDiscount/100);
			if (pCfg->m_nFlag != -1)
			{
				m_pForm->pCFrame->GetFPComp(i,"XX3")->SetVisible(true);
				m_pForm->pCFrame->GetFPComp(i,"ZZ3")->SetVisible(false);
			}else if (pCfg->m_nDiscount<100)
			{
				m_pForm->pCFrame->GetFPComp(i,"XX3")->SetVisible(false);
				m_pForm->pCFrame->GetFPComp(i,"ZZ3")->SetVisible(true);
				m_pForm->pCFrame->GetFPComp(i,"ZZ3")->SetCaptionEx("%d",1,pCfg->m_nDiscount/10);
			}else{
				m_pForm->pCFrame->GetFPComp(i,"XX3")->SetVisible(false);
				m_pForm->pCFrame->GetFPComp(i,"ZZ3")->SetVisible(false);
			}
		}

	}
}
