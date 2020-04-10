#include "stdafx.h"
#include "HorseData.h"
#include "DBControl.h"
#include "UserData.h"
#include "ios_yjgslave_packet.h"

HorseBaseData::HorseBaseData(UserData* pUser)
{
	memset(m_nParam,0,4*HORSE_PARAM_MAX);
	m_pNumData[HORSE_NUM_CULTIVATE] = new NumberData(HORSE_CULTIVATE_MAX);
	memset(m_szTempCul,0,4*HORSE_CULTIVATE_MAX);
	m_dwUin = 0;
	m_dwID = 0;
	m_bySlot = 0;
	m_byLevel = 0;
	m_nExp = 0;
	m_ifChange = false;
	m_ifSendExp = false;
	m_ifEquip = false;
	m_ifSendCul = false;
	m_pMaster = pUser;
	CLuaClassObject<HorseBaseData>::Init(this);
}

HorseBaseData::~HorseBaseData()
{
	for(int i = 0; i < HORSE_NUM_MAX; i ++)
	{
		if(m_pNumData[i])
			delete m_pNumData[i];
	}
}

void HorseBaseData::CreateScriptClass( CLuaState * pLuaState )
{
	CLuaClass<HorseBaseData> * pscHorseData = pLuaState->CreateScriptClass((HorseBaseData*)0);

	 	pscHorseData->DefObjectFunction("GetLevel", &HorseBaseData::GetLevel)
			.DefObjectFunction("GetQuality", &HorseBaseData::GetQuality)
	 		.DefObjectFunction("GetBaseID", &HorseBaseData::GetBaseID);
}

void HorseBaseData::ReleaseScriptClass( CLuaState * pLuaState )
{
	pLuaState->ReleaseScriptClass((HorseBaseData*)0);
}

LUAFUN	int		HorseBaseData::GetLevel()
{
	return m_byLevel;
}

LUAFUN	int		HorseBaseData::GetBaseID()
{
	return m_nType;
}

LUAFUN	int		HorseBaseData::GetQuality()
{
	HorseBasic* pBasic = HorseBaseCfg::GetInstance().GetBasic(m_nType);
	if(pBasic)
		return pBasic->byQuality;
	return 0;
}

void HorseBaseData::AddExp(int nExp)
{
	HorseBasic* pBasic = HorseBaseCfg::GetInstance().GetBasic(m_nType);
	if(pBasic == NULL)
		return;
	BYTE byNowLv = m_byLevel;
	int nNowExp = m_nExp;
	int nTotalExp = nExp;
	int nCount = 0;
	bool ifLvup = false;
	while(nTotalExp > 0)
	{
		if(nCount > 100)
			break;
		if(byNowLv >= pBasic->byLvLimit || byNowLv >= m_pMaster->m_xUWealth.GetWealth(UW_LV) + 5)
		{
			nTotalExp = 0;
			break;
		}
		int nLimit = HorseBaseCfg::GetInstance().GetLvExp(byNowLv);
		if(nNowExp + nTotalExp >= nLimit)
		{
			nTotalExp = nNowExp + nTotalExp - nLimit;
			nNowExp = 0;
			byNowLv ++;
			ifLvup = true;
		}
		else
		{
			nNowExp += nTotalExp;
			nTotalExp = 0;
		}
		nCount ++;
	}
	if(nCount > 0)
	{
		m_byLevel = byNowLv;
		m_nExp = nNowExp;
		m_ifSendExp = true;
	}
	if(ifLvup)
	{
		m_pMaster->m_pUserRecord->AddSlaveLog(m_dwID,m_nType,m_byLevel,0,SLAVE_SAVE_HORSE_LVUP);
		m_pMaster->m_pUserRecord->RunLuaHorseFunc(2,this,0);
	}
}

void HorseBaseData::RecalParam()
{
	memset(m_nParam,0,4*HORSE_PARAM_MAX);
	HorseBasic* pBasic = HorseBaseCfg::GetInstance().GetBasic(m_nType);
	if(pBasic != NULL)
	{
		for(int i = 0; i < HORSE_PARAM_MAX; i ++)
		{
			m_nParam[i] += pBasic->szBaseParam[i];
			m_nParam[i] += (m_byLevel-1)*pBasic->szBaseUp[i];
			m_nParam[i] += m_pNumData[HORSE_NUM_CULTIVATE]->GetVal(i);
			if(m_nParam[i] < 0)
				m_nParam[i] = 0;
		}
	}
}

HorseCenter::HorseCenter(UserData* pUser)
{
	m_pParent = pUser;
	m_pHorseList = xnList::Create();
	m_dwEquipID = 0;
	m_pEquipHorse = NULL;
	m_ifEquipChange = false;
	m_ifRide = false;
	m_ifRideChange = false;
	memset(m_szHorseCount,0,20);
	CLuaClassObject<HorseCenter>::Init(this);
}

HorseCenter::~HorseCenter()
{
	if(m_pHorseList)
	{
		while(m_pHorseList->Count)
			delete (HorseBaseData*)m_pHorseList->Delete(0);
		m_pHorseList->Free();
	}
}

void HorseCenter::CreateScriptClass( CLuaState * pLuaState )
{
	CLuaClass<HorseCenter> * pscHorseCenter = pLuaState->CreateScriptClass((HorseCenter*)0);

	// 	pscItemCenter->DefObjectFunction("GetBlankBag", &ItemCenter::GetBlankBag)
	// 		.DefObjectFunction("AddItemByID", &ItemCenter::AddItemByID)
	// 		.DefObjectFunction("RemoveItemByIndex", &ItemCenter::RemoveItemByIndex)
	// 		.DefObjectFunction("GetItemByIndex", &ItemCenter::GetItemByIndex)
	// 		.DefObjectFunction("CheckItemBlankBagByID", &ItemCenter::CheckItemBlankBagByID)
	// 		.DefObjectFunction("GetUser", &ItemCenter::GetUser)
	// 		.DefObjectFunction("AddBagSize", &ItemCenter::_addBagSize)
	// 		.DefObjectFunction("GetBagSize", &ItemCenter::GetBagSize)
	// 		.DefObjectFunction("GetItemCountByID", &ItemCenter::GetItemCountByID);
}

void HorseCenter::ReleaseScriptClass( CLuaState * pLuaState )
{
	pLuaState->ReleaseScriptClass((HorseCenter*)0);
}

void HorseCenter::Update()
{
	bool ifNeedRefresh = false;
	for(int i = m_pHorseList->Count-1; i >= 0; i --)
	{
		HorseBaseData* pHorse = (HorseBaseData*)m_pHorseList->Items[i];
		if(pHorse->m_ifChange)
		{
			_updateHorse(pHorse);
			SendHorse2C(pHorse);
			pHorse->RecalParam();
			if(pHorse->m_ifEquip)
				ifNeedRefresh = true;
		}
		if(pHorse->m_ifSendExp)
		{
			_updateHorseExp(pHorse);
			SendExpChange(pHorse);
			pHorse->RecalParam();
			if(pHorse->m_ifEquip)
				ifNeedRefresh = true;
		}
		if(pHorse->m_ifSendCul)
		{
			SendCulChange(pHorse);
		}
	}	
	if(m_ifEquipChange)
	{
		m_ifRide = false;
		m_ifEquipChange = false;
		_saveEquip();
		SendEquip();
		ifNeedRefresh = true;
	}
	if(m_ifRideChange)
	{
		m_ifRideChange = false;
		SendRide();
		ifNeedRefresh = true;
		if(m_ifRide)
			m_pParent->AddState(1400,false);
		else
			m_pParent->AddState(1400,true);
	}
	if(ifNeedRefresh)
		m_pParent->RecalcParam();
}

bool HorseCenter::DBInit()
{
	QueryRlt xRlt;

	if(!m_pParent->m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_NEWSLAVE_TABLE" WHERE uin = %d and kind = 1",m_pParent->m_dwUin))  
		return false;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if(pCmd != NULL)
	{
		while(pCmd->FetchRow())
		{
			HorseBaseData* pData = new HorseBaseData(m_pParent);
			pData->m_dwUin	= pCmd->GetInt("uin");
			pData->m_nType	= pCmd->GetInt("baseid");
			pData->m_dwID	= pCmd->GetInt("id");
			pData->llKey	= _atoi64(pCmd->GetStr("objkey"));
			if(pData->llKey == 0)
			{
				LONGLONG temp = pData->m_dwUin;
				pData->llKey = (temp<<32 | pData->m_dwID);
				m_pParent->m_pDB->ExecuteSQL(FALSE,"update "RPG_NEWSLAVE_TABLE" set objkey=%I64d where uin=%d and id=%d",pData->llKey,pData->m_dwUin,pData->m_dwID);
			}
			pData->m_byLevel = pCmd->GetInt("level");
			pData->m_nExp = pCmd->GetInt("exp");
			pData->m_bySlot = pCmd->GetInt("slot");
			pData->m_nEatCulLimt = pCmd->GetInt("eatgainlimit");
			pData->m_pName = strdup(pCmd->GetStr("name"));
			int nSize = 0;
			BYTE*p = pCmd->GetBLOB("data0",&nSize);
			pData->m_pNumData[HORSE_NUM_CULTIVATE]->SetData(p,nSize);
			pData->RecalParam();
			m_szHorseCount[pData->GetQuality()]++;
			m_pHorseList->Add(pData);
		}
	}
	if(m_dwEquipID > 0)
	{
		m_pEquipHorse = GetHorse(m_dwEquipID);
		m_dwEquipID = 0;
		if(m_pEquipHorse)
			m_pEquipHorse->m_ifEquip = true;
	}
	return true;
}

void HorseCenter::SaveAll()
{
	for(int i = 0; i < m_pHorseList->Count;i ++)
	{
		HorseBaseData* pHorse = (HorseBaseData*)m_pHorseList->Items[i];
		_updateHorse(pHorse);
	}
	_saveEquip();
}

bool HorseCenter::AddNewHorse(int nModel)
{
	if(m_pHorseList->Count >= MAX_HORSE_CNT)
		return false;
	HorseBasic* pBase = HorseBaseCfg::GetInstance().GetBasic(nModel);
	if(pBase == NULL)
		return false;
	HorseBaseData* pData = new HorseBaseData(m_pParent);
	pData->m_dwUin = m_pParent->m_dwUin;
	pData->m_nType = nModel;
	pData->m_dwID = m_pParent->m_dwNextItemID;
	m_pParent->m_dwNextItemID ++;
	m_pParent->m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set nextitemid=%d where uin=%d",m_pParent->m_dwNextItemID,m_pParent->m_dwUin);
	LONGLONG temp = pData->m_dwUin;
	pData->llKey = (temp<<32 | pData->m_dwID);
	pData->m_byLevel = 1;
	pData->m_pName = strdup(pBase->pName);
	pData->m_nEatCulLimt = 0;
	_insertHorse(pData);
	pData->m_pNumData[HORSE_NUM_CULTIVATE]->SetVal(HORSE_CULTIVATE_TOTAL,pBase->nPotential);
	pData->m_ifChange = true;
	m_szHorseCount[pBase->byQuality]++;
	m_pParent->m_pUserRecord->AddSlaveLog(pData->m_dwID,pData->m_nType,pData->m_byLevel,0,SLAVE_SAVE_GETHORSE);
	m_pParent->m_pUserRecord->RunLuaHorseFunc(1,pData,0);
	m_pHorseList->Add((void*)pData);
	return true;
}

void HorseCenter::RemoveHorse(DWORD id)
{
	for(int i = 0;i < m_pHorseList->Count;i ++)
	{
		HorseBaseData* pHorse = (HorseBaseData*)m_pHorseList->Items[i];
		if(pHorse->m_dwID == id)
		{
			m_szHorseCount[pHorse->GetQuality()]--;
			m_pParent->m_pUserRecord->AddSlaveLog(pHorse->m_dwID,pHorse->m_nType,pHorse->m_byLevel,0,SLAVE_SAVE_REMOVEHORSE);
			BuildPacketEx(pPacket,JGSLAVE,S_SEND_REMOVEHORSE,buf,1024);
			SET_DATA(pSend,JGSLAVE,S_SEND_REMOVEHORSE,pPacket);
			pSend->id = id;
			TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
			_deleteHorse(pHorse);
			m_pHorseList->Delete(i);
			delete pHorse;
		}
	}
}

HorseBaseData* HorseCenter::GetHorse(DWORD id)
{
	for(int i = 0;i < m_pHorseList->Count;i ++)
	{
		HorseBaseData* pHorse = (HorseBaseData*)m_pHorseList->Items[i];
		if(pHorse->m_dwID == id)
		{
			return pHorse;
		}
	}
	return NULL;
}

void HorseCenter::SendHorse2C(HorseBaseData* pHorse)
{
	BuildPacketEx(pPacket,JGSLAVE,S_SEND_HORSEINFO,buf,1024);
	SET_DATA(pData,JGSLAVE,S_SEND_HORSEINFO,pPacket);
	pData->Info.dwUin	= pHorse->m_dwUin;
	pData->Info.dwID	= pHorse->m_dwID;
	pData->Info.dwExp	= pHorse->m_nExp;
	pData->Info.nType	= pHorse->m_nType;
	pData->Info.byLevel	= pHorse->m_byLevel;
	pData->Info.bySlot	= pHorse->m_bySlot;
	pData->Info.nEatLimt = pHorse->m_nEatCulLimt;
	sprintf(pData->Info.szName,pHorse->m_pName);
	int nSize = 0;
	BYTE p[1024];
	pHorse->m_pNumData[HORSE_NUM_CULTIVATE]->GetData(p,nSize);
	memcpy(pData->Info.nPotential,p,HORSE_CULTIVATE_MAX*4);
	TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
	pHorse->m_ifChange = false;
	pHorse->m_ifSendExp = false;
}

void HorseCenter::SendHorse2C()
{
	BuildPacketEx(pPacket,JGSLAVE,S_SEND_ALLHORSE,buf,2048);
	SET_DATA(pData,JGSLAVE,S_SEND_ALLHORSE,pPacket);
	int n = 0;
	BYTE p[1024];
	int nSize;
	for(int i = 0; i <m_pHorseList->Count; i ++)
	{
		HorseBaseData* pHorse = (HorseBaseData*)m_pHorseList->Items[i];
		pData->szInfo[n].dwUin	= pHorse->m_dwUin;
		pData->szInfo[n].dwID	= pHorse->m_dwID;
		pData->szInfo[n].dwExp	= pHorse->m_nExp;
		pData->szInfo[n].nType	= pHorse->m_nType;
		pData->szInfo[n].byLevel	= pHorse->m_byLevel;
		pData->szInfo[n].bySlot	= pHorse->m_bySlot;
		pData->szInfo[n].nEatLimt = pHorse->m_nEatCulLimt;
		sprintf(pData->szInfo[n].szName,pHorse->m_pName);
		pHorse->m_pNumData[HORSE_NUM_CULTIVATE]->GetData(p,nSize);
		memcpy(pData->szInfo[n].nPotential,p,HORSE_CULTIVATE_MAX*4);
		
		n++;
		if(n >= 20)
		{
			pData->nCnt = n;
			pPacket->wSize += n*sizeof(SlaveSendInfo);
			TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
			pPacket->wSize	= sizeof(JGSLAVE_S_SEND_ALLHORSE_Data);
			n = 0;
		}
	}
	if(n > 0)
	{
		pData->nCnt = n;
		pPacket->wSize += n*sizeof(SlaveSendInfo);
		TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
	}
}

void HorseCenter::SendExpChange(HorseBaseData* pHorse)
{
	BuildPacketEx(pPacket,JGSLAVE,S_SEND_HORSEEXP,buf,1024);
	SET_DATA(pData,JGSLAVE,S_SEND_HORSEEXP,pPacket);
	pData->dwID = pHorse->m_dwID;
	pData->byLevel = pHorse->m_byLevel;
	pData->dwExp = pHorse->m_nExp;
	TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
	pHorse->m_ifSendExp = false;
}

void HorseCenter::SendEquip()
{
	BuildPacketEx(pPacket,JGSLAVE,S_SEND_EQUIP,buf,1024);
	SET_DATA(pData,JGSLAVE,S_SEND_EQUIP,pPacket);
	if(m_pEquipHorse)
		pData->dwID = m_pEquipHorse->m_dwID;
	else
		pData->dwID = 0;
	pData->ifRide = m_ifRide;
	TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
}

void HorseCenter::SendRide()
{
	BuildPacketEx(pPacket,JGSLAVE,S_SEND_RIDE,buf,1024);
	SET_DATA(pData,JGSLAVE,S_SEND_RIDE,pPacket);
	pData->ifRide = m_ifRide;
	TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
}

void		HorseCenter::SendCulChange(HorseBaseData* pHorse)
{
	if(pHorse == NULL)
		return;
	BuildPacketEx(pPacket,JGSLAVE,S_SEND_CULCHANGE,buf,1024);
	SET_DATA(pData,JGSLAVE,S_SEND_CULCHANGE,pPacket);
	pData->dwID = pHorse->m_dwID;
	memcpy(pData->szVal,pHorse->m_szTempCul,4*HORSE_CULTIVATE_MAX);
	TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
	pHorse->m_ifSendCul = false;
}

bool HorseCenter::_insertHorse(HorseBaseData* pHorse)
{
	if(pHorse == NULL)
		return false;
	m_pParent->m_pDB->ExecuteSQL(FALSE,"insert into "RPG_NEWSLAVE_TABLE" (uin,id,baseid,kind,objkey,name,slot,level,exp,hp,mp,data0,data1,data2) values (%d,%d,%d,1,%I64d,'%s',%d,%d,%d,%d,%d,'','','')",
		pHorse->m_dwUin,pHorse->m_dwID,pHorse->m_nType,pHorse->llKey,pHorse->m_pName,pHorse->m_bySlot,pHorse->m_byLevel,pHorse->m_nExp,0,0);
	return true;
}

bool HorseCenter::_updateHorse(HorseBaseData* pHorse)
{
	if(pHorse == NULL)
		return false;
	m_pParent->m_pDB->ExecuteSQL(FALSE,"update "RPG_NEWSLAVE_TABLE" set uin=%d, id=%d, baseid=%d, slot=%d, level=%d, exp=%d,eatgainlimit=%d, name='%s' where uin=%d and id=%d",
		pHorse->m_dwUin,pHorse->m_dwID,pHorse->m_nType,pHorse->m_bySlot,pHorse->m_byLevel,pHorse->m_nExp,pHorse->m_nEatCulLimt,pHorse->m_pName,pHorse->m_dwUin,pHorse->m_dwID);
	BYTE pData[1024];int nSize;
	pHorse->m_pNumData[HORSE_NUM_CULTIVATE]->GetData(pData,nSize);
	m_pParent->m_pDB->SetBLOB(false,pData,nSize,"update "RPG_NEWSLAVE_TABLE" set data0=? where uin=%d and id=%d",pHorse->m_dwUin,pHorse->m_dwID);
	return true;
}

bool HorseCenter::_deleteHorse(HorseBaseData* pHorse)
{
	if(pHorse == NULL)
		return false;
	m_pParent->m_pDB->ExecuteSQL(FALSE,"delete from "RPG_NEWSLAVE_TABLE" where uin=%d and id=%d",pHorse->m_dwUin,pHorse->m_dwID);
	return true;
}

bool HorseCenter::_updateHorseExp(HorseBaseData* pHorse)
{
	if(pHorse == NULL)
		return false;
	m_pParent->m_pDB->ExecuteSQL(FALSE,"update "RPG_NEWSLAVE_TABLE" set level=%d, exp=%d where uin=%d and id=%d",pHorse->m_byLevel,pHorse->m_nExp,pHorse->m_dwUin,pHorse->m_dwID);
	return true;
}

bool HorseCenter::_saveEquip()
{
	int nID = 0;
	if(m_pEquipHorse)
		nID = m_pEquipHorse->m_dwID;
	m_pParent->m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set equiphorse=%d where uin=%d",nID,m_pParent->m_dwUin);
	return true;
}