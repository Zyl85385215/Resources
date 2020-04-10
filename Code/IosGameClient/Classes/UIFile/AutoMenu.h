#pragma once

#include "CtrlObj.h"

class AutoMenu
{
public:
	AutoMenu();
	~AutoMenu();

	DForm*		m_pFrom;
	DForm*		m_pFromEx;
	DForm*		m_pFromPetEx;
	DForm*		m_pFormActive;

	CCtrlSlide*	m_pSlideHp;
	CCtrlSlide*	m_pSlideMp;
	CCtrlCheck*	m_pCheckAutoTeam;
	CCtrlCheck*	m_pCheckAutoBuy;
	CCtrlCheck*	m_pCheckAutoFix;
	CCtrlCheck*	m_pCheckAutoSkill;
	CCtrlCheck*	m_pCheckAutoRecover;
	CCtrlCheck*	m_pCheckLockPlayer;

	CCtrlCheck*	m_pNoPickUpWhiteEquip;
	CCtrlCheck*	m_pNoPickUpBlueEquip;
	CCtrlCheck*	m_pNoPickUpDrug;
	CCtrlCheck*	m_pNoPickUpMaterial;

	CCtrlSlide*	m_pSlidePetHp;
	CCtrlCheck*	m_pCheckAutoAtk;
	CCtrlCheck*	m_pCheckAutoAtkb;
	CCtrlCheck*	m_pCheckAutoFlw;

	void		Update();
	void		SwitchActive(bool bActive,WORD wX = 0,WORD wY = 0);
	bool		m_bActive;

	int			m_nValSHp;
	int			m_nValSMp;
	bool		m_bAutoTeam;
	bool		m_bAutoBuy;
	bool		m_bAutoFix;
	bool		m_bAutoSkill;
	bool		m_bAutoRecover;
	bool		m_bLockPlayer;

	bool		m_bNoWhite;
	bool		m_bNoBlue;
	bool		m_bNoDrug;
	bool		m_bNoMaterial;

	int			m_nValPetHp;
	bool		m_bAutoAtk;
	bool		m_bAutoAtkb;
	bool		m_bAutoFlw;

	int			m_nHpDrugID;
	int			m_nMpDrugID;
	int			m_nPetHpDrugID;

	DWORD	    s_dwBuyDrugTick;
	DWORD		m_dwFixDrugTick;

	bool		m_bRecharge;
public:
	void		RunAuto();
	void        AutoTeam();
	void        CheckPetState();
	void		LoadPersonalCfg();
	void		CheckSaveCfg();
	bool		m_bifResetPetState;

	int			nMIndex;
	WORD		wStartX;
	WORD		wStartY;

	void        AutoSwitchDrug(int nType,int nID);
private:
	bool		_checkHealth();
	bool		_checkSlave();
	bool		_checkSelObj();
	void		_randWalk();
	void		_loadCfg();
	
	WORD		wSkillPos;
	DWORD		m_dwAutoTeamTick;
};

class ChooseAutoDrug
{
public:
	ChooseAutoDrug();
	~ChooseAutoDrug();
	void OpenDForm(int nType, void (*func)(xnList* pList,void* param),void* param,int nCount);
	void Close();
	void AddRlt(void* pData);
	void ReFresh();

	DForm*		m_pCDForm;
  
	void*	m_pParam;
	xnList*     m_pRltItems;
	xnList*     m_pListItems;

	void (*Func_Return)(xnList* pList,void* param);
};