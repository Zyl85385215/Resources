#pragma once

#include "Interface.h"
#include "CtrlObj.h"
#include "RankData.h"
#include "ios_mapcmd_packet.h"
enum EXP_UPDATE{
	EXP_INIT,
	EXP_ADD,
	EXP_STOP,
};
class ControlMenu
{
public:
	ControlMenu();
	~ControlMenu();

	void		Update();
	void		UpdateSkillCD();
	void        UpdateFastItem();//===zjk
	void		ResetSkill();
	void		StrikeSkill(WORD wSkill);
	bool		CheckHitStrike(int nX, int nY, int nTX, int nTY);

	DForm*		m_pFormTeam;
	DForm*		m_pFormSkill;
	DForm*		m_pFormSelObj;
	DForm*		m_pFormStrike;
	DForm*		m_pFormRevive;

	DComponent*	m_pCompHp;
	DComponent* m_pCompMp;
	DComponent*	m_pCompName;
	DComponent*	m_pCompIco;
	DComponent*	m_pCompAtk;
	DComponent*	m_pCompMCopy;
	DComponent*	m_pCompMail;
	DComponent*	m_pCompLvUp;
	DComponent*	m_pHarvest;

	DComponent* m_pPetIcon;
	DComponent* m_pPetLvl;
	DComponent* m_pPetIconback;
	DComponent* m_pPetLvlback;
	DComponent*	m_pPetHp;
	DComponent* m_pPetHpBack;
	DComponent* m_pPetName;

	DComponent*	m_pCompSelIco;
	DComponent*	m_pCompSelName;
	DComponent*	m_pCompSelHp;
	DComponent*	m_pCompSelLv;
	DComponent*	m_pCompLv;
	DComponent*	m_pCompSkill[MAX_ACTIVE_SKILL];
	DComponent*	m_pCompCD[7];
	DComponent*	m_pCompFuryBar;
	DComponent*	m_pCompState[7];

	DComponent*	m_pCompStrikeIco;
	DComponent*	m_pCompStrikeBtn;
	DWORD		m_dwTickStrike;
	WORD		m_wStrikeSkill;
	BYTE		m_byStrikeStep;
	CCtrlProgress*	m_pCompExp;
	DWORD		m_dwReviveAuto;
	DWORD		m_dwTickMCopyLeave;
private:
	void		_updateFuryBar();
	void		_updateSelObj();
	void		_updateStrike();
	void		_updateState();

	int			m_nCurValFury;
	DWORD		m_dwUpdateTick;
};

class SettingMenu
{
public:
	SettingMenu();
	~SettingMenu();

	void	Update();

	DForm	*	m_pForm;
	
	CCtrlSlide*	m_pSlideSEff;
	CCtrlSlide*	m_pSlideMp3;
	CCtrlCheck*	m_pCheckHideUser;
	CCtrlCheck*	m_pCheckHideTitle;
	CCtrlCheck*	m_pCheckNoTeam;
	CCtrlCheck*	m_pCheckNoSkill;

	int			m_nValSEff;
	int			m_nValSMp3;
	bool		m_bHideUser;
	bool		m_bHideTitle;
	bool		m_bNoTeam;
	bool		m_bNoSkill;
private:
	void		_loadCfg();
};

class SelObjHandleMenu
{
public:
	SelObjHandleMenu();
	~SelObjHandleMenu();
	void		Open();

	DForm*		m_pForm;
};

class SelObjInfoMenu
{
public:
	SelObjInfoMenu();
	~SelObjInfoMenu();
	void		Open(ClientUserInfo* pInfo);
	void		ClearWeapon();

	DForm*		m_pForm;
	DComponent* m_pEquip[10];

	ClientUserInfo* m_pClientInfo;
};

class FishMenu
{
public:
	FishMenu();
	~FishMenu();
	void		Open();
	void		Close();
	void		Update();

	DForm*		m_pForm;
	DComponent*	m_pCompFish;
	DComponent*	m_pCompCircle;
	DComponent*	m_pCompTxt;

	bool         m_bFishBite;
	int          m_nRandSec;
	DWORD        m_dwFishBiteTick;
	DWORD        m_dwWaiteTick;
};