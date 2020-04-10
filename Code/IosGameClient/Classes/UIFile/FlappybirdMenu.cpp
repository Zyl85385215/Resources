#include "UserData.h"
#include "FlappybirdMenu.h"
#include "MainMenu.h"
#include "SoundCenter.h"

void ClickBirdFlyEvent(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	if (param)
	{
		FlappyBirdMenu* pMenu = (FlappyBirdMenu*)param;
		if (pMenu && pMenu->m_bCanChick)
			pMenu->ReSetFly();
	}
}

void fun_StartFlappyGame(void* pParam, int nVal)
{
	if (nVal)
		pChar->SendCmd(USER, C_FLAPPYBIRD_OPEN);
}

void ClickStartFly(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if (!param)
		return;
	if (pChar->m_pClientFlag->GetFlag(105) > 0)
		pMainMenu->PopAskMsgBox("确定消耗200000金币进行游戏吗？",fun_StartFlappyGame,(void*)0);
	else
		pChar->SendCmd(USER, C_FLAPPYBIRD_OPEN);
}

void ClickLeaveGame(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	if (param)
	{
		FlappyBirdMenu* pMenu = (FlappyBirdMenu*)param;
		pMenu->Close();

	}
}


FlappyBirdMenu::FlappyBirdMenu()
{
	m_pForm = pMainInterface->GetDForm("xiaoniao");

	m_pCompBird = m_pForm->GetComponent("niao");
	m_pForm->GetComponent("xx")->SetEvent(ClickBirdFlyEvent,this);

	char szTxt[32];
	for (int i = 0; i < 6; i++)
	{
		sprintf(szTxt,"zhuzi%d",i+1);
		m_pCompColumn[i] = m_pForm->GetComponent(szTxt);
		m_nCOriginalX[i] = m_pCompColumn[i]->nX;
	}

	for (int j = 0; j < 9; j++)
	{
		sprintf(szTxt,"K%d",j+1);
		m_pCompRoad[j] = m_pForm->GetComponent(szTxt);
		m_nROriginalX[j] = m_pCompRoad[j]->nX;
	}

	m_pStartForm = pMainInterface->GetDForm("xiaoniaostart");
	m_pStartForm->GetComponent("ks")->SetEvent(ClickStartFly,this);
	m_pStartForm->GetComponent("close")->SetEvent(ClickLeaveGame,this);

	m_pGameOverForm = pMainInterface->GetDForm("xiaoniaoend");
	m_pGameOverForm->GetComponent("ks")->SetEvent(ClickStartFly,this);
	m_pGameOverForm->GetComponent("close")->SetEvent(ClickLeaveGame,this);

	m_nCOriginalW = m_pCompColumn[0]->nY - m_pCompColumn[1]->nY;
	m_nCOriginalY = m_pCompColumn[1]->nY;

	for (int k = 0; k < 3; k++)
	{
		m_bPass[k] = false;
		m_nCMiddleWidth[k] = m_nCOriginalW;
	}

	m_bStart = false;
	m_bDeath = false;
	m_bInitColumnPos  = true;
	m_nChgY = 0;
	m_nFlyTimes = 0;
	m_dwStarFlyTick = 0;
	m_nClapRate = 200;
	m_nNowGameScore = 0;

	m_nBOriginalX = m_pCompBird->nX;
	m_nBOriginalY = m_pCompBird->nY;
	m_bCanChick = false;
}

FlappyBirdMenu::~FlappyBirdMenu()
{
	
}

void FlappyBirdMenu::OpenForm()
{
	m_pForm->Open();
	m_pStartForm->Open(2);
	m_pStartForm->GetComponent("cs")->SetCaptionEx("%d",1,(pChar->m_pClientFlag->GetFlag(105)>0?0:1));

//	m_pStartForm->GetComponent("jinbi")->SetCaptionEx("%d",1,200000);
	//m_pStartForm->GetComponent("cs")->SetCaptionEx("%d",1,(pChar->m_pClientFlag->GetFlag(105)>0?0:1));

	m_bInitColumnPos = true;
	InitStart();
}

void FlappyBirdMenu::Open()
{
	if (pChar->m_byLvl < 5)
	{
		pMainMenu->PopTipMsg("等级达到5级开启飞跃大海之旅");
		return;
	}
}


void FlappyBirdMenu::StartGame()
{
	m_pStartForm->Close();
	m_pGameOverForm->Close();
	InitStart();
	ReSetFly();
	m_bCanChick = true;
}

void FlappyBirdMenu::Close()
{
	//BeDead();
	m_pStartForm->Close();
	m_pGameOverForm->Close();
	m_pForm->Close();
	m_bCanChick = false;
}

void FlappyBirdMenu::InitStart()
{
	m_nNowGameScore = 0;
	m_bDeath = false;
	m_nChgY = 0;
	m_nFlyTimes = 0;
	m_nClapRate = 200;
	m_dwStarFlyTick = xnGetTickCount()/30;

	m_pCompBird->SetPos(m_nBOriginalX,m_nBOriginalY);
	m_pCompBird->SetViewSpr("MB_XIAONIAO1");

	m_pForm->GetComponent("z1")->SetCaptionEx("%d",1,0);

	int nColumnTeam = 0;

	if (m_bInitColumnPos == true)
	{
		m_bInitColumnPos = false;

		for (int i = 0; i < 6;)
		{
			ColumnRandChgY(m_pCompColumn[i],m_pCompColumn[i+1],nColumnTeam,true);
			i = i + 2;
			nColumnTeam++;
		}
	}

	for (int j = 0; j < 9; j++)
	{
		m_pCompRoad[j]->SetPosX(m_nROriginalX[j]);
	}
	for (int k = 0; k < 3; k++)
	{
		m_bPass[k] = false;
	}
}

void FlappyBirdMenu::ReSetFly()
{
	if (!m_bStart&&!m_bDeath)
		m_bStart = true;

	m_nClapRate = 80;
	m_nFlyTimes = 15;
	m_dwStarFlyTick = xnGetTickCount()/30;
}

void FlappyBirdMenu::BeDead()
{
	m_bCanChick = false;
	m_bStart = false;
	m_bDeath = true;
	m_bInitColumnPos = true;
	m_nFlyTimes = 30;

	m_pGameOverForm->GetComponent("s2")->SetCaptionEx("%d",1,m_nNowGameScore);
	m_pGameOverForm->GetComponent("jinbi")->SetCaptionEx("%d",1,200000);
	g_pSoundCenter->PlayerEffect ("Sound/sfx_die");
	m_pGameOverForm->Open();

	if (pChar->m_pClientFlag->GetFlag(105) < 5)
	{
		BuildPacketEx(pPacket,USER,C_FLAPPYBIRD_SCORE,buf,256);
		SET_DATA(pData,USER,C_FLAPPYBIRD_SCORE,pPacket);
		pData->wScore	= m_nNowGameScore;
		pChar->SendPacket(pPacket);
	}
}

static int nRandNarrow[3] = {40,60,80};
static int nRandWidth[3] = {100,120,140};

void FlappyBirdMenu::ColumnRandChgY(DComponent* pComp1,DComponent* pComp2,int nTeamNum,bool bInit)
{
	int nChgY = 0;
	if (pComp2->nY > m_nCOriginalY - 200 &&pComp2->nY < m_nCOriginalY + 100)
	{
		nChgY = m_nCOriginalY + 100 - rand()%300;
	}else
		nChgY = m_nCOriginalY;

	pComp2->SetPos((bInit?m_nCOriginalX[nTeamNum*2+1]:pComp2->nX) + pComp2->nW + 1024,nChgY);
	m_nCMiddleWidth[nTeamNum] = m_nCOriginalW + (rand()%100>m_nNowGameScore/10*10?nRandWidth[(rand()%3)]:nRandNarrow[(rand()%3)]);
	pComp1->SetPos((bInit?m_nCOriginalX[nTeamNum*2]:pComp1->nX) + pComp1->nW + 1024,nChgY + m_nCMiddleWidth[nTeamNum]);
	
}

bool FlappyBirdMenu::CheckCrash(DComponent* pBird,DComponent* pColumn,int nMiddleWidth)
{
	if(pBird == NULL||pColumn == NULL)
		return false;

	if(pBird->nY + pBird->nH > m_pCompRoad[0]->nY)
		return false;

	if((pBird->nX+pBird->nW)>pColumn->nX&&pBird->nX<(pColumn->nX+pColumn->nW))
	{
		if(pBird->nY<(pColumn->nY+pColumn->nH)||(pBird->nY+pBird->nH)>(pColumn->nY+nMiddleWidth))
			return false;
	}

	return true;
}

void FlappyBirdMenu::Update()
{
	if (m_pForm->bOpened == false)
		return;

	if (m_pStartForm->bOpened)
	{
		int nChgTime = xnGetTickCount()/300%2;
		if (nChgTime == 0)
		{
			m_pStartForm->GetComponent("niao")->SetViewSpr("MB_XIAONIAO1.png");
		}
		if (nChgTime == 1)
		{
			g_pSoundCenter->PlayerEffect ("Sound/sfx_swooshing");
			m_pStartForm->GetComponent("niao")->SetViewSpr("MB_XIAONIAO2.png");
		}
	}

	if(m_bDeath)
	{
		m_pCompBird->SetViewSpr("MB_XIAONIAO3.png");
		if(xnGetTickCount()/30 > m_dwStarFlyTick&&m_pCompBird->nY < m_pCompRoad[0]->nY)
	      {
	     	m_dwStarFlyTick = xnGetTickCount()/30;
		    m_nChgY = (m_nFlyTimes - 15)*(m_nFlyTimes - 15)/30 - 8;
	        m_pCompBird->SetPos(m_pCompBird->nX,m_pCompBird->nY+m_nChgY);
		    m_nFlyTimes++;
	      }
	}

	if (m_bStart == false)
		return;

	{
		int nChgTime = xnGetTickCount()/m_nClapRate%2;
		if (nChgTime == 0)
		{
			m_pCompBird->SetViewSpr("MB_XIAONIAO1.png");
		}
		if (nChgTime == 1)
		{
			m_pCompBird->SetViewSpr("MB_XIAONIAO2.png");
			g_pSoundCenter->PlayerEffect ("Sound/sfx_wing");
		}
	}

	if(xnGetTickCount()/30 > m_dwStarFlyTick)
	{
		m_dwStarFlyTick = xnGetTickCount()/30;
		m_nChgY = (m_nFlyTimes - 15)*(m_nFlyTimes - 15)/30 - 8;
	    m_pCompBird->SetPos(m_pCompBird->nX,m_pCompBird->nY+m_nChgY);
		m_nFlyTimes++;
		if (m_nChgY>0)
			m_nClapRate = 200;
	}

	for (int i = 0; i < 9; i++)
	{
		m_pCompRoad[i]->SetPosX(m_pCompRoad[i]->nX-10);
		if (m_pCompRoad[i]->nX < m_nROriginalX[0] - m_pCompRoad[i]->nW)
			m_pCompRoad[i]->SetPosX(m_nROriginalX[8]);
	}

	int nColumnTeam = 0;
	for (int i = 0; i < 6;)
	{
		if (m_pCompColumn[i]->nX<m_pCompBird->nX&&m_bPass[nColumnTeam] == false)
		{
			m_bPass[nColumnTeam] = true;
			m_nNowGameScore++;

			g_pSoundCenter->PlayerEffect ("Sound/sfx_point");
		}

		if(CheckCrash(m_pCompBird,m_pCompColumn[i+1],m_nCMiddleWidth[nColumnTeam]) == false)
		{
			g_pSoundCenter->PlayerEffect ("Sound/sfx_hit");
			BeDead();
			break;
		}

		m_pCompColumn[i]->SetPosX(m_pCompColumn[i]->nX-10);
		m_pCompColumn[i+1]->SetPosX(m_pCompColumn[i+1]->nX-10);

		if (m_pCompColumn[i]->nX < -m_pCompColumn[i]->nW)
		{
			m_bPass[nColumnTeam] = false;
			ColumnRandChgY(m_pCompColumn[i],m_pCompColumn[i+1],nColumnTeam,false);
		}

		i = i + 2;
		nColumnTeam++;
	}

	m_pForm->GetComponent("z1")->SetCaptionEx("%d",1,m_nNowGameScore);
}
