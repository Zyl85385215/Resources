#include "UserData.h"
#include "RankMenu.h"
#include "RankData.h"
#include "MainMenu.h"


RankMenuCenter* g_pRankMenuCenter = NULL;
void SetCompArray(DForm* pForm,DComponent** ppCom,int& cnt,char* szName)
{
	if(pForm == NULL)
		return;
	ppCom[cnt] = pForm->GetComponent(szName);
	if(ppCom[cnt] != NULL)
		cnt++;
}

void RankFrmCheckEvent(int nPos,char* szName,void* param)
{
	if(strcmp(szName,"ck") == 0)
	{
		int nRankPos = (int)param-1;
		if(nRankPos < RANK_MAX)
		{
			RankList* pList = pChar->m_pRankCenter->m_szRankList[nRankPos];
			if(nPos < pList->m_pList->Count)
			{
				Rank_Data* pData = (Rank_Data*) pList->m_pList->Items[nPos];
				if(nRankPos == RANK_EQUIP)
				{
					ClientWatchEquip* pGet = pChar->m_pRankCenter->GetUserEquip(pData->m_dwID1,pData->m_dwID2);
					if(pGet)
						pMainMenu->m_pEquipInfoMenu->Open(pGet->pItem,FORM_NULL);
				}
				else
				{
					ClientUserInfo* pGet = pChar->m_pRankCenter->GetUserInfo(pData->m_dwID1);
					if(pGet)
					{
						pMainMenu->m_pSelObjInfoMenu->Open(pGet);
					}
				}
			}
		}
	}
}

void ChickDamageAwardBtnEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	pMainMenu->m_pRankDamageAwardMenu->Open();
}

RankMenuObj::RankMenuObj(char * szname,BYTE byPos)
{
	m_pForm = pMainInterface->GetDForm(szname);
	m_byPos = byPos;
	m_dwValueFlag = 0;
	DComponent* pBase = m_pForm->GetComponent("n1");
	m_pForm->pCFrame = new CCtrlFrame(pBase->nX,pBase->nY,pBase->nW,pBase->nH*5);
	DComponent* ppComp[11];
	int nCount = 0;
	SetCompArray(m_pForm,ppComp,nCount,"xz");
	SetCompArray(m_pForm,ppComp,nCount,"rank");
	SetCompArray(m_pForm,ppComp,nCount,"val1");
	SetCompArray(m_pForm,ppComp,nCount,"val2");
	SetCompArray(m_pForm,ppComp,nCount,"val3");
	SetCompArray(m_pForm,ppComp,nCount,"val4");
	SetCompArray(m_pForm,ppComp,nCount,"desc1");
	SetCompArray(m_pForm,ppComp,nCount,"desc2");
	SetCompArray(m_pForm,ppComp,nCount,"desc3");
	SetCompArray(m_pForm,ppComp,nCount,"desc4");
	SetCompArray(m_pForm,ppComp,nCount,"ck");
	SetCompArray(m_pForm,ppComp,nCount,"x1");
	m_pForm->pCFrame->InitFParts(pBase,ppComp,nCount);
	m_pForm->pCFrame->nDisStep = pBase->nH;
	m_pForm->pCFrame->SetFPCnt(0);
	m_pForm->pCFrame->SetEventEx(RankFrmCheckEvent,(void*)(byPos+1));
	g_pRankMenuCenter->AddObj(this);

	DComponent* pDeComp = m_pForm->GetComponent("jiangli");
	if (pDeComp)
		pDeComp->SetEvent(ChickDamageAwardBtnEvent);
}

bool RankMenuObj::CheckColor(int nCnt)
{
	char szName[16];
	for(int i = 0; i < nCnt; i ++)
	{
		sprintf(szName,"page%d",i+1);
		DComponent* pComp = m_pForm->GetComponent(szName);
		if(pComp)
		{
			if(i == 2 || (i - 4) == m_byPos)
				pComp->SetSprColor(255,255,255);
			else
				pComp->SetSprColor(127,127,127);
		}
		
	}
	return true;
}

bool RankMenuObj::CheckCanOpen()
{
	if(pChar->m_byLvl < 20)
	{
		pMainMenu->PopTipMsg("等级达到20级开启排行功能");
		return false;
	}
	pChar->m_pRankCenter->RequestRankInfo();
	return true;
}

void RankMenuObj::UpdateDamage()
{
	Rank_Data* pMine = pChar->m_pRankCenter->m_pDamageLastData;
	if (!pMine)
	{
		return;
	}
	DComponent* pMCom = m_pForm->GetComponent("mval1");
	if(pMCom)
		pMCom->SetCaptionEx("%d",0,pMine->m_dwVal[0]);
	pMCom = m_pForm->GetComponent("mval2");
	if(pMCom)
		pMCom->SetCaptionEx("%d",0,pMine->m_dwVal[1]);
	pMCom = m_pForm->GetComponent("mval3");
	if(pMCom)
		pMCom->SetCaptionEx("%d",0,pMine->m_dwVal[2]);
	pMCom = m_pForm->GetComponent("mval4");
	if(pMCom)
		pMCom->SetCaptionEx("%d",0,pMine->m_dwVal[3]);
	pMCom = m_pForm->GetComponent("mdesc1");
	if(pMCom)
		pMCom->SetCaptionEx("%s",0,pMine->m_szDesc[0]);
	pMCom = m_pForm->GetComponent("mdesc2");
	if(pMCom)
		pMCom->SetCaptionEx("%s",0,pMine->m_szDesc[1]);
	pMCom = m_pForm->GetComponent("mdesc3");
	if(pMCom)
		pMCom->SetCaptionEx("%s",0,pMine->m_szDesc[2]);
	pMCom = m_pForm->GetComponent("mdesc4");
	if(pMCom)
		pMCom->SetCaptionEx("%s",0,pMine->m_szDesc[3]);
}

RankMenuCenter::RankMenuCenter()
{
	for(int i = 0; i < RANK_MAX; i ++)
		m_szObj[i] = NULL;
}

RankMenuCenter::~RankMenuCenter()
{

}

void RankMenuCenter::AddObj(RankMenuObj* pObj)
{
	if(pObj->m_byPos < RANK_MAX)
		m_szObj[pObj->m_byPos] = pObj;
}

void RankMenuCenter::Update()
{
	for(int i = 0; i < RANK_MAX; i ++)
	{
		if(pChar->m_pRankCenter->m_szCH[i])
		{
			pChar->m_pRankCenter->m_szCH[i] = false;
			if(m_szObj[i] == NULL || pChar->m_pRankCenter->m_szRankList[i] == NULL)
				continue;
			int nCount = pChar->m_pRankCenter->m_szRankList[i]->m_pList->Count;
			m_szObj[i]->m_pForm->pCFrame->SetFPCnt(nCount);
			for(int j = 0; j < nCount; j ++)
			{
				Rank_Data* pData = (Rank_Data*)pChar->m_pRankCenter->m_szRankList[i]->m_pList->Items[j];
				DComponent* pComp = NULL;
				pComp = m_szObj[i]->m_pForm->pCFrame->GetFPComp(j,"val1");
				if(pComp)
					pComp->SetCaptionEx("%d",0,pData->m_dwVal[0]);
				pComp = m_szObj[i]->m_pForm->pCFrame->GetFPComp(j,"val2");
				if(pComp)
					pComp->SetCaptionEx("%d",0,pData->m_dwVal[1]);
				pComp = m_szObj[i]->m_pForm->pCFrame->GetFPComp(j,"val3");
				if(pComp)
					pComp->SetCaptionEx("%d",0,pData->m_dwVal[2]);
				pComp = m_szObj[i]->m_pForm->pCFrame->GetFPComp(j,"val4");
				if(pComp)
					pComp->SetCaptionEx("%d",0,pData->m_dwVal[3]);
				pComp = m_szObj[i]->m_pForm->pCFrame->GetFPComp(j,"desc1");
				if(pComp)
					pComp->SetCaptionEx("%s",0,pData->m_szDesc[0]);
				pComp = m_szObj[i]->m_pForm->pCFrame->GetFPComp(j,"desc2");
				if(pComp)
					pComp->SetCaptionEx("%s",0,pData->m_szDesc[1]);
				pComp = m_szObj[i]->m_pForm->pCFrame->GetFPComp(j,"desc3");
				if(pComp)
					pComp->SetCaptionEx("%s",0,pData->m_szDesc[2]);
				pComp = m_szObj[i]->m_pForm->pCFrame->GetFPComp(j,"desc4");
				if(pComp)
					pComp->SetCaptionEx("%s",0,pData->m_szDesc[3]);
				pComp = m_szObj[i]->m_pForm->pCFrame->GetFPComp(j,"rank");
				if(pComp)
				{
					if(pData->m_nPos < 3)
					{
						pComp->SetCaption("",1);
						pComp->SetViewSpr("");
					}
					else if(pData->m_nPos < 10)
					{
						char szIcon[32];
						sprintf(szIcon,"MB_DT%d.png",pData->m_nPos+1);
						pComp->SetViewSpr(szIcon);
					}
					else
						pComp->SetCaptionEx("%d",1,pData->m_nPos+1);
				}
				pComp = m_szObj[i]->m_pForm->pCFrame->GetFPComp(j,"xz");
				if(pComp)
					pComp->SetVisible(true);
				pComp = m_szObj[i]->m_pForm->pCFrame->GetFPComp(j,"x1");
				if(pComp)
					pComp->SetVisible(true);
				DComponent* pCompBtn = m_szObj[i]->m_pForm->pCFrame->GetFPCompBase(j);
				if(pCompBtn)
				{
					int nEffID = 0;
					if (j >= 0 && j <3)
					{
						nEffID = 281 + j;
					}
					pCompBtn->SetEffect(nEffID, 1);
				}
			}
			Rank_Data* pMine = NULL;
			if (i == 6)
			{
				pMine = pChar->m_pRankCenter->m_pDamageLastData;
			}else
				pMine = pChar->m_pRankCenter->m_szRankList[i]->pMine;
			if(pMine)
			{
				if(pMine->m_nPos >= 100)
				{
					DComponent* pMCom = m_szObj[i]->m_pForm->GetComponent("mval1");
					if(pMCom)
						pMCom->SetCaption("",1);
					pMCom = m_szObj[i]->m_pForm->GetComponent("mval2");
					if(pMCom)
						pMCom->SetCaption("",1);
					pMCom = m_szObj[i]->m_pForm->GetComponent("mval3");
					if(pMCom)
						pMCom->SetCaption("",1);
					pMCom = m_szObj[i]->m_pForm->GetComponent("mval4");
					if(pMCom)
						pMCom->SetCaption("",1);
					pMCom = m_szObj[i]->m_pForm->GetComponent("mdesc1");
					if(pMCom)
						pMCom->SetCaption("",1);
					pMCom = m_szObj[i]->m_pForm->GetComponent("mdesc2");
					if(pMCom)
						pMCom->SetCaption("",1);
					pMCom = m_szObj[i]->m_pForm->GetComponent("mdesc3");
					if(pMCom)
						pMCom->SetCaption("",1);
					pMCom = m_szObj[i]->m_pForm->GetComponent("mdesc4");
					if(pMCom)
						pMCom->SetCaption("",1);
					pMCom = m_szObj[i]->m_pForm->GetComponent("mrank");
					if(pMCom)
						pMCom->SetCaption("",1);
					pMCom = m_szObj[i]->m_pForm->GetComponent("me");
					if(pMCom)
						pMCom->SetCaption("您未进入排行榜",1);
				}
				else
				{
					DComponent* pMCom = m_szObj[i]->m_pForm->GetComponent("mval1");
					if(pMCom)
						pMCom->SetCaptionEx("%d",0,pMine->m_dwVal[0]);
					pMCom = m_szObj[i]->m_pForm->GetComponent("mval2");
					if(pMCom)
						pMCom->SetCaptionEx("%d",0,pMine->m_dwVal[1]);
					pMCom = m_szObj[i]->m_pForm->GetComponent("mval3");
					if(pMCom)
						pMCom->SetCaptionEx("%d",0,pMine->m_dwVal[2]);
					pMCom = m_szObj[i]->m_pForm->GetComponent("mval4");
					if(pMCom)
						pMCom->SetCaptionEx("%d",0,pMine->m_dwVal[3]);
					pMCom = m_szObj[i]->m_pForm->GetComponent("mdesc1");
					if(pMCom)
						pMCom->SetCaptionEx("%s",0,pMine->m_szDesc[0]);
					pMCom = m_szObj[i]->m_pForm->GetComponent("mdesc2");
					if(pMCom)
						pMCom->SetCaptionEx("%s",0,pMine->m_szDesc[1]);
					pMCom = m_szObj[i]->m_pForm->GetComponent("mdesc3");
					if(pMCom)
						pMCom->SetCaptionEx("%s",0,pMine->m_szDesc[2]);
					pMCom = m_szObj[i]->m_pForm->GetComponent("mdesc4");
					if(pMCom)
						pMCom->SetCaptionEx("%s",0,pMine->m_szDesc[3]);
					pMCom = m_szObj[i]->m_pForm->GetComponent("mrank");
					if(pMCom)
					{
						if(pMine->m_nPos < 10)
						{
							char szIcon[32];
							sprintf(szIcon,"MB_DT%d.png",pMine->m_nPos+1);
							pMCom->SetViewSpr(szIcon);
						}
						else
							pMCom->SetCaptionEx("%d",1,pMine->m_nPos+1);
					}
					pMCom = m_szObj[i]->m_pForm->GetComponent("me");
					if(pMCom)
						pMCom->SetCaption("",1);
				}
			}
			else
			{
				DComponent* pMCom = m_szObj[i]->m_pForm->GetComponent("mval1");
				if(pMCom)
					pMCom->SetCaption("",1);
				pMCom = m_szObj[i]->m_pForm->GetComponent("mval2");
				if(pMCom)
					pMCom->SetCaption("",1);
				pMCom = m_szObj[i]->m_pForm->GetComponent("mval3");
				if(pMCom)
					pMCom->SetCaption("",1);
				pMCom = m_szObj[i]->m_pForm->GetComponent("mval4");
				if(pMCom)
					pMCom->SetCaption("",1);
				pMCom = m_szObj[i]->m_pForm->GetComponent("mdesc1");
				if(pMCom)
					pMCom->SetCaption("",1);
				pMCom = m_szObj[i]->m_pForm->GetComponent("mdesc2");
				if(pMCom)
					pMCom->SetCaption("",1);
				pMCom = m_szObj[i]->m_pForm->GetComponent("mdesc3");
				if(pMCom)
					pMCom->SetCaption("",1);
				pMCom = m_szObj[i]->m_pForm->GetComponent("mdesc4");
				if(pMCom)
					pMCom->SetCaption("",1);
				pMCom = m_szObj[i]->m_pForm->GetComponent("mrank");
				if(pMCom)
					pMCom->SetCaption("",1);
				pMCom = m_szObj[i]->m_pForm->GetComponent("me");
				if(pMCom)
					pMCom->SetCaption("",1);
			}
		}
	}

	m_szObj[RANK_DAMAGE]->UpdateDamage();
}

void	ClickDamageShowEvent(InterfaceEvent* pEvent,void* param)
{
	if(param)
	{
		static int nClickY = 0;
		RankDamageAwardMenu* pMenu = (RankDamageAwardMenu*)param;
		switch(pEvent->event)
		{
		case INTERFACE_EVENT_DOWN:
			{
				nClickY = pEvent->y;
			}
			break;
		case INTERFACE_EVENT_MOVE:
			{
				pMenu->MoveXY(pEvent->pComponent->nY + pEvent->y - nClickY);
				nClickY		= pEvent->y;
			}
			break;
		case INTERFACE_EVENT_UP:
			{
				pMenu->MoveXY(pEvent->pComponent->nY + pEvent->y - nClickY);
				nClickY		= 0;
			}
			break;
		}
	}
}

RankDamageAwardMenu::RankDamageAwardMenu()
{
	m_pForm = pMainInterface->GetDForm("tumo");
	m_pShowComp = m_pForm->GetComponent("z");
	m_pShowComp->byType = COMPONENT_MAPGUIDE;
	m_pShowComp->SetViewSpr("ui/MB_TUMOJL.png");
	m_pShowComp->SetEvent(ClickDamageShowEvent,this);
	m_pShowComp->bAlignX	= 1;
	m_pShowComp->bAlignY	= 0;
	m_nBaseX = m_pShowComp->nX;
	m_nBaseY = m_pShowComp->nY;
}

void RankDamageAwardMenu::Open()
{
	m_pForm->Open();
	m_pShowComp->SetPos(m_nBaseX, m_nBaseY);
}

void RankDamageAwardMenu::MoveXY( int nMoveY )
{
	if(nMoveY > m_nBaseY)
		nMoveY = m_nBaseY;
	if(nMoveY < (m_nBaseY - m_pShowComp->nSprPixHeight + m_pShowComp->nH))
		nMoveY = m_nBaseY - m_pShowComp->nSprPixHeight + m_pShowComp->nH;

	m_pShowComp->SetPos(m_nBaseX, nMoveY);
}
