#include "InterfaceLayer.h"
#include "CreateRoleMenu.h"
#include "TextConfig.h"
#include "CtrlObj.h"
#include "ios_ac_packet.h"
#include "ZNetPacket.h"
#include "MainMenu.h"

extern char* szJobTxt[];
extern char* szCountryTxt[];
extern void	SetCharToDCom(DComponent* pDom,BYTE byJob,BYTE bySex,char* szAction = "stand",int nDirection = 5);
extern void	SetWeaponToDCom(DComponent* pDom,BYTE byJob,BYTE bySex,char* szAction = "stand",int nDirection = 5);

static bool btemp = false;
const char* const szUserPic[] = {"MB_MAN1.png","MB_WOMAN1.png","MB_MAN2.png","MB_WOMAN2.png","MB_MAN3.png","MB_WOMAN3.png","MB_MAN4.png","MB_WOMAN4.png"};
const char* const szUserJob[] = {"MB_ZHIYE1.png","MB_ZHIYE1.png","MB_ZHIYE2.png","MB_ZHIYE2.png","MB_ZHIYE3.png","MB_ZHIYE3.png","MB_ZHIYE4.png","MB_ZHIYE4.png"};
const char* const szUserJob2[] = {"MB_ZY1.png","MB_ZY1.png","MB_ZY2.png","MB_ZY2.png","MB_ZY3.png","MB_ZY3.png","MB_ZY4.png","MB_ZY4.png"};
const char* const szUserSex[] = {"BT_MAN.png","BT_WOMAN.png","BT_MAN.png","BT_WOMAN.png","BT_MAN.png","BT_WOMAN.png","BT_MAN.png","BT_WOMAN.png"};
static bool bTempMove = false;
void RandNameEvent(InterfaceEvent * pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	CreateRoleMenu* pMenu = (CreateRoleMenu*)param;

	char*	szFile		= RandNameConfig::GetInstance().GetRndName(pMenu->nSex);
	pMenu->m_pForm->GetComponent("name")->SetEdit(szFile);

}

void CRoleViewEvent(InterfaceEvent * pEvent,void * param)
{
	
	switch(pEvent->event)
	{
	case INTERFACE_EVENT_HANDSIGN+1:		//right
	case INTERFACE_EVENT_HANDSIGN+4:		//right
	case INTERFACE_EVENT_HANDSIGN+6:		//right
	case INTERFACE_EVENT_HANDSIGN+8:		//right
		{
			int nJump = true;
			for (int i=0;i<CreateRoleMenu::pMenu->m_pForm->pCFrame->GetFPCnt();i++)
			{
				int nColor = CreateRoleMenu::pMenu->m_pForm->pCFrame->GetFPCompBase(i)->nR;
				if (nColor == 255)
				{
					nJump = false;
				}
			}

			if (nJump)
			{
				return;
			}
			for (int i=0;i<CreateRoleMenu::pMenu->m_pForm->pCFrame->GetFPCnt();i++)
			{

				CreateRoleMenu::pMenu->SetPosAction(i,"stand");
				CreateRoleMenu::pMenu->SetPosDark(i,true);

			}
			CCtrlFrame* pCFrame	= (CCtrlFrame*)param;
			if(pCFrame->bIsHoriz)
				pCFrame->nChgX	= -pCFrame->nDisStep;
			else
				pCFrame->nChgY	= -pCFrame->nDisStep;

			CreateRoleMenu::pMenu->nSex	= (CreateRoleMenu::pMenu->nSex+1)%2;
			if(CreateRoleMenu::pMenu->nSex == 1)
			{
				CreateRoleMenu::pMenu->nJob++;
				if(CreateRoleMenu::pMenu->nJob == 5)
					CreateRoleMenu::pMenu->nJob	= 1;
			}
			CreateRoleMenu::pMenu->m_pForm->GetComponent("p")->byRunAction = 11;
		}
		break;
	case INTERFACE_EVENT_HANDSIGN+2:		//left
	case INTERFACE_EVENT_HANDSIGN+3:		//left
	case INTERFACE_EVENT_HANDSIGN+5:		//left
	case INTERFACE_EVENT_HANDSIGN+7:		//left
		{
			int nJump = true;
			for (int i=0;i<CreateRoleMenu::pMenu->m_pForm->pCFrame->GetFPCnt();i++)
			{
				int nColor = CreateRoleMenu::pMenu->m_pForm->pCFrame->GetFPCompBase(i)->nR;
				if (nColor == 255)
				{
					nJump = false;
				}
				CreateRoleMenu::pMenu->SetPosAction(i,"stand");
				CreateRoleMenu::pMenu->SetPosDark(i,true);

			}

			if (nJump)
			{
				return;
			}
			CCtrlFrame* pCFrame	= (CCtrlFrame*)param;
			if(pCFrame->bIsHoriz)
				pCFrame->nChgX	= pCFrame->nDisStep;
			else
				pCFrame->nChgY	= pCFrame->nDisStep;

			CreateRoleMenu::pMenu->nSex	= (CreateRoleMenu::pMenu->nSex+1)%2;
			if(CreateRoleMenu::pMenu->nSex == 0)
			{
				CreateRoleMenu::pMenu->nJob--;
				if(CreateRoleMenu::pMenu->nJob == 0)
					CreateRoleMenu::pMenu->nJob	= 4;
			}
			CreateRoleMenu::pMenu->m_pForm->GetComponent("p")->byRunAction = 11;
		}
		break;
	}


}

void MoveFrmLeftEx(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nJump = true;
	for (int i=0;i<CreateRoleMenu::pMenu->m_pForm->pCFrame->GetFPCnt();i++)
	{
		int nColor = CreateRoleMenu::pMenu->m_pForm->pCFrame->GetFPCompBase(i)->nR;
		if (nColor == 255)
		{
			nJump = false;
		}
		CreateRoleMenu::pMenu->SetPosAction(i,"stand");
		CreateRoleMenu::pMenu->SetPosDark(i,true);

	}

	if (nJump)
	{
		return;
	}
	CCtrlFrame* pCFrame	= (CCtrlFrame*)param;
	if(pCFrame->bIsHoriz)
		pCFrame->nChgX	= pCFrame->nDisStep;
	else
		pCFrame->nChgY	= pCFrame->nDisStep;

	CreateRoleMenu::pMenu->nSex	= (CreateRoleMenu::pMenu->nSex+1)%2;
	if(CreateRoleMenu::pMenu->nSex == 0)
	{
		CreateRoleMenu::pMenu->nJob--;
		if(CreateRoleMenu::pMenu->nJob == 0)
			CreateRoleMenu::pMenu->nJob	= 4;
	}
	CreateRoleMenu::pMenu->m_pForm->GetComponent("p")->byRunAction = 11;
}

void SetCountry(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	
	CreateRoleMenu::pMenu->m_nCountry = (int)param;
}

void MoveFrmRightEx(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nJump = true;
	for (int i=0;i<CreateRoleMenu::pMenu->m_pForm->pCFrame->GetFPCnt();i++)
	{
		int nColor = CreateRoleMenu::pMenu->m_pForm->pCFrame->GetFPCompBase(i)->nR;
		if (nColor == 255)
		{
			nJump = false;
		}
		//CreateRoleMenu::pMenu->SetPosAction(i,"stand");
		//CreateRoleMenu::pMenu->SetPosDark(i,true);

	}

	if (nJump)
	{
		return;
	}
	for (int i=0;i<CreateRoleMenu::pMenu->m_pForm->pCFrame->GetFPCnt();i++)
	{

		CreateRoleMenu::pMenu->SetPosAction(i,"stand");
		CreateRoleMenu::pMenu->SetPosDark(i,true);

	}
	CCtrlFrame* pCFrame	= (CCtrlFrame*)param;
	if(pCFrame->bIsHoriz)
		pCFrame->nChgX	= -pCFrame->nDisStep;
	else
		pCFrame->nChgY	= -pCFrame->nDisStep;

	CreateRoleMenu::pMenu->nSex	= (CreateRoleMenu::pMenu->nSex+1)%2;
	if(CreateRoleMenu::pMenu->nSex == 1)
	{
		CreateRoleMenu::pMenu->nJob++;
		if(CreateRoleMenu::pMenu->nJob == 5)
			CreateRoleMenu::pMenu->nJob	= 1;
	}
	CreateRoleMenu::pMenu->m_pForm->GetComponent("p")->byRunAction = 11;
}

static void CBFrameFBStepEvent(int nPos)
{
	DForm* pForm = pMainInterface->GetDForm("chuangjian");
	if (nPos>=0)
	{
		nPos = nPos+1;
		for (int i=0;i<pForm->pCFrame->GetFPCnt();i++)
		{
			if (i%8 == nPos%8)
			{
				CreateRoleMenu::pMenu->SetPosDark(i,false);
				//pForm->pCFrame->GetFPCompBase(i)->SetSprColor(255,255,255);
			}
			//pForm->pCFrame->GetFPCompBase(i)->SetEnable(i%8 == nPos%8);
		}

		int nJob = nPos%8/2+1;
		int nSex = (nPos+1)%2;
		if (CreateRoleMenu::pMenu->nJob!=nJob||CreateRoleMenu::pMenu->nSex!=nSex)
		{
			bTempMove = false;
			pForm->GetComponent("p")->byRunAction = 11;
			if(CreateRoleMenu::pMenu->nSex!=nSex){
				char*	szFile		= RandNameConfig::GetInstance().GetRndName(nSex);
				pForm->GetComponent("name")->SetEdit(szFile);		
			}
			CreateRoleMenu::pMenu->nJob = nJob;
			CreateRoleMenu::pMenu->nSex = nSex;
			btemp = true;
			
		}
	}else{
		switch(nPos){
		case STEP_EVENT_BEGIN:
			{
				for (int i=0;i<pForm->pCFrame->GetFPCnt();i++)
				{
					CreateRoleMenu::pMenu->SetPosAction(i,"stand");
					CreateRoleMenu::pMenu->SetPosDark(i,true);

				}
				bTempMove = true;
			}
			break;
		}
	}
	
}

static void ActionEventFunc(void * param,void* pActionParam){
	int nType = (int)param;
	switch(nType){
	case 11:
		
		CreateRoleMenu::pMenu->UpdateJob();
		
		break;
	}
}

CreateRoleMenu::CreateRoleMenu():nJob(1),nSex(0),m_nDefCountry(0)
{
	CreateRoleMenu::pMenu = this;
	m_pForm		= pMainInterface->GetDForm("chuangjian");

	m_pForm->GetComponent("shaizi")->SetEvent(RandNameEvent,this);
	m_pForm->GetComponent("xxx")->m_nOrder = 255;
	m_pForm->GetComponent("yqm")->SetCaption("СЎМо",0);
	DComponent*	pCompC[2];
	pCompC[1] = m_pForm->GetComponent ("xingxiang2");
	pCompC[0] = m_pForm->GetComponent ("xingxiang");
	//pCompC[2] = m_pForm->GetComponent ("21112");

	m_pForm->pCFrame	= new CCtrlFrame(500,150,400,405);
	DComponent*	pComp	= m_pForm->GetComponent ("x");

	m_pForm->pCFrame->InitFParts(pComp,pCompC,2);
	//m_pForm->pCFrame->SetEvent (CBFrameFBEvent);
	//m_pForm->pCFrame->SetStepEvent(CBFrameFBStepEvent);
	m_pForm->pCFrame->nSpace	= 10;
	m_pForm->pCFrame->nDisStep	= 130;
	m_pForm->pCFrame->SetFPCnt(8,true,true);
	m_pForm->pCFrame->SetStepEvent(CBFrameFBStepEvent);
	for (int i=0;i<m_pForm->pCFrame->GetFPCnt();i++) 
	{
		SetPosAction(i,"stand");
		if (i%8 == 2||i%8 == 3)
		{
			CCtrlFPart* pFP	= (CCtrlFPart*)m_pForm->pCFrame->pListFP->Items[i];
			pFP->pListComps->Exchange(0,1);
		}
	}
	m_pForm->pCFrame->bSplitTouch = true;
	m_pForm->GetComponent("p")->SetActionCallback(ActionEventFunc);
	m_pForm->GetComponent("p1")->SetEvent(CRoleViewEvent,m_pForm->pCFrame);

	m_pForm->GetComponent("left")->SetEvent(MoveFrmLeftEx,m_pForm->pCFrame);
	m_pForm->GetComponent("right")->SetEvent(MoveFrmRightEx,m_pForm->pCFrame);

	m_pForm->GetComponent("guo1")->SetEvent(SetCountry,(void*)1);
	m_pForm->GetComponent("guo2")->SetEvent(SetCountry,(void*)2);

	m_nCountry = rand()%2+1;
}

CreateRoleMenu::~CreateRoleMenu()
{
	m_pForm->Close();
}
void CreateRoleMenu::Open()
{
	srand(xnGetTickCount());
	nSex = rand()%2;
	nJob = rand()%4+1;

	if (m_nDefCountry >0 && m_nDefCountry <3)
		m_nCountry = m_nDefCountry;

	int nPos = (nSex?0:1)+(nJob-1)*2;

	char*	szFile		= RandNameConfig::GetInstance().GetRndName(nSex);
	m_pForm->GetComponent("name")->SetEdit(szFile);
	m_pForm->pCFrame->nChgX = -(nPos-1)*m_pForm->pCFrame->nDisStep;
	for (int i=0;i<m_pForm->pCFrame->GetFPCnt();i++) 
	{
		//int nColor = (i%8 == 1)?255:70;
		CreateRoleMenu::pMenu->SetPosDark(i,i%8 != nPos);
		//m_pForm->pCFrame->GetFPCompBase(i)->SetSprColor(nColor,nColor,nColor);
	}
	m_pForm->Open();
	UpdateJob();

// 	SetPosAction(nPos,"stand");
// 	SetPosAction(nPos+8,"stand");
	btemp = true;
}

void CreateRoleMenu::update( float dt )
{
	if (!m_pForm->bOpened)
	{
		return;
	}
	if (m_pForm->pCFrame)
	{
		//FormLayer* pLayer = (FormLayer*)m_pForm->pFormLayer;

		//FrameLayer* pFrame	= (FrameLayer*)pLayer->getChildByTag((int)m_pForm->pCFrame);
		//if (pFrame)
		{
			//float fOff = pFrame->getPositionX()*POINT_PIX_CNT;
			for (int i=0;i<m_pForm->pCFrame->GetFPCnt();i++) 
			{
				//float temp = (520-(abs(i*130%(130*8)+fOff-m_pForm->pCFrame->nFrmX-m_pForm->pCFrame->nFrmW/2+75)))/520.0;
				//float temp1 = (520-abs(i%8*130-(int)(m_pForm->pCFrame->nFrmX+130-fOff)%(8*130)))/520.0;
				float temp = (520-abs(i%8*130-(int)(130-m_pForm->pCFrame->nOffChgPos)%(8*130)))/520.0;
				temp = myabs(temp);
				if (temp<0.5)
				{
					temp = 0.5;
				}
				m_pForm->pCFrame->GetFPComp(i,"xingxiang")->SetScale(temp,temp);
				m_pForm->pCFrame->GetFPComp(i,"xingxiang2")->SetScale(temp,temp);
			}
		}

	}

	m_pForm->GetComponent("guo1")->SetSprColor(m_nCountry==2?70:255,m_nCountry==2?70:255,m_nCountry==2?70:255);
	m_pForm->GetComponent("z1")->SetSprColor(m_nCountry==2?70:255,m_nCountry==2?70:255,m_nCountry==2?70:255);
	m_pForm->GetComponent("guo2")->SetSprColor(m_nCountry==2?255:70,m_nCountry==2?255:70,m_nCountry==2?255:70);
	m_pForm->GetComponent("z2")->SetSprColor(m_nCountry==2?255:70,m_nCountry==2?255:70,m_nCountry==2?255:70);
	m_pForm->GetComponent("d1")->SetVisible(m_nCountry == 2);
	m_pForm->GetComponent("d2")->SetVisible(m_nCountry == 1);
	static float dtt = 0;
	static int ndt = 0;
	dtt += dt;


	
	static char* sztxt[] = {"run","stand","atk1","atk2"};
	
	
	int nPos = (nSex?0:1)+(nJob-1)*2;

	
	bool bJump = true;
	for (int i=0;i<m_pForm->pCFrame->GetFPCnt();i++)
	{
		int nColor = m_pForm->pCFrame->GetFPCompBase(i)->nR;
		if (nColor == 255)
		{
			bJump = false;
		}
	}
	if (bJump)
	{
		return;
	}

	if (ndt/3 != (int)dtt/3)
	{
		if (btemp)
		{
			btemp = false;
			ndt = dtt;
			return;
		}
		int nRand = rand ()%4;
		SetPosAction(nPos,sztxt[nRand]);
		SetPosAction(nPos+8,sztxt[nRand]);
		CCLOG("update:%d",nPos);
	}
	ndt = dtt;
}

void CreateRoleMenu::SetPosAction( int nPos,char* szAct )
{
	char szList[64];
	char szFile1[64];
	sprintf(szList,"skin/%d/1.plist",1000+nPos%8+1);
	sprintf(szFile1,"skin/%d/%s/5",1000+nPos%8+1,szAct);
	m_pForm->pCFrame->GetFPComp(nPos,"xingxiang")->SetCartoonAction(szList,szFile1,600,100);
	sprintf(szList,"skin/%d/1.plist",3000+nPos%8+1);
	sprintf(szFile1,"skin/%d/%s/5",3000+nPos%8+1,szAct);
	m_pForm->pCFrame->GetFPComp(nPos,"xingxiang2")->SetCartoonAction(szList,szFile1,600,100);
 	if (!strncmp(szAct,"atk1",4))
 	{
		static int nEff[4][2] = {{142,109},{172,120},{81,63},{59,92}};
 		m_pForm->GetComponent("xxx")->SetEffect(nEff[nJob-1][nSex],1);
 	}else if (!strncmp(szAct,"atk2",4)){
 		static int nEff[4][2] = {{142,109},{172,120},{81,63},{59,92}};
 		m_pForm->GetComponent("xxx")->SetEffect(nEff[nJob-1][nSex],1);
 	}else{
		m_pForm->GetComponent("xxx")->ClearEffect();
	}
}

void CreateRoleMenu::SetPosDark( int nPos,bool bDark )
{
	m_pForm->pCFrame->GetFPComp(nPos,"xingxiang")->SetSprColor(bDark?70:255,bDark?70:255,bDark?70:255);
	m_pForm->pCFrame->GetFPComp(nPos,"xingxiang2")->SetSprColor(bDark?70:255,bDark?70:255,bDark?70:255);
	m_pForm->pCFrame->GetFPCompBase(nPos)->SetSprColor(bDark?70:255,bDark?70:255,bDark?70:255);
}

void CreateRoleMenu::UpdateJob()
{
	int nPos =  (CreateRoleMenu::pMenu->nSex?0:1)+(CreateRoleMenu::pMenu->nJob-1)*2;

	m_pForm->GetComponent("p")->SetViewSpr(szUserPic[nPos]);
	m_pForm->GetComponent("zy")->SetViewSpr(szUserJob2[nPos]);
	m_pForm->GetComponent("zhiye")->SetViewSpr(szUserJob[nPos]);
	m_pForm->GetComponent("xingbie")->SetViewSpr(szUserSex[nPos]);
	SetPosAction(nPos%8,"atk1");
	SetPosAction(nPos%8+8,"atk1");
	CCLOG("updatejob:%d",nPos);
	//
}


CreateRoleMenu* CreateRoleMenu::pMenu = NULL;




void ClickRoleButton(int nPos,char* pName,void* param)
{
	if(param)
	{
		ChooseRoleMenu* pMenu = (ChooseRoleMenu*)param;
		pMenu->m_nChoosen = nPos;
	}
}

ChooseRoleMenu::ChooseRoleMenu()
{
	m_pForm = pMainInterface->GetDForm("xuanjue");
	DComponent* pCompBase = m_pForm->GetComponent("k1");
	DComponent* ppComp[10];
	ppComp[0] = m_pForm->GetComponent("di1");
	ppComp[1] = m_pForm->GetComponent("xuan");
	ppComp[2] = m_pForm->GetComponent("model2");
	ppComp[3] = m_pForm->GetComponent("model");
	ppComp[4] = m_pForm->GetComponent("model3");
	ppComp[5] = m_pForm->GetComponent("MB");
	ppComp[6] = m_pForm->GetComponent("name");
	ppComp[7] = m_pForm->GetComponent("lv");
	ppComp[8] = m_pForm->GetComponent("guojia");
	ppComp[9] = m_pForm->GetComponent("gj");
	m_pForm->pCFrame = new CCtrlFrame(pCompBase->nX,pCompBase->nY,(pCompBase->nW+30)*3,pCompBase->nH);
	m_pForm->pCFrame->InitFParts(pCompBase,ppComp,10);
	m_pForm->pCFrame->nSpace = 30;
	m_pForm->pCFrame->nDisStep = pCompBase->nW+30;
	m_pForm->pCFrame->SetEventEx(ClickRoleButton,this);
	m_pForm->pCFrame->SetFPCnt(0);
	m_pRoleList = xnList::Create();
	m_nChoosen = LoadIntegerFromXML("role_choosen",0);
	m_pForm->GetComponent("left")->SetEvent(MoveFrmLeft,m_pForm->pCFrame);
	m_pForm->GetComponent("right")->SetEvent(MoveFrmRight,m_pForm->pCFrame);
}

ChooseRoleMenu::~ChooseRoleMenu()
{
	if(m_pRoleList)
	{
		while(m_pRoleList->Count)
			delete (USimpleInfo*)m_pRoleList->Delete(0);
		m_pRoleList->Free();
	}
	m_pForm->Close();
}

void	ChooseRoleMenu::Open(ACCOUNT_SIMPLEINFOS_Data* pData)
{
	if(pData != NULL && pData->bCnt%100 > 0)
	{
		while(m_pRoleList->Count)
			delete (USimpleInfo*)m_pRoleList->Delete(0);
		for(int i = 0; i < pData->bCnt%100; i ++)
		{
			USimpleInfo* pNew = new USimpleInfo();
			memcpy(pNew,&pData->szUSInfos[i],sizeof(USimpleInfo));
			m_pRoleList->Add(pNew);
		}
	}
	
	if(m_nChoosen < 0)
		m_nChoosen = 0;
	if(m_nChoosen >= m_pRoleList->Count)
		m_nChoosen = m_pRoleList->Count-1;
	m_pForm->Open();
}

void	ChooseRoleMenu::Delete(int nUin)
{
	for(int i = 0; i < m_pRoleList->Count; i ++)
	{
		USimpleInfo* pCheck = (USimpleInfo*)m_pRoleList->Items[i];
		if(pCheck->dwUin == nUin)
		{
			m_pRoleList->Delete(i);
			delete pCheck;
			break;
		}
	}
}

void	ChooseRoleMenu::Close()
{
	m_pForm->Close();
}

void	ChooseRoleMenu::Update()
{
	if(m_pForm && m_pForm->bOpened)
	{
		int nCnt = m_pForm->pCFrame->GetFPCnt();
		if(nCnt != m_pRoleList->Count)
		{
			nCnt = m_pRoleList->Count;
			m_pForm->pCFrame->SetFPCnt(nCnt,true);
		}
		DComponent* pComp = NULL;
		for(int i = 0; i < nCnt; i ++)
		{
			USimpleInfo* pInfo = (USimpleInfo*)m_pRoleList->Items[i];
			m_pForm->pCFrame->GetFPComp(i,"name")->SetCaptionEx("%s",1,pInfo->szNick);
			m_pForm->pCFrame->GetFPComp(i,"lv")->SetCaptionEx("%d",1,pInfo->byLv);
			pComp = m_pForm->pCFrame->GetFPComp(i,"gj");
			pComp->SetCaptionEx("%s",1,szCountryTxt[pInfo->byCountry]);
			pComp->m_nFontSize = 28;
			pComp = m_pForm->pCFrame->GetFPComp(i,"guojia");
			if(pInfo->byCountry == 1)
				pComp->SetViewSpr("MB_G11.png");
			else
				pComp->SetViewSpr("MB_G21.png");
			pComp = m_pForm->pCFrame->GetFPComp(i,"xuan");
			if(i == m_nChoosen)
				pComp->SetVisible(true);
			else
				pComp->SetVisible(false);
			pComp = m_pForm->pCFrame->GetFPComp(i,"model");
			SetCharToDCom(pComp,pInfo->byJob,pInfo->bySex);
			DComponent* pCModel1 = m_pForm->pCFrame->GetFPComp(i,"model3");
			DComponent* pCModel2 = m_pForm->pCFrame->GetFPComp(i,"model2");
			bool bCheck = true;
			if(pInfo->byJob == 2)
				bCheck = false;
			if(bCheck)
			{
				pCModel1->SetVisible(true);
				pCModel2->SetVisible(false);
			}
			else
			{
				pCModel1->SetVisible(false);
				pCModel2->SetVisible(true);
			}
			SetWeaponToDCom(bCheck?pCModel1:pCModel2,pInfo->byJob,pInfo->bySex);
		}
	}
}