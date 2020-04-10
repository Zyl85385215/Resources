#include "MainMenu.h"
#include "UserData.h"
#include "LuckBetMenu.h"
#include "ios_user_packet.h"
#include "SoundCenter.h"

void LBStartEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	LuckBetMenu*	pMenu	= (LuckBetMenu*)param;

	pMenu->RqStart();
}

void LBBetEvent(InterfaceEvent * pEvent,void * param)
{
	LuckBetMenu*	pMenu	= (LuckBetMenu*)param;
	
	if(pMenu->m_dwRunTick)
		return;	//开始了就不能操作了

	int nPos	= 0;
	sscanf(pEvent->pComponent->szName,"bt%d",&nPos);
	nPos--;
	switch(pEvent->event)
	{
	case INTERFACE_EVENT_DOWN:
		pMenu->m_dwTickBetHold	= xnGetTickCount();
		pMenu->m_bySelBet	= nPos;
		break;
	case INTERFACE_EVENT_UP:
		pMenu->m_dwTickBetHold	= 0;
		break;
	case INTERFACE_EVENT_CLICK:
		pMenu->SetBetVal(nPos,pMenu->m_sznPutVal[nPos]+1);
		break;
	}
}
void LBGetRtlEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	LuckBetMenu* pMenu	= (LuckBetMenu*)param;
	pChar->SendCmd(USER,C_GET_LUCK_AWARD);
	pMenu->m_pFormRtl->Close();
}

void LBBetMinEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	LuckBetMenu* pMenu	= (LuckBetMenu*)param;
	pMenu->m_byExSel	= 1;
	pMenu->m_dwTickExStart	= xnGetTickCount();
}
void LBBetMaxEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	LuckBetMenu* pMenu	= (LuckBetMenu*)param;
	pMenu->m_byExSel	= 2;
	pMenu->m_dwTickExStart	= xnGetTickCount();
}

LuckBetMenu::LuckBetMenu()
{
	m_pForm		= pMainInterface->GetDForm("kuangshiji");
	m_pFormRtl	= pMainInterface->GetDForm("kuodashouyi");
	char	szTmp[64];
	for (int i = 0; i < 24; i++)
	{
		sprintf(szTmp,"%d",i+1);
		m_ppCompBox[i]	= m_pForm->GetComponent(szTmp);
	}

	for (int i = 0; i < 8; i++)
	{
		sprintf(szTmp,"x%d",i+1);
		m_ppCompBetRtl[i]	= m_pFormRtl->GetComponent(szTmp);
		sprintf(szTmp,"s%d",i+1);
		m_ppCompMyCoin[i]	= m_pFormRtl->GetComponent(szTmp);
	}
	m_pFormRtl->GetComponent("end")->SetEvent(LBGetRtlEvent,this);
	m_pFormRtl->GetComponent("xia")->SetEvent(LBBetMinEvent,this);
	m_pFormRtl->GetComponent("da")->SetEvent(LBBetMaxEvent,this);
	
	m_pCompIco	= m_pForm->GetComponent("tu");
	m_pCompNum	= m_pForm->GetComponent("num");

	m_pForm->GetComponent("kaishi")->SetEvent(LBStartEvent, this);

	for (int i = 0; i < 8; i++)
	{
		sprintf(szTmp,"bt%d", i+1);
		m_pForm->GetComponent(szTmp)->SetEvent(LBBetEvent,this);
	}

	for (int i = 0; i < 3; i++)
	{
		sprintf(szTmp,"a%d", i+1);
		m_ppCompBetNum[0][i]	= m_pForm->GetComponent(szTmp);
		sprintf(szTmp,"b%d", i+1);
		m_ppCompBetNum[1][i]	= m_pForm->GetComponent(szTmp);
		sprintf(szTmp,"c%d", i+1);
		m_ppCompBetNum[2][i]	= m_pForm->GetComponent(szTmp);
		sprintf(szTmp,"d%d", i+1);
		m_ppCompBetNum[3][i]	= m_pForm->GetComponent(szTmp);
		sprintf(szTmp,"e%d", i+1);
		m_ppCompBetNum[4][i]	= m_pForm->GetComponent(szTmp);
		sprintf(szTmp,"f%d", i+1);
		m_ppCompBetNum[5][i]	= m_pForm->GetComponent(szTmp);
		sprintf(szTmp,"h%d", i+1);
		m_ppCompBetNum[6][i]	= m_pForm->GetComponent(szTmp);
		sprintf(szTmp,"i%d", i+1);
		m_ppCompBetNum[7][i]	= m_pForm->GetComponent(szTmp);
	}

	m_dwTickBetHold	= 0;
	m_dwTickExStart	= 0;

	DComponent* ppComp[2];
	ppComp[0]		= m_pForm->GetComponent("g1");
	ppComp[1]		= m_pForm->GetComponent("g2");
	m_pCRadioSel	= new CCtrlRadio(ppComp,2);

	m_pCRadioSel->SetSel(1);
	m_byBetType		= 1;
	m_nTotalUse		= 0;
	m_byRunPos		= 1;
	m_dwRunTick		= 0;
	m_pCompIco->SetViewSpr("MB_MONEY.png");
}

LuckBetMenu::~LuckBetMenu()
{

}

void LuckBetMenu::OpenForm()
{
	//重置界面
	for (int i = 0; i < 8; i++)
	{
		SetBetVal(i,0);
	}
	m_nTotalUse	= 0;
	m_ppCompBox[m_byRunPos-1]->SetAddSprByTag("",0,0,1);
	m_byRunPos	= rand()%24+1;
	m_dwRunTick	= 0;
	m_byExSel	= 0;
	m_pForm->Open(2);
}

void LuckBetMenu::Update()
{
	if(m_pForm->bOpened == false)
		return;

	int	nRemain	= 0;
	if(m_byBetType == 1)
		nRemain	= pChar->m_dwMoney-m_nTotalUse;
	else
		nRemain	= pChar->m_dwRmb-m_nTotalUse;

	if(m_byBetType != m_pCRadioSel->GetSel() || nRemain < 0)
	{
		m_byBetType	= m_pCRadioSel->GetSel();
		if(m_byBetType == 1)
		{
			m_pCompIco->SetViewSpr("MB_MONEY.png");
		}
		else
			m_pCompIco->SetViewSpr("MB_ZUANSHI.png");

		for (int i = 0; i < 8; i++)
		{
			SetBetVal(i,0);
		}
		nRemain	+= m_nTotalUse;
		m_nTotalUse	= 0;

	}

	m_pCompNum->SetCaptionEx("%d",1,nRemain);

	if(m_dwTickBetHold && xnGetTickCount() - m_dwTickBetHold > 500)
	{
		int	nPassTick	= xnGetTickCount() - m_dwTickBetHold - 500;
		if(nPassTick /50 % 4 == 0)
		{
			SetBetVal(m_bySelBet,m_sznPutVal[m_bySelBet]+nPassTick/100);
		}
	}

	if(m_byExSel)
	{

		if(m_dwTickExStart && xnGetTickCount() - m_dwTickExStart > 3000)
		{
			m_dwTickExStart		= 0;
			BuildPacketEx(pPacket, USER, C_RQ_LUCKBET_EX,buf, 256);
			SET_DATA(pSend, USER, C_RQ_LUCKBET_EX, pPacket);
			pSend->bySide	= m_byExSel-1;
			pSend->nExVal	= 0;
			pChar->SendPacket(pPacket);
		}
		g_pSoundCenter->PlayerEffect("Sound/zhuan");
		char	szTmp[32];
		BYTE bRtl	= rand()%14;
		sprintf(szTmp,"MB_DX%d.png", bRtl%10);
		m_pFormRtl->GetComponent("t2")->SetViewSpr(szTmp);
		sprintf(szTmp,"MB_DX%d.png", bRtl/10);
		m_pFormRtl->GetComponent("t1")->SetViewSpr(szTmp);
	}

	static	DWORD	s_LPdwEffPos	= xnGetTickCount();
	if(m_dwRunTick)
	{
		int nTickPass	= xnGetTickCount() - m_dwRunTick;
		int nSpeed	= 20;

		if(m_byEndPos && m_byStep == 1)
		{
			nSpeed	+= nTickPass/6;

			if(nTickPass > 1000 && m_byRunPos == m_byEndPos)
			{//摇号结束
				m_byStep	= 2;
				m_dwRunTick	= 0;

				if(m_nTotalGetVal != 0)
				{
					g_pSoundCenter->PlayerEffect("Sound/bswin");
					m_pFormRtl->Open(2);
					m_pFormRtl->GetComponent("t2")->SetViewSpr("MB_DX7.png");
					m_pFormRtl->GetComponent("t1")->SetViewSpr("MB_DX0.png");
					m_byExSel	= 0;
				}
				else
				{
					g_pSoundCenter->PlayerEffect("Sound/nothing");
				}

				m_pForm->GetComponent("kaishi")->SetEnable(true);
				m_pForm->GetComponent("close")->SetEnable(true);
				
				//pChar->SendCmd(USER,C_GET_LUCK_AWARD);
			}
		}
		else
		{
			if(nTickPass < 1000)
			{
				nSpeed	+= 80-80*nTickPass/1000;
			}
		}

		if(xnGetTickCount() - s_LPdwEffPos > nSpeed)
		{
			s_LPdwEffPos	= xnGetTickCount();
			m_ppCompBox[m_byRunPos-1]->SetAddSprByTag("",0,0,1);
			m_byRunPos++;
			g_pSoundCenter->PlayerEffect("Sound/zhuan");
			if(m_byRunPos > 24)
				m_byRunPos	= 1;

			if(m_byStep == 0 && m_byEndPos && (m_byRunPos+16)%24+1 == m_byEndPos && nTickPass > 2200)
			{
				m_byStep	= 1;
				m_dwRunTick	= xnGetTickCount();
			}
			
		}
	}

	m_ppCompBox[m_byRunPos-1]->SetAddSprByTag("MB_WK_XUAN.png",-30,-30,1);
}

void RechargeReturn2Event(void* param,int nVal)
{
	if(nVal == 1)
	{
		pMainMenu->m_pRechargeMenu->OpenForm(2);
	}
}

void LuckBetMenu::RqStart()
{
	m_byExSel	= 0;
	m_nTotalGetVal	= 0;
	if(m_nTotalUse == 0)
	{
		pMainMenu->PopTipMsg("请选择想要挖的矿种");
		return;
	}

	if(m_byBetType == 1)
	{
		if(m_nTotalUse > pChar->m_dwMoney)
		{
			pMainMenu->PopTipMsg("金币不足");
			return;
		}
	}
	else
	{
		if(m_nTotalUse > pChar->m_dwRmb)
		{
			pMainMenu->PopAskMsgBox("钻石不足，是否前往充值?",RechargeReturn2Event,(void*)0,3);
			return;
		}
	}


	//判断条件, 发包请求结果
	m_dwRunTick	= xnGetTickCount();
	m_byEndPos	= 0;
	m_byStep	= 0;
	m_pForm->GetComponent("kaishi")->SetEnable(false);
	m_pForm->GetComponent("close")->SetEnable(false);
	BuildPacketEx(pPacket, USER, C_REQUEST_LUCKBET,buf,256);
	SET_DATA(pSend, USER, C_REQUEST_LUCKBET,pPacket);
	pSend->byCoinType	= m_byBetType;
	for (int i = 0; i < 8; i++)
	{
		pSend->wVal[i]	= m_sznPutVal[i];
	}
	pChar->SendPacket(pPacket);
}

void LuckBetMenu::GetResult( BYTE byEndPos, int nGetVal )
{
	m_nTotalGetVal	= nGetVal;
	m_byEndPos	= byEndPos;	//1-24
	SetExBetView(nGetVal);
}

void LuckBetMenu::SetBetVal( BYTE byType, int nVal )
{
	if(byType >= 8)
		return;
	if(nVal > 999)
		nVal	= 999;

	if(nVal != 0)
	{
		int nAddVal	= nVal-m_sznPutVal[byType];
		if(m_byBetType == 1)
		{
			if(m_nTotalUse+nAddVal > pChar->m_dwMoney)
				nAddVal	= pChar->m_dwMoney-m_nTotalUse;
		}
		else
		{
			if(m_nTotalUse+nAddVal > pChar->m_dwRmb)
				nAddVal	= pChar->m_dwRmb-m_nTotalUse;
		}

		m_sznPutVal[byType]	+= nAddVal;
		m_nTotalUse	+= nAddVal;
	}
	else
		m_sznPutVal[byType]	= 0;

	char	szTmp[32];
	sprintf(szTmp,"MB_WK%d.png", m_sznPutVal[byType]%10);
	m_ppCompBetNum[byType][2]->SetViewSpr(szTmp);
	sprintf(szTmp,"MB_WK%d.png", m_sznPutVal[byType]%100/10);
	m_ppCompBetNum[byType][1]->SetViewSpr(szTmp);
	sprintf(szTmp,"MB_WK%d.png", m_sznPutVal[byType]%1000/100);
	m_ppCompBetNum[byType][0]->SetViewSpr(szTmp);


	g_pSoundCenter->PlayerEffect("Sound/diu");
}

void LuckBetMenu::GetResultEx( BYTE byRtlVal, int nGetVal )
{
	m_dwTickExStart		= 0;
	m_nTotalGetVal	= nGetVal;
	m_byExSel		= 0;
	char	szTmp[32];
	sprintf(szTmp,"MB_DX%d.png", byRtlVal%10);
	m_pFormRtl->GetComponent("t2")->SetViewSpr(szTmp);
	sprintf(szTmp,"MB_DX%d.png", byRtlVal%100/10);
	m_pFormRtl->GetComponent("t1")->SetViewSpr(szTmp);
	
	if(nGetVal > 0)
	{
		g_pSoundCenter->PlayerEffect("Sound/bswin");
	}
	else
	{
		g_pSoundCenter->PlayerEffect("Sound/nothing");
	}
	SetExBetView(nGetVal);
}

void LuckBetMenu::SetExBetView( int nVal )
{
	if(nVal == 0)
	{
		m_pFormRtl->GetComponent("xia")->SetVisible(false);
		m_pFormRtl->GetComponent("da")->SetVisible(false);
		m_pFormRtl->GetComponent("close")->SetVisible(true);
		m_pFormRtl->GetComponent("end")->SetVisible(false);
	}
	else
	{
		m_pFormRtl->GetComponent("xia")->SetVisible(true);
		m_pFormRtl->GetComponent("da")->SetVisible(true);
		m_pFormRtl->GetComponent("close")->SetVisible(false);
		m_pFormRtl->GetComponent("end")->SetVisible(true);
	}

	char	szTmp[32];
	sprintf(szTmp,"MB_DX%d.png", nVal%10);
	m_ppCompBetRtl[7]->SetViewSpr(szTmp);
	sprintf(szTmp,"MB_DX%d.png", nVal%100/10);
	m_ppCompBetRtl[6]->SetViewSpr(szTmp);
	sprintf(szTmp,"MB_DX%d.png", nVal%1000/100);
	m_ppCompBetRtl[5]->SetViewSpr(szTmp);
	sprintf(szTmp,"MB_DX%d.png", nVal%10000/1000);
	m_ppCompBetRtl[4]->SetViewSpr(szTmp);
	sprintf(szTmp,"MB_DX%d.png", nVal%100000/10000);
	m_ppCompBetRtl[3]->SetViewSpr(szTmp);
	sprintf(szTmp,"MB_DX%d.png", nVal%1000000/100000);
	m_ppCompBetRtl[2]->SetViewSpr(szTmp);
	sprintf(szTmp,"MB_DX%d.png", nVal%10000000/1000000);
	m_ppCompBetRtl[1]->SetViewSpr(szTmp);
	sprintf(szTmp,"MB_DX%d.png", nVal%100000000/10000000);
	m_ppCompBetRtl[0]->SetViewSpr(szTmp);
}


