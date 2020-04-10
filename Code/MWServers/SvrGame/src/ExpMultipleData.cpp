#include "stdafx.h"
#include "ExpMultipleData.h"
#include "DBControl.h"
#include "UserData.h"
#include "ExpMultipleCfg.h"


UserExpCenter::UserExpCenter(UserData* pUser)
{
	m_pList = xnList::Create();
	m_pParent = pUser;
	m_nPetAdd = 0;
	m_nUserAdd = 0;
	m_nUserAddSec = 0;
	m_nPetAddSec = 0;
	m_ifSave = false;
	m_ifChg = false;
}

UserExpCenter::~UserExpCenter()
{
	if(m_pList)
	{
		while(m_pList->Count > 0)
			delete (UserExpObj*)m_pList->Delete(0);
		m_pList->Free();
	}
}

UserExpObj* UserExpCenter::GetObj(int nID)
{
	for(int i = 0; i < m_pList->Count; i ++)
	{
		UserExpObj* pObj = (UserExpObj*)m_pList->Items[i];
		if(pObj->nID == nID)
			return pObj;
	}
	return NULL;
}

void UserExpCenter::SendExp()
{
	BuildPacketEx(pPacket,USER,S_SEND_EXPMULTIPILE,buf,1024);
	SET_DATA(pData,USER,S_SEND_EXPMULTIPILE,pPacket);
	pData->nUserMultipile = 100+m_nUserAdd;
	pData->nPetMultipile = 100+m_nPetAdd;
	pData->nUserMultipileSec = m_nUserAddSec;
	pData->nPetMultipileSec = m_nPetAddSec;
	TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
}

void	UserExpCenter::Update()
{
	if(m_ifSave)
	{
		m_ifSave = false;
		SaveDB();
	}
	if(m_ifChg)
	{
		m_ifChg = false;
		ReclaExp();
		SendExp();
	}
}

void	UserExpCenter::UpdateSec()
{
	for(int i = m_pList->Count -1; i >= 0; i --)
	{
		UserExpObj* pObj = (UserExpObj*)m_pList->Items[i];
		if(pObj->nTimeSec > 0)
			pObj->nTimeSec--;
		else if(pObj->nTimeSec == 0)
		{
			if(pObj->ifSaveDB)
				m_ifSave = true;
			m_ifChg = true;
			m_pList->Delete(i);
			delete pObj;
		}
		if(pObj->nTimeSec%60 == 0)
			m_ifSave = true;
	}
}

bool	UserExpCenter::AddObj(int nID,int nTime)
{
	ExpMulObj* pCheck = ExpMultipleCfg::GetInstance().GetObj(nID);
	if(pCheck == NULL)
		return false;
	UserExpObj* pObj = GetObj(nID);
	if(pObj == NULL)
	{
		for(int i = m_pList->Count-1; i >= 0; i --)
		{
			UserExpObj* pTObj = (UserExpObj*)m_pList->Items[i];
			ExpMulObj* pTest = ExpMultipleCfg::GetInstance().GetObj(pTObj->nID);
			if(pTest && pTest->nFamily == pCheck->nFamily)
			{
				m_pList->Delete(i);
				if(pTObj->ifSaveDB)
					m_ifSave = true;
				delete pTObj;
			}
		}
		pObj = new UserExpObj();
		pObj->nID = nID;
		if(nTime != 0)
			pObj->nTimeSec = nTime;
		else
		{
			pObj->nTimeSec = pCheck->nTime;
		}
		if(!pCheck->ifDisOnline)
		{
			pObj->ifSaveDB = true;
			m_ifSave = true;
		}
		else
			pObj->ifSaveDB = false;
		m_pList->Add(pObj);
	}
	else
	{
		if(pCheck->ifTimeAdd)
		{
			if(nTime > 0)
				pObj->nTimeSec += nTime;
			else if(nTime = -1)
				pObj->nTimeSec = -1;
			else
			{
				if(pCheck->nTime == -1)
					pObj->nTimeSec = -1;
				else
					pObj->nTimeSec += pCheck->nTime;
			}
			if(pObj->ifSaveDB)
				m_ifSave = true;
		}
		else
		{
			if(nTime > 0 && nTime > pObj->nTimeSec)
				pObj->nTimeSec = nTime;
			else
			{
				if(pCheck->nTime == -1)
					pObj->nTimeSec = -1;
				else if(pCheck->nTime > pObj->nTimeSec)
					pObj->nTimeSec = pCheck->nTime;
			}
			if(pObj->ifSaveDB)
				m_ifSave = true;
		}
	}
	m_ifChg = true;
	return true;
}

void	UserExpCenter::CheckWhenDie()
{
	for(int i = m_pList->Count-1; i >= 0; i --)
	{
		UserExpObj* pObj = (UserExpObj*)m_pList->Items[i];
		ExpMulObj* pCheck = ExpMultipleCfg::GetInstance().GetObj(pObj->nID);
		if(pCheck == NULL)
		{
			if(pObj->ifSaveDB)
				m_ifSave = true;
			m_ifChg = true;
			m_pList->Delete(i);
			delete pObj;
		}
		else
		{
			if(pCheck->ifDisDie)
			{
				if(pObj->ifSaveDB)
					m_ifSave = true;
				m_ifChg = true;
				m_pList->Delete(i);
				delete pObj;
			}
		}
	}
}

bool	UserExpCenter::RemoveObj(int nID)
{
	for(int i = m_pList->Count-1; i >= 0; i --)
	{
		UserExpObj* pObj = (UserExpObj*)m_pList->Items[i];
		if(pObj->nID == nID)
		{
			if(pObj->ifSaveDB)
				m_ifSave = true;
			m_ifChg = true;
			m_pList->Delete(i);
			delete pObj;
			return true;
		}
	}
	return false;
}

bool	UserExpCenter::SaveDB()
{
	BYTE byBuf[2048] = {0};
	int nSize = 0;
	BYTE* p = byBuf;
	*(int*)p = m_pList->Count;
	nSize += 4;
	p = p + 4;
	for(int i = 0; i < m_pList->Count; i ++)
	{
		UserExpObj* pObj = (UserExpObj*)m_pList->Items[i];
		*(int*)p = pObj->nID;
		p = p + 4;
		*(int*)p = pObj->nTimeSec;
		p = p + 4;
		nSize += 8;
	}
	m_pParent->m_pDB->SetBLOB(FALSE,byBuf,nSize,"update "RPG_USER_TABLE" set expmultiple = ? where uin = %d",m_pParent->m_dwUin);
	return true;
}

bool	UserExpCenter::SetData(BYTE* pData,int nSize)
{
	if(nSize == 0 || nSize < 4)
		return false;
	BYTE* p = pData;
	int nCount = *(int*)p;
	if(nCount*8+4 < nSize)
		return false;
	p = p + 4;
	for(int i = 0; i < nCount; i ++)
	{
		int nID = *(int*)p;
		p = p + 4;
		int nTime = *(int*)p;
		p = p + 4;
		ExpMulObj* pCheck = ExpMultipleCfg::GetInstance().GetObj(nID);
		if(pCheck == NULL || pCheck->ifDisOnline)
			continue;
		UserExpObj* pObj = new UserExpObj();
		pObj->nID = nID;
		pObj->nTimeSec = nTime;
		pObj->ifSaveDB = true;
		m_pList->Add(pObj);
	}
	m_ifChg = true;
	return true;
}

void UserExpCenter::ReclaExp()
{
	m_nUserAdd = 0;
	m_nPetAdd = 0;
	m_nUserAddSec = 0;
	m_nPetAddSec = 0;
	for(int i = 0; i < m_pList->Count; i ++)
	{
		UserExpObj* pObj = (UserExpObj*)m_pList->Items[i];
		if(pObj->nTimeSec == -1 || pObj->nTimeSec > 0)
		{
			ExpMulObj* pCheck = ExpMultipleCfg::GetInstance().GetObj(pObj->nID);
			if(pCheck)
			{
				if(pCheck->byTargent == 0)
				{
					m_nUserAdd	+= pCheck->nAddPer;
					m_nUserAddSec += pObj->nTimeSec;
				}
				else
				{
					m_nPetAdd	+= pCheck->nAddPer;
					m_nPetAddSec += pObj->nTimeSec;
				}
			}
		}
	}
	if(m_nUserAdd < -100)
		m_nUserAdd = -100;
	if(m_nPetAdd < -100)
		m_nPetAdd = -100;
}

int UserExpCenter::GetExp(int nExp,BYTE byType)
{
	if(byType == 0)
	{
		nExp = nExp*(m_nUserAdd+100)/100;
	}
	else
		nExp = nExp*(m_nPetAdd+100)/100;
	return nExp;
}