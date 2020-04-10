#pragma once

#include <Windows.h>
#include "xnlist.h"
#include "HorseConfig.h"
#include "NumberData.h"
#include "LuaStateManager.h"
#include "LuaClassObject.h"


class UserData;

class HorseBaseData:public CLuaClassObject<HorseBaseData> 
{
public:
	HorseBaseData(UserData* pUser);
	~HorseBaseData();
	static void			CreateScriptClass(CLuaState * pLuaState);
	static void			ReleaseScriptClass(CLuaState * pLuaState);
	void	RecalParam();
	void	AddExp(int nExp);

	LUAFUN	int		GetLevel();
	LUAFUN	int		GetBaseID();
	LUAFUN	int		GetQuality();

	NumberData* m_pNumData[HORSE_NUM_MAX];
	int			m_nParam[HORSE_PARAM_MAX];
	int			m_szTempCul[HORSE_CULTIVATE_MAX];
	DWORD		m_dwUin;
	DWORD		m_dwID;
	LONGLONG	llKey;
	char*		m_pName;
	int			m_nType;
	BYTE		m_bySlot;
	BYTE		m_byLevel;
	DWORD		m_nExp;
	int         m_nEatCulLimt;
	bool		m_ifSendExp;
	bool		m_ifChange;
	bool		m_ifEquip;
	bool		m_ifSendCul;
	UserData*	m_pMaster;
};


class HorseCenter:public CLuaClassObject<HorseCenter> 
{
public:
	HorseCenter(UserData* pUser);
	~HorseCenter();
	static void			CreateScriptClass(CLuaState * pLuaState);
	static void			ReleaseScriptClass(CLuaState * pLuaState);
//操作
	bool		AddNewHorse(int nModel);
	void		RemoveHorse(DWORD id);
	HorseBaseData* GetHorse(DWORD id);
//发包
	void		SendHorse2C();
	void		SendHorse2C(HorseBaseData* pHorse);
	void		SendExpChange(HorseBaseData* pHorse);
	void		SendEquip();
	void		SendCulChange(HorseBaseData* pHorse);
	void		SendRide();
//管理
	void		Update();
	bool		DBInit();
	void		SaveAll();

	xnList*		m_pHorseList;
	UserData*	m_pParent;
	int			m_szHorseCount[5];
	HorseBaseData* m_pEquipHorse;
	DWORD		m_dwEquipID;
	bool		m_ifEquipChange;
	bool		m_ifRideChange;
	bool		m_ifRide;
private:
	bool		_insertHorse(HorseBaseData* pHorse);
	bool		_updateHorse(HorseBaseData* pHorse);
	bool		_deleteHorse(HorseBaseData* pHorse);
	bool		_updateHorseExp(HorseBaseData* pHorse);
	bool		_saveEquip();
};