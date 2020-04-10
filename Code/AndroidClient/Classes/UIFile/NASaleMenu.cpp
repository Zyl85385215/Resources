#include "MainMenu.h"
#include "UserData.h"
#include "NASaleMenu.h"
#include "ios_nasale_packet.h"

static int	nItemClr[4][3] = {
	{255,255,255},
	{0,255,0},
	{0,133,207},
	{188,52,186},
};
static char* NASaleFilter1[] = {"不限","药品","装备","宠物技能书","怒气技能书","坐骑","强化石","其他"};
static char* NASaleFilter2[] = {"不限","战士","法师","枪手","牧师"};
static char* NASaleFilter3[] = {"不限","0-9","10-19","20-29","30-39","40-49","50-59","60-69","70-79","80-89","90-99"};

bool fun_cbNAItemCheck(ItemObj* pObj,void* param)
{
	if(pObj->GetVal(ITEM_AD_BIND)||pObj->GetVal(ITEM_AD_SLOT)!=ITEM_SLOT_BAG)
	{
		return false;
	}
	return true;
}


void fun_cbInputSellPrice(int nResult,void * param)
{
	ItemObj* pItem	= (ItemObj*)param;

	//pMainMenu->m_pNaSaleMenu->RequestSellItem(pItem,nResult);

	pMainMenu->ShowNetWaiting();
}

void fun_cbNAItemSel(void* pParam,BYTE byType)
{
	

	//pMainMenu->m_pGetNumMenu->Open(fun_cbInputSellPrice,99999999,0,pParam);
	pMainMenu->m_pNASalePriceMenu->OpenForm((ItemObj*)pParam);
	pMainMenu->m_pChatMenu->m_pItemMenu->Close();
	
}

void RQOpenMyEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	NASaleMenu*	pMenu	= (NASaleMenu*)param;
	pMenu->m_pForm->Close();
	if(pChar->m_byLvl < 20)
	{
		pMainMenu->PopTipMsg("等级达到20级开启寄售功能");
		return;
	}
	pMenu->OpenMySale();
}

void RQBackSaleEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	NASaleMenu*	pMenu	= (NASaleMenu*)param;
	pMenu->m_pMySaleForm->Close();
	pMenu->OpenForm();
}

void RQCalcSaleEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	NASaleMenu*	pMenu	= (NASaleMenu*)param;
	if(pMenu->m_nMySaleMoney)
	{
		pMenu->SendCmd(NASALE,NA_CALC_MYMONEY);
		pMenu->SendCmd(NASALE,NA_MY_LIST);
	}
}


void RQSellEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	NASaleMenu*	pMenu	= (NASaleMenu*)param;

	pMainMenu->m_pChatMenu->m_pItemMenu->SetType(FORM_EX);
	pMainMenu->m_pChatMenu->m_pItemMenu->SetFilter(fun_cbNAItemCheck);
	pMainMenu->m_pChatMenu->m_pItemMenu->m_bReSort = true;
	pMainMenu->m_pChatMenu->m_pItemMenu->SetLockEquip(true);
	pMainMenu->m_pChatMenu->m_pItemMenu->SetChooseCallback(fun_cbNAItemSel);
	pMainMenu->m_pChatMenu->m_pItemMenu->SetButtonSpr("BT_JISHOU1.png");
	pMainMenu->m_pChatMenu->m_pItemMenu->Open();
}

void fun_cbBuyItem(void* pParam,BYTE byType)
{
	pMainMenu->m_pNaSaleMenu->RequestBuyItem();
}

void fun_cbGetMyItem(void* pParam,BYTE byType)
{
	pMainMenu->m_pNaSaleMenu->RequestGetMyItem();
}

void fun_cbExInfo(ItemObj* pObj)
{
	if (pObj)
	{
		if (pObj->pItemBase->byType == ITEMTYPE_EQUIP)
		{
			pMainMenu->m_pEquipInfoMenu->Open(pObj,FORM_EX);
			pMainMenu->m_pEquipInfoMenu->SetButtonSpr("BT_GM_CZ.png",NULL);
			pMainMenu->m_pEquipInfoMenu->SetCallBackEvent(fun_cbBuyItem);
		}else{
			pMainMenu->m_pItemInfoMenu->Open(pObj,FORM_EX);
			pMainMenu->m_pItemInfoMenu->SetButtonSpr("BT_GM_CZ.png",NULL);
			pMainMenu->m_pItemInfoMenu->SetCallBackEvent(fun_cbBuyItem);
		}
	}
}

void fun_cbMyExInfo(ItemObj* pObj)
{
	if (pObj)
	{
		if (pObj->pItemBase->byType == ITEMTYPE_EQUIP)
		{
			pMainMenu->m_pEquipInfoMenu->Open(pObj,FORM_EX);
			pMainMenu->m_pEquipInfoMenu->SetButtonSpr("BT_JS_QH.png",NULL);
			pMainMenu->m_pEquipInfoMenu->SetCallBackEvent(fun_cbGetMyItem);
		}else{
			pMainMenu->m_pItemInfoMenu->Open(pObj,FORM_EX);
			pMainMenu->m_pItemInfoMenu->SetButtonSpr("BT_JS_QH.png",NULL);
			pMainMenu->m_pItemInfoMenu->SetCallBackEvent(fun_cbGetMyItem);
		}
	}
}

void NASaleClickFrm(int nPos,char* szName,void* param)
{
	NASaleMenu* pMenu	= (NASaleMenu*)param;
	pMenu->m_nCurSelPos = nPos;
	for (int i = 0; i < pMenu->m_pListSale->Count; i++)
	{
		if (i == nPos)
			pMenu->m_pForm->pCFrame->GetFPComp(i,"mb")->SetVisible(true);
		else
			pMenu->m_pForm->pCFrame->GetFPComp(i,"mb")->SetVisible(false);
	}
	if(strncmp(szName,"icon1",5) == 0)
	{
		NASaleClient* pSale	= (NASaleClient*)pMenu->m_pForm->pCFrame->GetFPTag(nPos);
		pMenu->m_pSelSale	= pSale;
		if(pSale->dwItemInfo)
		{
			pChar->m_pItemCenter->GetItemInfoFormServer(0,pSale->dwItemInfo,fun_cbExInfo);
			pMainMenu->ShowNetWaiting(500);
		}
		else
		{
			ItemExBase*	pBase	= ItemConfig::GetInstance().GetItemBase(pSale->wItemID);
			if(pBase)
			{
				pMainMenu->m_pItemInfoMenu->Open(pBase,FORM_EX);
				pMainMenu->m_pItemInfoMenu->SetButtonSpr("BT_GM_CZ.png",NULL);
				pMainMenu->m_pItemInfoMenu->SetCallBackEvent(fun_cbBuyItem);
			}
		}
	}
	else
	{

	}
}

void NAMyClickFrm(int nPos,char* szName,void* param)
{
	NASaleMenu* pMenu	= (NASaleMenu*)param;
	pMenu->m_nCurSelPos = nPos;
	for (int i = 0; i < pMenu->m_pListSale->Count; i++)
	{
		if (i == nPos)
			pMenu->m_pMySaleForm->pCFrame->GetFPComp(i,"mb")->SetVisible(true);
		else
			pMenu->m_pMySaleForm->pCFrame->GetFPComp(i,"mb")->SetVisible(false);
	}
	if(strncmp(szName,"icon1",5) == 0)
	{
		NASaleClient* pSale	= (NASaleClient*)pMenu->m_pMySaleForm->pCFrame->GetFPTag(nPos);
		pMenu->m_pSelSale	= pSale;
		if(pSale->dwItemInfo)
		{
			pChar->m_pItemCenter->GetItemInfoFormServer(0,pSale->dwItemInfo,fun_cbMyExInfo);
			pMainMenu->ShowNetWaiting(500);
		}
		else
		{
			ItemExBase*	pBase	= ItemConfig::GetInstance().GetItemBase(pSale->wItemID);
			if(pBase)
			{
				pMainMenu->m_pItemInfoMenu->Open(pBase,FORM_EX);
				pMainMenu->m_pItemInfoMenu->SetButtonSpr("BT_JS_QH.png",NULL);
				pMainMenu->m_pItemInfoMenu->SetCallBackEvent(fun_cbGetMyItem);
			}
		}
	}
	else
	{

	}
}

void NASaleFilterEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nType = (int)param;

	if (pMainMenu->m_pNASaleFilterMenu->m_pForm->bOpened&&pMainMenu->m_pNASaleFilterMenu->m_nFilterType == nType)
	{
		pMainMenu->m_pNASaleFilterMenu->m_pForm->Close();
		return;
	}
	if (nType>=0&&nType<3)
	{
		char sztxt[32];
		sprintf(sztxt,"xia%d",nType+1);
		pMainMenu->m_pNASaleFilterMenu->OpenForm(nType,pMainMenu->m_pNaSaleMenu->m_pForm->GetComponent(sztxt)->nX,pMainMenu->m_pNaSaleMenu->m_pForm->GetComponent(sztxt)->nY);
	}
}

void ClickLastPageEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	NASaleMenu*	pMenu	= (NASaleMenu*)param;
	if (pMenu)
	{
		if (pMenu->m_nPage < 1)
			return;
		pMenu->FilterUpdata(pMenu->m_nPage-1);
		pMenu->m_nPage--;
	}
}

void ClickNextPageEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	NASaleMenu*	pMenu	= (NASaleMenu*)param;
	if (pMenu)
	{
		if (pMenu->m_nPage > pMenu->m_nMaxPage-1)
			return;
		pMenu->FilterUpdata(pMenu->m_nPage+1);
		pMenu->m_nPage++;
	}
}

NASaleMenu::NASaleMenu():m_nMySaleCnt(0),m_nPage(0),m_nMaxPage(0)
{
	m_pForm		= pMainInterface->GetDForm("jishou2");
	m_pMySaleForm	= pMainInterface->GetDForm("jishou");


	DComponent* pCompBase	= m_pForm->GetComponent("zi");
	DComponent*	ppComp[8];
	ppComp[0]	= m_pForm->GetComponent("mb");
	ppComp[1]	= m_pForm->GetComponent("gezi1");
	ppComp[2]	= m_pForm->GetComponent("pz");
	ppComp[3]	= m_pForm->GetComponent("lv");
	ppComp[4]	= m_pForm->GetComponent("num");
	ppComp[5]	= m_pForm->GetComponent("dj");
	ppComp[6]	= m_pForm->GetComponent("icon1");
	ppComp[7]	= m_pForm->GetComponent("s");
	m_pForm->pCFrame	= new CCtrlFrame(pCompBase->nX,pCompBase->nY,pCompBase->nW,(pCompBase->nH+5)*4);
	m_pForm->pCFrame->InitFParts(pCompBase,ppComp,8);
	m_pForm->pCFrame->nSpace	= 5;
	m_pForm->pCFrame->nDisStep	= pCompBase->nH+5;
	m_pForm->pCFrame->SetEventEx(NASaleClickFrm,this);

	pCompBase	= m_pMySaleForm->GetComponent("zi");
	ppComp[0]	= m_pMySaleForm->GetComponent("mb");
	ppComp[1]	= m_pMySaleForm->GetComponent("gezi1");
	ppComp[2]	= m_pMySaleForm->GetComponent("pz");
	ppComp[3]	= m_pMySaleForm->GetComponent("lv");
	ppComp[4]	= m_pMySaleForm->GetComponent("num");
	ppComp[5]	= m_pMySaleForm->GetComponent("dj");
	ppComp[6]	= m_pMySaleForm->GetComponent("icon1");
	ppComp[7]	= m_pMySaleForm->GetComponent("s");
	m_pMySaleForm->pCFrame	= new CCtrlFrame(pCompBase->nX,pCompBase->nY,pCompBase->nW,(pCompBase->nH+5)*4);
	m_pMySaleForm->pCFrame->InitFParts(pCompBase,ppComp,8);
	m_pMySaleForm->pCFrame->nSpace	= 5;
	m_pMySaleForm->pCFrame->nDisStep	= pCompBase->nH+5;
	m_pMySaleForm->pCFrame->SetEventEx(NAMyClickFrm,this);

	m_pForm->GetComponent("me")->SetEvent(RQOpenMyEvent, this);
	m_pForm->GetComponent("qg")->SetVisible(false);

	m_pForm->GetComponent("xia1")->SetEvent(NASaleFilterEvent, (void*)0);
	m_pForm->GetComponent("xia2")->SetEvent(NASaleFilterEvent, (void*)1);
	m_pForm->GetComponent("xia3")->SetEvent(NASaleFilterEvent, (void*)2);

	m_pForm->GetComponent("left")->SetEvent(ClickLastPageEvent, this);
	m_pForm->GetComponent("right")->SetEvent(ClickNextPageEvent, this);

	m_pForm->GetComponent("page1")->SetVisible(false);
	m_pForm->GetComponent("page2")->SetVisible(false);

	m_pMySaleForm->GetComponent("js")->SetEvent(RQSellEvent, this);
	m_pMySaleForm->GetComponent("fh")->SetEvent(RQBackSaleEvent, this);
	m_pMySaleForm->GetComponent("qc")->SetEvent(RQCalcSaleEvent, this);
	m_pMySaleForm->GetComponent("qg")->SetVisible(false);
	m_pMySaleForm->GetComponent("page1")->SetVisible(false);
	m_pMySaleForm->GetComponent("page2")->SetVisible(false);
	m_nCurSelPos	= -1;

	m_pListSale		= xnList::Create();
	m_pListMine		= xnList::Create();

	for (int i = 0; i < 3; i++)
	{
		m_nFilterType[i] = 0;
	}
	//m_pTypeFilterList = xnList::Create();
	//m_pJopFilterList = xnList::Create();
	//m_pRankFilterList = xnList::Create();
}

NASaleMenu::~NASaleMenu()
{
	while(m_pListSale->Count)
		delete (NASaleClient*)m_pListSale->Delete(0);
	m_pListSale->Free();

	while(m_pListMine->Count)
		delete (NASaleClient*)m_pListMine->Delete(0);
	m_pListMine->Free();

	/*while(m_pTypeFilterList->Count)
		delete m_pTypeFilterList->Delete(0);
	m_pTypeFilterList->Free();

	while(m_pJopFilterList->Count)
		delete m_pJopFilterList->Delete(0);
	m_pJopFilterList->Free();

	while(m_pRankFilterList->Count)
		delete m_pRankFilterList->Delete(0);
	m_pRankFilterList->Free();*/
}

extern void	SetItemToDCom(DComponent* pDom,int nID);
extern void	SetItemToDCom(DComponent* pDom,ItemObj* pObj);

bool NASaleMenu::PacketProcess( ZPacket* pPacket )
{
	if(pPacket->bCmdGroup != NASALE)
		return false;

	switch(pPacket->bCmd)
	{
	case NA_SELL_LIST:
		{
			SET_DATA(pData, NASALE, NA_SELL_LIST, pPacket);
			m_pForm->pCFrame->SetFPCnt(pData->wCnt);
			m_nCurSelPos	= -1;
			m_nMaxPage = pData->wPageCnt;
			char	szTmp[256];
			DWORD	dwCurSec	= xnGetSecCount();
			for (int i = 0; i < pData->wCnt; i++)
			{
				
				ItemExBase*	pBase	= ItemConfig::GetInstance().GetItemBase(pData->szData[i].wItemID);
				if(pBase)
				{
					NASaleClient* pSale	= new NASaleClient();
					pSale->byType		= pData->szData[i].byType;
					pSale->dwItemInfo	= pData->szData[i].dwExInfo;
					pSale->dwNAIndex	= pData->szData[i].dwNaKey;
					pSale->dwEndSec		= pData->szData[i].nRmSec+dwCurSec;
					pSale->nPrice		= pData->szData[i].nPrice;
					pSale->wItemCnt		= pData->szData[i].wCnt;
					pSale->wItemID		= pData->szData[i].wItemID;
					pSale->pBase		= pBase;
					m_pListSale->Add(pSale);

					m_pForm->pCFrame->GetFPComp(i,"mb")->SetVisible(false);
					DComponent* pComp	= m_pForm->pCFrame->GetFPComp(i,"icon1");
					sprintf(szTmp,"item/%d.png",pBase->nSpr);
					pComp->SetViewSpr(szTmp);
					if(pSale->wItemCnt > 1)
						pComp->SetCaptionEx("%d",2,pSale->wItemCnt);
					else
						pComp->SetCaption("",2);
					char	szTmp[32];
					sprintf(szTmp,"MB_PZ%d.png",pBase->nStar+1);
					pComp->SetAddSprByTag(szTmp,-2,-2,1,100,0,-1);

					DWORD dwTime = pSale->dwEndSec-dwCurSec;
					if (dwTime/3600)
						m_pForm->pCFrame->GetFPComp(i,"dj")->SetCaptionEx("%d小时",1,dwTime/3600);
					else
						m_pForm->pCFrame->GetFPComp(i,"dj")->SetCaption("小于1小时",1);
					m_pForm->pCFrame->GetFPComp(i,"lv")->SetCaptionEx("%d",1,pBase->byLv);
					m_pForm->pCFrame->GetFPComp(i,"num")->SetCaptionEx("%d",1,pSale->nPrice);
					m_pForm->pCFrame->GetFPComp(i,"pz")->SetCaption(pBase->szName,1);
					m_pForm->pCFrame->GetFPComp(i,"pz")->SetSprColor(nItemClr[pBase->nStar][0],nItemClr[pBase->nStar][1],nItemClr[pBase->nStar][2]);
					m_pForm->pCFrame->SetFPTag(i,pSale);
				}
			}
		}
		return true;
	case NA_MY_LIST:
		{
			SET_DATA(pData, NASALE, NA_MY_LIST, pPacket);
			m_pMySaleForm->pCFrame->SetFPCnt(pData->wCnt);
			m_nCurSelPos	= -1;
			m_nMySaleCnt = pData->wCnt;
			char	szTmp[256];
			DWORD	dwCurSec	= xnGetSecCount();
			m_nMySaleMoney	= 0;
			for (int i = 0; i < pData->wCnt; i++)
			{

				ItemExBase*	pBase	= ItemConfig::GetInstance().GetItemBase(pData->szData[i].wItemID);
				if(pBase)
				{
					NASaleClient* pSale	= new NASaleClient();
					pSale->byType		= pData->szData[i].byType;
					pSale->dwItemInfo	= pData->szData[i].dwExInfo;
					pSale->dwNAIndex	= pData->szData[i].dwNaKey;
					pSale->dwEndSec		= pData->szData[i].nRmSec+dwCurSec;
					pSale->nPrice		= pData->szData[i].nPrice;
					pSale->wItemCnt		= pData->szData[i].wCnt;
					pSale->wItemID		= pData->szData[i].wItemID;
					pSale->pBase		= pBase;
					m_pListMine->Add(pSale);

					m_pMySaleForm->pCFrame->GetFPComp(i,"mb")->SetVisible(false);
					DComponent* pComp	= m_pMySaleForm->pCFrame->GetFPComp(i,"icon1");
					sprintf(szTmp,"item/%d.png",pBase->nSpr);
					pComp->SetViewSpr(szTmp);
					if(pSale->wItemCnt > 1)
						pComp->SetCaptionEx("%d",2,pSale->wItemCnt);
					else
						pComp->SetCaption("",2);

					if(pSale->byType	== 9)
					{
						pComp->SetAddSprByTag("BT_PM_GUOQI.png",-2,-2,1);
						m_pMySaleForm->pCFrame->GetFPComp(i,"dj")->SetCaption("0",1);
					}
					else if(pSale->byType == 8)
					{
						pComp->SetAddSprByTag("MB_PM_YISHOU.png",-2,-2,1);
						m_nMySaleMoney	+= pData->szData[i].nPrice;
						m_pMySaleForm->pCFrame->GetFPComp(i,"dj")->SetCaption("",1);
					}
					else
					{
						char	szTmp[32];
						sprintf(szTmp,"MB_PZ%d.png",pBase->nStar+1);
						pComp->SetAddSprByTag(szTmp,-2,-2,1,100,0,-1);

						DWORD dwTime = pSale->dwEndSec-dwCurSec;
						if (dwTime/3600)
							m_pMySaleForm->pCFrame->GetFPComp(i,"dj")->SetCaptionEx("%d小时",1,dwTime/3600);
						else
							m_pMySaleForm->pCFrame->GetFPComp(i,"dj")->SetCaption("小于1小时",1);
					}

					m_pMySaleForm->pCFrame->GetFPComp(i,"lv")->SetCaptionEx("%d",1,pBase->byLv);
					m_pMySaleForm->pCFrame->GetFPComp(i,"num")->SetCaptionEx("%d",1,pSale->nPrice);
					m_pMySaleForm->pCFrame->GetFPComp(i,"pz")->SetCaption(pBase->szName,1);
					m_pMySaleForm->pCFrame->GetFPComp(i,"pz")->SetSprColor(nItemClr[pBase->nStar][0],nItemClr[pBase->nStar][1],nItemClr[pBase->nStar][2]);
					m_pMySaleForm->pCFrame->SetFPTag(i,pSale);
				}
			}
		}
		return true;
	}
	return true;
}
static int nFType[8] = {0,6,2,4,7,8,5,20};
void NASaleMenu::FilterUpdata(int nPage)
{
	if (nPage<0||nPage>m_nMaxPage)
		return;

	SearchSellItems(m_nFilterType[1],nFType[m_nFilterType[0]],m_nFilterType[2],nPage*30);
	/*if (m_pTypeFilterList)
	{
		while(m_pTypeFilterList->Count)
			 m_pTypeFilterList->Delete(0);
	}
	if (m_pJopFilterList)
	{
		while(m_pJopFilterList->Count)
			 m_pJopFilterList->Delete(0);
	}
	if (m_pRankFilterList)
	{
		while(m_pRankFilterList->Count)
			 m_pRankFilterList->Delete(0);
	}
	
	if (m_nFilterType[0])
	{
		for (int i = 0; i < m_pListSale->Count; i++)
		{
			NASaleClient* pTemp	= (NASaleClient*)m_pListSale->Items[i];
			if (pTemp&&pTemp->pBase&&pTemp->pBase->byType == nFType[m_nFilterType[0]]&&m_nFilterType[0]<7)
			{
				m_pTypeFilterList->Add(pTemp);
			}else if (pTemp&&pTemp->pBase&&nFType[m_nFilterType[0]] == 20&&(pTemp->pBase->byType == 0||pTemp->pBase->byType == 1))
			{
				m_pTypeFilterList->Add(pTemp);
			}
		}
	}
	if (m_nFilterType[1])
	{
		if (m_nFilterType[0])
		{
			for (int i = 0; i < m_pTypeFilterList->Count; i++)
			{
				NASaleClient* pTemp	= (NASaleClient*)m_pTypeFilterList->Items[i];
				if (pTemp&&pTemp->pBase&&pTemp->pBase->nJob&(1<<m_nFilterType[1]))
				{
					m_pJopFilterList->Add(pTemp);
				}
			}
		}else
		{
			for (int i = 0; i < m_pListSale->Count; i++)
			{
				NASaleClient* pTemp	= (NASaleClient*)m_pListSale->Items[i];
				if (pTemp&&pTemp->pBase&&pTemp->pBase->nJob&(1<<m_nFilterType[1]))
				{
					m_pJopFilterList->Add(pTemp);
				}
			}
		}
	}
	if (m_nFilterType[2])
	{
		if (m_nFilterType[0]&&!m_nFilterType[1])
		{
			for (int i = 0; i < m_pTypeFilterList->Count; i++)
			{
				NASaleClient* pTemp	= (NASaleClient*)m_pTypeFilterList->Items[i];
				if (pTemp&&pTemp->pBase&&(pTemp->pBase->byLv > (m_nFilterType[2]-1)*10)&&(pTemp->pBase->byLv <= m_nFilterType[2]*10))
				{
					m_pRankFilterList->Add(pTemp);
				}
			}
		}else if (m_nFilterType[1])
		{
			for (int i = 0; i < m_pJopFilterList->Count; i++)
			{
				NASaleClient* pTemp	= (NASaleClient*)m_pJopFilterList->Items[i];
				if (pTemp&&pTemp->pBase&&(pTemp->pBase->byLv > (m_nFilterType[2]-1)*10)&&(pTemp->pBase->byLv <= m_nFilterType[2]*10))
				{
					m_pRankFilterList->Add(pTemp);
				}
			}
		}else
		{
			for (int i = 0; i < m_pListSale->Count; i++)
			{
				NASaleClient* pTemp	= (NASaleClient*)m_pListSale->Items[i];
				if (pTemp&&pTemp->pBase&&(pTemp->pBase->byLv > (m_nFilterType[2]-1)*10)&&(pTemp->pBase->byLv <= m_nFilterType[2]*10))
				{
					m_pRankFilterList->Add(pTemp);
				}
			}
		}
	}
	if (!m_nFilterType[0]&&!m_nFilterType[1]&&!m_nFilterType[2])
	{
		Refresh(m_pListSale);
	}else if (m_nFilterType[0]&&!m_nFilterType[1]&&!m_nFilterType[2])
	{
		Refresh(m_pTypeFilterList);
	}else if (m_nFilterType[1]&&!m_nFilterType[2])
	{
		Refresh(m_pJopFilterList);
	}else if (m_nFilterType[2])
	{
		Refresh(m_pRankFilterList);
	}*/
}

/*void NASaleMenu::Refresh(xnList* pList)
{
	m_pForm->pCFrame->SetFPCnt(pList->Count);

	if (pList->Count)
	{
		char	szTmp[256];
		for (int i = 0; i < pList->Count; i++)
		{
			NASaleClient* pSale	= (NASaleClient*)pList->Items[i];
			m_pForm->pCFrame->GetFPComp(i,"mb")->SetVisible(false);
			DComponent* pComp	= m_pForm->pCFrame->GetFPComp(i,"icon1");
			sprintf(szTmp,"item/%d.png",pSale->pBase->nSpr);
			pComp->SetViewSpr(szTmp);
			if(pSale->wItemCnt > 1)
				pComp->SetCaptionEx("%d",2,pSale->wItemCnt);
			else
				pComp->SetCaption("",2);
			char	szTmp[32];
			sprintf(szTmp,"MB_PZ%d.png",pSale->pBase->nStar+1);
			pComp->SetAddSprByTag(szTmp,-2,-2,1,100,0,-1);

			DWORD dwTime = pSale->dwEndSec-xnGetSecCount();
			if (dwTime/3600)
				m_pForm->pCFrame->GetFPComp(i,"dj")->SetCaptionEx("%d小时",1,dwTime/3600);
			else
				m_pForm->pCFrame->GetFPComp(i,"dj")->SetCaption("小于1小时",1);
			m_pForm->pCFrame->GetFPComp(i,"lv")->SetCaptionEx("%d",1,pSale->pBase->byLv);
			m_pForm->pCFrame->GetFPComp(i,"num")->SetCaptionEx("%d",1,pSale->nPrice);
			m_pForm->pCFrame->GetFPComp(i,"pz")->SetCaption(pSale->pBase->szName,1);
			m_pForm->pCFrame->GetFPComp(i,"pz")->SetSprColor(nItemClr[pSale->pBase->nStar][0],nItemClr[pSale->pBase->nStar][1],nItemClr[pSale->pBase->nStar][2]);
			m_pForm->pCFrame->SetFPTag(i,pSale);
		}
	}
}*/

void NASaleMenu::OpenForm()
{
	m_pForm->Open();
	
	m_pForm->GetComponent("qy")->SetCaption(NASaleFilter1[m_nFilterType[0]],0);
	m_pForm->GetComponent("zy")->SetCaption(NASaleFilter2[m_nFilterType[1]],0);
	m_pForm->GetComponent("lvl")->SetCaption(NASaleFilter3[m_nFilterType[2]],0);

	//SearchSellItems(0,0,0,0);
	SearchSellItems(m_nFilterType[1],nFType[m_nFilterType[0]],m_nFilterType[2],0);
	m_pForm->pCFrame->SetFPCnt(0);
	m_pSelSale	= NULL;
}

void NASaleMenu::OpenMySale()
{
	m_nMySaleMoney	= 0;
	m_pMySaleForm->Open();

	ViewMySales();
	while(m_pListMine->Count)
		delete (NASaleClient*)m_pListMine->Delete(0);
	m_pMySaleForm->pCFrame->SetFPCnt(0);
	m_pSelSale	= NULL;
}

void NASaleMenu::Update()
{
	if(m_pForm->bOpened == false && m_pMySaleForm->bOpened == false)
		return;
	m_pForm->GetComponent("left")->SetVisible(m_nPage>0);
	m_pForm->GetComponent("right")->SetVisible(m_nPage<m_nMaxPage);
	m_pForm->GetComponent("jinbiC4")->SetCaptionEx("%d/%d",1,m_nPage+1,m_nMaxPage+1);
	m_pForm->GetComponent("jinbi")->SetCaptionEx("%d",1,pChar->m_dwMoney);

	m_pMySaleForm->GetComponent("jinbi")->SetCaptionEx("%d",1,pChar->m_dwMoney);
	m_pMySaleForm->GetComponent("total")->SetCaptionEx("%d",1,m_nMySaleMoney);
	m_pMySaleForm->GetComponent("qc")->SetVisible(m_nMySaleMoney);
	m_pMySaleForm->GetComponent("num1")->SetCaptionEx("%d/10",1,m_nMySaleCnt>0?m_nMySaleCnt:0);
}

void NASaleMenu::SearchSellItems( BYTE bJob, BYTE bType, BYTE bLvStep, int nStartPos )
{
	BuildPacketEx(pPacket, NASALE, NA_SEARCH_SELL,buf,256);
	SET_DATA(pSend, NASALE,NA_SEARCH_SELL,pPacket);
	pSend->bJob		= bJob;
	pSend->bLvStep	= bLvStep;
	pSend->bType	= bType;
	pSend->nStartPos	= nStartPos;
	SendPacket(pPacket);

	while(m_pListSale->Count)
		delete (NASaleClient*)m_pListSale->Delete(0);
}

void NASaleMenu::ViewMySales()
{
	SendCmd(NASALE,NA_MY_LIST);
}

void NASaleMenu::RequestSellItem( ItemObj* pItem, int nPrice, int nCount )
{
	if(nPrice < 100)
	{
		pMainMenu->PopTipMsg("不支持过低的价格");
		return;
	}
	if (nCount > pItem->GetVal(ITEM_AD_NUM))
	{
		pMainMenu->PopTipMsg("超过拥有数量");
		return;
	}
	if (m_nMySaleCnt >= 10)
	{
		pMainMenu->PopTipMsg("寄售数量已满，无法再寄售。");
		return;
	}
	BuildPacketEx(pPacket, NASALE, NA_SELL_ITEM_EX,buf,256);
	SET_DATA(pSend, NASALE,NA_SELL_ITEM_EX,pPacket);
	pSend->dwIndex		= pItem->GetVal(ITEM_AD_ID);
	pSend->nPrice	= nPrice;
	pSend->nCount   = nCount;
	SendPacket(pPacket);

	pMainMenu->PopTipMsg("寄售成功！");
	//SearchSellItems(0,0,0,0);
	ViewMySales();
}

void NASaleMenu::RequestBuyItem()
{
	if(!m_pSelSale)
		return;

	if(pChar->m_byLvl < 20)
	{
		pMainMenu->PopTipMsg("等级达到20级开启购买寄售行物品功能");
		return;
	}

	if (m_pSelSale->nPrice > pChar->m_dwMoney)
	{
		pMainMenu->PopTipMsg("金币不足");
		return;
	}

	BuildPacketEx(pPacket, NASALE, NA_BUY_ITEM,buf,256);
	SET_DATA(pSend, NASALE,NA_BUY_ITEM,pPacket);
	pSend->dwNAIndex	= m_pSelSale->dwNAIndex;
	pSend->wCnt			= m_pSelSale->wItemCnt;
	SendPacket(pPacket);

	SearchSellItems(m_nFilterType[1],nFType[m_nFilterType[0]],m_nFilterType[2],0);
}

void NASaleMenu::RequestGetMyItem()
{
	if(!m_pSelSale)
		return;
	if(m_pSelSale->byType == 8)
	{
		pMainMenu->PopTipMsg("已售物品不可取回");
		return;
	}
	BuildPacketEx(pPacket, NASALE, NA_BUY_ITEM,buf,256);
	SET_DATA(pSend, NASALE,NA_BUY_ITEM,pPacket);
	pSend->dwNAIndex	= m_pSelSale->dwNAIndex;
	pSend->wCnt			= 0;
	SendPacket(pPacket);

	ViewMySales();
}
static char* szFilterTypeTxt[] = {"qy","zy","lvl"};
static void CBFrameEvent(int nPos,char* szName,void* param)
{
	NASaleFilterMenu* pMenu = (NASaleFilterMenu*)param;

	if (strncmp (szName, "qy1",3) == 0&&pMenu->m_nFilterType >= 0&&pMenu->m_nFilterType<3)
	{
		pMainMenu->m_pNaSaleMenu->m_pForm->GetComponent(szFilterTypeTxt[pMenu->m_nFilterType])->SetCaption("",0);
		switch(pMenu->m_nFilterType)
		{
		case 0:
			{
				pMainMenu->m_pNaSaleMenu->m_nFilterType[0] = nPos;
				pMainMenu->m_pNaSaleMenu->m_pForm->GetComponent(szFilterTypeTxt[pMenu->m_nFilterType])->SetCaption(NASaleFilter1[nPos],0);
				pMainMenu->m_pNaSaleMenu->FilterUpdata();
			}
			break;
		case 1:
			{
				pMainMenu->m_pNaSaleMenu->m_nFilterType[1] = nPos;
				pMainMenu->m_pNaSaleMenu->m_pForm->GetComponent(szFilterTypeTxt[pMenu->m_nFilterType])->SetCaption(NASaleFilter2[nPos],0);
				pMainMenu->m_pNaSaleMenu->FilterUpdata();
			}
			break;
		case 2:
			{
				pMainMenu->m_pNaSaleMenu->m_nFilterType[2] = nPos;
				pMainMenu->m_pNaSaleMenu->m_pForm->GetComponent(szFilterTypeTxt[pMenu->m_nFilterType])->SetCaption(NASaleFilter3[nPos],0);
				pMainMenu->m_pNaSaleMenu->FilterUpdata();
			}
			break;
		}
	}
	pMenu->m_pForm->Close();
}

NASaleFilterMenu::NASaleFilterMenu()
{
	m_pForm = pMainInterface->GetDForm("jishou3");

	DComponent*	pCompBase	= m_pForm->GetComponent("d1");
	m_pForm->pCFrame	= new CCtrlFrame(pCompBase->nX,pCompBase->nY,pCompBase->nW,(pCompBase->nH+15)*5);
	DComponent*	ppCompPt[2];
	ppCompPt[0]		= m_pForm->GetComponent("qy1");
	ppCompPt[1]		= m_pForm->GetComponent("x1");
	m_pForm->pCFrame->InitFParts(pCompBase,ppCompPt,2);
	m_pForm->pCFrame->nSpace	= 15;
	m_pForm->pCFrame->nDisStep	= pCompBase->nH+15;
	m_pForm->pCFrame->SetEventEx(CBFrameEvent,this);

	m_nFilterType = -1;
}

NASaleFilterMenu::~NASaleFilterMenu()
{

}

void NASaleFilterMenu::OpenForm(int nType,int nX,int nY)
{
	m_nFilterType = nType;

	m_pForm->SetPos(nX,nY-125);
	CCtrlFrame*	pFrame	= m_pForm->pCFrame;
	m_pForm->Open();

	switch(nType)
	{
	case 0:
		{
			pFrame->SetFPCnt(8);
			for (int i = 0; i < 8; i++)
			{
				pFrame->GetFPComp(i,"qy1")->SetCaptionEx("%s",1,NASaleFilter1[i]);
			}
		}
		break;
	case 1:
		{
			pFrame->SetFPCnt(5);
			for (int i = 0; i < 5; i++)
			{
				pFrame->GetFPComp(i,"qy1")->SetCaptionEx("%s",1,NASaleFilter2[i]);
			}
		}
		break;
	case 2:
		{
			pFrame->SetFPCnt(11);
			for (int i = 0; i < 11; i++)
			{
				pFrame->GetFPComp(i,"qy1")->SetCaptionEx("%s",1,NASaleFilter3[i]);
			}
		}
		break;
	}
}

void fun_cbInputSaleNums(int nResult,void * param)
{
	if (nResult < 0)
		nResult = 0;

	pMainMenu->m_pNASalePriceMenu->m_nNums = nResult;
	pMainMenu->m_pNASalePriceMenu->m_pForm->GetComponent("sl")->SetCaptionEx("%d",1,nResult);
}

void InputSaleNums(InterfaceEvent* pEvent, void* param)
{
	if (pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	pMainMenu->m_pGetNumMenu->Open(fun_cbInputSaleNums,99999999,0,NULL);
}

void fun_cbInputSalePrice(int nResult,void * param)
{
	if (nResult < 0)
		nResult = 0;

	pMainMenu->m_pNASalePriceMenu->m_nPrice = nResult;
	pMainMenu->m_pNASalePriceMenu->m_pForm->GetComponent("jg")->SetCaptionEx("%d",1,nResult);
}

void InputSalePrice(InterfaceEvent* pEvent, void* param)
{
	if (pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	pMainMenu->m_pGetNumMenu->Open(fun_cbInputSalePrice,99999999,0,NULL);
}

void SetNaSaleItem(InterfaceEvent* pEvent, void* param)
{
	if (pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	NASalePriceMenu* pMenu = (NASalePriceMenu*)param;
	if (pMenu)
	{
		if (pMenu->m_nNums < 1)
		{
			pMainMenu->PopTipMsg("请输入数量");
			return;
		}

		if (pMenu->m_nPrice < 100)
		{
			pMainMenu->PopTipMsg("不支持过低价格");
			return;
		}

		if (pMenu->m_pSaleItem)
		{
			if (pMenu->m_nNums > pMenu->m_pSaleItem->GetVal(ITEM_AD_NUM))
			{
				pMainMenu->PopTipMsg("超过物品拥有数量");
				return;
			}

			if (pMenu->m_nNums > pMenu->m_pSaleItem->pItemBase->nNum)
			{
				pMainMenu->PopTipMsg("超过物品堆叠数量");
				return;
			}

			pMainMenu->m_pNaSaleMenu->RequestSellItem(pMenu->m_pSaleItem,pMenu->m_nPrice,pMenu->m_nNums);
		}else
			pMainMenu->PopTipMsg("寄售失败");

		pMainMenu->ShowNetWaiting();
		pMenu->Close();
	}
}

NASalePriceMenu::NASalePriceMenu()
{
	m_pForm = pMainInterface->GetDForm("xuanjs");

	m_pForm->GetComponent("sl")->SetEvent(InputSaleNums);
	m_pForm->GetComponent("jg")->SetEvent(InputSalePrice);

	m_pForm->GetComponent("js")->SetEvent(SetNaSaleItem,this);

	m_nNums = 0;
	m_nPrice = 0;
	m_pSaleItem = NULL;
}

NASalePriceMenu::~NASalePriceMenu()
{

}

extern char* szItemType[ITEMTYPE_MAX];
extern char* szEquipSlot[EQUIP_MAX];
extern  void ChangeJobToTxt(char* sztxt,int nJob);

void NASalePriceMenu::OpenForm(ItemObj* pObj)
{
	m_pSaleItem = pObj;
	if (pObj)
	{
		SetItemToDCom(m_pForm->GetComponent("icon"),pObj->pItemBase->nTypeID);

		m_pForm->GetComponent("name")->SetCaption(pObj->pItemBase->szName,0);
		m_pForm->GetComponent("name")->SetSprColor(nItemColor[pObj->pItemBase->nStar][0],nItemColor[pObj->pItemBase->nStar][1],nItemColor[pObj->pItemBase->nStar][2]);

		if (pObj->pItemBase->byType == ITEMTYPE_EQUIP)
			m_pForm->GetComponent("buwei")->SetCaption(szEquipSlot[pObj->pItemBase->bySlot],0);
		else
			m_pForm->GetComponent("buwei")->SetCaption(szItemType[pObj->pItemBase->byType],0);

		m_pForm->GetComponent("lv")->SetCaptionEx("使用等级:%d",0,pObj->pItemBase->byLv);
		char sztxt[32];
		ChangeJobToTxt(sztxt,pObj->pItemBase->nJob);
		m_pForm->GetComponent("zhiye")->SetCaptionEx("职业限制:%s",0,sztxt);
		bool bLimit = pObj->pItemBase->nJob&(1<<pChar->m_byJob);
		m_pForm->GetComponent("zhiye")->SetSprColor(bLimit?255:nItemLimitColor[0][0],bLimit?255:nItemLimitColor[0][1],bLimit?255:nItemLimitColor[0][2]);

		if (pObj->pItemBase->nNum > 1)
			m_nNums = pObj->GetVal(ITEM_AD_NUM);
		else
			m_nNums = 1;

		m_nPrice = 0;

		m_pForm->GetComponent("sl")->SetCaptionEx("%d",1,m_nNums);
		m_pForm->GetComponent("jg")->SetCaptionEx("%d",1,m_nPrice);

		m_pForm->Open();
	}
}

void NASalePriceMenu::Close()
{
	m_nPrice = 0;
	m_nNums  = 0;
	m_pSaleItem = NULL;

	m_pForm->Close();
}