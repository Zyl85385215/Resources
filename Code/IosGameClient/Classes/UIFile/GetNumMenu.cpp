#include "MainMenu.h"
#include "UserData.h"
#include "GetNumMenu.h"
#include "ios_item_packet.h"


static void NumInput(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nVal = (int)param;
	int nPer = pMainMenu->m_pGetNumMenu->m_nCount;
	if (pMainMenu->m_pGetNumMenu->m_nCount)
	{
		pMainMenu->m_pGetNumMenu->m_nCount = pMainMenu->m_pGetNumMenu->m_nCount*10+nVal;
	}else{
		pMainMenu->m_pGetNumMenu->m_nCount = nVal;
	}
	if (pMainMenu->m_pGetNumMenu->m_nCount>pMainMenu->m_pGetNumMenu->m_nMaxCount)
	{
		pMainMenu->m_pGetNumMenu->m_nCount = nPer;
	}
	pMainMenu->m_pGetNumMenu->m_bUpdate = true;
}
static void DeleteNumInput(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pGetNumMenu->m_nCount = pMainMenu->m_pGetNumMenu->m_nCount/10;
	pMainMenu->m_pGetNumMenu->m_bUpdate = true;

}

static void CloseNumInput(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;	
	pMainMenu->m_pGetNumMenu->Close();
	if (pMainMenu->m_pGetNumMenu->m_nCount < 1)
	{
		pMainMenu->PopTipMsg("数量不能为零！");
		return;
	}
	pMainMenu->m_pGetNumMenu->eventFunc(pMainMenu->m_pGetNumMenu->m_nCount,pMainMenu->m_pGetNumMenu->m_param);


}
GetNumMenu::GetNumMenu():m_bUpdate(true),m_nMaxCount(9999999),m_nCount(0)
{
	m_pForm = pMainInterface->GetDForm ("numjianpan");
	m_pForm->bZorde = 150;
	char sztxt[32];
	for (int i=0;i<10;i++)
	{
		sprintf(sztxt,"%d",i);
		m_pForm->GetComponent(sztxt)->SetEvent(NumInput,(void*)i);
	}
	m_pForm->GetComponent("no")->SetEvent(DeleteNumInput);
	m_pForm->GetComponent("ok")->SetEvent(CloseNumInput);

}

GetNumMenu::~GetNumMenu()
{
}



void GetNumMenu::Open( void (*EventFunc)(int nResult,void * param),int nMaxCount /*= 0*/,int nStartCount /*= 0*/, void * para /*= NULL*/ )
{
	eventFunc = EventFunc;
	m_nMaxCount = nMaxCount;
	m_nCount = nStartCount;
	m_param = para;
	m_bUpdate = true;
	m_pForm->Open();
}

void GetNumMenu::Close()
{
	m_pForm->Close();
}

void GetNumMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}
	if (m_bUpdate)
	{
		m_bUpdate = false;
		if (m_nCount>m_nMaxCount)
		{
			m_nCount = m_nMaxCount;
		}
		m_pForm->GetComponent("txt")->SetCaptionEx("%d",2,m_nCount);
//		m_pForm->GetComponent("txt")->SetCaptionEx("%d  最大值%d",2,m_nCount,m_nMaxCount);
	}
}
