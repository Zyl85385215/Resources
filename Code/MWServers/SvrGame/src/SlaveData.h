
#pragma once

#include <Windows.h>
#include "xnlist.h"
#include "SlaveConfig.h"
#include "NumberData.h"
#include "LuaStateManager.h"
#include "LuaClassObject.h"


class UserData;

class SlaveBaseData:public CLuaClassObject<SlaveBaseData> 
{
public:
	SlaveBaseData(UserData* pUser);
	~SlaveBaseData();
	static void			CreateScriptClass(CLuaState * pLuaState);
	static void			ReleaseScriptClass(CLuaState * pLuaState);
	void	RecalParam();
	void	AddExp(int nExp);
	void	ChangeHp(int nVal);
	void	SetHp(int nVal);
	void	ChangeMp(int nVal);
	int		GetTotalStrengthLevel();

	LUAFUN	int		GetLevel();
	LUAFUN	int		GetBaseID();
	LUAFUN	int		GetStrengthLevel(int nPos);
	LUAFUN	int		GetVariatlyLevel();
	LUAFUN	int		GetSkillCount();
	LUAFUN	int		GetQuality();

	NumberData* m_pNumData[SLAVE_NUM_MAX];
	int			m_nParam[SLAVE_PARAM_MAX];
	DWORD		m_dwUin;
	DWORD		m_dwID;
	LONGLONG	llKey;
	char*		m_pName;
	int			m_nType;
	BYTE		m_bySlot;
	BYTE		m_byLevel;
	BYTE		m_byVariatLv;			//变异等级
	DWORD		m_nExp;
	int			m_nHP;
	int			m_nMp;
	int			m_nLifePoint;			//宠物寿命,基础250，死亡随机掉1~5点
	bool		m_ifChange;
	int			m_nSaveExp;
	int			m_nSendExp;
	bool		m_ifLevelUp;
	bool		m_ifActive;
	bool		m_ifHgChg;
	DWORD		m_dwMapID;		//传给地图服务器的ID
	UserData*	m_pMaster;
};


class SlaveCenter:public CLuaClassObject<SlaveCenter> 
{
public:
	SlaveCenter(UserData* pUser);
	~SlaveCenter();
	static void			CreateScriptClass(CLuaState * pLuaState);
	static void			ReleaseScriptClass(CLuaState * pLuaState);
//操作
	bool		AddNewSlave(int nModel);
	void		RemoveSlave(DWORD id);
	SlaveBaseData* GetSlave(int nID);
	void		SetMapObj();
	void		LeaveMap();
	void		UpdateMapObj();
	void		CheckActiveWhenJoin();

	void		AddExp(int nExp);
//发包
	void		SendSlave2C();
	void		SendSlave2C(SlaveBaseData* pSlave);
	void		SendExpChange(SlaveBaseData* pSlave);
	void		SendActive();
	void		SendGet(int nID);
	void		SendQuickGet(int* nID);
//管理
	void		Update();
	bool		DBInit();
	void		SaveAll();
	bool		CheckActiveState();
	void		CallBackSlave();

	xnList*		m_pSlaveList;
	int			m_szPetCount[5];
	BYTE		m_byUnlockCnt;
	UserData*	m_pParent;
	SlaveBaseData* m_pActiveSlave;
	DWORD		m_dwActiveID;
	DWORD		ticktime;
	bool		m_ifActiveChg;
	DWORD		m_dwReviveTick;
	SlaveBaseData* m_pLastActiveSlave;
private:
	bool		_insertSlave(SlaveBaseData* pSlave);
	bool		_updateSlave(SlaveBaseData* pSlave);
	bool		_deleteSlave(SlaveBaseData* pSlave);
	bool		_updateSlaveExp(SlaveBaseData* pSlave);
	bool		_updateSlaveHpMp(SlaveBaseData* pSlave);
	bool		_saveActive();
};