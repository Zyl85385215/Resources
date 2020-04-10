
#include "cocos2d.h"
#include "GuideCenter.h"
#include "xnExtstr.h"
#include "UserData.h"
#include "LoginScene.h"

#include "MainMenu.h"

#include "GameActivityCfg.h"

#include "MapControl.h"
#include "EnemyConfig.h"
#include "MapMouse.h"
#include "MapStaticCfg.h"
#include "GuideLayer.h"
#include "MapLayer.h"

GuideCenter*	pGuideCenter	= NULL;

GuideCenter::GuideCenter():bGatherQuest(false),bAtk(false),bStrikeEff(false),nActivityPos(0),nActivityID(0),nAwardItemPos(0),m_bGuideInFB(false),m_bGatherAuto(false)
{
	memset(bCountGuide,1,100);
	if (pMainMenu->m_bBackLogin)
	{		
		memset(bCountGuide,1,100);
	}

	m_bGuideInOtherFB = false;
	m_bStoneGuide = false;
	m_nRunMIndex = 0;
	m_nAwardInlayPos = 0;
	m_bInlayGuide = false;
	m_nAwardEatPos = 0;
	m_bEatGuide = false;
	m_bCatchFishGuide = false;

	m_pCurGuide		= NULL;
	xnExtstr * pStr = xnExtstr::Import("cfg/GuideConfig.txt");
	if( pStr == NULL )
		return;
	
	if(g_szLoginAcc)
	{
		char	szTmp[64];
	
		sprintf(szTmp,"A%d%s_guideid", g_nServerID, g_szLoginAcc);
		nStartID	= LoadIntegerFromXML(szTmp);	
		char	szTmpe[64];
		sprintf(szTmpe,"Aquit%d%s_guideid", g_nServerID, g_szLoginAcc);
		nQuitID	= LoadIntegerFromXML(szTmpe);

		char	szTmpLv[64];
		sprintf(szTmpLv,"Alvup%d%s_guideid", g_nServerID, g_szLoginAcc);
		nLvUpID	= LoadIntegerFromXML(szTmpLv);
	
	}

	if (nStartID>1)
	{
		nCurID = nStartID;
	}	
	m_pList		= xnList::Create();
	int index	= 0;
	while (pStr->SetSection(index++) != NULL)
	{
		int nID	= atoi (pStr->sectionName);

		/*if(nID <= nStartID && nID	< 1000)
			continue;*/

		GuideObj* pGuide = new GuideObj();
		m_pList->Add(pGuide);
		pGuide->nID		= nID;
		pGuide->nLimitLv	= atoi (pStr->ReadString ("lvlimit","0"));
		pGuide->szTxt = strdup(pStr->ReadString ("txt",""));

		pGuide->txtX	= abs(atoi (pStr->ReadString ("txtX","530")));
		pGuide->txtY	= abs(atoi (pStr->ReadString ("txtY","140")));

		pGuide->szForm = strdup(pStr->ReadString ("form",""));
		pGuide->szComp = strdup(pStr->ReadString ("comp",""));

		//����ͼƬID�� ˮƽλ��
		pGuide->mapId = strdup(pStr->ReadString ("mapId",""));		
		pGuide->map2 = strdup(pStr->ReadString ("map2",""));
		pGuide->freeDian = atoi(pStr->ReadString ("free","0"));
		pGuide->nSprX = atoi(pStr->ReadString ("posX","100"));

		pGuide->nSprY	= atoi (pStr->ReadString ("posY","200"));
		//cX,cY
		pGuide->nCX	= atoi (pStr->ReadString ("cX","10"));
		pGuide->nCY	= atoi (pStr->ReadString ("cY","10"));

		pGuide->nEffID	= atoi (pStr->ReadString ("eff","0"));
		//�ڶ���Ч  X Y
		pGuide->nEffID2	= atoi (pStr->ReadString ("eff2","0"));
		pGuide->eff2X	= atoi (pStr->ReadString ("eff2X","500"));
		pGuide->eff2Y	= atoi (pStr->ReadString ("eff2Y","700"));

		pGuide->nX	= atoi (pStr->ReadString ("X","280"));
		pGuide->nY	= atoi (pStr->ReadString ("Y","50"));
		pGuide->nW	= atoi (pStr->ReadString ("W","80"));
		pGuide->nH	= atoi (pStr->ReadString ("H","80"));
		pGuide->nNextID	= atoi (pStr->ReadString ("next","0"));
		//pGuide->ifGift = atoi(pStr->ReadString("gift","0"));
	}
	pStr->Free ();
}

GuideCenter::~GuideCenter()
{
	while(m_pList->Count)
		delete (GuideObj*)m_pList->Delete(0);
	m_pList->Free();
	if (m_pCurGuide)
		delete m_pCurGuide;
}

void GuideCenter::Update()
{
	if (this == NULL)
		return;

	SetBagMenuGuide();

	SetActivityMenuGuide();

	UpdateMapCtrl();

	if (m_bGuideInFB)
	{
		//pMainMenu->m_pCtrlMenu->m_pCompMCopy->SetVisible(false);
		SetGuideInFirstFB(1037, 5301);
		SetGuideInFirstFB(1401, 5401);
	}
	if (m_bGuideInOtherFB)
	{
		//pMainMenu->m_pCtrlMenu->m_pCompMCopy->SetVisible(false);
		SetGuideInFirstFB(1405, 5701);
		SetGuideInFirstFB(1406, 5801);
	}

	if (m_bGatherAuto)
	{
		pMainMenu->m_pAutoMenu->SwitchActive(true);
		m_bGatherAuto = false;
	}
	if (nCurID == 1602 && m_bStoneGuide)
	{
		if(m_nRunMIndex != g_pMapCenter->m_nMapIndex)
		{
			if(pMainMenu->m_pMainFormLA->bOpened)
			{
				pMainMenu->m_pMainFormLA->Close();
				pMainMenu->m_pMainFormTA->Close();
				if(pMainMenu->m_ifQMenuShow)
					pMainMenu->m_pMainFormLB->Open(3);
				else
					pMainMenu->m_pMainFormHide->Open();
				pMainMenu->m_pMainFormTB->Open(5);
				pCtrlLayer->setVisible(true);
			}
			m_bStoneGuide = false;
		}
	}

	if (m_bInlayGuide && nCurID == 3706 && pMainMenu->m_pEquipInlayMenu->m_pForm->bOpened)
	{
		SetInlayBagMenuGuide();
		SetGuide(3707);
		m_bInlayGuide = false;
		return;
	}
	if (m_bEatGuide && nCurID == 6504 && pMainMenu->m_pEquipEatMenu->m_pForm->bOpened)
	{
		SetEatBagMenuGuide();
		SetGuide(6505);
		m_bEatGuide = false;
	}
	/*if (pChar->m_pActivityCenter->m_pWarnActiveList->Count)
	{
	int nCatchFishID = (int)pChar->m_pActivityCenter->m_pWarnActiveList->Items[0];
	if (nCatchFishID == 1012)
	{
	m_bActivityFishGuide = true;
	char	szTmpFish[64];
	sprintf(szTmpFish,"Acatch%d%s_fishid", g_nServerID, g_szLoginAcc);
	SaveBooleanToXML(szTmpFish,true);
	SaveToXMLOK();
	}
	}*/
	//(PosX <= 1500 and PosY >= 1700) or (PosX >=1500 and PosX <= 2400 and PosY >= 2400)
	static bool bHdCatchFish = true;
	if (g_pMapCenter->m_nMapIndex == 30 && m_bCatchFishGuide /*&& m_bActivityFishGuide*/)
	{
		if((g_pMapCenter->m_nX <= 1500 && g_pMapCenter->m_nY >= 1700) || (g_pMapCenter->m_nX >= 1500 && g_pMapCenter->m_nX <= 2400 && g_pMapCenter->m_nY >= 2400))
		{
			ItemObj* pCatchItem = pChar->m_pItemCenter->GetItemByTypeID(22000);
			if (pCatchItem)
			{
				if (!pMainMenu->m_pFastEquipMenu->m_pTipListForm->bOpened)
					pMainMenu->m_pFastEquipMenu->m_pTipListForm->Open();
				if (!pMainMenu->m_pFastEquipMenu->m_pCatchFishTip->bVisable)
					pMainMenu->m_pFastEquipMenu->m_pCatchFishTip->SetVisible(true);
				
				static bool bCatch = true;
				if (nQuitID != 5011 && bCatch)
				{
					SetGuide(7401);				//������������
					SaveQuitGuide(5011);
					bCatch = false;
				}
				bHdCatchFish = false;
			}
			else
			{
				if (!bHdCatchFish)
				{
					pMainMenu->m_pFastEquipMenu->m_pCatchFishTip->SetVisible(false);
					pMainMenu->m_pFastEquipMenu->m_pCatchFishTip->byRunAction = 16;
					pMainMenu->m_pFastEquipMenu->CheckListFormClose();
					m_bCatchFishGuide = false;
				}
			}
		}else
		{
			pMainMenu->m_pFastEquipMenu->m_pCatchFishTip->SetVisible(false);
			pMainMenu->m_pFastEquipMenu->m_pCatchFishTip->byRunAction = 16;
			pMainMenu->m_pFastEquipMenu->CheckListFormClose();
		}
	}
}

void GuideCenter::SetGuide( int nID)
{	
	if(this==NULL)
		return;
	if(m_pCurGuide && m_pCurGuide->nID == nID)
		return;
	m_pCurGuide	= _GetGuide(nID);	

	if(m_pCurGuide)
	{		
		nCurID = nID; //���浱ǰ����ID

		m_pList->Remove(m_pCurGuide);
		
		//if(nID <100)
		//{
			char	szTmp[256];
			sprintf(szTmp,"A%d%s_guideid", g_nServerID, g_szLoginAcc);
			SaveIntegerToXML(szTmp,nID);
			SaveToXMLOK();		
		//}
	}
}

GuideObj* GuideCenter::_GetGuide( int nID )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		GuideObj*	pGuide	= (GuideObj*)m_pList->Items[i];
		if(pChar->m_byLvl > pGuide->nLimitLv)
			continue;
		if(pGuide->nID	== nID)
			return pGuide;
	}
	return NULL;
}

void GuideCenter::NextGuide()
{
	if(m_pCurGuide)
	{
		int nNext	= m_pCurGuide->nNextID;
		delete m_pCurGuide;
		m_pCurGuide = NULL;
		SetGuide(nNext);
	}
}

void GuideCenter::SetGameSceneGuide()
{ 		
	if(this==NULL)
		return;

	if (nStartID < 10&&nStartID!=3)
	{
		SetGuide(nStartID*100 + 1);
	}
	else	
		SetGuide(nStartID + 1);		

}

void GuideCenter::SetUserLvGuide()
{ 
	if (this == NULL)
		return;

	static bool blv = true;
	if (pChar->m_byLvl == 40 && blv && nLvUpID != 40)
	{
		SetGuide(6301);			//40������

		char	szTmps[256];
		sprintf(szTmps,"Alvup%d%s_guideid", g_nServerID, g_szLoginAcc);
		SaveIntegerToXML(szTmps,40);
		SaveToXMLOK();

		blv = false;
	}
}

void GuideCenter::SetEnemyDeadGuide(MapAIObj*	pAObj)
{	
	if(this==NULL)
		return;
	static int  sid = 0;	
	if(pAObj == NULL)
		return;
	if (sid == 0 && pAObj->enemyID == 1004 && nCurID == 201)
	{
		if (pAObj->nHP == 0)
		{			
			SetGuide(3);             //�ͷż�������
			sid	= pAObj->m_nID;
		}
	}	
	if (sid > 1 && sid	!= pAObj->m_nID && pAObj->enemyID == 1004 && nCurID == 3)
	{	
		if (pAObj->nHP==0)
		{
			SetGuide(4);            //װ����һ������
		}	
		sid	= 5;
	}	
	static bool nGuaji = true;
	if (nGuaji && pAObj->enemyID == 1001 && nCurID == 501)
	{		
		if (pAObj->nHP==0)
		{
			SetGuide(6);	          //ֹͣ�һ���������
			nGuaji = false;
		}				
		sid =8;
	}
	static bool bBoss = true;
	if (pAObj->enemyID == 5005 && pAObj->nHP==0&&bBoss && nCurID == 5405)
	{
		pChar->SendCmd(USER,C_OPENCARD_EX);
		SetGuide(1301);            //����boss
		bBoss = false;
	}

	static bool bNewBoss = true;
	if (pAObj->enemyID == 1010 && pAObj->nHP==0 && bNewBoss && nCurID ==5005)
	{
		SetGuide(5001);
		bNewBoss = false;
	}
	
	static bool bWuZuBoss = true;//===zjk
	if (pAObj->enemyID == 1135 && pAObj->nHP == 0 && bWuZuBoss && nCurID == 5105)
	{
		SetGuide(5101);         //����֮Ӱ
		bWuZuBoss = false;
	}

	if (pMainMenu->m_bBackLogin)
	{
		sid = 0;
		nGuaji = true;
		bBoss = true;
		bNewBoss = true;
		bWuZuBoss = true; //===new year
		pMainMenu->m_bBackLogin = false;
	}	
}

void GuideCenter::SetAttackGuide(MapAIObj*	pAObj)
{
	if(this==NULL)
		return;
	//static  bool  bEnemy = true;
	if (pAObj->enemyID==1001/*&&!bEnemy*/&& nCurID==412)
	{
		SetGuide(5);          //�һ�����
		//bEnemy = true;
	}
	if (pAObj->enemyID==1004/*&&bEnemy*/&& nCurID==112)
	{				
		SetGuide(2);           //��ͨ��������
		//bEnemy = false;
	}
	if (nCurID == 2222 && bAtk)
	{		
		if (g_pMapCommand->pGuideCmd)
			g_pMapCommand->pGuideCmd = NULL;
	
		if (pMainMenu->m_pAutoMenu->m_bActive)
		{	
			pMainMenu->m_pAutoMenu->SwitchActive(false);
			SetGuide(2201);                                  //ŭ������������ �����ͷ�		
		}else 
		{
			SetGuide(2201);                      //ŭ������������ �����ͷ�				
		}	

		bAtk	= false;
	}
}

void GuideCenter::SetGatherGuide(MapAIObj*	pAObj)
{	
	if(this==NULL)
		return;
	if (pAObj->enemyID == 1011&&bGatherQuest&&bCountGuide[0])
	{
		m_bGatherAuto = true;
		SetGuide(2001);                      //�ɼ���ǲ�
		bCountGuide[0] = false;
	}	
}

void GuideCenter::SetBagMenuGuide()
{	
	if(this==NULL)
		return;
	if(!pMainMenu->m_pBagMenu->m_pForm->bOpened)
		return;
	///////////////////////
	
	if (nCurID == 407)
	{
		GetItemInBagPos((pChar->m_byJob-1)*1000 + 10000);
		SetGuide(408);		//��һ������λ�ã���������
		return;
	}
	if(nCurID==802)//30001�죬30004��
	{
		GetItemInBagPos(30001);
		SetGuide(9);  
		return;//ѡ��ҩƷλ������			
	}	

	if (nCurID==1502)
	{
		GetItemInBagPos(30009);
		SetGuide(1601);                 //¯ʯ ����λ������		
		return;
	}  
	if (nCurID==1920)
	{
		//������ID��ְҵ�޹��� ֻ��һһ��Ӧ
		if(pChar->m_byJob == 1)
			GetItemInBagPos(35000);
		if(pChar->m_byJob == 2)
			GetItemInBagPos(35002);
		if(pChar->m_byJob == 3)
			GetItemInBagPos(35001);
		if(pChar->m_byJob == 4)
			GetItemInBagPos(35003); 

		SetGuide(2101);                 //ŭ�������� ����λ������
		return;
	}	
	//////////////////////
	if (nCurID == 3805)
	{
		GetItemInBagPos(30015);
		SetGuide(3808);                 //˫�����鿨 ����λ������		
		return;
	}

	if (nCurID == 7205)
	{
		GetItemInBagPos(20010);
		SetGuide(7208);					//��ʯ�ϳ�
		return;
	}

}


void GuideCenter::SetQuestInfoGuide(USER_S_QUEST_INFO_Data* pData)
{	
	if(this == NULL)
		return;
	if (pData->wQid == 1013 && nQuitID != 1013&&bCountGuide[1])
	{
		bGatherQuest = true;          //���ܲɼ���ǲ�����
		bCountGuide[1] = false;

		SaveQuitGuide(1013);
	}		
	if (pData->wQid == 1005&& nQuitID != 1005&&bCountGuide[2]) 
	{
		SetGuide(605);                //ҩƷ����
		bCountGuide[2] = false;
		SaveQuitGuide(1005);
	}
	if (pData->wQid == 1008&& nQuitID != 1008&&bCountGuide[3])
	{		
		SetGuide(7);                //ѧϰ��������
		bCountGuide[3] = false;
		SaveQuitGuide(1008);
	}	
	if (pData->wQid == 1037&& nQuitID != 1037&&bCountGuide[4])             //������������
	{
		m_bGuideInFB = true;
		SetGuide(1201);
		bCountGuide[4] = false;
		SaveQuitGuide(1037);
	}
	if (pData->wQid == 1026&& nQuitID != 1026&&bCountGuide[5])             //�����ȡ����
	{
		pMainInterface->GetDForm("cwchoujiang")->Open();
		SetGuide(1401);
		bCountGuide[5] = false;
		SaveQuitGuide(1026);
	}
	if (pData->wQid == 1032&& nQuitID != 1032&&bCountGuide[6])            //¯ʯ����
	{
		m_bStoneGuide = true;
		m_nRunMIndex = g_pMapCenter->m_nMapIndex;
		SetGuide(1501);
		bCountGuide[6] = false;
		SaveQuitGuide(1032);
	}
	if (pData->wQid == 1046&&bCountGuide[7]&&nQuitID != 1046)            //��ȥ��� �ͷ�ŭ��
	{
		bAtk = true;
		bCountGuide[7] = false;		
		SaveQuitGuide(1046);
	}
	if (pData->wQid == 1024&& nQuitID != 1024&&bCountGuide[8])
	{
		SetGuide(3001);              //�ǳ�����
		bCountGuide[8] = false;
		SaveQuitGuide(1024);
	}	
	if (pData->wQid == 1038&& nQuitID != 1038&&bCountGuide[9])
	{
		SetGuide(3401);              //�ǳ�����
		bCountGuide[9] = false;
		SaveQuitGuide(1038);
	}
	if (pData->wQid == 1015&& nQuitID != 1015&&bCountGuide[10])
	{
		SetGuide(3601);              //������������
		bCountGuide[10] = false;
		SaveQuitGuide(1015);
	}
	if (pData->wQid == 1405&& nQuitID != 1405&&bCountGuide[23])
	{
		m_bGuideInOtherFB = true;
		SetGuide(3901);              //20����������
		bCountGuide[23] = false;
		SaveQuitGuide(1405);
	}
	if (pData->wQid == 1119&& nQuitID != 1119&&bCountGuide[24])
	{
		SetGuide(4001);              //30����������
		bCountGuide[24] = false;
		SaveQuitGuide(1119);
	}
	if (pData->wQid == 1031 /*&& nQuitID != 1031*/ && bCountGuide[30])
	{
		nCurID = 5005;              //ս�۵��ս�-->����п�
		//SaveQuitGuide(1031);
		bCountGuide[30] = false;
	}
	//===zjk new year
	if (pData->wQid == 1404 /*&& nQuitID != 1404*/ && bCountGuide[31])
	{
		nCurID = 5105;              //����֮Ӱ-->����֮Ӱ
		bCountGuide[31] = false;
	}
	if (pData->wQid == 5011 /*&& nQuitID!= 5011*/ && bCountGuide[53])
	{
		
		m_bCatchFishGuide = true;		//������������
		//SaveQuitGuide(5011);
		bCountGuide[53] = false;
	}
}

void GuideCenter::SetQuestRemoveGuide(USER_S_REMOVE_QUEST_Data* pData)
{
	if(this==NULL)
		return;
	if (pData->wQid == 1003&&bCountGuide[22])            //��һ����������
	{
		SetGuide(405); 
		bCountGuide[22] = false;
	}
	if (pData->wQid == 1009&&bCountGuide[11])            //ҩƷ����
	{
		SetGuide(8); 
		bCountGuide[11] = false;
	}	
	if (pData->wQid == 1034&&bCountGuide[12])             //ǿ��װ������
	{
		SetGuide(1001);
		bCountGuide[12] = false;
	}
	if (pData->wQid == 1029&&bCountGuide[13])             //���＼������
	{
		SetGuide(1701);
		bCountGuide[13] = false;
	}
	if (pData->wQid == 1049&&bCountGuide[14])             //��������18ϵ��
	{
		SetGuide(1801);
		bCountGuide[14] = false;
	}
	if (pData->wQid == 1043&&bCountGuide[15])             //ָ��ѧϰŭ������19ϵ�� 1043
	{
		SetGuide(1901);
		bCountGuide[15] = false;
	}
	if (pData->wQid == 1059&&bCountGuide[16])
	{
		SetGuide(3101);                         //ׯ԰��ֲ����
		bCountGuide[16] = false;
	}
	if (pData->wQid == 1071 &&bCountGuide[17])
	{
		SetGuide(3201);                   //ι����������
		bCountGuide[17] = false;
	}
	if (pData->wQid == 1142&&bCountGuide[18])
	{
		SetGuide(3301);                 //������������
		bCountGuide[18] = false;
	}
	if (pData->wQid == 1064 &&bCountGuide[19])
	{
		SetGuide(3501);                 //����ǿ������
		bCountGuide[19] = false;
	}
	if (pData->wQid == 1082 &&bCountGuide[20])
	{
		m_bInlayGuide = true;
		SetGuide(3701);                 //װ�������Ƕ����
		bCountGuide[20] = false;
	}
	if (pData->wQid == 1060 &&bCountGuide[21])
	{
		//pChar->m_pItemCenter->SortBag();
		SetGuide(3801);                 //˫�����鿨����
		bCountGuide[21] = false;
	}
	if (pData->wQid == 1106 && bCountGuide[25])
	{
		SetGuide(4101);                //�������30�� ����������
		bCountGuide[25] = false;
	}
	if (pData->wQid == 1018 && bCountGuide[26])
	{
		SetGuide(4201);
		bCountGuide[26] = false;       //9��������ͷ�������
	}
	if (pData->wQid ==1052 && bCountGuide[27])
	{
		SetGuide(4301);
		bCountGuide[27] = false;       //20���򿪻�������� 
	}	
	if (pData->wQid ==1078 && bCountGuide[28])
	{
		SetGuide(4401);
		bCountGuide[28] = false;       //25���򿪻�������� 
	}
	if (pData->wQid ==1108 && bCountGuide[29])
	{
		SetGuide(4501);
		bCountGuide[29] = false;       //30���򿪻�������� 
	}

	if (pData->wQid == 6053 && bCountGuide[32])
	{
		//pMainMenu->m_pShopMenu->Open((int)4);
		//pChar->m_pItemCenter->m_bBuyBook = true;
		SetGuide(4701);
		bCountGuide[32] = false;       //3֧�����������
	}

	if (pData->wQid == 1038 && bCountGuide[33])
	{
		SetGuide(5501);
		bCountGuide[33] = false;		//15-20��֮�����ӵ������ֵ����ť������
	}
	if (pData->wQid == 1041 && bCountGuide[34])
	{
		SetGuide(5601);
		bCountGuide[34] = false;		//���� �̳���������
	}

	if (pData->wQid == 1138 && bCountGuide[36])
	{
		SetGuide(5901);
		bCountGuide[36] = false;		//����ξ� ����
	}

	if (pData->wQid == 1149 && bCountGuide[37])
	{
		SetGuide(4601);
		bCountGuide[37] = false;		//38�� �˳�
	}
	if (pData->wQid == 1151 && bCountGuide[38])
	{
		SetGuide(6101);
		bCountGuide[38] = false;		//�����������
	}
	if (pData->wQid == 1123 && bCountGuide[39])
	{
		SetGuide(6201);
		bCountGuide[39] = false;		//���
	}
	if (pData->wQid == 1125 && bCountGuide[40])
	{
		SetGuide(6401);					//�������
		bCountGuide[40] = false;
	}
	if (pData->wQid == 1164 && bCountGuide[42])
	{
		SetGuide(6601);					//�������
		bCountGuide[42] = false;
	}
	if (pData->wQid == 1100 && bCountGuide[41])
	{
		m_bEatGuide = true;
		SetGuide(6501);					//װ������
		bCountGuide[41] = false;
	}
	if (pData->wQid == 1181 && bCountGuide[43])
	{
		SetGuide(6701);					//�������� 45��
		bCountGuide[43] = false;
	}
	if (pData->wQid == 1192 && bCountGuide[44])
	{
		SetGuide(6801);					//�������� 45��
		bCountGuide[44] = false;
	}
	if (pData->wQid == 1075 && bCountGuide[46])
	{
		SetGuide(6901);					//��������
		bCountGuide[46] = false;
	}
	if (pData->wQid == 1402 && bCountGuide[47])
	{
		SetGuide(7001);					//������
		bCountGuide[47] = false;
	}
	if (pData->wQid == 1115 && bCountGuide[48])
	{
		SetGuide(7101);					//��Ծ��
		bCountGuide[48] = false;
	}
	if (pData->wQid == 1055 && bCountGuide[51])
	{
		SetGuide(7201);					//��ʯ�ϳ�����
		bCountGuide[51] = false;
	}
	if (pData->wQid == 1040 && bCountGuide[52])
	{
		SetGuide(7301);					//����ҩƷ����
		bCountGuide[52] = false;
	}
	if (pData->wQid == 5011 && bCountGuide[54])
	{
		m_bCatchFishGuide = false;
		bCountGuide[54] = false;
	}
}

void GuideCenter::SetStrikeGuide()
{	
	if(this==NULL)
		return;
	if (bCountGuide[45] && pChar->m_byLvl <= 18)
	{		
		pMainMenu->PlayEffect(212);
		//pMainMenu->m_pCtrlMenu->m_dwTickStrike = xnGetTickCount() - 5000;
		bStrikeEff = true;
		bCountGuide[45] = false;
	}	
}

void GuideCenter::SaveQuitGuide( int nID )
{
	if(g_szLoginAcc == NULL)
		return;
	char	szTmps[64];
	sprintf(szTmps,"Aquit%d%s_guideid", g_nServerID, g_szLoginAcc);
	SaveIntegerToXML(szTmps,nID);
	SaveToXMLOK();
}

void GuideCenter::SetActivityMenuGuide()
{
	if(this == NULL)
		return;
	if (!pMainMenu->m_pActivityMenu->m_pForm->bOpened)
		return;

	if (nCurID == 4303)
		SetActivityGuide(4305, 1002);      //���˴�ת�� 20��
	if (nCurID == 4403)
		SetActivityGuide(4405, 1003);      //������Դ  25��
	if (nCurID == 4503)
		SetActivityGuide(4505, 1007);      //��������  30��
	if (nCurID == 4601)
		SetActivityGuide(4605, 1008);      //�����̶�  35��
}

void GuideCenter::SetActivityGuide(int nGuideID, int nActivityID)
{
	//DComponent* pBaseCom = pMainMenu->m_pActivityMenu->m_pForm->GetComponent("zi");

	for (int i=0; i<pChar->m_pActivityCenter->m_pList->Count; i++)
	{
		ActivityObj* pObj = (ActivityObj*)pChar->m_pActivityCenter->m_pList->Items[i];	

		if (pObj && pObj->nID == nActivityID)     
		{
			nActivityPos = i;			
		
			if (nActivityPos >= 3)
			{
				pMainMenu->m_pActivityMenu->m_pForm->pCFrame->nSetY = pMainMenu->m_pActivityMenu->m_pForm->pCFrame->nDisStep*(i-1);

			}else
				pMainMenu->m_pActivityMenu->m_pForm->pCFrame->nSetY = pMainMenu->m_pActivityMenu->m_pForm->pCFrame->nDisStep*(-1);
			SetGuide(nGuideID);			                   
			return;
		}
	}
}

void GuideCenter::UpdateMapCtrl()
{
	if (this==NULL)
		return;

	if (!g_pMapCenter->m_pMObjSel)
		return;

	MapAIObj*	pAObj	= (MapAIObj*)g_pMapCenter->m_pMObjSel->pMObj;
	if (!pAObj)
		return;

	if(g_pMapCenter->m_pMObjSel->byIsEnemy)
	{
		if(pAObj->m_byType == MAPOBJ_ENEMY && pAObj->pBaseCfg->bType == 3)
			SetGatherGuide(pAObj);
		else
			SetAttackGuide(pAObj);
	}	

	SetEnemyDeadGuide(pAObj);
}


void GuideCenter::GetItemInBagPos( int nItemID )
{
	if (this == NULL)
		return;

	xnList* pList = xnList::Create();
	pChar->m_pItemCenter->GetItemInBagByPos(pList);

	int nPos = 0;
	for (int i = 0;i<pList->Count;i++)
	{
		ItemObj* pItem = (ItemObj*)pList->Items[i];

		if (pItem == NULL)
			return;
		if (pItem->pItemBase->nTypeID == nItemID)
		{
			nPos = pItem->GetVal(ITEM_AD_POS);
			break;
		}
	}
	if (nPos/20 > 0)
	{
		DComponent* pBaseCom = pMainMenu->m_pBagMenu->m_pForm->GetComponent("di3");
		pMainMenu->m_pBagMenu->m_pForm->pCFrame->nSetX = pBaseCom->nW * (nPos/20) * (-1);
	}
	nAwardItemPos = nPos%20;
	pList->Free();
}

void GuideCenter::SetGuideInFirstFB( int nQuestID, int nGuideID)
{
	if(this == NULL)
		return;

	for (int i = 0; i < pChar->m_pCQuestCenter->m_pListQuest->Count; i++)
	{
		ClientQuest* pCQuest	= (ClientQuest*)pChar->m_pCQuestCenter->m_pListQuest->Items[i];

		if (pCQuest && pCQuest->wQID == nQuestID && pCQuest->bOk)
		{
			static bool bQuestFB = true;
			if (bQuestFB && pCQuest->wQID == 1037)
			{
				SetGuide(nGuideID);
				bQuestFB = false;
			}
			static bool bQuestFB2 = true;
			if (bQuestFB2 && pCQuest->wQID == 1401)
			{
				SetGuide(nGuideID);
				bQuestFB2 = false;
				m_bGuideInFB = false;
			}
			static bool bQuestFB3 = true;
			if (bQuestFB3 && pCQuest->wQID == 1405)
			{
				SetGuide(nGuideID);
				bQuestFB3 = false;
			}
			static bool bQuestFB4 = true;
			if (bQuestFB4 && pCQuest->wQID == 1406)
			{
				SetGuide(nGuideID);
				bQuestFB4 = false;
				m_bGuideInOtherFB = false;
			}
		}

	/*	for (int j = 0; j < pCQuest->pQBase->pLQCondtion->Count; j++)
		{
			_QConditon* pQC	= (_QConditon*)pCQuest->pQBase->pLQCondtion->Items[j];
			if (pCQuest->wQID == nQuestID && pCQuest->pszVal[pQC->bValPos] == pQC->wDNum)
				SetGuide(nGuideID);
		}*/
	}
}

void GuideCenter::SetInlayBagMenuGuide()
{
	if (this ==NULL)
		return;
	if (!pMainMenu->m_pEquipInlayMenu->m_pForm->bOpened)
		return;

	xnList* pList = xnList::Create();
	pChar->m_pItemCenter->GetItemInBagByType(pList,ITEMTYPE_STONE);
	int nPos = 0;
	for (int i= 0;i<pList->Count;i++)
	{
		ItemObj* pItem = (ItemObj*)pList->Items[i];
		if (pItem && pItem->pItemBase->nTypeID == 20070)
		{
			nPos = i;
		}
	}
	if ((nPos + 1)/20 > 0)
	{
		DComponent* pBaseCom = pMainMenu->m_pEquipInlayMenu->m_pForm->GetComponent("di2");
		pMainMenu->m_pEquipInlayMenu->m_pForm->pCFrame->nSetX = pBaseCom->nW * (nPos/20) * (-1);
	}
	m_nAwardInlayPos = nPos%20;

	pList->Free();
}

static bool GetItemEat(ItemObj* pObj,void* param){
	if (pObj->GetVal(ITEM_AD_SLOT) !=ITEM_SLOT_BAG )
	{
		return false;
	}
	if (pObj == EquipStarMenu::sLvUpEquipObj)
	{
		return false;
	}
	if (pObj->pItemBase->nStar >= 1&&pObj->pItemBase->byType == ITEMTYPE_EQUIP/*&&pObj->pItemBase->bySlot != EQUIP_SLOT_TO_INDEX(ITEM_SLOT_EQUIP_SKIN)*/)
	{
		return true;
	}
	return false;
}

void GuideCenter::SetEatBagMenuGuide()
{
	if (this == NULL)
		return;
	if (!pMainMenu->m_pEquipEatMenu->m_pForm->bOpened)
		return;

	xnList* pList = xnList::Create();
	pChar->m_pItemCenter->GetItemInBagByFilter(pList,GetItemEat);
	int nPos = 0;
	for (int i= 0;i<pList->Count;i++)
	{
		ItemObj* pItem = (ItemObj*)pList->Items[i];

		if (pItem && pItem->pItemBase->nTypeID == (10100 + (pChar->m_byJob-1)*1000))
		{
			nPos = i;
		}
	}
	if ((nPos + 1)/20 > 0)
	{
		DComponent* pBaseCom = pMainMenu->m_pEquipEatMenu->m_pForm->GetComponent("di2");
		pMainMenu->m_pEquipEatMenu->m_pForm->pCFrame->nSetX = pBaseCom->nW * (nPos/20) * (-1);
	}
	m_nAwardEatPos = nPos%20;
	pList->Free();
}


GuideObj::GuideObj()
{
	nID     = 0;
	nX		= 0;
	nY		= 0;
	nW		= 0;
	nY		= 0;
	nCX		= 0;
	nCY		= 0;

	freeDian	= 0;
	nSprX	= 0;
	nSprY	= 0;
	nNextID	= 0;
	nLimitLv	= 0;
	nEffID	= 0;
	nEffID2 = 0;
	eff2X	= 0;
	eff2Y	= 0;
	txtX	= 0;
	txtY	= 0;

	szTxt	= NULL;
	szForm	= NULL;
	szComp	= NULL;
	pForm	= NULL;

	mapId	= NULL;
	map2	= NULL;
}

GuideObj::~GuideObj()
{
	if(szTxt)
		free(szTxt);
	if(szForm)		
		free(szForm);
	if(szComp)
		free(szComp);
	if (mapId)
		free(mapId);
	if (map2)
		free(map2);
}

