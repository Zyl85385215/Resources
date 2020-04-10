#include "LoginScene.h"
#include "LoginMenu.h"

void LRegistEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	LoginMenu* pMenu	= (LoginMenu*)param;
	pMenu->m_pFormRegist->Open();
	pMenu->m_pForm->Close();
}
void LRegistClose(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	LoginMenu* pMenu	= (LoginMenu*)param;
	pMenu->m_pFormRegist->Close();
	pMenu->m_pForm->Open();
}

void LogChangePswdClose(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	LoginMenu* pMenu	= (LoginMenu*)param;
	pMenu->m_pFormChgPswd->Close();
	pMenu->m_pForm->Open();
}

void OpenChangePswdForm(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	LoginMenu* pMenu	= (LoginMenu*)param;
	pMenu->m_pForm->Close();
	pMenu->m_pFormChgPswd->Open();
}

LoginMenu::LoginMenu()
{
	LoadConfig();
	m_pForm		= pMainInterface->GetDForm("denglu");
	m_pForm->GetComponent("zhuce")->SetEvent(LRegistEvent, this);
	m_pForm->GetComponent("name")->SetEdit(g_szLoginAcc);
	m_pForm->GetComponent("mima")->SetEdit(g_szLoginPswd);	
	m_pForm->GetComponent("mima")->byInputType	= INPUT_TYPE_PSWD;
	m_pForm->GetComponent("gmm")->SetEvent(OpenChangePswdForm, this);

	m_pFormRegist	= pMainInterface->GetDForm("zhuce");
	m_pFormRegist->GetComponent("name")->SetCaption("����6-16λ���ֻ���ĸ", 0);
	m_pFormRegist->GetComponent("mima")->SetCaption("����", 0);
	m_pFormRegist->GetComponent("ma")->SetCaption("����", 0);
	m_pFormRegist->GetComponent("mima")->byInputType	= INPUT_TYPE_PSWD;
	m_pFormRegist->GetComponent("ma")->byInputType	= INPUT_TYPE_PSWD;
	m_pFormRegist->GetComponent("close")->SetEvent(LRegistClose, this);

	m_pFormChgPswd	= pMainInterface->GetDForm("xgmm");
	m_pFormChgPswd->GetComponent("zh")->SetCaption("����6-16λ���ֻ���ĸ", 0);
	m_pFormChgPswd->GetComponent("laomima")->SetCaption("����", 0);
	m_pFormChgPswd->GetComponent("mima")->SetCaption("����", 0);
	m_pFormChgPswd->GetComponent("ma")->SetCaption("����", 0);
	m_pFormChgPswd->GetComponent("mima")->byInputType	= INPUT_TYPE_PSWD;
	m_pFormChgPswd->GetComponent("ma")->byInputType	= INPUT_TYPE_PSWD;
	m_pFormChgPswd->GetComponent("close")->SetEvent(LogChangePswdClose, this);
}

LoginMenu::~LoginMenu()
{
	Close();
}

void LoginMenu::LoadConfig()
{
	std::string strAcc = LoadStringFromXML("default_account","");
	std::string strPass = LoadStringFromXML("default_password","");
	g_szLoginAcc	= strdup(strAcc.c_str());
	g_szLoginPswd	= strdup(strPass.c_str());
	g_nServerID		= LoadIntegerFromXML("default_serverid",0);
	if(g_nServerID >= 10000)
		g_nServerID	= 0;
}

void LoginMenu::SaveConfig()
{
	SaveStringToXML("default_account",g_szLoginAcc);
	SaveStringToXML("default_password",g_szLoginPswd);
	SaveIntegerToXML("default_serverid",g_nServerID);
	SaveToXMLOK();
}

void LoginMenu::Open()
{
	m_pForm->Open();
	m_pFormRegist->Close();
}

void LoginMenu::Close()
{
	m_pForm->Close();
	m_pFormRegist->Close();
}
