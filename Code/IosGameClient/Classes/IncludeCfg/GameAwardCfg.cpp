#include "stdafx.h"
#include "GameAwardCfg.h"
#include "xnExtstr.h"

CreateSingleton(GameAwardConfig);

GameAwardConfig::GameAwardConfig()
{
	m_pOnlineList = xnList::Create();
	m_pMounthList = xnList::Create();
	m_pLvupList = xnList::Create();
	m_pLoginList = xnList::Create();
	m_pRechargeList = xnList::Create();
	m_pInviteList = xnList::Create();
	m_pLoginNewList = xnList::Create();
}

GameAwardConfig::~GameAwardConfig()
{
	if(m_pOnlineList)
	{
		while(m_pOnlineList->Count)
			delete (OnlineAward*)m_pOnlineList->Delete(0);
		m_pOnlineList->Free();
	}
	if(m_pMounthList)
	{
		while(m_pMounthList->Count)
			delete (MounthAward*)m_pMounthList->Delete(0);
		m_pMounthList->Free();
	}
	if(m_pLvupList)
	{
		while(m_pLvupList->Count)
			delete (LvupAward*)m_pLvupList->Delete(0);
		m_pLvupList->Free();
	}
	if (m_pLoginList)
	{
		while(m_pLoginList->Count)
			delete (LoginAward*)m_pLoginList->Delete(0);
		m_pLoginList->Free();
	}
	if(m_pRechargeList)
	{
		while(m_pRechargeList->Count)
			delete (RechargeAward*)m_pRechargeList->Delete(0);
		m_pRechargeList->Free();
	}
	if (m_pInviteList)
	{
		while(m_pInviteList->Count)
			delete (_simpleAward*)m_pInviteList->Delete(0);
		m_pInviteList->Free();
	}
	if (m_pLoginNewList)
	{
		while(m_pLoginNewList->Count)
			delete (LoginAward*)m_pLoginNewList->Delete(0);
		m_pLoginNewList->Free();
	}
}

void GameAwardConfig::LoadFile(char* pFile)
{
	xnExtstr* pStr = xnExtstr::Import(pFile);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	if(pStr->SetSection(nIndex++))
	{
		int nCnt = atoi(pStr->ReadString("cnt","0"));
		char szName[32];
		for(int i = 0; i < nCnt; i ++)
		{
			OnlineAward* pAward = new OnlineAward();
			sprintf(szName,"time%d",i+1);
			pAward->nTime = atoi(pStr->ReadString(szName,"0"));
			sprintf(szName,"item%d",i+1);
			pAward->xAward.nID = atoi(pStr->ReadString(szName,"0"));
			sprintf(szName,"num%d",i+1);
			pAward->xAward.nNum = atoi(pStr->ReadString(szName,"0"));
			m_pOnlineList->Add((void*)pAward);
		}
	}
	if(pStr->SetSection(nIndex++))
	{
		int nCnt = atoi(pStr->ReadString("cnt","0"));
		char szName[32];
		for(int i = 0; i < nCnt; i ++)
		{
			MounthAward* pAward = new MounthAward();
			sprintf(szName,"count%d",i+1);
			pAward->nCount = atoi(pStr->ReadString(szName,"0"));
			sprintf(szName,"item%d",i+1);
			pAward->xAward.nID = atoi(pStr->ReadString(szName,"0"));
			sprintf(szName,"num%d",i+1);
			pAward->xAward.nNum = atoi(pStr->ReadString(szName,"0"));
			m_pMounthList->Add((void*)pAward);
		}
	}
	if(pStr->SetSection(nIndex++))
	{
		int nCnt = atoi(pStr->ReadString("cnt","0"));
		char szName[32];
		for(int i = 0; i < nCnt; i ++)
		{
			LvupAward* pAward = new LvupAward();
			pAward->pList = xnList::Create();
			sprintf(szName,"lv%d",i+1);
			pAward->nLv = atoi(pStr->ReadString(szName,"0"));
			sprintf(szName,"item%d",i+1);
			char* pTemp1 = pStr->ReadString(szName);
			sprintf(szName,"num%d",i+1);
			char* pTemp2 = pStr->ReadString(szName);
			std::string strTemp1(pTemp1);
			std::string strTemp2(pTemp2);
			std::vector<std::string> vec1,vec2;
			StringSplit(strTemp1,"|",vec1);
			StringSplit(strTemp2,"|",vec2);
			for(int i = 0; i < vec1.size(); i ++)
			{
				_simpleAward* pSimA = new _simpleAward();
				pSimA->nID = atoi(vec1[i].c_str());
				pSimA->nNum= atoi(vec2[i].c_str());
				pAward->pList->Add((void*)pSimA);
			}
			m_pLvupList->Add((void*)pAward);
		}
	}
	if(pStr->SetSection(nIndex++))
	{
		int nCnt = atoi(pStr->ReadString("cnt","0"));
		char szName[32];
		for(int i = 0; i < nCnt; i ++)
		{
			RechargeAward* pAward = new RechargeAward();
			pAward->pList = xnList::Create();
			sprintf(szName,"day%d",i+1);
			pAward->nDays = atoi(pStr->ReadString(szName,"0"));
			sprintf(szName,"item%d",i+1);
			char* pTemp1 = pStr->ReadString(szName);
			sprintf(szName,"num%d",i+1);
			char* pTemp2 = pStr->ReadString(szName);
			std::string strTemp1(pTemp1);
			std::string strTemp2(pTemp2);
			std::vector<std::string> vec1,vec2;
			StringSplit(strTemp1,"|",vec1);
			StringSplit(strTemp2,"|",vec2);
			for(int i = 0; i < vec1.size(); i ++)
			{
				_simpleAward* pSimA = new _simpleAward();
				pSimA->nID = atoi(vec1[i].c_str());
				pSimA->nNum= atoi(vec2[i].c_str());
				pAward->pList->Add((void*)pSimA);
			}
			m_pRechargeList->Add((void*)pAward);
		}
	}
	if(pStr->SetSection(nIndex++))
	{
		int nCnt = atoi(pStr->ReadString("cnt","0"));
		char szName[32];
		for(int i = 0; i < nCnt; i ++)
		{
			LoginAward* pAward = new LoginAward();
			pAward->pList = xnList::Create();
			sprintf(szName,"day%d",i+1);
			pAward->nDays = atoi(pStr->ReadString(szName,"0"));
			sprintf(szName,"item%d",i+1);
			char* pTemp1 = pStr->ReadString(szName);
			sprintf(szName,"num%d",i+1);
			char* pTemp2 = pStr->ReadString(szName);
			std::string strTemp1(pTemp1);
			std::string strTemp2(pTemp2);
			std::vector<std::string> vec1,vec2;
			StringSplit(strTemp1,"|",vec1);
			StringSplit(strTemp2,"|",vec2);
			for(int i = 0; i < vec1.size(); i ++)
			{
				_simpleAward* pSimA = new _simpleAward();
				pSimA->nID = atoi(vec1[i].c_str());
				pSimA->nNum= atoi(vec2[i].c_str());
				pAward->pList->Add((void*)pSimA);
			}
			m_pLoginList->Add((void*)pAward);
		}
	}
	if(pStr->SetSection(nIndex++))
	{
		char* pTemp1 = pStr->ReadString("item");
		char* pTemp2 = pStr->ReadString("num");
		std::string strTemp1(pTemp1);
		std::string strTemp2(pTemp2);
		std::vector<std::string> vec1,vec2;
		StringSplit(strTemp1,"|",vec1);
		StringSplit(strTemp2,"|",vec2);
		for(int i = 0; i < vec1.size(); i ++)
		{
			_simpleAward* pSimA = new _simpleAward();
			pSimA->nID = atoi(vec1[i].c_str());
			pSimA->nNum= atoi(vec2[i].c_str());
			m_pInviteList->Add((void*)pSimA);
		}
	}
	if(pStr->SetSection(nIndex++))
	{
		int nCnt = atoi(pStr->ReadString("cnt","0"));
		char szName[32];
		for(int i = 0; i < nCnt; i ++)
		{
			LoginAward* pAward = new LoginAward();
			pAward->pList = xnList::Create();
			sprintf(szName,"day%d",i+1);
			pAward->nDays = atoi(pStr->ReadString(szName,"0"));
			sprintf(szName,"item%d",i+1);
			char* pTemp1 = pStr->ReadString(szName);
			sprintf(szName,"num%d",i+1);
			char* pTemp2 = pStr->ReadString(szName);
			std::string strTemp1(pTemp1);
			std::string strTemp2(pTemp2);
			std::vector<std::string> vec1,vec2;
			StringSplit(strTemp1,"|",vec1);
			StringSplit(strTemp2,"|",vec2);
			for(int i = 0; i < vec1.size(); i ++)
			{
				_simpleAward* pSimA = new _simpleAward();
				pSimA->nID = atoi(vec1[i].c_str());
				pSimA->nNum= atoi(vec2[i].c_str());
				pAward->pList->Add((void*)pSimA);
			}
			m_pLoginNewList->Add((void*)pAward);
		}
	}
	pStr->Free();
}

OnlineAward* GameAwardConfig::GetOnlineAward(BYTE byFlag)
{
	if(byFlag >= m_pOnlineList->Count)
		return NULL;
	return (OnlineAward*)m_pOnlineList->Items[byFlag];
}