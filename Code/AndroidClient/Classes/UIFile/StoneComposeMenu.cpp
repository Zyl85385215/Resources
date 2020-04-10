#include "MainMenu.h"
#include "UserData.h"
#include "StoneComposeMenu.h"
#include "ios_item_packet.h"

char* szAttriName[EQUIP_ATTR_MAX] = {"ÉúÃü","Ä§·¨","Îï¹¥","Ä§¹¥","Îï·À","Ä§·À","ÃüÖÐ","ÉÁ±Ü","±©»÷"};
extern void	SetItemToDCom(DComponent* pDom,ItemObj* pObj);
extern void	SetBagItemToDComByID(DComponent* pDom,int nID,int nNeedCount = 0);
extern void	SetItemToDCom(DComponent* pDom,int nID);
static void ComposeStone(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	StoneComposeMenu* pMenu = (StoneComposeMenu*)param;
	pChar->m_pItemCenter->ComposeStone(pMenu->m_nTypeID,pMenu->m_nCount);
}
static void	InputResult(int nResult,void * param){
	pMainMenu->m_pStoneComposeMenu->m_nCount = nResult;
	pMainMenu->m_pStoneComposeMenu->m_bUpdate = true;
};

static void InputComposeNum(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	StoneComposeMenu* pMenu = (StoneComposeMenu*)param;
	pMainMenu->m_pGetNumMenu->Open(InputResult,pMainMenu->m_pStoneComposeMenu->m_nMaxCount,pMainMenu->m_pStoneComposeMenu->m_nCount);
}
StoneComposeMenu::StoneComposeMenu():m_bUpdate(true),m_nTypeID(0),m_nMaxCount(0),m_nCount(0)
{
	m_pForm = pMainInterface->GetDForm ("baoshihecheng");
	
	
	m_pForm->GetComponent("hc")->SetEvent(ComposeStone,this);
	m_pForm->GetComponent("num")->SetEvent(InputComposeNum,this);

}

StoneComposeMenu::~StoneComposeMenu()
{
}

void StoneComposeMenu::Open( int nType )
{
	m_nTypeID = nType;
	m_nCount = 1;
	m_bUpdate = true;
	m_pForm->Open();
}

void StoneComposeMenu::Close()
{
	m_pForm->Close();
}

void StoneComposeMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}
	if (m_bUpdate)
	{
		m_bUpdate = false;
		int nItemCount = pChar->m_pItemCenter->GetItemCountByID(m_nTypeID);
		ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(m_nTypeID);
		if (!pBase)
		{
			return;
		}
		ItemExBase* pBaseNext = ItemConfig::GetInstance().GetItemBase(pBase->nDura);
		if (!pBaseNext)
		{
			return;
		}
		SetBagItemToDComByID(m_pForm->GetComponent("icon1"),m_nTypeID,nStoneComposeCount);
		SetItemToDCom(m_pForm->GetComponent("icon2"),pBase->nDura);
		m_nMaxCount = nItemCount/nStoneComposeCount;

		int nVal1 = 0;
		int nVal2 = 0;
		int nActCount1 = 0;
		int nActCount2 = 0;
		for(int i = EQUIP_ATTR_HP; i < EQUIP_ATTR_MAX; i ++)
		{
			if(pBase->nParam[i] > 0)
			{
				nVal1 = pBase->nParam[i];
				nActCount1 = i;
			}
			if(pBaseNext->nParam[i] > 0)
			{
				nVal2 = pBaseNext->nParam[i];
				nActCount2 = i;
			}
		}

		m_pForm->GetComponent("name1")->SetCaptionEx(pBase->szName,0);
		m_pForm->GetComponent("num1")->SetCaptionEx("%d/%d",1,nItemCount,nStoneComposeCount);
		if(nVal1 > 0)
		{
			m_pForm->GetComponent("sx1")->SetCaptionEx("%s+%d",1,szAttriName[nActCount1],nVal1);
			m_pForm->GetComponent("sx1")->SetSprColor(0, 255, 0);
		}
		else
			m_pForm->GetComponent("sx1")->SetCaption("",1);
		m_pForm->GetComponent("name2")->SetCaptionEx(pBaseNext->szName,0);
		m_pForm->GetComponent("num2")->SetCaptionEx("%d",1,m_nMaxCount);
		if(nVal2 > 0)
		{
			m_pForm->GetComponent("sx2")->SetCaptionEx("%s+%d",1,szAttriName[nActCount2],nVal2);
			m_pForm->GetComponent("sx2")->SetSprColor(0, 255, 0);
		}
		else
			m_pForm->GetComponent("sx2")->SetCaption("",1);
		
		if (m_nCount>=m_nMaxCount)
		{
			m_nCount = m_nMaxCount;
		}
		m_pForm->GetComponent("num")->SetCaptionEx("%d",1,m_nCount);
		m_pForm->GetComponent("money")->SetCaptionEx("%d",1,m_nCount*nStoneComposeMoney);

	}
}
