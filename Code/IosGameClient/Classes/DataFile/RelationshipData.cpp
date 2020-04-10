#include "UserData.h"
#include "RelationshipData.h"
#include "MainMenu.h"
#include "MentoringCfg.h"

MentoringClient::MentoringClient()
{
	m_pRelationList = xnList::Create();
	m_pTeacherList = xnList::Create();
	m_pRequestList = xnList::Create();
	m_pMyRequestList = xnList::Create();
	m_nTListFlag = 0;
	m_ifInstruct = false;
	m_ifChg = false;
	m_ifTChg = false;
	m_ifRChg = false;
	m_ifShow = true;
}

MentoringClient::~MentoringClient()
{
	if(m_pRelationList)
	{
		while(m_pRelationList->Count)
			delete (MCData*)m_pRelationList->Delete(0);
		m_pRelationList->Free();
	}
	if(m_pRequestList)
	{
		while(m_pRequestList->Count)
			delete (MCRequest*)m_pRequestList->Delete(0);
		m_pRequestList->Free();
	}
	if(m_pMyRequestList)
	{
		while(m_pMyRequestList->Count)
			m_pMyRequestList->Delete(0);
		m_pMyRequestList->Free();
	}
}

bool MentoringClient::PacketProcess(ZPacket* pPacket)
{
	if(pPacket->bCmdGroup != RPGRELATION)
		return false;
	switch(pPacket->bCmd)
	{
	case S_SEND_INSTRUCT_CHG:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_INSTRUCT_CHG,pPacket);
			m_ifInstruct = pRecv->ifInstruct;
		}
		return true;
	case S_SEND_SHOWFLAG_CHG:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_SHOWFLAG_CHG,pPacket);
			m_ifShow = pRecv->ifShow;
		}
		return true;
	case S_SEND_MENTORING_ALL:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_MENTORING_ALL,pPacket);
			for(int i = 0; i < pRecv->nCnt; i ++)
			{
				MCData* pData = new MCData();
				memcpy(pData,&pRecv->szInfo[i],sizeof(MCData));
				m_pRelationList->Add(pData);
			}
			m_ifChg = true;
		}
		return true;
	case S_SEND_MENTOR_MSG:
		{
			SET_DATA(pRecv,RPGRELATION, S_SEND_MENTOR_MSG, pPacket);
			ShowMentorAsk( pRecv->szName, pRecv->dwSFUin );
		}
		return true;
	case S_SEND_MREQUEST_ALL:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_MREQUEST_ALL,pPacket);
			for(int i = 0; i < pRecv->nCnt; i ++)
			{
				MCRequest* pData = new MCRequest();
				memcpy(pData,&pRecv->szInfo[i],sizeof(MCRequest));
				m_pRequestList->Add(pData);
			}
			m_ifRChg = true;
		}
		return true;
	case S_SEND_MYMREQUEST_ALL:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_MYMREQUEST_ALL,pPacket);
			for(int i = 0; i < pRecv->nCnt; i ++)
			{
				m_pMyRequestList->Add((void*)pRecv->nUin[i]);
				m_ifTChg = true;
			}
		}
		return true;
	case S_SEND_TEACHERLIST_RLT:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_TEACHERLIST_RLT,pPacket);
			if(pRecv->ifClear)
			{
				while(m_pTeacherList->Count)
					delete (MCRequest*)m_pTeacherList->Delete(0);
			}
			m_nTListFlag = pRecv->nFlag;
			for(int i = 0; i < pRecv->nCnt; i ++)
			{
				MCRequest* pData = new MCRequest();
				memcpy(pData,&pRecv->szInfo[i],sizeof(MCRequest));
				m_pTeacherList->Add(pData);
				m_ifTChg = true;
			}
		}
		return true;
	case S_SEND_MENTORING_ADD:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_MENTORING_ADD,pPacket);
			AddData(&pRecv->xInfo);
		}
		return true;
	case S_SEND_MENTORING_CHG:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_MENTORING_CHG,pPacket);
			UpdateData(&pRecv->xInfo);
		}
		return true;
	case S_SEND_MENTORING_DEL:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_MENTORING_DEL,pPacket);
			DeleteData(pRecv->nID);
		}
		return true;
	case S_SEND_MREQUEST_ADD:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_MREQUEST_ADD,pPacket);
			AddRequest(&pRecv->xInfo);
		}
		return true;
	case S_SEND_MREQUEST_DEL:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_MREQUEST_DEL,pPacket);
			DeleteRequest(pRecv->nID);
		}
		return true;
	case S_SEND_MYMREQUEST_ADD:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_MYMREQUEST_ADD,pPacket);
			AddMyRequest(pRecv->nUin);
		}
		return true;
	case S_SEND_MYMREQUEST_DEL:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_MYMREQUEST_DEL,pPacket);
			DeleteMyRequest(pRecv->nID);
		}
		return true;
	case S_SEND_HONOUR_ADD:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_HONOUR_ADD,pPacket);
			UpdateHonour(pRecv->nHonour,pRecv->dwUin);
		}
		return true;
	}
	return false;
}

bool	MentoringClient::CheckGift()
{
	for(int i = 0; i < m_pRelationList->Count; i ++)
	{
		MCData* pData = (MCData*)m_pRelationList->Items[i];
		if(pData->dwPupil == pChar->m_dwUin)
		{
			if(pData->bTeachFlag == 1)
				return true;
			else
				return false;
		}
		else if(pData->dwTeacher == pChar->m_dwUin)
		{
			if(pData->bStudyFlag == 1)
				return true;
			else
				return false;
		}
	}
	return false;
}

void	MentoringClient::AddData(MCData* pData)
{
	MCData* pNew = new MCData();
	memcpy(pNew,pData,sizeof(MCData));
	m_pRelationList->Add(pNew);
	m_ifChg = true;
}

void	MentoringClient::UpdateData(MCData* pData)
{
	for(int i = 0; i < m_pRelationList->Count; i ++)
	{
		MCData* pCheck = (MCData*)m_pRelationList->Items[i];
		if(pCheck->dwID == pData->dwID)
		{
			memcpy(pCheck,pData,sizeof(MCData));
			m_ifChg = true;
		}
	}
}
void	MentoringClient::DeleteData(int nID)
{
	for(int i = 0; i < m_pRelationList->Count; i ++)
	{
		MCData* pCheck = (MCData*)m_pRelationList->Items[i];
		if(pCheck->dwID == nID)
		{
			delete (MCData*)m_pRelationList->Delete(i);
			m_ifChg = true;
		}
	}
}
void	MentoringClient::AddRequest(MCRequest* pData)
{
	MCRequest* pNew = new MCRequest();
	memcpy(pNew,pData,sizeof(MCRequest));
	m_pRequestList->Add(pNew);
	m_ifRChg = true;
}
void	MentoringClient::DeleteRequest(int nID)
{
	for(int i = 0; i < m_pRequestList->Count; i ++)
	{
		MCRequest* pCheck = (MCRequest*)m_pRequestList->Items[i];
		if(pCheck->dwPuin == nID)
		{
			m_pRequestList->Delete(i);
			m_ifRChg = true;
		}
	}
}

void	MentoringClient::AddMyRequest(int nUin)
{
	m_pMyRequestList->Add((void*)nUin);
	m_ifTChg = true;
}
void	MentoringClient::DeleteMyRequest(int nID)
{
	for(int i = 0; i < m_pMyRequestList->Count; i ++)
	{
		int nCheck = (int)m_pMyRequestList->Items[i];
		if(nCheck == nID)
		{
			m_pMyRequestList->Delete(i);
			m_ifTChg = true;
		}
	}
}

bool	MentoringClient::ClearList()
{
	while(m_pRelationList->Count)
		delete (MCData*)m_pRelationList->Delete(0);
	while(m_pRequestList->Count)
		delete (MCRequest*)m_pRequestList->Delete(0);
	while(m_pMyRequestList->Count)
		m_pMyRequestList->Delete(0);
	m_ifChg = true;
	m_ifTChg = true;
	m_ifRChg = true;
	m_ifInstruct = false;
	m_nTListFlag = 0;
	return true;
}

void	MentoringClient::RequestTeacherList()
{
	BuildPacketEx(pPacket,RPGRELATION,C_REQUEST_TEACHERLIST,buf,1024);
	SET_DATA(pData,RPGRELATION,C_REQUEST_TEACHERLIST,pPacket);
	pData->nFlag = m_nTListFlag;
	SendPacket(pPacket);
}

void	MentoringClient::RequestInstruct()
{
	SendCmd(RPGRELATION,C_REQUEST_INSTRUCT);
}

void	MentoringClient::RequestSendItem(int nPos)
{
	if(nPos >= 0 && nPos < m_pRelationList->Count)
	{
		MCData* pData = (MCData*)m_pRelationList->Items[nPos];
		if(pData->dwPupil == pChar->m_dwUin && pData->bStudyFlag == 0)
		{
			BuildPacketEx(pPacket,RPGRELATION,C_REQUEST_SENDITEM,buf,1024);
			SET_DATA(pSend,RPGRELATION,C_REQUEST_SENDITEM,pPacket);
			pSend->nID = pData->dwID;
			SendPacket(pPacket);
		}
		else if(pData->dwTeacher == pChar->m_dwUin && pData->bTeachFlag == 0)
		{
			BuildPacketEx(pPacket,RPGRELATION,C_REQUEST_SENDITEM,buf,1024);
			SET_DATA(pSend,RPGRELATION,C_REQUEST_SENDITEM,pPacket);
			pSend->nID = pData->dwID;
			SendPacket(pPacket);
		}
	}
}

void	MentoringClient::RequestGetItem(int nPos)
{
	if(nPos >= 0 && nPos < m_pRelationList->Count)
	{
		MCData* pData = (MCData*)m_pRelationList->Items[nPos];
		if(pData->dwPupil == pChar->m_dwUin && pData->bTeachFlag == 1)
		{
			BuildPacketEx(pPacket,RPGRELATION,C_REQUEST_GETITEM,buf,1024);
			SET_DATA(pSend,RPGRELATION,C_REQUEST_GETITEM,pPacket);
			pSend->nID = pData->dwID;
			SendPacket(pPacket);
		}
		else if(pData->dwTeacher == pChar->m_dwUin && pData->bStudyFlag == 1)
		{
			BuildPacketEx(pPacket,RPGRELATION,C_REQUEST_GETITEM,buf,1024);
			SET_DATA(pSend,RPGRELATION,C_REQUEST_GETITEM,pPacket);
			pSend->nID = pData->dwID;
			SendPacket(pPacket);
		}
	}
}

void	BreakRelationAskReturn(void* param,int nVal)
{
	if(nVal == 1)
	{
		BuildPacketEx(pPacket,RPGRELATION,C_REQUEST_BREAKRELATION,buf,1024);
		SET_DATA(pSend,RPGRELATION,C_REQUEST_BREAKRELATION,pPacket);
		pSend->nID = (int)param;
		pChar->SendPacket(pPacket);
	}
}

void	MentoringClient::RequestBreakRelation(int nPos)
{
	if(nPos >= 0 && nPos < m_pRelationList->Count)
	{
		MCData* pData = (MCData*)m_pRelationList->Items[nPos];
		char szMsg[128];
		if(pData->dwPupil == pChar->m_dwUin)
		{
			sprintf(szMsg,"是否与导师[%s]脱离关系？脱离后将在24小时内无法寻找导师",pData->szTName);
			pMainMenu->PopAskMsgBox(szMsg,BreakRelationAskReturn,(void*)pData->dwID);
		}
		else if(pData->dwTeacher == pChar->m_dwUin)
		{
			sprintf(szMsg,"是否开除学生[%s]？开除学生需支付100000金币，并将在24小时内无法收徒",pData->szPName);
			pMainMenu->PopAskMsgBox(szMsg,BreakRelationAskReturn,(void*)pData->dwID);
		}
	}
}

void	MentoringClient::RequestDealWith(int nPos,bool ifOK)
{
	if(nPos >= 0 && nPos < m_pRequestList->Count)
	{
		MCRequest* pData = (MCRequest*)m_pRequestList->Items[nPos];
		
		BuildPacketEx(pPacket,RPGRELATION,C_REQUEST_DEALWITH,buf,1024);
		SET_DATA(pSend,RPGRELATION,C_REQUEST_DEALWITH,pPacket);
		pSend->nUin = pData->dwPuin;
		pSend->ifOk = ifOK;
		SendPacket(pPacket);
	}
}

void MentoringClient::RequestDealWithST( DWORD dwStuUin, bool ifOK )
{
	BuildPacketEx(pPacket,RPGRELATION,C_REQUEST_DEALWITH_ST,buf,1024);
	SET_DATA(pSend,RPGRELATION,C_REQUEST_DEALWITH_ST,pPacket);
	pSend->nUin = dwStuUin;
	pSend->ifOk = ifOK;
	SendPacket(pPacket);
}
void MentoringClient::RequestMsg( DWORD dwStuUin )
{
	BuildPacketEx(pPacket,RPGRELATION,C_SEND_MENTOR_MSG,buf,1024);
	SET_DATA(pSend,RPGRELATION,C_SEND_MENTOR_MSG,pPacket);
	pSend->dwStuUin = dwStuUin;
	SendPacket(pPacket);
}

void	MentorAskRltFunc(void*param,int nVal)
{

	if(nVal == 1)
		pChar->m_pMentoringClient->RequestDealWithST((DWORD)param, true);
	else
		pChar->m_pMentoringClient->RequestDealWithST((DWORD)param, false);
}

void MentoringClient::ShowMentorAsk( char* szName, int nSFUin )
{
	char szMsg[128];
	sprintf(szMsg,"%s想要收你为学生，您是否愿意？",szName);
	pMainMenu->PopAskMsgBox(szMsg, MentorAskRltFunc, (void*)nSFUin);
}

void	MentoringClient::RequestStudy(int nPos)
{
	if(nPos >= 0 && nPos < m_pTeacherList->Count)
	{
		MCRequest* pData = (MCRequest*)m_pTeacherList->Items[nPos];
		if(CheckTeacherUin(pData->dwPuin))
			return;
		BuildPacketEx(pPacket,RPGRELATION,C_REQUEST_STUDY,buf,1024);
		SET_DATA(pSend,RPGRELATION,C_REQUEST_STUDY,pPacket);
		pSend->nUin = pData->dwPuin;
		SendPacket(pPacket);
	}
}

bool	MentoringClient::CheckTeacherUin(int nUin)
{
	for(int i = 0; i < m_pMyRequestList->Count; i ++)
	{
		int nCheck = (int)m_pMyRequestList->Items[i];
		if(nCheck == nUin)
			return true;
	}
	return false;
}

void MentoringClient::UpdateHonour( int nHonour,DWORD dwUin )
{
	for(int i = 0; i < m_pRelationList->Count; i ++)
	{
		MCData* pCheck = (MCData*)m_pRelationList->Items[i];
		if(pCheck && pCheck->dwTeacher == dwUin)
		{
			pCheck->nHonour = nHonour;
			m_ifChg = true;
		}
	}
}

MarryClient::MarryClient()
{
	m_pRelationList = xnList::Create();
	m_pRequestList = xnList::Create();
	m_nRListFlag = 0;
	m_ifInstruct = false;
	m_ifShowTitle = false;
	m_ifChg = false;
	m_ifRChg = false;

	m_bMarryRingRequest = false;
}

MarryClient::~MarryClient()
{
	if(m_pRelationList)
	{
		while(m_pRelationList->Count)
			delete (ClientMarryData*)m_pRelationList->Delete(0);
		m_pRelationList->Free();
	}
	if(m_pRequestList)
	{
		while(m_pRequestList->Count)
			delete (MCRequest*)m_pRequestList->Delete(0);
		m_pRequestList->Free();
	}
}

bool	MarryClient::PacketProcess(ZPacket* pPacket)
{
	if(pPacket->bCmdGroup != RPGRELATION)
		return false;
	switch(pPacket->bCmd)
	{
	case S_SEND_MARRY_REQUESTLIST:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_MARRY_REQUESTLIST,pPacket);
			if(pRecv->ifClear)
			{
				while(m_pRequestList->Count)
					delete (MCRequest*)m_pRequestList->Delete(0);
			}
			m_nRListFlag = pRecv->nFlag;
			for(int i = 0; i < pRecv->nCnt; i ++)
			{
				MCRequest* pData = new MCRequest();
				memcpy(pData,&pRecv->szInfo[i],sizeof(MCRequest));
				m_pRequestList->Add(pData);
			}
			m_ifRChg = true;
		}
		return true;
	case S_SEND_MARRY_ALL:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_MARRY_ALL,pPacket);
			for(int i = 0; i < pRecv->nCnt; i ++)
			{
				ClientMarryData* pData = new ClientMarryData();
				memcpy(pData,&pRecv->szInfo[i],sizeof(ClientMarryData));
				m_pRelationList->Add(pData);
			}
			m_ifChg = true;
		}
		return true;
	case S_SEND_MARRY_INSTRUCT:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_MARRY_INSTRUCT,pPacket);
			m_ifInstruct = pRecv->ifInstruct;
		}
		return true;
	case S_SEND_MARRY_DATA_ADD:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_MARRY_DATA_ADD,pPacket);
			AddData(&pRecv->xInfo);
		}
		return true;
	case S_SEND_MARRY_DATA_CHG:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_MARRY_DATA_CHG,pPacket);
			UpdateData(&pRecv->xInfo);
		}
		return true;
	case S_SEND_MARRY_DATA_DEL:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_MARRY_DATA_DEL,pPacket);
			DeleteData(pRecv->nID);
		}
		return true;
	case S_MARRY_SENDASK:
		{
			SET_DATA(pRecv,RPGRELATION,S_MARRY_SENDASK,pPacket);
			ShowMarryAsk(pRecv->nUin,pRecv->szName);
		}
		return true;
	case S_SEND_GETMARRIED:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_GETMARRIED,pPacket);
			ShowGetmarriedAsk(pRecv->szName);
		}
		return true;
	case S_SEND_POP_MARRYMESSAGE:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_POP_MARRYMESSAGE,pPacket);
			m_bMarryRingRequest = true;
			strncpy(m_szMsgTitle,pRecv->szTitle,22);
			strncpy(m_szMsgDetail,pRecv->szMsg,128);
			
		}
		return true;
	case S_SEND_MARRY_SHOW_TITLE:
		{
			SET_DATA(pRecv,RPGRELATION,S_SEND_MARRY_SHOW_TITLE,pPacket);
			m_ifShowTitle = pRecv->ifShow;
		}
		return true;
	}
	return false;
}

ClientMarryData*	MarryClient::GetRelation()
{
	for(int i = 0; i < m_pRelationList->Count; i ++)
	{
		ClientMarryData* pData = (ClientMarryData*)m_pRelationList->Items[i];
		if(pData->byState > 0)
			return pData;
	}
	return NULL;
}

void	MarryClient::AddData(ClientMarryData* pData)
{
	ClientMarryData* pNew = new ClientMarryData();
	memcpy(pNew,pData,sizeof(ClientMarryData));
	m_pRelationList->Add(pNew);
	m_ifChg = true;
}

void	MarryClient::UpdateData(ClientMarryData* pData)
{
	for(int i = 0; i < m_pRelationList->Count; i ++)
	{
		ClientMarryData* pCheck = (ClientMarryData*)m_pRelationList->Items[i];
		if(pCheck->dwID == pData->dwID)
		{
			memcpy(pCheck,pData,sizeof(ClientMarryData));
			m_ifChg = true;
		}
	}
}
void	MarryClient::DeleteData(int nID)
{
	for(int i = 0; i < m_pRelationList->Count; i ++)
	{
		ClientMarryData* pCheck = (ClientMarryData*)m_pRelationList->Items[i];
		if(pCheck->dwID == nID)
		{
			delete (ClientMarryData*)m_pRelationList->Delete(i);
			m_ifChg = true;
		}
	}
}

bool	MarryClient::ClearList()
{
	while(m_pRelationList->Count)
		delete (ClientMarryData*)m_pRelationList->Delete(0);
	while(m_pRequestList->Count)
		delete (MCRequest*)m_pRequestList->Delete(0);
	
	m_ifChg = true;
	m_ifRChg = true;
	m_ifInstruct = false;
	m_nRListFlag = 0;
	return true;
}

void	MarryClient::GetRequestList()
{
	BuildPacketEx(pPacket,RPGRELATION,C_REQUEST_MARRY_LIST,buf,1024);
	SET_DATA(pData,RPGRELATION,C_REQUEST_MARRY_LIST,pPacket);
	pData->nFlag = m_nRListFlag;
	SendPacket(pPacket);
}

void	MarryClient::RequestInstruct()
{
	if(pChar->m_byLvl < 30)
	{
		pMainMenu->PopTipMsg("您还未成年，30级才能寻找有缘人。");
		return;
	}
	SendCmd(RPGRELATION,C_REQUEST_MARRY_INSTRUCT);
}

void	MarryClient::CancelInstruct()
{
	SendCmd(RPGRELATION,C_REQUEST_CANCEL_MINSTRUCT);
}

void	MarryClient::ShowMarryTitle(bool ifShow)
{
	BuildPacketEx(pPacket,RPGRELATION,C_REQUEST_MARRY_SHOW_TITLE,buf,1024);
	SET_DATA(pData,RPGRELATION,C_REQUEST_MARRY_SHOW_TITLE,pPacket);
	pData->ifShow = ifShow;
	SendPacket(pPacket);
}

void	BreakMarryAskReturn(void* param,int nVal)
{
	if(nVal == 1)
	{
		pChar->SendCmd(RPGRELATION,C_REQUEST_MARRY_BREAKE);
	}
}

void	MarryClient::RequestBreakRelation()
{
	if(m_pRelationList->Count == 0)
		return;
	ClientMarryData* pData = (ClientMarryData*)m_pRelationList->Items[0];
	char szMsg[128];
	if(pData->dwMan == pChar->m_dwUin)
	{
		sprintf(szMsg,"是否与[%s]脱离夫妻关系？需支付1000000金币",pData->szWName);
		pMainMenu->PopAskMsgBox(szMsg,BreakMarryAskReturn,NULL);
	}
	else if(pData->dwWoman == pChar->m_dwUin)
	{
		sprintf(szMsg,"是否与[%s]脱离夫妻关系？需支付1000000金币",pData->szMName);
		pMainMenu->PopAskMsgBox(szMsg,BreakMarryAskReturn,NULL);
	}
}

void	MarryClient::RequestUpSkill(BYTE byPos)
{
	if(byPos >= MarrySkillCfg::GetInstance().m_pList->Count)
		return;
	BuildPacketEx(pPacket,RPGRELATION,C_REQUEST_MARRY_SKILL,buf,1024);
	SET_DATA(pData,RPGRELATION,C_REQUEST_MARRY_SKILL,pPacket);
	pData->byPos = byPos;
	SendPacket(pPacket);
}

void	MarryClient::SendRing(BYTE byPos)
{
	if(byPos == 1 && pChar->m_dwRmb < 52)
	{
		pMainMenu->PopTipMsg("需要52钻石");
		return;
	}
	else if(byPos == 2 && pChar->m_dwRmb < 520)
	{
		pMainMenu->PopTipMsg("需要520钻石");
		return;
	}
	else if(byPos == 0 && pChar->m_dwMoney < 50000)
	{
		if(pChar->m_dwMoney < 50000)
		{
			pMainMenu->PopTipMsg("需要50000金币");
			return;
		}
		if(pChar->m_pClientFlag->GetFlag(30) >= 5)
		{
			pMainMenu->PopTipMsg("今日的赠送次数已用完");
			return;
		}
	}

	BuildPacketEx(pPacket,RPGRELATION,C_MARRY_SENDRING,buf,1024);
	SET_DATA(pData,RPGRELATION,C_MARRY_SENDRING,pPacket);
	pData->byPos = byPos;
	SendPacket(pPacket);
}

void	MarryAskRltFunc(void*param,int nVal)
{
	BuildPacketEx(pPacket,RPGRELATION,C_MARRY_SENDANSWER,buf,1024);
	SET_DATA(pData,RPGRELATION,C_MARRY_SENDANSWER,pPacket);
	pData->nUin = (int)param;
	if(nVal == 1)
		pData->bRlt = true;
	else
		pData->bRlt = false;
	pChar->SendPacket(pPacket);
}

void	MarryClient::ShowMarryAsk(int nUin,char* szName)
{
	char szMsg[128];
	sprintf(szMsg,"爱需要大胆说出来。%s向您献上钻戒一枚，您是否愿意与%s订下婚约?",szName,pChar->m_bySex==1?"她":"他");
	pMainMenu->m_pMarryAskMenu->PopMsgbox("求婚",szMsg,MarryAskRltFunc,(void*)nUin);
}

void	GetmarriedAskRltFunc(void*param,int nVal)
{
	BuildPacketEx(pPacket,RPGRELATION,C_ANSWER_GETMARRIED,buf,1024);
	SET_DATA(pData,RPGRELATION,C_ANSWER_GETMARRIED,pPacket);
	if(nVal == 1)
		pData->bRlt = true;
	else
		pData->bRlt = false;
	pChar->SendPacket(pPacket);
}

void	MarryClient::ShowGetmarriedAsk(char* szName)
{
	char szMsg[128];
	sprintf(szMsg,"%s,你愿意接受%s作为你的合法%s吗？",pChar->m_szNick,szName,pChar->m_bySex==1?"妻子":"丈夫");
	pMainMenu->m_pMarryAskMenu->PopMsgbox("求婚",szMsg,GetmarriedAskRltFunc,NULL);
}

void	MarryClient::SendActionEvent(BYTE byType)
{
	BuildPacketEx(pPacket,RPGRELATION,C_MARRY_ACTION_EVENT,buf,1024);
	SET_DATA(pData,RPGRELATION,C_MARRY_ACTION_EVENT,pPacket);
	pData->byAction = byType;
	SendPacket(pPacket);
}