#include "MainMenu.h"
#include "MailMenu.h"
#include "UserData.h"
#include "ios_user_packet.h"
extern UserData*	pChar;
extern void	SetItemToDCom(DComponent* pDom,ItemObj* pObj);

static int ReSortMailListByRead(const void * Item1, const void * Item2)
{
	MailObj* pObj1 = *(MailObj**)Item1 ;
	MailObj* pObj2 = *(MailObj**)Item2 ;

	return (pObj2->byState == STATE_MAIL_SEND?100:0)-(pObj1->byState == STATE_MAIL_SEND?100:0);
}

static void CBFrameMailList(int nPos,char* szName)
{
	if(strncmp (szName, "new",3) == 0||strncmp (szName, "yidu",4) == 0)
	{
		pMainMenu->m_pMailMenu->SelectReadMail(nPos);
	}
}
static void	InputResult(int nResult,void * param){
	if(nResult){
		ItemObj* pObj = (ItemObj*)param;
		pMainMenu->m_pMailMenu->AddItemToMail(pObj,nResult);
	}
};
void	InfoCallBackEvent(void* pParam,BYTE byType){
	ItemObj* pObj = (ItemObj*)pParam;
	if (pObj)
	{
		if (pObj->pItemBase->byType == ITEMTYPE_EQUIP||pObj->GetVal(ITEM_AD_NUM) == 1)
		{
			pMainMenu->m_pMailMenu->AddItemToMail(pObj,1);
		}else{
			pMainMenu->m_pGetNumMenu->Open(InputResult,pObj->GetVal(ITEM_AD_NUM),pObj->GetVal(ITEM_AD_NUM),pObj);
		}
	}
}
static void CBFrameFBEvent(int nPos,char* szName)
{
	if(strncmp (szName, "bag",3) == 0)
	{
		ItemObj* pItem = (ItemObj*)pMainMenu->m_pMailMenu->m_pFormMail[MAILMENU_WRITEMAIL]->pCFrame->GetFPComp(nPos,szName)->GetTag();

		if (pItem)
		{
			// 			pChar->m_pItemCenter->GetItemInfoFormServer(pItem->GetVal(ITEM_AD_UIN),pItem->GetVal(ITEM_AD_ID),callBacktest);
			// 			return;
			if (pItem->pItemBase->byType == ITEMTYPE_EQUIP)
			{
				pMainMenu->m_pEquipInfoMenu->Open(pItem);
				pMainMenu->m_pEquipInfoMenu->SetCallBackEvent(InfoCallBackEvent);
				pMainMenu->m_pEquipInfoMenu->SetButtonSpr("BT_YH_XUANZHONG.png");
			}else{
				pMainMenu->m_pItemInfoMenu->Open(pItem);
				pMainMenu->m_pItemInfoMenu->SetCallBackEvent(InfoCallBackEvent);
				pMainMenu->m_pItemInfoMenu->SetButtonSpr("BT_YH_XUANZHONG.png");
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
	pMainMenu->m_pMailMenu->SetSelectPage(nPos);
// 	if (nPos!=pMainMenu->m_pBagMenu->m_nSelectPage)
// 	{
// 		pMainMenu->m_pBagMenu->m_nSelectPage = nPos;
// 		if (nPos>=1)
// 		{
// 			pMainMenu->m_pBagMenu->UpdatePage(nPos-1,false);
// 		}
// 		//pMainMenu->m_pBagMenu->UpdatePage(nPos,true);
// 		if (nPos<nBagPageMax-1)
// 		{
// 			pMainMenu->m_pBagMenu->UpdatePage(nPos+1,false);
// 		}
// 	}
// 	pMainMenu->m_pBagMenu->UpdatePage(nPos,true);
}
static bool GetItemCanMail(ItemObj* pObj,void* param){
	if (pObj->GetVal(ITEM_AD_SLOT) !=ITEM_SLOT_BAG )
	{
		return false;
	}
	if (pObj->GetVal(ITEM_AD_BIND))
	{
		return false;
	}
	return true;
}
static void SwitchMailMenu(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nType = (int)param;
	pMainMenu->m_pMailMenu->m_nSelectMenu = nType;
	for (int i=0;i<MAINMENU_MAX;i++)
	{
		if (nType == i)
		{
			pMainMenu->m_pMailMenu->m_pFormMail[i]->Open();
		}else{
			pMainMenu->m_pMailMenu->m_pFormMail[i]->Close();
		}
		
	}
	if (nType == MAILMENU_WRITEMAIL)
	{
		pMainMenu->m_pMailMenu->ResetWriteMenu();
	}
	pMainMenu->m_pMailMenu->m_bUpdate = true;
	pMainMenu->m_pMailMenu->m_bWriteUpdate = true;
}
static void SwitchPageMenu(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
 	int nType = (int)param;
	pMainMenu->m_pMailMenu->m_pFormMail[pMainMenu->m_pMailMenu->m_nSelectMenu]->Close();
	pMainMenu->m_pBagMainCtrl->Open(0,nType);
}
static void RemoveItemEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pMailMenu->RemoveItemFromMail((ItemObj*)pEvent->pComponent->GetTag());
}

void cb_SendMailMsg(void* pParam, int nVal)
{
	if (nVal)
	{
		BuildPacketEx(pPacket, USER,C_SEND_MAIL_INFO , buf, 1024);
		SET_DATA(pSend, USER, C_SEND_MAIL_INFO, pPacket);
		char* szTitle = pMainMenu->m_pMailMenu->m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("name")->GetEdit();
		if (strlen(szTitle)<=0&&strlen(szTitle)>=19)
		{
			pMainMenu->PopTipMsg("输入的名字太长或者太短");
			return;
		}
		strncpy(pSend->szTitle , szTitle, 32);
		char* szBody = pMainMenu->m_pMailMenu->m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("neirong")->GetEdit();
		if (strlen(szBody)>=255)
		{
			pMainMenu->PopTipMsg("输入的内容太长");
			return;
		}

		strcpy(pSend->szBody , szBody);
		pSend->nItemIndex[0] = pMainMenu->m_pMailMenu->m_nIndex[0];
		pSend->nItemIndex[1] = pMainMenu->m_pMailMenu->m_nIndex[1];
		pSend->nCnt[0] = pMainMenu->m_pMailMenu->m_nItemCnt[0];
		pSend->nCnt[1] = pMainMenu->m_pMailMenu->m_nItemCnt[1];
		pSend->nMoney = atoi(pMainMenu->m_pMailMenu->m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("jinbi2")->szCaption);
		pChar->SendPacket(pPacket);

		pMainMenu->m_pMailMenu->ResetWriteMenu();
	}
}

static void SendMailEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	char szTemp[256];
	sprintf(szTemp, "是否消耗%d金币发送邮件？", pChar->m_byLvl * 100);
	pMainMenu->PopAskMsgBox(szTemp, cb_SendMailMsg, (void*)0, 1);
}

static void ItemInfoCallback(ItemObj* pObj){
	if (pObj)
	{
		if (pObj->pItemBase->byType == ITEMTYPE_EQUIP)
		{
			pMainMenu->m_pEquipInfoMenu->Open(pObj);
			pMainMenu->m_pEquipInfoMenu->SetButtonSpr(NULL,NULL);
			pMainMenu->m_pEquipInfoMenu->SetCallBackEvent(NULL);
		}else{
			pMainMenu->m_pItemInfoMenu->Open(pObj);
			pMainMenu->m_pItemInfoMenu->SetButtonSpr(NULL,NULL);
			pMainMenu->m_pItemInfoMenu->SetCallBackEvent(NULL);
		}
	}
}
static void GetItemInfo(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nIndex = (int)pEvent->pComponent->GetTag();
	pChar->m_pItemCenter->GetItemInfoFormServer(0,nIndex,ItemInfoCallback);
}
static void GetItemFromMail(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	MailObj* pObj = (MailObj*)pMainMenu->m_pMailMenu->m_pFormMail[MAILMENU_GETMAIL]->pCFrame->GetFPTag(pMainMenu->m_pMailMenu->m_nSelectMail);
	if (pObj)
	{
		pMainMenu->PopTipMsg("领取成功！已放入背包！");
		BuildPacketEx(pPacket, USER,C_GETMAIL_ITEM , buf, 1024);
		SET_DATA(pSend, USER, C_GETMAIL_ITEM, pPacket);
		pSend->dwMailID = pObj->dwMailID;
		pChar->SendPacket(pPacket);
	}

}
static void ReturnMail(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	MailObj* pObj = (MailObj*)pMainMenu->m_pMailMenu->m_pFormMail[MAILMENU_GETMAIL]->pCFrame->GetFPTag(pMainMenu->m_pMailMenu->m_nSelectMail);
	if (pObj)
	{
		pMainMenu->m_pMailMenu->m_pFormMail[MAILMENU_GETMAIL]->Close();
		pMainMenu->m_pMailMenu->m_pFormMail[MAILMENU_WRITEMAIL]->Open();
		pMainMenu->m_pMailMenu->ResetWriteMenu();
		pMainMenu->m_pMailMenu->m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("name")->SetEdit(pObj->szTitle);
	}

}
void CBAskDeleteMail(void * pParam,int nVal)
{
	if (nVal)
	{
		MailObj* pInfo = (MailObj*)pParam;
		if (!pInfo)
		{
			return;
		}
		BuildPacketEx(pPacket, USER,C_DELETE_MAIL , buf, 1024);
		SET_DATA(pSend, USER, C_DELETE_MAIL, pPacket);
		pSend->dwMailID = pInfo->dwMailID;
		pChar->SendPacket(pPacket);
	}
}
static void DeleteMailEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	MailObj* pObj = (MailObj*)pMainMenu->m_pMailMenu->m_pFormMail[MAILMENU_GETMAIL]->pCFrame->GetFPTag(pMainMenu->m_pMailMenu->m_nSelectMail);
	if (pObj)
	{
		bool bItem = false;
		for (int i=0;i<nMaxMailItemCount;i++)
		{
			if(pObj->nItemID[i]) bItem = true;
		}
		if(pObj->nMoney) bItem = true;
		if (bItem)
		{
			pMainMenu->PopAskMsgBox("该邮件还有未领取物品,是否删除?",CBAskDeleteMail,pObj);
			return;
		}
		BuildPacketEx(pPacket, USER,C_DELETE_MAIL , buf, 1024);
		SET_DATA(pSend, USER, C_DELETE_MAIL, pPacket);
		pSend->dwMailID = pObj->dwMailID;
		pChar->SendPacket(pPacket);
	}

}

void CBAskDeleteReadedMail(void * pParam,int nVal)
{
	if (nVal)
	{
		if (!pMainMenu->m_pMailMenu->m_pDeleteList->Count)
			return;
		BuildPacketEx(pPacket, USER,C_DELETE_MAIL_ALL , buf, 1024);
		SET_DATA(pSend, USER, C_DELETE_MAIL_ALL, pPacket);
		for (int j = 0; j < pMainMenu->m_pMailMenu->m_pDeleteList->Count; j++)
		{
			pSend->dwMailID[j] = (DWORD)pMainMenu->m_pMailMenu->m_pDeleteList->Items[j];
		}
		pSend->wCnt = pMainMenu->m_pMailMenu->m_pDeleteList->Count;
		pPacket->wSize += (pMainMenu->m_pMailMenu->m_pDeleteList->Count-1)*sizeof(DWORD);
		pChar->SendPacket(pPacket);
	}
}

static void DeleteNullMailEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	while(pMainMenu->m_pMailMenu->m_pDeleteList->Count)
		pMainMenu->m_pMailMenu->m_pDeleteList->Delete(0);

	bool bifNull = false;
	for (int j = 0; j < pChar->m_pMailList.size(); j++)
	{
		MailObj* pObj = (MailObj*)pMainMenu->m_pMailMenu->m_pFormMail[MAILMENU_GETMAIL]->pCFrame->GetFPTag(j);
		if (pObj)
		{
			bool bItem = false;
			for (int n=0;n<nMaxMailItemCount;n++)
			{
				if(pObj->nItemID[n]) bItem = true;
			}
			if(pObj->nMoney) bItem = true;
			if (pObj->byState == STATE_MAIL_READ||pObj->byState == STATE_MAIL_GOT)
			{
				pMainMenu->m_pMailMenu->m_pDeleteList->Add((void*)pObj->dwMailID);
				if (bItem)
					bifNull = true;
			}
		}
	}
	if(pMainMenu->m_pMailMenu->m_pDeleteList->Count&&bifNull)
	{
		pMainMenu->PopAskMsgBox("已读邮件内还有未领取物品，是否删除?",CBAskDeleteReadedMail,(void*)0);
		return;
	}
	if(pMainMenu->m_pMailMenu->m_pDeleteList->Count&&!bifNull)
	{
		pMainMenu->PopAskMsgBox("是否删除全部已读邮件?",CBAskDeleteReadedMail,(void*)0);
		return;
	}
}
static void ChooseFriendEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pFriendChooseMenu->Open();
}
static void	InputResult2(int nResult,void * param){
	if(nResult){
		pMainMenu->m_pMailMenu->m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("jinbi2")->SetCaptionEx("%d",0,nResult);
	}
};
static void AddMoneyEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	if (!pChar->m_dwMoney)
	{
		pMainMenu->PopTipMsg("没有金币");
		return;
	}
	pMainMenu->m_pGetNumMenu->Open(InputResult2,pChar->m_dwMoney,0);

}
MailMenu::MailMenu():m_nSelectMenu(MAILMENU_GETMAIL),m_bUpdate(true),m_bWriteUpdate(true),m_nSelectPage(0),m_nSelectMail(-1),m_nSelectMailIndex(0)
{
	m_pFormMail[MAILMENU_GETMAIL] = pMainInterface->GetDForm ("youjian1");
	m_pForm = pMainInterface->GetDForm ("youjian1");
	m_pFormMail[MAILMENU_GETMAIL]->pCFrame = new CCtrlFrame(38,178,380,380);
	DComponent*	pComp	= m_pFormMail[MAILMENU_GETMAIL]->GetComponent ("n");
	pComp->SetVisible (false);
	DComponent*	ppComps[3];
	ppComps[0]		= m_pFormMail[MAILMENU_GETMAIL]->GetComponent ("yidu");
	ppComps[1]		= m_pFormMail[MAILMENU_GETMAIL]->GetComponent ("new");
	ppComps[2]		= m_pFormMail[MAILMENU_GETMAIL]->GetComponent ("laizi");

	m_pFormMail[MAILMENU_GETMAIL]->pCFrame->nSpace=11;
	m_pFormMail[MAILMENU_GETMAIL]->pCFrame->nDisStep=65;
	m_pFormMail[MAILMENU_GETMAIL]->pCFrame->InitFParts (pComp,ppComps,3);
	m_pFormMail[MAILMENU_GETMAIL]->pCFrame->SetEvent(CBFrameMailList);

	m_pFormMail[MAILMENU_GETMAIL]->GetComponent("xx")->SetEvent(SwitchMailMenu,(void*)MAILMENU_WRITEMAIL);

	m_pFormMail[MAILMENU_GETMAIL]->GetComponent("icon1")->SetEvent(GetItemInfo);
	m_pFormMail[MAILMENU_GETMAIL]->GetComponent("icon2")->SetEvent(GetItemInfo);

	m_pFormMail[MAILMENU_GETMAIL]->GetComponent("lq")->SetEvent(GetItemFromMail);
	m_pFormMail[MAILMENU_GETMAIL]->GetComponent("hf")->SetEvent(ReturnMail);
	m_pFormMail[MAILMENU_GETMAIL]->GetComponent("sc")->SetEvent(DeleteMailEvent);
	m_pFormMail[MAILMENU_GETMAIL]->GetComponent("quan")->SetEvent(DeleteNullMailEvent);

	char sztxt[32];

	for (int i=0;i<3;i++)
	{
		sprintf(sztxt,"page%d",i+1);
		if(i!=2) m_pFormMail[MAILMENU_GETMAIL]->GetComponent(sztxt)->SetSprColor(127,127,127);
		m_pFormMail[MAILMENU_GETMAIL]->GetComponent(sztxt)->SetEvent(SwitchPageMenu,(void*)i);
	}

	m_pMailTmpList = xnList::Create();

	m_pFormMail[MAILMENU_WRITEMAIL] = pMainInterface->GetDForm ("youjian2");
	DComponent* pComItem[20];
	for (int i=0;i<20;i++)
	{
		sprintf(sztxt,"bag%d",i+1);
		// 		m_pBagItem[i] = m_pForm->GetComponent(sztxt);
		// 		m_pBagItem[i]->SetEvent(UseItemEvent);
		pComItem[i] = m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent(sztxt);
		pComItem[i]->SetVisible(false);
	}

	m_pFormMail[MAILMENU_WRITEMAIL]->pCFrame	= new CCtrlFrame(445,145,485,340);
	pComp	= m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent ("di4");

	m_pFormMail[MAILMENU_WRITEMAIL]->pCFrame->InitFParts(pComp,pComItem,20);
	m_pFormMail[MAILMENU_WRITEMAIL]->pCFrame->SetEvent (CBFrameFBEvent);
	m_pFormMail[MAILMENU_WRITEMAIL]->pCFrame->SetStepEvent(CBFrameFBStepEvent);
	m_pFormMail[MAILMENU_WRITEMAIL]->pCFrame->nSpace	= 55;
	m_pFormMail[MAILMENU_WRITEMAIL]->pCFrame->nDisStep	= 510;

	m_pFormMail[MAILMENU_WRITEMAIL]->pCFrame->SetFPCnt(nBagPageMax,true);
	m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("sx")->SetEvent(SwitchMailMenu,(void*)MAILMENU_GETMAIL);

	m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("icon1")->SetEvent(RemoveItemEvent);
	m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("icon2")->SetEvent(RemoveItemEvent);

	m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("fs")->SetEvent(SendMailEvent);
	m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("hy")->SetEvent(ChooseFriendEvent);

	for (int i=0;i<3;i++)
	{
		sprintf(sztxt,"page%d",i+1);
		if(i!=2) m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent(sztxt)->SetSprColor(127,127,127);
		m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent(sztxt)->SetEvent(SwitchPageMenu,(void*)i);
	}

	m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("jinbi2")->SetEvent(AddMoneyEvent);

	m_pDeleteList = xnList::Create();
}

void MailMenu::RefreshMailTmpList()
{
	while(m_pMailTmpList->Count)
		m_pMailTmpList->Delete(0);

	for (std::map<int,MailObj>::iterator itor = pChar->m_pMailList.begin();itor != pChar->m_pMailList.end();itor++)
	{
		m_pMailTmpList->Add((void*)&itor->second);
	}

	m_pMailTmpList->Sort(ReSortMailListByRead);

	m_nSelectMail = 0;
}

void MailMenu::Open( int nT )
{
	if (m_nSelectMenu == MAILMENU_GETMAIL)
	{
		m_bUpdate = true;
	}else{
		ResetWriteMenu();
		m_bWriteUpdate = true;
	}
	while(m_pDeleteList->Count)
		m_pDeleteList->Delete(0);
	m_pFormMail[m_nSelectMenu]->Open(nT);

	RefreshMailTmpList();
}

void MailMenu::Update()
{
	if (m_pFormMail[MAILMENU_GETMAIL]->bOpened)
	{
		if (m_bUpdate)
		{
			m_bUpdate = false;
			m_pFormMail[MAILMENU_GETMAIL]->pCFrame->SetFPCnt(pChar->m_pMailList.size());
			int i = 0;
			if (pChar->m_pMailList.size()&&m_nSelectMail<0)
			{
				m_nSelectMail = 0;
			}else if (pChar->m_pMailList.size()&&m_nSelectMail >= pChar->m_pMailList.size())
			{
				m_nSelectMail--;
			}
			bool bUpdateMail = false;
			for (int j = 0; j < m_pMailTmpList->Count; j++)
			{
				MailObj* pMailObj = (MailObj*)m_pMailTmpList->Items[j];
				if (pMailObj)
				{
					m_pFormMail[MAILMENU_GETMAIL]->pCFrame->SetFPTag(i,pMailObj);
					m_pFormMail[MAILMENU_GETMAIL]->pCFrame->GetFPComp(i,"laizi")->SetCaptionEx("来自%s的信件",0,pMailObj->szTitle);
					m_pFormMail[MAILMENU_GETMAIL]->pCFrame->GetFPComp(i,"new")->SetViewSpr(pMailObj->byState == STATE_MAIL_SEND?"MB_YJ_NWE.png":"MB_YJ_YIDU.png");
					if (i == m_nSelectMail&& pMailObj->dwMailID != m_nSelectMailIndex)
					{
						bUpdateMail = true;
					}
					i++;
				}
			}

			if (!pChar->m_pMailList.size())
				ClearReadMail();
			
			if (bUpdateMail)
			{
				SelectReadMail(m_nSelectMail);
			}
		}
	}
	if (m_pFormMail[MAILMENU_WRITEMAIL]->bOpened)
	{
		if (m_bWriteUpdate)
		{
			m_bWriteUpdate = false;
			char sztxt[32];
			char szSpr[32];
			sprintf(sztxt,"BT_SHAIZI.png");
			for (int i=0;i<nBagPageMax*20;i++)
			{
				sprintf(szSpr,"bag%d",i%20+1);
				if (i>=pChar->m_pItemCenter->m_nBagSize)
				{
					SetItemToDCom(m_pFormMail[MAILMENU_WRITEMAIL]->pCFrame->GetFPComp(i/20,szSpr),NULL);
					m_pFormMail[MAILMENU_WRITEMAIL]->pCFrame->GetFPComp(i/20,szSpr)->SetViewSpr("MB_SUO.png");
				}else{
					SetItemToDCom(m_pFormMail[MAILMENU_WRITEMAIL]->pCFrame->GetFPComp(i/20,szSpr),NULL);
				}
			}
			xnList* pList = xnList::Create();
			pChar->m_pItemCenter->GetItemInBagByFilter(pList,GetItemCanMail);
			for (int i= 0;i<pList->Count;i++)
			{
				ItemObj* pItem = (ItemObj*)pList->Items[i];
				int nPos = i;/*pItem->GetVal(ITEM_AD_POS);*/
				bool bEnable = (pItem->GetVal(ITEM_AD_ID) != m_nIndex[0]) && (pItem->GetVal(ITEM_AD_ID) != m_nIndex[1]);
				sprintf(szSpr,"bag%d",nPos%20+1);
				SetItemToDCom(m_pFormMail[MAILMENU_WRITEMAIL]->pCFrame->GetFPComp(nPos/20,szSpr),pItem);
				m_pFormMail[MAILMENU_WRITEMAIL]->pCFrame->GetFPComp(nPos/20,szSpr)->SetEnable(bEnable);
				
			}
			pList->Free();
			SetSelectPage(m_nSelectPage);
			
		}
		m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("jinbi")->SetCaptionEx("%d",1,pChar->m_dwMoney);
		m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("zuan")->SetCaptionEx("%d",1,pChar->m_dwRmb);
		m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("money")->SetCaptionEx("%d",1,pChar->m_dwBindRmb);

	}
}

MailMenu::~MailMenu()
{
	while(m_pDeleteList->Count)
		m_pDeleteList->Delete(0);
	m_pDeleteList->Free();

	while(m_pMailTmpList->Count)
		m_pMailTmpList->Delete(0);
	m_pMailTmpList->Free();
}

void MailMenu::ClearReadMail()
{
	m_pFormMail[MAILMENU_GETMAIL]->GetComponent("neirong")->SetCaptionEx("",0);
	m_pFormMail[MAILMENU_GETMAIL]->GetComponent("jinbi")->SetCaptionEx("",0);
	m_pFormMail[MAILMENU_GETMAIL]->GetComponent("shenyu")->SetCaptionEx("",0);
	m_pFormMail[MAILMENU_GETMAIL]->GetComponent("lq")->SetEnable(false);

	char sztxt[32];
	for (int i=0;i<2;i++)
	{
		sprintf(sztxt,"icon%d",i+1);
		m_pFormMail[MAILMENU_GETMAIL]->GetComponent(sztxt)->SetViewSpr("");
		m_pFormMail[MAILMENU_GETMAIL]->GetComponent(sztxt)->SetCaptionEx(" ",0);
		m_pFormMail[MAILMENU_GETMAIL]->GetComponent(sztxt)->SetTag(NULL);
	}
	m_pFormMail[MAILMENU_GETMAIL]->GetComponent("hf")->SetVisible(false);

	while(m_pMailTmpList->Count)
		m_pMailTmpList->Delete(0);
}

void MailMenu::SelectReadMail( int nPos )
{
	char sztxt[32];
	if (nPos == -1)
	{
		m_pFormMail[MAILMENU_GETMAIL]->GetComponent("neirong")->SetCaptionEx("",0);
		m_pFormMail[MAILMENU_GETMAIL]->GetComponent("jinbi")->SetCaptionEx("",0);
		m_pFormMail[MAILMENU_GETMAIL]->GetComponent("shenyu")->SetCaptionEx("",0);
		m_pFormMail[MAILMENU_GETMAIL]->GetComponent("lq")->SetEnable(false);
		for (int i=0;i<2;i++)
		{
			sprintf(sztxt,"icon%d",i+1);
			m_pFormMail[MAILMENU_GETMAIL]->GetComponent(sztxt)->SetViewSpr("");
			m_pFormMail[MAILMENU_GETMAIL]->GetComponent(sztxt)->SetCaptionEx(" ",0);
			m_pFormMail[MAILMENU_GETMAIL]->GetComponent(sztxt)->SetTag(NULL);
		}
		m_pFormMail[MAILMENU_GETMAIL]->GetComponent("hf")->SetVisible(false);
		return;
	}
	MailObj* pObj = (MailObj*)m_pFormMail[MAILMENU_GETMAIL]->pCFrame->GetFPTag(nPos);
	if (!pObj)
	{
		return;
	}
	m_nSelectMailIndex = pObj->dwMailID;
	if (pObj->byState == STATE_MAIL_SEND)
	{
		pObj->byState = STATE_MAIL_READ;
		//m_pFormMail[MAILMENU_GETMAIL]->pCFrame->GetFPComp(nPos,"new")->SetVisible(false);
		BuildPacketEx(pPacket, USER,C_READMAIL , buf, 1024);
		SET_DATA(pSend, USER, C_READMAIL, pPacket);
		pSend->dwMailID = pObj->dwMailID;
		pChar->SendPacket(pPacket);
		pMainMenu->m_pCtrlMenu->m_pCompMail->SetVisible(pChar->IsNewMail());
		m_bUpdate = true;
	}
	m_pFormMail[MAILMENU_GETMAIL]->GetComponent("neirong")->SetCaptionEx("%s",0,pObj->szBody);
	m_pFormMail[MAILMENU_GETMAIL]->GetComponent("jinbi")->SetCaptionEx("%d",1,pObj->nMoney);

	DWORD nTime = 0;
	if (pObj->dwReturnTick>pChar->GetServerTime())
	{
		nTime = pObj->dwReturnTick - pChar->GetServerTime();
	}
	if (nTime>=86400)
	{
		sprintf(sztxt,"剩余 %d天",nTime/86400);
	}else if (nTime>=3600)
	{
		sprintf(sztxt,"剩余 %d小时",nTime/3600);
	}else{
		sprintf(sztxt,"剩余 小于1小时");
	}
	m_pFormMail[MAILMENU_GETMAIL]->GetComponent("shenyu")->SetCaptionEx("%s",0,sztxt);
	bool bItem = false;
	for (int i=0;i<nMaxMailItemCount;i++)
	{
		if(pObj->nItemID[i]) bItem = true;
	}
	if(pObj->nMoney) bItem = true;
	m_pFormMail[MAILMENU_GETMAIL]->GetComponent("lq")->SetEnable(pObj->byState != STATE_MAIL_GOT&&bItem);
	char szSpr[32];
	for (int i=0;i<2;i++)
	{
		sprintf(sztxt,"icon%d",i+1);
		ItemExBase* pBase  = ItemConfig::GetInstance().GetItemBase(pObj->nItemID[i]);
		if (pBase&&pObj->byState != STATE_MAIL_GOT)
		{
			sprintf(szSpr,"item/%d.png",pBase->nSpr);
			m_pFormMail[MAILMENU_GETMAIL]->GetComponent(sztxt)->SetCaptionEx("%d",2,pObj->nCnt[i]);
			m_pFormMail[MAILMENU_GETMAIL]->GetComponent(sztxt)->SetViewSpr(szSpr);
			m_pFormMail[MAILMENU_GETMAIL]->GetComponent(sztxt)->SetTag((void*)pObj->nIndex[i]);
		}else{
			m_pFormMail[MAILMENU_GETMAIL]->GetComponent(sztxt)->SetViewSpr("");
			m_pFormMail[MAILMENU_GETMAIL]->GetComponent(sztxt)->SetCaptionEx(" ",0);
			m_pFormMail[MAILMENU_GETMAIL]->GetComponent(sztxt)->SetTag(NULL);
		}
	}
	pMainMenu->m_pMailMenu->m_pFormMail[MAILMENU_GETMAIL]->GetComponent("hf")->SetVisible(pObj->uinSender);
	m_nSelectMail = nPos;
}

void MailMenu::ResetWriteMenu()
{
	m_nIndex[0] = 0;
	m_nIndex[1] = 0;
	m_nItemCnt[0] = 0;
	m_nItemCnt[1] = 0;
	m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("icon1")->SetViewSpr(" ");
	m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("icon1")->ClearEffect();
	m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("icon1")->SetCaptionEx(" ",0);
	m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("icon1")->SetTag(NULL);
	m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("icon2")->SetViewSpr(" ");
	m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("icon2")->SetCaptionEx(" ",0);
	m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("icon2")->SetTag(NULL);
	m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("icon2")->ClearEffect();
	m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("neirong")->SetEdit("");
	m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("name")->SetEdit("");
	m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("jinbi2")->SetCaptionEx("点击添加金币",0);
}

void MailMenu::SetSelectPage( int nSelect )
{
	m_nSelectPage = nSelect;
	char sztxt[32];
	for (int i=0;i<nBagPageMax;i++)
	{
		sprintf(sztxt,"p%d",i+1);
		m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent(sztxt)->SetViewSpr( i== nSelect?(char*)"MB_DIAN_B.png":(char*)"MB_DIAN_S.png");
	}
}

void MailMenu::AddItemToMail( ItemObj* pObj ,int nCount)
{
	if (!pObj)
	{
		return;
	}
	if (!m_nIndex[0])
	{
		m_nIndex[0] = pObj->GetVal(ITEM_AD_ID);
		SetItemToDCom(m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("icon1"),pObj);
		if (nCount == 1)
		{
			m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("icon1")->SetCaptionEx(" ",2);
		}else{
			m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("icon1")->SetCaptionEx("%d",0,nCount);
		}
		m_nItemCnt[0] = nCount;
	}else if (!m_nIndex[1])
	{
		m_nIndex[1] = pObj->GetVal(ITEM_AD_ID);
		SetItemToDCom(m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("icon2"),pObj);
		if (nCount == 1)
		{
			m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("icon2")->SetCaptionEx(" ",2);
		}else{
			m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("icon2")->SetCaptionEx("%d",2,nCount);
		}
		m_nItemCnt[1] = nCount;
	}
	m_bWriteUpdate = true;
}

void MailMenu::RemoveItemFromMail( ItemObj* pObj )
{
	if (!pObj)
	{
		return;
	}
	if (m_nIndex[0] == pObj->GetVal(ITEM_AD_ID))
	{
		m_nIndex[0] = 0;
		SetItemToDCom(m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("icon1"),NULL);
	}else if (m_nIndex[1] == pObj->GetVal(ITEM_AD_ID))
	{
		m_nIndex[1] = 0;
		SetItemToDCom(m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("icon2"),NULL);
	}
	m_bWriteUpdate = true;

}

bool MailMenu::CheckCanOpen()
{
	if(pChar->m_byLvl < 20)
	{
		pMainMenu->PopTipMsg("等级达到20级开启邮件功能");
		return false;
	}
	return true;
}
