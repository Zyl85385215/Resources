#include "stdafx.h"

#include "DebugMsg.h"
#include "UserData.h"
#include "FsGameCenter.h"
#include "MailCenter.h"
#include "CallMapEnemy.h"
#include "UserQuest.h"
#include "SlaveData.h"
#include "GMActiveCfg.h"
#include "FamilyCenter.h"
#include "FortCenter.h"
#include "WarRank.h"

CreateSingleton(DebugCmd);

int SplitString(const char * szSrc,int nC, OUT char* szOut[],int nOutCnt)
{
	char* szPosS,* szPosE;
	szPosE = szPosS = (char*)szSrc;
	int nCnt = 0;
	if(strlen(szSrc) == 0) return nCnt;

	int nEnd = (int)szPosS+strlen(szSrc)-1;
	while ((int)szPosE < nEnd)
	{
		if((WORD)szPosE[0] == nC)
		{
			*szPosE = 0;
			szOut[nCnt] = strdup(szPosS);
			nCnt++;
			szPosS = szPosE+1;

			if(nCnt >= nOutCnt-1)
				break;
		}

		if((WORD)szPosE[0] >= 0x80)
			szPosE+=2;
		else
			szPosE++;
	}
	szOut[nCnt] = strdup(szPosS);
	nCnt++;

	return nCnt;
}

MsgCommand s_msgCommandArray[] = {
	{"$sendmail", 3, DebugCmd::MsgCommand_SendSystemMail},
	{"$addpow", 1, DebugCmd::MsgCommand_AddPow},
	{"$addgold", 3, DebugCmd::MsgCommand_AddGold},
	{"$addpetexp", 2, DebugCmd::MsgCommand_AddPetExp},
	{"$addexp", 2, DebugCmd::MsgCommand_AddExp},
	{"$addrmb", 999, DebugCmd::MsgCommand_AddRmb},
	{"$additem", 3, DebugCmd::MsgCommand_AddItem},
	{"$addpet",3,DebugCmd::MsgCommand_AddPet},
	{"$addhorse",3,DebugCmd::MsgCommand_AddHorse},
	{"$msg",1,DebugCmd::MsgCommand_Msg},
	{"$jump",1,DebugCmd::MsgCommand_Jump},
	{"$full",1,DebugCmd::MsgCommand_Full},
	{"$fix",1,DebugCmd::MsgCommand_FixEquip},
	{"$chgskin",1,DebugCmd::MsgCommand_ChgSkin},
	{"$addhonour",3,DebugCmd::MsgCommand_AddHonour},
	{"$callenemy",3,DebugCmd::MsgCommand_CallEnemy},
	{"$addskill",2,DebugCmd::MsgCommand_AddSkill},
	{"$addstate",2,DebugCmd::MsgCommand_AddState},
	{"$addquest",2,DebugCmd::MsgCommand_AddQuest},
	{"$locknpc",2,DebugCmd::MsgCommand_LockNpcCmd},
	{"$kill",3,DebugCmd::MsgCommand_Kill},
	{"$push",1,DebugCmd::MsgCommand_Push},
	{"$pull",1,DebugCmd::MsgCommand_Pull},
	{"$worldtest",3,DebugCmd::MsgCommand_RefleshTest},
	{"$addactive",1, DebugCmd::MsgCommand_AddAcitve},
	{"$changetime",3, DebugCmd::MsgCommand_SetActiveOffsetTime},
	{"$addhomectb",3, DebugCmd::MsgCommand_AddHomeCtb},
	{"$addhomersc",3, DebugCmd::MsgCommand_AddHomeRsc},
	{"$addfortexp",3, DebugCmd::MsgCommand_AddFortExp},
	{"$addsvrexp",1, DebugCmd::MsgCommand_AddSvrExpMult},
	{"$setwealth",3, DebugCmd::MsgCommand_SetWealth},
	{"$setjjcpos",3, DebugCmd::MsgCommand_ChgWarRankPos},
	{"$killmapenemy",3, DebugCmd::MsgCommand_KillMapEnemyByID},
	{"$shutup",1, DebugCmd::MsgCommand_ForbidPlayer},
};

bool DebugCmd::DealMsg( UserData* pData, const char* szMsg,BYTE bGmLv )
{
	if(szMsg[0]	!= '$')
		return false;

	pData->m_pUserRecord->AddGMCodeLog(szMsg);
	char* szSplit[10];
	int nSpltCnt = SplitString(szMsg, '/', szSplit, 10);

	std::vector<char*> comstr;
	for (int i=1; i<nSpltCnt;i++)
		comstr.push_back(szSplit[i]);

	for(int i=0; i<m_nCommandSize; i++)
	{
		// 匹配字符串
		if(strcmp(szSplit[0], s_msgCommandArray[i].m_szMsgCommand) == 0)
		{
			// 判断权限
#ifndef _DEBUG
			if(s_msgCommandArray[i].m_byGmLv > bGmLv) return true;
#endif
			if((*s_msgCommandArray[i].m_lpFunction)(pData, comstr))
			{
				pData->SendWarnMsg("%s操作成功",szSplit[0]);
			}
			else
			{
				pData->SendWarnMsg("%s操作失败",szSplit[0]);
			}
			break;
		}
	}
	comstr.clear();
 	for (int j=0; j<nSpltCnt;j++){
 		if(szSplit[j]) delete szSplit[j];
 	}
	return true;
}

DebugCmd::DebugCmd()
{
	m_nCommandSize = sizeof(s_msgCommandArray) / sizeof(MsgCommand);
}

bool DebugCmd::MsgCommand_ForbidPlayer(UserData* pData, std::vector<char*> comstr)
{
	if (comstr.size() == 2)
	{
		g_pGameCenter->ForbidPlayer(atoi(comstr[0]),atoi(comstr[1]));
	}

	return true;
}

bool DebugCmd::MsgCommand_KillMapEnemyByID(UserData* pData, std::vector<char*> comstr)
{
	if (comstr.size() == 1)
	{
		g_pGameCenter->m_pCallEnemyCenter->KillMapEnemyByID(pData->m_nMIndex,atoi(comstr[0]));
	}else if (comstr.size() == 2)
	{
		g_pGameCenter->m_pCallEnemyCenter->KillMapEnemyByID(atoi(comstr[0]),atoi(comstr[1]));
	}

	return true;
}

bool DebugCmd::MsgCommand_ChgWarRankPos(UserData* pData, std::vector<char*> comstr)
{
	if (comstr.size() == 1)
	{
		g_pGameCenter->m_pWarRankCenter->GMChgWarRankPos(pData,atoi(comstr[0])-1);
	}

	return true;
}

bool DebugCmd::MsgCommand_SetWealth(UserData* pData, std::vector<char*> comstr)
{
	if (comstr.size() == 2)
	{
		pData->SetWealth(atoi(comstr[0]),atoi(comstr[1]));
		if (atoi(comstr[0]) == 6)
			pData->m_pVIPCenter->SendChgAutoRight(atoi(comstr[1]));
	}

	return true;
}

bool DebugCmd::MsgCommand_AddSvrExpMult(UserData* pData, std::vector<char*> comstr)
{
	if (comstr.size() == 1)
	{
		g_bSvrExpMult = true;
		g_pGameCenter->GMAddSvrExpMult(atoi(comstr[0]),false);
	}else if (comstr.size() == 2)
	{
		if (atoi(comstr[1]))
			g_bSvrExpMult = false;
		else
			g_bSvrExpMult = true;

		g_pGameCenter->GMAddSvrExpMult(atoi(comstr[0]),!g_bSvrExpMult);
	}

	return true;
}

bool DebugCmd::MsgCommand_AddFortExp(UserData* pData, std::vector<char*> comstr)
{
	if (comstr.size() == 1)
	{
		pData->GetFortData()->AddManorExp(atoi(comstr[0]));
	}

	return true;
}

bool DebugCmd::MsgCommand_AddHomeCtb(UserData* pData, std::vector<char*> comstr)
{
	if (comstr.size() == 1)
	{
		pData->AddContribute(atoi(comstr[0]));
	}

	return true;
}

bool DebugCmd::MsgCommand_AddHomeRsc(UserData* pData, std::vector<char*> comstr)
{
	if (comstr.size() == 1)
	{
		g_pGameCenter->m_pHomeCenter->AddHomeExp(pData->m_dwHomeID,atoi(comstr[0]));
	}

	return true;
}

bool DebugCmd::MsgCommand_SetActiveOffsetTime(UserData* pData, std::vector<char*> comstr)
{
	if (comstr.size() == 3)
	{
		g_pGameCenter->m_pActivityCenter->m_nOffsetTime = atoi(comstr[0])*24*60 + atoi(comstr[1])*60 + atoi(comstr[2]);
	}
	return true;
}

bool DebugCmd::MsgCommand_AddAcitve(UserData* pData, std::vector<char*> comstr)
{
	if (comstr.size() == 1)
	{
		g_pGameCenter->m_pGMActiveCenter->AddActive(atoi(comstr[0]));
	}else if (comstr.size() == 2)
	{
		g_pGameCenter->m_pGMActiveCenter->AddActive(atoi(comstr[0]),atoi(comstr[1]));
	}
	return true;
}

bool DebugCmd::MsgCommand_SendSystemMail( UserData* pUser, std::vector<char*> comstr )
{
	if(comstr.size() < 3)
		return false;
	if (comstr.size()  ==  3)
	{
		g_pGameCenter->m_pMailCenter->RequestSendMail(pUser->m_dwUin,atoi(comstr[0]),comstr[1],comstr[2]);

	}else if (comstr.size()  ==  7)
	{
		g_pGameCenter->m_pMailCenter->RequestSendItemMail(pUser->m_dwUin,atoi(comstr[0]),comstr[1],comstr[2],atoi(comstr[3]),atoi(comstr[4]),atoi(comstr[5]),atoi(comstr[6]));
	}
	return true;
}

bool DebugCmd::MsgCommand_AddHonour( UserData* pUser, std::vector<char*> comstr )
{
	if(comstr.size() == 1)
	{
		pUser->m_pTitleCenter->AddHonour(atoi(comstr[0]));
	}
	return true;
}

bool DebugCmd::MsgCommand_AddSkill( UserData* pUser, std::vector<char*> comstr )
{
	if(comstr.size() == 1)
		pUser->LearnSkill(atoi(comstr[0]),1);

	return true;
}

bool DebugCmd::MsgCommand_AddState( UserData* pUser, std::vector<char*> comstr )
{
	if(comstr.size() == 1)
		pUser->m_pMObjCenter->AddState(pUser->m_pMObjCenter->m_pPlayer,atoi(comstr[0]),false);
	else if(comstr.size() == 2)
		pUser->m_pMObjCenter->AddState(pUser->m_pMObjCenter->m_pPlayer,atoi(comstr[0]),atoi(comstr[1]));

	return true;
}

bool DebugCmd::MsgCommand_AddQuest( UserData* pUser, std::vector<char*> comstr )
{
	if(comstr.size() == 1)
		pUser->m_pQuestCenter->RequestAcceptQuest(atoi(comstr[0]),true);

	return true;
}

bool DebugCmd::MsgCommand_Kill( UserData* pUser, std::vector<char*> comstr )
{
	g_pGameCenter->m_pCallEnemyCenter->KillEnemy(pUser->m_nMIndex,pUser->GetPosX(),pUser->GetPosY(),500,pUser->m_dwUin);
	return true;
}

bool DebugCmd::MsgCommand_RefleshTest( UserData* pUser, std::vector<char*> comstr )
{
	g_pGameCenter->m_pWorldTestCenter->SetState(TEST_STATE_REFLESH);
	return true;
}

bool DebugCmd::MsgCommand_LockNpcCmd( UserData* pUser, std::vector<char*> comstr )
{
	if(comstr.size() == 1)
	{
		int		nNpcCmd	= atoi(comstr[0]);
		if(g_pGameCenter->m_pListLockNpcCmd->IndexOf((void*)nNpcCmd) == -1)
		{
			g_pGameCenter->m_pListLockNpcCmd->Add((void*)nNpcCmd);
		}
	}
	else if(comstr.size() == 2)
	{
		int		nNpcCmd	= atoi(comstr[0]);
		g_pGameCenter->m_pListLockNpcCmd->Remove((void*)nNpcCmd);
	}

	return true;
}

bool DebugCmd::MsgCommand_CallEnemy( UserData* pUser, std::vector<char*> comstr )
{
	if(comstr.size() == 5)
	{
		for (int i = 0; i < atoi(comstr[3]); i++)
		{
			DWORD	dwID	= g_pGameCenter->m_pCallEnemyCenter->CallEnemyByCfg(atoi(comstr[0]),pUser->m_nMIndex,pUser->m_pMObjCenter->m_pPlayer->nPosX,pUser->m_pMObjCenter->m_pPlayer->nPosY,atoi(comstr[1]),100,NULL,atoi(comstr[4]));
			if(atoi(comstr[2]))
				pUser->m_pMObjCenter->SetCtrl(dwID,MAPOBJ_ENEMY,atoi(comstr[2]));
		}
	}else if (comstr.size() == 4)
	{
		DWORD	dwID	= g_pGameCenter->m_pCallEnemyCenter->CallEnemyByCfg(atoi(comstr[0]),pUser->m_nMIndex,pUser->m_pMObjCenter->m_pPlayer->nPosX,pUser->m_pMObjCenter->m_pPlayer->nPosY,atoi(comstr[1]));
		if(atoi(comstr[2]))
			pUser->m_pMObjCenter->SetCtrl(dwID,MAPOBJ_ENEMY,atoi(comstr[2]));
	}
	return true;
}

bool DebugCmd::MsgCommand_AddPow( UserData* pData, std::vector<char*> comstr )
{
	if(comstr.size() == 0)
	{	
		pData->m_xUWealth.AddWealth(UW_POW,50);
	}
	else if(comstr.size() == 1)
	{
		pData->m_xUWealth.AddWealth(UW_POW,atoi(comstr[0]));
	}
	return true;
}

bool DebugCmd::MsgCommand_AddRmb( UserData* pData, std::vector<char*> comstr )
{
	if(comstr.size() == 0)
	{	
		pData->m_xUWealth.AddWealth(UW_RMB,9999,RMB_SAVE_DEBUGCODE);
	}
	else if(comstr.size() == 1)
	{
		pData->m_xUWealth.AddWealth(UW_RMB,atoi(comstr[0]),RMB_SAVE_DEBUGCODE);
	}
	return true;
}

bool DebugCmd::MsgCommand_AddGold( UserData* pData, std::vector<char*> comstr )
{
	if(comstr.size() == 0)
	{	
		pData->m_xUWealth.AddWealth(UW_GOLD,999999,1,NULL,UW_GOLD_ADD);
	}
	else if(comstr.size() == 1)
	{
		pData->m_xUWealth.AddWealth(UW_GOLD,atoi(comstr[0]),1,NULL,UW_GOLD_ADD);
	}
	return true;
}

bool DebugCmd::MsgCommand_AddExp( UserData* pData, std::vector<char*> comstr )
{
	if(comstr.size() == 0)
	{	
		pData->m_xUWealth.AddWealth(UW_EXP,9999);
	}
	else if(comstr.size() == 1)
	{
		pData->m_xUWealth.AddWealth(UW_EXP,atoi(comstr[0]));
	}
	return true;
}

bool DebugCmd::MsgCommand_AddItem( UserData* pData, std::vector<char*> comstr )
{
	if(comstr.size() == 2)
	{	

		pData->m_pItemCenter->AddItemByID(atoi(comstr[0]),atoi(comstr[1]),0,ITEM_LOG_ADD);
	}else if(comstr.size() == 3)
	{
		pData->m_pItemCenter->AddItemByID(atoi(comstr[0]),atoi(comstr[1]),atoi(comstr[2]),ITEM_LOG_ADD);
	}
	else if(comstr.size() == 1)
	{
		pData->m_pItemCenter->AddItemByID(atoi(comstr[0]),1,0,ITEM_LOG_ADD);
	}
	return true;
}

bool DebugCmd::MsgCommand_AddPet( UserData* pData, std::vector<char*> comstr )
{
	if(comstr.size() == 1)
	{	

		pData->m_pSlaveCenter->AddNewSlave(atoi(comstr[0]));
	}
	return true;
}

bool DebugCmd::MsgCommand_AddHorse( UserData* pData, std::vector<char*> comstr )
{
	if(comstr.size() == 1)
	{	

		pData->m_pHorseCenter->AddNewHorse(atoi(comstr[0]));
	}
	return true;
}

bool DebugCmd::MsgCommand_Msg( UserData* pData, std::vector<char*> comstr )
{
	if(comstr.size() == 3)
	{	

		ChatCenter::GetInstance().SendSystemMsg(comstr[0],atoi(comstr[1]),0,atoi(comstr[2]));
		//ChatCenter::GetInstance().SendSystemMsg("我我我我，人人人人，有有有有，的的的的，工工工工，要要要要，在在在在，地地地地，经经经经，以以以以，",atoi(comstr[1]),0,atoi(comstr[2]));
	}
	else if(comstr.size() == 4)
	{//  次数+间隔
		GMMsgObj*	pMsg	= new GMMsgObj();
		pMsg->szMsg	= strdup(comstr[0]);
		pMsg->bChannel	= atoi(comstr[1]);
		pMsg->nTime		= atoi(comstr[2]);
		pMsg->dwSplitSec	= atoi(comstr[3]);
		pMsg->dwTickNextShow	= GetTickCount();
		g_pGameCenter->m_pListNotice->Add(pMsg);
	}
	else if(comstr.size() == 0)
	{//关闭所有次数公告
		while(g_pGameCenter->m_pListNotice->Count)
		{
			GMMsgObj*	pMsg	= (GMMsgObj*)g_pGameCenter->m_pListNotice->Delete(0);
			if(pMsg->szMsg)
				free(pMsg->szMsg);
			delete pMsg;
		}
	}
	return true;
}

bool DebugCmd::MsgCommand_Jump( UserData* pData, std::vector<char*> comstr )
{
	if(comstr.size() == 3)
	{	

		pData->JumpTo(atoi(comstr[0]),atoi(comstr[1]),atoi(comstr[2]));
	}
	return true;
}

bool DebugCmd::MsgCommand_Pull( UserData* pData, std::vector<char*> comstr )
{
	if(comstr.size() == 1)
	{	
		UserData* pUser	= g_pGameCenter->GetPlayer(atoi(comstr[0]));
		if(pUser)
			pUser->JumpTo(pData->m_nMIndex,pData->m_pMObjCenter->m_pPlayer->nPosX,pData->m_pMObjCenter->m_pPlayer->nPosY);
	}
	return true;
}

bool DebugCmd::MsgCommand_Push( UserData* pData, std::vector<char*> comstr )
{
	if(comstr.size() == 1)
	{	
		UserData* pUser	= g_pGameCenter->GetPlayer(atoi(comstr[0]));
		if(pUser)
			pData->JumpTo(pUser->m_nMIndex,pUser->m_pMObjCenter->m_pPlayer->nPosX,pUser->m_pMObjCenter->m_pPlayer->nPosY);
	}
	return true;
}

bool DebugCmd::MsgCommand_Full( UserData* pData, std::vector<char*> comstr )
{
	pData->m_pMObjCenter->HealthHpMp(pData->m_pMObjCenter->m_pPlayer,pData->m_pMObjCenter->m_pPlayer->xParam.nHpMax,pData->m_pMObjCenter->m_pPlayer->xParam.nMpMax,true);
	return true;
}

bool DebugCmd::MsgCommand_FixEquip( UserData* pData, std::vector<char*> comstr )
{
	pData->m_pItemCenter->FixAllUsedEquip();
	return true;
}

bool DebugCmd::MsgCommand_ChgSkin( UserData* pData, std::vector<char*> comstr )
{
	if(comstr.size() == 3)
	{	
		pData->m_pMObjCenter->m_pPlayer->wSkin[0]	= atoi(comstr[0]);
		pData->m_pMObjCenter->m_pPlayer->wSkin[1]	= atoi(comstr[1]);
		pData->m_pMObjCenter->m_pPlayer->wSkin[2]	= atoi(comstr[2]);
		pData->m_pMObjCenter->m_pPlayer->bParamChged	= true;
	}
	return true;
}

bool DebugCmd::MsgCommand_AddPetExp( UserData* pData, std::vector<char*> comstr )
{
	if(comstr.size() == 0)
	{	
		pData->m_pSlaveCenter->AddExp(9999);
	}
	else if(comstr.size() == 1)
	{
		pData->m_pSlaveCenter->AddExp(atoi(comstr[0]));
	}
	return true;
	
}
