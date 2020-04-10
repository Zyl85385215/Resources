#include "UserData.h"
#include "VipMenu.h"
#include "VipConfig.h"
#include "MainMenu.h"
#include "ios_user_packet.h"
#include "LoginScene.h"
void ClickVIPOKButton(InterfaceEvent* pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		//pChar->SendCmd(USER,C_REQUEST_VIPTEST);
		VipMenu* pMenu = (VipMenu*)param;
		pMenu->Close();
		pMainMenu->m_pRechargeMenu->OpenForm(2);
	}
}

void VIPStepEvent(int nStep)
{
	if(nStep >= 0 && pMainMenu->m_pVipMenu->byPage != nStep+1)
	{
		pMainMenu->m_pVipMenu->byPage = nStep+1;
		pMainMenu->m_pVipMenu->ResetPoint();
	}
}

void ClickVIPIconButton(InterfaceEvent* pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		VipMenu* pMenu = (VipMenu*)param;
		ItemExBase* pBaseItem = ItemConfig::GetInstance().GetItemBase(46010+pMenu->byPage-1);
		if(pBaseItem)
			pMainMenu->m_pItemInfoMenu->Open(pBaseItem,FORM_NULL);
	}
}

void ClickGetVIPGiftButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		VipMenu* pMenu = (VipMenu*)param;
		BuildPacketEx(pPacket,USER,C_REQUEST_VIPGIFT,buf,1024);
		SET_DATA(pData,USER,C_REQUEST_VIPGIFT,pPacket);
		pData->nLv = pMenu->byPage;
		pChar->SendPacket(pPacket);
	}
}

void ClickGetVIPPowerButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		if (pChar->m_dwPow > 49)
		{
			pMainMenu->PopTipMsg("体力值已满，无法领取");
			return;
		}
		pChar->SendCmd(USER,C_REQUEST_VIPPOWER);
	}
}

static void ClickMoveCFramPosY(InterfaceEvent* pEvent, void* param)
{
	if (param)
	{
		VipMenu* pMenu = (VipMenu*)param;

		switch(pEvent->event)
		{
		case INTERFACE_EVENT_DOWN:
		case INTERFACE_EVENT_MOVE:
			{
				int	nChgTo	= pEvent->y + pMenu->m_pSlideBack->nY;
				if(nChgTo < pMenu->m_pSlideBack->nY)
					nChgTo	= pMenu->m_pSlideBack->nY;
				if(nChgTo > pMenu->m_pSlideBack->nY + pMenu->m_pSlideBack->nH-10)
					nChgTo	= pMenu->m_pSlideBack->nY + pMenu->m_pSlideBack->nH-10;
				pMenu->m_pSlideButton->SetPos(pMenu->m_pSlideButton->nX,nChgTo);
				pMenu->MoveSetCFramPosY();
			}
			break;
		}
	}
}

VipMenu::VipMenu()
{
	m_pForm = pMainInterface->GetDForm("VIP");
	byCurPage = 0;
	byPage = 1;
	DComponent* pBase = m_pForm->GetComponent("k1");
	pBase->nH	= (pBase->nH+1)/2*2;
	//DComponent* pComp[12];
	//for(int i = 0; i < 12; i ++)
	//{
	//	char szName[32];
	//	sprintf(szName,"miaoshu%d",i);
	//	pComp[i] = m_pForm->GetComponent(szName);
	//}
	DComponent* pComp[1];
	pComp[0] = m_pForm->GetComponent("k2");
	m_pForm->pCFrame = new CCtrlFrame(pBase->nX,pBase->nY,pBase->nW,pBase->nH+4);
	m_pForm->pCFrame->InitFParts(pBase,pComp,1);
	m_pForm->pCFrame->nDisStep = pBase->nH;
	//m_pForm->pCFrame->SetFPCnt(0,true);
	m_pForm->pCFrame->SetStepEvent(VIPStepEvent);
	m_pForm->GetComponent("cz")->SetEvent(ClickVIPOKButton,this);
	m_pForm->GetComponent("left")->SetEvent(MoveFrmLeft,m_pForm->pCFrame);
	m_pForm->GetComponent("right")->SetEvent(MoveFrmRight,m_pForm->pCFrame);
	//m_pForm->GetComponent("icon")->SetEvent(ClickVIPIconButton,this);
	m_pForm->GetComponent("libao")->SetEvent(ClickGetVIPGiftButton,this);
	m_pForm->GetComponent("tili")->SetEvent(ClickGetVIPPowerButton,this);

	char szTmp[32];
	for (int j = 0; j < 7; j++)
	{
		sprintf(szTmp,"icon%d",j+1);
		m_pAwardIcon[j] = m_pForm->GetComponent(szTmp);
		sprintf(szTmp,"gezi%d",j+1);
		m_pAwardBack[j] = m_pForm->GetComponent(szTmp);
	}

	m_pSlideButton = m_pForm->GetComponent("move1");
	m_pSlideBack   = m_pForm->GetComponent("move");
	m_pSlideBack->SetEvent(ClickMoveCFramPosY,this);
}

VipMenu::~VipMenu()
{

}

void VipMenu::Open()
{	
	byCurPage = pChar->m_byVipLvl;
	if(byCurPage == 0)
		byCurPage = 1;
	byPage = byCurPage;
	Refresh();
	m_pForm->Open(2);
}

void VipMenu::Close()
{
	m_pForm->Close();
}

void VipMenu::MoveSetCFramPosY()
{
	if(m_pForm->pCFrame && m_pForm->pCFrame->pFrame)
	{
		FrameLayer* pFrm = (FrameLayer*)m_pForm->pCFrame->pFrame;
		int nSlide = (m_pSlideButton->nY - m_pSlideBack->nY)*100/m_pSlideBack->nH;
		int nChgY  = m_pForm->pCFrame->nFrmH*m_pForm->pCFrame->GetFPCnt()*nSlide/100 - m_pForm->pCFrame->nFrmH;
		pFrm->setPositionY(nChgY/POINT_PIX_CNT);
	}
}

void VipMenu::Update()
{
	if(pChar && pChar->m_bVIPCh)
	{
		pChar->m_bVIPCh = false;
		Refresh();
	}
	if(m_pForm->pCFrame && m_pForm->pCFrame->pFrame)
	{
		FrameLayer* pFrm = (FrameLayer*)m_pForm->pCFrame->pFrame;
		int nDelta = pFrm->getPositionY()*POINT_PIX_CNT+m_pForm->pCFrame->nFrmY;
		if((nDelta + m_pForm->pCFrame->nFrmH/2)/(m_pForm->pCFrame->nFrmH) + 1!= byPage)
		{
			byPage = (nDelta + m_pForm->pCFrame->nFrmH/2)/m_pForm->pCFrame->nFrmH + 1;
			ResetPoint();
		}

		int nSlide = nDelta*100/(m_pForm->pCFrame->nFrmH*m_pForm->pCFrame->GetFPCnt());
		int nPos	= nSlide*m_pSlideBack->nH/100;
		m_pSlideButton->SetPos (m_pSlideButton->nX,m_pSlideBack->nY+nPos);
	}

	VipInfoObj* pMyObj = VipConfig::GetInstance().GetObjByLv(pChar->m_byVipLvl);

	DComponent* pCompTemp = m_pForm->GetComponent("tili");
	DComponent* pCompTemp2= m_pForm->GetComponent("yi2");
	if(pMyObj && pMyObj->m_nPoweradd > 0)
	{
		int nFlag = pChar->m_pClientFlag->GetFlag(17);
		if(nFlag == 0)
		{
			pCompTemp->SetVisible(true);
			pCompTemp2->SetVisible(false);
		}
		else
		{
			pCompTemp->SetVisible(true);
			pCompTemp2->SetVisible(true);
		}
	}
	else
	{
		pCompTemp->SetVisible(false);
		pCompTemp2->SetVisible(false);
	}
	pCompTemp = m_pForm->GetComponent("libao");
	pCompTemp2= m_pForm->GetComponent("yi1");
	bool ifShow = false;
	bool ifShow2= false;
	int nFlag = pChar->m_pClientFlag->GetFlag(16);
	int nCheck = 0x01;
	if(byPage > 1)
		nCheck = nCheck << (byPage-1);
	if(byPage > 0&&byPage <= pChar->m_byVipLvl)
	{
		if(((nFlag & nCheck) != nCheck))
		{
			ifShow = true;
			ifShow2 = false;
		}
		else
		{
			ifShow = true;
			ifShow2 = true;
		}
	}
	pCompTemp->SetVisible(ifShow);
	pCompTemp2->SetVisible(ifShow2);
}

void VipMenu::ResetPoint()
{
	//for(int i = 0; i < 8 ; i ++)
	//{
	//	char szName[8];
	//	sprintf(szName,"p%d",i+1);
	//	m_pForm->GetComponent(szName)->SetViewSpr( (i+1)== byPage?(char*)"MB_DIAN_B.png":(char*)"MB_DIAN_S.png");
	//}

	if(byPage > 0)
	{
		char szName[32];
		sprintf(szName,"ZB_VIP%d.png",byPage);
		m_pForm->GetComponent("z")->SetViewSpr(szName);

		for (int j = 0; j < 7; j++)
		{
			m_pAwardIcon[j]->SetViewSpr("");
			m_pAwardIcon[j]->SetCaption("",1);
			m_pAwardIcon[j]->ClearEffect();
			m_pAwardIcon[j]->SetEvent(NULL);
			m_pAwardBack[j]->SetVisible(false);
		}

		VipAward* pObj = VipAwardConfig::GetInstance().GetObjByVipLv(byPage);
		if (!pObj)
			return;


		for (int i = 0; i < pObj->pList->Count; i++)
		{
			int nIDandNum = (int)pObj->pList->Items[i];
			ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(nIDandNum/100);
			if(pBase)
			{
				if (pBase->nStar == 3)
					m_pAwardIcon[i]->SetEffect(238,2);
				ShowItemInfoOnComp(m_pAwardIcon[i],pBase->nTypeID,nIDandNum%100);
				m_pAwardBack[i]->SetVisible(true);
			}
		}

		m_pAwardIcon[pObj->pList->Count]->SetViewSpr("MB_MONEY1.png");
		m_pAwardIcon[pObj->pList->Count]->SetCaptionEx("%d",2,pObj->nGold);
		m_pAwardBack[pObj->pList->Count]->SetVisible(true);
	}
}

void VipMenu::Refresh()
{
//	if(m_pForm && m_pForm->bOpened)
	{
		if(byCurPage > VipConfig::GetInstance().GetMaxLvl())
			byCurPage = VipConfig::GetInstance().GetMaxLvl();
		else if(byCurPage == 0)
			byCurPage = 1;
		
		char szLvTxt[20];         //===zjk
		sprintf(szLvTxt, "MB_LV%d.png", pChar->m_byVipLvl);
		m_pForm->GetComponent("lv1")->SetViewSpr(szLvTxt);

		int nCnt = VipConfig::GetInstance().GetMaxLvl();
		m_pForm->pCFrame->SetFPCnt(nCnt);

		if(byCurPage > 0)
			m_pForm->pCFrame->nSetY = m_pForm->pCFrame->nFrmH*(byCurPage-1)+1;
		VipInfoObj* pMyObj = VipConfig::GetInstance().GetObjByLv(pChar->m_byVipLvl);
		BYTE szColor[2][3] = {{255,255,255,},
								{0,255,0,}};
		char szWords[32];
	
		for(int i = 0; i < nCnt; i ++)
		{
			//bool ifGreen = false;
			//int nIndex = 0;
			//VipInfoObj* pObj = VipConfig::GetInstance().GetObjByLv(i+1);
			//if(pObj == NULL)
			//	continue;
		
			//DComponent* pViewComp = m_pForm->pCFrame->GetFPComp(i,"miaoshu0");
			//pViewComp->SetSprColor(220,220,0);
			//pViewComp->SetCaptionEx("VIP%d特权",0,pObj->m_nLv);
			//for(int j = 0; j < 11; j ++)
			//{
			//	char szName[32];
			//	sprintf(szName,"miaoshu%d",j+1);
			//	pViewComp = m_pForm->pCFrame->GetFPComp(i,szName);
			//	if(j < pObj->vecdesc.size())
			//	{
			//		if(pMyObj == NULL || pMyObj->vecdesc.size() <= j || pObj->vecdesc[j].size() > pMyObj->vecdesc[j].size() || (pObj->vecdesc[j].size() == pMyObj->vecdesc[j].size() && strcmp(pObj->vecdesc[j].c_str(),pMyObj->vecdesc[j].c_str()) > 0) )
			//			ifGreen = true;
			//		else
			//			ifGreen = false;
			//		pViewComp->SetSprColor(szColor[ifGreen][0],szColor[ifGreen][1],szColor[ifGreen][2]);
			//		pViewComp->SetCaptionEx("%d.%s",0,j+1,pObj->vecdesc[j].c_str());
			//	}
			//	else
			//	{
			//		pViewComp->SetCaption("",1);
			//	}
			//}
			sprintf(szWords,"MB_VIP%d.png",i+1);
			m_pForm->pCFrame->GetFPComp(i,"k2")->SetViewSpr(szWords);
		}
		VipInfoObj* pNext = VipConfig::GetInstance().GetObjByLv(pChar->m_byVipLvl+1);
		if(pNext)
		{
			int nDelta = pNext->m_nNeedgold - pChar->m_dwVipExp;
			if(nDelta < 0)
				nDelta = 0;
			m_pForm->GetComponent("money")->SetCaptionEx("%d",1,nDelta);
			//m_pForm->GetComponent("lv2")->SetCaptionEx("VIP%d",1,pChar->m_byVipLvl+1);
			sprintf(szWords,"BT_VIP%d.png",pChar->m_byVipLvl+1);
			m_pForm->GetComponent("lv2")->SetViewSpr(szWords);
			m_pForm->GetComponent("lv2")->SetCaption("",0);
			m_pForm->GetComponent("jd")->SetProgress(pChar->m_dwVipExp,pNext->m_nNeedgold);
		}
		else{
			m_pForm->GetComponent("money")->SetCaption("0",1);
			m_pForm->GetComponent("lv2")->SetCaption("未开放",1);
			m_pForm->GetComponent("lv2")->SetViewSpr("");
			m_pForm->GetComponent("jd")->SetProgress(100,100,"999/999");
		}
		ResetPoint();
	}
}

char* RechargeZuan[] = {"7776","3772","2178","780","300","60",};
int  RechargeRmb[] = {648,328,198,78,30,6,1};

void RqGoPayEvent(int nPos,char* szName,void* param)
{
	char	szOrder[128]	= {0};
	sprintf(szOrder,"%s-%d-%d-%d",PACK_VERSION,g_nServerID,pChar->m_dwUin,xnGetSecCount());

	if(g_pTPObj->TP_GetPlat() == 35)
		g_pTPObj->TP_OtherEvent(pChar->m_dwRmb,"");

	g_pTPObj->TP_RqPay(RechargeRmb[nPos],g_nServerID,g_szLoginAcc,szOrder);
}

RechargeMenu::RechargeMenu()
{
	m_pForm = pMainInterface->GetDForm("chongzhi");

	DComponent*	pCompBase	= m_pForm->GetComponent("cz");
	m_pForm->pCFrame	= new CCtrlFrame(pCompBase->nX,pCompBase->nY,pCompBase->nW,(pCompBase->nH+5)*6);
	DComponent*	ppCompPt[4];
	ppCompPt[0]		= m_pForm->GetComponent("ds");
	ppCompPt[1]		= m_pForm->GetComponent("zuan");
	ppCompPt[2]		= m_pForm->GetComponent("rmb");
	ppCompPt[3]		= m_pForm->GetComponent("mai");
	m_pForm->pCFrame->InitFParts(pCompBase,ppCompPt,4);
	m_pForm->pCFrame->nSpace	= 5;
	m_pForm->pCFrame->nDisStep	= pCompBase->nH+5;

	m_pForm->pCFrame->SetFPCnt(6,false);
	m_pForm->pCFrame->SetEventEx(RqGoPayEvent,this);
}

RechargeMenu::~RechargeMenu()
{

}

void RechargeMenu::OpenForm(int nType)
{
	m_pForm->Open(nType);
	m_pForm->pCFrame->nChgY = 0;
	for (int i = 0; i < 6 ; i++)
	{
		if (i == 0)
		{
			m_pForm->pCFrame->GetFPComp(i,"ds")->SetViewSpr("MB_CZ_DUOSONG2.png");
		}else if (i == 1)
		{
			m_pForm->pCFrame->GetFPComp(i,"ds")->SetViewSpr("MB_CZ_DUOSONG1.png");
		}else if (i == 2)
		{
			m_pForm->pCFrame->GetFPComp(i,"ds")->SetViewSpr("MB_CZ_DUOSONG0.png");
		}else
		{
			m_pForm->pCFrame->GetFPComp(i,"ds")->SetViewSpr("");
		}
		m_pForm->pCFrame->GetFPComp(i,"zuan")->SetCaptionEx("%s",0,RechargeZuan[i]);
		m_pForm->pCFrame->GetFPComp(i,"rmb")->SetCaptionEx("%d.00",0,RechargeRmb[i]);
	}

	m_pForm->GetComponent("zuan1")->SetCaptionEx("%d",0,pChar->m_dwRmb);

}

void RechargeMenu::Update()
{
	if(m_pForm->bOpened == false)
		return;
	m_pForm->GetComponent("zuan1")->SetCaptionEx("%d",0,pChar->m_dwRmb);
	m_pForm->GetComponent("z")->SetVisible(!pChar->m_pRechargeAwardCenter->m_ifGet[0]);
}