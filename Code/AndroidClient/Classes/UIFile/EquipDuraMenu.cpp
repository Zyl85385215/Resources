#include "UserData.h"
#include "EquipDuraMenu.h"
#include "ItemData.h"
#include "ios_item_packet.h"
#include "SoundCenter.h"
#include "MainMenu.h"

extern int GetFixNeedGold();
void ClickOpenFixMenu(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		EDuraMenu* pMenu = (EDuraMenu*)param;
		pMenu->m_pForm2->GetComponent("money")->SetCaptionEx("%d",0,GetFixNeedGold());
		pMenu->m_pForm2->Open(2);
	}
}

void ClickCloseFixMenu(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		EDuraMenu* pMenu = (EDuraMenu*)param;
		pMenu->m_pForm2->Close();
	}
}

void ClickGoFixMenu(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		EDuraMenu* pMenu = (EDuraMenu*)param;
		pMenu->m_pForm2->Close();
		if (pChar->m_byVipLvl > 2)
		{
			g_pSoundCenter->PlayerEffect("Sound/hammer");
			pChar->SendCmd(ITEM,C_ITEM_FIXALL);	
		}else
			pChar->m_pItemCenter->GoForFix();
	}
}

void ClickGoFixExMenu(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		EDuraMenu* pMenu = (EDuraMenu*)param;
		if (pChar->m_dwRmb < 10)
		{
			pMainMenu->PopTipMsg("×êÊ¯²»×ã");
			return;
		}

		g_pSoundCenter->PlayerEffect("Sound/hammer");
		pChar->SendCmd(ITEM,C_ITEM_QUICK_FIX);	
		pMenu->m_pForm2->Close();
	}
}

static void OpenVipMenu(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	if(param)
	{
		EDuraMenu* pMenu = (EDuraMenu*)param;
		pMenu->m_pForm2->Close();
		pMainMenu->m_pVipMenu->Open();
	}
}

EDuraMenu::EDuraMenu()
{
	m_pForm = pMainInterface->GetDForm("xiuli2");
	m_pForm2 = pMainInterface->GetDForm("xiuli");
	char szName[8];
	DComponent* pComp = NULL;
	for(int i = 0; i < EQUIP_MAX; i ++)
	{
		sprintf(szName,"di%d",i+1);
		DComponent* pComp = m_pForm->GetComponent(szName);
		if(pComp)
			pComp->SetEvent(ClickOpenFixMenu,this);
	}
	m_pForm2->GetComponent("ok")->SetEvent(ClickGoFixMenu,this);
	m_pForm2->GetComponent("ok2")->SetEvent(ClickGoFixExMenu,this);
	m_pForm2->GetComponent("no")->SetEvent(ClickCloseFixMenu,this);
	m_pForm2->GetComponent("vip")->SetEvent(OpenVipMenu,this);
	m_pForm->GetComponent("di0")->SetEvent(ClickOpenFixMenu,this);
}

EDuraMenu::~EDuraMenu()
{

}

void EDuraMenu::Open()
{
	m_pForm->Open();
}

void EDuraMenu::Close()
{
	m_pForm->Close();
}

void EDuraMenu::Update()
{
	if(m_pForm && m_pForm->bOpened)
	{
		char szName[8];
		DComponent* pComp = NULL;
		for(int i = 0; i < EQUIP_MAX; i ++)
		{
			sprintf(szName,"di%d",i+1);
			DComponent* pComp = m_pForm->GetComponent(szName);
			if(pComp)
			{
				ItemObj* pItem = pChar->m_pItemCenter->GetEquipBySlot(i);
				if(pItem && pItem->GetVal(ITEM_AD_NUM) == 0)
					pComp->SetVisible(true);
				else
					pComp->SetVisible(false);
			}
		}
	}
}