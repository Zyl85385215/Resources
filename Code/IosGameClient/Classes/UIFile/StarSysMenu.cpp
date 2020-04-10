#include "MainMenu.h"
#include "ios_user_packet.h"
#include "UserData.h"
#include "StarSysMenu.h"

void StarSysClick(int nPos,char* szName,void* param)
{
	StarSysMenu*	pMenu	= (StarSysMenu*)param;

	if(strncmp(szName,"q",1) == 0)
	{
		int nPos	= atoi(szName+1);
		pMenu->RqOpenStar(pMenu->m_nCurPage*10+nPos+10);
		pMenu->m_pCompHold	= pMenu->m_pForm->pCFrame->GetFPComp(pMenu->m_nCurPage,szName);
		pMenu->m_pCompHold->byRunAction	= 87;
		pMenu->m_bShowInfo	= true;
	}
	else
	{
		pMenu->m_pCompHold	= NULL;
		pMenu->m_bShowInfo	= false;
	}
}

StarSysMenu::StarSysMenu()
{
	m_pForm		= pMainInterface->GetDForm("xingchen");

	DComponent*	ppComps[19];
	DComponent* pCompBase	= m_pForm->GetComponent("kuang");

	char	szTmp[64];
	ppComps[9]	= m_pForm->GetComponent("q1");
	ppComps[9]->SetViewSpr("MB_XC_HUIQUAN.png");
	for (int i = 0; i < 9; i++)
	{

		sprintf(szTmp,"x%d",i+1);
		ppComps[i]	= m_pForm->GetComponent(szTmp);
		ppComps[i]->SetSprColor(100,100,100);
		sprintf(szTmp,"q%d",i+2);
		ppComps[i+10]	= m_pForm->GetComponent(szTmp);
		ppComps[i+10]->SetViewSpr("MB_XC_HUIQUAN.png");

	}
	m_pForm->pCFrame	= new CCtrlFrame(20,109,914,440);
	m_pForm->pCFrame->InitFParts(pCompBase,ppComps,19);
	m_pForm->pCFrame->nSpace	= 54;
	m_pForm->pCFrame->nDisStep	= 914;
	m_pForm->pCFrame->SetFPCnt(9,true);
	m_pForm->pCFrame->SetEventEx(StarSysClick,this);
	m_pForm->pCFrame->nZorder	= -1;
	m_pForm->GetComponent("left")->SetEvent(MoveFrmLeft,m_pForm->pCFrame);
	m_pForm->GetComponent("right")->SetEvent(MoveFrmRight,m_pForm->pCFrame);
	m_pForm->GetComponent("dengji")->SetSprColor(200,180,33);

	m_pCompHold	= NULL;
	m_nCurPage	= -1;
}

StarSysMenu::~StarSysMenu()
{

}
//
//void StarSysMenu::Open()
//{
//
//	m_bShowInfo	= false;
//	if (pChar->m_byLvl < 11)
//	{
//		pMainMenu->PopTipMsg("�ȼ��ﵽ11�������ǳ�ϵͳ");
//		return;
//	}
//	m_pForm->Open(2);
//
//
//}


bool StarSysMenu::CheckCanOpen()
{
	m_bShowInfo	= false;
	if (pChar->m_byLvl < 11)
	{
		pMainMenu->PopTipMsg("�ȼ��ﵽ11�������ǳ�ϵͳ");
		return false;
	}
	return true;
}


void StarSysMenu::Update()
{
	if(m_pForm->bOpened == false)
		return;
	m_pForm->pCFrame->SetFPCnt(pChar->m_byLvl/10,true);
	SetStarLv(pChar->m_wStarLv);
	int nRmPt	= pChar->m_byLvl;
	if(pChar->m_wStarLv < 11)
		nRmPt -= 10;
	else
		nRmPt -= pChar->m_wStarLv;
	if(nRmPt < 0)
		nRmPt	= 0;
	m_pForm->GetComponent("num")->SetCaptionEx("%d",1,nRmPt);

	int nPerJSVal = pMainMenu->m_pLifeStarSysMenu->GetLifeStarLvPvpDef();
	m_pForm->GetComponent("js")->SetCaptionEx("%d.%d%%", 1, nPerJSVal/10,nPerJSVal%10);

	int nPage	= -m_pForm->pCFrame->nOffChgPos/910;

	if(nPage <0 || nPage >= m_pForm->pCFrame->GetFPCnt())
		return;

	DWORD	dwTickCur	= xnGetTickCount();
	if(nPage != m_nCurPage)
	{
		for (int i = 0; i < 10; i++)
		{
			m_dwRandActTick[i]	= rand()%300+dwTickCur;
		}
		m_nCurPage	= nPage;
		m_bShowInfo	= false;
		m_pCompHold	= NULL;
	}

	m_pForm->GetComponent("right")->SetVisible(m_nCurPage < pChar->m_byLvl/10 - 1);
	m_pForm->GetComponent("dengji")->SetCaptionEx("%d--%d",1,(nPage+1)*10+1,(nPage+2)*10);

	
	char	szTmp[64];
	for (int i = 0; i < 10; i++)
	{
		sprintf(szTmp,"q%d",i+1);
		DComponent* pComp	= m_pForm->pCFrame->GetFPComp(nPage,szTmp);

		if(pComp && m_pCompHold != pComp)
		{//�������
			if(dwTickCur > m_dwRandActTick[i])
				pComp->byRunAction	= 80+(i+nPage)%5;
		}

		BYTE	byCompLv	= nPage*10+11+i;
		
		int nPos1	= byCompLv/10%10;
		int nPos2	= byCompLv%10;
		char	szTmp[64];
		if(nPos1 <= 3)
		{
			sprintf(szTmp,"MB_DT0%d.png",nPos1);
		}
		else
			sprintf(szTmp,"MB_DT%d.png",nPos1);

		pComp->SetAddSprByTag(szTmp,-4,16,1);
		if(nPos2 <= 3)
		{
			sprintf(szTmp,"MB_DT0%d.png",nPos2);
		}
		else
			sprintf(szTmp,"MB_DT%d.png",nPos2);

		pComp->SetAddSprByTag(szTmp,12,16,2);

		if(1/*m_bShowInfo*/)
		{
			StarLvObject* pStar	= StarSysCfg::GetInstance().GetStarInfo(byCompLv);
			if(pStar)
			{
				if(pComp->nY < 150)
				{//��
					if(strlen(pStar->szIco1) && strlen(pStar->szIco2))
					{
						pComp->SetAddSprByTag("MB_GEZI.png",-40,70,10);
						pComp->SetAddSprByTag(pStar->szIco1,-35,75,11);

						pComp->SetAddSprByTag("MB_GEZI.png",40,70,12);
						pComp->SetAddSprByTag(pStar->szIco2,45,75,13);
					}
					else if(strlen(pStar->szIco1))
					{
						pComp->SetAddSprByTag("MB_GEZI.png",0,70,10);
						pComp->SetAddSprByTag(pStar->szIco1,5,75,11);
					}
				}
				else
				{//��
					if(strlen(pStar->szIco1) && strlen(pStar->szIco2))
					{
						pComp->SetAddSprByTag("MB_GEZI.png",-40,-70,10);
						pComp->SetAddSprByTag(pStar->szIco1,-35,-65,11);

						pComp->SetAddSprByTag("MB_GEZI.png",40,-70,12);
						pComp->SetAddSprByTag(pStar->szIco2,45,-65,13);
					}
					else if(strlen(pStar->szIco1))
					{
						pComp->SetAddSprByTag("MB_GEZI.png",0,-70,10);
						pComp->SetAddSprByTag(pStar->szIco1,5,-65,11);
					}
				}
			}


		}
		else
		{
			//pComp->SetAddSprByTag("",-4,16,1);
			//pComp->SetAddSprByTag("",12,16,2);
			pComp->SetAddSprByTag("",0, 0,10);
			pComp->SetAddSprByTag("",0, 0,11);
			pComp->SetAddSprByTag("",0, 0,12);
			pComp->SetAddSprByTag("",0, 0,13);
		}

	}
	m_pForm->GetComponent("left")->SetVisible(nPage != 0);
	//m_pForm->GetComponent("right")->SetVisible(nPage != 8);
	m_pForm->pCFrame->CalcVisable();
}

void StarSysMenu::SetStarLv( BYTE bLv )
{
	if(bLv == 0 && pChar->m_byLvl > 10)
		bLv = 10;
	char	szTmp[64];
	for (int i = 1; i < (pChar->m_byLvl>=100?99:pChar->m_byLvl)/10+1; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			sprintf(szTmp,"q%d",j+1);
			DComponent* pComp	= m_pForm->pCFrame->GetFPComp(i-1,szTmp);
			if(i*10+j < bLv)
			{//�ѵ��
				StarLvObject* pSLv	= StarSysCfg::GetInstance().GetStarInfo(i*10+j+1);
				if(pSLv)
				{
					if(strlen(pSLv->szIco1))
					{
						pComp->SetViewSpr("MB_XC_DAQUAN.png");
						pComp->SetEffect(148,0);
					}
					else
					{
						pComp->SetViewSpr("MB_XC_XIAQUAN.png");
						pComp->SetEffect(155,0);
					}
				}
				sprintf(szTmp,"x%d",j+1);
				pComp	= m_pForm->pCFrame->GetFPComp(i-1,szTmp);
				if(pComp)
					pComp->SetSprColor(255,255,255);
			}
			else if(i*10+j == bLv)
			{//�ɵ��
				pComp->SetViewSpr("MB_XC_XIAQUAN.png");
				pComp->SetEffect(149,0);
			}
			else
			{
				pComp->SetViewSpr("MB_XC_HUIQUAN.png");
				pComp->SetEffect(0,0);
			}
		}
	}

	StarLvObject*	pStar	= StarSysCfg::GetInstance().GetStarInfo(bLv);
	if(pStar)
	{
		int nJob	= pChar->m_byJob-1;
		if(nJob >= MAXJOB)
			nJob	= 0;
		m_pForm->GetComponent("hp")->SetCaptionEx("%d",1,pStar->xParamTotal[nJob].nHpMax);
		m_pForm->GetComponent("sd")->SetCaptionEx("%d",1,pStar->xParamTotal[nJob].nMpMax);
		m_pForm->GetComponent("wg")->SetCaptionEx("%d",1,pStar->xParamTotal[nJob].nAtkW);
		m_pForm->GetComponent("mg")->SetCaptionEx("%d",1,pStar->xParamTotal[nJob].nAtkN);
		m_pForm->GetComponent("wf")->SetCaptionEx("%d",1,pStar->xParamTotal[nJob].nDefW);
		m_pForm->GetComponent("mf")->SetCaptionEx("%d",1,pStar->xParamTotal[nJob].nDefN);
		m_pForm->GetComponent("mz")->SetCaptionEx("%d",1,pStar->xParamTotal[nJob].nAim);
		m_pForm->GetComponent("sb")->SetCaptionEx("%d",1,pStar->xParamTotal[nJob].nDodge);
		m_pForm->GetComponent("bj")->SetCaptionEx("%d",1,pStar->xParamTotal[nJob].nCrit);
	}
	else
	{
		m_pForm->GetComponent("hp")->SetCaptionEx("%d",1,0);
		m_pForm->GetComponent("sd")->SetCaptionEx("%d",1,0);
		m_pForm->GetComponent("wg")->SetCaptionEx("%d",1,0);
		m_pForm->GetComponent("mg")->SetCaptionEx("%d",1,0);
		m_pForm->GetComponent("wf")->SetCaptionEx("%d",1,0);
		m_pForm->GetComponent("mf")->SetCaptionEx("%d",1,0);
		m_pForm->GetComponent("mz")->SetCaptionEx("%d",1,0);
		m_pForm->GetComponent("sb")->SetCaptionEx("%d",1,0);
		m_pForm->GetComponent("bj")->SetCaptionEx("%d",1,0);
	}
}

void UseGoldAndExp(void* param,int nVal)
{
	if(nVal == 1)
	{		
		pMainMenu->PopTipMsg("      �����ǳ��ɹ���");	

		char	szTmp[32];
		sprintf(szTmp,"q%d",pChar->m_wStarLv+1-pMainMenu->m_pStarSysMenu->m_nCurPage*10-10);
		pMainInterface->GetDForm("xingchen")->pCFrame->GetFPComp(pMainMenu->m_pStarSysMenu->m_nCurPage,szTmp)->SetEffect(147,-1);

		BuildPacketEx(pSendPacket, USER, C_RQ_STAR,buf,256);
		SET_DATA(pSend, USER, C_RQ_STAR,pSendPacket);
		pSend->bToLv	= (int)param;
		pChar->SendPacket(pSendPacket);
	}
}

void StarSysMenu::RqOpenStar( BYTE bLv )
{
	int nStar	= pChar->m_wStarLv;
	if(nStar < 10)
		nStar	= 10;
	if(bLv == nStar+1 && bLv <= pChar->m_byLvl)
	{
		StarLvObject* pStar	= StarSysCfg::GetInstance().GetStarInfo(bLv);
		if(pStar)
		{
			

			if(pStar->nCostGold)
			{				
				if(pStar->nCostExp > pChar->m_dwExp)
				{
					char szExp[64];
					sprintf(szExp, "��Ҫ����%d����,��ͨ��������߸�����ֻ�á�", pStar->nCostExp);
					pMainMenu->PopTipMsg(szExp);
					return;
				}
				if(pStar->nCostGold > pChar->m_dwMoney)
				{
					char szGold[64];
					sprintf(szGold, "��Ҫ����%d���,���Ե���ճ��μӻ���ٻ�ý�ҡ�", pStar->nCostGold);
					pMainMenu->PopTipMsg(szGold);
					return;
				}
				char szTemp[128];
				if (pStar->nCostExp == 0)
				{
					sprintf(szTemp, "�����ǳ���Ҫ����%d��ң��Ƿ񼤻", pStar->nCostGold);
				}else
					sprintf(szTemp, "�����ǳ���Ҫ����%d��Һ�%d���飬�Ƿ񼤻", pStar->nCostGold, pStar->nCostExp);
				pMainMenu->PopAskMsgBox(szTemp, UseGoldAndExp , (void*)bLv);			
			}else{
				char	szTmp[32];
				sprintf(szTmp,"q%d",nStar+1-m_nCurPage*10-10);
				m_pForm->pCFrame->GetFPComp(m_nCurPage,szTmp)->SetEffect(147,-1);

				BuildPacketEx(pSendPacket, USER, C_RQ_STAR,buf,256);
				SET_DATA(pSend, USER, C_RQ_STAR,pSendPacket);
				pSend->bToLv	= bLv;
				pChar->SendPacket(pSendPacket);				
			}						
		}		
	}else{
		if (bLv > pChar->m_byLvl)
		{
			pMainMenu->PopTipMsg("�ȼ����㣬���Ե���ճ��μӻ���ٻ�þ��顣");
		}
	}
}

void LifeStarSysClick(int nPos,char* szName,void* param)
{
	LifeStarSysMenu*	pMenu	= (LifeStarSysMenu*)param;

	if(strncmp(szName,"q",1) == 0)
	{
		int nPos	= atoi(szName+1);
		pMenu->RqOpenStar(nPos);
		pMenu->m_pCompHold	= pMenu->m_pForm->pCFrame->GetFPComp(pMenu->m_nCurPage,szName);
		pMenu->m_pCompHold->byRunAction	= 87;
		pMenu->m_bShowInfo	= true;
	}
	else
	{
		pMenu->m_pCompHold	= NULL;
		pMenu->m_bShowInfo	= false;
	}
}

LifeStarSysMenu::LifeStarSysMenu()
{
	m_pForm		= pMainInterface->GetDForm("ryue");

	DComponent*	ppComps[19];
	DComponent* pCompBase	= m_pForm->GetComponent("kuang");

	char	szTmp[64];
	ppComps[9]	= m_pForm->GetComponent("q1");
	ppComps[9]->SetViewSpr("MB_XC_HUIQUAN.png");
	for (int i = 0; i < 9; i++)
	{

		sprintf(szTmp,"x%d",i+1);
		ppComps[i]	= m_pForm->GetComponent(szTmp);
		ppComps[i]->SetSprColor(100,100,100);
		sprintf(szTmp,"q%d",i+2);
		ppComps[i+10]	= m_pForm->GetComponent(szTmp);
		ppComps[i+10]->SetViewSpr("MB_XC_HUIQUAN.png");

	}
	m_pForm->pCFrame	= new CCtrlFrame(20,109,914,440);
	m_pForm->pCFrame->InitFParts(pCompBase,ppComps,19);
	m_pForm->pCFrame->nSpace	= 54;
	m_pForm->pCFrame->nDisStep	= 914;
	m_pForm->pCFrame->SetFPCnt(1,true);
	m_pForm->pCFrame->SetEventEx(LifeStarSysClick,this);
	m_pForm->pCFrame->nZorder	= -1;
	m_pForm->pCFrame->bDisMoveTouch = true;
	m_pCompHold	= NULL;
	m_nCurPage	= -1;
}

LifeStarSysMenu::~LifeStarSysMenu()
{

}


bool LifeStarSysMenu::CheckCanOpen()
{
	m_bShowInfo	= false;
	if (pChar->m_wStarLv < 90)
	{
		pMainMenu->PopTipMsg("�����ǳ��ﵽ90����������ϵͳ");
		return false;
	}
	pChar->SendCmd(MAPCMD,C_GET_PARAM);
	return true;
}

void LifeStarSysMenu::Update()
{
	if(m_pForm->bOpened == false)
		return;

	m_pForm->pCFrame->SetFPCnt(1,true);
	SetStarLv(pChar->m_wLifeStarLv);
	int nRmPt	= pChar->m_byLvl - 90 - pChar->m_wLifeStarLv+1;
	if(nRmPt <= 0)
		nRmPt	= 0;
	m_pForm->GetComponent("num")->SetCaptionEx("%d",1,nRmPt);	
	int nPerJSVal = GetLifeStarLvPvpDef();
	m_pForm->GetComponent("js")->SetCaptionEx("%d.%d%%", 1, nPerJSVal/10,nPerJSVal%10);

	StarLvObject*	pStar	= StarSysCfg::GetInstance().GetStarInfo(pChar->m_wStarLv);
	if(pStar)
	{
		int nJob	= pChar->m_byJob-1;
		if(nJob >= MAXJOB)
			nJob	= 0;
		m_pForm->GetComponent("hp")->SetCaptionEx("%d",1,pStar->xParamTotal[nJob].nHpMax);
		m_pForm->GetComponent("sd")->SetCaptionEx("%d",1,pStar->xParamTotal[nJob].nMpMax);
		m_pForm->GetComponent("wg")->SetCaptionEx("%d",1,pStar->xParamTotal[nJob].nAtkW);
		m_pForm->GetComponent("mg")->SetCaptionEx("%d",1,pStar->xParamTotal[nJob].nAtkN);
		m_pForm->GetComponent("wf")->SetCaptionEx("%d",1,pStar->xParamTotal[nJob].nDefW);
		m_pForm->GetComponent("mf")->SetCaptionEx("%d",1,pStar->xParamTotal[nJob].nDefN);
		m_pForm->GetComponent("mz")->SetCaptionEx("%d",1,pStar->xParamTotal[nJob].nAim);
		m_pForm->GetComponent("sb")->SetCaptionEx("%d",1,pStar->xParamTotal[nJob].nDodge);
		m_pForm->GetComponent("bj")->SetCaptionEx("%d",1,pStar->xParamTotal[nJob].nCrit);
	}
	else
	{
		m_pForm->GetComponent("hp")->SetCaptionEx("%d",1,0);
		m_pForm->GetComponent("sd")->SetCaptionEx("%d",1,0);
		m_pForm->GetComponent("wg")->SetCaptionEx("%d",1,0);
		m_pForm->GetComponent("mg")->SetCaptionEx("%d",1,0);
		m_pForm->GetComponent("wf")->SetCaptionEx("%d",1,0);
		m_pForm->GetComponent("mf")->SetCaptionEx("%d",1,0);
		m_pForm->GetComponent("mz")->SetCaptionEx("%d",1,0);
		m_pForm->GetComponent("sb")->SetCaptionEx("%d",1,0);
		m_pForm->GetComponent("bj")->SetCaptionEx("%d",1,0);
	}

	int nPage = 0;
	DWORD	dwTickCur	= xnGetTickCount();
	if(nPage != m_nCurPage)
	{
		for (int i = 0; i < 10; i++)
		{
			m_dwRandActTick[i]	= rand()%300+dwTickCur;
		}
		m_nCurPage	= nPage;
		m_bShowInfo	= false;
		m_pCompHold	= NULL;
	}
	//m_pForm->GetComponent("dengji")->SetCaptionEx("%d--%d",1,(nPage+1)*10+1,(nPage+2)*10);
	char	szTmp[64];
	for (int i = 0; i < 10; i++)
	{
		sprintf(szTmp,"q%d",i+1);
		DComponent* pComp	= m_pForm->pCFrame->GetFPComp(nPage,szTmp);
		if(pComp && m_pCompHold != pComp)
		{//�������
			if(dwTickCur > m_dwRandActTick[i])
				pComp->byRunAction	= 80+(i+nPage)%5;
		}
		BYTE	byCompLv	= 90+i;
		int nPos1	= byCompLv/10%10;
		int nPos2	= byCompLv%10;
		char	szTmp[64];
		if(nPos1 <= 3)
		{
			sprintf(szTmp,"MB_DT0%d.png",nPos1);
		}
		else
			sprintf(szTmp,"MB_DT%d.png",nPos1);
		pComp->SetAddSprByTag(szTmp,-4,16,1);
		if(nPos2 <= 3)
		{
			sprintf(szTmp,"MB_DT0%d.png",nPos2);
		}
		else
			sprintf(szTmp,"MB_DT%d.png",nPos2);
		pComp->SetAddSprByTag(szTmp,12,16,2);
	}
}

void LifeStarSysMenu::SetStarLv( BYTE bLv )
{
	if (bLv > 10)
		bLv = 10;

	char	szTmp[64];
	for (int j = 0; j < 10; j++)
	{
		sprintf(szTmp,"q%d",j+1);
		DComponent* pComp	= m_pForm->pCFrame->GetFPComp(0,szTmp);
		if(j < bLv)
		{
			/*StarLvObject* pSLv	= StarSysCfg::GetInstance().GetStarInfo(j+1001);
			if(pSLv)
			{
				if(strlen(pSLv->szIco1))
				{
					pComp->SetViewSpr("MB_XC_DAQUAN.png");
					pComp->SetEffect(148,0);
				}
				else
				{
					pComp->SetViewSpr("MB_XC_XIAQUAN.png");
					pComp->SetEffect(155,0);
				}
			}*/
			pComp->SetViewSpr("MB_XC_DAQUAN.png");
			pComp->SetEffect(148,0);
			sprintf(szTmp,"x%d",j+1);
			pComp	= m_pForm->pCFrame->GetFPComp(0,szTmp);
			if(pComp)
				pComp->SetSprColor(255,255,255);
		}
		else if(j == bLv)
		{
			pComp->SetViewSpr("MB_XC_XIAQUAN.png");
			pComp->SetEffect(149,0);
		}
		else
		{
			pComp->SetViewSpr("MB_XC_HUIQUAN.png");
			pComp->SetEffect(0,0);
		}
	}
}

void UseGoldAndExpLife(void* param,int nVal)
{
	if(nVal == 1)
	{		
		pMainMenu->PopTipMsg("      �������³ɹ���");	

		char	szTmp[32];
		sprintf(szTmp,"q%d",pChar->m_wLifeStarLv+1);
		pMainMenu->m_pLifeStarSysMenu->m_pForm->pCFrame->GetFPComp(pMainMenu->m_pLifeStarSysMenu->m_nCurPage,szTmp)->SetEffect(147,-1);

		BuildPacketEx(pSendPacket, USER, C_RQ_LIFE_STAR,buf,256);
		SET_DATA(pSend, USER, C_RQ_LIFE_STAR,pSendPacket);
		pSend->bToLv	= (int)param;
		pChar->SendPacket(pSendPacket);
	}
}

void LifeStarSysMenu::RqOpenStar( BYTE bLv )
{
	if(bLv == pChar->m_wLifeStarLv+1 && bLv+90-1 <= pChar->m_byLvl)
	{
		StarLvObject* pStar	= StarSysCfg::GetInstance().GetStarInfo(bLv+1000);
		if(pStar)
		{
			if(pStar->nCostGold)
			{				
				if(pStar->nCostExp > pChar->m_dwExp)
				{
					char szExp[64];
					sprintf(szExp, "��Ҫ����%u����,��ͨ��������߸�����ֻ�á�", pStar->nCostExp);
					pMainMenu->PopTipMsg(szExp);
					return;
				}
				if(pStar->nCostGold > pChar->m_dwMoney)
				{
					char szGold[64];
					sprintf(szGold, "��Ҫ����%u���,���Ե���ճ��μӻ���ٻ�ý�ҡ�", pStar->nCostGold);
					pMainMenu->PopTipMsg(szGold);
					return;
				}
				char szTemp[128];
				sprintf(szTemp, "����������Ҫ����%u��Һ�%u���飬�Ƿ񼤻", pStar->nCostGold, pStar->nCostExp);
				pMainMenu->PopAskMsgBox(szTemp, UseGoldAndExpLife , (void*)bLv);			
			}					
		}		
	}else{
		if (bLv + 90-1 > pChar->m_byLvl)
		{
			pMainMenu->PopTipMsg("�ȼ����㣬���Ե���ճ��μӻ���ٻ�þ��顣");
		}
	}
}

int LifeStarSysMenu::GetLifeStarLvPvpDef()
{
	int nPerVal = 0;
	switch(pChar->m_wLifeStarLv)
	{
	case 0:
		nPerVal = 0;
		break;
	case 1:
		nPerVal = 10;
		break;
	case 2:
		nPerVal = 20;
		break;
	case 3:
		nPerVal = 35;
		break;
	case 4:
		nPerVal = 55;
		break;
	case 5:
		nPerVal = 80;
		break;
	case 6:
		nPerVal = 110;
		break;
	case 7:
		nPerVal = 145;
		break;
	case 8:
		nPerVal = 185;
		break;
	case 9:
		nPerVal = 230;
		break;
	case 10:
		nPerVal = 280;
		break;
	}
	return nPerVal;
}
