#include "UserData.h"
#include "AchievementMenu.h"
#include "AchievementCfg.h"
#include "ios_activity_packet.h"
#include "MainMenu.h"
#include "GameAwardCfg.h"

static int CompareAchieveShow(const void * Item1, const void * Item2)
{
	AchievemetnObj* pObj1 =	*(AchievemetnObj**)Item1 ;
	AchievemetnObj* pObj2 =	*(AchievemetnObj**)Item2 ;
	bool ifDone1 = false;
	bool ifDone2 = false;
	ClientAchievement* pCheck = pChar->m_pCAchievementCenter->GetObj(pObj1->nID);
	if(pCheck && pCheck->ifDone)
		ifDone1 = true;
	pCheck = pChar->m_pCAchievementCenter->GetObj(pObj2->nID);
	if(pCheck && pCheck->ifDone)
		ifDone2 = true;
	return pObj1->nPoint+(ifDone2?100:0)-pObj2->nPoint-(ifDone1?100:0);
}

AchievementListMenu::AchievementListMenu()
{
	m_pShowList = xnList::Create();
	m_pForm = pMainInterface->GetDForm("chengjiu4");
	m_pForm->bZorde = FORM_ZORDER_KUANG1;
	m_nFloor = -2;
	m_nGroup = -2;
	m_nPage = -2;
	DComponent* pBaseComp = m_pForm->GetComponent("qq");
	DComponent* ppComp[5];
	ppComp[0] = m_pForm->GetComponent("kuang");
	ppComp[1] = m_pForm->GetComponent("name");
	ppComp[2] = m_pForm->GetComponent("des");
	ppComp[3] = m_pForm->GetComponent("fen");
	ppComp[4] = m_pForm->GetComponent("jd");
	m_pForm->pCFrame = new CCtrlFrame(pBaseComp->nX,pBaseComp->nY,pBaseComp->nW,pBaseComp->nH*5);
	m_pForm->pCFrame->InitFParts(pBaseComp,ppComp,5);
	m_pForm->pCFrame->nDisStep = pBaseComp->nH;
	m_pForm->pCFrame->SetFPCnt(0);
}

AchievementListMenu::~AchievementListMenu()
{
	if(m_pShowList)
	{
		while(m_pShowList->Count)
			m_pShowList->Delete(0);
		m_pShowList->Free();
	}
}

void	AchievementListMenu::Open(int nT)
{
	m_pForm->Open(nT);
}

void	AchievementListMenu::Close()
{
	m_pForm->Close();
}

void	AchievementListMenu::SetList(int nPage,int nGroup,int nFloor/* =-1 */)
{
	if(m_nPage == nPage && m_nGroup == nGroup && m_nFloor == nFloor)
		return;
	m_nPage = nPage;
	m_nGroup = nGroup;
	m_nFloor = nFloor;
	while(m_pShowList->Count)
		m_pShowList->Delete(0);
	int nGID = 0;
	int nC = nGroup;
	for(int i = 0; i < AchievementCfg::GetInstance().m_pGroupList->Count; i ++)
	{
		ATileObj* pObj = (ATileObj*)AchievementCfg::GetInstance().m_pGroupList->Items[i];
		if(pObj->nGroup == nPage)
		{
			nC--;
			if(nC == 0)
			{
				nGID = pObj->nID;
				break;
			}
		}
	}
	if(nGID == 0)
		return;
	nC = nFloor;
	for(int i = 0; i < AchievementCfg::GetInstance().m_pFloorList->Count; i ++)
	{
		ATileObj* pObj = (ATileObj*)AchievementCfg::GetInstance().m_pFloorList->Items[i];
		if(pObj->nGroup == nGID)
		{
			nC --;
			if(nFloor == -1 || nC == 0)
				AchievementCfg::GetInstance().GetObjList(pObj->nID,m_pShowList);
		}
	}
	m_pShowList->Sort(CompareAchieveShow);
	DComponent* pComp = NULL;
	m_pForm->pCFrame->SetFPCnt(m_pShowList->Count);
	int szColor[2][3] = {255,255,255,160,255,100};
	for(int i = 0; i < m_pShowList->Count; i ++)
	{
		bool ifBright = false;
		AchievemetnObj* pObj = (AchievemetnObj*)m_pShowList->Items[i];
		ClientAchievement* pDoneObj = pChar->m_pCAchievementCenter->GetObj(pObj->nID);
		if(pDoneObj && pDoneObj->ifDone)
			ifBright = true;
		pComp = m_pForm->pCFrame->GetFPComp(i,"name");
		pComp->SetCaptionEx("%s",0,pObj->pName);
		pComp->SetSprColor(szColor[ifBright][0],szColor[ifBright][1],szColor[ifBright][2]);
		pComp = m_pForm->pCFrame->GetFPComp(i,"des");
		pComp->SetCaptionEx("%s",0,pObj->pDesc);
		pComp->SetSprColor(szColor[ifBright][0],szColor[ifBright][1],szColor[ifBright][2]);
		pComp = m_pForm->pCFrame->GetFPComp(i,"fen");
		char szTemp[32];
		sprintf(szTemp,"MB_CHENGJIUDIAN%d.png",pObj->nPoint);
		pComp->SetViewSpr(szTemp);
		//pComp->SetSprColor(szColor[ifBright][0],szColor[ifBright][1],szColor[ifBright][2]);
		pComp = m_pForm->pCFrame->GetFPComp(i,"jd");
		pComp->SetVisible(pObj->nCnt==1?false:true);
		if(pDoneObj)
		{
			if(pDoneObj->ifDone)
				pComp->SetCaptionEx("完成度%d/%d",0,pObj->nCnt,pObj->nCnt);
			else
				pComp->SetCaptionEx("完成度%d/%d",0,pDoneObj->nCnt,pObj->nCnt);
		}
		else
		{
			pComp->SetCaptionEx("完成度0/%d",0,pObj->nCnt);
		}
		pComp->SetSprColor(szColor[ifBright][0],szColor[ifBright][1],szColor[ifBright][2]);

		pComp = m_pForm->pCFrame->GetFPComp(i,"fen");
		if (ifBright)
		{
			//pComp->SetSprColor(255, 215, 0);
		}else
			pComp->SetSprColor(112, 112, 112);
	}
}

void	ATitleEvent(int nPos,char* pName,void* param)
{
	AchievementMenu* pMenu = (AchievementMenu*)param;
	if(pMenu)
	{
		if(strcmp(pName,"b1") == 0)
		{
			int nG = -1;
			if(nPos < pMenu->m_nGroup)
				nG = nPos+1;
			else if(nPos >= pMenu->m_nGroup + pMenu->m_nFCnt)
				nG = nPos-pMenu->m_nFCnt+1;
			pMenu->SetList(nG,-1);
		}
		else
		{
			int nF = -1;
			if(nPos >= pMenu->m_nGroup && nPos < pMenu->m_nGroup+pMenu->m_nFCnt)
				nF = nPos-pMenu->m_nGroup+1;
			pMenu->SetList(pMenu->m_nGroup,nF);
		}
	}
}

AchievementMenu::AchievementMenu(BYTE byPage)
{
	byMyPage = byPage;
	m_nGroup = -1;
	m_nFloor = -1;
	m_nFCnt = 0;
	m_pTitleList = xnList::Create();
	char szTemp[32];
	sprintf(szTemp,"chengjiu%d",byMyPage);
	m_pForm = pMainInterface->GetDForm(szTemp);
	DComponent* pBaseComp = m_pForm->GetComponent("b1");
	DComponent* ppComp[3];
	ppComp[0] = m_pForm->GetComponent("s1");
	ppComp[1] = m_pForm->GetComponent("s2");
	ppComp[1]->m_nFontSize = 28;
	ppComp[2] = m_pForm->GetComponent("xuan");
	m_pForm->pCFrame = new CCtrlFrame(pBaseComp->nX,pBaseComp->nY,pBaseComp->nW,pBaseComp->nH*7);
	m_pForm->pCFrame->InitFParts(pBaseComp,ppComp,3);
	m_pForm->pCFrame->SetEventEx(ATitleEvent,this);
	m_pForm->pCFrame->SetFPCnt(0);
}

AchievementMenu::~AchievementMenu()
{
	if(m_pTitleList)
	{
		while(m_pTitleList->Count)
			m_pTitleList->Delete(0);
		m_pTitleList->Free();
	}
}

void	AchievementMenu::SetList(int nGroup,int nFloor,bool ifForce)
{
	if(!ifForce && m_nGroup == nGroup && m_nFloor == nFloor)
	{
		pMainMenu->m_pAchievementListMenu->SetList(byMyPage,m_nGroup,m_nFloor);
		return;
	}
	m_nGroup = nGroup;
	m_nFloor = nFloor;
	while(m_pTitleList->Count)
		m_pTitleList->Delete(0);
	for(int i = 0; i < AchievementCfg::GetInstance().m_pGroupList->Count; i ++)
	{
		ATileObj* pObj = (ATileObj*)AchievementCfg::GetInstance().m_pGroupList->Items[i];
		if(pObj->nGroup == byMyPage)
			m_pTitleList->Add(pObj);
	}
	if(m_nGroup <= 0 || m_nGroup > m_pTitleList->Count)
		return;
	int nTempCount = m_pTitleList->Count;
	ATileObj* pObj = (ATileObj*)m_pTitleList->Items[m_nGroup-1];
	for(int i = AchievementCfg::GetInstance().m_pFloorList->Count-1; i >= 0; i --)
	{
		ATileObj* pFObj = (ATileObj*)AchievementCfg::GetInstance().m_pFloorList->Items[i];
		if(pFObj->nGroup == pObj->nID)
			m_pTitleList->Insert(m_nGroup,pFObj);
	}
	m_nFCnt = m_pTitleList->Count-nTempCount;
	m_pForm->pCFrame->SetFPCnt(m_pTitleList->Count);
	DComponent* pComp = NULL;
	int szColor[4][3] = {204,153,102,204,183,102,204,203,102,204,233,102};
	for(int i = 0; i < m_pTitleList->Count; i ++)
	{
		ATileObj* pObj = (ATileObj*)m_pTitleList->Items[i];
		if(i >= m_nGroup && i < m_nGroup+m_nFCnt)
		{
			pComp = m_pForm->pCFrame->GetFPComp(i,"b1");
			pComp->SetVisible(false);
			pComp = m_pForm->pCFrame->GetFPComp(i,"s1");
			pComp->SetVisible(true);
			pComp->SetSprColor(szColor[i-m_nGroup][0], szColor[i-m_nGroup][1], szColor[i-m_nGroup][2]);
			if(i == m_nGroup+m_nFCnt-1)
				m_pForm->pCFrame->SetFPNewSpace(i,0);
			else
				m_pForm->pCFrame->SetFPNewSpace(i,pComp->nH);
			pComp = m_pForm->pCFrame->GetFPComp(i,"s2");
			pComp->SetVisible(true);
			pComp->SetCaptionEx("%s",1,pObj->pName);
			//pComp->SetSprColor(218, 165, 32);
			pComp = m_pForm->pCFrame->GetFPComp(i,"xuan");
			if(i-m_nGroup+1 == nFloor)
				pComp->SetVisible(true);
			else
				pComp->SetVisible(false);
		}
		else
		{
			pComp = m_pForm->pCFrame->GetFPComp(i,"b1");
			pComp->SetVisible(true);
			pComp->SetViewSpr(pObj->pName);
			pComp = m_pForm->pCFrame->GetFPComp(i,"s1");
			pComp->SetVisible(false);
			if(i + 1 == m_nGroup)
				m_pForm->pCFrame->SetFPNewSpace(i,pComp->nH);
			else
				m_pForm->pCFrame->SetFPNewSpace(i,0);
			pComp = m_pForm->pCFrame->GetFPComp(i,"s2");
			pComp->SetVisible(false);
			pComp = m_pForm->pCFrame->GetFPComp(i,"xuan");
			pComp->SetVisible(false);
		}
	}
	m_pForm->pCFrame->ResetParts();
	pMainMenu->m_pAchievementListMenu->SetList(byMyPage,m_nGroup,m_nFloor);
	m_pForm->GetComponent("dc")->SetCaptionEx("%d/%d",0,pChar->m_pCAchievementCenter->m_nDoneCnt,AchievementCfg::GetInstance().m_pObjList->Count);
	m_pForm->GetComponent("dian")->SetCaptionEx("%d",0,pChar->m_pCAchievementCenter->m_nPoint);
}

void	AchievementMenu::Open(int nT)
{
	SetList(1,-1,true);
	m_pForm->Open(nT);
	pMainMenu->m_pAchievementListMenu->Open(nT);
}

void	AchievementMenu::Close()
{
	pMainMenu->m_pAchievementListMenu->Close();
	m_pForm->Close();
}

AchievementShowMenu::AchievementShowMenu()
{
	m_pForm = pMainInterface->GetDForm("chengjiu5");
	m_pForm->bZorde = FORM_ZORDER_NET_WAIT-1;
	m_dwEndTick = 0;
	m_byState = 0;
}

AchievementShowMenu::~AchievementShowMenu()
{

}

void	AchievementShowMenu::Open()
{	
	m_pForm->Open(1);
 	DComponent* pComp = NULL;
	pComp = m_pForm->GetComponent("kuang");
	pComp->SetVisible(false);
	//pComp->SetOpacity(0);
	pComp = m_pForm->GetComponent("name");
	pComp->SetVisible(false);
	//pComp->SetOpacity(0);
	pComp = m_pForm->GetComponent("fen");
	pComp->SetVisible(false);
	//pComp->SetOpacity(0);
	pComp = m_pForm->GetComponent("des");
	pComp->SetVisible(false);
	//pComp->SetOpacity(0);
}

void	AchievementShowMenu::Close()
{
	if(m_pForm->bOpened)
		m_pForm->Close();
}

void	AchievementShowMenu::Update()
{
	DWORD dwNow = xnGetTickCount();
	if(dwNow > m_dwEndTick)
	{
		if(m_byState == 2)
		{
			m_pForm->bOpenStyle = 7;
			m_pForm->Close();
			m_dwEndTick = dwNow + 1000;
			m_byState = 0;
		}
		else if(m_byState == 1)
		{
			DComponent* pComp = NULL;
			pComp = m_pForm->GetComponent("kuang");
			pComp->SetVisible(true);
			pComp->SetOpacity(0);
			pComp->byRunAction = 23;
			pComp = m_pForm->GetComponent("name");
			pComp->SetVisible(true);
			pComp->SetOpacity(0);
			pComp->byRunAction = 23;
			pComp = m_pForm->GetComponent("fen");
			pComp->SetVisible(true);
			pComp->SetOpacity(0);
			pComp->byRunAction = 23;
			pComp = m_pForm->GetComponent("des");
			pComp->SetVisible(true);
			pComp->SetOpacity(0);
			pComp->byRunAction = 23;
			m_byState = 2;
			m_dwEndTick = dwNow + 3000;

			pComp = m_pForm->GetComponent("qq");
			pComp->SetEffect(267,2);
		}
		else
		{
			while(pChar->m_pCAchievementCenter->m_pShowList->Count)
			{
				int nID = (int)pChar->m_pCAchievementCenter->m_pShowList->Delete(0);
				AchievemetnObj* pObj = AchievementCfg::GetInstance().GetObj(nID);
				if(pObj)
				{
					m_pForm->GetComponent("name")->SetCaptionEx("%s",0,pObj->pName);
					m_pForm->GetComponent("des")->SetCaptionEx("%s",0,pObj->pDesc);
					char szIcon[32];
					sprintf(szIcon,"MB_CHENGJIUDIAN%d.png",pObj->nPoint);
					m_pForm->GetComponent("fen")->SetViewSpr(szIcon);
					Open();
					m_dwEndTick = dwNow + 100;
					m_byState = 1;
				}
			}
		}
		
	}
}

void	ClickAAwardEvent(int nPos,char* pName,void* param)
{
	AchievementAwardMenu* pMenu = (AchievementAwardMenu*)param;
	if(pMenu)
	{
		if(strcmp(pName,"get1") == 0)
		{
			pChar->m_pCAchievementCenter->RequestForGift(nPos);
		}
	}
}

AchievementAwardMenu::AchievementAwardMenu()
{
	m_pForm = pMainInterface->GetDForm("chengjiu3");
	DComponent* pBaseComp = m_pForm->GetComponent("di2");
	DComponent* ppComp[6];
	ppComp[0] = m_pForm->GetComponent("name");
	ppComp[1] = m_pForm->GetComponent("des");
	ppComp[2] = m_pForm->GetComponent("icon1");
	ppComp[3] = m_pForm->GetComponent("icon2");
	ppComp[4] = m_pForm->GetComponent("yi");
	ppComp[5] = m_pForm->GetComponent("get1");
	m_pForm->pCFrame = new CCtrlFrame(pBaseComp->nX,pBaseComp->nY,pBaseComp->nW,pBaseComp->nH*3);
	m_pForm->pCFrame->InitFParts(pBaseComp,ppComp,6);
	m_pForm->pCFrame->nDisStep = pBaseComp->nH;
	m_pForm->pCFrame->SetEventEx(ClickAAwardEvent,this);
	m_pForm->pCFrame->SetFPCnt(0);
}

AchievementAwardMenu::~AchievementAwardMenu()
{

}

void	AchievementAwardMenu::Open(int nT)
{
	xnList* pList = AchievementCfg::GetInstance().m_pAwardList;
	m_pForm->pCFrame->SetFPCnt(pList->Count);
	DComponent* pComp = NULL;
	char szName[32];
	char szIcon[32];
	for(int i = 0;i < pList->Count; i ++)
	{
		AchieveAward* pAward = (AchieveAward*)pList->Items[i];
		pComp = m_pForm->pCFrame->GetFPComp(i,"name");
		pComp->SetCaptionEx("%s",0,pAward->pName);
		pComp = m_pForm->pCFrame->GetFPComp(i,"des");
		pComp->SetCaptionEx("%s",0,pAward->pDesc);
		pComp = m_pForm->pCFrame->GetFPComp(i,"yi");
		pComp->SetVisible(false);
		pComp = m_pForm->pCFrame->GetFPComp(i,"get1");
		pComp->SetVisible(false);
		for(int j = 0; j < 2; j ++)
		{
			if(j < pAward->pList->Count)
			{
				sprintf(szName,"icon%d",j+1);
				DComponent* pIcon = m_pForm->pCFrame->GetFPComp(i,szName);
				pIcon->SetVisible(true);
				_simpleAward* pA = (_simpleAward*)pAward->pList->Items[j];
				bool bIsItem = false;
				if(pA->nID == 555)
					sprintf(szIcon,"MB_ZI_EXP1.png");
				else if(pA->nID == 666)
					sprintf(szIcon,"MB_MONEY1.png");
				else if(pA->nID == 888)
					sprintf(szIcon,"MB_ZUANSHI1.png");
				else if(pA->nID == 333)
					sprintf(szIcon,"MB_BANGZUAN1.png");
				else
				{
					ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(pA->nID);
					sprintf(szIcon,"item/%d.png",pBase->nSpr);
					bIsItem = true;
				}
				if (bIsItem)
				{
					pIcon->SetViewSpr(szIcon);
					pIcon->SetCaptionEx("%d",2,pA->nNum);

					ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(pA->nID);
					char szText[32];
					sprintf(szText,"MB_PZ%d.png",pBase->nStar+1);
					pIcon->SetAddSprByTag(szText,-3,-3,1,100,0,-1);
					pIcon->ClearEffect();
					if (pBase->nStar == 2)
					{
						pIcon->SetEffect(174,1);
					}else if (pBase->nStar == 3)
					{
						pIcon->SetEffect(175,1);
					}
				}else
				{
					pIcon->ClearEffect();
					pIcon->SetViewSpr(szIcon);
					pIcon->SetCaptionEx("%d",2,pA->nNum);
				}
			}
			else
			{
				sprintf(szName,"icon%d",j+1);
				m_pForm->pCFrame->GetFPComp(i,szName)->SetVisible(false);
			}
		}
	}
	m_pForm->Open(nT);
}

void	AchievementAwardMenu::Update()
{
	if(m_pForm && m_pForm->bOpened)
	{
		DComponent* pGetComp = NULL;
		DComponent* pRltComp = NULL;
		for(int i = 0; i < AchievementCfg::GetInstance().m_pAwardList->Count; i ++)
		{
			AchieveAward* pAward = (AchieveAward*)AchievementCfg::GetInstance().m_pAwardList->Items[i];
			pGetComp = m_pForm->pCFrame->GetFPComp(i,"get1");
			pRltComp = m_pForm->pCFrame->GetFPComp(i,"yi");
			if(pChar->m_pCAchievementCenter->CheckAward(i))
			{
				int nFlag = pChar->m_pClientFlag->GetFlag(25);
				int nCheck = 0x01;
				nCheck = nCheck<<i;
				if((nFlag&nCheck) == nCheck)
				{
					pGetComp->SetVisible(false);
					pRltComp->SetVisible(true);
				}
				else
				{
					pGetComp->SetVisible(true);
					pGetComp->bClickLocked = false;
					pGetComp->SetViewSpr("BT_LINGQU234.png");
					pRltComp->SetVisible(false);
				}
			}
			else
			{
				pGetComp->SetVisible(true);
				pGetComp->bClickLocked = true;
				pGetComp->SetViewSpr("BT_LINGQU234_H.png");
				pRltComp->SetVisible(false);
			}
		}
		m_pForm->GetComponent("dc")->SetCaptionEx("%d/%d",0,pChar->m_pCAchievementCenter->m_nDoneCnt,AchievementCfg::GetInstance().m_pObjList->Count);
		m_pForm->GetComponent("dian")->SetCaptionEx("%d",0,pChar->m_pCAchievementCenter->m_nPoint);
	}
}

void	AchievementAwardMenu::Close()
{
	m_pForm->Close();
}

AchievementOpenMenu::AchievementOpenMenu()
{
	m_pForm = pMainInterface->GetDForm("chengjiu6");
}

void	AchievementOpenMenu::Open(int nT)
{
	pMainMenu->m_pAchievementTabCtrl->Open(nT,0);
}