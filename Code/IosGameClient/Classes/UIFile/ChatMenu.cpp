#include "UserData.h"
#include "ChatMenu.h"
#include "MainMenu.h"
#include "ItemConfig.h"
#include "FaceConfig.h"
#include "xnExtstr.h"
#include "xnlist.h"
extern void	SetItemToDCom(DComponent* pDom,ItemObj* pObj);
extern void	SetCharToDCom(DComponent* pDom,BYTE byJob,BYTE bySex,char* szAction = "stand",int nDirection = 5);
extern void	SetWeaponToDCom(DComponent* pDom,int nID,char* szAction = "stand",int nDirection = 5);
extern void	SetCharToDCom(DComponent* pDom,int	nSkin,char* szAction = "stand",int nDirection = 5);
static char* szChatChannel[] = {"系统","世界","国家","私聊","当前","队伍","军团"};
static BYTE szChatColor[][3] = {
	220,20,60,
	255,140,0,
	0,139,139,
	100,149,237,
	245,245,220,
	0,100,0,
	0,255,255,
	255,255,0,
};

static char* szVipIcon[] = {"","ui/VIP1.png","ui/VIP2.png","ui/VIP3.png","ui/VIP4.png","ui/VIP5.png","ui/VIP6.png","ui/VIP7.png","ui/VIP8.png","ui/VIP9.png","ui/VIP10.png","ui/VIP11.png","ui/VIP12.png",};

void	ClickNameProcessButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	int i = atoi(&pEvent->pComponent->szName[2]);
	if(param)
	{
		ChatNameProcessMenu* pMenu = (ChatNameProcessMenu*)param;
		switch(i)
		{
		case 1:
			{
				BuildPacketEx(pPacket,FRIEND,C_P2PASK,buf,256);
				SET_DATA(pData, FRIEND,C_P2PASK,pPacket);
				strcpy(pData->szName,pMenu->m_pName);
				pData->bType	= PAT_FRIEND;
				pChar->SendPacket(pPacket);		
			}
			break;
		case 2:
			{
				pChar->InvitePlayer(pMenu->m_nUin);
			}
			break;
		case 3:
			{
				BuildPacketEx(pPacket,USER,C_GET_EQUIPINFO,buf,256);
				SET_DATA(pData, USER,C_GET_EQUIPINFO,pPacket);
				pData->dwUin = pMenu->m_nUin;
				pChar->SendPacket(pPacket);
			}
			break;
		case 4:
			{
				pMainMenu->m_pChatMenu->SetTargent(pMenu->m_pName);
			}
			break;
		}
		pMenu->Close();
	}
}

ChatNameProcessMenu::ChatNameProcessMenu()
{
	m_pForm	= pMainInterface->GetDForm("liaotianf2");
	m_pForm->bZorde = FORM_ZORDER_CHAT_TIP;
	m_nUin = 0;
	m_pName = NULL;

	char szTemp[32];
	for(int i = 0; i < 5; i ++)
	{
		sprintf(szTemp,"pd%d",i+1);
		m_pForm->GetComponent(szTemp)->SetEvent(ClickNameProcessButton,this);
	}
}

ChatNameProcessMenu::~ChatNameProcessMenu()
{
	m_pName = NULL;
}

void	ChatNameProcessMenu::Open(int nUin,int nx,int ny,char* pName)
{
	m_nUin = nUin;
	if(m_pName)
		free(m_pName);
	m_pName = strdup(pName);
	m_pForm->SetPos(nx,ny);
	m_pForm->Open();
}

void	ChatNameProcessMenu::Close()
{
	if(m_pForm->bOpened)
		m_pForm->Close();
}

void ClickPageButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		ChatMenu* pMenu = (ChatMenu*)param;
		int i = atoi(&pEvent->pComponent->szName[1]);
		if (i>0 && i<8)
		{
			pMenu->SetChannel(i);
		}
		pMenu->SetPage(i);
	}
}

void ClickChannelButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		ChatMenu* pMenu = (ChatMenu*)param;
		int i = atoi(&pEvent->pComponent->szName[2]);
		pMenu->SetChannel(i);
		pMenu->SetPage(i);
		pMenu->m_pButtonForm->Close();
	}
}

void ClickShowButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		ChatMenu* pMenu = (ChatMenu*)param;
		if(pMenu->m_pButtonForm->bOpened)
			pMenu->m_pButtonForm->Close();
		else
			pMenu->m_pButtonForm->Open();
	}
}

void ClickShowVoiceButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		ChatMenu* pMenu = (ChatMenu*)param;
		pMenu->m_ifSpeek = !pMenu->m_ifSpeek;
	}
}

void ClickdSendButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		ChatMenu* pMenu = (ChatMenu*)param;
		pMenu->Send();
	}
}

void ClickItemButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		ChatMenu* pMenu = (ChatMenu*)param;
		if(pMenu->m_pItemMenu->m_pForm->bOpened)
			pMenu->m_pItemMenu->Close();
		else
			pMenu->m_pItemMenu->Open();
	}
}

void ClickFaceButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		ChatMenu* pMenu = (ChatMenu*)param;
		//pMenu->InsertFace(1001 + rand()%10);
		if(pMenu->m_pFacemenu->m_pForm->bOpened)
			pMenu->m_pFacemenu->Close();
		else
			pMenu->m_pFacemenu->Open();
	}
}

void ClickChatCloseButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		ChatMenu* pMenu = (ChatMenu*)param;
		pMenu->Close();
	}
}

void ClickChatSysButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		ChatMenu* pMenu = (ChatMenu*)param;
		if(pMenu->m_pSysMenu)
			pMenu->m_pSysMenu->Open();
	}
}

ChatMenu::ChatMenu():m_dwWCLmtTick(0)
{
	m_pForm = pMainInterface->GetDForm("liaotian");
	m_pForm->bZorde = FORM_ZORDER_CHAT;
	m_pButtonForm = pMainInterface->GetDForm("liaotianf");
	m_pButtonForm->bZorde = FORM_ZORDER_CHAT_TIP;
	m_pEdit = m_pForm->GetComponent("shuru");
	m_pView = m_pForm->GetComponent("txt");
	m_pView->pSTextCenter->SetSlide(m_pForm->GetComponent("move"),m_pForm->GetComponent("move1"));
	m_nCnt	= 0;
	memset(m_szItemID,0,20);
	memset(m_szTarName,0,20);
	m_strWords.assign("");
	m_bChannel = 2;		//默认国家频道
	m_byPage = 7;		//默认综合页
	m_ifSpeek = false;
	m_pForm->GetComponent("yy")->SetEvent(ClickShowVoiceButton,this);
	m_pForm->GetComponent("pindao")->SetEvent(ClickShowButton,this);
	m_pForm->GetComponent("send")->SetEvent(ClickdSendButton,this);
	m_pForm->GetComponent("bq")->SetEvent(ClickFaceButton,this);
	m_pForm->GetComponent("wupin")->SetEvent(ClickItemButton,this);
	m_pForm->GetComponent("close")->SetEvent(ClickChatCloseButton,this);
	m_pForm->GetComponent("shezhi")->SetEvent(ClickChatSysButton,this);
	char szName[8];
	for(int i = 0; i < 6; i ++)
	{
		sprintf(szName,"pd%d",i+1);
		m_pButtonForm->GetComponent(szName)->SetEvent(ClickChannelButton,this);
	}
	for(int i = 0; i < 8; i ++)
	{
		sprintf(szName,"p%d",i);
		m_pForm->GetComponent(szName)->SetEvent(ClickPageButton,this);
	}
	m_pFaceList = xnList::Create();

	m_pItemMenu = new ItemChooseMenu();
	m_pFacemenu = new FaceChooseMenu();
	m_pProcessMenu = new ChatNameProcessMenu();
	m_pSysMenu	= new ChatSysMenu();

	m_pBack = m_pForm->GetComponent("di0");
	m_pBack->bAlignX	= 0;
	m_pBack->bAlignY	= 0;
	m_pBack->SetViewSprByScale("WHITE.png",95.0,89.0);
}

ChatMenu::~ChatMenu()
{
	if(m_pFaceList)
	{
		while(m_pFaceList->Count)
			m_pFaceList->Delete(0);
		m_pFaceList->Free();
	}
	delete m_pItemMenu;
	delete m_pFacemenu;
	delete m_pProcessMenu;
	delete m_pSysMenu;
}

void ChatMenu::Open()
{
	m_pForm->Open(2);
}

void ChatMenu::Close()
{
	m_pForm->Close();
	m_pButtonForm->Close();
	m_pProcessMenu->Close();
}

void ChatMenu::Update()
{

	if(m_pForm == NULL)
		return;
	//对话显示
	if(pChar->m_pChatCenter->m_bifLineChg)
	{
		pChar->m_pChatCenter->m_bifLineChg = false;
		RefleshPage();
	}
	if(pChar->m_pChatCenter->m_ifSimpleLineChg)
	{
		pChar->m_pChatCenter->m_ifSimpleLineChg = false;
		pMainMenu->m_pChatMenu2->RefreshPage();
	}
	if(m_pForm->bOpened)
	{
		std::string strWords(m_pEdit->GetEdit());
		int nLenF = strWords.size();
		int nLenB = m_strWords.size();
		if(nLenF > nLenB)		//前台字符长，表示输入,后台要补足多余字符
		{
			m_strWords.append(strWords,nLenB,nLenF-nLenB);
		}
		else if(nLenF < nLenB)		//前台字符短，表示删除，后台删除多余字符
		{
			m_strWords.erase(nLenF,nLenB-nLenF);
		}
		int nLimit = 60;
		if(nLenF > nLimit)
		{
			strWords.erase(nLimit,nLenF-nLimit);
			m_pEdit->SetEdit((char*)strWords.c_str());
			m_strWords.erase(nLimit,nLenF-nLimit);
		}
		//检查后台字符中的物品信息数量
		int nStart = 0;
		int nCnt = 0;
		while((nStart=m_strWords.find("/#!WP$",nStart)) != std::string::npos)
		{
			nStart++;
			nCnt ++;
		}
		if(nCnt < m_nCnt)
			m_nCnt = nCnt;

		//界面变化
		for(int i = 0; i < 8; i ++)
		{
			char szName[4];
			sprintf(szName,"p%d",i);
			if(i == m_byPage)
				m_pForm->GetComponent(szName)->SetSprColor(255,255,255);
			else
				m_pForm->GetComponent(szName)->SetSprColor(110,110,110);
		}
		char szName[4];
		sprintf(szName,"pd%d",m_bChannel);
		DComponent* pSB = m_pButtonForm->GetComponent(szName);
		DComponent* pChaBtn = m_pForm->GetComponent("pindao");
		pChaBtn->SetViewSpr(pSB->szSpr);

		if(m_ifSpeek)
		{
			m_pForm->GetComponent("yy")->SetViewSpr("BT_JIANPAN.png");
			m_pForm->GetComponent("fayin")->SetVisible(true);
			m_pEdit->SetVisible(false);
		}
		else
		{
			m_pForm->GetComponent("yy")->SetViewSpr("BT_YUYIN.png");
			m_pForm->GetComponent("fayin")->SetVisible(false);
			m_pEdit->SetVisible(true);
		}
		UpdateFace();	//动态表情刷新

		
	}
	
}

void ChatMenu::InsertItem(ItemObj* pItem)
{
	if(pItem == NULL)
		return;
	if(m_nCnt >= 5)
	{
		pMainMenu->PopTipMsg("最多可显示五件物品");
		return;
	}
	if(m_strWords.size() > 34)
	{
		pMainMenu->PopTipMsg("文字输入不能超过40个字符");
		return;
	}
	m_szItemID[m_nCnt] = pItem->GetVal(ITEM_AD_ID);
	m_nCnt ++;
	std::string strWords(m_pEdit->GetEdit());
	strWords.append("{物品}");		//前台显示
	m_strWords.append("/#!WP$");	//后台字符串
	m_pEdit->SetEdit((char*)strWords.c_str());
}

void ChatMenu::InsertFace(int nIcon)
{
	if(m_strWords.size() > 34)
	{
		pMainMenu->PopTipMsg("文字输入不能超过40个字符");
		return;
	}
	char szName[7];
	sprintf(szName,"/#%04d",nIcon);
	std::string strWords(m_pEdit->GetEdit());
	strWords.append("{表情}");		//前台显示
	m_strWords.append(szName);		//后台字符串
	m_pEdit->SetEdit((char*)strWords.c_str());
}

void ChatMenu::Clear()
{
	m_pEdit->SetEdit("");
	m_strWords.assign("");
	m_nCnt = 0;
	memset(m_szItemID,0,20);
}

void ChatMenu::Send()
{
	BYTE szChannel[] = {CHAT_ROOM_WORLD,CHAT_ROOM_COUNTRY,CHAT_ROOM_MAP,CHAT_ROOM_HOME,CHAT_ROOM_TEAM,CHAT_ROOM_P2P};//世界，国家，地图，公会，队伍，私聊
	if (m_bChannel-1 == 0&&pChar->m_byLvl < 20)
	{
		pMainMenu->PopTipMsg("20级之后才可在世界频道发言");
		return;
	}
	if ((m_bChannel-1 == 0)&&(pChar->m_byLvl >= 20)&&(xnGetSecCount()-m_dwWCLmtTick>((60-((pChar->m_byLvl-20)/10)*10)>5?(60-((pChar->m_byLvl-20)/10)*10):5)))
	{
		m_dwWCLmtTick = xnGetSecCount();
	}else if ((m_bChannel-1 == 0)&&(pChar->m_byLvl >= 20)&&(xnGetSecCount()-m_dwWCLmtTick<((60-((pChar->m_byLvl-20)/10)*10)>5?(60-((pChar->m_byLvl-20)/10)*10):5)))
	{
		pMainMenu->PopTipMsg("%d秒后才可继续发言",((60-((pChar->m_byLvl-20)/10)*10)>5?(60-((pChar->m_byLvl-20)/10)*10):5)-(xnGetSecCount()-m_dwWCLmtTick));
		return;
	}else
		m_dwWCLmtTick = 0;
	if (m_bChannel == CHAT_ROOM_TEAM && !pMainMenu->m_pTeamMenu->GetTeamedCount())
	{
		pMainMenu->PopTipMsg("您尚未组队");
		return;
	}
	if (m_bChannel+2 == CHAT_ROOM_HOME && !pChar->m_dwHomeID)
	{
		pMainMenu->PopTipMsg("您尚未加入军团");
		return;
	}
	pChar->m_pChatCenter->SendMsg(szChannel[m_bChannel-1],(char*)m_strWords.c_str(),m_szItemID,m_nCnt);
	Clear();
	if(strlen(m_szTarName)>0)
		SetTargent(m_szTarName);
}

void ChatMenu::SetChannel(BYTE byCha)
{
	if(byCha >6 || byCha < 1)
		return;
	m_bChannel = byCha;
	memset(m_szTarName,0,20);
	m_pProcessMenu->Close();
}

void ChatMenu::SetPage(BYTE byPg)
{
	if(byPg > 8)
		return;
	m_byPage = byPg;
	RefleshPage();
	m_pProcessMenu->Close();
}

void ChatMenu::SetTargent(char * szName)
{
	m_bChannel = 6;
	char szTemp[128];
	sprintf(szTemp,"对%s说:",szName);
	m_pEdit->SetEdit(szTemp);
	sprintf(szTemp,"//%s//:",szName);
	m_strWords.assign(szTemp);
	sprintf(m_szTarName,szName);
}

void ClickShowID(void* param)
{
	int nID = (int)param;
	ClientShowInfo* pInfo = pChar->m_pChatCenter->GetShowInfo(nID);
	if(pInfo && pInfo->pObj)
	{
		if (pInfo->pObj->pItemBase->byType == ITEMTYPE_EQUIP)
		{
			pMainMenu->m_pEquipInfoMenu->Open(pInfo->pObj,FORM_NULL);
		}else{
			pMainMenu->m_pItemInfoMenu->Open(pInfo->pObj,FORM_NULL);
		}
	}
	else
	{
		pChar->m_pChatCenter->RequestForItemInfo(nID);
	}
}

void ClickNameFunc(void* param)
{
	//点击名字事件	
	_MuliParamObjEx* pCheck = (_MuliParamObjEx*)param;
	if(pCheck->nParam2 != 0)
		return;
	char* pName = (char*)pCheck->nParam1;
	int* pUin = (int*)(&pName[20]);
	//if(strcmp(pName,pChar->m_szNick) == 0 || *pUin==pChar->m_dwUin)
	//	return;

	int nX = pMainMenu->m_pChatMenu->m_pForm->nPosX+pMainMenu->m_pChatMenu->m_pView->nX+pCheck->nParam3/POINT_PIX_CNT;
	int nY = pMainMenu->m_pChatMenu->m_pForm->nPosY+pMainMenu->m_pChatMenu->m_pView->nY+pCheck->nParam4/POINT_PIX_CNT;
	int nBellow = pMainMenu->m_pChatMenu->m_pForm->nPosY+pMainMenu->m_pChatMenu->m_pView->nY + pMainMenu->m_pChatMenu->m_pView->nH - pMainMenu->m_pChatMenu->m_pProcessMenu->m_pForm->GetComponent("di")->nH - 150;
	int nTop = pMainMenu->m_pChatMenu->m_pForm->nPosY+pMainMenu->m_pChatMenu->m_pView->nY;
	if(nY > nBellow)
		nY = nBellow;
	if(nY < nTop)
		nY < nTop;

	pMainMenu->m_pChatMenu->m_pProcessMenu->Open(*pUin,nX,nY,pName);
}

void ChatMenu::RefleshPage()
{
	while(m_pFaceList->Count)
		m_pFaceList->Delete(0);
	m_pView->pSTextCenter->ClearSText();
	xnList* pList = pChar->m_pChatCenter->m_pMsgList[m_byPage];
	int nIndex = 1;
	char szTemp[256];
	for(int i = 0; i < pList->Count; i ++)
	{
		ChatInfo* pMsg = (ChatInfo*)pList->Items[i];
		char szTitle[32];
		if(pMsg->byCRType == CHAT_ROOM_NULL)
		{
			m_pView->pSTextCenter->AddSText(nIndex++,0,true,"",szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
		}
		else
		{
			sprintf(szTitle,"<%s>",szChatChannel[pMsg->byCRType]);
			m_pView->pSTextCenter->AddSText(nIndex++,0,true,szTitle,szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
		}
		if(pMsg->byCRType == CHAT_ROOM_P2P)
		{
			if(pMsg->nSUin == pChar->m_dwUin)
				m_pView->pSTextCenter->AddSText(nIndex++,0,false,"你",szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
			else
			{
				if(pMsg->bySV > 0)
					m_pView->pSTextCenter->AddSText(nIndex++,1,false,szVipIcon[pMsg->bySV],255,255,255);
				STextObj* pCheckST = m_pView->pSTextCenter->AddSText(nIndex++,0,false,pMsg->szSName,szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2],(void*)pMsg->szSName,ClickNameFunc);
				pCheckST->bTouchEvent = true;
			}
			if(pMsg->nRUin == pChar->m_dwUin)
			{
				sprintf(szTemp," 对 你 :");
				m_pView->pSTextCenter->AddSText(nIndex++,0,false,szTemp,szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
			}
			else
			{
				if(pMsg->byRV > 0)
				{
					m_pView->pSTextCenter->AddSText(nIndex++,0,false,"对",szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
					m_pView->pSTextCenter->AddSText(nIndex++,1,false,szVipIcon[pMsg->byRV],255,255,255);
					STextObj* pCheckST = m_pView->pSTextCenter->AddSText(nIndex++,0,false,(char*)pMsg->szRName,szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2],(void*)pMsg->szRName,ClickNameFunc);
					pCheckST->bTouchEvent = true;
					m_pView->pSTextCenter->AddSText(nIndex++,0,false," :",szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
				}
				else
				{
					sprintf(szTemp," 对 %s :",pMsg->szRName);
					STextObj* pCheckST = m_pView->pSTextCenter->AddSText(nIndex++,0,false,(char*)szTemp,szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2],(void*)pMsg->szRName,ClickNameFunc);
					pCheckST->bTouchEvent = true;
				}
			}
		}
		else if(pMsg->nSUin > 0)
		{
			if(pMsg->bySV > 0)
				m_pView->pSTextCenter->AddSText(nIndex++,1,false,szVipIcon[pMsg->bySV],255,255,255);
			sprintf(szTemp,"%s : ",pMsg->szSName);
			STextObj* pCheckST = m_pView->pSTextCenter->AddSText(nIndex++,0,false,(char*)szTemp,szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2],(void*)pMsg->szSName,ClickNameFunc);
			pCheckST->bTouchEvent = true;
		}
		std::string strWords(pMsg->szMsg);
		std::vector<std::string> vec;
		StringSplit(strWords,"/#",vec);
		int nCount = 0;
		for(int j = 0; j < vec.size(); j ++)
		{
			if(j == 0)
			{
				m_pView->pSTextCenter->AddSText(nIndex++,0,false,(char*)vec[j].c_str(),szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
				continue;
			}
			if(vec[j].size() < 4)
				m_pView->pSTextCenter->AddSText(nIndex++,0,false,(char*)vec[j].c_str(),szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
			std::string strKey = vec[j].substr(0,4);
			if(strKey.compare("!WP$") == 0)		//物品
			{
				if(nCount < pMsg->nCnt)
				{
					ItemExBase* pItem = ItemConfig::GetInstance().GetItemBase(pMsg->xInfo[nCount].nItemType);				
					if(pItem)
					{
						char szItemName[32];
						sprintf(szItemName,"[%s]",pItem->szName);
						m_pView->pSTextCenter->AddSText(nIndex++,0,false,szItemName,nItemColor[pItem->nStar][0],nItemColor[pItem->nStar][1],nItemColor[pItem->nStar][2],(void*)pMsg->xInfo[nCount].showID,ClickShowID);
						vec[j].erase(0,4);
						m_pView->pSTextCenter->AddSText(nIndex++,0,false,(char*)vec[j].c_str(),szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
					}
					nCount ++;
				}
				else
				{
					vec[j].erase(0,4);
					m_pView->pSTextCenter->AddSText(nIndex++,0,true,(char*)vec[j].c_str(),szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
				}
			}
			else			//表情
			{
				int nIcon = atoi(strKey.c_str());
				char szIcon[32];
				sprintf(szIcon,"ui/face/%d/1.png",nIcon);
				STextObj* pTextObj = m_pView->pSTextCenter->AddSText(nIndex++,1,false,szIcon,255,255,255);
				pTextObj->nOffY = 1;
				m_pFaceList->Add((void*)pTextObj->nPID);
				vec[j].erase(0,4);
				m_pView->pSTextCenter->AddSText(nIndex++,0,false,(char*)vec[j].c_str(),szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
			}
		}
	}
	m_pView->pSTextCenter->m_bSetBottom = true;
}

void ChatMenu::UpdateFace()
{
	static DWORD dwTick = 40;
	if(m_pFaceList->Count == 0)
		return;
	for(int i = m_pFaceList->Count-1; i >= 0; i --)
	{
		int nPID = (int)m_pFaceList->Items[i];
		STextObj* pObj = m_pView->pSTextCenter->GetSText(nPID);
		int nID = 0;
		int nFrm = 0;
		if(pObj && pObj->szContent)
		{
			sscanf(pObj->szContent,"ui/face/%d/%d.png",&nID,&nFrm);
			FaceObj* pFace = FaceConfig::GetInstance().GetFace(nID);
			if(pFace && (dwTick %pFace->nSpeed) == 0)
			{
				nFrm ++;
				if(nFrm > pFace->nTotalFrm)
					nFrm = 1;
				char szIcon[32];
				sprintf(szIcon,"ui/face/%d/%d.png",nID,nFrm);
				free(pObj->szContent);
				pObj->szContent = strdup(szIcon);
				pObj->bReset = true;
			}
		}
		else
			m_pFaceList->Delete(i);
	}
	dwTick += 40;
}

void ClickFaceIconButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		FaceChooseMenu* pMenu = (FaceChooseMenu*)param;
		int nID;
		sscanf(pEvent->pComponent->szName,"b%d",&nID);
		if(nID <= FaceConfig::GetInstance().m_pList->Count)
		{
			pMainMenu->m_pChatMenu->InsertFace(nID+1000);
			pMenu->Close();
		}
	}
}

FaceChooseMenu::FaceChooseMenu()
{
	m_pForm = pMainInterface->GetDForm("biaoqing");
	m_pForm->bZorde = FORM_ZORDER_CHAT_TIP;
	char szName[32];
	char szIcon[32];
	DComponent * pComp = NULL;
	for(int i = 0; i < FACECOUNT; i ++)
	{
		sprintf(szName,"b%d",i+1);
		pComp = m_pForm->GetComponent(szName);
		if(pComp)
		{
			pComp->SetEvent(ClickFaceIconButton,this);
		}
	}
}

FaceChooseMenu::~FaceChooseMenu()
{

}

void FaceChooseMenu::Open()
{
	char szName[32];
	char szIcon[32];
	DComponent * pComp = NULL;
	for(int i = 0; i < FACECOUNT; i ++)
	{
		sprintf(szName,"b%d",i+1);
		pComp = m_pForm->GetComponent(szName);
		if(pComp)
		{
			if(i < FaceConfig::GetInstance().m_pList->Count)
			{
				FaceObj* pObj = (FaceObj*)FaceConfig::GetInstance().m_pList->Items[i];
				sprintf(szIcon,"face/%d/1.png",pObj->nID);
				pComp->SetViewSpr(szIcon);
			}
			else
			{
				pComp->SetViewSpr("");
			}
		}
	}
	m_pForm->Open(2);
}

void FaceChooseMenu::Close()
{
	m_pForm->Close();
}

void ClickOpenChatButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		SimpleChatMenu* pMenu = (SimpleChatMenu*)param;
		pMainMenu->m_pChatMenu->Open();
	}
}

SimpleChatMenu::SimpleChatMenu()
{
	m_pForm = pMainInterface->GetDForm("liaotian2");
	m_pForm->GetComponent("chat")->SetEvent(ClickOpenChatButton,this);
	m_pView = m_pForm->GetComponent("txt");
	m_pView->pSTextCenter->bDisableTouch = true;
	m_pView->pSTextCenter->bAutoBottom = true;
	m_pView->pSTextCenter->m_byFontSize = 18;
}

SimpleChatMenu::~SimpleChatMenu()
{

}

void SimpleChatMenu::RefreshPage()
{
	m_pView->pSTextCenter->ClearSText();
	xnList* pList = pChar->m_pChatCenter->m_pSimMsgList;
	int nIndex = 1;
	char szTemp[256];
	for(int i = 0; i < pList->Count; i ++)
	{
		ChatInfo* pMsg = (ChatInfo*)pList->Items[i];
		char szTitle[32];
		if(pMsg->byCRType == CHAT_ROOM_NULL)
		{
			m_pView->pSTextCenter->AddSText(nIndex++,0,true,"",szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
		}
		else
		{
			sprintf(szTitle,"<%s>",szChatChannel[pMsg->byCRType]);
			m_pView->pSTextCenter->AddSText(nIndex++,0,true,szTitle,szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
		}
		if(pMsg->byCRType == CHAT_ROOM_P2P)
		{
			if(pMsg->nSUin == pChar->m_dwUin)
				m_pView->pSTextCenter->AddSText(nIndex++,0,false,"你",szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
			else
			{
				if(pMsg->bySV > 0)
					m_pView->pSTextCenter->AddSText(nIndex++,1,false,(char*)szVipIcon[pMsg->bySV],255,255,255);
				m_pView->pSTextCenter->AddSText(nIndex++,0,false,pMsg->szSName,szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
			}
			if(pMsg->nRUin == pChar->m_dwUin)
			{
				sprintf(szTemp," 对 你 :");
				m_pView->pSTextCenter->AddSText(nIndex++,0,false,szTemp,szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
			}
			else
			{
				if(pMsg->byRV > 0)
				{
					m_pView->pSTextCenter->AddSText(nIndex++,0,false,"对",szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
					m_pView->pSTextCenter->AddSText(nIndex++,1,false,szVipIcon[pMsg->byRV],255,255,255);
					m_pView->pSTextCenter->AddSText(nIndex++,0,false,(char*)pMsg->szRName,szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
					m_pView->pSTextCenter->AddSText(nIndex++,0,false," :",szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
				}
				else
				{
					sprintf(szTemp," 对 %s :",pMsg->szRName);
					m_pView->pSTextCenter->AddSText(nIndex++,0,false,(char*)szTemp,szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
				}			}
		}
		else if(pMsg->nSUin > 0)
		{
			if(pMsg->bySV > 0)
				m_pView->pSTextCenter->AddSText(nIndex++,1,false,(char*)szVipIcon[pMsg->bySV],255,255,255);
			sprintf(szTemp,"%s : ",pMsg->szSName);
			m_pView->pSTextCenter->AddSText(nIndex++,0,false,(char*)szTemp,szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
		}
		std::string strWords(pMsg->szMsg);
		std::vector<std::string> vec;
		StringSplit(strWords,"/#",vec);
		int nCount = 0;
		for(int j = 0; j < vec.size(); j ++)
		{
			if(j == 0)
			{
				m_pView->pSTextCenter->AddSText(nIndex++,0,false,(char*)vec[j].c_str(),szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
				continue;
			}
			if(vec[j].size() < 4)
				m_pView->pSTextCenter->AddSText(nIndex++,0,false,(char*)vec[j].c_str(),szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
			std::string strKey = vec[j].substr(0,4);
			if(strKey.compare("!WP$") == 0)		//物品
			{
				if(nCount < pMsg->nCnt)
				{
					ItemExBase* pItem = ItemConfig::GetInstance().GetItemBase(pMsg->xInfo[nCount].nItemType);				
					if(pItem)
					{
						char szItemName[32];
						sprintf(szItemName,"[%s]",pItem->szName);
						m_pView->pSTextCenter->AddSText(nIndex++,0,false,szItemName,nItemColor[pItem->nStar][0],nItemColor[pItem->nStar][1],nItemColor[pItem->nStar][2]);
						vec[j].erase(0,4);
						m_pView->pSTextCenter->AddSText(nIndex++,0,false,(char*)vec[j].c_str(),szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
					}
					nCount ++;
				}
				else
				{
					m_pView->pSTextCenter->AddSText(nIndex++,0,true,(char*)vec[j].c_str(),szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
				}
			}
			else			//表情
			{
				int nIcon = atoi(strKey.c_str());
				char szIcon[32];
				sprintf(szIcon,"ui/face/%d/1.png",nIcon);
				STextObj* pTextObj = m_pView->pSTextCenter->AddSText(nIndex++,1,false,szIcon,255,255,255);
				vec[j].erase(0,4);
				m_pView->pSTextCenter->AddSText(nIndex++,0,false,(char*)vec[j].c_str(),szChatColor[pMsg->byCRType][0],szChatColor[pMsg->byCRType][1],szChatColor[pMsg->byCRType][2]);
			}
		}
	}
}

static void CBFrameFBEvent(int nPos,char* szName)
{
	if(strncmp (szName, "bag",3) == 0)
	{
		ItemObj* pItem = (ItemObj*)pMainMenu->m_pChatMenu->m_pItemMenu->m_pForm->pCFrame->GetFPComp(nPos,szName)->GetTag();
		if (pItem)
		{
			if (pItem->pItemBase->byType == ITEMTYPE_EQUIP)
			{
				pMainMenu->m_pEquipInfoMenu->Open(pItem,pMainMenu->m_pChatMenu->m_pItemMenu->m_nType == -1?FORM_SHOW:pMainMenu->m_pChatMenu->m_pItemMenu->m_nType);
				if (pMainMenu->m_pChatMenu->m_pItemMenu->fCallBackEvent)
				{
					pMainMenu->m_pEquipInfoMenu->SetCallBackEvent(pMainMenu->m_pChatMenu->m_pItemMenu->fCallBackEvent);
					pMainMenu->m_pEquipInfoMenu->SetButtonSpr(pMainMenu->m_pChatMenu->m_pItemMenu->szSpr1,pMainMenu->m_pChatMenu->m_pItemMenu->szSpr2);
				}
			}else{
				pMainMenu->m_pItemInfoMenu->Open(pItem,pMainMenu->m_pChatMenu->m_pItemMenu->m_nType == -1?FORM_SHOW:pMainMenu->m_pChatMenu->m_pItemMenu->m_nType);
				if (pMainMenu->m_pChatMenu->m_pItemMenu->fCallBackEvent)
				{
					pMainMenu->m_pItemInfoMenu->SetCallBackEvent(pMainMenu->m_pChatMenu->m_pItemMenu->fCallBackEvent);
					pMainMenu->m_pItemInfoMenu->SetButtonSpr(pMainMenu->m_pChatMenu->m_pItemMenu->szSpr1,pMainMenu->m_pChatMenu->m_pItemMenu->szSpr2);
				}
			}
		}
	}
}
static void CBFrameFBStepEvent(int nPos)
{
	if (nPos!=pMainMenu->m_pChatMenu->m_pItemMenu->m_nSelectPage)
	{
		pMainMenu->m_pChatMenu->m_pItemMenu->m_nSelectPage = nPos;
		if (nPos>=1)
		{
			pMainMenu->m_pChatMenu->m_pItemMenu->UpdatePage(nPos-1,false);
		}
		//pMainMenu->m_pBagMenu->UpdatePage(nPos,true);
		if (nPos<nBagPageMax-1)
		{
			pMainMenu->m_pChatMenu->m_pItemMenu->UpdatePage(nPos+1,false);
		}
	}
	pMainMenu->m_pChatMenu->m_pItemMenu->UpdatePage(nPos,true);
}

static void ClickEquipButton(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	ItemObj* pItem = (ItemObj*)pEvent->pComponent->GetTag();
	if (pItem&&pMainMenu->m_pChatMenu->m_pItemMenu->m_nType == -1)
	{
		//pMainMenu->m_pChatMenu->InsertItem(pItem);
		//pMainMenu->m_pChatMenu->m_pItemMenu->Close();
		pMainMenu->m_pEquipInfoMenu->Open(pItem,FORM_SHOW);
	}
}

static void ClickCloseMenu(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	
	pMainMenu->m_pChatMenu->m_pItemMenu->Close();
}

extern char* szEquipComp[];
extern void	SetItemToDCom(DComponent* pDom,ItemObj* pObj);

ItemChooseMenu::ItemChooseMenu():m_nType(-1),nFilter(NULL),m_pParam(NULL),m_bLockEquip(false),szSpr1(NULL),szSpr2(NULL),fCallBackEvent(NULL),m_bReSort(false)
{
	m_pForm = pMainInterface->GetDForm("xuanbag");
	m_pForm->bZorde = FORM_ZORDER_CHAT_TIP;
	m_bUpdate = true;
	m_nSelectPage = 0;
	char sztxt[32];
	DComponent* pComItem[20];
	for (int i=0;i<20;i++)
	{
		sprintf(sztxt,"bag%d",i+1);
		pComItem[i] = m_pForm->GetComponent(sztxt);
		pComItem[i]->SetVisible(false);
	}
	m_pForm->pCFrame	= new CCtrlFrame(445,145,485,340);
	DComponent*	pComp	= m_pForm->GetComponent ("di3");

	m_pForm->pCFrame->InitFParts(pComp,pComItem,20);
	m_pForm->pCFrame->SetEvent (CBFrameFBEvent);
	m_pForm->pCFrame->SetStepEvent(CBFrameFBStepEvent);
	m_pForm->pCFrame->nSpace	= 55;
	m_pForm->pCFrame->nDisStep	= 510;

	m_pForm->pCFrame->SetFPCnt(nBagPageMax,true);
	for (int i=0;i<10;i++)
	{
		m_pEquip[i] = m_pForm->GetComponent(szEquipComp[i]);
		m_pEquip[i]->SetEvent(ClickEquipButton);
	}	
	m_pForm->GetComponent("close")->SetEvent(ClickCloseMenu,this);
}

ItemChooseMenu::~ItemChooseMenu()
{

}

void ItemChooseMenu::Open()
{
	m_nSelectPage = 0;
	m_pForm->pCFrame->nChgX = 0;
	m_bUpdate = true;
	m_pForm->Open(2);
}

void ItemChooseMenu::Close()
{
	m_nType = -1;
	fCallBackEvent = NULL;
	nFilter = NULL;
	m_pParam = NULL;
	m_bLockEquip = false;
	m_pForm->Close();
	m_bReSort = false;
}

void ItemChooseMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}
	if (m_bUpdate)
	{
		m_bUpdate = false;

		bool bSwitch = true;
		if (pChar->m_byJob==2)
		{
			bSwitch = false;
		}
		if (pChar->m_xUserParam.nSkin > 1500)
		{
			m_pForm->GetComponent((char*)(bSwitch?"model":"model2"))->SetVisible(false);
		}else{
			m_pForm->GetComponent((char*)(bSwitch?"model":"model2"))->SetVisible(true);
			SetWeaponToDCom(m_pForm->GetComponent((char*)(bSwitch?"model":"model2")),pChar->m_pItemCenter->GetWeaponSkin());
		}
		SetCharToDCom(m_pForm->GetComponent(bSwitch?(char*)"model2":(char*)"model"),pChar->m_xUserParam.nSkin);
// 		char szList[64];
// 		char szFile[64];
// 		sprintf(szList,"skin/%d/1.plist",1000+pChar->m_byJob*2+(pChar->m_bySex?0:1));
// 		sprintf(szFile,"skin/%d/stand/5",1000+pChar->m_byJob*2+(pChar->m_bySex?0:1));
// 		m_pForm->GetComponent("model")->SetCartoonAction(szList,szFile,600,100);

		m_pForm->GetComponent("zuan")->SetCaptionEx("%d", 1, pChar->m_dwRmb);
		m_pForm->GetComponent("jinbi")->SetCaptionEx("%d", 1, pChar->m_dwMoney);
		m_pForm->GetComponent("money")->SetCaptionEx("%d",1,pChar->m_dwBindRmb);
		m_pForm->GetComponent("zd")->SetCaptionEx("%d",1,pChar->GetScore());

		char sztxt[32];
		char szSpr[32];
		if (m_nSelectPage == 0)
		{
			UpdatePage(1,false);
		}
		UpdatePage(m_nSelectPage,true);

//		xnList* pList = xnList::Create();
// 		pChar->m_pItemCenter->GetItemInBagByPos(pList);
// 		for (int i= 0;i<pList->Count;i++)
// 		{
// 			ItemObj* pItem = (ItemObj*)pList->Items[i];
// 			int nPos = pItem->GetVal(ITEM_AD_POS);
// 			if (nPos<m_nSelectPage*20+20&&nPos>=m_nSelectPage*20)
// 			{
// 				sprintf(szSpr,"bag%d",nPos%20+1);
// 				SetItemToDCom(m_pForm->pCFrame->GetFPComp(m_nSelectPage,szSpr),pItem);
// 			}	
// 		}
//		pList->Free();

		for (int i=0;i<10;i++)
		{
			m_pEquip[i]->SetViewSpr("");
			m_pEquip[i]->SetTag(NULL);
		}
		//if (!m_bLockEquip)
		{
			for (int i=0;i<10;i++)
			{
				ItemObj* pItem = pChar->m_pItemCenter->GetEquipBySlot(i);
				if (pItem)
				{
					//sprintf(sztxt,"item/%d.png",pItem->pItemBase->nSpr);
					//m_pEquip[i]->SetViewSpr(sztxt);
					//m_pEquip[i]->SetTag(pItem);
					SetItemToDCom(m_pEquip[i],pItem);
					if(m_bLockEquip) m_pEquip[i]->SetEvent(NULL);
				}

			}
		}
		
	}
}

void ItemChooseMenu::UpdatePage( int nSelectPage,bool bSelect )
{
	char sztxt[32];
	char szSpr[32];
	if (nSelectPage < 0)
	{
		return;
	}
	sprintf(sztxt,"BT_SHAIZI.png");
	for (int i=nSelectPage*20;i<nSelectPage*20+20;i++)
	{
		sprintf(szSpr,"bag%d",i%20+1);
		if (i>=pChar->m_pItemCenter->m_nBagSize)
		{
			SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),NULL);
			m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr)->SetViewSpr("MB_SUO.png");
		}else{
			SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),NULL);
		}
		/*SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),NULL);*/
	}

	xnList* pList = xnList::Create();
	if (nFilter)
	{
		pChar->m_pItemCenter->GetItemInBagByFilter(pList,nFilter,m_pParam);
	}else{
		pChar->m_pItemCenter->GetItemInBagByPos(pList);

	}
	int k = 0;
	for (;k<pList->Count;k++)
	{
		ItemObj* pItem = (ItemObj*)pList->Items[k];
		int nPos = m_bReSort?k:pItem->GetVal(ITEM_AD_POS);
		if (nPos<nSelectPage*20+20&&nPos>=nSelectPage*20)
		{
			sprintf(szSpr,"bag%d",nPos%20+1);
			SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),pItem);
		}	
	}
// 	for(;k < 20; k ++)
// 	{
// 		sprintf(szSpr,"bag%d",k+1);
// 		SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),NULL);
// 	}
	pList->Free();
	if (bSelect)
	{
		for (int i=0;i<nBagPageMax;i++)
		{
			sprintf(sztxt,"p%d",i+1);
			m_pForm->GetComponent(sztxt)->SetViewSpr( i== nSelectPage?(char*)"MB_DIAN_B.png":(char*)"MB_DIAN_S.png");
		}
	}

}

void ItemChooseMenu::SetType( int nType )
{
	m_nType = nType;
}

void ItemChooseMenu::SetFilter( bool (*nFun)(ItemObj* pObj,void* param),void* param /*= NULL*/ )
{
	nFilter = nFun;
	m_pParam = param;
}

void ItemChooseMenu::SetLockEquip( bool bLock )
{
	m_bLockEquip = bLock;
}

void ItemChooseMenu::SetChooseCallback( void (*fnCallBack)(void* pParam,BYTE byType) )
{
	fCallBackEvent = fnCallBack;
}

void ItemChooseMenu::SetButtonSpr( const char* szSpr1,const char* szSpr2 /*= NULL*/ )
{
	this->szSpr1 = szSpr1;
	this->szSpr2 = szSpr2;
}

void ClickCSysOkButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		ChatSysMenu* pMenu = (ChatSysMenu*)param;
		pMenu->Close(true);
	}
}

void ClickCSysCloseButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		ChatSysMenu* pMenu = (ChatSysMenu*)param;
		pMenu->Close();
	}
}

ChatSysMenu::ChatSysMenu()
{
	m_pForm = pMainInterface->GetDForm("pingbi");
	m_pForm->bZorde = FORM_ZORDER_CHAT_TIP;
	char szName[16];
	for(int i = 0;i < 6; i ++)
	{
		sprintf(szName,"g%d",i+1);
		DComponent* pComp = m_pForm->GetComponent(szName);
		m_pCheck[i] = new CCtrlCheck(pComp,0);
	}
	m_pForm->GetComponent("ok")->SetEvent(ClickCSysOkButton,this);
	m_pForm->GetComponent("close")->SetEvent(ClickCSysCloseButton,this);
}

ChatSysMenu::~ChatSysMenu()
{

}

void ChatSysMenu::Open()
{
	for(int i = 0; i < 6; i ++)
	{
		m_pCheck[i]->SetCheck(pChar->m_pChatCenter->m_ShieldSwitch[i]);
	}

	m_pForm->Open();
}

void ChatSysMenu::Close(bool ifSave)
{
	if(ifSave)
	{
		for(int i = 0; i < 6; i ++)
		{
			pChar->m_pChatCenter->m_ShieldSwitch[i] = m_pCheck[i]->GetCheck();
		}
		pChar->m_pChatCenter->SaveSystemSet();
	}
	m_pForm->Close();
}

static void FixString2Vec(char* szMsg,int nLen,std::vector<std::string>& vec)
{
	std::string strMsg(szMsg);
	int nStart = 0;
	int nKick = 0;
	while(strMsg.size()-nStart > nLen)
	{
		nKick = nLen;
		int nCount = 0;
		for(int i = nStart; i < nStart+nLen; i ++)
		{
			if(strMsg[i] < 0)
				nCount ++;
		}
		if(nCount%2 == 1)
			nKick --;
		vec.push_back(strMsg.substr(nStart,nKick));
		nStart = nStart + nKick;
	}
	nKick = strMsg.size()-nStart;
	vec.push_back(strMsg.substr(nStart,nKick));
}

SystemMessageMenu::SystemMessageMenu()
{
	m_isPlaying = false;
	m_ShowMsg = NULL;
	m_pForm = pMainInterface->GetDForm("gonggao");
	m_pCompFront = m_pForm->GetComponent("gonggao");
	m_pCompFront->m_nFontSize = 24;
	m_pForm->pCFrame = new CCtrlFrame(m_pCompFront->nX,m_pCompFront->nY,m_pCompFront->nW,m_pCompFront->nH);
	m_pForm->pCFrame->InitFParts(m_pCompFront,NULL,0);
	//m_pForm->pCFrame->nDisStep = m_pCompFront->nH;
	//m_pForm->pCFrame->bSplitTouch = true;
	m_pForm->pCFrame->bDisableTouch = true;
	m_pForm->pCFrame->SetFPCnt(0);
}

SystemMessageMenu::~SystemMessageMenu()
{
	if(m_ShowMsg)
	 free(m_ShowMsg);
}

void SystemMessageMenu::Update()
{
	static int nCTick = 0;
	static int nCount = 0;
	static bool ifNeedSet = false;
	if(!m_isPlaying)
	{
		if(nCTick == 0)
		{
			if(pChar->m_pChatCenter->m_pSystemMsg->Count > 0)
			{
				SystemMsg* p = (SystemMsg*)pChar->m_pChatCenter->m_pSystemMsg->Items[0];
				m_ShowMsg = strdup(p->szMsg);
				if(p->nTimes > 1)
				{
					p->nTimes --;
				}
				else
				{
					delete (SystemMsg*)pChar->m_pChatCenter->m_pSystemMsg->Delete(0);
				}

			}
			if(m_ShowMsg != NULL)
			{
				m_pForm->Open(2);
				m_isPlaying = true;
				std::vector<std::string> vec;
				int nLineLimit = 50;
				FixString2Vec(m_ShowMsg,nLineLimit,vec);
				nCount = 0;
				m_pForm->pCFrame->SetFPCnt(vec.size()+2,true);
				for(int i = 0; i < vec.size(); i ++)
				{
					DComponent* pComp = m_pForm->pCFrame->GetFPComp(i+1,"gonggao");
					pComp->bAutoChgLine = false;
					pComp->bAlignX = 0;
					pComp->SetCaptionEx("%s",0,vec[i].c_str());
					pComp->SetSprColor(229,151,28);
// 					if(i == vec.size()-1)
// 						nCount += strlen(vec[i].c_str())*m_pCompFront->nW/nLineLimit;
// 					else
					nCount += m_pCompFront->nW;
				}
				DComponent* pComp = m_pForm->pCFrame->GetFPComp(0,"gonggao");
				pComp->SetCaption("",0);
				pComp = m_pForm->pCFrame->GetFPComp(vec.size()+1,"gonggao");
				pComp->SetCaption("",0);
				nCount += m_pCompFront->nW;
				m_pForm->pCFrame->nChgX = -m_pCompFront->nW/3;
				nCTick = 10;
				ifNeedSet = true;
			}
		}
		if(nCTick < 0)
			nCTick ++;
	}
	else
	{
		if(nCTick == 0)
		{
			if(ifNeedSet)
			{
				ifNeedSet = false;
				int nTemp = 0;
				int nMaxLine = m_pForm->pCFrame->GetFPCnt();
				if(nMaxLine > 0)
				{
					nCTick = 10;
					for(int i = 1; i < nMaxLine-1; i ++)
					{
						DComponent* pComp = m_pForm->pCFrame->GetFPComp(i,"gonggao");
						if(nTemp != 0)
							pComp->SetPosX(pComp->nX+nTemp);
						nTemp += pComp->nObjPixWidth - m_pCompFront->nW;
					}
					nCount += nTemp;
				}
			}
			else if(nCount > 0)
			{
				nCount -= 6;
				if(nCount < 0)
				{
					nCount = 0;
					nCTick = 5;
				}
				else
					nCTick = 2;
				m_pForm->pCFrame->nChgX = -6;
				m_pForm->pCFrame->byChgFrom = 1;
			}
			else
			{
				m_isPlaying = false;
				m_pForm->Close();
				free(m_ShowMsg);
				m_ShowMsg = NULL;
				m_pForm->pCFrame->SetFPCnt(0);
				nCTick = -20;
			}		
		}
		if(nCTick > 0)
			nCTick --;
	}
	
}


WarMessageMenu::WarMessageMenu()
{
	m_pForm = pMainInterface->GetDForm("gonggao1");
	for(int i = 0; i < 3; i ++)
	{
		char szName[16];
		sprintf(szName,"gonggao%d",i+1);
		m_pComp[i] = m_pForm->GetComponent(szName);
		m_pComp[i]->SetCaption("",1);
		m_pComp[i]->SetVisible(false);
		m_pComp[i]->SetSprColor(255,192,0);
		sprintf(szName,"di%d",i+1);
		m_pBack[i] = m_pForm->GetComponent(szName);
		m_pBack[i]->SetVisible(false);
		m_dwMoveTick[i] = 0;
	}
	m_pMsg = NULL;
	m_dwTick = 0;
}

WarMessageMenu::~WarMessageMenu()
{
	if(m_pMsg)
		free(m_pMsg);
}

void WarMessageMenu::Update()
{
	static int nCBaseY = m_pComp[0]->nY;
	static int nCBaseX = m_pComp[0]->nX;
	static int nBBaseY = m_pBack[0]->nY;
	static int nBBaseX = m_pBack[0]->nX;
	if(pChar->m_pChatCenter->m_pWarSysMsg->Count > 0 && m_pMsg == NULL)
	{
		SystemMsg* p = (SystemMsg*)pChar->m_pChatCenter->m_pWarSysMsg->Items[0];
		m_pMsg = strdup(p->szMsg);
		if(p->nTimes > 1)
		{
			p->nTimes --;
		}
		else
		{
			delete (SystemMsg*)pChar->m_pChatCenter->m_pWarSysMsg->Delete(0);
		}

	}
	if(m_dwTick == 0 && m_pMsg != NULL)
	{
		for(int i = 0; i < 3; i ++)
		{
			if(m_dwMoveTick[i] == 0)
			{
				m_pComp[i]->SetCaptionEx("%s",1,m_pMsg);
				m_pComp[i]->SetVisible(true);
				m_pComp[i]->SetOpacity(0);
				m_pComp[i]->byRunAction = 22;
				m_pBack[i]->SetVisible(true);
				m_pBack[i]->SetOpacity(0);
				m_pBack[i]->byRunAction = 22;
				m_dwMoveTick[i]  = 2500 + xnGetTickCount();
				m_dwTick = 750 + xnGetTickCount();
				free(m_pMsg);
				m_pMsg = NULL;
				break;
			}
		}
	}
	else if(m_dwTick > 0 && m_dwTick < xnGetTickCount())
	{
		m_dwTick = 0;
	}
	for(int i = 0; i < 3; i ++)
	{
		if(m_dwMoveTick[i] > 0 && m_dwMoveTick[i] < xnGetTickCount())
		{
			m_dwMoveTick[i] = 0;
			m_pComp[i]->SetVisible(false);
			m_pComp[i]->SetPos(nCBaseX,nCBaseY);
			m_pBack[i]->SetVisible(false);
			m_pBack[i]->SetPos(nBBaseX,nBBaseY);
		}
	}
}