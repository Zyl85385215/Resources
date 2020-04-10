#include "ActivateMenu.h"
#include "UserData.h"
#include "ios_user_packet.h"
void ActivateButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	ActivateMenu*	pMenu = (ActivateMenu*)param;
	BuildPacketEx(pPacket, USER, C_ACTIVATE,buf, 256);
	SET_DATA(pSend, USER, C_ACTIVATE, pPacket);
	pSend->sztxt[31] = 0;
	strncpy(pSend->sztxt,pMenu->m_pForm->GetComponent("jhm")->GetEdit(),31);
	pChar->SendPacket(pPacket);
}

ActivateMenu::ActivateMenu()
{
	m_pForm = pMainInterface->GetDForm("jihuoma");
	m_pForm->GetComponent("ok")->SetEvent(ActivateButton,this);
}

ActivateMenu::~ActivateMenu()
{

}

void ActivateMenu::Open()
{
	m_pForm->GetComponent("jhm")->SetEdit("");
	m_pForm->Open();
}

void ActivateMenu::Close()
{
	m_pForm->Close();
}
