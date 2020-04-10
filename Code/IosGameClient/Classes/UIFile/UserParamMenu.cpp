#include "MainMenu.h"
#include "UserData.h"
#include "UserParamMenu.h"
#include "ios_user_packet.h"
#include "UserConfig.h"
#include "TitleConfig.h"
#include "FamilyCenter.h"
extern char* szEquipComp[];
char* szAttrComp[] = {"hp","mp","wg","mg","wf","mf","mz","sb","bj","rx"};
extern void	SetItemToDCom(DComponent* pDom,ItemObj* pObj);
extern void	SetCharToDCom(DComponent* pDom,BYTE byJob,BYTE bySex,char* szAction = "stand",int nDirection = 5);
extern void	SetWeaponToDCom(DComponent* pDom,int nID,char* szAction = "stand",int nDirection = 5);
extern void	SetCharToDCom(DComponent* pDom,int	nSkin,char* szAction = "stand",int nDirection = 5);
static void UseItemEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	ItemObj* pItem = (ItemObj*)pEvent->pComponent->GetTag();
	if (pItem)
	{
		if (pItem->pItemBase->byType == ITEMTYPE_EQUIP/*&&pItem->pItemBase->bySlot != EQUIP_SLOT_TO_INDEX(ITEM_SLOT_EQUIP_SKIN)*/)
		{
			pMainMenu->m_pEquipInfoMenu->Open(pItem,FORM_BAG);
		}else{
			pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_BAG);
		}
	}
}

void	OpenAchievementButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pAchievementTabCtrl->Open(2,0);
}
UserParamMenu::UserParamMenu():m_bUpdate(true)
{
	m_pForm = pMainInterface->GetDForm ("information");
	char sztxt[32];
// 	for (int i=0;i<10;i++)
// 	{
// 		sprintf(sztxt,"%d",i);
// 		m_pForm->GetComponent(sztxt)->SetEvent(NumInput,(void*)i);
// 	}
// 	m_pForm->GetComponent("no")->SetEvent(DeleteNumInput);
// 	m_pForm->GetComponent("ok")->SetEvent(CloseNumInput);

	for (int i=0;i<10;i++)
	{
		m_pEquip[i] = m_pForm->GetComponent(szEquipComp[i]);
		m_pEquip[i]->SetEvent(UseItemEvent);
	}
	m_pForm->GetComponent("cj")->SetEvent(OpenAchievementButton,this);
}

UserParamMenu::~UserParamMenu()
{
}



void UserParamMenu::Open(int nT)
{

	m_bUpdate = true;
	pChar->SendCmd(NHOME,C_RQHOMES);
	pChar->SendCmd(MAPCMD,C_GET_PARAM);
	m_pForm->Open(nT);
}

void UserParamMenu::Close()
{
	m_pForm->Close();
}
char* szJobTxt[] = {"无","战士","法师","枪手","牧师"};
void UserParamMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}
	m_pForm->GetComponent("lv")->SetCaptionEx("%d",0,pChar->m_byLvl);
	m_pForm->GetComponent("lv")->m_nFontSize = 28;
	m_pForm->GetComponent("lv")->SetSprColor(255,255,255);
	
	if (pChar->m_dwExp > 10000)
	{
		char szText[64];
		sprintf(szText, "%u / %u",pChar->m_dwExp,UserConfig::GetInstance().GetExp(pChar->m_byLvl));
		m_pForm->GetComponent("exp")->SetProgress(pChar->m_dwExp/10000,UserConfig::GetInstance().GetExp(pChar->m_byLvl)/10000,szText);
	}
	else
		m_pForm->GetComponent("exp")->SetProgress(pChar->m_dwExp,UserConfig::GetInstance().GetExp(pChar->m_byLvl));

	if (m_bUpdate)
	{
		m_bUpdate = false;

		for (int i=0;i<10;i++)
		{
			ItemObj* pItem = pChar->m_pItemCenter->GetEquipBySlot(i);
			SetItemToDCom(m_pEquip[i],pItem);

		}
		for (int i=0;i<EQUIP_ATTR_MAX;i++)
		{
			m_pForm->GetComponent(szAttrComp[i])->SetCaptionEx("%d",0,pChar->m_xUserParam2.nParam[i]);
		}
		m_pForm->GetComponent("rx")->SetCaptionEx("%d",0,(pChar->m_byLvl*5+95)/2);
		m_pForm->GetComponent("sd")->SetCaptionEx("%d",0,pChar->m_xUserParam2.nSpeed);
		m_pForm->GetComponent("name")->SetCaptionEx("%s",0,pChar->m_szNick);
		m_pForm->GetComponent("id")->SetCaptionEx("%d",0,pChar->m_dwUin);
		m_pForm->GetComponent("tl")->SetCaptionEx("%d",0,pChar->m_dwPow);
		m_pForm->GetComponent("gx")->SetCaptionEx("%d",0,pChar->m_dwHonour>0?pChar->m_dwHonour:0);

		m_pForm->GetComponent("js")->SetCaptionEx("%d.%d%%",0,pChar->m_xUserParam2.nPvpJSDef/10,pChar->m_xUserParam2.nPvpJSDef%10);
		
		if (pChar->m_dwHomeID)
		{
			NHomeSInfo*  HomeInfo = pLeague->GetHomeInfoByID(pChar->m_dwHomeID);
			if (HomeInfo)
			{
				m_pForm->GetComponent("org")->SetCaptionEx("%s",0, HomeInfo->szName);
			}else
				m_pForm->GetComponent("org")->SetCaptionEx("%s",0, (char*)"无");
		}else
		    m_pForm->GetComponent("org")->SetCaptionEx("%s",0, (char*)"无");
		m_pForm->GetComponent("gj")->SetCaptionEx("%s",0,pChar->m_byCountry == 1?(char*)"联盟":(char*)"部落");
		m_pForm->GetComponent("zy")->SetCaptionEx("%s",0,pChar->m_bySex?(char*)"男":(char*)"女");
		m_pForm->GetComponent("job")->SetCaptionEx("%s",0,szJobTxt[pChar->m_byJob]);
		m_pForm->GetComponent("zd")->SetCaptionEx("%d",1,pChar->GetScore());

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
	}
}


UserTitleMenu::UserTitleMenu()
{
	m_pForm = pMainInterface->GetDForm("chenghao");
	for (int i=0;i<10;i++)
	{
		m_pEquip[i] = m_pForm->GetComponent(szEquipComp[i]);
		m_pEquip[i]->SetEvent(UseItemEvent);
	}	
	m_pForm->GetComponent("name1")->SetSprColor(15,193,219);
	char szName[8];
	for(int i = 0; i < 6; i ++)
	{
		sprintf(szName,"a%d",i+1);
		m_pForm->GetComponent(szName)->SetSprColor(15,193,219);
	}
}

UserTitleMenu::~UserTitleMenu()
{

}

void UserTitleMenu::Update()
{
	if(m_pForm && m_pForm->bOpened)
	{
		for (int i=0;i<10;i++)
		{
			ItemObj* pItem = pChar->m_pItemCenter->GetEquipBySlot(i);
			SetItemToDCom(m_pEquip[i],pItem);
		}
		DComponent* pComp = NULL;
		char szWords[128];

		if(pChar->m_pTitleObj)
		{
			char* pTemp = pChar->m_pTitleObj->pName1;
			if(pChar->m_byCountry == 2)
				pTemp = pChar->m_pTitleObj->pName2;
			pComp = m_pForm->GetComponent("name1");
			pComp->SetCaptionEx("%s",1,pTemp);
			pComp = m_pForm->GetComponent("a1");
			pComp->SetCaptionEx("生命值+%.2f%%",0,pChar->m_pTitleObj->nHpAdd/100.0);
			pComp = m_pForm->GetComponent("a2");
			pComp->SetCaptionEx("魔法值+%.2f%%",0,pChar->m_pTitleObj->nMpAdd/100.0);
			pComp = m_pForm->GetComponent("a3");
			pComp->SetCaptionEx("物攻+%.2f%%",0,pChar->m_pTitleObj->nAdAdd/100.0);
			pComp = m_pForm->GetComponent("a4");
			pComp->SetCaptionEx("魔攻+%.2f%%",0,pChar->m_pTitleObj->nApAdd/100.0);
			pComp = m_pForm->GetComponent("a5");
			pComp->SetCaptionEx("物防+%.2f%%",0,pChar->m_pTitleObj->nDdAdd/100.0);
			pComp = m_pForm->GetComponent("a6");
			pComp->SetCaptionEx("魔防+%.2f%%",0,pChar->m_pTitleObj->nDpAdd/100.0);
		}
		else
		{	
			m_pForm->GetComponent("name1")->SetCaptionEx("%s",1,"平民");
			pComp = m_pForm->GetComponent("a1");
			pComp->SetCaptionEx("生命值+%.2f%%",0,0.0);
			pComp = m_pForm->GetComponent("a2");
			pComp->SetCaptionEx("魔法值+%.2f%%",0,0.0);
			pComp = m_pForm->GetComponent("a3");
			pComp->SetCaptionEx("物攻+%.2f%%",0,0.0);
			pComp = m_pForm->GetComponent("a4");
			pComp->SetCaptionEx("魔攻+%.2f%%",0,0.0);
			pComp = m_pForm->GetComponent("a5");
			pComp->SetCaptionEx("物防+%.2f%%",0,0.0);
			pComp = m_pForm->GetComponent("a6");
			pComp->SetCaptionEx("魔防+%.2f%%",0,0.0);
		}

		if(pChar->m_pTitleNextObj)
		{
			char* pTemp = pChar->m_pTitleNextObj->pName1;
			if(pChar->m_byCountry == 2)
				pTemp = pChar->m_pTitleNextObj->pName2;
			m_pForm->GetComponent("name2")->SetCaptionEx("%s",1,pTemp);
			pComp = m_pForm->GetComponent("b1");
			pComp->SetCaptionEx("生命值+%.2f%%",0,pChar->m_pTitleNextObj->nHpAdd/100.0);
			pComp = m_pForm->GetComponent("b2");
			pComp->SetCaptionEx("魔法值+%.2f%%",0,pChar->m_pTitleNextObj->nMpAdd/100.0);
			pComp = m_pForm->GetComponent("b3");
			pComp->SetCaptionEx("物攻+%.2f%%",0,pChar->m_pTitleNextObj->nAdAdd/100.0);
			pComp = m_pForm->GetComponent("b4");
			pComp->SetCaptionEx("魔攻+%.2f%%",0,pChar->m_pTitleNextObj->nApAdd/100.0);
			pComp = m_pForm->GetComponent("b5");
			pComp->SetCaptionEx("物防+%.2f%%",0,pChar->m_pTitleNextObj->nDdAdd/100.0);
			pComp = m_pForm->GetComponent("b6");
			pComp->SetCaptionEx("魔防+%.2f%%",0,pChar->m_pTitleNextObj->nDpAdd/100.0);
		}
		else
		{
			if(pChar->m_pTitleObj)
			{
				char* pTemp = pChar->m_pTitleObj->pName1;
				if(pChar->m_byCountry == 2)
					pTemp = pChar->m_pTitleObj->pName2;
				pComp = m_pForm->GetComponent("name2");
				pComp->SetCaptionEx("%s",1,pTemp);
				pComp = m_pForm->GetComponent("b1");
				pComp->SetCaptionEx("生命值+%.2f%%",0,pChar->m_pTitleObj->nHpAdd/100.0);
				pComp = m_pForm->GetComponent("b2");
				pComp->SetCaptionEx("魔法值+%.2f%%",0,pChar->m_pTitleObj->nMpAdd/100.0);
				pComp = m_pForm->GetComponent("b3");
				pComp->SetCaptionEx("物攻+%.2f%%",0,pChar->m_pTitleObj->nAdAdd/100.0);
				pComp = m_pForm->GetComponent("b4");
				pComp->SetCaptionEx("魔攻+%.2f%%",0,pChar->m_pTitleObj->nApAdd/100.0);
				pComp = m_pForm->GetComponent("b5");
				pComp->SetCaptionEx("物防+%.2f%%",0,pChar->m_pTitleObj->nDdAdd/100.0);
				pComp = m_pForm->GetComponent("b6");
				pComp->SetCaptionEx("魔防+%.2f%%",0,pChar->m_pTitleObj->nDpAdd/100.0);
			}else
			{
				m_pForm->GetComponent("name2")->SetCaptionEx("%s",1,"已获至高荣誉");
				pComp = m_pForm->GetComponent("b1");
				pComp->SetCaption("",0);
				pComp = m_pForm->GetComponent("b2");
				pComp->SetCaption("",0);
				pComp = m_pForm->GetComponent("b3");
				pComp->SetCaption("",0);
				pComp = m_pForm->GetComponent("b4");
				pComp->SetCaption("",0);
				pComp = m_pForm->GetComponent("b5");
			    pComp->SetCaption("",0);
				pComp = m_pForm->GetComponent("b6");
				pComp->SetCaption("",0);
			}
		}
		int nCur = 0;
		int nMax = 0;
		TitleConfig::GetInstance().GetPrograssVal(pChar->m_dwHonour,nCur,nMax);
		m_pForm->GetComponent("exp")->SetProgress(nCur,nMax);

		bool bSwitch2 = true;
		if (pChar->m_byJob==2)
		{
			bSwitch2 = false;
		}
		if (pChar->m_xUserParam.nSkin > 1500)
		{
			m_pForm->GetComponent((char*)(bSwitch2?"model":"model2"))->SetVisible(false);
		}else{
			m_pForm->GetComponent((char*)(bSwitch2?"model":"model2"))->SetVisible(true);
			SetWeaponToDCom(m_pForm->GetComponent((char*)(bSwitch2?"model":"model2")),pChar->m_pItemCenter->GetWeaponSkin());
		}
		SetCharToDCom(m_pForm->GetComponent(bSwitch2?(char*)"model2":(char*)"model"),pChar->m_xUserParam.nSkin);
		m_pForm->GetComponent("zd")->SetCaptionEx("%d",1,pChar->GetScore());
	}
}