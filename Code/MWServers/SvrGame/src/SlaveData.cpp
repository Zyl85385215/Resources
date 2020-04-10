#include "stdafx.h"
#include "SlaveData.h"
#include "DBControl.h"
#include "UserData.h"
#include "ios_yjgslave_packet.h"
#include "MapSObject.h"
#include "ios_mapobj_packet.h"
#include "FsGameCenter.h"


SlaveBaseData::SlaveBaseData(UserData* pUser)
{
	memset(m_nParam,0,4*SLAVE_PARAM_MAX);
	m_pNumData[SLAVE_NUM_SKILL] = new NumberData(SLAVE_SKILL_MAX);
	m_pNumData[SLAVE_NUM_STRENGTHEN] = new NumberData(SLAVE_STRENGTHEN_MAX);
	m_pNumData[SLAVE_NUM_SKILLLV] = new NumberData(SLAVE_SKILL_MAX);
	m_dwUin = 0;
	m_dwID = 0;
	m_bySlot = 0;
	m_byLevel = 0;
	m_nExp = 0;
	m_byVariatLv = 0;
	m_ifChange = false;
	m_nSaveExp = 0;
	m_nSendExp = 0;
	m_ifLevelUp = false;
	m_ifActive = false;
	m_ifHgChg = false;
	m_dwMapID = 0;
	m_nLifePoint = 250;
	m_pMaster = pUser;
	CLuaClassObject<SlaveBaseData>::Init(this);
}

SlaveBaseData::~SlaveBaseData()
{
	for(int i =0; i < SLAVE_NUM_MAX; i ++)
	{
		if( m_pNumData[i])
			delete m_pNumData[i];
	}
}

void SlaveBaseData::CreateScriptClass( CLuaState * pLuaState )
{
	CLuaClass<SlaveBaseData> * pscSlaveData = pLuaState->CreateScriptClass((SlaveBaseData*)0);

	 	pscSlaveData->DefObjectFunction("GetLevel", &SlaveBaseData::GetLevel)
	 		.DefObjectFunction("GetBaseID", &SlaveBaseData::GetBaseID)
			.DefObjectFunction("GetStrengthLevel", &SlaveBaseData::GetStrengthLevel)
			.DefObjectFunction("GetVariatlyLevel", &SlaveBaseData::GetVariatlyLevel)
			.DefObjectFunction("GetQuality", &SlaveBaseData::GetQuality)
	 		.DefObjectFunction("GetSkillCount", &SlaveBaseData::GetSkillCount);
}

void SlaveBaseData::ReleaseScriptClass( CLuaState * pLuaState )
{
	pLuaState->ReleaseScriptClass((SlaveBaseData*)0);
}

int		SlaveBaseData::GetTotalStrengthLevel()
{
	int nRlt = 0;
	for(int i = 0; i < SLAVE_STRENGTHEN_MAX; i ++)
		nRlt +=m_pNumData[SLAVE_NUM_STRENGTHEN]->GetVal(i);
	return nRlt;
}

LUAFUN	int		SlaveBaseData::GetLevel()
{
	return m_byLevel;
}

LUAFUN	int		SlaveBaseData::GetBaseID()
{
	return m_nType;
}

LUAFUN	int		SlaveBaseData::GetQuality()
{
	SlaveBasic* pBasic = SlaveBaseCfg::GetInstance().GetBaseInfo(m_nType);
	if(pBasic)
		return pBasic->byQuality;
	return 0;
}

LUAFUN	int		SlaveBaseData::GetStrengthLevel(int nPos)
{
	if(nPos >= SLAVE_STRENGTHEN_MAX)
		return 0;
	return m_pNumData[SLAVE_NUM_STRENGTHEN]->GetVal(nPos);
}

LUAFUN	int		SlaveBaseData::GetVariatlyLevel()
{
	return m_byVariatLv;
}

LUAFUN	int		SlaveBaseData::GetSkillCount()
{
	int nCount = 0;
	for(int i = 0; i < SLAVE_SKILL_MAX; i ++)
	{
		if(m_pNumData[SLAVE_NUM_SKILL]->GetVal(i) > 0)
			nCount ++;
	}
	return nCount;
}

void SlaveBaseData::AddExp(int nExp)
{
	if(m_nHP <= 0)
		return;
	SlaveBasic* pBasic = SlaveBaseCfg::GetInstance().GetBaseInfo(m_nType);
	if(pBasic == NULL)
		return;
	BYTE byNowLv = m_byLevel;
	int nNowExp = m_nExp;
	int nTotalExp = nExp;
	int nCount = 0;
	while(nTotalExp > 0)
	{
		if(nCount > 100)
			break;
		if(byNowLv >= pBasic->byLvLimit || byNowLv >= m_pMaster->m_xUWealth.GetWealth(UW_LV) + 5)
		{
			nTotalExp = 0;
			break;
		}
		int nLimit = SlaveBaseCfg::GetInstance().GetLvExp(byNowLv);
		if(nNowExp + nTotalExp >= nLimit)
		{
			nTotalExp = nNowExp + nTotalExp - nLimit;
			nNowExp = 0;
			byNowLv ++;
			m_ifLevelUp = true;
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
		m_nSendExp += nExp;
		m_nSaveExp += nExp;
	}
	if(m_ifLevelUp)
		m_pMaster->m_pUserRecord->AddSlaveLog(m_dwID,m_nType,m_byLevel,m_byVariatLv,SLAVE_SAVE_PET_LVUP);
}

void SlaveBaseData::ChangeHp(int nVal)
{
	if(m_nHP <= 0)
		return;
	if(nVal == 0)
		return;
	if(m_nHP + nVal < 0)
		m_nHP = 0;
	else if(m_nHP + nVal > m_nParam[SLAVE_PARAM_HPMAX])
		m_nHP = m_nParam[SLAVE_PARAM_HPMAX];
	else
		m_nHP += nVal;
	m_ifChange = true;
}

void SlaveBaseData::SetHp(int nVal)
{
	if(m_nHP == nVal)
		return;
	m_nHP = nVal;
	if(m_nHP < 0)
		m_nHP = 0;
	else if(m_nHP > m_nParam[SLAVE_PARAM_HPMAX])
		m_nHP = m_nParam[SLAVE_PARAM_HPMAX];
	m_ifHgChg = true;
}

void SlaveBaseData::ChangeMp(int nVal)
{
	if(m_nHP <= 0)
		return;
	if(nVal == 0)
		return;
	if(m_nMp + nVal < 0)
		m_nMp = 0;
	else if(m_nMp + nVal > m_nParam[SLAVE_PARAM_MPMAX])
		m_nMp = m_nParam[SLAVE_PARAM_MPMAX];
	else
		m_nMp += nVal;
}

void SlaveBaseData::RecalParam()
{
	memset(m_nParam,0,4*SLAVE_PARAM_MAX);
	SlaveBasic* pBasic = SlaveBaseCfg::GetInstance().GetBaseInfo(m_nType);
	if(pBasic != NULL)
	{
		for(int i = 0; i < SLAVE_PARAM_MAX; i ++)
		{
			m_nParam[i] += pBasic->szBaseParam[i];
			m_nParam[i] += (m_byLevel-1)*pBasic->szBaseUp[i];
			SlaveStreBasic* pStrBasic = SlaveStrengthenCfg::GetInstance().GetBase(i,m_pNumData[SLAVE_NUM_STRENGTHEN]->GetVal(i));
			if(pStrBasic)
				m_nParam[i] = m_nParam[i]*(100+pStrBasic->nVal)/100;
		}
	}
}

SlaveCenter::SlaveCenter(UserData* pUser)
{
	m_pParent = pUser;
	m_byUnlockCnt = 0;
	m_pSlaveList = xnList::Create();
	m_pActiveSlave = NULL;
	m_pLastActiveSlave = NULL;
	ticktime = 0;
	m_dwActiveID = 0;
	m_ifActiveChg = false;
	m_dwReviveTick = 0;
	memset(m_szPetCount,0,20);
	CLuaClassObject<SlaveCenter>::Init(this);
}

SlaveCenter::~SlaveCenter()
{
	if(m_pSlaveList)
	{
		while(m_pSlaveList->Count)
			delete (SlaveBaseData*)m_pSlaveList->Delete(0);
		m_pSlaveList->Free();
	}
}

void SlaveCenter::CreateScriptClass( CLuaState * pLuaState )
{
	CLuaClass<SlaveCenter> * pscSlaveCenter = pLuaState->CreateScriptClass((SlaveCenter*)0);

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

void SlaveCenter::ReleaseScriptClass( CLuaState * pLuaState )
{
	pLuaState->ReleaseScriptClass((SlaveCenter*)0);
}

bool	SlaveCenter::CheckActiveState()
{
	if(m_pParent->m_nRealMIndex == 27 || m_pParent->m_nRealMIndex == 66)
		return true;
	return false;
}

void	SlaveCenter::CallBackSlave()
{
	if(m_pActiveSlave)
	{
		m_pActiveSlave->m_ifActive = false;
		m_pLastActiveSlave = m_pActiveSlave;
		m_pActiveSlave = NULL;
		LeaveMap();
		SendActive();
	}
}

void SlaveCenter::Update()
{
	//static DWORD ticktime = 0;
	bool ifNeedRefresh = false;
	if(m_dwReviveTick > 0 && m_dwReviveTick<= GetTickCount() && m_pParent->m_pMObjCenter->m_pPlayer->xParam.nHp >0)
	{
		m_dwReviveTick = 0;
		if(m_pActiveSlave && m_pActiveSlave->m_nHP <= 0)
		{
			m_pActiveSlave->SetHp(m_pActiveSlave->m_nParam[SLAVE_PARAM_HPMAX]);
			SetMapObj();
		}
	}
 	if(CheckActiveState())
 		CallBackSlave();
	for(int i = m_pSlaveList->Count-1; i >= 0; i --)
	{
		SlaveBaseData* pData = (SlaveBaseData*)m_pSlaveList->Items[i];
		if(pData->m_ifChange)
		{
			pData->m_ifChange = false;
			pData->m_ifHgChg = false;
			pData->RecalParam();
			_updateSlave(pData);
			SendSlave2C(pData);
			if(pData == m_pActiveSlave)
			{
				UpdateMapObj();
				ifNeedRefresh = true;
			}
		}
		else if(pData->m_ifHgChg)
		{
			pData->m_ifHgChg = false;
			pData->RecalParam();
			SendSlave2C(pData);
			if(pData == m_pActiveSlave)
			{
				UpdateMapObj();
				ifNeedRefresh = true;
			}
		}
		else
		{
			if(ticktime % 25 == 0 && pData->m_nSendExp > 0)
				SendExpChange(pData);
			if(pData->m_ifLevelUp)
			{
				pData->RecalParam();
				pData->ChangeHp(pData->m_nParam[SLAVE_PARAM_HPMAX]);
				pData->ChangeMp(pData->m_nParam[SLAVE_PARAM_MPMAX]);
				_updateSlaveExp(pData);
				if(pData == m_pActiveSlave)
				{
					UpdateMapObj();
					ifNeedRefresh = true;
				}
			}
			else if(pData->m_nSaveExp > (pData->m_byLevel*100))
			{
				_updateSlaveExp(pData);
			}
		}
	}
	if(m_ifActiveChg)
	{
		m_ifActiveChg = false;
		ifNeedRefresh = true;
	}
	if(ifNeedRefresh)
		m_pParent->RecalcParam();
	ticktime ++;
}

bool SlaveCenter::AddNewSlave(int nModel)
{	
	if(m_pSlaveList->Count >= MIN_SLAVE_CNT + m_byUnlockCnt)
	{
		m_pParent->SendPopMsg("宠物数量已达到上限");
		return false;
	}
	SlaveBasic* pBase = SlaveBaseCfg::GetInstance().GetBaseInfo(nModel);
	if(pBase == NULL)
		return false;
	SlaveBaseData* pData = new SlaveBaseData(m_pParent);
	pData->m_dwUin = m_pParent->m_dwUin;
	pData->m_nType = nModel;
	pData->m_dwID = m_pParent->m_dwNextItemID;
	m_pParent->m_dwNextItemID ++;
	m_pParent->m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set nextitemid=%d where uin=%d",m_pParent->m_dwNextItemID,m_pParent->m_dwUin);
	LONGLONG temp = pData->m_dwUin;
	pData->llKey = (temp<<32 | pData->m_dwID);
	pData->m_byLevel = 1;
	pData->m_pName = strdup(pBase->pName);
	_insertSlave(pData);
	if(pBase->nSkill > 0)
	{
		pData->m_pNumData[SLAVE_NUM_SKILL]->SetVal(SLAVE_SKILL_ID1,pBase->nSkill);
		pData->m_pNumData[SLAVE_NUM_SKILLLV]->SetVal(SLAVE_SKILL_ID1,1);
	}
	pData->RecalParam();
	pData->m_nHP = pData->m_nParam[SLAVE_PARAM_HPMAX];
	pData->m_nMp = pData->m_nParam[SLAVE_PARAM_MPMAX];
	pData->m_ifChange = true;
	m_pSlaveList->Add((void*)pData);
	m_szPetCount[pBase->byQuality]++;
	m_pParent->m_pUserRecord->AddRecord(UA_GAIN,UO_PET,nModel,1);
	m_pParent->m_pUserRecord->RunLuaPetFunc(1,pData);
	m_pParent->m_pUserRecord->AddSlaveLog(pData->m_dwID,pData->m_nType,pData->m_byLevel,pData->m_byVariatLv,SLAVE_SAVE_GETPET);
	return true;
}

void SlaveCenter::RemoveSlave(DWORD id)
{
	for(int i = 0; i < m_pSlaveList->Count;i ++)
	{
		SlaveBaseData* pData = (SlaveBaseData*)m_pSlaveList->Items[i];
		if(pData->m_dwID == id)
		{
			m_szPetCount[pData->GetQuality()] --;
			m_pParent->m_pUserRecord->AddSlaveLog(pData->m_dwID,pData->m_nType,pData->m_byLevel,pData->m_byVariatLv,SLAVE_SAVE_REMOVEPET);
			BuildPacketEx(pPacket,JGSLAVE,S_SEND_REMOVE,buf,1024);
			SET_DATA(pSend,JGSLAVE,S_SEND_REMOVE,pPacket);
			pSend->id = id;
			TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
			_deleteSlave(pData);
			m_pSlaveList->Delete(i);
			delete pData;
		}
	}
}

SlaveBaseData* SlaveCenter::GetSlave(int nID)
{
	for(int i = 0; i < m_pSlaveList->Count; i ++)
	{
		SlaveBaseData* pData = (SlaveBaseData*)m_pSlaveList->Items[i];
		if(pData->m_dwID == nID)
			return pData;
	}
	return NULL;
}

void	SlaveCenter::LeaveMap()
{
	if(m_pParent->m_pMObjCenter->m_pSlave)
	{
		m_pParent->m_pMObjCenter->ObjLeaveMap(m_pParent->m_pMObjCenter->m_pSlave);
		delete m_pParent->m_pMObjCenter->m_pSlave;
		m_pParent->m_pMObjCenter->m_pSlave = NULL;
	}
}

void SlaveCenter::SetMapObj()
{
// 	if(m_pParent->m_pMObjCenter->m_pSlave)
// 	{
// 		m_pParent->m_pMObjCenter->ObjLeaveMap(m_pParent->m_pMObjCenter->m_pSlave);
// 		delete m_pParent->m_pMObjCenter->m_pSlave;
// 		m_pParent->m_pMObjCenter->m_pSlave = NULL;
// 	}
	if(m_pActiveSlave == NULL)
		return;
	if(m_pParent->m_pMObjCenter->m_pSlave && m_pParent->m_pMObjCenter->m_pSlave->dwID != m_pActiveSlave->m_dwMapID)
		LeaveMap();
	m_pActiveSlave->RecalParam();
	m_pParent->m_pMObjCenter->m_pSlave = new MapSObject();
	MapSObject* pObj = m_pParent->m_pMObjCenter->m_pSlave;
	if(m_pActiveSlave->m_dwMapID == 0)
	{
		m_pActiveSlave->m_dwMapID	= m_pParent->m_dwUin + 100000000;
	}
	pObj->dwID = m_pActiveSlave->m_dwMapID;
	pObj->nPosX = m_pParent->m_pMObjCenter->m_pPlayer->nPosX;
	pObj->nPosY = m_pParent->m_pMObjCenter->m_pPlayer->nPosY;
	pObj->byType = MAPOBJ_SLAVE;
	sprintf(pObj->szName,m_pActiveSlave->m_pName);
	//宠物强化总级数达到 150  300   500   800  1200分别添加光效276、277、278、279、280
	pObj->nMarryEff = 0;
	int nTotalLv = m_pActiveSlave->GetTotalStrengthLevel();
	if(nTotalLv >= 1200)
		pObj->nMarryEff = 280;
	else if(nTotalLv >= 800)
		pObj->nMarryEff = 279;
	else if(nTotalLv >= 500)
		pObj->nMarryEff = 278;
	else if(nTotalLv >= 300)
		pObj->nMarryEff = 277;
	else if(nTotalLv >= 150)
		pObj->nMarryEff = 276;


	//sprintf(pObj->szTitle,"%s的宠物",m_pParent->m_szNick);
	pObj->xParam.nMSpeed	= 180;

	pObj->byLv				= m_pActiveSlave->m_byLevel;
	pObj->xParam.nHp		= m_pActiveSlave->m_nHP;
	pObj->xParam.nHpMax		= m_pActiveSlave->m_nParam[SLAVE_PARAM_HPMAX];	
	pObj->xParam.nMp		= m_pActiveSlave->m_nMp;
	pObj->xParam.nMpMax		= m_pActiveSlave->m_nParam[SLAVE_PARAM_MPMAX];
	pObj->xParam.nAtkW		= m_pActiveSlave->m_nParam[SLAVE_PARAM_AD];
	pObj->xParam.nAtkN		= m_pActiveSlave->m_nParam[SLAVE_PARAM_AP];
	pObj->xParam.nDefW		= m_pActiveSlave->m_nParam[SLAVE_PARAM_DD];
	pObj->xParam.nDefN		= m_pActiveSlave->m_nParam[SLAVE_PARAM_DP];

	pObj->xParam.nAim		= 100;
	pObj->xParam.nDodge		= 100;
	pObj->xParam.nCrit		= 100;
	pObj->xParam.nPvpDef	= (95+pObj->byLv*5)/2;

	SlaveBasic* pBasic = SlaveBaseCfg::GetInstance().GetBaseInfo(m_pActiveSlave->m_nType);
	if(pBasic == NULL)
	{
		pObj->wSkin[0] = 5001;
		pObj->byJob = 5;
	}
	else
	{
		pObj->wSkin[0] = pBasic->nSkin;
		pObj->byJob = pBasic->nAtkDis;
	}

	for(int i = 0; i < 6; i ++)
	{
		pObj->SetSkill(m_pActiveSlave->m_pNumData[SLAVE_NUM_SKILL]->GetVal(i),m_pActiveSlave->m_pNumData[SLAVE_NUM_SKILLLV]->GetVal(i),0,0);
	}
	m_pParent->m_pMObjCenter->ObjEnterMap(pObj);
	m_pParent->m_pMObjCenter->SetCtrl(pObj,m_pParent->m_bySlaveAct);
}

void SlaveCenter::UpdateMapObj()
{
	MapSObject* pObj = m_pParent->m_pMObjCenter->m_pSlave;
	if(m_pActiveSlave == NULL	|| pObj == NULL)
		return;
	sprintf(pObj->szName,m_pActiveSlave->m_pName);

	pObj->xParam.nHp		= m_pActiveSlave->m_nHP;
	pObj->xParam.nHpMax		= m_pActiveSlave->m_nParam[SLAVE_PARAM_HPMAX];	
	pObj->xParam.nMp		= m_pActiveSlave->m_nMp;
	pObj->xParam.nMpMax		= m_pActiveSlave->m_nParam[SLAVE_PARAM_MPMAX];
	pObj->xParam.nAtkW		= m_pActiveSlave->m_nParam[SLAVE_PARAM_AD];
	pObj->xParam.nAtkN		= m_pActiveSlave->m_nParam[SLAVE_PARAM_AP];
	pObj->xParam.nDefW		= m_pActiveSlave->m_nParam[SLAVE_PARAM_DD];
	pObj->xParam.nDefN		= m_pActiveSlave->m_nParam[SLAVE_PARAM_DP];
	pObj->byLv				= m_pActiveSlave->m_byLevel;
	pObj->xParam.nPvpDef	= (95+pObj->byLv*5)/2;

	SlaveBasic* pBasic = SlaveBaseCfg::GetInstance().GetBaseInfo(m_pActiveSlave->m_nType);
	if(pBasic)
		pObj->wSkin[0] = pBasic->nSkin;
	else
		pObj->wSkin[0] = 5001;
	for(int i = 0; i < 6; i ++)
	{
		pObj->SetSkill(m_pActiveSlave->m_pNumData[SLAVE_NUM_SKILL]->GetVal(i),m_pActiveSlave->m_pNumData[SLAVE_NUM_SKILLLV]->GetVal(i),0,0);
	}
	pObj->bParamChged = true;
}

void SlaveCenter::CheckActiveWhenJoin()
{
	if(m_dwActiveID == 0)
		return;
	SlaveBaseData* pData = GetSlave(m_dwActiveID);
	m_dwActiveID = 0;
	if(m_pActiveSlave)
		m_pActiveSlave->m_ifActive = false;
	m_pActiveSlave = pData;
	m_pActiveSlave->m_ifActive = true;
	SetMapObj();
	SendActive();
}

void	SlaveCenter::AddExp(int nExp)
{
	if(m_pActiveSlave)
	{
		m_pActiveSlave->AddExp(nExp);
	}
}

void SlaveCenter::SendExpChange(SlaveBaseData* pSlave)
{
	BuildPacketEx(pPacket,JGSLAVE,S_SEND_EXP,buf,1024);
	SET_DATA(pData,JGSLAVE,S_SEND_EXP,pPacket);
	pData->dwID = pSlave->m_dwID;
	pData->byLevel = pSlave->m_byLevel;
	pData->dwExp = pSlave->m_nExp;
	TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
	pSlave->m_nSendExp = 0;
}

void SlaveCenter::SendActive()
{
	m_ifActiveChg = true;
	_saveActive();
	BuildPacketEx(pPacket,JGSLAVE,S_SEND_ACTIVE,buf,1024);
	SET_DATA(pData,JGSLAVE,S_SEND_ACTIVE,pPacket);
	if(m_pActiveSlave)
	{
		pData->dwID = m_pActiveSlave->m_dwID;
		pData->dwMapID = m_pActiveSlave->m_dwMapID;
	}
	else
	{
		pData->dwID = 0;
		pData->dwMapID = 0;
	}
	TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
}

void SlaveCenter::SendGet(int nID)
{
	BuildPacketEx(pPacket,JGSLAVE,S_REQUEST_GET,buf,1024);
	SET_DATA(pData,JGSLAVE,S_REQUEST_GET,pPacket);
	pData->nSID = nID;
	TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
}

void SlaveCenter::SendQuickGet(int* nID)
{
	BuildPacketEx(pPacket,JGSLAVE,S_REQUEST_QUICKGET,buf,1024);
	SET_DATA(pData,JGSLAVE,S_REQUEST_QUICKGET,pPacket);
	for (int i = 0; i < 10; i++)
	{
		pData->nSID[i] = nID[i];
	}
	TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
}

void SlaveCenter::SendSlave2C(SlaveBaseData* pSlave)
{
	BuildPacketEx(pPacket,JGSLAVE,S_SEND_INFO,buf,1024);
	SET_DATA(pData,JGSLAVE,S_SEND_INFO,pPacket);
	pData->Info.dwUin	= pSlave->m_dwUin;
	pData->Info.dwID	= pSlave->m_dwID;
	pData->Info.dwExp	= pSlave->m_nExp;
	pData->Info.nType	= pSlave->m_nType;
	pData->Info.byLevel	= pSlave->m_byLevel;
	pData->Info.bySlot	= pSlave->m_bySlot;
	pData->Info.byVariatlv= pSlave->m_byVariatLv;
	pData->Info.nHp		= pSlave->m_nHP;
	pData->Info.nLifePoint = pSlave->m_nLifePoint;
	sprintf(pData->Info.szName,pSlave->m_pName);
	int nSize = 0;
	BYTE p[1024];
	pSlave->m_pNumData[SLAVE_NUM_STRENGTHEN]->GetData(p,nSize);
	memcpy(pData->Info.nStrengthen,p,SLAVE_STRENGTHEN_MAX*4);
	for (int i = 0; i < SLAVE_SKILL_MAX; i++)
	{
		pData->Info.wSkill[i]	= pSlave->m_pNumData[SLAVE_NUM_SKILL]->GetVal(i);
		pData->Info.bySkillLv[i]	= pSlave->m_pNumData[SLAVE_NUM_SKILLLV]->GetVal(i);
	}
	TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
	pSlave->m_nSendExp = 0;
}

void SlaveCenter::SendSlave2C()
{
	BuildPacketEx(pPacket,JGSLAVE,S_SEND_ALL,buf,4096);
	SET_DATA(pData,JGSLAVE,S_SEND_ALL,pPacket);
	int n = 0;
	BYTE p[1024];
	int nSize;
	for(int i = 0; i < m_pSlaveList->Count; i ++)
	{
		SlaveBaseData* pSlave = (SlaveBaseData*)m_pSlaveList->Items[i];
		pData->szInfo[n].dwUin	= pSlave->m_dwUin;
		pData->szInfo[n].dwID	= pSlave->m_dwID;
		pData->szInfo[n].dwExp	= pSlave->m_nExp;
		pData->szInfo[n].nType	= pSlave->m_nType;
		pData->szInfo[n].byLevel	= pSlave->m_byLevel;
		pData->szInfo[n].bySlot	= pSlave->m_bySlot;
		pData->szInfo[n].byVariatlv	= pSlave->m_byVariatLv;
		pData->szInfo[n].nHp	= pSlave->m_nHP;
		pData->szInfo[n].nLifePoint = pSlave->m_nLifePoint;
		sprintf(pData->szInfo[n].szName,pSlave->m_pName);

		pSlave->m_pNumData[SLAVE_NUM_STRENGTHEN]->GetData(p,nSize);
		memcpy(pData->szInfo[n].nStrengthen,p,SLAVE_STRENGTHEN_MAX*4);

		for (int s = 0; s < SLAVE_SKILL_MAX; s++)
		{
			pData->szInfo[n].wSkill[s]	= pSlave->m_pNumData[SLAVE_NUM_SKILL]->GetVal(s);
			pData->szInfo[n].bySkillLv[s]	= pSlave->m_pNumData[SLAVE_NUM_SKILLLV]->GetVal(s);
		}
		pSlave->m_nSendExp = 0;
		n++;
		if(n >= 20)
		{
			pData->nCnt = n;
			pPacket->wSize += n*sizeof(SlaveSendInfo);
			TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
			pPacket->wSize	= sizeof(JGSLAVE_S_SEND_ALL_Data);
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

bool SlaveCenter::DBInit()
{
	QueryRlt xRlt;

	if(!m_pParent->m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_NEWSLAVE_TABLE" WHERE uin = %d and kind = 0",m_pParent->m_dwUin))  
		return false;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if(pCmd != NULL)
	{
		while(pCmd->FetchRow())
		{
			SlaveBaseData* pData = new SlaveBaseData(m_pParent);
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
			pData->m_byVariatLv = pCmd->GetInt("variatlv");
			pData->m_pName = strdup(pCmd->GetStr("name"));
			pData->m_nHP = pCmd->GetInt("hp");
			pData->m_nMp = pCmd->GetInt("mp");
			pData->m_nLifePoint = pCmd->GetInt("lifepoint");
			int nSize = 0;
			BYTE*p = pCmd->GetBLOB("data0",&nSize);
			pData->m_pNumData[SLAVE_NUM_SKILL]->SetData(p,nSize);
			p = pCmd->GetBLOB("data1",&nSize);
			pData->m_pNumData[SLAVE_NUM_STRENGTHEN]->SetData(p,nSize);
			p = pCmd->GetBLOB("data2",&nSize);
			pData->m_pNumData[SLAVE_NUM_SKILLLV]->SetData(p,nSize);
			pData->RecalParam();
			m_szPetCount[pData->GetQuality()]++;
			if(pData->m_nHP <= 0 || pData->m_nHP > pData->m_nParam[SLAVE_PARAM_HPMAX])
				pData->m_nHP = pData->m_nParam[SLAVE_PARAM_HPMAX];
			m_pSlaveList->Add(pData);
		}
	}
	return true;
}

void SlaveCenter::SaveAll()
{
	for(int i = m_pSlaveList->Count - 1; i >= 0;i --)
	{
		SlaveBaseData* pData = (SlaveBaseData*)m_pSlaveList->Items[i];
		if(pData->m_nSaveExp > 0 || pData->m_ifChange || pData->m_ifHgChg || pData->m_ifLevelUp)
			_updateSlave(pData);
	}
}

bool SlaveCenter::_insertSlave(SlaveBaseData* pSlave)
{
	if(pSlave == NULL)
		return false;
	m_pParent->m_pDB->ExecuteSQL(FALSE,"insert into "RPG_NEWSLAVE_TABLE" (uin,id,baseid,kind,objkey,name,slot,level,exp,hp,mp,data0,data1,data2) values (%d,%d,%d,0,%I64d,'%s',%d,%d,%d,%d,%d,'','','')",
		pSlave->m_dwUin,pSlave->m_dwID,pSlave->m_nType,pSlave->llKey,pSlave->m_pName,pSlave->m_bySlot,pSlave->m_byLevel,pSlave->m_nExp,pSlave->m_nHP,pSlave->m_nMp);
	return true;
}

bool SlaveCenter::_updateSlave(SlaveBaseData* pSlave)
{
	if(pSlave == NULL)
		return false;
	m_pParent->m_pDB->ExecuteSQL(FALSE,"update "RPG_NEWSLAVE_TABLE" set uin=%d, id=%d, baseid=%d, slot=%d, level=%d, exp=%d, hp=%d, mp=%d, name='%s',variatlv=%d,lifepoint=%d where uin=%d and id=%d",
		pSlave->m_dwUin,pSlave->m_dwID,pSlave->m_nType,pSlave->m_bySlot,pSlave->m_byLevel,pSlave->m_nExp,pSlave->m_nHP,pSlave->m_nMp,pSlave->m_pName,pSlave->m_byVariatLv,pSlave->m_nLifePoint,pSlave->m_dwUin,pSlave->m_dwID);
	BYTE pData[1024];int nSize;
	pSlave->m_pNumData[SLAVE_NUM_SKILL]->GetData(pData,nSize);
	m_pParent->m_pDB->SetBLOB(false,pData,nSize,"update "RPG_NEWSLAVE_TABLE" set data0=? where uin=%d and id=%d",pSlave->m_dwUin,pSlave->m_dwID);
	pSlave->m_pNumData[SLAVE_NUM_STRENGTHEN]->GetData(pData,nSize);
	m_pParent->m_pDB->SetBLOB(false,pData,nSize,"update "RPG_NEWSLAVE_TABLE" set data1=? where uin=%d and id=%d",pSlave->m_dwUin,pSlave->m_dwID);
	pSlave->m_nSaveExp = 0;
	pSlave->m_pNumData[SLAVE_NUM_SKILLLV]->GetData(pData,nSize);
	m_pParent->m_pDB->SetBLOB(false,pData,nSize,"update "RPG_NEWSLAVE_TABLE" set data2=? where uin=%d and id=%d",pSlave->m_dwUin,pSlave->m_dwID);
	pSlave->m_nSaveExp = 0;
	pSlave->m_ifLevelUp = false;
	return true;
}

bool SlaveCenter::_updateSlaveExp(SlaveBaseData* pSlave)
{
	if(pSlave == NULL)
		return false;
	m_pParent->m_pDB->ExecuteSQL(FALSE,"update "RPG_NEWSLAVE_TABLE" set level=%d, exp=%d where uin=%d and id=%d",pSlave->m_byLevel,pSlave->m_nExp,pSlave->m_dwUin,pSlave->m_dwID);
	pSlave->m_nSaveExp = 0;
	pSlave->m_ifLevelUp = false;
	return true;
}

bool SlaveCenter::_updateSlaveHpMp(SlaveBaseData* pSlave)
{
	if(pSlave == NULL)
		return false;
	m_pParent->m_pDB->ExecuteSQL(FALSE,"update "RPG_NEWSLAVE_TABLE" set hp=%d, mp=%d where uin=%d and id=%d",pSlave->m_nHP,pSlave->m_nMp,pSlave->m_dwUin,pSlave->m_dwID);
	return true;
}

bool SlaveCenter::_deleteSlave(SlaveBaseData* pSlave)
{
	if(pSlave == NULL)
		return false;
	m_pParent->m_pDB->ExecuteSQL(FALSE,"delete from "RPG_NEWSLAVE_TABLE" where uin=%d and id=%d",pSlave->m_dwUin,pSlave->m_dwID);
	return true;
}

bool SlaveCenter::_saveActive()
{
	int nID = 0;
	if(m_pActiveSlave)
		nID = m_pActiveSlave->m_dwID;
	m_pParent->m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set activepet=%d where uin=%d",nID,m_pParent->m_dwUin);
	return true;
}