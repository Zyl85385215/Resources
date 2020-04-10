#include "SelLineMenu.h"
#include "xnExtstr.h"
#include "LoginScene.h"
#include <list>
using namespace std;

static int	s_nMaxLine	= 1;
CreateSingleton(ServerLineCfg);

ServerLineCfg::ServerLineCfg()
{
	m_pList		= xnList::Create();


}

ServerLineCfg::~ServerLineCfg()
{
	while(m_pList->Count)
	{
		delete (ServerLine*)m_pList->Delete(0);
	}
	m_pList->Free();
}


void ServerLineCfg::LoadFile(  )
{
	while(m_pList->Count)
		delete (ServerLine*)m_pList->Delete(0);

	xnExtstr* pStr = xnExtstr::Import("Servercfg.ini");
	if(pStr)
	{
		int i = 0;
		while(pStr->SetSection(i++))
		{
			ServerLine* pLine = new ServerLine();
			pLine->szName = strdup(pStr->sectionName);
			pLine->szIP = strdup(pStr->ReadString("ip","192.168.1.100"));
			pLine->nPort = atoi(pStr->ReadString("port","8030"));
			pLine->szSpr = strdup(pStr->ReadString("spr","1x.png"));
			pLine->nSvrID	= atoi(pStr->ReadString("index","101"));
			m_pList->Add(pLine);

			if(i == 1)
				m_szAppUrl	= strdup(pStr->ReadString("appurl",""));
		}
	}
	pStr->Free();
}

ServerLine* ServerLineCfg::GetLine( int nArea, BYTE bLine )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		ServerLine* pSLine	= (ServerLine*)m_pList->Items[i];
		if(pSLine->nSvrID	== nArea*100+bLine)
			return pSLine;
	}
	return NULL;
}

void ServerLineCfg::RfSvrLine( int nIndex, char* szIP, int nPort )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		ServerLine* pSLine	= (ServerLine*)m_pList->Items[i];
		if(pSLine->nSvrID	== nIndex)
		{
			pSLine->szIP = strdup(szIP);
			pSLine->nPort	= nPort;

			return;
		}
	}

	ServerLine* pSLine	= new ServerLine();
	pSLine->nSvrID	= nIndex;
	pSLine->szIP	= strdup(szIP);
	pSLine->nPort	= nPort;
	m_pList->Add(pSLine);
}

ServerLine* ServerLineCfg::RandGetLine()
{
	if (m_pList->Count == 0)
		return NULL;

	int nLineCnt = m_pList->Count;
	int nRandRlt = 1 + rand()%(nLineCnt-1);  // 第一个是网关服
	ServerLine* pLine = (ServerLine*)m_pList->Items[nRandRlt];
	if (pLine)
		return pLine;
	else
		return (ServerLine*)m_pList->Items[1];
}

bool ServerLineCfg::CheckAreaVisiable(BYTE bArea)
{
	if (m_pList->Count == 0)
		return false;

	for (int i = 0; i < m_pList->Count; i++)
	{
		ServerLine* pSLine	= (ServerLine*)m_pList->Items[i];
		if (pSLine&&pSLine->nSvrID > 0 && pSLine->nSvrID/100 == bArea)
			return true;
	}

	return false;
}

void ServerLineCfg::GetAreaList(xnList* pList,BYTE byType)
{
	while(pList->Count)
		pList->Delete(0);


	for (int i = 0; i < m_pList->Count; i++)
	{
		ServerLine* pSLine	= (ServerLine*)m_pList->Items[i];
		if (pSLine)
		{
			int nArea = pSLine->nSvrID/100;
			if (!nArea)
				continue;

			if (byType == 1 && nArea < 11)
				continue;

			if (byType < 1  && nArea > 10)
				continue;

			bool  bAdd = true;
			int nTmp = 0;
			for (int j = 0; j < pList->Count; j++)
			{
				nTmp = (int)pList->Items[j];
				if (nArea == nTmp)
				{
					bAdd = false;
					break;
				}
			}
			if (bAdd)
				pList->Add((void*)nArea);
		}
	}
}

int ServerLineCfg::GetAreaCount(int nMaxLine)
{
	if (m_pList->Count == 0)
		return 1;

	int nCnt = (m_pList->Count > nMaxLine+1)?nMaxLine+1:m_pList->Count;

	list<int> listArea;
	for (int i = 0; i < nCnt; i++)
	{
		ServerLine* pSLine	= (ServerLine*)m_pList->Items[i];
		if (pSLine&&pSLine->nSvrID < 100)
			continue;

		if (g_nClientAreaLock == 1 && pSLine->nSvrID/100 < 10)
			continue;

		if (g_nClientAreaLock == 0 && pSLine->nSvrID/100 > 10)
			continue;

		bool bSame = false;
		for (list<int>::iterator itor = listArea.begin(); itor != listArea.end(); itor++)
		{
			if ((*itor) == pSLine->nSvrID/100)
				bSame = true;
		}
		if (!bSame)
			listArea.push_back(pSLine->nSvrID/100);
	}

	return (listArea.size()>0?(listArea.size()):1);

}

void ServerLineCfg::GetSvrListByArea(xnList* pList,BYTE ByArea)
{
	if (m_pList->Count == 0)
		return;

	for (int i = 0; i < m_pList->Count; i++)
	{
		ServerLine* pSLine	= (ServerLine*)m_pList->Items[i];
		if (pSLine&&pSLine->nSvrID/100 == ByArea)
		{
			pList->Add((void*)pSLine);
		}
	}
}

void SelLineEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	SelLineMenu* pMenu	= (SelLineMenu*)param;
	if (pMenu)
	{
		ServerLine* pSLine	= (ServerLine*)pEvent->pComponent->GetTag();
		if (pSLine)
		{
			pMenu->SetSelLine(pSLine->nSvrID);
		}
	}
}

void SelAreaEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	SelLineMenu* pMenu	= (SelLineMenu*)param;
	if (pMenu)
	{
		int nArea = (int)pEvent->pComponent->GetTag();
		if (nArea)
		{
			pMenu->SetSelLine(nArea*100 + 1);
		}
	}
}

SelLineMenu::SelLineMenu()
{
	m_pForm		= pMainInterface->GetDForm("xuanxian");
	m_pFormEx   = pMainInterface->GetDForm("denglu2");
	bArea = 1;
	nSelIndex	= -1;
	m_nMaxSrvCnt	= s_nMaxLine;

	m_pStateList = xnList::Create();

//------------新选服界面---------------------//
	char	szTmp[32];
	for (int i = 0; i < 9; i++)
	{
		sprintf(szTmp,"x%d",i+1);
		m_ppCompBtn[i]	= m_pForm->GetComponent(szTmp);
		m_ppCompBtn[i]->SetEvent(SelLineEvent, this);
		sprintf(szTmp,"n%d",i+1);
		m_ppCompBack[i]	= m_pForm->GetComponent(szTmp);
		sprintf(szTmp,"r%d",i+1);
		m_ppCompState[i]	= m_pForm->GetComponent(szTmp);
	}

//--------------------------------------------

	m_pFormNotice = pMainInterface->GetDForm("gengxin");
	strNotice.assign("");

	m_pView       = m_pFormNotice->GetComponent("txt");
	m_pView->pSTextCenter->SetSlide(m_pFormNotice->GetComponent("move"),m_pFormNotice->GetComponent("move1"),false);

	SetMaxLine(1);
}

SelLineMenu::~SelLineMenu()
{
	m_pForm->Close();

	while(m_pStateList->Count)
		m_pStateList->Delete(0);
	m_pStateList->Free();

	//while(m_pAreaNameList->Count)
	//	m_pAreaNameList->Delete(0);
	//m_pAreaNameList->Free();
}

void	SelLineMenu::Close()
{
	m_pForm->Close();
	m_pFormEx->Close();
	m_pFormNotice->Close();
}

void SelLineMenu::ShowNotice()
{
	if (strNotice.size())
	{
		m_pFormNotice->Open();
	}
}

int SelLineMenu::GetLineState(int nLine)
{
	if (m_pStateList->Count == 0)
		return 0;

	for (int i = 0; i < m_pStateList->Count; i++)
	{
		int nTmpState = (int)m_pStateList->Items[i];
		if (nTmpState&&((nTmpState>>16)==nLine))
			return (nTmpState&0x0f);
	}
	return 0;
}

//char* SelLineMenu::GetAreaName(BYTE byArea)
//{
//	if (byArea < 0 || byArea > m_pAreaNameList->Count)
//		return "";
//
//	return (char*)m_pAreaNameList->Items[byArea];
//}



void SelLineMenu::SetSelLine( int nIndex )
{
	if(nIndex >= 100)
		bArea = nIndex/100;
	else
		bArea = 1;
	RefreshLineByArea(bArea);
	for (int i = 0; i < 9; i++)
	{
		m_ppCompBtn[i]->SetAddSprByTag("",0,0,1);
		ServerLine* pSvrLine = (ServerLine*)m_ppCompBtn[i]->GetTag();
		if (pSvrLine && pSvrLine->nSvrID == nIndex)
			m_ppCompBtn[i]->SetAddSprByTag("MB_XUANZHONG.png",-16,16,1);
	}
	
	nSelIndex	= nIndex%100;
	m_pSelLine	= ServerLineCfg::GetInstance().GetLine(bArea,nSelIndex);
	if (m_pSelLine)
	{
		char szTmp[32];
		sprintf(szTmp,"X_QU%d.png",(bArea-1));
		m_pFormEx->GetComponent("qu")->SetViewSpr(szTmp);
		m_pFormEx->GetComponent("1n")->SetViewSpr(m_pSelLine->szSpr);
		sprintf(szTmp,"MB_QU%d.png",GetLineState(m_pSelLine->nSvrID)+1);
		m_pFormEx->GetComponent("r1")->SetViewSpr(szTmp);
	}
}

void SelLineMenu::SetMaxLine( int nMaxCnt)
{
	s_nMaxLine	= nMaxCnt;
	m_nMaxSrvCnt = nMaxCnt;

	RefreshLineByArea(1);
}

void SelLineMenu::RefreshArea(BYTE byArea)
{
	char szTmp[32];
	DComponent* pComp = NULL;
	for (int i = 0; i < 20; i++)
	{
		sprintf(szTmp,"q%d",i+1);
		pComp = m_pForm->GetComponent(szTmp);
		if (pComp)
			pComp->SetVisible(false);

		sprintf(szTmp,"qu%d",i+1);
		pComp = m_pForm->GetComponent(szTmp);
		if (pComp)
			pComp->SetViewSpr("");
	}

	xnList* pAreaList = xnList::Create();
	ServerLineCfg::GetInstance().GetAreaList(pAreaList,g_nClientAreaLock);
	if (pAreaList->Count)
	{
		for (int j = 0; j < pAreaList->Count; j++)
		{
			sprintf(szTmp,"q%d",j+1);
			pComp = m_pForm->GetComponent(szTmp);
			if (pComp)
			{
				pComp->SetVisible(true);
				pComp->SetSprColor(125,125,125);
				pComp->SetTag(pAreaList->Items[j]);
				pComp->SetEvent(SelAreaEvent, this);

				if (byArea == (int)pAreaList->Items[j])
					pComp->SetSprColor(255,255,255);
			}

			sprintf(szTmp,"qu%d",j+1);
			pComp = m_pForm->GetComponent(szTmp);
			if (pComp)
			{
				sprintf(szTmp,"X_QU%d.png",j);
				pComp->SetViewSpr(szTmp);
				pComp->SetSprColor(125,125,125);

				if (byArea == (int)pAreaList->Items[j])
					pComp->SetSprColor(255,255,255);
			}
		}
	}
	pAreaList->Free();
}

void SelLineMenu::RefreshLineByArea(BYTE byArea)
{
	RefreshArea(byArea);

	for (int i = 0; i < 9; i++)
	{
		m_ppCompBtn[i]->SetVisible(false);
		m_ppCompBack[i]->SetViewSpr("");
		m_ppCompState[i]->SetVisible(false);
	}

	xnList* pList = xnList::Create();
	ServerLineCfg::GetInstance().GetSvrListByArea(pList,bArea);
	if (pList->Count)
	{
		char szTmp[32];
		for (int k = 0; k < pList->Count; k++)
		{
			ServerLine* pSLine	= (ServerLine*)pList->Items[k];
			if (pSLine && pSLine->nSvrID)
			{
				m_ppCompBtn[k]->SetVisible(true);
				m_ppCompBtn[k]->SetTag(pSLine);

				m_ppCompBack[k]->SetViewSpr(pSLine->szSpr);

				m_ppCompState[k]->SetVisible(true);
				sprintf(szTmp,"MB_QU%d.png",GetLineState(pSLine->nSvrID)+1);
				m_ppCompState[k]->SetViewSpr(szTmp);
			}
		}
	}
	pList->Free();
}