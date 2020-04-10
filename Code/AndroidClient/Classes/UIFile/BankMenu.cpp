#include "MainMenu.h"
#include "UserData.h"
#include "BankMenu.h"
#include "ios_item_packet.h"

static void UseItemEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	ItemObj* pItem = (ItemObj*)pEvent->pComponent->GetTag();
	if (pItem)
	{
		if (pItem->pItemBase->byType == ITEMTYPE_EQUIP)
		{
			if (pItem->pItemBase->bySlot == 9)
			{
				pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_BANK);
			}else
				pMainMenu->m_pEquipInfoMenu->Open(pItem,FORM_BANK);
		}else{
			pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_BANK);
		}
	}
}
static void ItemTest(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	//pChar->SendCmd(ITEM,C_ITEM_TEST);
}

static void CBFrameFBEvent(int nPos,char* szName)
{
	if(strncmp (szName, "bag",3) == 0)
	{
		ItemObj* pItem = (ItemObj*)pMainMenu->m_pBankMenu->m_pForm->pCFrame->GetFPComp(nPos,szName)->GetTag();
		if (pItem)
		{
			if (pItem->pItemBase->byType == ITEMTYPE_EQUIP)
			{
				if (pItem->pItemBase->bySlot == 9)
				{
					pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_BANK);
				}else
					pMainMenu->m_pEquipInfoMenu->Open(pItem,FORM_BANK);
			}else{
				pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_BANK);
			}
		}
	}
}
static void CBFrameFBStepEvent(int nPos)
{
	if (nPos<0)
	{
		return;
	}
	if (nPos!=pMainMenu->m_pBankMenu->m_nSelectPage)
	{
		pMainMenu->m_pBankMenu->m_nSelectPage = nPos;
		if (nPos>=1)
		{
			pMainMenu->m_pBankMenu->UpdatePage(nPos-1);
		}		
		if (nPos<nBagPageMax-1)
		{
			pMainMenu->m_pBankMenu->UpdatePage(nPos+1);
		}
	}
	pMainMenu->m_pBankMenu->UpdatePage(nPos);
}
static void CBFrameFBEventBank(int nPos,char* szName)
{
	if(strncmp (szName, "ck",2) == 0)
	{
		ItemObj* pItem = (ItemObj*)pMainMenu->m_pBankMenu->m_pBankEx->m_pForm->pCFrame->GetFPComp(nPos,szName)->GetTag();
		if (pItem&&pItem->pItemBase)
		{
			if (pItem->pItemBase->byType == ITEMTYPE_EQUIP)
			{
				if (pItem->pItemBase->bySlot == 9)
				{
					pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_BANK);
				}else
					pMainMenu->m_pEquipInfoMenu->Open(pItem,FORM_BANK);
			}else{
				pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_BANK);
			}
			//pMainMenu->m_pEquipInfoMenu->Open(pItem,FORM_BANK);
			//pChar->m_pItemCenter->UseItem(pItem);
		}
	}
}
static void CBFrameFBStepEventBank(int nPos)
{
	if (nPos<0)
	{
		return;
	}
	if (nPos!=pMainMenu->m_pBankMenu->m_pBankEx->m_nSelectPage)
	{
		pMainMenu->m_pBankMenu->m_pBankEx->m_nSelectPage = nPos;
		if (nPos>=1)
		{
			pMainMenu->m_pBankMenu->m_pBankEx->UpdatePage(nPos-1);
		}		
		if (nPos< 4)
		{
			pMainMenu->m_pBankMenu->m_pBankEx->UpdatePage(nPos+1);
		}
	}
	pMainMenu->m_pBankMenu->m_pBankEx->UpdatePage(nPos);
}
static void ItemSort(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pChar->m_pItemCenter->SortBag();
}
extern void	SetItemToDCom(DComponent* pDom,ItemObj* pObj);
BankMenu::BankMenu():m_bBagUpdate(true),m_nSelectPage(0),m_bBankUpdate(true)
{
	m_pForm = pMainInterface->GetDForm ("cangku");
	char sztxt[32];
	
	DComponent* pComItem[20];
	for (int i=0;i<20;i++)
	{
		sprintf(sztxt,"bag%d",i+1);
// 		m_pBagItem[i] = m_pForm->GetComponent(sztxt);
// 		m_pBagItem[i]->SetEvent(UseItemEvent);
		pComItem[i] = m_pForm->GetComponent(sztxt);
		pComItem[i]->SetVisible(false);
	}
	m_pForm->pCFrame	= new CCtrlFrame(470,145,460,340);
	DComponent*	pComp	= m_pForm->GetComponent ("di3");

	m_pForm->pCFrame->InitFParts(pComp,pComItem,20);
	m_pForm->pCFrame->SetEvent (CBFrameFBEvent);
	m_pForm->pCFrame->SetStepEvent(CBFrameFBStepEvent);
	m_pForm->pCFrame->nSpace	= 5;
	m_pForm->pCFrame->nDisStep	= 460;

	m_pForm->pCFrame->SetFPCnt(nBagPageMax,true);
	
	m_pForm->GetComponent("zhengli")->SetEvent(ItemSort);

	m_pBankEx = new BankMenuEx();
}

BankMenu::~BankMenu()
{
	delete m_pBankEx;
}

void BankMenu::Open()
{
	m_pForm->Open();
	m_pBankEx->Open();
}

void BankMenu::Close()
{
	m_pBankEx->Close();
	m_pForm->Close();
}

void BankMenu::Update()
{
	if (!m_pForm->bOpened&&m_pBankEx->m_pForm->bOpened)
	{
		m_pBankEx->Close();
	}
	if (!m_pForm->bOpened||!m_pBankEx->m_pForm->bOpened)
	{
		return;
	}
	if (m_bBagUpdate)
	{
		m_bBagUpdate = false;
		
		char szSpr[32];
		if (m_nSelectPage < 1)
		{
			UpdatePage(0);
		}else
			UpdatePage(m_nSelectPage);
//		for (int i=m_nSelectPage*20;i<m_nSelectPage*20+20;i++)
//		{
//			sprintf(szSpr,"bag%d",i%20+1);
//			SetItemToDCom(m_pForm->pCFrame->GetFPComp(m_nSelectPage,szSpr),NULL);
//// 			m_pBagItem[i]->SetViewSpr("");
//// 			m_pBagItem[i]->SetTag(NULL);
//		}
//
//		xnList* pList = xnList::Create();
//		pChar->m_pItemCenter->GetItemInBagByPos(pList);
//		for (int i= 0;i<pList->Count;i++)
//		{
//			ItemObj* pItem = (ItemObj*)pList->Items[i];
//			int nPos = pItem->GetVal(ITEM_AD_POS);
//			if (nPos<m_nSelectPage*20+20&&nPos>=m_nSelectPage*20)
//			{
//				sprintf(szSpr,"bag%d",nPos%20+1);
//				SetItemToDCom(m_pForm->pCFrame->GetFPComp(m_nSelectPage,szSpr),pItem);
//			}	
//		}
//		pList->Free();

	}
	m_pForm->GetComponent("zuan")->SetCaptionEx("%d",1,pChar->m_dwRmb);
	m_pForm->GetComponent("jinbi")->SetCaptionEx("%d",1,pChar->m_dwMoney);
	m_pForm->GetComponent("money")->SetCaptionEx("%d",1,pChar->m_dwBindRmb);
	m_pBankEx->Update();
}

void BankMenu::UpdatePage( int nSelectPage )
{
//	char sztxt[32];
	char szSpr[32];
//	sprintf(sztxt,"BT_SHAIZI.png");
	for (int i=nSelectPage*20;i<nSelectPage*20+20;i++)
	{
		sprintf(szSpr,"bag%d",i%20+1);
		SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),NULL);
	}

	xnList* pList = xnList::Create();
	pChar->m_pItemCenter->GetItemInBagByPos(pList);
	for (int i= 0;i<pList->Count;i++)
	{
		ItemObj* pItem = (ItemObj*)pList->Items[i];
		int nPos = pItem->GetVal(ITEM_AD_POS);
		if (nPos<nSelectPage*20+20&&nPos>=nSelectPage*20)
		{
			sprintf(szSpr,"bag%d",nPos%20+1);
			SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),pItem);
		}	
	}
	pList->Free();

	for (int i = 0; i < 5; i++)
	{
		char sztxt[32];
		sprintf(sztxt,"p%d",i+1);
		m_pForm->GetComponent(sztxt)->SetViewSpr( i== nSelectPage?(char*)"MB_DIAN_B.png":(char*)"MB_DIAN_S.png");
	}	
}

BankMenuEx::BankMenuEx():m_nSelectPage(0),m_bBankUpdate(true)
{
	m_pForm = pMainInterface->GetDForm ("cangku2");
	char sztxt[32];

	DComponent* pComItem[16];
	for (int i=0;i<16;i++)
	{
		sprintf(sztxt,"ck%d",i+1);
		// 		m_pBagItem[i] = m_pForm->GetComponent(sztxt);
		// 		m_pBagItem[i]->SetEvent(UseItemEvent);
		pComItem[i] = m_pForm->GetComponent(sztxt);
		pComItem[i]->SetVisible(false);
	}
	m_pForm->pCFrame	= new CCtrlFrame(40,150,362,338);
	DComponent*	pComp	= m_pForm->GetComponent ("di4");

	m_pForm->pCFrame->InitFParts(pComp,pComItem,16);
	m_pForm->pCFrame->SetEvent (CBFrameFBEventBank);
	m_pForm->pCFrame->SetStepEvent(CBFrameFBStepEventBank);
	m_pForm->pCFrame->nSpace	= 5;
	m_pForm->pCFrame->nDisStep	= 367;

	m_pForm->pCFrame->SetFPCnt(nBagPageMax,true);

}

BankMenuEx::~BankMenuEx()
{

}

void BankMenuEx::Open()
{
	//m_nSelectPage = 0;
	m_pForm->Open();
}

void BankMenuEx::Close()
{
	m_pForm->Close();
}

void BankMenuEx::Update()
{
	if (m_bBankUpdate)
	{
		m_bBankUpdate = false;

		char szSpr[32];

		if (m_nSelectPage < 1)
		{
			UpdatePage(0);
		}else
			UpdatePage(m_nSelectPage);

		//for (int i=0;i<16;i++)
		//{
		//	sprintf(szSpr,"ck%d",i%16+1);
		//	SetItemToDCom(m_pForm->pCFrame->GetFPComp(m_nSelectPage,szSpr),NULL);
		//}

		//xnList* pList = xnList::Create();
		//pChar->m_pItemCenter->GetItemInBankByPos(pList);
		//for (int i= 0;i<pList->Count;i++)
		//{
		//	ItemObj* pItem = (ItemObj*)pList->Items[i];
		//	int nPos = pItem->GetVal(ITEM_AD_POS);
		//	if (nPos<16*(m_nSelectPage+1)&&nPos>=0)
		//	{
		//		sprintf(szSpr,"ck%d",nPos%16+1);
		//		SetItemToDCom(m_pForm->pCFrame->GetFPComp(m_nSelectPage,szSpr),pItem);
		//	}	
		//}
		//pList->Free();	
	}
}

void BankMenuEx::UpdatePage( int nSelectPage )
{
	char szSpr[32];
	for (int i=nSelectPage*16;i<nSelectPage*16+16;i++)
	{
		sprintf(szSpr,"ck%d",i%16+1);
		SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),NULL);
	}

	xnList* pList = xnList::Create();
	pChar->m_pItemCenter->GetItemInBankByPos(pList);
	for (int i= 0;i<pList->Count;i++)
	{
		ItemObj* pItem = (ItemObj*)pList->Items[i];
		int nPos = pItem->GetVal(ITEM_AD_POS);
		if (nPos<nSelectPage*16+16&&nPos>=nSelectPage*16)
		{
			sprintf(szSpr,"ck%d",nPos%16+1);
			SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),pItem);
		}	
	}
	pList->Free();

	for (int i = 0; i < 5; i++)
	{
		char sztxts[32];
		sprintf(sztxts,"cp%d",i+1);
		pMainInterface->GetDForm ("cangku")->GetComponent(sztxts)->SetViewSpr( i== nSelectPage?(char*)"MB_DIAN_B.png":(char*)"MB_DIAN_S.png");
	}	
}
